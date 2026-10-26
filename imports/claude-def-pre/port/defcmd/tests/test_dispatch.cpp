// Tier 1 of the command dispatcher FUN_140378e00 and the KEY loop hooks (packet p5-01).
// Expected values come from the decompile/listing of 140378e00 (out/bm_cmd.c, out/p501).
// ctest-labels: p5-01 dispatch
#include "../../defkey/tests/key_test_util.hpp"

#include <cstring>
#include <string>

#include <defcmd/dispatch.hpp>
#include <defkey/key_reader.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

using def::cmd::dispatch;
using def::cmd::dispatch_hooks;
using def::model::DB_STEP_DATA;
using def::model::field;

namespace {

std::string tmp(const char* base) { return keytest::tmp_name(base); }

int g_seen = 0;
std::string g_rest;

} // namespace

TEST_CASE(probid_reads_the_next_line)
{
    DB_STEP_DATA m(3);
    const std::string n = tmp("disp_probid.txt");
    keytest::InFile in(n.c_str(), " ignored\nMY PROBLEM  \nNEXT\n");
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kPROBID), 0);
    CHECK_EQ(std::string(m.u340), std::string("MY PROBLEM"));
    CHECK_EQ(in.rest(), std::string("NEXT\n"));
}

TEST_CASE(probid_with_flag_8_keeps_the_text)
{
    DB_STEP_DATA m(3);
    std::strcpy(m.u340, "OLD");
    m.u1740 = 8;
    const std::string n = tmp("disp_probid8.txt");
    keytest::InFile in(n.c_str(), "\nNEW\n");
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kPROBID), 0);
    CHECK_EQ(std::string(m.u340), std::string("OLD"));
}

TEST_CASE(probid_at_eof_is_a_reading_error)
{
    DB_STEP_DATA m(3);
    const std::string n = tmp("disp_probid_eof.txt");
    keytest::InFile in(n.c_str(), "\n");
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kPROBID), 2);
}

TEST_CASE(runtyp_reads_an_int)
{
    DB_STEP_DATA m(3);
    const std::string n = tmp("disp_runtyp.txt");
    keytest::InFile in(n.c_str(), " 0x3\n");
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kRUNTYP), 0);
    CHECK_EQ(m.u1738, 3);
}

TEST_CASE(ids_without_a_case_return_2)
{
    DB_STEP_DATA m(3);
    const std::string n = tmp("disp_none.txt");
    keytest::InFile in(n.c_str(), "1\n");
    for (int id : {22, 100, 153, 155, 285, 288, 289, 290, 299, 400, -1}) CHECK_EQ(dispatch(&m, in.get(), id), 2);
    CHECK_EQ(in.rest(), std::string("1\n")); // nothing read
}

TEST_CASE(tloc_adds_the_start_time)
{
    DB_STEP_DATA m(3);
    field<double>(&m, 0xd90) = 1.5;
    field<double>(&m, 0xd98) = 10.0;
    field<std::int32_t>(&m, 0xda0) = 0;
    const std::string n = tmp("disp_tloc.txt");
    keytest::InFile in(n.c_str(), " 2.25\n 1\n\n");
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kTLOC), 0);
    CHECK_EQ(field<double>(&m, 0x8c8), 3.75);
    field<std::int32_t>(&m, 0xda0) = 1;
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kTLOC), 0);
    CHECK_EQ(field<double>(&m, 0x8c8), 11.0);
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kTLOC), 2); // blank line: sscanf -1
}

TEST_CASE(actout_stores_the_line)
{
    DB_STEP_DATA m(3);
    const std::string n = tmp("disp_actout.txt");
    keytest::InFile in(n.c_str(), " 4\nsome action text\nrest\n");
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kACTOUT), 0);
    CHECK_EQ(std::string(m.text), std::string("some action text"));
    CHECK_EQ(in.rest(), std::string("rest\n"));
}

TEST_CASE(case_hooks_are_called_with_the_stream)
{
    DB_STEP_DATA m(3);
    auto& h = dispatch_hooks();
    h.objpos = [](DB_STEP_DATA*, std::FILE* f) {
        char b[64];
        std::fgets(b, sizeof b, f);
        g_rest = b;
        ++g_seen;
        return 3;
    };
    h.gfread = [](DB_STEP_DATA*, std::FILE*) { ++g_seen; return 0; };
    const std::string n = tmp("disp_hook.txt");
    keytest::InFile in(n.c_str(), " 1 2 3\n");
    g_seen = 0;
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kOBJPOS), 3); // case status passed through
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kGFREAD), 0);
    CHECK_EQ(g_seen, 2);
    CHECK_EQ(g_rest, std::string(" 1 2 3\n"));
    def::cmd::reset_dispatch_hooks();
}

TEST_CASE(kfread_refuses_the_file_being_read)
{
    DB_STEP_DATA m(3);
    std::strcpy(reinterpret_cast<char*>(&m) + 0x2f8a, "SELF.KEY");
    auto& h = dispatch_hooks();
    h.read_keyword_file = [](DB_STEP_DATA*, const char* name, const char* reset, const std::int32_t* fl) {
        g_rest = name;
        g_seen = *reset * 10 + *fl;
        return 0;
    };
    const std::string n = tmp("disp_kfread.txt");
    keytest::InFile in(n.c_str(), " 0\nSELF.KEY\n 1\nOTHER.KEY   \n");
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kKFREAD), 2);
    g_seen = -1;
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kKFREAD), 0);
    CHECK_EQ(g_rest, std::string("OTHER.KEY"));
    CHECK_EQ(g_seen, 0);
    def::cmd::reset_dispatch_hooks();
}

TEST_CASE(dbread_with_read_flag_skips)
{
    DB_STEP_DATA m(3);
    m.u1740 = 1;
    const std::string n = tmp("disp_dbread.txt");
    keytest::InFile in(n.c_str(), " 5\nA.DB\nNEXT\n");
    CHECK_EQ(dispatch(&m, in.get(), def::cmd::kDBREAD), 0);
    CHECK_EQ(in.rest(), std::string("NEXT\n"));
}

TEST_CASE(step_order)
{
    using def::cmd::db_step_compare;
    CHECK_EQ(db_step_compare(5, 5), 0);
    CHECK_EQ(db_step_compare(4, 5), -1);
    CHECK_EQ(db_step_compare(6, 5), 1);
    CHECK_EQ(db_step_compare(-5, 5), -1); // -5 sorts as 4.5
    CHECK_EQ(db_step_compare(-5, 4), 1);
    CHECK_EQ(db_step_compare(0, 1), -1);  // 0 sorts as -0.5
}

TEST_CASE(key_loop_routes_group_6_through_the_dispatcher)
{
    DB_STEP_DATA m(3);
    const std::string n = tmp("disp_loop.key");
    keytest::InFile in(n.c_str(), "PROBID\nLOOP TEST\nRUNTYP 2\nBOGUS 1\n");
    const std::string o = tmp("disp_loop.out");
    keytest::OutFile out(o.c_str());
    def::cmd::ModelKeyHooks hooks(&m);
    CHECK_EQ(def::key::read_key_stream(in.get(), hooks, out.get()), 0);
    CHECK_EQ(std::string(m.u340), std::string("LOOP TEST"));
    CHECK_EQ(m.u1738, 2);
}

TEST_CASE(kfwrit_without_name_or_file)
{
    DB_STEP_DATA m(3);
    const std::uint32_t fl = 0;
    CHECK_EQ(def::cmd::write_key_file(&m, "", &fl), 1);
    CHECK_EQ(def::cmd::write_key_file(&m, nullptr, &fl), 1);
    CHECK_EQ(def::cmd::write_key_file(&m, "Z:\\no\\such\\dir\\x.key", &fl), 1);
}

int main() { return testkit::run_all(); }
