#pragma once

namespace m2m::doe {
struct Study;

// Isolated 0x14091c820 value/state stage on coherent concrete header owners.
// Collect names with no keyword/geometry rows, then remove in collection order,
// clearing incoming links before concrete destruction and invalidating samples.
// No file access, reindexing or claim that earlier Save reconciliation ran.
// Public nonempty-variable Save remains guarded before this stage.
void pruneEmptyHeaderVariables(Study&);
} // namespace m2m::doe
