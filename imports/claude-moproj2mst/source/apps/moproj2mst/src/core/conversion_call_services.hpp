#pragma once

#include "m2m/convert_details.hpp"

namespace m2m::moproj::detail { class ProjectOutputFactory; }
namespace m2m::detail { class RemeshTerminalCallService; }

namespace m2m::detail {

// Borrowed for one synchronous conversion. No pointer enters Project, Session,
// public settings, the C ABI, or the caller-owned ConversionDetails result.
struct ConversionCallServices {
    moproj::detail::ProjectOutputFactory* projectOutput = nullptr;
    RemeshTerminalCallService* remeshTerminal = nullptr;
};

Result convertDetailedWithServices(
    const std::u16string& moproj, const std::u16string& outMst,
    unsigned flags, unsigned codepage, ConversionDetails& details,
    const ConversionCallServices& services,
    int guiUnit = 0, const std::u16string& deformDir = {},
    const OriginalSettings& settings = {});

}  // namespace m2m::detail
