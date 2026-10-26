// Tier-1: the COMMON at 140b84720..140b84730 is one object. defrt owns it
// (def::rt::geoutl::g_common_140b84720); p1-09's g_common_140b84720 and p1-10's dat_140b84720..30
// must be views of it. Two objects here once made the 2D curve kernels miss the extent that
// 140345f5b writes (the die-curve refit found by the GENDB end-to-end run).
// ctest-labels: kw-merge globals
#include <defmodel/globals_p1_09.hpp>
#include <defmodel/globals_p1_10.hpp>
#include <defrt/ftn/common_140b84720.hpp>
#include <testkit/check.hpp>

TEST_CASE(one_object_for_the_common)
{
    auto& owner = def::rt::geoutl::g_common_140b84720;
    CHECK(&def::model::g_common_140b84720 == &owner);
    CHECK(&def::model::dat_140b84720 == &owner.u00);
    CHECK(&def::model::dat_140b84728 == &owner.u08);
    CHECK(&def::model::dat_140b84730 == &owner.u10);
    // a write through one name is seen through the others
    def::model::dat_140b84730 = 48.25;
    CHECK(def::model::g_common_140b84720.u10 == 48.25);
    def::model::reset_globals_p1_09();
    CHECK(owner.u10 == 0.0);
}

int main() { return testkit::run_all(); }
