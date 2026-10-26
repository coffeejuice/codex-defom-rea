// Tier 1 of the menu shell pieces that need no console (packet p5-01). Expected values from the
// decompiles of 140006420, 14001c770, 140224d80 (deform-rea-work/out/p510/p5.c).
// ctest-labels: p5-01 shell
#include <def_pre/menu_shell.hpp>
#include <defmodel/layout/cls_14001c770.hpp>
#include <defmodel/program_info.hpp>
#include <testkit/check.hpp>

#include <cstdlib>
#include <cstring>
#include <memory>
#include <new>

using def::pre::select_accepts;

TEST_CASE(selection_rules)
{
    const std::int32_t r[2] = {1, 9};
    for (const char* s : {"e", "E", "r", "R", "/", "", "abc", "1", "9", " 5", "0"})
        CHECK(select_accepts(r, s, false, 1, 9));
    for (const char* s : {"10", "-1", "12", "99x"}) CHECK(!select_accepts(r, s, false, 1, 9));
    CHECK(!select_accepts(r, "1", true, 0, 0)); // menu 9: only 0 and non-numbers
}

TEST_CASE(session_object_defaults)
{
    void* p = std::calloc(1, sizeof(def::model::Cls_14001c770));
    auto* m = new (p) def::model::Cls_14001c770();
    CHECK_EQ(m->u000, 1);
    CHECK_EQ(m->u004, 9);
    CHECK_EQ(m->u170, 1);
    CHECK_EQ(m->u174, 9);
    CHECK_EQ(m->u188, 1);
    CHECK_EQ(m->u18c, 10);
    CHECK(m->step_a.db == &m->db_a);
    CHECK(m->step_b.db == &m->db_b);
    CHECK(m->step_a.companion == &m->step_b);
    CHECK_EQ(std::strcmp(m->step_a.u340, "DEFORM"), 0);
    m->~Cls_14001c770();
    std::free(p);
}

TEST_CASE(program_info)
{
    def::model::reset_program_info();
    CHECK_EQ(std::strcmp(def::model::version(), "1.0"), 0);
    CHECK_EQ(def::model::info_dim(), 2);
    CHECK_EQ(std::strcmp(def::model::product_name_for_dim(2), "DEFORM-2D"), 0);
    CHECK_EQ(std::strcmp(def::model::product_name_for_dim(3), "DEFORM-3D"), 0);
    char buf[100];
    def::pre::format100(buf, "%d", 42);
    CHECK_EQ(std::strcmp(buf, "42"), 0);
}

int main() { return testkit::run_all(); }
