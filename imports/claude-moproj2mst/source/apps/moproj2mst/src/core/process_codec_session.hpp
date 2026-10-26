#pragma once

#include <optional>

#include "m2m/common.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/engine/process_codec_owner.hpp"

namespace m2m::detail {

// Internal run-construction seam, not a native observable or codec admission.
// Called once with empty stack storage inside run's try. Construction failures
// therefore retain the existing catch mapping and no Session move/heap is added.
inline engine::ConversionSession& emplaceProcessCodecSession(
    std::optional<engine::ConversionSession>& storage, engine::DiagramEffectDriver& driver,
    engine::ProcessCodecOwner& processOwner, unsigned flags)
{
    if (flags & M2M_FLAG_WRITE_SAVE) storage.emplace(driver, processOwner);
    else storage.emplace(driver); // Legacy internal Sessions are also unbound.
    return *storage;
}

} // namespace m2m::detail
