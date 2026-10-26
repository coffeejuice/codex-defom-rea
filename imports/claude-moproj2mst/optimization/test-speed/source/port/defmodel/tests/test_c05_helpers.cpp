// ctest-labels: kw-g1b kw-g4b c05
// Census follow-up C05: the list helpers under the CVNAME collection (14032f9b0, 14032f9f0,
// 14032fa50, 14032fb10, 14032fc00, 14032fcf0, 14032ed30) and the MaterialList accessors of
// 1401d0fc0 (1401d8fa0, 1401d8fb0). Expected values follow the decompiles
// (deform-rea-work/out/c05/rows.c); CVNAME round trips are covered by test_kwg1b_model and the
// defkey kw_g1b oracle test.
#include <cstdint>
#include <cstring>
#include <string>

#include <defmodel/cvname/cvname.hpp>
#include <defmodel/cvname/cvname_list.hpp>
#include <defmodel/material/state_remap.hpp>
#include <testkit/check.hpp>

using namespace def::model;

TEST_CASE(var_list_begin_end_push_back_back)
{
    VarListRep rep;
    CHECK(var_list_iter_default() == nullptr);          // 14032fc00: node = 0
    CHECK(var_list_end(&rep) == rep.head);              // 14032f9f0: the sentinel
    CHECK(var_list_begin(&rep) == var_list_end(&rep));  // 14032f9b0: empty -> head->next == head

    CustomVarDef a;
    cv_def_construct(&a, 7);  // 14032ed30
    CHECK_EQ(a.id, 7);
    CHECK_EQ(a.name, std::string());
    CHECK_EQ(a.items.rep->size, 0u);

    VarListNode* n1 = var_list_push_back(&rep, a);  // 14032fa50
    CHECK_EQ(rep.size, 1u);
    CHECK(var_list_begin(&rep) == n1);
    CHECK(n1->next == rep.head);
    CHECK(rep.head->prev == n1);
    CHECK_EQ(var_list_back(&rep)->id, 7);  // 14032fb10

    CustomVarDef b;
    cv_def_construct(&b, 9);
    b.set_name("nine");
    cv_def_append(&b, "x");
    cv_def_append(&b, "y");
    VarListNode* n2 = var_list_push_back(&rep, b);
    CHECK_EQ(rep.size, 2u);
    CHECK(var_list_begin(&rep) == n1);  // insertion order kept
    CHECK(n1->next == n2);
    CHECK(var_list_back(&rep) == &n2->value);
    CHECK_EQ(n2->value.name, std::string("nine"));
    CHECK_EQ(n2->value.items.rep->size, 2u);
    CHECK_EQ(n2->value.items.rep->head->next->value, std::string("x"));
    CHECK_EQ(n2->value.items.rep->head->prev->value, std::string("y"));
    CHECK_EQ(b.items.rep->size, 2u);  // the source keeps its items (a copy, not a move)
}

TEST_CASE(cv_def_construct_resets_existing)
{
    CustomVarDef d;
    d.set_name("old");
    cv_def_append(&d, "i");
    cv_def_construct(&d, -3);
    CHECK_EQ(d.id, -3);
    CHECK_EQ(d.name, std::string());
    CHECK_EQ(d.items.rep->size, 0u);
}

TEST_CASE(str_list_end_is_sentinel)
{
    StrListRep rep;
    CHECK(str_list_end(&rep) == rep.head);  // 14032fcf0
    CustomVarDef d;
    cv_def_append(&d, "a");
    CHECK(str_list_end(d.items.rep) == d.items.rep->head);
    CHECK(d.items.rep->head->next != str_list_end(d.items.rep));
}

TEST_CASE(material_list_accessors)
{
    alignas(8) unsigned char buf[0x30] = {};
    std::int32_t numbers[2] = {4, 5};
    std::int32_t index_of[2] = {1, 0};
    std::int32_t* p = numbers;
    std::memcpy(buf + 0x18, &p, 8);
    p = index_of;
    std::memcpy(buf + 0x20, &p, 8);
    const auto* l = reinterpret_cast<const MaterialList*>(buf);
    CHECK(material_list_numbers(l) == numbers);    // 1401d8fa0: *(l + 0x18)
    CHECK(material_list_index_of(l) == index_of);  // 1401d8fb0: *(l + 0x20)
}

int main() { return testkit::run_all(); }
