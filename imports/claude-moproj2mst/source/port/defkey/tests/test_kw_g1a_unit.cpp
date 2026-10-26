// ctest-labels: kw-g1a keywords
// Tier-1 checks of group 1 behaviour that a KFREAD/KFWRIT round trip cannot reach (read and
// write option bits, the writer's abort code, model side effects). Expected values are read off
// the decompiles of FUN_14039aa90, FUN_1403d4990 and the per-keyword writers
// (docs/re/keywords/<KW>.md), not from running the port.
#include "kw_g1a_support.hpp"

#include <bit>

using namespace kwg1a_test;
namespace g1a = def::key::g1a;

namespace {

std::string line_of(const std::string& text, const char* kw)
{
    std::string key = std::string(kw);
    std::size_t p = text.find("\n" + key);
    if (p == std::string::npos) return "";
    std::size_t e = text.find('\n', p + 1);
    return text.substr(p + 1, e - p - 1);
}

} // namespace

TEST_CASE(write_option_bits)
{
    Model mdl(3);
    auto& s = mdl.m->sim_ctrl;
    s.tnow1 = 2.0;
    s.tnow2 = 3.0;
    s.tnow3 = 4;
    s.tnow4 = 5.0;
    // bit 1: no NSTART; bit 4: no MESHNO; bit 8: TNOW 1, 2 replaced by -1e21 and 0
    mdl.m->u1744 = 1 | 4 | 8;
    std::string out = write_sections(mdl.m);
    CHECK_EQ(line_of(out, "NSTART"), std::string(""));
    CHECK_EQ(line_of(out, "MESHNO"), std::string(""));
    CHECK_EQ(line_of(out, "TNOW"),
             std::string("TNOW     -1.0000000000E+021    0.0000000000E+000       4    5.0000000000E+000"));
    mdl.m->u1744 = 0;
    out = write_sections(mdl.m);
    CHECK_EQ(line_of(out, "NSTART"), std::string("NSTART      -1"));
    CHECK_EQ(line_of(out, "MESHNO"), std::string("MESHNO       1"));
    CHECK_EQ(line_of(out, "TNOW"),
             std::string("TNOW      2.0000000000E+000    3.0000000000E+000       4    5.0000000000E+000"));
}

TEST_CASE(read_option_bits)
{
    Model mdl(3);
    auto& s = mdl.m->sim_ctrl;
    read_text(mdl, "NSTART 7\nMESHNO 9\n");
    CHECK_EQ(s.nstart, 7);
    CHECK_EQ(static_cast<int>(mdl.m->u2f88), 1);  // "NSTART given"
    CHECK_EQ(s.meshno, 9);

    Model other(3);
    other.m->u1740 = 2 | 4;  // MESHNO and NSTART go to a dummy
    read_text(other, "NSTART 7\nMESHNO 9\n");
    CHECK_EQ(other.m->sim_ctrl.nstart, -1);
    CHECK_EQ(static_cast<int>(other.m->u2f88), 0);
    CHECK_EQ(other.m->sim_ctrl.meshno, 1);

    Model failed(3);
    std::string msgs = read_text(failed, "NSTART\n");
    CHECK_EQ(msgs, std::string("ERROR: Keyword NSTART Reading Error. \n"));
    CHECK_EQ(static_cast<int>(failed.m->u2f88), 0);  // set only on success
    CHECK_EQ(failed.m->sim_ctrl.nstart, 0);           // the helper zeroed it
}

TEST_CASE(unit_change_resets_environment_defaults)
{
    Model mdl(3);
    auto& s = mdl.m->sim_ctrl;
    read_text(mdl, "UNIT 1\n");  // 0 -> 1: applied (FUN_1401f4100 tests == 1)
    CHECK_EQ(s.unit, 1);
    CHECK_BITS(s.unte2h, 1.0);
    CHECK_BITS(s.blzman, std::bit_cast<double>(0x3dcf2a6880c4f0aaULL));  // DAT_14052e8b0 5.669e-11
    CHECK_BITS(s.envtmp.value, 20.0);
    CHECK_BITS(s.cnvcof.value, std::bit_cast<double>(0x3f947ae147ae147bULL));
    CHECK_BITS(s.envatm.value, std::bit_cast<double>(0x3ffb0a3d70a3d70aULL));
    CHECK_BITS(s.acvcof.value, std::bit_cast<double>(0x3f30624dd2f1a9fcULL));
    CHECK_BITS(s.envmpr, std::bit_cast<double>(0x3e15a589e1f37f03ULL));
    CHECK_BITS(s.envmpt, std::bit_cast<double>(0x3d03d0dac864deb1ULL));
    read_text(mdl, "UNIT 2\n");
    CHECK_EQ(s.unit, 2);
    CHECK_BITS(s.unte2h, std::bit_cast<double>(0x3fbb645a1cac0831ULL));  // DAT_14052e8c8 0.107
    CHECK_BITS(s.blzman, std::bit_cast<double>(0x3cedc71df2f4abbdULL));
    CHECK_BITS(s.envtmp.value, 68.0);
    CHECK_BITS(s.cnvcof.value, std::bit_cast<double>(0x3ee025e7f1158172ULL));
    CHECK_BITS(s.envatm.value, std::bit_cast<double>(0x3ffb0a3d70a3d70aULL));
    CHECK_BITS(s.acvcof.value, std::bit_cast<double>(0x3ee4f8b588e368f1ULL));
    CHECK_BITS(s.envmpr, std::bit_cast<double>(0x3e61204d7839eab6ULL));
    CHECK_BITS(s.envmpt, std::bit_cast<double>(0x3d4faa7ab552a552ULL));
    s.envtmp.value = 99.0;
    read_text(mdl, "UNIT 2\nUNIT 3\nUNIT 0\n");  // same value or not 1/2: ignored
    CHECK_EQ(s.unit, 2);
    CHECK_BITS(s.envtmp.value, 99.0);
}

TEST_CASE(smode_sets_trans_and_writer_recomputes)
{
    Model mdl(3);
    auto& s = mdl.m->sim_ctrl;
    read_text(mdl, "SMODE 3\n");
    CHECK_EQ(s.smode, 3);
    CHECK_EQ(s.trans[0], 1);
    CHECK_EQ(s.trans[1], 1);
    read_text(mdl, "SMODE 2\n");
    CHECK_EQ(s.trans[0], 1);
    CHECK_EQ(s.trans[1], 0);
    read_text(mdl, "SMODE 9\n");  // other values: only SMODE changes
    CHECK_EQ(s.smode, 9);
    CHECK_EQ(s.trans[0], 1);
    CHECK_EQ(s.trans[1], 0);
    std::string out = write_sections(mdl.m);
    CHECK_EQ(line_of(out, "SMODE"), std::string("SMODE        2"));
    CHECK_EQ(s.smode, 2);  // the writer stores the recomputed value
}

TEST_CASE(dttype_with_unsupported_ndim_aborts_the_write)
{
    Model mdl(3);
    mdl.m->sim_ctrl.dttype.ndim = 2;
    int rc = -1;
    std::string out = write_sections(mdl.m, &rc);
    CHECK_EQ(rc, 3);
    // Everything up to DEMAX is written, nothing after it.
    std::size_t p = out.rfind("DEMAX ");
    REQUIRE(p != std::string::npos);
    CHECK_EQ(out.substr(p), std::string("DEMAX     0.0000000000E+000\n"));
    // The shell passes the code on.
    std::FILE* f = std::tmpfile();
    CHECK_EQ(def::key::write_group1(mdl.m, f), 3);
    std::fclose(f);
}

TEST_CASE(dttype_header_codes)
{
    // var 1/2 with flag 0 -> 1/2, with flag 1 -> 3/4; any other var: no header line
    struct Row {
        int var, flag;
        const char* header;
    } rows[] = {{1, 0, "DTTYPE       1       0\n"}, {2, 0, "DTTYPE       2       0\n"},
                {1, 1, "DTTYPE       3       0\n"}, {2, 1, "DTTYPE       4       0\n"},
                {5, 0, ""},                         {5, 1, ""}};
    for (const Row& r : rows) {
        Model mdl(3);
        auto& t = mdl.m->sim_ctrl.dttype;
        t.ndim = 1;
        t.var[0] = r.var;
        t.u58 = r.flag;
        std::FILE* f = std::tmpfile();
        CHECK_EQ(g1a::write_dttype(f, mdl.m, {}), 0);
        CHECK_EQ(slurp(f), std::string(r.header));  // n = 0: the table writer writes nothing
        std::fclose(f);
    }
}

TEST_CASE(dttype_registry_accessor)
{
    Model mdl(3);
    def::model::DYNARY* out = nullptr;
    CHECK_EQ(g1a::dttype_table(mdl.m, nullptr, {}, &out), 0);
    CHECK(out == &mdl.m->sim_ctrl.dttype);
}

TEST_CASE(dsmax_nstep_needs_object_count_and_value)
{
    Model mdl(3);
    auto& s = mdl.m->sim_ctrl;
    s.dsmax = 7.0;
    read_text(mdl, "DSMAX_NSTEP 10 4\n");  // no object with id PDIE[0] = 0
    CHECK_BITS(s.dsmax, 7.0);
    mdl.add_object(0, -2.5);
    read_text(mdl, "DSMAX_NSTEP 0 4\nDSMAX_NSTEP 10 0\n");  // d == 0 or n == 0: no change
    CHECK_BITS(s.dsmax, 7.0);
    std::string msgs = read_text(mdl, "DSMAX_NSTEP 10 4\nDSMAX_NSTEP\n");  // never an error
    CHECK_BITS(s.dsmax, (10.0 - 2.5) / 4.0);
    CHECK_EQ(msgs, std::string(""));
}

TEST_CASE(layout_rebuild_hook_calls)
{
    static int calls = 0;
    calls = 0;
    g1a::model_hooks().rebuild_state_layout = [](DB_STEP_DATA*, int) { ++calls; };
    Model m3(3);
    read_text(m3, "GEOTYP 2\nSTNOUT 1\nELMNOD 0\n\n");
    CHECK_EQ(calls, 2);
    Model m2(2);
    read_text(m2, "GEOTYP 2\nSTNOUT 1\nELMNOD 0\n\n");
    CHECK_EQ(calls, 5);
    CHECK_EQ(m2.m->sim_ctrl.geotyp, 2);
    g1a::model_hooks().rebuild_state_layout = nullptr;
}

TEST_CASE(shell_dispatch)
{
    Model mdl(3);
    std::FILE* f = std::tmpfile();
    CHECK_EQ(def::key::read_group1(mdl.m, f, 21), 2);   // below TITLE: default case
    CHECK_EQ(def::key::read_group1(mdl.m, f, 92), 2);   // above FRQNCY: default case
    if (!def::key::group1_has_g1b()) CHECK_EQ(def::key::read_group1(mdl.m, f, 61), 2);
    std::fclose(f);
}

TEST_CASE(tnow_int_field_truncates_like_cvttsd2si)
{
    Model mdl(3);
    auto& s = mdl.m->sim_ctrl;
    read_text(mdl, "TNOW 0 0 -2.9 0\n");
    CHECK_EQ(s.tnow3, -2);
    read_text(mdl, "TNOW 0 0 3e10 0\n");  // out of int range: 0x80000000
    CHECK_EQ(s.tnow3, static_cast<std::int32_t>(0x80000000u));
}

int main() { return testkit::run_all(); }
