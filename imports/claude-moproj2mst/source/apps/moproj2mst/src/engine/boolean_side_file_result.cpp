#include "m2m/mst/boolean_side_file_result.hpp"

#include "m2m/data/boolean.hpp"
#include "m2m/model.hpp"
#include "m2m/props.hpp"

namespace m2m::mst {
namespace {
[[noreturn]] void unavailable(const char* reason)
{ throw Error(M2M_E_UNSUPPORTED, reason); }
}

int nativeBooleanSideFileSequence(Project& project, Operation& expected,
    const data::BooleanData& data, const BooleanSideFileAttempts& attempts)
{
    // Data+10 maps to its actual BooleanProps owner in this adapter. An absent
    // owner is a native -1 branch; another property/data object is not absence.
    if (!expected.props) return -1;
    const auto properties = std::dynamic_pointer_cast<BooleanProps>(expected.props);
    if (!properties || !properties->data || properties->data.get() != &data)
        unavailable("Boolean side files: actual Boolean data/properties owner unavailable");
    Operation* const owner = properties->owner;
    if (!owner) return -1;

    // Validate membership before dereferencing the guarded raw owner address.
    bool member = false;
    for (const auto& operation : project.operations)
        if (operation.get() == owner) { member = true; break; }
    if (!member || owner != &expected || expected.project != &project ||
        owner->props.get() != properties.get())
        unavailable("Boolean side files: foreign Project/Operation/properties owner");

    // 5f8d90 resolves properties+20 through c1aef0. The existing actual OPI
    // identity is lifetime-bound; a committed QObject boundary resolves null.
    const auto guard = owner->operationInfoIdentity().guard();
    if (!guard.get()) return -1;
    if (guard.get() != owner)
        unavailable("Boolean side files: guarded operation identity differs");
    if (!attempts.bwz || !attempts.ini)
        unavailable("Boolean side files: both native writer attempts required");

    const auto bwz = attempts.bwz();
    // Native1902 saves BWZ's result;1938 attempts INI unconditionally. Do not
    // reinterpret returned open failure or replace the saved result with INI.
    (void)attempts.ini();
    return static_cast<int>(bwz);
}

} // namespace m2m::mst
