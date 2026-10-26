#include "type3_optimizer_scalar_sequence.hpp"

#include <cmath>
#include <limits>

#include "m2m/qt4num.hpp"

namespace m2m::mst::detail {
namespace {

bool isAscii(const qt::String& value)
{
    for (char16_t c : value.u16()) {
        if (c > 0x7f) return false;
    }
    return true;
}

bool parseAsciiInt(const qt::String& value, std::int32_t& parsed)
{
    if (!isAscii(value)) return false;
    bool ok = false;
    parsed = qt::toInt(value, &ok);
    return ok;
}

bool parseAsciiFiniteDouble(const qt::String& value, double& parsed)
{
    if (!isAscii(value)) return false;
    bool ok = false;
    parsed = qt::toDouble(value, &ok);
    return ok && std::isfinite(parsed);
}

bool parseNumConRow(const std::vector<qt::String>& fields,
                    std::int32_t& field1,
                    std::int32_t& field2,
                    double& field3,
                    std::int32_t& field4)
{
    // The original successful path reads fields 1 through 4. Short rows and
    // noncanonical conversion cases stay guarded rather than being treated as
    // native parse failures.
    return fields.size() >= 5 &&
        parseAsciiInt(fields[1], field1) &&
        parseAsciiInt(fields[2], field2) &&
        parseAsciiFiniteDouble(fields[3], field3) &&
        parseAsciiInt(fields[4], field4);
}

}  // namespace

Type3ScalarSequenceResult applyType3OptimizerScalarLines(
    Type3OptimizerScalarState& state,
    std::span<const qt::String> lines)
{
    std::vector<qt::String> fields;
    std::size_t index = 0;
    while (index < lines.size()) {
        if (tokenizeType3OptimizerLine(lines[index], fields) ==
            Type3LineTokenizeResult::UnsupportedSurrogateInput) {
            return {Type3ScalarSequenceStop::UnsupportedSurrogateLine, index};
        }

        if (!fields.empty() && fields.front() == qt::L("NUM_CON")) {
            if (fields.size() < 2) return {Type3ScalarSequenceStop::UnsupportedRowBody, index};
            std::int32_t rowCount = 0;
            if (!parseAsciiInt(fields[1], rowCount))
                return {Type3ScalarSequenceStop::UnsupportedRowBody, index};

            ++index;  // The NUM_CON header has been consumed.
            for (std::int32_t row = 0; row < rowCount && index < lines.size(); ++row, ++index) {
                const std::size_t rowLineIndex = index;
                std::vector<qt::String> rowFields;
                if (tokenizeType3OptimizerLine(lines[rowLineIndex], rowFields) !=
                    Type3LineTokenizeResult::Tokenized) {
                    return {Type3ScalarSequenceStop::UnsupportedSurrogateLine, rowLineIndex};
                }

                std::int32_t field1 = 0;
                std::int32_t field2 = 0;
                std::int32_t field4 = 0;
                double field3 = 0.0;
                if (!parseNumConRow(rowFields, field1, field2, field3, field4) ||
                    state.numConRows.size() >=
                        static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max())) {
                    return {Type3ScalarSequenceStop::UnsupportedRowBody, rowLineIndex};
                }

                const qt::String emptyValue(std::u16string{});
                const auto classifier = classifyType3OptimizerRowTag(field1);
                (void)appendType3NumConPreparedRow(
                    state, field1, classifier, field3, emptyValue, field2, field4);
            }
            continue;
        }

        const auto lineResult = applyType3OptimizerScalarLine(state, fields);
        if (lineResult == Type3ScalarLineResult::Terminate)
            return {Type3ScalarSequenceStop::EndData, index + 1};
        if (lineResult == Type3ScalarLineResult::RowBodyNotImplemented)
            return {Type3ScalarSequenceStop::UnsupportedRowBody, index};
        ++index;
    }
    return {Type3ScalarSequenceStop::AllLinesConsumed, lines.size()};
}

}  // namespace m2m::mst::detail
