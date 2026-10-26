// ctest-labels: kw-g1a keywords
// Tier-1 round trips for keyword group 1, sections Process Definition and Stopping & Step
// Controls: each tests/data/kw_g1a/cNN_*.KEY is read into a fresh model through the group 1
// parser, the two sections are written back, and both the text and the reader's messages must
// equal what DEF_PRE produced for the same file (KFREAD / KFWRIT through the oracle, captured by
// tests/data/kw_g1a/make_expected.py into .expected and .msgs).
#include "kw_g1a_support.hpp"

using namespace kwg1a_test;

namespace {

struct Recorder {
    static inline int rebuilds = 0;
    static inline int units = 0;
    static inline int last_unit = 0;
    static void rebuild(DB_STEP_DATA*, int arg)
    {
        CHECK_EQ(arg, 0);
        ++rebuilds;
    }
    static void unit(int u)
    {
        ++units;
        last_unit = u;
    }
};

void run_case(const char* stem, void (*setup)(Model&) = nullptr)
{
    auto dir = data_dir();
    std::string key = read_file(dir / (std::string(stem) + ".KEY"));
    REQUIRE(!key.empty());
    int dim = key.find("DEFORM 2D") != std::string::npos ? 2 : 3;
    Model mdl(dim);
    if (setup != nullptr) setup(mdl);
    std::string msgs = read_text(mdl, key);
    CHECK_EQ(message_lines(msgs), read_file(dir / (std::string(stem) + ".msgs")));
    std::string out = write_sections(mdl.m);
    std::string expected = read_file(dir / (std::string(stem) + ".expected"));
    CHECK_EQ(out, expected);
}

void install_recorders()
{
    def::key::g1a::model_hooks().rebuild_state_layout = &Recorder::rebuild;
    def::key::g1a::model_hooks().set_unit_system = &Recorder::unit;
    Recorder::rebuilds = Recorder::units = Recorder::last_unit = 0;
}

} // namespace

TEST_CASE(c01_process_definition_3d)
{
    install_recorders();
    run_case("c01_pd3d");
    CHECK_EQ(Recorder::rebuilds, 2);  // STNOUT, ELMNOD (GEOTYP reads nothing in 3D)
    CHECK_EQ(Recorder::units, 1);
    CHECK_EQ(Recorder::last_unit, 2);
}

TEST_CASE(c02_process_definition_2d)
{
    install_recorders();
    run_case("c02_pd2d");
    CHECK_EQ(Recorder::rebuilds, 3);  // GEOTYP, STNOUT, ELMNOD
    CHECK_EQ(Recorder::units, 1);
    CHECK_EQ(Recorder::last_unit, 1);
}

TEST_CASE(c03_stopping_3d) { run_case("c03_stop3d"); }
TEST_CASE(c04_stopping_2d) { run_case("c04_stop2d"); }
TEST_CASE(c05_dsmax_then_dtmax) { run_case("c05_dsmax"); }
TEST_CASE(c06_dttype_time_table) { run_case("c06_dttype3"); }
TEST_CASE(c07_dttype_other_code) { run_case("c07_dttype9"); }
TEST_CASE(c08_dttype_back_to_none) { run_case("c08_dttype0"); }
TEST_CASE(c09_reading_errors) { run_case("c09_errors"); }
TEST_CASE(c10_dttype_short_table) { run_case("c10_dttype_eof"); }

// OBJTYP/STROKE are group 2 (not read here); the object they create, id 1 with Z stroke 3.5,
// is put in the object list by hand.
TEST_CASE(c11_dsmax_nstep_3d)
{
    run_case("c11_dsmax_nstep", [](Model& m) { m.add_object(1, 3.5); });
}

// In 2D the STROKE line sets X and Y only, so the Z stroke the formula uses stays 0
// (docs/re/keywords/DSMAX_NSTEP.md): DSMAX = 10 / 3.
TEST_CASE(c12_dsmax_nstep_2d)
{
    run_case("c12_dsmax_nstep2d", [](Model& m) { m.add_object(1, 0.0); });
}

int main() { return testkit::run_all(); }
