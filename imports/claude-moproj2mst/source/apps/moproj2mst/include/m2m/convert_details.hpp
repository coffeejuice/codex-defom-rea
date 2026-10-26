#pragma once

#include <memory>
#include <optional>

#include "m2m/convert.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/engine.hpp"

namespace m2m {

// C++ observation surface. The journal owns value observations only; no
// Project, Session, device, callback, or replayable FileEffects escape here.
// It may be empty after an actual StudyIo was created; null means no owner was
// created. RefreshBoundaryReached records pre-dispatch reach, not Qt delivery
// or Save completion. For saveCall, interpret project/mst Succeeded only when
// the corresponding Attempted and Returned flags are both true. Reuse resets
// the whole output before a new conversion; inspect it after the call returns,
// not concurrently while the call is mutating the journal.
struct ConversionDetails {
    std::shared_ptr<const doe::StudyXmlJournal> xmlJournal;
    std::optional<doe::IoMode> ioMode;
    std::optional<engine::SaveCallState> saveCall;
    std::optional<int> returnedCode;
};

Result convertDetailed(const std::u16string& moproj, const std::u16string& outMst,
                       unsigned flags, unsigned codepage, ConversionDetails& details,
                       int guiUnit = 0, const std::u16string& deformDir = {},
                       const OriginalSettings& settings = {});

}  // namespace m2m
