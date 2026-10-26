#include "m2m/doe_constraint_record.hpp"

#include <limits>
#include <memory>
#include <optional>
#include <utility>

#include "m2m/doe_study.hpp"

namespace m2m::doe {
namespace {
std::optional<int> canonicalPositive(const qt::String& text)
{
    const auto& value = text.u16();
    if (value.empty() || value.front() < u'1' || value.front() > u'9') return {};
    int number = 0;
    for (const auto ch : value) {
        if (ch < u'0' || ch > u'9') return {};
        const int digit = ch - u'0';
        if (number > (std::numeric_limits<int>::max() - digit) / 10) return {};
        number = number * 10 + digit;
    }
    return number;
}
int category(int id)
{
    if (id == 1081) return 1;
    return id == 1010 || id == 1012 || id == 1080 ? 0 : 2;
}
}

ConstraintRecord::ConstraintRecord(int recordCategory, int recordId, int recordObjectNo, int recordRegionNo)
    : category(recordCategory), id(recordId), objectNo(recordObjectNo), regionNo(recordRegionNo) {}

void clearOwnedConstraintRecords(Study& study)
{
    // Final-owner native loop is reverse; host reclamation is adapter-only and
    // does not assert native list aliases were released or refcounts observed.
    while (!study.constraintRecords.empty()) study.constraintRecords.pop_back();
    study.constraints = 0;
}

bool appendBoundedConstraintRecord(Study& study, const xml::Node& row)
{
    static_assert(sizeof(int) == sizeof(std::int32_t));
    using qt::L;
    if (row.kind != xml::Node::Element || row.tag != L("Constraint") ||
        !row.hasAttribute(L("Id")) || !row.hasAttribute(L("Value")) ||
        !row.hasAttribute(L("ObjectNo")) || !row.hasAttribute(L("RegionNo"))) return false;
    // Source attribute order; this row has no persistent mutation before append.
    const auto idText = row.attribute(L("Id"));
    const auto valueText = row.attribute(L("Value"));
    const auto objectText = row.attribute(L("ObjectNo"));
    const auto regionText = row.attribute(L("RegionNo"));
    const auto id = canonicalPositive(idText);
    const auto objectNo = canonicalPositive(objectText);
    const auto regionNo = canonicalPositive(regionText);
    if (!id || !objectNo || !regionNo || valueText != L("0")) return false;
    if (study.constraintRecords.size() >= static_cast<std::size_t>(std::numeric_limits<int>::max()) ||
        study.constraints != static_cast<int>(study.constraintRecords.size())) return false;
    auto record = std::make_unique<ConstraintRecord>(category(*id), *id, *objectNo, *regionNo);
    study.constraintRecords.push_back(std::move(record));
    study.constraints = static_cast<int>(study.constraintRecords.size());
    return true; // Native0x14091e110 returns count-1; XML caller ignores it.
}
} // namespace m2m::doe
