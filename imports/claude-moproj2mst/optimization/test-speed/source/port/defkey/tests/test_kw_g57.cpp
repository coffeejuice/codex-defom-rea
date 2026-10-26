// ctest-labels: kw-g57
// Tier-1 tests of the group 5 (phase transformation) reader FUN_140383120, the group 7 DIM reader
// FUN_14037e530 and the phase-pair writer FUN_1403cbf00.
//
// Golden files (tests/data/kw_g57): g57_*.KEY are hand-written inputs; g57_*.expected is the
// "Inter-Material Data" section of the original DEF_PRE's KFWRIT after KFREAD of that input
// (oracle cache, CMD.KEY "KFREAD / in.KEY / KFWRIT / out.KEY"; CR removed). KFWRIT is itself a
// command keyword, so it writes the model DIM selected last. Other expected values are worked out
// from the decompiled code in the comments next to them.
#include "kw_g57_test_util.hpp"

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/phase/phase_ops.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <testkit/check.hpp>

#include <string>
#include <vector>

using namespace def::key::g57;
using def::model::keyword_target_model;
using def::model::phase_pair_count;
using def::model::phase_pair_find;
using g57test::data_path;
using g57test::ModelPair;
using g57test::read_file;
using g57test::read_into;
using g57test::strip_cr;
using g57test::write_section;

namespace {

std::string round_trip_file(const char* name, const char* tag, int* read_status)
{
    ModelPair models;
    std::string msgs;
    *read_status = read_into(models, read_file(data_path(name)), tag, &msgs);
    CHECK_EQ(msgs, std::string(""));
    return write_section(keyword_target_model(models.primary.get()), tag);
}

} // namespace

// ---- oracle round trips -------------------------------------------------------------------------

// Every TTTD type (1 with subs 0/2, 2, 3, 4, 5, 6, 7, 8, 9 with nc 1 and 2, 10, 13, negative,
// ignored types 0 and 14), PHASLH/PHASVL/IPSNRG t 0..3, TRNSFP on/off, PHEQVF with PHASTF 2 and
// PHASTF 1 (dropped), COARSN, VARSEL, a custom TTTN name (not written), a switched-off pair.
TEST_CASE(oracle_round_trip_3d)
{
    int st = -1;
    const std::string out = round_trip_file("g57_3d.KEY", "g57rt", &st);
    CHECK_EQ(st, 0);
    CHECK_EQ(out, strip_cr(read_file(data_path("g57_3d.expected"))));
}

// DIM 2 routes to the 2D companion, DIM 3 back; KFWRIT then writes the primary (a) or, after a
// final DIM 2 (b), the companion. DIM 7 is ignored.
TEST_CASE(oracle_round_trip_dim)
{
    int st = -1;
    CHECK_EQ(round_trip_file("g57_dim_a.KEY", "g57da", &st), strip_cr(read_file(data_path("g57_dim_a.expected"))));
    CHECK_EQ(st, 0);
    CHECK_EQ(round_trip_file("g57_dim_b.KEY", "g57db", &st), strip_cr(read_file(data_path("g57_dim_b.expected"))));
    CHECK_EQ(st, 0);
}

// S45C-JAPAN (materials/JIS-S45C(HeatTreatment)_s000006.KEY, phases 1-3 and mixture 4): the
// input is the file's comment lines and group 5 records (a copy made for the test); the expected
// text is the oracle's KFWRIT of the whole corpus file (cached kfrw round trip).
TEST_CASE(s45c_japan_round_trip)
{
    int st = -1;
    const std::string out = round_trip_file("s45c_japan_s000006.g57.KEY", "g57s45", &st);
    CHECK_EQ(st, 0);
    CHECK_EQ(out, strip_cr(read_file(data_path("s45c_japan_s000006.expected"))));
}

// ---- reader details (from the decompile of 140383120 / 14037e530) ------------------------------

TEST_CASE(phastf_counts_and_hook)
{
    static int calls = 0;
    static int last_flag = -1;
    def::model::g_hook_1401d0fc0 = [](DB_STEP_DATA*, std::int32_t flag) {
        ++calls;
        last_flag = flag;
    };
    ModelPair mp;
    read_into(mp, "PHASTF 1 2 1\nPHASTF 1 3 2\nPHASTF 1 2 0\n", "g57ph", nullptr);
    DB_STEP_DATA* m = mp.primary.get();
    CHECK_EQ(phase_pair_count(&m->phase_pairs), 2);
    // 1401d6450 runs only for a non-zero value, so switching 1->2 off leaves the count at 2
    CHECK_EQ(m->n_phase_on, 2);
    CHECK_EQ(calls, 2);
    CHECK_EQ(last_flag, 0);
    CHECK_EQ(phase_pair_find(&m->phase_pairs, 1, 2)->phastf_on, 0);
    def::model::g_hook_1401d0fc0 = nullptr;
}

TEST_CASE(blank_value_is_stored_and_reported)
{
    ModelPair mp;
    std::string msgs;
    const int st = read_into(mp, "PHASTF 1 2 5\nPHASTF 1 2\n\nVARSEL 1 2 4\nVARSEL 1 2\n\n", "g57bl", &msgs);
    CHECK_EQ(st, 2);
    const auto* p = phase_pair_find(&mp.primary.get()->phase_pairs, 1, 2);
    CHECK_EQ(p->phastf, 0); // the blank remainder stored 0 before the parser returned 2
    CHECK_EQ(p->varsel, 0);
    CHECK_EQ(msgs, std::string("ERROR: Keyword PHASTF Reading Error. \nERROR: Keyword VARSEL Reading Error. \n"));
}

TEST_CASE(tttd_type2_bad_count_is_data_error)
{
    ModelPair mp;
    std::string msgs;
    const int st = read_into(mp, "TTTD 1 2 2 0\n 3 1 1 1\n", "g57t2", &msgs);
    CHECK_EQ(st, 3);
    const auto* p = phase_pair_find(&mp.primary.get()->phase_pairs, 1, 2);
    CHECK_EQ(p->tttd_type, 4);      // set before the check
    CHECK_EQ(p->tttd_t2.n[0], 1);   // counts stored before the check
    CHECK_EQ(p->tttd_t2.ncomp, 2);  // template 0x30 gives 2
    CHECK_EQ(msgs, std::string("ERROR: Keyword TTTD Data Error. \n"));
}

TEST_CASE(tttd_unknown_sub_keeps_kind)
{
    ModelPair mp;
    read_into(mp, "TTTD 1 2 5 0\n 1 2 3 4 5 6\nTTTD 1 2 1 9\n 7 1 0\n 0.5 0.6\n 0 0 0\n", "g57ts", nullptr);
    const auto* p = phase_pair_find(&mp.primary.get()->phase_pairs, 1, 2);
    CHECK_EQ(p->tttd_type, 7);      // sub 7 is not 0..3: the type-5 kind stays selected
    CHECK_EQ(p->tttd_kind, 0);      // kind 9 is not stored
    CHECK_EQ(p->tttd_nc, 1);
    CHECK_BITS(p->tttd_f1, 0.5);    // nc 1: f1 at +0x70, f2 at +0x170
    CHECK_BITS(p->tttd_f1b, 0.6);
    CHECK_BITS(p->tttd_c5[5], 6.0);
}

TEST_CASE(trnsfp_keeps_stale_shape)
{
    ModelPair mp;
    read_into(mp, "TRNSFP 1 2 1 3 2 2\n 1 2\n 3 4\n 5 6 7 8\nTRNSFP 1 2 1 1 2\n 10 1\n 20 2\n", "g57tr", nullptr);
    const auto* p = phase_pair_find(&mp.primary.get()->phase_pairs, 1, 2);
    CHECK_EQ(p->trnsfp.ndim, 1);
    CHECK_EQ(p->trnsfp.var[1], 0xc); // left from the t 3 table (no re-init)
    CHECK_EQ(p->trnsfp.n[1], 2);
}

TEST_CASE(dim_without_companion_does_nothing)
{
    g57test::FakeModel solo(3);
    keytest::InFile in("g57dn_in.txt", "DIM 2\nPHASTF 1 2 1\n");
    g57test::Hooks hooks(solo.get());
    CHECK_EQ(def::key::read_key_stream(in.get(), hooks, nullptr == nullptr ? stdout : stdout), 0);
    CHECK_EQ(solo.get()->use_companion, 0);
    CHECK_EQ(phase_pair_count(&solo.get()->phase_pairs), 1);
}

// ---- writer details -----------------------------------------------------------------------------

TEST_CASE(write_switched_off_pair_and_bad_kind)
{
    ModelPair mp;
    read_into(mp, "PHASTF 1 2 0\nPHASTF 3 4 1\n", "g57wo", nullptr);
    DB_STEP_DATA* m = mp.primary.get();
    // a pair that is off writes nothing; an unknown TTTD kind (only reachable from a DB) aborts
    // with 3 right after PHASTF
    phase_pair_find(&m->phase_pairs, 3, 4)->tttd_type = 0x11;
    int st = 0;
    const std::string out = write_section(m, "g57wo", &st);
    CHECK_EQ(st, 3);
    CHECK_EQ(out, std::string("*\n*  Inter-Material Data\n*\n"
                              "*\n*  Inter-Material Data for     3 and 4\n*\n"
                              "TTTN         3       4\n"
                              "Transformation from 3 -> 4\n"
                              "PHASTF       3       4       1\n"));
}

TEST_CASE(accessors)
{
    ModelPair mp;
    read_into(mp, "PHASTF 1 2 1\nPHASTF 3 4 1\n", "g57ac", nullptr);
    DB_STEP_DATA* m = mp.primary.get();
    DYNARY* t = nullptr;
    CHECK_EQ(access_tttd(m, "Phase Compositions", {2}, &t), 0);
    CHECK(t == &phase_pair_find(&m->phase_pairs, 3, 4)->tttd_t9);
    CHECK_EQ(access_tttd(m, "Log(Time)", {1}, &t), 0);
    CHECK(t == &phase_pair_find(&m->phase_pairs, 1, 2)->tttd_t1);
    CHECK_EQ(access_tttd(m, "nope", {1}, &t), 3);
    CHECK_EQ(access_tttd(m, "Log(Time)", {9}, &t), 3); // no pair 9
    CHECK_EQ(access_pheqvf(m, "", {2}, &t), 0);
    CHECK(t == &phase_pair_find(&m->phase_pairs, 3, 4)->pheqvf);
    DYNARY* none = nullptr;
    CHECK_EQ(access_phaslh(m, "", {9}, &none), 0);
    CHECK(none == nullptr);
}

int main() { return testkit::run_all(); }
