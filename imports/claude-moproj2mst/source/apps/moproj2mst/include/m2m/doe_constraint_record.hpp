#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>

#include "m2m/qt4.hpp"

namespace m2m::xml { struct Node; }
namespace m2m::doe {
struct Study;
inline constexpr std::size_t kNativeConstraintRecordAllocationBytes = 0x28;

// Committed known fields after0x14091e110, not a native-layout memory image.
// No native QList/QString reference/allocator or unknown-double value is implied.
struct ConstraintRecord final {
    ConstraintRecord(int recordCategory, int recordId, int recordObjectNo, int recordRegionNo);
    const int category;  //+0
    const int id;        //+4
    const int objectNo;  //+8, native ctor1 then parsed overwrite
    const int regionNo;  //+c, native ctor1 then parsed overwrite
    // Native ctor/append leave+10 unwritten; copy transports its unknown bits.
    // This import never supplies a value or converts that field to double.
    const std::optional<std::uint64_t> sourceField10Bits = std::nullopt;
    const double value = 0.0; //+18; CP90 exact Value="0" admission only
    const qt::String text = qt::L(""); //+20, explicit non-null empty source text
};

// Model cleanup only, reverse record order. No native COW/ref/free is emitted.
void clearOwnedConstraintRecords(Study&);
// False leaves this row uncommitted. Positive canonical int32 attributes plus
// exact Value="0" only; a reconstruction bound, not original input validation.
bool appendBoundedConstraintRecord(Study&, const xml::Node&);
} // namespace m2m::doe
