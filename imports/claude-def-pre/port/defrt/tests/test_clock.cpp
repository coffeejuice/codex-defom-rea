#include <defrt/clock.hpp>
#include <testkit/check.hpp>

TEST_CASE(pinned_clock_is_returned)
{
    def::rt::pin_clock(1411948800); // 2014-09-29, the DEF_PRE_64 build date
    CHECK_EQ(def::rt::now(), 1411948800);
    def::rt::pin_clock(-1);
    CHECK(def::rt::now() > 1411948800);
}

int main() { return testkit::run_all(); }
