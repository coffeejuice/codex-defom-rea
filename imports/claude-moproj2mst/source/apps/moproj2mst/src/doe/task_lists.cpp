#include "m2m/doe_task_lists.hpp"

#include <algorithm>

#include "m2m/common.hpp"

namespace m2m::doe {
namespace {
using qt::L;

int shortAttribute(const xml::Node& node, const char* name)
{
    bool ok = false;
    const int value = qt::toInt(node.attribute(L(name)), &ok);
    return ok && value >= -32768 && value <= 32767 ? value : 0;
}

xml::NodePtr operationList(const TaskLists& lists)
{
    auto out = xml::createElement(L("OprList"));
    for (const auto& value : lists.operations) {
        auto child = xml::createElement(L("Opr"));
        xml::setAttribute(*child, L("No"), qt::number(value.no));
        xml::setAttribute(*child, L("StartStep"), qt::number(value.startStep ? 1 : 0));
        xml::setAttribute(*child, L("EndStep"), qt::number(value.endStep ? 1 : 0));
        xml::setAttribute(*child, L("MiddleStep"), qt::number(value.middleStep ? 1 : 0));
        xml::setAttribute(*child, L("NumMiddleSteps"), qt::number(value.numMiddleSteps));
        out->children.push_back(child);
    }
    return out;
}

xml::NodePtr objectList(const TaskLists& lists)
{
    auto out = xml::createElement(L("ObjList"));
    for (int value : lists.objects) {
        auto child = xml::createElement(L("Object"));
        xml::setAttribute(*child, L("No"), qt::number(value));
        out->children.push_back(child);
    }
    return out;
}
}  // namespace

void readTaskLists(const xml::Node& task, TaskLists& lists)
{
    for (const auto* child : task.childElements()) {
        if (child->tag == L("OprList")) {
            lists.operations.clear();                     // 0x14140c780
            for (const auto* row : child->childElements(L("Opr"))) {
                lists.operations.push_back({shortAttribute(*row, "No"),
                    shortAttribute(*row, "StartStep") != 0,
                    shortAttribute(*row, "EndStep") != 0,
                    shortAttribute(*row, "MiddleStep") != 0,
                    shortAttribute(*row, "NumMiddleSteps")});
            }
        } else if (child->tag == L("ObjList")) {
            lists.objects.clear();                        // 0x14140cf00
            for (const auto* row : child->childElements(L("Object"))) {
                const int value = shortAttribute(*row, "No");
                // 0x1400251d0 compares int entries; append only when absent.
                if (std::find(lists.objects.begin(), lists.objects.end(), value) == lists.objects.end())
                    lists.objects.push_back(value);
            }
        } else if (child->tag == L("OutputList") && !child->children.empty()) {
            // 0x14140cf80 dispatches even non-element raw children. Do not silently
            // ignore a comment/text payload or invent concrete output objects.
            throw Error(M2M_E_UNSUPPORTED, "DOE populated Task OutputList is not recovered");
        }
    }
}

void updateTaskLists(const TaskLists& lists, xml::Node& task)
{
    for (auto& child : task.children) {
        if (!child || child->kind != xml::Node::Element) continue;
        if (child->tag == L("OprList") && !lists.operations.empty())
            child = operationList(lists);
        else if (child->tag == L("ObjList") && !lists.objects.empty())
            child = objectList(lists);
    }
}

}  // namespace m2m::doe
