# Checkpoint 59 isolated Operation-to-PPV Save adapter

## Implementation

Added a separate adapter in `include/m2m/engine/simctrl_ppv_save_file.hpp` and
`src/engine/simctrl_ppv_save_file.cpp`. It takes an actual `Operation&`, checks
the native local-list gate first, and for a reached writer call obtains the
shared root through `recoveryListA(operation)` only when `prevOp` establishes a
predecessor. Optional operation flags remain optional. The adapter copies the
existing local/root `RecoveryEntry` data and flags into call-local snapshots,
maps entries to the existing `simctrl_ppv::Point` representation, and delegates
payload generation to the already-qualified `writePpvPayload` helper. It does
not mutate those model owners or write a physical file.

The path is derived from the operation task folder as
`cleanPath(taskDirAbs(op) + separator + "SimCtrl.PPV")`, not the separate
`SimCtrl.KEY` path. A caller must qualify that operation-task path and supply an
ASCII-compatible locale codepage; empty/relative paths and unknown codec states
refuse before the modeled QFile-open boundary. The open result is explicit:
unknown stops at the attempted-open boundary, failure returns no bytes and
false, success returns the payload and native true result. This is an injected
open observation, not actual QFile I/O.

The local-empty predicate is evaluated before path, codec, root/flag inspection,
or open. Thus even a nonempty root list returns `LocalListEmpty` with no open
attempt, matching original `0x140d4afe0`'s local `QListData::isEmpty()` gate.
Nonempty local state is guarded for missing owners, nonrepresentable signed
count/index values, a supplied flags list shorter than the shared list, and
unqualified path/codec/open state. When predecessor flags are null, the
existing payload helper preserves the native count-versus-shared-row asymmetry.

## Test coverage

Added `tests/mdt/test_simctrl_ppv_save_file.cpp` with Release-active Testkit
assertions over an actual read-only Oracle-B Project/Operation fixture. Cases
cover local-empty/root-nonempty short-circuit before path/codec/open, qualified
root-only gate, QFile-open failure without payload or owner mutation, successful
shared-before-local byte output from actual owner snapshots, malformed short
flags, unknown path/codec/open, no-predecessor local-only routing, and signed
count boundary checks without allocating huge vectors. The fixture input is
read only; assertions inspect model state and returned bytes.

No CMake or public `save_current.cpp` integration was made. Existing source and
test globs discover the new files after configure; root owns build/test
validation. No build or test was run in this lane.

## Qualification and limits

The original writer contract is bounded by `analysis/resume57-ppv-writer.md`
and the resolved local-list gate by `analysis/resume58-ppv-save-root-gate.md`.
The adapter represents the writer call contract and output payload only. It
does not perform the preceding SimCtrl.KEY write, physically open/truncate the
PPV path, commit bytes, inspect post-open stream/close status, or change Save's
ignored-helper-return behavior. The supplied ASCII-compatible codec profile is
an explicit adapter input; it does not claim a fresh observation of the
original process locale. Public PPV Save remains guarded.

No GUI, mutable REA, CMake, build, or physical fixture mutation occurred.
