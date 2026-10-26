#pragma once

#include <vector>

#include "m2m/xml.hpp"

namespace m2m::doe {

// Explicit list state for the recovered SfTask XML seam, not TaskDoe construction
// or reset defaults. The original record stores two ints and three boolean bytes.
struct TaskOperationSelection {
    int no = 0;
    bool startStep = false;
    bool endStep = false;
    bool middleStep = false;
    int numMiddleSteps = 0;
    bool operator==(const TaskOperationSelection&) const = default;
};

struct TaskLists {
    std::vector<TaskOperationSelection> operations;
    std::vector<int> objects;
    // No output payloads are represented. Their polymorphic parser is unsupported.
};

// 0x14140cba0 -> ce80/cf00 -> 141410580/141410bb0. Each encountered list
// clears its corresponding state, then reads in order. Opr duplicates survive;
// Object duplicates do not. Absent lists preserve prior state. Attributes use
// QString::toShort: failed/out-of-range conversion is zero, never wrapping.
// Empty OutputList is accepted for this output-free state; any raw child refuses
// at encounter, retaining preceding list effects. Uses the existing Qt integer seam.
void readTaskLists(const xml::Node& task, TaskLists& lists);

// 0x14140d2e0 -> d660/d6c0 -> 141410df0/141411150. Replace each existing
// OprList/ObjList only when its state list is nonempty. Never append absent lists.
// OutputList and unknown children remain unchanged for the output-free state.
void updateTaskLists(const TaskLists& lists, xml::Node& task);

}  // namespace m2m::doe
