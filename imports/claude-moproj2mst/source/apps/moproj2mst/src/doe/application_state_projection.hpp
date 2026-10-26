#pragma once

#include <cstdint>
#include <optional>

namespace m2m::doe {

struct ProcessDwordObservation {
    enum class Knowledge { Unavailable, Known };
    Knowledge knowledge = Knowledge::Unavailable;
    std::uint32_t value = 0;

    static ProcessDwordObservation known(std::uint32_t value) noexcept
    { return {Knowledge::Known, value}; }
    bool isKnown() const noexcept { return knowledge == Knowledge::Known; }
};

// Application-scope projection, deliberately not owned by ConversionSession.
// Only the retained mode-0 process-type refresh is modeled. The DBLIST +0x1c
// admission value remains unavailable because its setter/reset lifecycle is
// unresolved; this class has no API that invents that transition.
class ApplicationStateProjection final {
public:
    ProcessDwordObservation processType() const noexcept { return processType_; }
    ProcessDwordObservation dbListAdmission() const noexcept { return dbListAdmission_; }

    // Source-qualified mode-0 refresh at 0x140013d40:
    // no current Project -> 5; Project with native-open-success -> 5;
    // Project without native-open-success -> 1. nullopt is valid only when
    // currentProjectPresent is false (the Project+0x55 byte is not read then).
    void observeMode0ProcessTypeRefresh(bool currentProjectPresent,
                                        std::optional<bool> nativeOpenSucceeded);

private:
    ProcessDwordObservation processType_;
    ProcessDwordObservation dbListAdmission_;
};

} // namespace m2m::doe
