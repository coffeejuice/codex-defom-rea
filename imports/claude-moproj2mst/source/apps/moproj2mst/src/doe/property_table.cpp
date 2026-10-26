#include "m2m/doe_property_table.hpp"

#include <limits>

#include "m2m/common.hpp"

namespace m2m::doe {

bool PropertyTableState::itemIsAbsent(std::int32_t row, std::int32_t column) const
{
    const PropertyTableQuery query{row, column};
    lastQuery_ = query;
    if (queryCount_ != std::numeric_limits<std::uint64_t>::max()) ++queryCount_;
    else queryReportTruncated_ = true;
    if (queryPrefixSize_ < queries_.size()) queries_[queryPrefixSize_++] = query;
    else queryReportTruncated_ = true;

    if (row < 0 || column < 0 || constructorEmpty_) return true;
    throw Error(M2M_E_UNSUPPORTED,
        "DOE StepData populated property-table item/check-state behavior remains unsupported");
}

} // namespace m2m::doe
