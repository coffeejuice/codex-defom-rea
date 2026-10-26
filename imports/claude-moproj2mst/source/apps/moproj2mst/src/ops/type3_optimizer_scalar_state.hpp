#pragma once

#include <cstdint>
#include <vector>

#include "m2m/common.hpp"
#include "m2m/qt4.hpp"

namespace m2m::mst::detail {

// Prepared-value projections of the records passed to DefOptModel vtable +0x8/+0xa8.
// These are not the native optimizer's container/storage layouts.
struct Type3NumConRowValues {
    std::int32_t value00;
    std::int32_t value04;
    std::int32_t value08;
    std::int32_t value0c;
    double value18;
    qt::String value20;
};

struct Type3RegionRowValues {
    std::int32_t value00;
    qt::String value08;
};

// Value-only projection of recovered Type3 optimizer scalar fields and prepared rows.
// It does not model the native object or its nontrivial member storage at +0x18/+0x20.
struct Type3OptimizerScalarState {
    std::int32_t value0c = 1;
    std::int32_t value10 = 1;
    std::int32_t value30 = 0x3fd;
    std::int32_t value34 = -1;
    std::int32_t value38 = 200;
    std::int32_t value3c = 0;
    double value40 = 0.001;
    bool value48 = false;
    std::vector<Type3NumConRowValues> numConRows;
    std::vector<Type3RegionRowValues> regionRows;
};

enum class Type3ScalarLineResult {
    Applied,
    Ignored,
    Terminate,
    RowBodyNotImplemented,
};

// Applies one already-tokenized top-level .OPT line. fields[0] is the key.
// This helper intentionally does not split input or handle row bodies.
Type3ScalarLineResult applyType3OptimizerScalarLine(
    Type3OptimizerScalarState& state,
    const std::vector<qt::String>& fields);

enum class Type3LineTokenizeResult {
    Tokenized,
    UnsupportedSurrogateInput,
};

// Splits one supplied BMP QString line as QRegExp("\\s+") / SkipEmptyParts.
// UTF-16 surrogate code units are explicitly outside this helper's admitted profile.
Type3LineTokenizeResult tokenizeType3OptimizerLine(
    const qt::String& line,
    std::vector<qt::String>& fields);

// Stateless DefOptModel +0xa0 tag mapping recovered from the embedded lookup table.
std::int32_t classifyType3OptimizerRowTag(std::int32_t tag) noexcept;

// Appends already-converted values in native call-argument order and returns size - 1.
// The caller keeps the resulting row count representable as a positive int32_t.
std::int32_t appendType3NumConPreparedRow(
    Type3OptimizerScalarState& state,
    std::int32_t field1,
    std::int32_t classifierResult,
    double field3,
    const qt::String& formattedValue,
    std::int32_t field2,
    std::int32_t field4);

// Native REGIONS call passes index zero and a QString reference; returns size - 1.
// The caller keeps the resulting row count representable as a positive int32_t.
std::int32_t appendType3RegionPreparedRow(
    Type3OptimizerScalarState& state,
    const qt::String& value);

}  // namespace m2m::mst::detail
