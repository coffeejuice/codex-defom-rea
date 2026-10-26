// ctest-labels: m2m-mdt mdt-l6
#include <testkit/check.hpp>

#include "m2m/common.hpp"
#include "m2m/doe_task_lists.hpp"

using namespace m2m;
using qt::L;

namespace {
xml::NodePtr parse(const std::string& text)
{
    xml::Document doc;
    qt::String error;
    int line = 0, column = 0;
    REQUIRE(xml::setContent(doc, text, &error, &line, &column));
    return doc.root;
}
}  // namespace

TEST_CASE(doe_task_read_preserves_operation_duplicates_and_deduplicates_objects)
{
    auto task = parse("<SfTask><OprList><Opr No='5' NumMiddleSteps='1'/><Ignore/>"
        "<Opr No='5' NumMiddleSteps='1'/></OprList><ObjList><Object No='2'/>"
        "<Object No='1'/><Object No='2'/><Object No='bad'/><Object/>"
        "<Object No='32768'/><Object No='-32768'/></ObjList><OutputList/></SfTask>");
    doe::TaskLists state;
    doe::readTaskLists(*task, state);
    REQUIRE(state.operations.size() == 2);
    CHECK(state.operations[0] == state.operations[1]);
    CHECK(state.operations[0].no == 5 && state.operations[0].numMiddleSteps == 1);
    CHECK(state.objects == std::vector<int>({2, 1, 0, -32768}));
}

TEST_CASE(doe_task_short_conversion_overflow_and_boolean_semantics)
{
    auto task = parse("<SfTask><OprList><Opr No='-32768' StartStep='-2' EndStep='32768' "
        "MiddleStep='32767' NumMiddleSteps='32767'/><Opr No='32768' StartStep='x' "
        "EndStep='-32768' MiddleStep='-32769' NumMiddleSteps='9999999999999999999999'/>"
        "<Opr/></OprList></SfTask>");
    doe::TaskLists state;
    doe::readTaskLists(*task, state);
    REQUIRE(state.operations.size() == 3);
    CHECK(state.operations[0] == doe::TaskOperationSelection({-32768, true, false, true, 32767}));
    CHECK(state.operations[1] == doe::TaskOperationSelection({0, false, true, false, 0}));
    CHECK(state.operations[2] == doe::TaskOperationSelection({0, false, false, false, 0}));
}

TEST_CASE(doe_task_repeated_lists_clear_in_encounter_order_and_absent_lists_retain_state)
{
    doe::TaskLists state;
    state.objects = {99};
    state.operations = {{88, false, false, false, 2}};
    doe::readTaskLists(*parse("<SfTask><Unknown/><OprList><Opr No='1'/></OprList>"
        "<OprList><Opr No='2'/></OprList></SfTask>"), state);
    CHECK(state.objects == std::vector<int>({99}));
    REQUIRE(state.operations.size() == 1);
    CHECK(state.operations[0].no == 2);
    doe::readTaskLists(*parse("<SfTask><ObjList><Object No='1'/></ObjList>"
        "<OprList/><ObjList/></SfTask>"), state);
    CHECK(state.objects.empty());
    CHECK(state.operations.empty());
}

TEST_CASE(doe_task_update_replaces_every_existing_list_without_inserting_absent_lists)
{
    auto task = parse("<SfTask><Unknown/><OprList old='x'><Old/></OprList>"
        "<ObjList/><OprList/><OutputList><Keep/></OutputList></SfTask>");
    auto unknown = task->children[0];
    auto output = task->children.back();
    doe::TaskLists state;
    state.operations = {{5, true, false, true, 3}, {5, false, true, false, 0}};
    state.objects = {4, 4, -2}; // serialization preserves supplied duplicates
    doe::updateTaskLists(state, *task);
    CHECK(task->children[0] == unknown && task->children.back() == output);
    const auto opr = task->childElements(L("OprList"));
    REQUIRE(opr.size() == 2);
    CHECK(opr[0] != opr[1]);
    CHECK(!opr[0]->hasAttribute(L("old")));
    for (const auto* list : opr) {
        const auto rows = list->childElements(L("Opr"));
        REQUIRE(rows.size() == 2);
        CHECK(rows[0]->attribute(L("No")) == L("5"));
        CHECK(rows[0]->attribute(L("StartStep")) == L("1"));
        CHECK(rows[0]->attribute(L("EndStep")) == L("0"));
        CHECK(rows[0]->attribute(L("MiddleStep")) == L("1"));
        CHECK(rows[0]->attribute(L("NumMiddleSteps")) == L("3"));
    }
    const auto objects = task->childElements(L("ObjList"))[0]->childElements(L("Object"));
    REQUIRE(objects.size() == 3);
    CHECK(objects[0]->attribute(L("No")) == L("4"));
    CHECK(objects[1]->attribute(L("No")) == L("4"));
    CHECK(objects[2]->attribute(L("No")) == L("-2"));
    auto absent = parse("<SfTask><Unknown/></SfTask>");
    doe::updateTaskLists(state, *absent);
    CHECK(absent->children.size() == 1);
}

TEST_CASE(doe_task_empty_state_retains_existing_dom_children)
{
    auto task = parse("<SfTask><OprList old='1'/><ObjList><Object No='7'/></ObjList></SfTask>");
    const auto before = task->children;
    doe::updateTaskLists({}, *task);
    CHECK(task->children == before);
}

TEST_CASE(doe_task_populated_output_refuses_after_prior_list_effects)
{
    for (const auto* payload : {"<Output/>", "<!--raw child-->"}) {
        auto task = parse(std::string("<SfTask><ObjList><Object No='3'/></ObjList><OutputList>") +
            payload + "</OutputList><ObjList><Object No='4'/></ObjList></SfTask>");
        doe::TaskLists state;
        state.objects = {9};
        bool refused = false;
        try { doe::readTaskLists(*task, state); }
        catch (const Error& e) { refused = e.code == M2M_E_UNSUPPORTED; }
        CHECK(refused);
        CHECK(state.objects == std::vector<int>({3}));
    }
}

int main() { return testkit::run_all(); }
