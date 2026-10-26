// Tier-1 tests of the comparator on synthetic inputs (no DEFORM files needed):
// digests, KEY line/keyword reports, transcript masking, DB masks and planted
// differences in known regions, and oracle-entry comparison.
#include <cstdio>
#include <cstring>
#include <fstream>
#include <random>
#include <string>

#include "deformdb/database.hpp"
#include "deformdb/writer.hpp"
#include "defcmp/defcmp.hpp"

using namespace defcmp;
namespace ddb = deformdb;

static int failures = 0;
#define CHECK(c)                                                                         \
    do {                                                                                 \
        if (!(c)) { std::printf("FAIL %s:%d %s\n", __FILE__, __LINE__, #c); ++failures; } \
    } while (0)
#define CHECK_HAS(s, sub)                                                                                  \
    do {                                                                                                   \
        if ((s).find(sub) == std::string::npos) {                                                          \
            std::printf("FAIL %s:%d \"%s\" does not contain \"%s\"\n", __FILE__, __LINE__, (s).c_str(), sub); \
            ++failures;                                                                                    \
        }                                                                                                  \
    } while (0)

static const fs::path kDir = fs::temp_directory_path() / "defcmp_test";

static void write_text(const fs::path& p, std::string_view s) {
    fs::create_directories(p.parent_path());
    std::ofstream f(p, std::ios::binary);
    f.write(s.data(), static_cast<std::streamsize>(s.size()));
}

static void patch(const fs::path& p, std::uint64_t off, const void* bytes, std::size_t n) {
    std::fstream f(p, std::ios::binary | std::ios::in | std::ios::out);
    f.seekp(static_cast<std::streamoff>(off));
    f.write(static_cast<const char*>(bytes), static_cast<std::streamsize>(n));
}

// ---------------------------------------------------------------- digest

static void test_digest() {
    CHECK(hash_bytes("") == 0xEF46DB3751D8E999ULL);
    CHECK(hash_bytes("abc") == 0x44BC2CF5AD770999ULL);
    std::string big(100003, '\0');
    std::mt19937 rng(1);
    for (auto& c : big) c = static_cast<char>(rng());
    Hasher h;
    for (std::size_t i = 0; i < big.size();) {   // uneven chunks
        const std::size_t n = std::min<std::size_t>(1 + rng() % 97, big.size() - i);
        h.update(big.data() + i, n);
        i += n;
    }
    CHECK(h.finish() == hash_bytes(big));
    CHECK(parse_digest(digest_hex(0x0123456789abcdefULL)) == 0x0123456789abcdefULL);
    CHECK(!parse_digest("xyz"));
}

// ---------------------------------------------------------------- KEY

static const char* kKey =
    "*\n"
    "*  DEFORM-3D V11.0 KEYWORD FILE (Qt)\n"
    "*\n"
    "TITLE \n"
    "SIMULATION\n"
    "OPRNAM\n"
    "CogABC\n"
    "NSTART      25\n"
    "OBJNAM       1\n"
    "Top\n"
    "RZ           1     3\n"
    "    1.0000000000E+000    2.0000000000E+000    3.0000000000E+000\n"
    "    4.0000000000E+000    5.0000000000E+000    6.0000000000E+000\n"
    "    7.0000000000E+000    8.0000000000E+000    9.0000000000E+000\n"
    "DENSTY       1     2\n"
    "    1.0000000000E+000\n"
    "    1.0000000000E+000\n";

static std::string replace_once(std::string s, std::string_view from, std::string_view to) {
    const auto p = s.find(from);
    if (p != std::string::npos) s.replace(p, from.size(), to);
    return s;
}

static void test_key() {
    const std::string k = kKey;
    CHECK(is_registry_keyword("RZ") && is_registry_keyword("OBJNAM") && !is_registry_keyword("Top"));
    {
        auto r = compare_key_text(k, k);
        CHECK(r.equal && r.by_digest);
    }
    {   // changed value in a data line of RZ: line 13, keyword RZ
        auto r = compare_key_text(k, replace_once(k, "5.0000000000E+000", "5.0000000001E+000"));
        CHECK(!r.equal);
        CHECK_HAS(r.where, "line 13 col ");
        CHECK_HAS(r.where, "keyword RZ (line 11");
        CHECK_HAS(r.expected, "5.0000000000E+000");
        CHECK_HAS(r.actual, "5.0000000001E+000");
    }
    {   // changed keyword line
        auto r = compare_key_text(k, replace_once(k, "NSTART      25", "NSTART      26"));
        CHECK(!r.equal);
        CHECK_HAS(r.where, "line 8 col 14, keyword NSTART");
    }
    {   // a text value that looks like a word is not a keyword: block is OBJNAM
        auto r = compare_key_text(k, replace_once(k, "Top\n", "Bot\n"));
        CHECK_HAS(r.where, "line 10 col 1, keyword OBJNAM (line 9");
    }
    {   // text before any keyword
        auto r = compare_key_text(k, replace_once(k, "(Qt)", "(QT)"));
        CHECK_HAS(r.where, "line 2");
        CHECK_HAS(r.where, "before the first keyword");
    }
    {   // missing last line
        std::string shorter = k.substr(0, k.size() - std::string("    1.0000000000E+000\n").size());
        auto r = compare_key_text(k, shorter);
        CHECK(!r.equal);
        CHECK_HAS(r.where, "line 17");
        CHECK_HAS(r.where, "keyword DENSTY");
        CHECK(r.actual == "<end of file>");
    }
    {   // CRLF vs LF is a byte difference
        auto r = compare_key_text(k, replace_once(k, "CogABC\n", "CogABC\r\n"));
        CHECK(!r.equal);
        CHECK_HAS(r.where, "line 7");
        CHECK_HAS(r.where, "line ending");
    }
    {   // files, and a cached expected digest with the expected file gone
        write_text(kDir / "a.KEY", k);
        write_text(kDir / "b.KEY", k);
        auto r = compare_key(kDir / "a.KEY", kDir / "b.KEY");
        CHECK(r.equal && r.by_digest);
        const Digest d = key_digest(kDir / "a.KEY");
        r = compare_key(kDir / "missing.KEY", kDir / "b.KEY", d);
        CHECK(r.equal && r.by_digest);
        write_text(kDir / "c.KEY", replace_once(k, "DENSTY", "DENSTX"));
        r = compare_key(kDir / "missing.KEY", kDir / "c.KEY", d);
        CHECK(!r.equal && r.error.empty());
        CHECK_HAS(r.where, "expected file not available");
        r = compare_key(kDir / "a.KEY", kDir / "c.KEY", d);
        CHECK_HAS(r.where, "line 15 col 6");
    }
}

// ---------------------------------------------------------------- transcript

static void test_transcript() {
    TranscriptMasks m;
    m.roots = {{"C:\\scratch\\case1", "<CASE>"}};
    const std::string e =
        " DEF_PRE started Wed Oct 21 09:31:50 2026\n"
        " Date: 10/21/26  Time: 09:31:50\n"
        " Reading C:\\scratch\\case1\\COG.DB\n"
        " Install dir C:\\Program Files\\SFTC\\DEFORM\\v11.0.2\\DEF_PRE_64.exe\n"
        " Release 11.0.2 built 2014-09-29\n"
        " Step 25 written\n";
    const std::string a =
        " DEF_PRE started Thu Oct 22 17:02:03 2026\n"
        " Date: 10/22/26  Time: 17:02:03\n"
        " Reading c:\\SCRATCH\\Case1\\COG.DB\n"
        " Install dir D:\\DEFORM\\DEF_PRE_64.exe\n"
        " Release 11.0.2 built 2015-01-02\n"
        " Step 25 written\n";
    auto r = compare_transcript_text(e, a, m);
    CHECK(r.equal);
    CHECK(normalize_transcript(" Reading C:\\scratch\\case1\\COG.DB", m) == " Reading <CASE>\\COG.DB");
    CHECK(normalize_transcript(" Release 11.0.2", m) == " Release 11.0.2");   // versions are not dates
    CHECK(normalize_transcript("at 9:05 PM on 21-Oct-2026", m) == "at <TIME> on <DATE>");
    CHECK(normalize_transcript("C:\\Program Files\\SFTC\\a.exe started", m) == "<PATH> started");

    // a different file under the root is still caught
    r = compare_transcript_text(e, replace_once(a, "COG.DB", "COH.DB"), m);
    CHECK(!r.equal);
    CHECK_HAS(r.where, "line 3 col 19");
    // a real message difference
    r = compare_transcript_text(e, replace_once(a, "Step 25", "Step 26"), m);
    CHECK_HAS(r.where, "line 6 col 8");
    // dates kept when asked
    TranscriptMasks keep = m;
    keep.dates = false;
    r = compare_transcript_text(e, a, keep);
    CHECK(!r.equal);
    CHECK_HAS(r.where, "line 1");
    // extra mask
    TranscriptMasks x = m;
    x.extra.emplace_back("CPU time = [0-9.]+");
    CHECK(compare_transcript_text(e + "CPU time = 0.11\n", a + "CPU time = 0.27\n", x).equal);
    CHECK(!compare_transcript_text(e + "CPU time = 0.11\n", a + "CPU time = 0.27\n", m).equal);
    // extra line
    r = compare_transcript_text(e, a + " done\n", m);
    CHECK_HAS(r.where, "line 7 (line count: expected 6, actual 7)");
}

// ---------------------------------------------------------------- DB

static std::mt19937 rng(11);

// 3D v11000 step whose MAR holds real IIAD1/IIAD2 address tables, so the
// comparator can name regions and keywords: MAR regions IIAD1, IIAD2, IIFNC;
// A regions 1..k where only region k = LLCRZ (21 nodes x 3) has data. 63 words
// with one run of 4 store in exactly one record; breaking the run needs two.
static ddb::StepData make_step(int step, double tnow) {
    ddb::StepData s;
    s.entry.mesh_no = 1;
    s.entry.step = step;
    s.entry.sim_no = 1;
    s.entry.sim_mode = 2;
    s.entry.dim = 3;
    s.entry.version = 11000;
    s.header.assign(500, 0.0);
    s.header[0] = 1;      // objects
    s.header[2] = tnow;
    s.header[3] = tnow;
    s.header[9] = 400;    // S
    s.header[12] = 100;   // F

    const auto a_names = ddb::a_region_names(3, 11000);
    int k = 0;
    while (k < static_cast<int>(a_names.size()) && std::strcmp(a_names[static_cast<std::size_t>(k)], "LLCRZ") != 0) ++k;
    const int crz = k + 1;   // 1-based A slot of LLCRZ

    s.mar.assign(430, 0);
    for (int j = 0; j < 70; ++j) s.mar[static_cast<std::size_t>(j)] = j == 0 ? 1 : (j == 1 ? 141 : (j == 2 ? 421 : 431));
    for (int j = 0; j < 140; ++j) s.mar[static_cast<std::size_t>(140 + j)] = j < crz ? 1 : 64;
    s.mar[static_cast<std::size_t>(140 + 140 + crz - 1)] = 3;   // width of LLCRZ
    for (int j = 0; j < 10; ++j) s.mar[static_cast<std::size_t>(420 + j)] = 1000 + 7 * j;   // IIFNC, no runs
    s.a.resize(63);
    for (int j = 0; j < 63; ++j) s.a[static_cast<std::size_t>(j)] = 0.5 + j * 1.25 + (rng() % 1000) * 1e-6;
    s.a[20] = s.a[21] = s.a[22] = s.a[23] = 7.0;   // one run of 4

    s.mar_table = {3, 0, 140, 1, 0, 0, 280, 141, 0, 0, 10, 421, 0, 0};
    s.a_table.assign(static_cast<std::size_t>(4 * crz + 2), 0);
    s.a_table[0] = crz;
    for (int r = 1; r <= crz; ++r) {
        s.a_table[static_cast<std::size_t>(4 * r - 2)] = r == crz ? 63 : 0;   // length (word 4r-1)
        s.a_table[static_cast<std::size_t>(4 * r - 1)] = 1;                   // start (word 4r)
    }
    return s;
}

static fs::path write_db(const fs::path& p, const std::vector<std::pair<ddb::StepData, bool>>& steps, const std::string& today) {
    ddb::WriterHeader h;
    h.title = "DEFCMP TEST";
    h.release_date = "06/30/14";
    h.today = today;
    auto w = ddb::Writer::create(p, h);
    for (const auto& [s, header_only] : steps) w->append_step(s, header_only);
    w->flush();
    return p;
}

static void test_db() {
    const auto s1 = make_step(-1, 0.0), s2 = make_step(5, 1.5), s3 = make_step(6, 2.0);
    auto steps = std::vector<std::pair<ddb::StepData, bool>>{{s1, false}, {s2, true}, {s3, false}};
    const auto base = write_db(kDir / "base.DB", steps, "10/21/26");

    {   // identical copy
        fs::copy_file(base, kDir / "copy.DB", fs::copy_options::overwrite_existing);
        auto r = compare_db(base, kDir / "copy.DB", DbMasks{});
        CHECK(r.equal && r.by_digest);
        r = compare_db(kDir / "gone.DB", kDir / "copy.DB", DbMasks{}, db_digest(base, DbMasks{}));
        CHECK(r.equal && r.by_digest);
    }
    {   // written on another day: SMI3/SMI4 differ, masked by default
        const auto other = write_db(kDir / "other_day.DB", steps, "11/02/26");
        auto r = compare_db(base, other, DbMasks{});
        CHECK(r.equal);
        r = compare_db(base, other, DbMasks::none());
        CHECK(!r.equal);
        CHECK_HAS(r.where, "record 2 SMI3 (first written)");
        CHECK_HAS(r.expected, "10/21/26");
        CHECK_HAS(r.actual, "11/02/26");
        r = compare_db(base, other, *DbMasks::parse("none,+smi3"));
        CHECK_HAS(r.where, "record 2 SMI4 (last written)");
    }
    {   // ISTART
        fs::copy_file(base, kDir / "istart.DB", fs::copy_options::overwrite_existing);
        const unsigned char v[4] = {0, 0, 0, 7};
        patch(kDir / "istart.DB", 512 + 112, v, 4);
        CHECK(compare_db(base, kDir / "istart.DB", DbMasks{}).equal);
        auto r = compare_db(base, kDir / "istart.DB", *DbMasks::parse("-istart"));
        CHECK(!r.equal);
        CHECK_HAS(r.where, "record 2 ISTART");
    }
    {   // unmasked header field
        fs::copy_file(base, kDir / "title.DB", fs::copy_options::overwrite_existing);
        patch(kDir / "title.DB", 512 + 3, "X", 1);
        auto r = compare_db(base, kDir / "title.DB", *DbMasks::parse("all"));
        CHECK_HAS(r.where, "record 2 TITLE");
    }
    {   // planted value in a known A region (same layout): LLCRZ word 5
        auto t = s3;
        t.a[4] += 0.25;
        const auto p = write_db(kDir / "value_a.DB", {{s1, false}, {s2, true}, {t, false}}, "10/21/26");
        CHECK(fs::file_size(p) == fs::file_size(base));
        auto r = compare_db(base, p, DbMasks{});
        CHECK(!r.equal);
        CHECK_HAS(r.where, "step 6 (slot 3), A[5], region LLCRZ word 5, keyword RZ, row 2 comp 2");
    }
    {   // planted value in a MAR region
        auto t = s1;
        t.mar[423] = 4242;
        const auto p = write_db(kDir / "value_mar.DB", {{t, false}, {s2, true}, {s3, false}}, "10/21/26");
        auto r = compare_db(base, p, DbMasks{});
        CHECK_HAS(r.where, "step -1 (slot 1), MAR[424], region IIFNC word 4");
        CHECK(r.expected == "1021" && r.actual == "4242");
    }
    {   // a change that breaks an RLE run shifts the records of later data; the
        // report must still name the value, not the moved record numbers
        auto t = s1;
        t.a[21] = 7.5;
        const auto p = write_db(kDir / "layout.DB", {{t, false}, {s2, true}, {s3, false}}, "10/21/26");
        CHECK(fs::file_size(p) != fs::file_size(base));
        auto r = compare_db(base, p, DbMasks{});
        CHECK_HAS(r.where, "step -1 (slot 1), A[22], region LLCRZ word 22, keyword RZ, row 8 comp 1");
    }
    {   // header word
        auto t = s3;
        t.header[2] = 2.5;
        const auto p = write_db(kDir / "header.DB", {{s1, false}, {s2, true}, {t, false}}, "10/21/26");
        auto r = compare_db(base, p, DbMasks{});
        CHECK_HAS(r.where, "step 6 (slot 3), step header H3 (start time)");
    }
    {   // header-only fields: H7/H8 and directory word 13 of slot 2
        const auto ents = ddb::Database::open(base)->entries();
        const auto& ho = ents[1];
        CHECK(ho.mar_size == 0 && ho.a_size == 0);
        fs::copy_file(base, kDir / "ho.DB", fs::copy_options::overwrite_existing);
        const unsigned char h7[8] = {0x41, 0x01, 0x64, 0x48, 0, 0, 0, 0};   // 142473.0 big-endian
        patch(kDir / "ho.DB", (static_cast<std::uint64_t>(ho.rec_header) - 1) * 512 + 6 * 8, h7, 8);
        auto r = compare_db(base, kDir / "ho.DB", DbMasks{});
        CHECK(!r.equal);
        CHECK_HAS(r.where, "step 5 (slot 2), step header H7 (MAR length)");
        CHECK(compare_db(base, kDir / "ho.DB", *DbMasks::parse("+ho-h7h8")).equal);

        // directory word 13 of the header-only entry; its file offset from the mask list
        const auto ranges = db_mask_ranges(base, *DbMasks::parse("none,+ho-dir"));
        CHECK(ranges.size() == 2);
        if (ranges.size() == 2) {
            fs::copy_file(base, kDir / "hodir.DB", fs::copy_options::overwrite_existing);
            const unsigned char rec[4] = {0, 0, 0x7f, 0x7f};
            patch(kDir / "hodir.DB", ranges[1].offset, rec, 4);
            r = compare_db(base, kDir / "hodir.DB", DbMasks{});
            CHECK_HAS(r.where, "directory word 13 A record");
            CHECK(compare_db(base, kDir / "hodir.DB", *DbMasks::parse("+header-only")).equal);
        }
    }
    {   // truncated file
        fs::copy_file(base, kDir / "short.DB", fs::copy_options::overwrite_existing);
        fs::resize_file(kDir / "short.DB", fs::file_size(base) - 512);
        auto r = compare_db(base, kDir / "short.DB", DbMasks{});
        CHECK(!r.equal);
        CHECK_HAS(r.where, "sizes: expected");
    }
    {   // mask spec parsing
        CHECK(DbMasks{}.canonical() == "smi3,smi4,istart");
        CHECK(DbMasks::parse("all,-smi2")->canonical() == "smi3,smi4,istart,ho-dir,ho-h7h8");
        CHECK(DbMasks::parse("none")->canonical() == "none");
        CHECK(DbMasks::parse("dates,+bytes:1024:8")->canonical() == "smi3,smi4,bytes:1024:8");
        CHECK(DbMasks::parse("+header-only")->canonical() == "smi3,smi4,istart,ho-dir,ho-h7h8");
        std::string err;
        CHECK(!DbMasks::parse("+bogus", &err) && err.find("bogus") != std::string::npos);
    }
}

// ---------------------------------------------------------------- oracle entry

static void test_oracle_entry() {
    const auto e = kDir / "entry", a = kDir / "port";
    fs::remove_all(e);
    fs::remove_all(a);
    write_text(e / "result.json",
               "{\"schema\": 1, \"exit_code\": 0, \"files\": {"
               "\"NEWEQ.MSG\": {\"status\": \"created\", \"size\": 0, \"stored\": true},"
               "\"STEP_25.KEY\": {\"status\": \"created\", \"size\": 12, \"stored\": true},"
               "\"BIG.DB\": {\"status\": \"created\", \"size\": 3000000000, \"stored\": false},"
               "\"OLD.KEY\": {\"status\": \"deleted\", \"size\": 3, \"stored\": false}}}");
    write_text(e / "stdout.txt", " Run on 10/21/26 09:31\n Done\n");
    write_text(e / "stderr.txt", "");
    write_text(e / "files/NEWEQ.MSG", "");
    write_text(e / "files/STEP_25.KEY", "NSTART  25\n");

    write_text(a / "exit_code", "0\n");
    write_text(a / "stdout.txt", " Run on 11/01/26 17:02\n Done\n");
    write_text(a / "stderr.txt", "");
    write_text(a / "files/NEWEQ.MSG", "");
    write_text(a / "files/STEP_25.KEY", "NSTART  25\n");

    auto count = [](const std::vector<DirEntryResult>& rows, bool (*pred)(const DirEntryResult&)) {
        int n = 0;
        for (const auto& r : rows) n += pred(r) ? 1 : 0;
        return n;
    };
    auto rows = compare_oracle_entry(e, a, Options{});
    CHECK(rows.size() == 7);   // exit code, stdout, stderr, BIG.DB, OLD.KEY, NEWEQ.MSG, STEP_25.KEY
    CHECK(count(rows, [](const DirEntryResult& r) { return !r.ok() && !r.skipped; }) == 0);
    CHECK(count(rows, [](const DirEntryResult& r) { return r.skipped; }) == 1);

    // the port forgot NEWEQ.MSG, kept OLD.KEY and exited with 1
    fs::remove(a / "files/NEWEQ.MSG");
    write_text(a / "files/OLD.KEY", "x");
    write_text(a / "exit_code", "1");
    rows = compare_oracle_entry(e, a, Options{});
    int bad = 0;
    for (const auto& r : rows) {
        if (r.ok() || r.skipped) continue;
        ++bad;
        if (r.name == "exit code") CHECK(r.result.expected == "0" && r.result.actual == "1");
        else if (r.name == "NEWEQ.MSG") CHECK(r.missing_actual);
        else if (r.name == "OLD.KEY") CHECK_HAS(r.result.where, "deleted");
        else std::printf("unexpected row %s\n", r.name.c_str()), ++failures;
    }
    CHECK(bad == 3);
}

int main() {
    fs::remove_all(kDir);
    fs::create_directories(kDir);
    test_digest();
    test_key();
    test_transcript();
    test_db();
    test_oracle_entry();
    if (failures == 0) {
        std::printf("all defcmp tests passed\n");
        fs::remove_all(kDir);
    }
    return failures == 0 ? 0 : 1;
}
