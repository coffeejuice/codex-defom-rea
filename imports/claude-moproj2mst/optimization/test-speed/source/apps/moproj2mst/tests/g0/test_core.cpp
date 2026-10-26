// G0: pieces of the core that do not depend on a packet (String basics, state, stubs).
#include <testkit/check.hpp>

#include "m2m/common.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/state.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/qt4.hpp"

using m2m::qt::String;

TEST_CASE(string_null_and_empty)
{
    String a;
    CHECK(a.isNull());
    CHECK(a.isEmpty());
    String b = m2m::qt::L("");
    CHECK(!b.isNull());
    CHECK(b.isEmpty());
    CHECK(a == b);                       // QString == compares content
    CHECK((a + a).isNull());             // null only when both are null
    CHECK(!(a + b).isNull());
    CHECK((m2m::qt::L("ab") + String()).utf8() == "ab");
}

TEST_CASE(latin1_and_utf8)
{
    String s = String::latin1(std::string_view("\xe4x", 2));
    CHECK_EQ(s.size(), 2);
    CHECK(s.at(0) == u'\x00e4');
    CHECK(s.utf8() == "\xc3\xa4x");
    CHECK(String::latin1(static_cast<const char*>(nullptr)).isNull());
}

TEST_CASE(objpos_offsets)
{
    m2m::ObjPos p;
    p.d(0x10) = 1.0;
    p.d(0xc8) = 2.0;
    CHECK(p.dbl[0] == 1.0);
    CHECK(p.dbl[23] == 2.0);
}

TEST_CASE(contact_block_defaults)
{
    m2m::ContactBlock c;
    CHECK_EQ(int(c.flag[0]), 1);
    CHECK_EQ(int(c.flag[1]), 0);
    CHECK_EQ(int(c.enable), 1);
    CHECK_EQ(int(c.gen), 1);
}

TEST_CASE(state_cycle_mode)
{
    m2m::mst::State st;
    CHECK(!st.cycleMode);
    st.setCycleMode(3, m2m::qt::L("Cycle"));
    CHECK(st.cycleMode);
    CHECK_EQ(st.cycleCursim, 3);
    st.clearCycleMode();
    CHECK(!st.cycleMode);
}

TEST_CASE(stub_throws_not_ported)
{
    bool thrown = false;
    try {
        m2m::not_ported(0x140131680, "MST header");
    } catch (const m2m::NotPorted& e) {
        thrown = true;
        CHECK_EQ(e.code, M2M_E_UNSUPPORTED);
        CHECK(e.src == 0x140131680u);
        CHECK(std::string(e.what()).find("0x140131680") != std::string::npos);
    }
    CHECK(thrown);
}

int main()
{
    return testkit::run_all();
}
