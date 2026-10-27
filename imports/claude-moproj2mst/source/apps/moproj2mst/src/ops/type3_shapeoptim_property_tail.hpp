#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace m2m::mst::detail {

// Prepared display names, after native display-name formatting. Children below
// the immediate StopCritGroup children are representable but never traversed.
struct Type3ShapeOptimPropertyNode {
    std::string displayName;
    std::vector<Type3ShapeOptimPropertyNode> children;
};

struct Type3ShapeOptimPropertyValues {
    std::int32_t maxSims = 0;
    double minObjective = 0;
};

struct Type3ShapeOptimPropertyCast {
    std::vector<Type3ShapeOptimPropertyNode> properties;
    Type3ShapeOptimPropertyValues* data = nullptr;
};

// Selected concrete B vt+58 refresh only. A null linkedClass writes nothing.
// B's source and destination are independent of current OB and cast object P;
// data may alias P's data. These pointers do not model native ownership.
struct Type3ShapeOptimPropertyRefresh {
    const Type3ShapeOptimPropertyValues* linkedClass = nullptr;
    Type3ShapeOptimPropertyValues* data = nullptr;
};

enum class Type3ShapeOptimPropertyEventKind {
    DebugTopDisplayName,
    SetMaxSims,
    SetMinObjective,
    RefreshMaxSims,
    RefreshMinObjective,
};

struct Type3ShapeOptimPropertyEvent {
    Type3ShapeOptimPropertyEventKind kind;
    std::string displayName;
    std::int32_t intValue = 0;
    double doubleValue = 0;
};

enum class Type3ShapeOptimPropertyTailResult {
    Complete,
    MissingPropertyData,
    MissingRefreshData,
};

// CP454's private normal-path tail, with prepared names and no callbacks or
// reentrant mutation. Scalar sources are read at their native effect points;
// currentOb carries OB+cc (int) and OB+e0 (double).
// A null castProperties skips traversal but still permits the optional refresh.
// Appends abstract debug/write events; this is not Qt log serialization. Safe
// missing-data refusals occur only at the first required write and preserve the
// preceding effects. They do not model native AVs, Qt failure/exception behavior,
// RTTI, ownership, dynamic dispatch, owner publication, or public Type3 Open/Save.
Type3ShapeOptimPropertyTailResult projectType3ShapeOptimPropertyTail(
    const Type3ShapeOptimPropertyValues& currentOb,
    const Type3ShapeOptimPropertyCast* castProperties,
    const Type3ShapeOptimPropertyRefresh* refresh,
    std::vector<Type3ShapeOptimPropertyEvent>& events);

}  // namespace m2m::mst::detail
