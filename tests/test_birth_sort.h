#pragma once
// ═══════════════════════════════════════════════════════
//  Part 78: BirthTime capture + date sorting
// ═══════════════════════════════════════════════════════

static void runBirthSortTests() {
    std::cout << "========================================\n";
    std::cout << "  Part 78: BirthTime + Sort Tests\n";
    std::cout << "========================================\n\n";

    SearchEngine engine;
    std::vector<FileRecord> records;
    // name, path, type, size, modTime, birthTime, inode, devId
    records.push_back({"b.txt", "/tmp/sort", 1, 100, 3000, 0, 0, 1000});
    records.push_back({"a.txt", "/tmp/sort", 1, 200, 1000, 0, 0, 3000});
    records.push_back({"c.txt", "/tmp/sort", 1, 300, 2000, 0, 0, 0}); // unknown birth
    engine.loadRecords(std::move(records));

    auto names = [&](const std::vector<uint32_t>& idxs) {
        std::string s;
        engine.forEachRecordWithPath(idxs, [&](uint32_t, const FileRecord& r, const std::string&) {
            if (!s.empty()) s += ",";
            s += r.name;
        });
        return s;
    };
    auto births = [&](const std::vector<uint32_t>& idxs) {
        std::string s;
        engine.forEachRecordWithPath(idxs, [&](uint32_t, const FileRecord& r, const std::string&) {
            if (!s.empty()) s += ",";
            s += std::to_string(r.birthTime);
        });
        return s;
    };

    // Default rank order still works
    auto rank = engine.query(".txt", 100, true, 0, "", SearchEngine::SortOrder::Rank);
    check(rank.size() == 3, "Rank finds 3");

    // mtime_desc: b(3000), c(2000), a(1000)
    auto md = engine.query(".txt", 100, true, 0, "", SearchEngine::SortOrder::MtimeDesc);
    check(names(md) == "b.txt,c.txt,a.txt", "mtime_desc orders b,c,a");

    // mtime_asc: a, c, b
    auto ma = engine.query(".txt", 100, true, 0, "", SearchEngine::SortOrder::MtimeAsc);
    check(names(ma) == "a.txt,c.txt,b.txt", "mtime_asc orders a,c,b");

    // birth_desc: a(3000), b(1000), c falls back to modTime 2000 -> a,c,b?
    // c birth 0 -> eff 2000: order a(3000), c(2000), b(1000)
    auto bd = engine.query(".txt", 100, true, 0, "", SearchEngine::SortOrder::BirthDesc);
    check(names(bd) == "a.txt,c.txt,b.txt", "birth_desc orders a,c,b (unknown falls back)");

    // birth_asc: b(1000), c(2000), a(3000)
    auto ba = engine.query(".txt", 100, true, 0, "", SearchEngine::SortOrder::BirthAsc);
    check(names(ba) == "b.txt,c.txt,a.txt", "birth_asc orders b,c,a");

    // name_asc: a, b, c
    auto na = engine.query(".txt", 100, true, 0, "", SearchEngine::SortOrder::NameAsc);
    check(names(na) == "a.txt,b.txt,c.txt", "name_asc orders a,b,c");

    // birthTime exposed with modTime fallback for unknown (c -> 2000)
    auto all = engine.query("c.txt");
    check(births(all) == "2000", "Unknown birthTime falls back to modTime");

    // sort + scope combine
    auto sc = engine.query(".txt", 100, true, 0, "/tmp/sort", SearchEngine::SortOrder::MtimeDesc);
    check(names(sc) == "b.txt,c.txt,a.txt", "Scope + mtime_desc combine");

    // sort + limit truncation keeps top-N
    auto lim = engine.query(".txt", 2, true, 0, "", SearchEngine::SortOrder::MtimeDesc);
    check(names(lim) == "b.txt,c.txt", "mtime_desc limit=2 keeps top-2");

    std::cout << "  All birthtime + sort tests passed!\n\n";
}
