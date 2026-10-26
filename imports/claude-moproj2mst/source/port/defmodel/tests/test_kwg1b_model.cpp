// kw-g1b model helpers: DB_STEP_DATA name arrays, the CVNAME collection, the trailing-blank trim.
// Expected values follow the decompiles cited in the sources (worked out by hand here); the KEY
// round trips through these functions are checked against the oracle in defkey.kw_g1b.
// ctest-labels: kw-g1b
#define _CRT_SECURE_NO_WARNINGS
#include <cstdlib>
#include <cstring>
#include <new>
#include <string>

#include <defmodel/cvname/cvname.hpp>
#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/step/name_arrays.hpp>
#include <defmodel/strtrim.hpp>
#include <testkit/check.hpp>

using namespace def::model;

static DB_STEP_DATA* zeroed_model(int dim)
{
    auto* m = static_cast<DB_STEP_DATA*>(std::calloc(1, sizeof(DB_STEP_DATA)));
    m->dimension = dim;
    new (&m->u33b8) Cls_14032f100();
    return m;
}

static int hook_calls = 0;
static void count_hook(DB_STEP_DATA*, std::int32_t flag)
{
    CHECK_EQ(flag, 0);
    ++hook_calls;
}

TEST_CASE(name_arrays_resize)
{
    DB_STEP_DATA* m = zeroed_model(3);
    g_hook_1401d0fc0 = &count_hook;
    hook_calls = 0;
    resize_node_names(m, 3);
    CHECK_EQ(m->u3398, 3);
    REQUIRE(m->u33a0 != nullptr);
    for (int i = 0; i < 3; ++i)
        CHECK(m->u33a0[i] != nullptr && m->u33a0[i][0] == '\0');
    std::strcpy(m->u33a0[0], "keep");
    resize_node_names(m, 3);  // same count: nothing happens, no hook
    CHECK_EQ(hook_calls, 1);
    resize_node_names(m, 1);  // shrink keeps the first names
    CHECK_EQ(m->u3398, 1);
    CHECK_EQ(std::string(m->u33a0[0]), std::string("keep"));
    CHECK_EQ(node_name_count(m), 1);
    resize_node_names(m, 0);  // zero frees everything
    CHECK(m->u33a0 == nullptr);
    CHECK_EQ(m->u3398, 0);
    CHECK_EQ(hook_calls, 3);

    resize_element_names(m, 2);
    CHECK_EQ(element_name_count(m), 2);
    CHECK(m->u33b0 != nullptr && m->u33b0[1] != nullptr);
    CHECK(m->u33a0 == nullptr);  // the node array is untouched
    g_hook_1401d0fc0 = nullptr;
}

TEST_CASE(cvname_collection)
{
    DB_STEP_DATA* m = zeroed_model(3);
    Cls_14032f100* c = &m->u33b8;
    CHECK_EQ(cv_count(c), 0);
    cv_set_name(c, 5, "Five");
    cv_append_item(c, 5, "a");
    cv_append_item(c, 5, "b");
    cv_set_name(c, 3, "Three");
    CHECK_EQ(cv_count(c), 2);
    // insertion order, not id order
    CHECK_EQ(cv_def_id(cv_entry(c, 0)), 5);
    CHECK_EQ(cv_def_id(cv_entry(c, 1)), 3);
    CustomVarDef* five = cv_entry(c, 0);
    CHECK_EQ(cv_def_name(five), std::string("Five"));
    CHECK_EQ(cv_def_item_count(five), 2);
    CHECK_EQ(cv_def_item(five, 1), std::string("b"));
    CHECK_EQ(cv_def_item(five, 2), std::string());   // out of range -> ""
    CHECK_EQ(cv_def_item(five, -1), std::string());
    // an existing id is reused; clearing keeps the name
    cv_clear_items(c, 5);
    CHECK_EQ(cv_count(c), 2);
    CHECK_EQ(cv_def_item_count(five), 0);
    CHECK_EQ(cv_def_name(five), std::string("Five"));
    // clear on a new id creates an empty entry at the end
    cv_clear_items(c, 9);
    CHECK_EQ(cv_count(c), 3);
    CHECK_EQ(cv_def_name(cv_entry(c, 2)), std::string());
}

TEST_CASE(trim_right_blanks_cases)
{
    char a[] = "name  \r\n";
    trim_right_blanks(a);
    CHECK_EQ(std::string(a), std::string("name"));
    char b[] = "  lead\tkept \n";
    trim_right_blanks(b);
    CHECK_EQ(std::string(b), std::string("  lead\tkept"));  // tabs are not trimmed
    char c[] = "   ";
    trim_right_blanks(c);
    CHECK_EQ(std::string(c), std::string());
    char d[] = "";
    trim_right_blanks(d);
    CHECK_EQ(std::string(d), std::string());
}

int main() { return testkit::run_all(); }
