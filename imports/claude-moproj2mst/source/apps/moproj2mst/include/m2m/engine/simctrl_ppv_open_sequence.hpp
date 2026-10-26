// Isolated QFile/decode, PPV parse, and caller-count sequence. Not public loader wiring.
#pragma once

#include "m2m/engine/simctrl_ppv_file_reader.hpp"

namespace m2m::engine::simctrl_ppv {

enum class OpenSequenceStatus {
    Applied,
    ParserReturnedFalse,
    OpenFailed,
    UnsupportedInput,
    OwnerUnavailable,
};

// Models the PPV portion after SimCtrl.KEY was found/imported. Call only when
// the caller's PPV-exists predicate is true. Unknown byte profiles are refused
// before model mutation; native public loader integration remains guarded.
OpenSequenceStatus readPpvOpenSequence(const Project& project, const qt::String& path,
                                       Operation& operation, bool hasPredecessor);

// Public native Open checks the physical QFile path, so it must read that
// same sidecar rather than a stale Project overlay entry.
OpenSequenceStatus readPpvOpenSequenceFromDisk(const Project& project, const qt::String& path,
                                               Operation& operation, bool hasPredecessor);

} // namespace m2m::engine::simctrl_ppv
