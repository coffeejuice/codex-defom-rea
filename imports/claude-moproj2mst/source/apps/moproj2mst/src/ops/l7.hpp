// MDT L7: what the load slots of SfOprCycle, SfOprGroup, SfOprDoeOutput, SfOprReport and
// SfOprShapeOptim share (docs/re/moproj2mst/mdt/L7/spec.md). Owner L7.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/model.hpp"

namespace m2m::mst::l7 {

// A step of a load slot the converter does not reproduce: the conversion stops with this error.
[[noreturn]] inline void refuse(const std::string& what)
{
    throw Error(M2M_E_UNSUPPORTED, "operation load: " + what);
}

// The sub-object at OB+0x60 of SfOprDoeOutput and SfOprShapeOptim (ctor 0x1405e0ca0). Offsets are
// OB offsets; only the stores of the load slots are kept.
struct TaskBlock60 {
    std::int32_t f6c = 0;          // +0x6c
    std::int32_t f70 = 0, f74 = 0, f78 = 0;    // +0x70 .. +0x78
    std::int32_t f7c = 0, f80 = 0, f84 = 0;    // +0x7c .. +0x84
    std::uint64_t f88 = 0, f90 = 0;            // +0x88, +0x90
    std::uint8_t f98[6] = {};                  // +0x98 .. +0x9d
    std::vector<std::int32_t> listA0;          // +0xa0 QList<int>
    std::vector<std::uint64_t> listA8;         // +0xa8 QList

    // @src 0x1405e0d50  (this = OB+0x60)
    void init()
    {
        f88 = 0;                   // +0x28
        f90 = 0;                   // +0x30
        for (std::uint8_t& b : f98) b = 0;     // +0x38 .. +0x3d
        f70 = 1;                   // +0x10
        f74 = 1;                   // +0x14
        f78 = 1;                   // +0x18
        f7c = 0;                   // +0x1c
        f80 = 0;                   // +0x20
        f84 = 0;                   // +0x24
        listA0.clear();            // 0x1400424b0(+0x40): assigns an empty QList
        listA8.clear();            // 0x1405e0e40(+0x48): the same through 0x1405e0e90
    }
};

// @src 0x140055050 + 0x1400a58b0 + table vt+0x1c8: the operation with this id in the table of op's
// project, null when op has no project or the id is not there
inline Operation* tableLookup(const Operation& op, int id)
{
    if (!op.project) return nullptr;
    return operationById(*op.project, id);
}

// @src 0x140062960  predecessor of op in its project's active sub-project chain (0x140062df0, -1 ->
// null), looked up in the operation table; null without a project
inline Operation* prevOperation(const Operation& op)
{
    if (!op.project) return nullptr;
    SubProject* sp = activeSubProject(*op.project);
    if (!sp) return nullptr;
    const int id = flowPredecessor(sp->flow, op.id);
    return id == -1 ? nullptr : operationById(*op.project, id);
}

}  // namespace m2m::mst::l7
