// kw-g1b tier-1 tests: group 1 Iteration Controls, Processing Conditions and User Defined
// Variables keywords (ids 61..91), read then written back.
// ctest-labels: kw-g1b keys
//
// Expected texts: tests/data/kw_g1b/<case>.expected and .stdout are the original DEF_PRE's
// KFREAD + KFWRIT round trip of <case>.key (oracle cases in oracle_cases.json, cached under
// %USERPROFILE%\.deform-oracle), cut to the kw-g1b sections. Other expectations are worked out
// from the decompile in the test itself.
#define _CRT_SECURE_NO_WARNINGS
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

#if defined(_WIN32)
#include <io.h>
#define dup _dup
#define dup2 _dup2
#define fileno _fileno
#else
#include <unistd.h>
#endif

#include <testkit/check.hpp>

#include "kw_g1b_keyloop.hpp"

using namespace kwg1b_test;
namespace g1b = def::key::g1b;

static std::string data_path(const char* name)
{
    return (testkit::repo_root() / "port" / "defkey" / "tests" / "data" / "kw_g1b" / name).string();
}

// Runs fn with stdout redirected to a temporary file and returns what was printed (no '\r').
template <class Fn>
static std::string capture_stdout(Fn fn)
{
    std::fflush(stdout);
    int saved = dup(fileno(stdout));
    std::FILE* t = std::tmpfile();
    dup2(fileno(t), fileno(stdout));
    fn();
    std::fflush(stdout);
    dup2(saved, fileno(stdout));
    std::string out;
    for (char c : slurp(t))
        if (c != '\r')
            out.push_back(c);
    std::fclose(t);
    return out;
}

static void check_case(const char* name)
{
    std::string key = data_path((std::string(name) + ".key").c_str());
    std::string expected = read_file(data_path((std::string(name) + ".expected").c_str()), true);
    std::string expected_out = read_file(data_path((std::string(name) + ".stdout").c_str()), true);
    REQUIRE(!expected.empty());
    std::string got;
    int rc = -1;
    std::string out = capture_stdout([&] { got = round_trip(key, stdout, nullptr, &rc); });
    CHECK_EQ(rc, 0);
    CHECK_EQ(got, expected);
    CHECK_EQ(out, expected_out);
}

// ---- oracle round trips -------------------------------------------------------------------

// 3D: every table form (ENVTMP 1, CNVCOF 1, ENVATM 1, ACVCOF 2, TIMEID, ERECID with a wrapped
// row), FRQNCY 2x3 with truncated doubles, %i hex (ITRMXT 0x10), SOLMTD 2 without parameters,
// USRDEF with a blank slot, UNNAME/UENAME/CVNAME, INIGES discarded, commas as separators.
TEST_CASE(oracle_c3_tables) { check_case("c3_tables"); }

// 3D: invalid ITRMTH/SOLMTD/SOLMTT ignored with messages, SOLMTD 6 4 (last two written as ints),
// ACVCOF 3, FRNNAM + ENVTMP 10 (21 region entries, text entry 21), NRECID 0 3 (Data Error),
// USRDEF 88-char line cut at 80, UNNAME flag 2 consumes the current count of lines, a 70-char
// UENAME name split by fgets(64).
TEST_CASE(oracle_c3_misc) { check_case("c3_misc"); }

// 3D: ACVCOF 1, unknown table codes (reset only, const 0), SOLMTD 2 with no second number.
TEST_CASE(oracle_c3_acvcof1) { check_case("c3_acvcof1"); }

// 2D: UNIT 2 defaults, ITRMTH 3 -> 1, SOLMTT 2 -> 1, SOLMTD 2 1 message then SOLMTD 2 4.
TEST_CASE(oracle_c2_conv) { check_case("c2_conv"); }

// 2D defaults (empty KEY file).
TEST_CASE(oracle_c2_empty) { check_case("c2_empty"); }

// ---- return codes and consumption ---------------------------------------------------------

TEST_CASE(unknown_id_is_reading_error)
{
    Model m(3);
    CHECK_EQ(read_kw(*m, 22, " x\n"), 2);  // TITLE is kw-g1a's: the switch default here
    CHECK(!g1b::handles(60));
    CHECK(g1b::handles(61));
    CHECK(g1b::handles(91));
    CHECK(!g1b::handles(92));
}

TEST_CASE(record_tables_data_error_consumes_nothing)
{
    for (int id : {84, 85, 86}) {  // TIMEID, ERECID, NRECID
        Model m(3);
        std::string left;
        CHECK_EQ(read_kw(*m, id, " 0 3\n1 2 3 4\n", &left), 3);
        CHECK_EQ(left, std::string("1 2 3 4\n"));
        CHECK_EQ(read_kw(*m, id, " 2 -1\n", &left), 3);
    }
}

TEST_CASE(empty_line_is_reading_error)
{
    Model m(3);
    // sscanf on "\n" returns -1: the line helpers report 1 and the parser maps it to 2.
    CHECK_EQ(read_kw(*m, 61, "\n"), 2);    // CVGERR
    CHECK_EQ(read_kw(*m, 72, ""), 2);      // PENINF at EOF
    CHECK_EQ(read_kw(*m, 75, "\n"), 2);    // ENVTMP returns 2 before touching the table
    CHECK_EQ(read_kw(*m, 87, " 3\nonly one\n"), 2);  // USRDEF runs out of lines
    // A line with text but no number converts nothing yet is not EOF: status 0, value 0.
    CHECK_EQ(read_kw(*m, 72, " abc\n"), 0);
    CHECK(testkit::same_bits(m->sim_ctrl.peninf, 0.0));
}

TEST_CASE(table_short_data_is_reading_error)
{
    Model m(3);
    std::string left;
    CHECK_EQ(read_kw(*m, 78, " 1 3\n100 0.1\n", &left), 2);  // CNVCOF wants 3 rows
    CHECK_EQ(left, std::string());
}

// ---- SOLMTD quirks ------------------------------------------------------------------------

TEST_CASE(solmtd_default_message_reads_itrmxd_bytes)
{
    // 14039c06c: S4 is (int) of the 8 bytes at M+0xa60 (ITRMXD, then DEFBWD) taken as a double.
    // ITRMXD = 0, DEFBWD = 0x40000000 gives the bit pattern 0x4000000000000000 = 2.0.
    Model m(3);
    m->sim_ctrl.itrmxd = 0;
    m->sim_ctrl.defbwd = 0x40000000;
    std::string out = capture_stdout([&] { CHECK_EQ(read_kw(*m, 70, " 2\n"), 0); });
    CHECK_EQ(out, std::string("SOLMTD 2 - parameters set to default values\n"
                              "S1:2;  S2:4;  S3:1;  S4:2\n"));
    CHECK_EQ(m->sim_ctrl.solmtd, 2);
    CHECK_EQ(m->sim_ctrl.u2e8, 4);
    CHECK(testkit::same_bits(m->sim_ctrl.solmtd_p[3], 1.0));  // stored S4 is 1.0
}

TEST_CASE(solmtd_6_without_parameters_keeps_old_ones)
{
    Model m(3);
    CHECK_EQ(read_kw(*m, 70, " 6 4\n1.5 2.5 3.5 4.5\n"), 0);
    std::string out = capture_stdout([&] { CHECK_EQ(read_kw(*m, 70, " 6 3\n"), 0); });
    CHECK_EQ(out, std::string("SOLMTD 6 - missing parameters\n"));
    CHECK_EQ(m->sim_ctrl.u2e8, 0);  // count reset, parameters kept
    CHECK(testkit::same_bits(m->sim_ctrl.solmtd_p[2], 3.5));
    // The writer still says 4 and prints the kept parameters.
    std::string w = write_all(*m);
    CHECK(w.find("SOLMTD       6       4\n    1.5000000000E+000    2.5000000000E+000       3       4\n") !=
          std::string::npos);
}

// ---- writer status ------------------------------------------------------------------------

TEST_CASE(writer_stops_on_unwritable_table)
{
    // ENVTMP with a 1-D table against variable 2 cannot be written: 1403d6c90 returns 3 and the
    // KEY writer stops right there (nothing after BLZMAN).
    Model m(3);
    m->sim_ctrl.envtmp.ndim = 1;
    m->sim_ctrl.envtmp.var[0] = 2;
    int rc = 0;
    std::string w = write_all(*m, &rc);
    CHECK_EQ(rc, 3);
    const std::string tail = "BLZMAN    5.6690000000E-011\n";
    CHECK(w.size() > tail.size() && w.compare(w.size() - tail.size(), tail.size(), tail) == 0);
    CHECK(w.find("ENVTMP") == std::string::npos);
}

TEST_CASE(frqncy_not_written_when_count_mismatches)
{
    Model m(3);
    m->sim_ctrl.frqncy.ncomp = 5;  // rows * columns = 6
    std::string w = write_all(*m);
    CHECK(w.find("FRQNCY") == std::string::npos);
    CHECK(w.find("LATENT       0\n*\n*  Processing Conditions\n") != std::string::npos);
}

TEST_CASE(registry_handlers)
{
    Model m(3);
    std::FILE* f = std::tmpfile();
    CHECK_EQ(g1b::write_cvgerr(f, *m, {}), 0);
    CHECK_EQ(g1b::write_ehrate(f, *m, {1, 2}), 0);
    CHECK_EQ(slurp(f), std::string("CVGERR    5.0000000000E-003    5.0000000000E-002\n"
                                   "EHRATE    0.0000000000E+000\n"));
    std::fclose(f);

    void* out = nullptr;
    CHECK_EQ(g1b::table_envtmp(m.p, nullptr, {}, &out), 0);
    CHECK(out == &m->sim_ctrl.envtmp);
    CHECK_EQ(g1b::table_cnvcof(m.p, nullptr, {}, &out), 0);
    CHECK(out == &m->sim_ctrl.cnvcof);
    CHECK_EQ(g1b::table_envatm(m.p, nullptr, {}, &out), 0);
    CHECK(out == &m->sim_ctrl.envatm);
    CHECK_EQ(g1b::table_acvcof(m.p, nullptr, {}, &out), 0);
    CHECK(out == &m->sim_ctrl.acvcof);
    // The null-object sentinel (model address -0x750) leaves *out alone.
    out = &out;
    auto* sentinel = reinterpret_cast<def::model::DB_STEP_DATA*>(static_cast<std::uintptr_t>(-0x750));
    CHECK_EQ(g1b::table_acvcof(sentinel, nullptr, {}, &out), 0);
    CHECK(out == &out);
}

int main() { return testkit::run_all(); }
