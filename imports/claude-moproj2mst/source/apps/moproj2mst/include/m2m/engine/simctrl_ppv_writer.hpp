// Isolated, source-qualified SimCtrl.PPV writer payload contract.
// This helper is not connected to Operation, Project, QFile, or public Save.
#pragma once

#include "m2m/engine/simctrl_ppv_reader.hpp"

#include <string>
#include <vector>

namespace m2m::engine::simctrl_ppv {

struct WriteResult {
    bool openAttempted = false;
    bool returned = false;
    std::string bytes;
};

// Models the bounded function's gate and successful-write payload. `initialPredicate`
// is the opaque return from native 0x140307410: true skips the QFile open and
// returns false. `fileOpenSucceeded` models QFile::open(WriteOnly | Text); it
// is ignored if the predicate skipped the attempt. After a successful open,
// native returns true without checking stream or close status. If both shared
// and flags are supplied, flags must contain at least shared->size() entries,
// matching the native caller's indexed access precondition. List sizes must fit
// the native signed-int count/index fields.
WriteResult writePpvPayload(bool initialPredicate, bool fileOpenSucceeded,
                            const std::vector<Point>& local,
                            const std::vector<Point>* shared,
                            const std::vector<bool>* flags,
                            unsigned localeCodepage);

} // namespace m2m::engine::simctrl_ppv
