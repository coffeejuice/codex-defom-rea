#include "type3_opt_count_section.hpp"

#include <stdexcept>

namespace m2m::mst::detail {
namespace {
void literal(const std::function<void(const Type3OptCountEvent&)>& emit,
             Type3OptCountLiteral role)
{
    // CP473 PE string verification established these source C-string values;
    // this does not establish their Qt-encoded physical file bytes.
    static constexpr char countLabel[] = "DOE_PAR    \t\t";
    static constexpr char indexedSuffix[] = "   0.0  1.0  0.5  0.01   0";
    emit({Type3OptCountEventKind::literal, role, 0,
          role == Type3OptCountLiteral::countLine ? countLabel : indexedSuffix});
}

void integer(const std::function<void(const Type3OptCountEvent&)>& emit,
             std::int32_t value)
{
    emit({Type3OptCountEventKind::integer, Type3OptCountLiteral::countLine, value, nullptr});
}

void endl(const std::function<void(const Type3OptCountEvent&)>& emit)
{
    emit({Type3OptCountEventKind::endl, Type3OptCountLiteral::countLine, 0, nullptr});
}
} // namespace

void planType3OptCountSection(
    std::int32_t preparedCount,
    const std::function<void(const Type3OptCountEvent&)>& emit)
{
    if (!emit)
        throw std::invalid_argument("Type3 OPT count planner requires an event receiver");

    literal(emit, Type3OptCountLiteral::countLine);
    integer(emit, preparedCount);
    endl(emit);

    for (std::int64_t index = 1; index <= static_cast<std::int64_t>(preparedCount); ++index) {
        integer(emit, static_cast<std::int32_t>(index));
        literal(emit, Type3OptCountLiteral::indexedLine);
        endl(emit);
    }

    // CP465 shows a final standalone QTextStream endl, including count <= 0.
    endl(emit);
}

} // namespace m2m::mst::detail
