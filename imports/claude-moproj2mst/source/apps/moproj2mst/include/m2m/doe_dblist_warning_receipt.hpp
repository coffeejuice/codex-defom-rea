#pragma once

#include <cstdint>
#include <optional>
#include <vector>

#include "m2m/qt4.hpp"

namespace m2m::doe {

// Data-only description of the statically recovered DBLIST warning call.
// This does not call QMessageBox or inspect files, managers, or global state.
struct DbListWarningReceipt {
    bool shouldDisplay = false;
    qt::String title;
    qt::String messagePrefix;
    // Raw DBNAME text values in the order supplied by the caller. The native
    // final separator/formatting between these values is not modeled here.
    std::vector<qt::String> missingDbNames;
    // Opaque supplied identity for the QWidget parent argument; never dereferenced.
    std::optional<std::uintptr_t> parentWidgetToken;
};

// Inputs are already-resolved facts from a caller that owns the real manager:
// whether any DBNAME path was missing, the global +0x0c warning state, ordered raw names,
// and (only when available) the main-window QWidget identity token. Mode 5
// suppresses only the warning. The helper does not model earlier registrations.
DbListWarningReceipt makeDbListWarningReceipt(
    bool hasMissingDatabase, int warningState,
    const std::vector<qt::String>& missingDbNames,
    std::optional<std::uintptr_t> mainWindowWidgetToken);

} // namespace m2m::doe
