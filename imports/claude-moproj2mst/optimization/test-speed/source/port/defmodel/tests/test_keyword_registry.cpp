// ctest-labels: registry
// Tier-1 tests of the keyword registry (FUN_1402f7680-FUN_1402f7880, FUN_1401b6520's unknown-keyword
// branch). Usage: test_keyword_registry <docs/re/keyword_registry.tsv>
#include "defmodel/keyword_registry.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <set>
#include <sstream>
#include <string>
#include <vector>

using namespace def::model;

// ---- compile-time checks: the lookup is usable in constant expressions -----------------------
static_assert(kKeywordTable[0].id == 0 && kKeywordTable[303].id == 288);  // PROBID first, DIM last
static_assert(keyword_id("PROBID") == 0);
static_assert(keyword_id("FORCE") == 303);
static_assert(keyword_id("probid") == kKeywordNotFound);
static_assert(find_keyword("SRFNRG") == nullptr);
static_assert(keyword_id_at(0) == kKeywordNotFound);
static_assert(keyword_group(288) == static_cast<int>(KeywordGroup::Dimension));

namespace {

int g_failures = 0;
int g_checks = 0;

void check(bool ok, const std::string& what) {
    ++g_checks;
    if (!ok) {
        ++g_failures;
        std::printf("FAIL: %s\n", what.c_str());
    }
}

struct TsvRow {
    int id, group, flag, index;
    std::string name;
    std::uint64_t h1, h2, h3;
};

std::vector<TsvRow> read_tsv(const char* path) {
    std::vector<TsvRow> rows;
    std::ifstream in(path);
    std::string line;
    std::getline(in, line);  // header
    while (std::getline(in, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        if (line.empty()) continue;
        std::vector<std::string> f;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, '\t')) f.push_back(cell);
        if (f.size() != 9) {
            check(false, "bad TSV line: " + line);
            continue;
        }
        rows.push_back({std::stoi(f[0]), std::stoi(f[2]), std::stoi(f[4]), std::stoi(f[8]), f[1],
                        std::stoull(f[5], nullptr, 16), std::stoull(f[6], nullptr, 16),
                        std::stoull(f[7], nullptr, 16)});
    }
    return rows;
}

// Every keyword in the TSV (written by the generator from the binary) resolves through the same
// paths the binary uses to the same id, group, flag and handler slots.
void test_against_tsv(const char* path) {
    std::vector<TsvRow> rows = read_tsv(path);
    check(rows.size() == kKeywordCount, "TSV has 304 rows, got " + std::to_string(rows.size()));
    std::set<int> ids;
    for (const TsvRow& r : rows) {
        const std::string tag = r.name + " (id " + std::to_string(r.id) + ")";
        ids.insert(r.id);
        int group = -1;
        check(lookup_keyword(r.name.c_str(), &group) == r.id, tag + ": lookup id");
        check(group == r.group, tag + ": lookup group");
        check(keyword_id(r.name.c_str()) == r.id, tag + ": keyword_id");
        check(keyword_group(r.id) == r.group, tag + ": keyword_group");
        check(keyword_flag(r.id) == r.flag, tag + ": keyword_flag");
        check(keyword_handler1(r.id) == r.h1, tag + ": handler1");
        check(std::strcmp(keyword_name(r.id), r.name.c_str()) == 0, tag + ": keyword_name");
        const KeywordRecord* rec = find_keyword(r.id);
        check(rec != nullptr && rec == &kKeywordTable[r.index], tag + ": table position");
        check(rec != nullptr && rec->handler2 == r.h2 && rec->handler3 == r.h3, tag + ": handlers 2/3");
        check(find_keyword(r.name.c_str()) == rec, tag + ": name and id find the same record");
        check(keyword_id_at(r.index) == (r.index == 0 ? kKeywordNotFound : r.id), tag + ": keyword_id_at");
    }
    check(ids.size() == kKeywordCount, "TSV ids are unique");
}

void test_table_shape() {
    std::set<std::string> names;
    int defaults2 = 0, defaults3 = 0;
    for (const KeywordRecord& r : kKeywordTable) {
        names.insert(r.name);
        check(r.handler1 == kDefaultHandler1, std::string(r.name) + ": handler1 is the default");
        defaults2 += r.handler2 == kDefaultHandler2;
        defaults3 += r.handler3 == kDefaultHandler3;
    }
    check(names.size() == kKeywordCount, "names are unique");
    // 155 distinct write handlers and 49 label handlers besides the defaults. (Plan §3 says 50: the
    // earlier extract read ZSTR's empty slot as 0x100000000.)
    std::set<std::uint64_t> h2, h3;
    for (const KeywordRecord& r : kKeywordTable) {
        if (r.handler2 != kDefaultHandler2) h2.insert(r.handler2);
        if (r.handler3 != kDefaultHandler3 && r.handler3 != 0) h3.insert(r.handler3);
    }
    check(h2.size() == 155, "155 KEY write handlers, got " + std::to_string(h2.size()));
    check(h3.size() == 49, "49 label handlers, got " + std::to_string(h3.size()));
    check(find_keyword("ZSTR") != nullptr && find_keyword("ZSTR")->handler3 == 0, "ZSTR has no label handler");
    // Table order is not id order: position 21 holds CRDSYS (id 282).
    check(kKeywordTable[21].id == 282 && std::strcmp(kKeywordTable[21].name, "CRDSYS") == 0, "table order");
}

void test_unknown_and_edge_cases() {
    int group = 99;
    check(lookup_keyword("SRFNRG", &group) == kKeywordNotFound && group == 0, "SRFNRG is not in the table");
    group = 99;
    check(lookup_keyword("", &group) == kKeywordNotFound && group == 0, "empty name");
    check(keyword_id("probid") == kKeywordNotFound, "lookup is case-sensitive");
    check(keyword_id("Probid") == kKeywordNotFound, "lookup is case-sensitive (mixed)");
    check(keyword_id("PROBID ") == kKeywordNotFound, "no trimming");
    check(keyword_id(" PROBID") == kKeywordNotFound, "no trimming (leading)");
    check(keyword_id("PROBI") == kKeywordNotFound, "no prefix match");
    check(keyword_id("PROBIDX") == kKeywordNotFound, "no prefix match (longer)");
    check(keyword_id("DSMAX_NSTEP") != kKeywordNotFound, "name with underscore resolves");
    check(keyword_id("DIM") == 288, "shortest name");
    // A 39-character token, the longest fscanf("%39s") produces.
    check(keyword_id("ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLM") == kKeywordNotFound, "39-char token");

    // Id accessors outside the table.
    check(find_keyword(-1) == nullptr && find_keyword(304) == nullptr && find_keyword(kKeywordNotFound) == nullptr,
          "find_keyword(id) outside 0..303");
    check(keyword_flag(kKeywordNotFound) == 0 && keyword_group(kKeywordNotFound) == 0 &&
              keyword_handler1(kKeywordNotFound) == 0,
          "id accessors return 0 for unknown ids");
    check(keyword_id_at(0) == kKeywordNotFound, "keyword_id_at rejects position 0");
    check(keyword_id_at(-1) == kKeywordNotFound && keyword_id_at(304) == kKeywordNotFound, "keyword_id_at bounds");
    check(keyword_id_at(303) == kKeywordTable[303].id, "keyword_id_at(303)");
    check(keyword_id_at(21) == 282, "keyword_id_at indexes by position, not id");

    // keyword_name: error text, and one shared static buffer.
    check(std::strcmp(keyword_name(kKeywordNotFound), "ERROR: KEYWORD NOT EXISTS!") == 0, "unknown id name");
    const char* a = keyword_name(0);
    const char* b = keyword_name(303);
    check(a == b && std::strcmp(a, "FORCE") == 0, "keyword_name reuses one buffer");

    // Unknown-keyword policy of the two readers.
    check(unknown_keyword_is_silent("SRFNRG"), "SRFNRG is skipped silently");
    check(!unknown_keyword_is_silent("srfnrg") && !unknown_keyword_is_silent("SRFNRGX") &&
              !unknown_keyword_is_silent("FOO") && !unknown_keyword_is_silent("1ABC"),
          "only exactly SRFNRG is silent in FUN_1401b6520");
    check(unknown_keyword_is_silent_1403987e0("1ABC") && unknown_keyword_is_silent_1403987e0("2"),
          "FUN_1403987e0 is silent for names starting with 1 or 2");
    check(!unknown_keyword_is_silent_1403987e0("SRFNRG") && !unknown_keyword_is_silent_1403987e0("3ABC"),
          "FUN_1403987e0 has no SRFNRG exception");

    check(unknown_keyword_warning("FOO", " 1 2 3\n") == "WARNING: Keyword FOO not found. Skip  1 2 3\n\n",
          "warning text");
    char buf[256] = {};
    std::FILE* f = std::tmpfile();
    if (f != nullptr) {
        print_unknown_keyword_warning(f, "FOO", " 1 2 3\n");
        std::rewind(f);
        size_t n = std::fread(buf, 1, sizeof buf - 1, f);
        std::fclose(f);
        check(std::string(buf, n) == unknown_keyword_warning("FOO", " 1 2 3\n"), "printed warning bytes");
    }
}

}  // namespace

int main(int argc, char** argv) {
    // Default: the repository's table (ctest sets DEF_PORT_REPO, see port/cmake/DefPort.cmake).
    std::string tsv;
    if (argc >= 2) {
        tsv = argv[1];
    } else if (const char* repo = std::getenv("DEF_PORT_REPO")) {
        tsv = std::string(repo) + "/docs/re/keyword_registry.tsv";
    } else {
        std::printf("usage: test_keyword_registry <keyword_registry.tsv>\n");
        return 2;
    }
    test_against_tsv(tsv.c_str());
    test_table_shape();
    test_unknown_and_edge_cases();
    std::printf("%d checks, %d failures\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
