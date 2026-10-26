// Tier 1 for packet p1-03 (defdbio part): FUN_1401dc3f0 and its helpers.
//
// The header and directory reads (p1-13), the signature check (p1-12) and the Fortran string
// helpers (p1-10) have not landed, so this test defines the five functions of
// src/step/ext_p1_1{0,2,3}.cpp itself (the linker then does not pull those stub objects) and
// feeds the loader chosen header and directory words. What is checked is the loader's own logic:
// return codes, field copies, the version from the text, column count per family/version, the
// unpacking of the directory entries and the stored count. Expected values are worked out from
// the decompile by hand in each case. The -m oracle cases cover the real reads at merge.
// ctest-labels: p1-03 step
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <defdbio/step/db_info_read.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

#include "../src/step/ext.hpp"

using namespace def::model;
using namespace def::dbio;

namespace {

struct Fake {
    std::int32_t is_db = 1;
    std::int32_t header_err = 0;
    std::int32_t dir_err = 0;
    const char* title = "TITLE";
    const char* smi1 = "11.0";
    const char* smi2 = "07/01/14";
    const char* smi3 = "01/02/15";
    const char* smi4 = "03/04/15";
    std::int32_t istart = 29;
    std::int32_t ismtyp = 30;
    std::int32_t nrsets = 0;
    std::int32_t nxtrec = 32;
    std::int32_t family = 2;
    std::int32_t version = 0;
    std::vector<std::int32_t> words; // what the directory read stores, 10 per entry
    // observed
    int header_calls = 0;
    int dir_calls = 0;
    std::int32_t dir_cols = 0;
    std::int32_t dir_version = 0;
    std::string fname;
};
Fake g;

void put(char* dst, const char* src, std::size_t n)
{
    std::memset(dst, ' ', n);
    std::memcpy(dst, src, std::strlen(src) < n ? std::strlen(src) : n);
}

} // namespace

namespace def::dbio::ext {

std::int32_t fun_140323bf0(char* dst, const char* src, std::int32_t n)
{
    put(dst, src, static_cast<std::size_t>(n));
    return n;
}

void fun_1403239f0(char* s, std::int32_t n)
{
    std::int32_t end = n;
    while (end > 0 && (s[end - 1] == ' ' || s[end - 1] == '\0')) {
        --end;
    }
    s[end] = '\0';
}

std::int32_t fun_1402248c0(const char*, std::int32_t* err)
{
    *err = 0;
    return g.is_db;
}

void fun_1403d84b0(char* fname, std::int32_t* len, char* title80, char* smi1, char* smi2,
                   char* smi3, char* smi4, std::int32_t* istart, std::int32_t* ismtyp,
                   std::int32_t* nrsets, std::int32_t* nxtrec, std::int32_t* family,
                   std::int32_t* version, std::int32_t* err, std::int32_t* print)
{
    ++g.header_calls;
    g.fname.assign(fname, static_cast<std::size_t>(*len));
    CHECK_EQ(*print, 0);
    put(title80, g.title, 80);
    put(smi1, g.smi1, 8);
    put(smi2, g.smi2, 8);
    put(smi3, g.smi3, 8);
    put(smi4, g.smi4, 8);
    *istart = g.istart;
    *ismtyp = g.ismtyp;
    *nrsets = g.nrsets;
    *nxtrec = g.nxtrec;
    *family = g.family;
    *version = g.version;
    *err = g.header_err;
}

void fun_1403d96fc(char*, std::int32_t*, std::int32_t* nrsets, std::int32_t* words,
                   std::int32_t* cols, std::int32_t* version, std::int32_t* err, std::int32_t*)
{
    ++g.dir_calls;
    g.dir_cols = *cols;
    g.dir_version = *version;
    for (std::size_t i = 0; i < g.words.size() && i < static_cast<std::size_t>(*nrsets) * 10; ++i) {
        words[i] = g.words[i];
    }
    *err = g.dir_err;
}

} // namespace def::dbio::ext

namespace {

struct TempFile {
    std::filesystem::path p;
    TempFile()
    {
        p = std::filesystem::temp_directory_path() / "p1_03_step_read.DB";
        std::FILE* f = std::fopen(p.string().c_str(), "wb");
        std::fputs("x", f);
        std::fclose(f);
    }
    ~TempFile() { std::filesystem::remove(p); }
    std::string s() const { return p.string(); }
};

// 10-word directory entry
void entry(std::vector<std::int32_t>& w, std::int32_t mesh, std::int32_t step, std::int32_t hs,
           std::int32_t mar, std::int32_t a, std::int32_t sim, std::int32_t mode, std::int32_t dim,
           std::int32_t ver)
{
    w.insert(w.end(), {mesh, step, hs, mar, a, sim, mode, dim, ver, 0});
}

} // namespace

TEST_CASE(file_exists_cases)
{
    TempFile t;
    CHECK(file_exists(t.s().c_str()));
    CHECK(!file_exists(nullptr));
    CHECK(!file_exists(""));
    CHECK(!file_exists((t.s() + ".missing").c_str()));
}

TEST_CASE(missing_file_returns_4_and_resets)
{
    g = Fake{};
    Cls_1401dbfe0 d;
    d.uac8 = 1;
    d.uad8 = new DbDirRecord[1];
    CHECK_EQ(load_db_info(d, "no/such/file.DB", false), 4);
    CHECK(d.uad8 == nullptr); // 1401dc220
    CHECK_EQ(d.uac8, 0);
    CHECK_EQ(g.header_calls, 0);
}

TEST_CASE(not_a_db_returns_1)
{
    TempFile t;
    g = Fake{};
    g.is_db = 0;
    Cls_1401dbfe0 d;
    d.u1900 = 5;
    d.uacc = 3;
    CHECK_EQ(load_db_info(d, t.s().c_str(), false), 1);
    CHECK_EQ(d.u1900, 0); // cleared before the check
    CHECK_EQ(d.uacc, 3);  // not yet
    CHECK_EQ(g.header_calls, 0);
}

TEST_CASE(header_errors_return_2)
{
    TempFile t;
    g = Fake{};
    g.header_err = 99;
    g.nrsets = 3;
    Cls_1401dbfe0 d;
    CHECK_EQ(load_db_info(d, t.s().c_str(), false), 2);
    CHECK_EQ(g.fname.size(), 0x400u); // blank-padded 1024-character name
    g = Fake{};
    g.nrsets = 0; // no step records
    CHECK_EQ(load_db_info(d, t.s().c_str(), false), 2);
    CHECK_EQ(d.uac8, 0);
}

TEST_CASE(header_only_fields)
{
    TempFile t;
    g = Fake{};
    g.nrsets = 4;
    g.smi1 = "11.0";
    Cls_1401dbfe0 d;
    d.u330[5] = std::malloc(3);
    d.u00c[7].a = 9;
    d.u008 = 0;
    CHECK_EQ(load_db_info(d, t.s().c_str(), true), 0);
    CHECK_EQ(g.dir_calls, 0);
    CHECK(std::strcmp(d.u650, t.s().c_str()) == 0);
    CHECK(std::strcmp(d.ua50, "TITLE") == 0);
    CHECK_EQ(d.uabc, 11000); // (int)((double)(11.0f * 1000.0f) + 0.5)
    // The three date fields are 8 bytes each with no room for a terminator: an 8-character
    // date's trim writes its NUL into the next field's first byte, which the next strncpy
    // overwrites, so uaa1 reads as one 24-character run (as in the binary, layout 0xaa1/0xaa9/0xab1).
    CHECK(std::strncmp(d.uaa1, "07/01/14", 8) == 0); // smi2
    CHECK(std::strncmp(d.uaa9, "01/02/15", 8) == 0); // smi3
    CHECK(std::strcmp(d.uab1, "03/04/15") == 0);     // smi4, terminator in 0xab9 (padding)
    CHECK(std::strcmp(d.uaa1, "07/01/1401/02/1503/04/15") == 0);
    CHECK_EQ(d.uac0, 29);
    CHECK_EQ(d.uac4, 30);
    CHECK_EQ(d.uac8, 4);
    CHECK_EQ(d.uad0, 32);
    CHECK(d.u330[5] == nullptr);
    CHECK_EQ(d.u00c[7].a, 0);
    CHECK_EQ(d.u008, 1);
    CHECK(d.uad8 == nullptr);
}

TEST_CASE(version_text_rounding)
{
    TempFile t;
    Cls_1401dbfe0 d;
    // 1.1f = 1.10000002384..; * 1000.0f in float = 1100.0 exactly (rounded); + 0.5 -> 1100
    g = Fake{};
    g.nrsets = 1;
    g.smi1 = "1.1";
    CHECK_EQ(load_db_info(d, t.s().c_str(), true), 0);
    CHECK_EQ(d.uabc, 1100);
    // 2.0005f * 1000.0f = 2000.5 in float (2.0005f = 2.000499963..., product rounds to 2000.5)
    g.smi1 = "2.0005";
    const float f = 2.0005f * 1000.0f;
    CHECK_EQ(load_db_info(d, t.s().c_str(), true), 0);
    CHECK_EQ(d.uabc, static_cast<std::int32_t>(static_cast<double>(f) + 0.5));
    g.smi1 = "abc"; // sscanf fails, vf stays 0
    CHECK_EQ(load_db_info(d, t.s().c_str(), true), 0);
    CHECK_EQ(d.uabc, 0);
}

TEST_CASE(directory_2d_new_version)
{
    TempFile t;
    g = Fake{};
    g.family = 2;
    g.version = 10100; // >= 10000: dim and version per entry from words 8 and 9
    g.nrsets = 3;
    entry(g.words, 1, 0, 100, 0, 0, 1, 0, 2, 10100);
    entry(g.words, 1, 5, 100, 10, 20, 1, 0, 2, 10100);
    entry(g.words, 2, -10, 100, 11, 21, 2, 1, 3, 3500); // a 3D entry in a 2D DB, version 3500
    Cls_1401dbfe0 d;
    CHECK_EQ(load_db_info(d, t.s().c_str(), false), 0);
    CHECK_EQ(g.dir_cols, 10);
    CHECK_EQ(g.dir_version, 10100);
    CHECK_EQ(d.uabc, 10100); // version >= 1 replaces the text version
    const DbDirRecord* r = db_info_records(d);
    REQUIRE(r != nullptr);
    CHECK_EQ(r[0].stored, 0);
    CHECK_EQ(r[1].step, 5);
    CHECK_EQ(r[1].mar_size, 10);
    CHECK_EQ(r[1].a_size, 20);
    CHECK_EQ(r[1].stored, 1);
    CHECK_EQ(r[2].step, -10);
    CHECK_EQ(r[2].dim, 3);
    CHECK_EQ(r[2].version, 3500);
    CHECK_EQ(r[2].sim_no, 2);   // dim 3, version >= 3000: word 6
    CHECK_EQ(r[2].sim_mode, 1);
    CHECK_EQ(d.uacc, 2);
    CHECK_EQ(last_stored_sim_no(d), 2);
}

TEST_CASE(directory_3d_old_version_5_columns)
{
    TempFile t;
    g = Fake{};
    g.family = 3;
    g.version = 0;   // < 1: the text version (2.3 -> 2300) is used, and it is < 3000 -> 5 columns
    g.smi1 = "2.3";
    g.nrsets = 2;
    // The fake directory writes 10 words per entry; the loader reads them with stride 5, like
    // the original: entry 1 starts at word 5 of the buffer.
    entry(g.words, 1, 0, 7, 3, 4, 9, 9, 9, 9);
    entry(g.words, 1, 1, 8, 0, 5, 9, 9, 9, 9);
    Cls_1401dbfe0 d;
    CHECK_EQ(load_db_info(d, t.s().c_str(), false), 0);
    CHECK_EQ(d.uabc, 2300);
    CHECK_EQ(g.dir_cols, 5);
    CHECK_EQ(g.dir_version, 2300);
    const DbDirRecord* r = db_info_records(d);
    REQUIRE(r != nullptr);
    CHECK_EQ(r[0].mesh_no, 1);
    CHECK_EQ(r[0].mar_size, 3);
    CHECK_EQ(r[0].dim, 3);       // the family
    CHECK_EQ(r[0].version, 2300);
    CHECK_EQ(r[0].sim_no, 1);    // 3D before 3000
    CHECK_EQ(r[0].sim_mode, 0);
    CHECK_EQ(r[0].stored, 1);
    // entry 1 = words 5..9 of entry 0's 10-word block: 9 9 9 9 0
    CHECK_EQ(r[1].mesh_no, 9);
    CHECK_EQ(r[1].mar_size, 9);
    CHECK_EQ(r[1].a_size, 0);
    CHECK_EQ(r[1].stored, 1);
    CHECK_EQ(d.uacc, 2);
    CHECK_EQ(last_stored_sim_no(d), 1);
}

TEST_CASE(family_23_and_bad_family)
{
    TempFile t;
    Cls_1401dbfe0 d;
    g = Fake{};
    g.family = 23;
    g.version = 500; // < 10000: dim = family, version = 500 for every entry
    g.nrsets = 1;
    entry(g.words, 1, 0, 1, 0, 0, 4, 2, 0, 0);
    CHECK_EQ(load_db_info(d, t.s().c_str(), false), 0);
    CHECK_EQ(g.dir_cols, 10);
    const DbDirRecord* r = db_info_records(d);
    CHECK_EQ(r[0].dim, 23);
    CHECK_EQ(r[0].version, 500);
    CHECK_EQ(r[0].sim_no, 1); // dim != 2 and version < 3000
    CHECK_EQ(r[0].stored, 0);
    CHECK_EQ(last_stored_sim_no(d), 0);

    g = Fake{};
    g.family = 5;
    g.nrsets = 1;
    CHECK_EQ(load_db_info(d, t.s().c_str(), false), 2);
    CHECK_EQ(g.dir_calls, 0);
}

TEST_CASE(directory_error_returns_3)
{
    TempFile t;
    g = Fake{};
    g.nrsets = 2;
    g.dir_err = 0x65;
    Cls_1401dbfe0 d;
    CHECK_EQ(load_db_info(d, t.s().c_str(), false), 3);
    CHECK(d.uad8 == nullptr);
    CHECK_EQ(d.uac8, 2); // header fields are already stored
}

TEST_CASE(reload_replaces_arrays)
{
    TempFile t;
    g = Fake{};
    g.nrsets = 1;
    entry(g.words, 1, 3, 1, 1, 1, 6, 0, 2, 0);
    Cls_1401dbfe0 d;
    d.uae8 = new DbInfoEntryE0[2];
    CHECK_EQ(load_db_info(d, t.s().c_str(), false), 0);
    CHECK(d.uae8 == nullptr); // deleted at the end of the load
    CHECK_EQ(last_stored_sim_no(d), 6);
    CHECK_EQ(load_db_info(d, t.s().c_str(), false), 0); // old directory array deleted first
    CHECK_EQ(d.uacc, 1);
}

TEST_CASE(step_interval_bounds)
{
    CHECK_EQ(step_interval(-5), 5);
    CHECK_EQ(step_interval(49), 5);
    CHECK_EQ(step_interval(50), 10);
    CHECK_EQ(step_interval(99), 10);
    CHECK_EQ(step_interval(100), 25);
    CHECK_EQ(step_interval(499), 25);
    CHECK_EQ(step_interval(500), 50);
    CHECK_EQ(step_interval(999), 50);
    CHECK_EQ(step_interval(1000), 100);
    CHECK_EQ(step_interval(5000), 100);
}

int main() { return testkit::run_all(); }
