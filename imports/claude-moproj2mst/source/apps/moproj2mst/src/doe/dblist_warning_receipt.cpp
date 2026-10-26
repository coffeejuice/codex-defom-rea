#include "m2m/doe_dblist_warning_receipt.hpp"

namespace m2m::doe {

DbListWarningReceipt makeDbListWarningReceipt(
    bool hasMissingDatabase, int warningState,
    const std::vector<qt::String>& missingDbNames,
    std::optional<std::uintptr_t> mainWindowWidgetToken)
{
    DbListWarningReceipt result;
    if (!hasMissingDatabase || warningState == 5) return result;

    result.shouldDisplay = true;
    result.title = qt::L("Error in reading");
    result.messagePrefix = qt::L(
        "The following Database files referred to in the Session file could not be found:\n\n");
    result.missingDbNames = missingDbNames;
    result.parentWidgetToken = mainWindowWidgetToken;
    return result;
}

} // namespace m2m::doe
