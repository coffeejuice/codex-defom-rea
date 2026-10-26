// Tier 1 for the helpers and quirks of menus 5-6 (packet p10-02): string compare 1400030d0, the
// int readers 140008a00 / 140009100, the step helpers 1401ca9e0..1401cae00, the list tag
// 140006bd0, the remesh form's store rule (140010a90) and menu 6's empty-model path. Expected
// values follow from the recovered code (docs/re/menus/MENU5_6.md).
// ctest-labels: p10-02 def_pre
#define _CRT_SECURE_NO_WARNINGS
#include <def_pre/ext_p10_01.hpp>
#include <def_pre/menu56.hpp>
#include <def_pre/menu7.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/layout/llobj_data.hpp>
#include <defmodel/llobj_ctor/llobj_ctor.hpp>
#include <defmodel/step/db_step.hpp>
#include <testkit/check.hpp>

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <fcntl.h>
#include <io.h>
#include <process.h>

using namespace def::pre;
using def::model::DB_STEP_DATA;
using def::model::field;
using def::model::LLOBJ_DATA;

namespace {

struct Input {
    std::istringstream in;
    std::streambuf* old;
    explicit Input(const std::string& s) : in(s), old(std::cin.rdbuf(in.rdbuf())) { std::cin.clear(); }
    ~Input()
    {
        std::cin.rdbuf(old);
        std::cin.clear();
    }
};

// stdout of f(), captured through a per-process temp file
template <class F>
std::string capture(F f)
{
    const auto p = std::filesystem::temp_directory_path() /
                   (std::to_string(_getpid()) + "_menu56_units.txt");
    std::fflush(stdout);
    const int saved = _dup(_fileno(stdout));
    std::FILE* t = std::fopen(p.string().c_str(), "wb");
    _dup2(_fileno(t), _fileno(stdout));
    f();
    std::fflush(stdout);
    _dup2(saved, _fileno(stdout));
    _close(saved);
    std::fclose(t);
    std::ifstream in(p, std::ios::binary);
    std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    in.close();
    std::filesystem::remove(p);
    std::string o;
    for (char c : s) if (c != '\r') o += c;
    return o;
}

// test-local selection: one line of cin, trimmed (no prompt)
std::string plain_select(MenuRec*, bool, std::int32_t, std::int32_t)
{
    char buf[0x100] = {0};
    std::cin.getline(buf, 0x100);
    return trim(buf);
}

// record area image (main+0x000..0x190)
struct Area {
    alignas(8) unsigned char b[0x190] = {};
    MenuRec* rec(std::size_t off, void* data, void* step, std::int32_t lo, std::int32_t hi)
    {
        auto* r = reinterpret_cast<MenuRec*>(b + off);
        r->lo = lo;
        r->hi = hi;
        r->data = data;
        r->step = step;
        return r;
    }
};

std::int32_t& type_of(LLOBJ_DATA* o) { return field<std::int32_t>(o, 0xc); }

} // namespace

TEST_CASE(string_compare_vs2008)
{
    CHECK_EQ(string_compare("5", 0, 1, "5", 1), 0);
    CHECK(string_compare("10", 0, 2, "1", 1) > 0);   // longer
    CHECK(string_compare("1", 0, 1, "10", 2) < 0);   // shorter: -1
    CHECK(string_compare("", 0, 0, "1", 1) < 0);
    CHECK(string_compare("11", 0, 2, "10", 2) > 0);
}

TEST_CASE(read_ints_field_rules)
{
    std::int32_t v[2] = {7, 8};
    {
        Input in("5\n");
        (void)capture([&] { read_ints(nullptr, 1, 99, v, "P ", 2, true); });
    }
    CHECK_EQ(v[0], 5); // one number fills every slot
    CHECK_EQ(v[1], 5);
    {
        Input in("3 4\n");
        (void)capture([&] { read_ints(nullptr, 1, 99, v, "P ", 2, true); });
    }
    CHECK_EQ(v[0], 5); // "3 " keeps its blank: not digits, nothing stored
    {
        Input in("0\nx\n");
        const std::string out = capture([&] { read_ints(nullptr, 1, 99, v, "P ", 2, true); });
        CHECK_EQ(out, std::string("P  ??? Invalid Data; Valid Data Range = (     1 to      99)\nP "));
    }
}

TEST_CASE(read_int_pair_range)
{
    std::int32_t lo = 1, hi = 10;
    {
        Input in("3\n");
        (void)capture([&] { read_int_pair(nullptr, &lo, &hi, "R ", true); });
    }
    CHECK_EQ(lo, 3);
    CHECK_EQ(hi, 3);
    lo = 1;
    hi = 10;
    {
        Input in("20\nx\n");
        const std::string out = capture([&] { read_int_pair(nullptr, &lo, &hi, "R ", true); });
        CHECK_EQ(out, std::string("R  ??? Invalid Data; Valid Data Range = (     1 to      10)\nR "));
    }
    CHECK_EQ(lo, 1);
}

TEST_CASE(step_object_helpers)
{
    auto* s = new DB_STEP_DATA(3);
    CHECK_EQ(step_count_objects(s), 0);
    CHECK_EQ(step_current_object(s, true), 0);
    LLOBJ_DATA* o3 = def::model::llobj_ctor::get_or_create(s, 3);
    LLOBJ_DATA* o1 = def::model::llobj_ctor::get_or_create(s, 1);
    LLOBJ_DATA* o2 = def::model::llobj_ctor::get_or_create(s, 2);
    CHECK(o1 != nullptr && o2 != nullptr && o3 != nullptr);
    type_of(o1) = 0;
    type_of(o2) = 1;
    type_of(o3) = 7;
    CHECK_EQ(step_count_objects(s), 2);
    CHECK_EQ(step_first_deformable(s), 2);
    CHECK_EQ(step_first_rigid(s), 1);
    CHECK_EQ(step_last_object(s, 0), 2);
    CHECK_EQ(step_last_object(s, 1), 1);
    CHECK_EQ(step_current_object(s, true), 2); // 0 -> first deformable
    step_set_current_object(s, 5);
    CHECK_EQ(step_current_object(s, false), 5);
    CHECK_EQ(step_current_object(s, true), 0); // beyond the last object: reset, no default
    type_of(o2) = 7;
    CHECK_EQ(step_first_deformable(s), 0); // an exhausted search gives 0, not the last object
    CHECK(step_current_pair(s) == nullptr);
    CHECK(step_find_pair(s, 1, 2) == nullptr);
}

TEST_CASE(list_tags)
{
    auto* s = new DB_STEP_DATA(3);
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(s, 1);
    Area a;
    MenuRec* m = a.rec(0x8, o, s, 1, 10);
    type_of(o) = 0;
    CHECK_EQ(not_applicable_tag(m, 5), std::string(" "));
    CHECK_EQ(not_applicable_tag(m, 6), std::string("(Not Applicable)"));
    type_of(o) = 7;
    CHECK_EQ(not_applicable_tag(m, 4), std::string("(Not Applicable)"));
    type_of(o) = 1;
    CHECK_EQ(not_applicable_tag(m, 12), std::string(" "));
}

TEST_CASE(remesh_store_rule)
{
    auto* s = new DB_STEP_DATA(3);
    LLOBJ_DATA* o = def::model::llobj_ctor::get_or_create(s, 1);
    type_of(o) = 1;
    Area a;
    MenuRec* m = a.rec(0x8, o, s, 1, 10);
    ext::g_hooks.select = &plain_select;
    field<double>(o, 0x2d20) = 2.0;
    const double cuv = field<double>(o, 0x2d48);
    {
        // RMSTEP not a number: MGSIZR keeps 2; then RMSTEP 5: MGSIZR takes 3, MGWCUV goes to +0x2d38
        Input in("4\nx\n6\n9\n4\n5\n6\n3\n10\n0.25\nE\n");
        (void)capture([&] { CHECK_EQ(form_remesh(m), std::string("")); });
    }
    ext::g_hooks.select = nullptr;
    CHECK_EQ(field<double>(o, 0x2dd8), 5.0);
    CHECK_EQ(field<double>(o, 0x2d20), 3.0);
    CHECK_EQ(field<double>(o, 0x2d38), 0.25);
    CHECK_EQ(field<double>(o, 0x2d48), cuv); // MGWCUV itself unchanged
}

TEST_CASE(menu6_without_objects)
{
    auto* s = new DB_STEP_DATA(3);
    Area a;
    MenuRec* m = a.rec(0xc0, nullptr, s, 1, 4);
    Input in("\n");
    const std::string out = capture([&] { menu6_inter_object(m); });
    CHECK_EQ(out, std::string("  No Object Defined; \n Hit <CR> to continue ..."));
}

int main() { return testkit::run_all(); }
