// ctest-labels: mdt item_writers
// Tier-1 tests of the single-item KEY writers (defkey/item_writers.hpp): the material KEY file
// 1401c5ea0, the equipment KEY file 1401c6d00 with 1402a83c0 and 1403d6690, and the UNIT setter
// 1401c8f70 / 1401f40b0.
//
// Corpus round trips: the DEF_GUI_MO_64 program wrote Materials/Material*.KEY and
// Equipments/Equipment*.KEY of the stage-1 corpus with the byte-identical twins of 1401c5ea0
// (0x1408959e0) and 1401c6d00 (0x140896840). Each file is read with the KEY loop (groups 2, 4, 5
// and 7) into a fresh model, written again, and compared byte for byte (text mode, CR LF). The
// corpus folder is $MDT_GUI_CORPUS, else C:/Users/alext/Projects/deform-rea-work/gui_mo/s1/corpus;
// without it those cases report SKIP and pass. The version text of the equipment banner is the
// GUI's (it is an input of the writer: model::g_version), taken from the expected file's line 2.
//
// The other cases are worked out by hand from the decompiled format strings ("%-6s %7i %20.10E",
// three-digit exponents) and keyword names from the binary's registry (0x1e UNIT, 0x2c VMIN,
// 0x2d LMAX, 0x97 MOVCTL).
#include "key_test_util.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include <defkey/item_writers.hpp>
#include <defkey/key_reader.hpp>
#include <defkey/kw_g2obj.hpp>
#include <defkey/kw_g4.hpp>
#include <defkey/kw_g57.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/layout/material.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/program_info.hpp>
#include <testkit/check.hpp>

namespace fs = std::filesystem;
using def::model::DB_STEP_DATA;
using def::model::LLOBJ_DATA;

namespace {

template <class T>
T& at(void* p, std::size_t off)
{
    return *reinterpret_cast<T*>(static_cast<unsigned char*>(p) + off);
}

std::string slurp(const fs::path& p)
{
    std::ifstream in(p, std::ios::binary);
    std::ostringstream s;
    s << in.rdbuf();
    return s.str();
}

fs::path corpus_dir()
{
    if (const char* d = std::getenv("MDT_GUI_CORPUS")) return d;
    return "C:/Users/alext/Projects/deform-rea-work/gui_mo/s1/corpus";
}

// text "\n" -> "\r\n", as a text-mode FILE writes it
std::string crlf(const std::string& s)
{
    std::string r;
    for (char c : s) {
        if (c == '\n') r += '\r';
        r += c;
    }
    return r;
}

struct Hooks : def::key::KeyReaderHooks {
    explicit Hooks(DB_STEP_DATA& m) : m_(m) {}
    bool has_sub_model() override { return false; }
    void set_model_dimension(int dim) override
    {
        const std::int32_t d = dim;
        m_.set_dimension(&d);  // vtable slot 0
    }
    void set_sub_model_dimension(int) override {}
    int parse_keyword(int group, std::FILE* f, int id) override
    {
        switch (group) {
        case 2: return def::key::g2obj::read_object_keyword(m_, f, id);
        case 4: return def::key::g4::read_keyword(m_, f, id);
        case 5: return def::key::g57::read_phase_keyword(&m_, f, id);
        case 7: return def::key::g57::read_dim_keyword(&m_, f, id);
        default: ++other; return 3;
        }
    }
    int other = 0;
    DB_STEP_DATA& m_;
};

// reads KEY bytes (CR LF; the loop reads them in text mode) into m; returns the loop status
int read_key(const std::string& bytes, Hooks& h, std::FILE* log)
{
    keytest::InFile in("item_writers_in.KEY", bytes);
    return def::key::read_key_stream(in.get(), h, log);
}

std::string first_diff(const std::string& a, const std::string& b)
{
    std::size_t i = 0;
    while (i < a.size() && i < b.size() && a[i] == b[i]) ++i;
    std::size_t s = a.rfind('\n', i == 0 ? 0 : i - 1);
    s = (s == std::string::npos) ? 0 : s + 1;
    auto line = [&](const std::string& t) {
        std::size_t e = t.find('\n', s);
        return t.substr(s, (e == std::string::npos ? t.size() : e) - s);
    };
    return "got [" + line(a) + "] want [" + line(b) + "]";
}

std::vector<fs::path> corpus_files(const char* sub)
{
    std::vector<fs::path> r;
    std::error_code ec;
    if (!fs::is_directory(corpus_dir(), ec)) return r;
    for (const auto& c : fs::directory_iterator(corpus_dir())) {
        const fs::path d = c.path() / sub;
        if (!fs::is_directory(d, ec)) continue;
        for (const auto& f : fs::directory_iterator(d))
            if (f.path().extension() == ".KEY") r.push_back(f.path());
    }
    return r;
}

} // namespace

TEST_CASE(corpus_material_files_round_trip)
{
    keytest::legacy_printf();
    const std::vector<fs::path> files = corpus_files("Materials");
    if (files.empty()) {
        std::printf("SKIP: no corpus in %s\n", corpus_dir().string().c_str());
        return;
    }
    std::FILE* log = std::fopen(keytest::tmp_name("item_writers_mat.log").c_str(), "w");
    const std::string out = keytest::tmp_name("item_writers_mat.KEY");
    int n = 0, pass = 0, other_writer = 0, lf_only = 0;
    for (const fs::path& p : files) {
        const std::string exp_raw = slurp(p);
        // Files without the banner were not written by 1401c5ea0 (it always prints it): the
        // GUI copied them from elsewhere. Not a test of this writer.
        if (exp_raw.find("*  DEFORM MATERIAL KEYWORD FILE") == std::string::npos) {
            ++other_writer;
            continue;
        }
        // A few corpus files have LF line ends (rewritten outside the GUI after it saved them);
        // their expected text is taken with LF -> CR LF.
        const bool lf = exp_raw.find('\r') == std::string::npos;
        if (lf) ++lf_only;
        const std::string exp = lf ? crlf(exp_raw) : exp_raw;
        // The material file has no dimension line; the GUI's model had the project's dimension,
        // which the same case's equipment banner names ("DEFORM-2D" / "DEFORM-3D").
        std::int32_t dim = 3;
        const std::string eq = slurp(p.parent_path().parent_path() / "Equipments" / "Equipment00001.KEY");
        if (eq.find("*  DEFORM-2D") != std::string::npos) dim = 2;
        auto m = std::make_unique<DB_STEP_DATA>(3);
        m->set_dimension(&dim);
        Hooks h(*m);
        read_key(exp, h, log);
        ++n;
        REQUIRE(m->materials.head != nullptr);
        const int rc = def::key::write_material_key_file(*m, out.c_str(), m->materials.head->number, false,
                                                         nullptr, nullptr);
        const std::string got = slurp(out);
        if (rc == 0 && got == exp && h.other == 0) {
            ++pass;
        } else {
            std::printf("FAIL %s: rc %d, other groups %d, %s\n", p.string().c_str(), rc, h.other,
                        first_diff(got, exp).c_str());
        }
    }
    std::remove(out.c_str());
    std::fclose(log);
    std::printf("material files: %d/%d (%d of them LF-only; %d files of another writer skipped)\n", pass, n,
                lf_only, other_writer);
    CHECK_EQ(pass, n);
}

TEST_CASE(corpus_equipment_files_round_trip)
{
    keytest::legacy_printf();
    const std::vector<fs::path> files = corpus_files("Equipments");
    if (files.empty()) {
        std::printf("SKIP: no corpus in %s\n", corpus_dir().string().c_str());
        return;
    }
    std::FILE* log = std::fopen(keytest::tmp_name("item_writers_eqp.log").c_str(), "w");
    const std::string out = keytest::tmp_name("item_writers_eqp.KEY");
    int n = 0, pass = 0, type5 = 0;
    for (const fs::path& p : files) {
        const std::string exp = slurp(p);
        // version text of the banner: "*  DEFORM-nD V<version> KEYWORD FILE (Qt)"
        const std::size_t l2 = exp.find('\n') + 1;
        const std::size_t v0 = exp.find(" V", l2) + 2;
        const std::size_t v1 = exp.find(" KEYWORD FILE (Qt)", v0);
        const std::string version = exp.substr(v0, v1 - v0);
        std::strncpy(def::model::g_version, version.c_str(), sizeof def::model::g_version - 1);
        // equipment number: the PRSNAM line
        const std::int32_t no = std::atoi(exp.c_str() + exp.find("PRSNAM") + 6);
        auto m = std::make_unique<DB_STEP_DATA>(3);
        Hooks h(*m);
        read_key(exp, h, log);
        ++n;
        // The object type (+0xc) is not in an equipment file (no OBJTYP line). In 1402a7c80 the
        // only path without STROKE and FORCE is type 5 (write_stroke / write_force skip it), so a
        // file without STROKE came from a type-5 object.
        LLOBJ_DATA* obj = def::model::llobj_ctor::get_or_create(m.get(), no);
        REQUIRE(obj != nullptr);
        // The movement name (+0x2160) is the line after PRSNAM, verbatim: the PRSNAM reader trims
        // a blank name to "", while the GUI's model held " " (52_BEND_2D).
        {
            const std::size_t a = exp.find('\n', exp.find("PRSNAM")) + 1;
            std::string name = exp.substr(a, exp.find('\n', a) - a);
            if (!name.empty() && name.back() == '\r') name.pop_back();
            std::strncpy(&at<char>(obj, 0x2160), name.c_str(), 0x50);
        }
        if (exp.find("\nSTROKE") == std::string::npos) {
            at<std::int32_t>(obj, 0xc) = 5;
            ++type5;
        }
        const int rc = def::key::write_equipment_key_file(*m, out.c_str(), no);
        const std::string got = slurp(out);
        if (rc == 0 && got == exp && h.other == 0) {
            ++pass;
        } else {
            std::printf("FAIL %s: rc %d, other groups %d, %s\n", p.string().c_str(), rc, h.other,
                        first_diff(got, exp).c_str());
        }
    }
    std::remove(out.c_str());
    std::fclose(log);
    std::strcpy(def::model::g_version, "1.0");
    std::printf("equipment files: %d/%d (%d from type-5 objects)\n", pass, n, type5);
    CHECK_EQ(pass, n);
}

// comment block and UNIT line of 1401c5ea0, and its status codes
TEST_CASE(material_file_comment_unit_and_status)
{
    keytest::legacy_printf();
    // one material, written by hand in KEY form (MTNAME is enough for a block)
    auto m = std::make_unique<DB_STEP_DATA>(3);
    Hooks h(*m);
    std::FILE* log = std::fopen(keytest::tmp_name("item_writers_unit.log").c_str(), "w");
    REQUIRE(read_key("MTNAME       4\nSTEEL\n", h, log) == 0);
    std::fclose(log);
    REQUIRE(m->materials.head != nullptr);
    const std::string out = keytest::tmp_name("item_writers_unit.KEY");
    // the block alone (comment null, no UNIT line), to compare the prefixes against
    REQUIRE(def::key::write_material_key_file(*m, out.c_str(), 4, false, nullptr, nullptr) == 0);
    const std::string plain = slurp(out);
    const std::string banner = crlf("*\n*  DEFORM MATERIAL KEYWORD FILE\n*\n");
    REQUIRE(plain.compare(0, banner.size(), banner) == 0);
    const std::string block = plain.substr(banner.size());

    at<std::int32_t>(m.get(), 0x84c) = 2;
    CHECK_EQ(def::key::write_material_key_file(*m, out.c_str(), 4, true, "line one\nline two", nullptr), 0);
    // putc('*'), the text with "\n" -> "\n*", then "\n\n"; the banner; "%-6s %7i\n" UNIT
    CHECK_EQ(slurp(out), crlf("*line one\n*line two\n\n") + banner + crlf("UNIT         2\n") + block);

    CHECK_EQ(def::key::write_material_key_file(*m, nullptr, 4, false, nullptr, nullptr), 1);
    CHECK_EQ(def::key::write_material_key_file(*m, "", 4, false, nullptr, nullptr), 1);
    CHECK_EQ(def::key::write_material_key_file(*m, out.c_str(), 9, false, nullptr, nullptr), 3);
    std::remove(out.c_str());
}

namespace {
std::string capture(const char* base, int* rc, int (*fn)(DB_STEP_DATA&, std::FILE*, const LLOBJ_DATA*),
                    DB_STEP_DATA& m, const LLOBJ_DATA* o)
{
    keytest::OutFile f(base);
    *rc = fn(m, f.get(), o);
    return f.text();
}
} // namespace

TEST_CASE(kw97_block_cases)
{
    keytest::legacy_printf();
    auto m = std::make_unique<DB_STEP_DATA>(3);
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(m.get(), 3);
    REQUIRE(o != nullptr);
    at<std::int32_t>(o, 0x21c8) = 2;
    at<double>(o, 0x21b0) = 1.0;
    at<double>(o, 0x21b8) = 2.0;
    at<double>(o, 0x21c0) = -3.0;
    at<double>(o, 0x2350) = 4.0;
    at<double>(o, 0x2358) = 5.0;
    at<double>(o, 0x2348) = 6.0;
    at<std::int32_t>(o, 0x2360) = 7;
    at<std::int32_t>(o, 0x236c) = 0;
    int rc = -1;
    std::string s = capture("item_writers_97.tmp", &rc, def::key::write_kw97_block, *m, o);
    CHECK_EQ(rc, 0);
    CHECK_EQ(s, std::string("MOVCTL       3       2    1.0000000000E+000    2.0000000000E+000   -3.0000000000E+000\n"
                            "    4.0000000000E+000    5.0000000000E+000    6.0000000000E+000       7\n"));
    at<std::int32_t>(o, 0x236c) = 1;  // sub-mode 1 needs +0x2378 == 5
    s = capture("item_writers_97.tmp", &rc, def::key::write_kw97_block, *m, o);
    CHECK_EQ(rc, 3);
    CHECK_EQ(s, std::string());
    at<std::int32_t>(o, 0x236c) = 2;  // no case: nothing, status 0
    s = capture("item_writers_97.tmp", &rc, def::key::write_kw97_block, *m, o);
    CHECK_EQ(rc, 0);
    CHECK_EQ(s, std::string());
    at<std::int32_t>(o, 0x21c8) = 1;  // movement type not 2
    s = capture("item_writers_97.tmp", &rc, def::key::write_kw97_block, *m, o);
    CHECK_EQ(rc, 3);
    CHECK_EQ(s, std::string());
    // 2D: the angle atan2(+0x21b8, +0x21b0) in degrees: atan2(0, -2) = pi -> 180
    m->dimension = 2;
    at<std::int32_t>(o, 0x21c8) = 2;
    at<std::int32_t>(o, 0x236c) = 0;
    at<double>(o, 0x21b0) = -2.0;
    at<double>(o, 0x21b8) = 0.0;
    s = capture("item_writers_97.tmp", &rc, def::key::write_kw97_block, *m, o);
    CHECK_EQ(rc, 0);
    CHECK_EQ(s, std::string("MOVCTL       3       2    1.8000000000E+002    4.0000000000E+000    5.0000000000E+000"
                            "    6.0000000000E+000       7\n"));
}

TEST_CASE(kw2d_2c_lines)
{
    keytest::legacy_printf();
    auto m = std::make_unique<DB_STEP_DATA>(3);
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(m.get(), 1);
    REQUIRE(o != nullptr);
    at<double>(m.get(), 0x900) = 1.0;
    at<double>(m.get(), 0x908) = 2.0;
    at<double>(m.get(), 0x910) = 3.0;
    at<double>(m.get(), 0x8e8) = 0.5;
    at<double>(m.get(), 0x8f0) = 0.25;
    at<double>(m.get(), 0x8f8) = -1.0;
    int rc = -1;
    at<std::int32_t>(o, 0x21c8) = 1;
    CHECK_EQ(capture("item_writers_2d.tmp", &rc, def::key::write_kw2d_2c, *m, o), std::string());
    CHECK_EQ(rc, 0);
    at<std::int32_t>(o, 0x21c8) = 4;
    CHECK_EQ(capture("item_writers_2d.tmp", &rc, def::key::write_kw2d_2c, *m, o),
             std::string("LMAX      1.0000000000E+000    2.0000000000E+000    3.0000000000E+000\n"));
    at<std::int32_t>(o, 0x21c8) = 9;
    CHECK_EQ(capture("item_writers_2d.tmp", &rc, def::key::write_kw2d_2c, *m, o),
             std::string("LMAX      1.0000000000E+000    2.0000000000E+000    3.0000000000E+000\n"
                         "VMIN      5.0000000000E-001    2.5000000000E-001   -1.0000000000E+000\n"));
    m->dimension = 2;
    CHECK_EQ(capture("item_writers_2d.tmp", &rc, def::key::write_kw2d_2c, *m, o),
             std::string("LMAX      1.0000000000E+000    2.0000000000E+000\n"
                         "VMIN      5.0000000000E-001    2.5000000000E-001\n"));
    CHECK_EQ(rc, 0);
}

TEST_CASE(equipment_file_open_failure)
{
    auto m = std::make_unique<DB_STEP_DATA>(3);
    CHECK_EQ(def::key::write_equipment_key_file(*m, "Z:/no/such/dir/x.KEY", 1), 3);
}

TEST_CASE(set_model_unit_resets_only_on_change)
{
    auto m = std::make_unique<DB_STEP_DATA>(3);
    m->sim_ctrl.unit = 0;
    def::key::set_model_unit(*m, 2, true);  // change + reset: the second constant set (1401f4100)
    CHECK_EQ(m->sim_ctrl.unit, 2);
    CHECK_EQ(m->sim_ctrl.envtmp.value, 68.0);
    CHECK_EQ(m->sim_ctrl.unte2h, 0.107);
    m->sim_ctrl.envtmp.value = 5.0;
    def::key::set_model_unit(*m, 2, true);  // no change: no reset
    CHECK_EQ(m->sim_ctrl.envtmp.value, 5.0);
    def::key::set_model_unit(*m, 1, false);  // change without reset
    CHECK_EQ(m->sim_ctrl.unit, 1);
    CHECK_EQ(m->sim_ctrl.envtmp.value, 5.0);
    def::key::set_model_unit(*m, 0, true);
    def::key::set_model_unit(*m, 1, true);
    CHECK_EQ(m->sim_ctrl.envtmp.value, 20.0);
    CHECK_EQ(m->sim_ctrl.unte2h, 1.0);
}

int main() { return testkit::run_all(); }
