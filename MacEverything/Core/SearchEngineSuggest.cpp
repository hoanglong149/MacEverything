// Prefix completions for typeahead suggest, backed by the name trigram index.
#include "SearchEngine.h"
#include "StringUtils.h"
#include <algorithm>
#include <chrono>
#include <cstring>
#include <shared_mutex>
#include <utility>
#include <vector>

std::vector<uint32_t> SearchEngine::suggest(const std::string& prefix, uint32_t maxResults,
                                            const std::string& scope,
                                            QueryTimingInfo& timing) const {
    auto t0 = std::chrono::steady_clock::now();
    timing = QueryTimingInfo{};
    if (maxResults == 0) maxResults = 10;

    std::string lower = me::toLower(prefix);
    if (lower.empty()) return {};

    // Scope: index bytes may be NFC or NFD (macOS stores either); accept both.
    auto slashTerm = [](std::string s) {
        while (!s.empty() && s.back() == '/') s.pop_back();
        if (!s.empty()) s.push_back('/');
        return s;
    };
    const std::string nfc = slashTerm(me::normalizeNFC(me::toLower(scope)));
    const std::string nfd = slashTerm(me::normalizeNFD(me::toLower(scope)));

    std::shared_lock lock(mutex_);
    timing.totalRecords = types_.size();

    std::vector<uint32_t> cands;
    bool useIndex = false;
    if (lower.size() >= 3) {
        bool allFound = false;
        cands = intersectPostingLists(nameTrigramIndex_, lower, allFound);
        useIndex = allFound && !cands.empty() && cands.size() <= types_.size() / 10;
    }

    std::vector<std::pair<int64_t, uint32_t>> scored;
    scored.reserve(1024);
    auto consider = [&](uint32_t idx) {
        if (idx >= types_.size() || types_[idx] == 0) return;
        const char* nm = namePool_.data(idx);
        size_t nl = namePool_.length(idx);
        if (nl < lower.size() || std::memcmp(nm, lower.data(), lower.size()) != 0) return;
        if (!nfc.empty()) {
            if (idx >= pathIndices_.size()) return;
            std::string full = lowerPathPool_.str(pathIndices_[idx]);
            full.push_back('/');
            full.append(nm, nl);
            if (full.compare(0, nfc.size(), nfc) != 0 &&
                full.compare(0, nfd.size(), nfd) != 0) return;
        }
        scored.emplace_back(modTimes_[idx], idx);
    };

    if (useIndex) {
        for (uint32_t idx : cands) consider(idx);
    } else {
        // Linear scan, bounded so a 1-char prefix can't stall writers for long.
        for (uint32_t i = 0; i < types_.size() && scored.size() < 20000; ++i) consider(i);
    }

    size_t n = std::min<size_t>(scored.size(), maxResults);
    std::partial_sort(scored.begin(), scored.begin() + n, scored.end(),
                      [](const auto& a, const auto& b) { return a.first > b.first; });
    std::vector<uint32_t> out;
    out.reserve(n);
    for (size_t i = 0; i < n; ++i) out.push_back(scored[i].second);

    timing.totalMs =
        std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
    timing.resultCount = out.size();
    timing.searchPath = "suggest";
    return out;
}
