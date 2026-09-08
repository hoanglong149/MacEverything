#pragma once
// ═══════════════════════════════════════════════════════
//  Part 77: Path-scope search + prefix suggest
// ═══════════════════════════════════════════════════════

static void runScopeSuggestTests() {
    std::cout << "========================================\n";
    std::cout << "  Part 77: Scope + Suggest Tests\n";
    std::cout << "========================================\n\n";

    SearchEngine engine;
    std::vector<FileRecord> records;
    records.push_back({"Report.docx", "/tmp/scope/work", 1, 100, 1000});
    records.push_back({"Report-Final.docx", "/tmp/scope/work", 1, 200, 2000});
    records.push_back({"Report.docx", "/tmp/scope/other", 1, 300, 3000});
    records.push_back({"Notes.txt", "/tmp/scope/work", 1, 400, 4000});
    records.push_back({"README.md", "/tmp/scope/work/docs", 1, 500, 5000});
    records.push_back({"MixedCase.PDF", "/tmp/scope/WORK", 1, 600, 6000});
    engine.loadRecords(std::move(records));

    // Unscoped baseline: both Report.docx found (Report-Final.docx lacks the
    // contiguous substring "Report.docx")
    auto all = engine.query("Report.docx");
    check(all.size() == 2,
          "Unscoped 'Report.docx' finds 2");

    // Scope restricts to subtree
    auto scoped = engine.query("Report.docx", 100, true, 0, "/tmp/scope/work");
    check(scoped.size() == 1,
          "Scoped 'Report.docx' in /tmp/scope/work finds 1");
    auto other = engine.query("Report.docx", 100, true, 0, "/tmp/scope/other");
    check(other.size() == 1,
          "Scoped 'Report.docx' in /tmp/scope/other finds 1");

    // Scope is case-insensitive, trailing slash tolerated
    auto ci = engine.query("report", 100, true, 0, "/TMP/SCOPE/WORK/");
    check(ci.size() == 2,
          "Case-insensitive scope with trailing slash finds 2");

    // Scope + limit truncation keeps ranking order
    auto lim = engine.query("Report", 1, true, 0, "/tmp/scope/work");
    check(lim.size() == 1,
          "Scoped limit=1 returns exactly 1");

    // Suggest: prefix completion, most recent first
    QueryTimingInfo timing;
    auto sug = engine.suggest("Rep", 10, "", timing);
    check(sug.size() == 3,
          "Suggest 'Rep' returns 3");
    check(timing.searchPath == "suggest",
          "Suggest timing path is 'suggest'");

    // Suggest respects scope
    auto sugScoped = engine.suggest("Rep", 10, "/tmp/scope/other", timing);
    check(sugScoped.size() == 1,
          "Scoped suggest 'Rep' in /tmp/scope/other returns 1");

    // Suggest is case-insensitive
    auto sugCI = engine.suggest("rep", 10, "", timing);
    check(sugCI.size() == sug.size(),
          "Suggest 'rep' matches 'Rep' count");

    // Suggest empty prefix returns nothing (no full dump)
    auto sugEmpty = engine.suggest("", 10, "", timing);
    // Unicode: NFD-stored path matches NFC scope and vice versa.
    // "caf\xC3\xA9" = NFC é, "cafe\xCC\x81" = NFD e + combining acute.
    {
        SearchEngine ueng;
        std::vector<FileRecord> urec;
        urec.push_back({"f.txt", "/tmp/scope/cafe\xCC\x81", 1, 100, 1000});
        urec.push_back({"g.txt", "/tmp/scope/caf\xC3\xA9", 1, 200, 2000});
        ueng.loadRecords(std::move(urec));
        auto nfcScope = ueng.query("f.txt", 100, true, 0, "/tmp/scope/caf\xC3\xA9");
        check(nfcScope.size() == 1,
              "NFC scope matches NFD-stored path");
        auto nfdScope = ueng.query("g.txt", 100, true, 0, "/tmp/scope/cafe\xCC\x81");
        check(nfdScope.size() == 1,
              "NFD scope matches NFC-stored path");
    }
    check(sugEmpty.size() == 0,
          "Empty prefix suggests nothing");

    // Suggest short prefix (1-2 chars) still works via linear path
    // (Report x3 + README.md)
    auto sugShort = engine.suggest("R", 10, "", timing);
    check(sugShort.size() == 4,
          "Single-char suggest 'R' returns 4");
}
