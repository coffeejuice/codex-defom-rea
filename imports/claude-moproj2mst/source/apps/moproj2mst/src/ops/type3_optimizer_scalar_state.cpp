#include "type3_optimizer_scalar_state.hpp"

#include "m2m/qt4num.hpp"

namespace m2m::mst::detail {
namespace {

bool isKey(const qt::String& candidate, const char* ascii)
{
    return candidate == qt::L(ascii);
}

}  // namespace

Type3ScalarLineResult applyType3OptimizerScalarLine(
    Type3OptimizerScalarState& state,
    const std::vector<qt::String>& fields)
{
    if (fields.empty()) return Type3ScalarLineResult::Ignored;

    const qt::String& key = fields.front();
    if (isKey(key, "OPR_NO")) {
        if (fields.size() < 2) return Type3ScalarLineResult::RowBodyNotImplemented;
        state.value38 = qt::toInt(fields[1]);
        return Type3ScalarLineResult::Applied;
    }
    if (isKey(key, "KRG_MAX")) {
        if (fields.size() < 2) return Type3ScalarLineResult::RowBodyNotImplemented;
        state.value0c = qt::toInt(fields[1]);
        return Type3ScalarLineResult::Applied;
    }
    if (isKey(key, "TYP_OBJ")) {
        if (fields.size() < 4) return Type3ScalarLineResult::RowBodyNotImplemented;
        const std::int32_t value30 = qt::toInt(fields[1]);
        const std::int32_t value10 = qt::toInt(fields[2]);
        const std::int32_t value34 = qt::toInt(fields[3]);
        state.value30 = value30;
        state.value10 = value10;
        state.value34 = value34;
        return Type3ScalarLineResult::Applied;
    }
    if (isKey(key, "EPS_OBJ")) {
        if (fields.size() < 2) return Type3ScalarLineResult::RowBodyNotImplemented;
        state.value40 = qt::toDouble(fields[1]);
        return Type3ScalarLineResult::Applied;
    }
    if (isKey(key, "DB_PURGE")) {
        state.value48 = fields.size() >= 2 && qt::toInt(fields[1]) != 1;
        return Type3ScalarLineResult::Applied;
    }
    if (isKey(key, "NUM_CON") || isKey(key, "REGIONS"))
        return Type3ScalarLineResult::RowBodyNotImplemented;
    if (isKey(key, "DEF_OPT") || isKey(key, "TYP_OPT") ||
        isKey(key, "GRD_TYP") || isKey(key, "DOE_PAR"))
        return Type3ScalarLineResult::Ignored;
    if (isKey(key, "ENDDATA")) return Type3ScalarLineResult::Terminate;
    return Type3ScalarLineResult::Ignored;
}

Type3LineTokenizeResult tokenizeType3OptimizerLine(
    const qt::String& line,
    std::vector<qt::String>& fields)
{
    fields.clear();
    const std::u16string& input = line.u16();

    // The retained QChar table is BMP-indexed. Reject surrogate code units so
    // this value-only helper makes no claim about supplementary-plane \s behavior.
    for (char16_t c : input) {
        if (c >= 0xd800 && c <= 0xdfff)
            return Type3LineTokenizeResult::UnsupportedSurrogateInput;
    }

    const auto isSpace = [](char16_t c) {
        if ((c >= 9 && c <= 13) || c == 32 || (c >= 0x2000 && c <= 0x200a))
            return true;
        switch (c) {
        case 0x00a0: case 0x1680: case 0x180e: case 0x2028:
        case 0x2029: case 0x202f: case 0x205f: case 0x3000:
            return true;
        default:
            return false;
        }
    };

    std::size_t at = 0;
    while (at < input.size()) {
        while (at < input.size() && isSpace(input[at])) ++at;
        const std::size_t start = at;
        while (at < input.size() && !isSpace(input[at])) ++at;
        if (at != start) fields.emplace_back(input.substr(start, at - start));
    }
    return Type3LineTokenizeResult::Tokenized;
}

std::int32_t classifyType3OptimizerRowTag(std::int32_t tag) noexcept
{
    if (tag == 0x3f2 || tag == 0x3f4 || tag == 0x438) return 0;
    if ((tag >= 0x3fd && tag <= 0x3ff) || tag == 0x406 || tag == 0x410 ||
        (tag >= 0x41f && tag <= 0x421) || tag == 0x424 ||
        (tag >= 0x442 && tag <= 0x444) || (tag >= 0x44c && tag <= 0x44e))
        return 1;
    if (tag == 0x439) return 2;
    return -1;
}

std::int32_t appendType3NumConPreparedRow(
    Type3OptimizerScalarState& state,
    std::int32_t field1,
    std::int32_t classifierResult,
    double field3,
    const qt::String& formattedValue,
    std::int32_t field2,
    std::int32_t field4)
{
    state.numConRows.push_back({classifierResult, field1, field2, field4, field3, formattedValue});
    return static_cast<std::int32_t>(state.numConRows.size()) - 1;
}

std::int32_t appendType3RegionPreparedRow(
    Type3OptimizerScalarState& state,
    const qt::String& value)
{
    state.regionRows.push_back({0, value});
    return static_cast<std::int32_t>(state.regionRows.size()) - 1;
}

}  // namespace m2m::mst::detail
