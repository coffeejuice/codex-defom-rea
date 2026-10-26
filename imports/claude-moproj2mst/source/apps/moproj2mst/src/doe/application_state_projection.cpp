#include "application_state_projection.hpp"

#include "m2m/common.hpp"

namespace m2m::doe {

void ApplicationStateProjection::observeMode0ProcessTypeRefresh(
    bool currentProjectPresent, std::optional<bool> nativeOpenSucceeded)
{
    if ((!currentProjectPresent && nativeOpenSucceeded.has_value()) ||
        (currentProjectPresent && !nativeOpenSucceeded.has_value()))
        throw Error(M2M_E_UNSUPPORTED,
                    "application process-type refresh requires source-qualified Project presence/open status");

    const std::uint32_t selected = !currentProjectPresent || *nativeOpenSucceeded ? 5u : 1u;
    processType_ = ProcessDwordObservation::known(selected);
}

} // namespace m2m::doe
