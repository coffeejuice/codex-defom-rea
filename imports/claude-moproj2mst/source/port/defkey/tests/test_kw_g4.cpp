// ctest-labels: kw-g4b
// Tier-1 tests of the group 4 parser FUN_140384a50, the material writer FUN_1403cdca0 and the
// kw-g4b keyword handlers, on the branches the corpus does not reach (docs/re/keywords/
// group4_5_7-index.md "Not in the corpus") and on the read/write asymmetries the specs list.
// Expected texts follow the write grammars of docs/re/keywords/<KW>.md.
#include "kw_g4_support.hpp"

#include <testkit/check.hpp>

#include <defkey/kw_g4.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/mesh2d/material_ops.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

#include <string>
#include <vector>

using def::model::Material;
namespace g4 = def::key::g4;

namespace {

// " %20.10E" of v (one value of a table line)
std::string E(double v)
{
    char b[64];
    def::rt::crt::sprintf(b, " %20.10E", v);
    return b;
}

// "%7i"-style int field with its leading space
std::string I(int v)
{
    char b[32];
    std::snprintf(b, sizeof b, " %7i", v);
    return b;
}

// LF text -> what a text-mode FILE writes
std::string crlf(const std::string& s)
{
    std::string r;
    for (char c : s) {
        if (c == '\n') r += '\r';
        r += c;
    }
    return r;
}

struct Fixture {
    kwg4::Model model;
    kwg4::Hooks hooks{model.m()};
    std::string log;

    int read(const std::string& key)
    {
        std::FILE* lf = std::fopen(keytest::tmp_name("kw_g4_unit.log").c_str(), "w+");
        const int rc = kwg4::read_key_text(model.m(), key, hooks, lf);
        std::rewind(lf);
        log.clear();
        int c;
        while ((c = std::fgetc(lf)) != EOF) log += static_cast<char>(c);
        std::fclose(lf);
        std::remove(keytest::tmp_name("kw_g4_unit.log").c_str());
        return rc;
    }
    Material* mat(int n) { return def::model::material_list_find(&model.m().materials, n); }

    template <class W>
    std::string write(W w, int n = 1)
    {
        return kwg4::text_output("kw_g4_unit_out.KEY", [&](std::FILE* f) { w(f, model.m(), std::vector<int>{n}); });
    }
};

} // namespace

TEST_CASE(constant_property_line_format)
{
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("YOUNG 1 0 2.68E+004\n"), 0);
    CHECK_EQ(fx.write(g4::write_young), crlf("YOUNG        1       0    2.6800000000E+004\n"));
    // default: constant 0
    CHECK_EQ(fx.write(g4::write_poison), crlf("POISON       1       0    0.0000000000E+000\n"));
}

TEST_CASE(new_materials_are_appended_in_first_use_order)
{
    Fixture fx;
    CHECK_EQ(fx.read("POISON 7 0 0.3\nYOUNG 2 0 1\nTHRCND 7 0 5\n"), 0);
    CHECK_EQ(fx.model.m().materials.count, 2);
    CHECK_EQ(fx.model.m().n_materials, 2);
    CHECK_EQ(fx.model.m().materials.head->number, 7);
    CHECK_EQ(fx.model.m().materials.head->next->number, 2);
    CHECK_EQ(fx.mat(7)->frae2h, 0.9); // ctor default
    CHECK(fx.model.m().materials.numbers == nullptr); // freed again after the add
}

TEST_CASE(young_type5_table_has_no_header_line)
{
    // t 5 (var 3, 8) is read but 1402f3aa0 has no header for ndim 2 / var (3,8): body only.
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("YOUNG 1 5 2 1\n10 20\n0.5\n1 2\n"), 0);
    CHECK_EQ(fx.write(g4::write_young), crlf(E(10) + E(20) + "\n" + E(0.5) + "\n" + E(1) + E(2) + "\n"));
}

TEST_CASE(heatcp_int_extra_and_expand_double_extra)
{
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("HEATCP 1 0 2.5 7.9\nEXPAND 1 0 1.2e-5 20\n"), 0);
    CHECK_EQ(fx.mat(1)->heatcp_extra, 7); // (int)7.9
    CHECK_EQ(fx.write(g4::write_heatcp), crlf("HEATCP       1       0" + E(2.5) + I(7) + "\n"));
    CHECK_EQ(fx.write(g4::write_expand), crlf("EXPAND       1       0" + E(1.2e-5) + E(20) + "\n"));
}

TEST_CASE(pmeab_two_d_table_is_forced_to_var_20)
{
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("PMEAB 1 4 2 1\n300 400\n0.1\n5 6\n"), 0);
    CHECK_EQ(fx.mat(1)->pmeab.var[1], 20);
    CHECK_EQ(fx.write(g4::write_pmeab),
             crlf("PMEAB        1       4       2       1\n" + E(300) + E(400) + "\n" + E(0.1) + "\n" +
                  E(5) + E(6) + "\n"));
}

TEST_CASE(difcoe_modes)
{
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("DIFCOE 1 1 2 1\n100 200\n0.5\n1 2\n"), 0);
    CHECK_EQ(fx.write(g4::write_difcoe),
             crlf("DIFCOE       1       1       2       1\n" + E(100) + E(200) + "\n" + E(0.5) + "\n" +
                  E(1) + E(2) + "\n"));
    CHECK_EQ(fx.read("DIFCOE 1 2 1\n500 1 2\n"), 0); // type 0x27: ncomp 2
    CHECK_EQ(fx.write(g4::write_difcoe), crlf("DIFCOE       1       2       1\n" + E(500) + E(1) + E(2) + "\n"));
    CHECK_EQ(fx.read("DIFCOE 1 -5\n"), 0);
    CHECK_EQ(fx.write(g4::write_difcoe), crlf("DIFCOE       1      -5\n"));
    CHECK_EQ(fx.read("DIFCOE 1 9 1\n"), 0); // t >= 4: nothing changes, mode stays 4
    CHECK_EQ(fx.mat(1)->difcoe.mode, 4);
    // default: type-0x29 table with ndim reset to 0 by the ctor: constant 0
    CHECK_EQ(fx.read("YOUNG 2 0 1\n"), 0);
    CHECK_EQ(fx.write(g4::write_difcoe, 2), crlf("DIFCOE       2       0    0.0000000000E+000\n"));
}

TEST_CASE(hdnpha_unknown_type_is_a_data_error_and_reading_goes_on)
{
    Fixture fx;
    CHECK_EQ(fx.read("HDNPHA 1 5 0 0\nYOUNG 1 0 3\n"), 3);
    CHECK(fx.log.find("ERROR: Keyword HDNPHA Data Error. \n") != std::string::npos);
    CHECK_EQ(fx.mat(1)->young.value, 3.0);
}

TEST_CASE(grndat_type2_records)
{
    keytest::legacy_printf();
    Fixture fx;
    const char* key =
        "GRNDAT 1 2 7\n"
        "2\n1.5 2.5\n3 4.5\n1 2\n10 100\n20 200\n" // main record, sub3 only
        "1\n"                                       // list 1: one record
        "1\n9.5\n2 0.25\n0 7.5\n2 1\n5 6\n-1\n"     // sub1 const, sub2 var 23, sub3 untouched
        "0\n0\n0\n0\n"
        "YOUNG 1 0 1\n";
    CHECK_EQ(fx.read(key), 0);
    CHECK_EQ(fx.mat(1)->young.value, 1.0); // stream stayed in step
    const std::string want =
        "GRNDAT       1       2       7\n"
        "       2\n" + E(1.5) + E(2.5) + "\n" + I(3) + E(4.5) + "\n" +
        "       1       2\n" + E(10) + E(100) + "\n" + E(20) + E(200) + "\n" +
        "       1\n" +
        "       1\n" + E(9.5) + "\n" + I(2) + E(0.25) + "\n" +
        I(0) + E(7.5) + "\n" + "       2       1\n" + E(5) + E(6) + "\n" + I(0) + E(0) + "\n" +
        "       0\n       0\n       0\n       0\n";
    CHECK_EQ(fx.write(g4::write_grndat), crlf(want));
}

TEST_CASE(grndat_types_4_5_and_negative)
{
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("GRNDAT 1 4\n1 2\n100 1\n200 2\n"), 0);
    CHECK_EQ(fx.write(g4::write_grndat),
             crlf("GRNDAT       1       4\n       5       2       0\n" + E(100) + E(1) + "\n" + E(200) + E(2) + "\n"));
    CHECK_EQ(fx.read("GRNDAT 1 5\n1 2 3 4 5 6\n"), 0);
    CHECK_EQ(fx.write(g4::write_grndat),
             crlf("GRNDAT       1       5\n" + E(1) + E(2) + E(3) + E(4) + E(5) + E(6) + "\n"));
    CHECK_EQ(fx.read("GRNDAT 1 -3\n"), 0);
    CHECK_EQ(fx.mat(1)->grndat.type, -1);
    CHECK_EQ(fx.mat(1)->grndat.ref, 3);
    CHECK_EQ(fx.write(g4::write_grndat), std::string());
}

TEST_CASE(grndat_record_count_above_12_is_cut)
{
    Fixture fx;
    CHECK_EQ(fx.read("GRNDAT 1 2 0\n13\n1 2 3 4 5 6 7 8 9 10 11 12\n13 0.5\n0 1\n0\n0\n0\n0\n0\n"), 0);
    CHECK_EQ(fx.mat(1)->grndat.type2.main.n, 12);
    // the 13th value was not read as a value: it is the record's int
    CHECK_EQ(fx.mat(1)->grndat.type2.main.i, 13);
}

TEST_CASE(difbnd_t7_and_t0)
{
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("DIFBND 1 7 2 1\n300 400\n0.1\n1 2 3 4\n"), 0); // ncomp 2 kept (type 0xaf)
    CHECK_EQ(fx.write(g4::write_difbnd),
             crlf("DIFBND       1       7       2       1\n" + E(300) + E(400) + "\n" + E(0.1) + "\n" +
                  E(1) + E(2) + "\n        " + E(3) + E(4) + "\n"));
    CHECK_EQ(fx.read("DIFBND 2 0 3.5\n"), 0);
    CHECK_EQ(fx.write(g4::write_difbnd, 2), crlf("DIFBND       2       0" + E(3.5) + "\n"));
    CHECK_EQ(fx.read("DIFBND 2 3 1\n"), 2);
}

TEST_CASE(sinter_models)
{
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("SINTER 1 3 4\n1 2 3 4\n"), 0);
    CHECK_EQ(fx.write(g4::write_sinter), crlf("SINTER       1       3       4\n" + E(1) + E(2) + E(3) + E(4) + "\n"));
    CHECK_EQ(fx.read("SINTER 1 2 0\nYOUNG 1 0 1\n"), 0); // count 0: no values line read
    CHECK_EQ(fx.write(g4::write_sinter), crlf("SINTER       1       2       2\n" + E(1) + E(2) + "\n"));
    CHECK_EQ(fx.read("SINTER 1 1 0\n"), 0);
    CHECK_EQ(fx.write(g4::write_sinter), crlf("SINTER       1       1       0\n"));
    CHECK_EQ(fx.read("SINTER 1 7 0\n"), 0);
    CHECK_EQ(fx.write(g4::write_sinter), std::string());
}

TEST_CASE(coarse_flag_and_table)
{
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("COARSE 1 0\n"), 0);
    CHECK_EQ(fx.write(g4::write_coarse), crlf("COARSE       1       0\n"));
    CHECK_EQ(fx.read("COARSE 1 2\n0 5.5\n"), 0);
    CHECK_EQ(fx.write(g4::write_coarse), crlf("COARSE       1       2\n" + I(0) + E(5.5) + "\n"));
}

TEST_CASE(gbmobi_jominy_hdntim)
{
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("GBMOBI 1 1.5 2.5\n"), 0);
    CHECK_EQ(fx.write(g4::write_gbmobi), crlf("GBMOBI       1" + E(1.5) + E(2.5) + "\n"));
    // empty default tables are not written
    CHECK_EQ(fx.write(g4::write_jominy), std::string());
    CHECK_EQ(fx.write(g4::write_hdntim), std::string());
    CHECK_EQ(fx.read("HDNTIM 1 2\n1 10\n2 20\nJOMINY 1 1 1 2\n0.2\n5 10\n30 40\n"), 0);
    CHECK_EQ(fx.write(g4::write_hdntim), crlf("HDNTIM       1       2\n" + E(1) + E(10) + "\n" + E(2) + E(20) + "\n"));
    CHECK_EQ(fx.write(g4::write_jominy),
             crlf("JOMINY       1       1       1       2\n" + E(0.2) + "\n" + E(5) + E(10) + "\n" +
                  E(30) + E(40) + "\n"));
}

TEST_CASE(read_only_keywords_store_or_discard)
{
    Fixture fx;
    CHECK_EQ(fx.read("LTHEAT 1 012\nTMPSOL 1 1350\nTMPLIQ 1 1450\nGRNMDL 1 3\nTXTURE 1 2 3 4\n"
                     "SIZEMD 1 5\nHDNRUL 1 2\nMATDEN 1 7.8\n"),
             0);
    Material* m = fx.mat(1);
    CHECK_EQ(m->tmpsol, 1350.0);
    CHECK_EQ(m->tmpliq, 1450.0);
    CHECK_EQ(m->txture.a, 2);
    CHECK_EQ(m->txture.c, 4);
    CHECK_EQ(m->sizemd, 5);
    CHECK_EQ(m->hdnrul, 2);
    CHECK_EQ(m->matden, 7.8);
}

TEST_CASE(mstmtr_sets_and_clears_the_mixture)
{
    Fixture fx;
    CHECK_EQ(fx.read("YOUNG 3 0 1\nYOUNG 4 0 1\nMSTMTR 1 2\n3 4\n"), 0);
    Material* m = fx.mat(1);
    CHECK_EQ(m->mstmtr_flag, 1);
    CHECK_EQ(m->mstmtr_count, 2);
    CHECK_EQ(m->mstmtr_list[1], 4);
    fx.mat(3)->mstmtr_flag = 1; // as a phase material would be marked
    CHECK_EQ(fx.read("MSTMTR 1 0\n"), 0);
    CHECK_EQ(m->mstmtr_flag, 0);
    CHECK_EQ(fx.mat(3)->mstmtr_flag, 0);
}

TEST_CASE(material_block_order_and_inline_keywords)
{
    keytest::legacy_printf();
    Fixture fx;
    CHECK_EQ(fx.read("MTNAME 5\n  STEEL A  \nMSTMTR 5 1\n2\nHDNRUL 5 1\nMATDEN 5 7.85\nSIZEMD 5 3\n"
                     "TXTURE 5 1 2 3\n"),
             0);
    Material* m = fx.mat(5);
    const std::string out = kwg4::text_output("kw_g4_unit_mat.KEY", [&](std::FILE* f) {
        CHECK_EQ(g4::write_material(fx.model.m(), f, m, nullptr), 0);
    });
    CHECK(out.rfind(crlf("*\n*  Property Data of Material     5\n*\nMTNAME       5\n  STEEL A\nFRAE2H"), 0) == 0);
    CHECK(out.find(crlf("MSTMTR       5       1\n       2\nCREEP")) != std::string::npos);
    CHECK(out.find(crlf("HDNRUL       5       1\nPMEAB")) != std::string::npos);
    CHECK(out.find(crlf("PMITT        5       0    0.0000000000E+000\nMATDEN       5    7.8500000000E+000\nBURGRS")) !=
          std::string::npos);
    CHECK(out.find(crlf("SIZEMD       5       3\nTXTURE       5       1       2       3\nGBENGY")) != std::string::npos);
    // 2-D model: no MATDEN line
    fx.model.m().dimension = 2;
    const std::string out2 = kwg4::text_output("kw_g4_unit_mat2.KEY", [&](std::FILE* f) {
        g4::write_material(fx.model.m(), f, m, "OTHER");
    });
    CHECK(out2.find("MATDEN") == std::string::npos);
    CHECK(out2.find(crlf("MTNAME       5\nOTHER\n")) != std::string::npos);
    // a material with the skip byte set writes nothing
    m->skip_key = 1;
    CHECK_EQ(kwg4::text_output("kw_g4_unit_mat3.KEY", [&](std::FILE* f) { g4::write_material(fx.model.m(), f, m, nullptr); }),
             std::string());
}

TEST_CASE(mtname_keeps_an_inner_carriage_return)
{
    Fixture fx;
    CHECK_EQ(fx.read("MTNAME 1\nab  \r])C02  \nYOUNG 1 0 1\n"), 0);
    CHECK_EQ(std::string(fx.mat(1)->name), std::string("ab  \r])C02"));
    const std::string out = kwg4::text_output("kw_g4_unit_cr.KEY", [&](std::FILE* f) {
        def::rt::crt::fprintf(f, "%s|", fx.mat(1)->name);
    });
    CHECK_EQ(out, std::string("ab  \r])C02|"));
}

TEST_CASE(accessors)
{
    Fixture fx;
    CHECK_EQ(fx.read("YOUNG 1 0 1\n"), 0);
    void* out = nullptr;
    CHECK_EQ(g4::table_young(&fx.model.m(), "x", {1}, &out), 0);
    CHECK(out == &fx.mat(1)->young);
    void* none = reinterpret_cast<void*>(1);
    CHECK_EQ(g4::table_utsdat(&fx.model.m(), "x", {9}, &none), 0);
    CHECK(none == reinterpret_cast<void*>(1)); // untouched for a missing material
    CHECK_EQ(g4::table_utsdat(&fx.model.m(), "x", {1}, &out), 0);
    CHECK(out == &fx.mat(1)->frcmod.uts);
    out = nullptr;
    CHECK_EQ(g4::table_difcoe(&fx.model.m(), "x", {1}, &out), 0); // mode 0: untouched
    CHECK(out == nullptr);
}

int main() { return testkit::run_all(); }
