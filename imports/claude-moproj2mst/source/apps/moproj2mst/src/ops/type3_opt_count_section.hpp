#pragma once

#include <cstdint>
#include <functional>

namespace m2m::mst::detail {

// CP465's detached manager-count section. The signed count must already have
// been prepared by another layer; this planner does not call manager +0x1c0,
// construct a native owner, serialize list elements, or model Qt bytes.
enum class Type3OptCountEventKind {
    literal,
    integer,
    endl
};

enum class Type3OptCountLiteral {
    countLine,
    indexedLine
};

struct Type3OptCountEvent {
    Type3OptCountEventKind kind;
    Type3OptCountLiteral literal = Type3OptCountLiteral::countLine;
    std::int32_t integer = 0;
    const char* literalText = nullptr;
};

// The caller supplies an already prepared signed count. Positive counts produce
// one event triplet per index; memory, callback failure and excessive-work
// behavior belong to this private model/caller boundary, not the native writer.
void planType3OptCountSection(std::int32_t preparedCount,
                              const std::function<void(const Type3OptCountEvent&)>& emit);

} // namespace m2m::mst::detail
