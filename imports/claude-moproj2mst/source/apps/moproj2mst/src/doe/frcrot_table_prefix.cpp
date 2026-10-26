#include "frcrot_table_prefix.hpp"

#include <algorithm>
#include <cstddef>

namespace m2m::doe {
namespace {
constexpr std::int32_t kMaxCandidateCount = 7;

bool supportedCode(int code) { return code == -1 || code == 0 || code == 1; }
}

FrcrotTablePrefixResult applyFrcrotTablePrefix(const FrcrotTablePrefixInput& input)
{
    FrcrotTablePrefixResult result;
    if (input.mode != 1 && input.mode != 2) {
        if (!input.observations.empty() || input.trailingLineReadReached)
            return result;
        result.disposition = FrcrotTablePrefixDisposition::Supported;
        if (input.mode == 0)
            result.constant = input.modeZeroConstant;
        return result;
    }

    if (!input.allocationAndZeroFillSucceeded)
        return result;

    result.dimension = 1;
    result.code = input.mode == 1 ? 1 : 4;
    result.normalizedCount = std::max<std::int32_t>(0, input.preparedCount);
    if (result.normalizedCount > kMaxCandidateCount)
        return result;

    const std::size_t required = static_cast<std::size_t>(result.normalizedCount) * 2;
    if (input.observations.size() > required)
        return FrcrotTablePrefixResult{};
    for (const auto& observation : input.observations) {
        if (!supportedCode(observation.returnCode))
            return FrcrotTablePrefixResult{};
    }

    result.disposition = FrcrotTablePrefixDisposition::Supported;
    result.axis.assign(static_cast<std::size_t>(result.normalizedCount), 0.0);
    result.values.assign(static_cast<std::size_t>(result.normalizedCount), 0.0);
    std::size_t consumed = 0;
    bool failed = false;
    for (std::int32_t row = 0; row < result.normalizedCount && !failed; ++row) {
        for (int field = 0; field < 2; ++field) {
            if (consumed == input.observations.size())
                return FrcrotTablePrefixResult{};
            const auto& scan = input.observations[consumed++];
            result.destinations.push_back(std::string(field == 0 ? "X" : "Y") +
                                          std::to_string(row));
            result.scanReturnCodes.push_back(scan.returnCode);
            if (scan.returnCode == 1) {
                (field == 0 ? result.axis : result.values)[static_cast<std::size_t>(row)] =
                    scan.assignedValue;
            } else if (scan.returnCode == -1) {
                result.tableStatus = 1;
                failed = true;
                break;
            }
        }
    }
    // Observation vectors enumerate actual consecutive scan calls. Entries
    // after EOF cannot describe any native call and make the shape unsupported.
    if (consumed != input.observations.size())
        return FrcrotTablePrefixResult{};
    // The native ignored line-read follows only a complete, positive-count scan.
    if (!failed && result.normalizedCount > 0) {
        result.trailingLineReadReached = true;
        // The supplied boundary bit is a qualification check, not a result value.
        if (!input.trailingLineReadReached)
            return FrcrotTablePrefixResult{};
    } else if (input.trailingLineReadReached) {
        return FrcrotTablePrefixResult{};
    }
    return result;
}

} // namespace m2m::doe
