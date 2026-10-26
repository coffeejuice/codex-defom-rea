// Tier-1 tests of the kw-g4a keyword readers and writers (FRAE2H, FPERV, FRCMOD, FSTRES, CREEP,
// ANISO, BURGRS, ALPHA, NDISFM, RECVRY).
// ctest-labels: kw-g4a keywords
//
// round_trip_oracle_cases: 28 KFWRIT round trips of the original DEF_PRE (8 library material
// KEYs incl. the 4 user materials, 20 example KEYs; tests/data/kw_g4a/make_cases.py). The test
// reads <case>.in.key with a small stand-in for the KEY loop and the group-4 dispatcher (kw-g4b
// owns the real ones), writes every material's kw-g4a keywords in the order of FUN_1403cdca0 and
// compares the bytes with the oracle's blocks in <case>.out.key.
//
// The other cases cover branches the corpus lacks; their expected text is worked out by hand
// from the write grammar in the specs (format strings, field widths, three-digit exponents).
#include <defkey/kw_g4a.hpp>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defmodel/layout/material.hpp>
#include <testkit/check.hpp>

#include "key_test_util.hpp"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <memory>
#include <new>
#include <sstream>
#include <string>
#include <vector>

using namespace def::key::g4a;
using def::model::DYNARY;
using def::model::Material;

namespace {

// ---- Material stand-in -------------------------------------------------------------------------
// The Material ctor (14028c6f0, kw-g4b) is not ported yet. A zeroed Material with the kw-g4a
// sub-objects constructed in place, plus what its init 14028cdc0 sets for kw-g4a fields
// (FRAE2H 0.9, both FPERV tables type 0), is the same state for every field this packet touches.
struct MatDeleter {
    void operator()(Material* m) const
    {
        m->fstres.~Cls_140288ae0();
        m->creep.~Cls_140289b80();
        m->disloc.~Cls_14028bcc0();
        m->frcmod.~Cls_14028c040();
        m->aniso.~Cls_140289fe0();
        m->fperv1.~DYNARY();
        m->fperv2.~DYNARY();
        ::operator delete(static_cast<void*>(m));
    }
};
using MatPtr = std::unique_ptr<Material, MatDeleter>;

MatPtr new_material(std::int32_t number)
{
    void* raw = ::operator new(sizeof(Material));
    std::memset(raw, 0, sizeof(Material));
    Material* m = static_cast<Material*>(raw);
    new (&m->fstres) def::model::Cls_140288ae0();
    new (&m->creep) def::model::Cls_140289b80();
    new (&m->disloc) def::model::Cls_14028bcc0();
    new (&m->frcmod) def::model::Cls_14028c040();
    new (&m->aniso) def::model::Cls_140289fe0();
    new (&m->fperv1) DYNARY();
    new (&m->fperv2) DYNARY();
    m->frae2h = 0.9;  // DAT_14051f220
    m->number = number;
    return MatPtr(m);
}

using ReadFn = int (*)(std::FILE*, Material&);
using WriteFn = int (*)(std::FILE*, std::int32_t, Material&);

struct Kw {
    const char* name;
    ReadFn read;
};

const Kw kMine[] = {
    {"FRAE2H", read_frae2h}, {"FPERV", read_fperv},   {"FRCMOD", read_frcmod}, {"FSTRES", read_fstres},
    {"CREEP", read_creep},   {"ANISO", read_aniso},   {"BURGRS", read_burgrs}, {"ALPHA", read_alpha},
    {"NDISFM", read_ndisfm}, {"RECVRY", read_recvry},
};

const char* const kGroup4[] = {
    "MTNAME", "FRAE2H", "FSTRES", "YOUNG",  "POISON", "EXPAND", "THRCND", "HEATCP", "EMSVTY", "LTHEAT", "TMPSOL",
    "TMPLIQ", "FPERV",  "FRCMOD", "CREEP",  "DIFCOE", "GRNDAT", "GRNMDL", "BURGRS", "ALPHA",  "NDISFM", "RECVRY",
    "HDNPHA", "ELRST",  "MSTMTR", "HDNTIM", "JOMINY", "UTSDAT", "HDNRUL", "PMEAB",  "PMITT",  "ANISO",  "MATDEN",
    "MASDEN", "COARSE", "SIZEMD", "TXTURE", "DIFBND", "SINTER", "GBENGY", "GBMOBI", "NUCSIZ",
};

// The kw-g4a writers in the order FUN_1403cdca0 calls them.
const WriteFn kWriteOrder[] = {
    write_frae2h_mat, write_fperv_mat,  write_frcmod_mat, write_fstres_mat, write_creep_mat,
    write_aniso_mat,  write_burgrs_mat, write_alpha_mat,  write_ndisfm_mat, write_recvry_mat,
};

struct Model {
    std::vector<MatPtr> mats;
    Material* find_or_add(std::int32_t n)
    {
        for (auto& m : mats)
            if (m->number == n) return m.get();
        mats.push_back(new_material(n));
        return mats.back().get();
    }
};

// Stand-in for FUN_1401b6520 + FUN_140384a50: returns the number of reader errors.
int read_key(std::FILE* fp, Model& model)
{
    int errors = 0;
    char tok[40];
    char rest[0x400];
    while (std::fscanf(fp, "%39s", tok) == 1) {
        bool g4 = false;
        for (const char* g : kGroup4) g4 = g4 || std::strcmp(g, tok) == 0;
        if (!g4) {
            std::fgets(rest, sizeof rest, fp);
            continue;
        }
        std::int32_t n = 0;
        if (std::fscanf(fp, "%i", &n) == -1) break;
        Material* m = model.find_or_add(n);
        const Kw* kw = nullptr;
        for (const Kw& k : kMine)
            if (std::strcmp(k.name, tok) == 0) kw = &k;
        if (kw == nullptr) {
            std::fgets(rest, sizeof rest, fp);
            continue;
        }
        if (kw->read(fp, *m) != 0) ++errors;
    }
    return errors;
}

// Like FUN_1403cdca0 for the kw-g4a keywords: stops at the first error.
int write_key(std::FILE* fp, Model& model)
{
    for (auto& m : model.mats)
        for (WriteFn w : kWriteOrder) {
            const int r = w(fp, m->number, *m);
            if (r != 0) return r;
        }
    return 0;
}

std::string slurp_binary(const std::string& path)
{
    std::ifstream f(path, std::ios::binary);
    std::ostringstream s;
    s << f.rdbuf();
    return s.str();
}

std::string data_dir() { return (testkit::repo_root() / "port/defkey/tests/data/kw_g4a").string() + "/"; }

std::string temp_path(const char* tag)
{
    return (std::filesystem::temp_directory_path() / keytest::tmp_name(std::string("kw_g4a_") + tag + ".key")).string();
}

// Reads `text` (written to a temp file in binary, read back in text mode like the original),
// applies `body` to the model, writes in text mode and returns the bytes written.
template <class F>
std::string run_text(const char* tag, const std::string& text, F body, int* write_ret = nullptr)
{
    const std::string in = temp_path((std::string(tag) + "_in").c_str());
    const std::string out = temp_path((std::string(tag) + "_out").c_str());
    {
        std::ofstream f(in, std::ios::binary);
        f << text;
    }
    Model model;
    std::FILE* fi = std::fopen(in.c_str(), "r");
    body(fi, model);
    std::fclose(fi);
    std::FILE* fo = std::fopen(out.c_str(), "w");
    const int r = write_key(fo, model);
    std::fclose(fo);
    if (write_ret != nullptr) *write_ret = r;
    std::string got = slurp_binary(out);
    std::remove(in.c_str());
    std::remove(out.c_str());
    return got;
}

std::string crlf(const std::string& s)
{
    std::string r;
    for (char c : s) {
        if (c == '\n') r += '\r';
        r += c;
    }
    return r;
}

void check_text(const std::string& got, const std::string& want, const char* what)
{
    if (got == want) {
        CHECK(true);
        return;
    }
    std::size_t i = 0;
    while (i < got.size() && i < want.size() && got[i] == want[i]) ++i;
    const std::size_t from = i < 120 ? 0 : i - 120;
    testkit::fail(__FILE__, __LINE__,
                  std::string(what) + ": first difference at byte " + std::to_string(i) + "\n--- got:\n" +
                      got.substr(from, 240) + "\n--- want:\n" + want.substr(from, 240));
}

} // namespace

TEST_CASE(round_trip_oracle_cases)
{
    std::ifstream list(data_dir() + "cases.txt");
    REQUIRE(list.good());
    std::string name;
    int n = 0;
    while (std::getline(list, name)) {
        if (name.empty()) continue;
        ++n;
        const std::string input = slurp_binary(data_dir() + name + ".in.key");
        const std::string want = slurp_binary(data_dir() + name + ".out.key");
        int errors = 0;
        const std::string got = run_text("oracle", input, [&](std::FILE* fp, Model& m) { errors = read_key(fp, m); });
        CHECK_EQ(errors, 0);
        check_text(got, want, name.c_str());
    }
    CHECK_EQ(n, 28);
}

// The four user materials, by name (also inside round_trip_oracle_cases).
TEST_CASE(user_materials_byte_for_byte)
{
    const char* names[] = {"IN718_6um_1650-2200F_900-1200C___s000006", "JIS-S45C_HeatTreatment__s000006",
                           "AISI-1020_Machining__s000008", "AISI4340-JohnsonCook_s000008"};
    for (const char* name : names) {
        const std::string input = slurp_binary(data_dir() + name + ".in.key");
        const std::string want = slurp_binary(data_dir() + name + ".out.key");
        REQUIRE(!input.empty());
        const std::string got = run_text("user", input, [&](std::FILE* fp, Model& m) { read_key(fp, m); });
        check_text(got, want, name);
    }
}

namespace {

// read `text` as KEY lines for material 1 and return what the writers print
std::string rt(const std::string& text, int* ret = nullptr)
{
    return run_text("unit", text, [](std::FILE* fp, Model& m) { read_key(fp, m); }, ret);
}

// default lines of all kw-g4a keywords except `skip` (in writer order), for material 1
std::string defaults_except(const std::string& skip, const std::string& replacement)
{
    const char* lines[][2] = {
        {"FRAE2H", "FRAE2H       1    9.0000000000E-001\n"},
        {"FPERV", "FPERV        1    0.0000000000E+000       0    0.0000000000E+000       0\n"},
        {"FRCMOD", "FRCMOD       1       0    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000"
                   "    0.0000000000E+000    0.0000000000E+000\n"},
        {"FSTRES", "FSTRES       1       3\n       0       0       0\n"},
        {"CREEP", "CREEP        1       0\n"},
        {"ANISO", ""},
        {"BURGRS", "BURGRS       1       0    0.0000000000E+000\n"},
        {"ALPHA", "ALPHA        1       0    0.0000000000E+000\n"},
        {"NDISFM", "NDISFM       1       0    0.0000000000E+000\n"},
        {"RECVRY", "RECVRY       1       0    0.0000000000E+000\n"},
    };
    std::string s;
    for (auto& l : lines) s += (skip == l[0]) ? replacement : l[1];
    return crlf(s);
}

} // namespace

TEST_CASE(defaults_of_a_new_material)
{
    // MTNAME only creates the material: everything else is the ctor default
    CHECK_EQ(rt("MTNAME 1\nX\n"), defaults_except("", ""));
}

TEST_CASE(fstres_type0_aborts_the_write)
{
    int ret = 0;
    const std::string got = rt("FSTRES 1 0\n", &ret);
    CHECK_EQ(ret, 3);
    // FRAE2H, FPERV and FRCMOD come before FSTRES and are already written
    CHECK_EQ(got, crlf("FRAE2H       1    9.0000000000E-001\n"
                       "FPERV        1    0.0000000000E+000       0    0.0000000000E+000       0\n"
                       "FRCMOD       1       0    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000"
                       "    0.0000000000E+000    0.0000000000E+000\n"));
}

TEST_CASE(fstres_types_5_9_10_14_16_negative)
{
    CHECK_EQ(rt("FSTRES 1 5\n1 2 3 4\n"),
             defaults_except("FSTRES", "FSTRES       1       5\n    1.0000000000E+000    2.0000000000E+000"
                                       "    3.0000000000E+000    4.0000000000E+000\n"));
    // t 10: seven numbers read, the 7th is dropped
    CHECK_EQ(rt("FSTRES 1 10\n1 2 3 4 5 6 7\n"),
             defaults_except("FSTRES", "FSTRES       1      10\n    1.0000000000E+000    2.0000000000E+000"
                                       "    3.0000000000E+000    4.0000000000E+000    5.0000000000E+000"
                                       "    6.0000000000E+000\n"));
    CHECK_EQ(rt("FSTRES 1 9\n1 2 3 4 5 6 7\n8 9 10 11 12 13 14\n"),
             defaults_except("FSTRES",
                             "FSTRES       1       9\n"
                             "    1.0000000000E+000    2.0000000000E+000    3.0000000000E+000    4.0000000000E+000"
                             "    5.0000000000E+000    6.0000000000E+000    7.0000000000E+000\n"
                             "    8.0000000000E+000    9.0000000000E+000    1.0000000000E+001    1.1000000000E+001"
                             "    1.2000000000E+001    1.3000000000E+001    1.4000000000E+001\n"));
    // t 14 with a constant exponent, and with a temperature table (written back as generic type 6)
    CHECK_EQ(rt("FSTRES 1 14\n1 2 3 4\n0 5\n"),
             defaults_except("FSTRES", "FSTRES       1      14\n    1.0000000000E+000    2.0000000000E+000"
                                       "    3.0000000000E+000    4.0000000000E+000\n       0    5.0000000000E+000\n"));
    CHECK_EQ(rt("FSTRES 1 14\n1 2 3 4\n1 2\n100 3\n200 4\n"),
             defaults_except("FSTRES", "FSTRES       1      14\n    1.0000000000E+000    2.0000000000E+000"
                                       "    3.0000000000E+000    4.0000000000E+000\n       6       2\n"
                                       "    1.0000000000E+002    3.0000000000E+000\n"
                                       "    2.0000000000E+002    4.0000000000E+000\n"));
    // t 16: ncomp 2, n 1 1 2 -> axes on one line each, 4 values wrapped every max(n) = 2
    CHECK_EQ(rt("FSTRES 1 16\n2\n1 1 2\n0.1\n1\n20 30\n1 2 3 4\n"),
             defaults_except("FSTRES", "FSTRES       1      16\n       2\n       1       1       2\n"
                                       "    1.0000000000E-001\n    1.0000000000E+000\n"
                                       "    2.0000000000E+001    3.0000000000E+001\n"
                                       "    1.0000000000E+000    2.0000000000E+000\n"
                                       "            3.0000000000E+000    4.0000000000E+000\n"));
    CHECK_EQ(rt("FSTRES 1 -3\n"), defaults_except("FSTRES", "FSTRES       1      -3\n"));
}

TEST_CASE(fstres_type13_nvar_table)
{
    // two variables with 2 and 3 points: B has 5 values (%15.7E, no separator), C has 6
    const std::string in = "FSTRES 1 13\n2\n2 7 8\n3 9 10\n1 2\n3 4 5\n1 2 3 4 5 6\n";
    CHECK_EQ(rt(in), defaults_except("FSTRES", "FSTRES       1      13\n       2\n"
                                               "       2       7       8\n       3       9      10\n"
                                               " 1.0000000E+000 2.0000000E+000\n"
                                               " 3.0000000E+000 4.0000000E+000 5.0000000E+000\n"
                                               "    1.0000000000E+000    2.0000000000E+000    3.0000000000E+000"
                                               "    4.0000000000E+000    5.0000000000E+000\n"
                                               "            6.0000000000E+000\n"));
}

TEST_CASE(fstres_type12_initial_flow_stress)
{
    CHECK_EQ(rt("FSTRES 1 12 2\n2\n0.1 5\n1 6\n"),
             defaults_except("FSTRES", "FSTRES       1      12       2       0\n       2\n"
                                       "    1.0000000000E-001    5.0000000000E+000\n"
                                       "    1.0000000000E+000    6.0000000000E+000\n"));
}

TEST_CASE(creep_types)
{
    // t 3 (GE, 5 coefficients): the 4th header field is written as ncomp + 1
    CHECK_EQ(rt("CREEP 1 3 1 99\n500 1 2 3 4 5\n"),
             defaults_except("CREEP", "CREEP        1       3       1       6\n"
                                      "    5.0000000000E+002    1.0000000000E+000    2.0000000000E+000"
                                      "    3.0000000000E+000    4.0000000000E+000    5.0000000000E+000\n"));
    CHECK_EQ(rt("CREEP 1 7\n1\n2 0 0\n1 2\n3 4\n"),
             defaults_except("CREEP", "CREEP        1       7\n       1\n       2       0       0\n"
                                      " 1.0000000E+000 2.0000000E+000\n"
                                      "    3.0000000000E+000    4.0000000000E+000\n"));
    CHECK_EQ(rt("CREEP 1 -4\n"), defaults_except("CREEP", "CREEP        1      -4\n"));
}

TEST_CASE(frcmod_types_11_12_17)
{
    CHECK_EQ(rt("FRCMOD 1 11 1 2 3 4 5\n1 2 3 4 5 6 7 8 9\n"),
             defaults_except("FRCMOD", "FRCMOD       1      11    1.0000000000E+000    0.0000000000E+000"
                                       "    0.0000000000E+000    0.0000000000E+000    5.0000000000E+000\n"
                                       "    1.0000000000E+000    2.0000000000E+000    3.0000000000E+000"
                                       "    4.0000000000E+000    5.0000000000E+000    6.0000000000E+000"
                                       "    7.0000000000E+000    8.0000000000E+000    9.0000000000E+000\n"));
    // t 12: e forced to 100, b = rows
    CHECK_EQ(rt("FRCMOD 1 12 0.5 1 0 0 7\n10 20\n"),
             defaults_except("FRCMOD", "FRCMOD       1      12    5.0000000000E-001    1.0000000000E+000"
                                       "    0.0000000000E+000    0.0000000000E+000    1.0000000000E+002\n"
                                       "    1.0000000000E+001    2.0000000000E+001\n"));
    CHECK_EQ(rt("FRCMOD 1 17 0 2\n1 2\n3 4\n"),
             defaults_except("FRCMOD", "FRCMOD       1      17    0.0000000000E+000    2.0000000000E+000"
                                       "    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"
                                       "    1.0000000000E+000    2.0000000000E+000\n"
                                       "    3.0000000000E+000    4.0000000000E+000\n"));
    CHECK_EQ(rt("FRCMOD 1 -2 3\n"),
             defaults_except("FRCMOD", "FRCMOD       1      -2    3.0000000000E+000    0.0000000000E+000"
                                       "    0.0000000000E+000    0.0000000000E+000    0.0000000000E+000\n"));
}

TEST_CASE(fperv_tables)
{
    CHECK_EQ(rt("FPERV 1 2 1 0.5 0\n0 1\n10 2\n"),
             defaults_except("FPERV", "FPERV        1       2       1    5.0000000000E-001       0\n"
                                      "    0.0000000000E+000    1.0000000000E+000\n"
                                      "    1.0000000000E+001    2.0000000000E+000\n"));
}

TEST_CASE(aniso_round_trip_and_tv4_abort)
{
    CHECK_EQ(rt("ANISO 1 2 3 1\n2\n20 1 2 3\n500 4 5 6\n"),
             defaults_except("ANISO", "ANISO        1       2       3       1\n       2\n"
                                      "    2.0000000000E+001    1.0000000000E+000    2.0000000000E+000"
                                      "    3.0000000000E+000\n"
                                      "    5.0000000000E+002    4.0000000000E+000    5.0000000000E+000"
                                      "    6.0000000000E+000\n"));
    CHECK_EQ(rt("ANISO 1 -7\n"), defaults_except("ANISO", "ANISO        1      -7\n"));
    int ret = 0;
    rt("ANISO 1 1 6 4\n1 1\n20\n0.1\n1 2 3 4 5 6\n", &ret);
    CHECK_EQ(ret, 3);
}

TEST_CASE(dislocation_tables)
{
    CHECK_EQ(rt("ALPHA 1 3 2 1\n20 30\n0.1\n1 2\n"),
             defaults_except("ALPHA", "ALPHA        1       3       2       1\n"
                                      "    2.0000000000E+001    3.0000000000E+001\n"
                                      "    1.0000000000E-001\n"
                                      "    1.0000000000E+000    2.0000000000E+000\n"));
    CHECK_EQ(rt("BURGRS 1 7\n1 1 1\n20 0.1 5 9\n"),
             defaults_except("BURGRS", "BURGRS       1       7\n       1       1       1\n"
                                       "    2.0000000000E+001\n    1.0000000000E-001\n    5.0000000000E+000\n"
                                       "    9.0000000000E+000\n"));
    // an incomplete table drops the keyword (n0 = 0)
    CHECK_EQ(rt("RECVRY 1 1 0\n"), defaults_except("RECVRY", ""));
    // t outside 0..3 is a reading error, the DYNARY keeps its default
    int ret = 0;
    CHECK_EQ(rt("NDISFM 1 9 1\n", &ret), defaults_except("", ""));
}

TEST_CASE(handlers_and_accessors_through_the_model)
{
    // DB_STEP_DATA is used only for its material list here (zeroed storage, list linked by hand)
    void* raw = ::operator new(sizeof(def::model::DB_STEP_DATA));
    std::memset(raw, 0, sizeof(def::model::DB_STEP_DATA));
    auto* md = static_cast<def::model::DB_STEP_DATA*>(raw);
    MatPtr m1 = new_material(4);
    MatPtr m2 = new_material(7);
    md->materials.head = m1.get();
    m1->next = m2.get();
    md->materials.tail = m2.get();
    md->materials.count = 2;
    m2->frae2h = 0.25;

    const std::string out = temp_path("handler");
    std::FILE* fo = std::fopen(out.c_str(), "w");
    CHECK_EQ(write_frae2h(fo, md, {7}), 0);
    std::fclose(fo);
    CHECK_EQ(slurp_binary(out), crlf("FRAE2H       7    2.5000000000E-001\n"));
    std::remove(out.c_str());

    void* p = nullptr;
    CHECK_EQ(accessor_fstres(md, "Coefficient (H)", {4}, &p), 0);
    CHECK(p == &m1->fstres.h);
    CHECK_EQ(accessor_fstres(md, "nothing", {4}, &p), 3);
    CHECK_EQ(accessor_creep(md, "Creep - TableData", {7}, &p), 0);
    CHECK(p == &m2->creep.d6);
    CHECK_EQ(accessor_fperv(md, "Body Force", {7}, &p), 0);
    CHECK(p == &m2->fperv1);
    CHECK_EQ(accessor_fperv(md, "x", {7}, &p), 0);
    CHECK(p == &m2->fperv2);
    CHECK_EQ(accessor_aniso(md, "x", {4}, &p), 0);
    CHECK(p == &m1->aniso.d);
    p = nullptr;
    CHECK_EQ(accessor_recvry(md, "x", {5}, &p), 0);  // missing material: 0, *out unchanged
    CHECK(p == nullptr);
    ::operator delete(raw);
}

int main()
{
    keytest::legacy_printf();  // what def::rt::init_stdio_compat() sets in def_pre
    return testkit::run_all();
}
