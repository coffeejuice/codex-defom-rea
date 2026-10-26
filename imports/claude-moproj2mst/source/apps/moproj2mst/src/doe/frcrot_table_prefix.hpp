#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace m2m::doe {

struct FrcrotScanObservation {
    int returnCode = 0; // Prepared result of one original %Flf call.
    double assignedValue = 0.0;
};

struct FrcrotTablePrefixInput {
    std::int32_t mode = 0;
    double modeZeroConstant = 0.0;
    std::int32_t preparedCount = 0; // Already converted by the qualified caller.
    bool allocationAndZeroFillSucceeded = false;
    std::vector<FrcrotScanObservation> observations;
    // Boundary was reached after positive-count scans; its result is ignored.
    bool trailingLineReadReached = false;
};

enum class FrcrotTablePrefixDisposition { Supported, Unsupported };

struct FrcrotTablePrefixResult {
    FrcrotTablePrefixDisposition disposition = FrcrotTablePrefixDisposition::Unsupported;
    std::int32_t dimension = 0;
    std::int32_t code = 0;
    std::int32_t normalizedCount = 0;
    double constant = 0.0;
    std::vector<double> axis;
    std::vector<double> values;
    // One destination per consumed observation: "X0", "Y0", ...
    std::vector<std::string> destinations;
    // Return 0 leaves its zero-filled destination unchanged; return 1 assigns.
    std::vector<int> scanReturnCodes;
    bool trailingLineReadReached = false;
    int tableStatus = 0;
};

// Projects only the FRCROT forward table reader over prepared scan outcomes.
// Requires the reset/mode writes, prepared cast, successful allocation and
// zero fill to have occurred. It neither parses text nor models a file, native
// owner, public Save route, or mirror payload. Counts above seven and malformed
// observation sequences are unsupported candidate inputs.
FrcrotTablePrefixResult applyFrcrotTablePrefix(const FrcrotTablePrefixInput& input);

} // namespace m2m::doe
