# Resume 54: PPV point-list owner and model boundary

## Finding

The retained native evidence identifies the PPV destinations as the existing
OPI and data-root list slots, rather than proving additional adjacent PPV list
members:

| Native slot | PPV role | Current model holder |
|---|---|---|
| OPI `+0x20` | operation-local point records | `Operation::recoveryB` |
| data root `+0x19a8` | shared/root point records | `Project::recoveryA`, or `SubProject::ownRootRecoveryA` for type 4, through `recoveryListA` |
| OPI `+0x28` | shared-row flag list | `Operation::recoveryFlags` |

The OPI constructor/default evidence (`0x14004fcd0`, `0x140050550`, retained
in `evidence/gui_mo/s1/s1-01/spec.md`) initializes the `+0x20` and `+0x28`
list slots. The PPV caller evidence in
`source/docs/re/moproj2mst/mdt/L0/spec_A.md` binds those same addresses to the
local list and flags, and binds DB list A at `+0x19a8` to the shared list when
a predecessor exists. The root accessor implementation in
`source/apps/moproj2mst/src/moproj/accessors_p01.cpp` selects project recovery A
or the type-4 subproject's own root recovery A.

The KEY writer evidence in `evidence/gui_mo/s1/s1-02/spec.md` independently
shows the same root and operation slots consumed as lists of native `BC*`:
root A is the DB/data-root list, local B is OPI `+0x20`, and flags are OPI
`+0x28`. For `BC`, KEY reads the first integer at `+0` as type and the double
at `+0x18` as value. This is an important ownership reconciliation: the PPV
reader and KEY serializer address the same native lists. They must not be
modeled as two parallel vectors with independent lifetimes.

## Record shape and safest model addition

PPV `FindOrAddPoint` constructs a 32-byte native record. The retained bounded
DUMPBIN receipt for `0x140d49e30` establishes initialization of type/int at
`+0`, the PPV second integer at `+4`, two PPV doubles at `+8` and `+0x10`, and
zero at `+0x18`. The PPV caller then overwrites `+0`, `+4`, `+8`, and `+0x10`
from the parsed row, leaving the KEY value at `+0x18` zero. The equality
routine (`0x140d4b9e0`, `analysis/resume52-ppv-dumpbin.md`) compares the two
integers exactly and both doubles with strict `abs(delta) < 1e-6`; lookup uses
the first matching list index. These details prove that the first and last
fields used by KEY coexist in the native point record, while PPV adds three
fields between them.

The smallest safe model extension is therefore to enrich the existing
`RecoveryEntry` in `include/m2m/model.hpp`, preserving its current `type` and
`value` members and their meanings for KEY, and appending default-initialized
PPV comparison fields (second integer and the two doubles). PPV-created
entries map the parsed first integer to `type`, set `value` to the native
constructor's zero, and store the remaining three PPV key fields in the new
members. Existing KEY entries retain their current `type/value` behavior and
default values for the additional members. The local/shared lists remain the
existing `recoveryB` and selected root recovery-A holders; flags remain the
existing optional `recoveryFlags` vector.

This is a logical model extension, not a C++ binary-layout overlay. The current
`RecoveryEntry` is a semantic `{type,value}` structure and its field order is
not the native `BC` byte layout. A change must preserve existing aggregate
initialization/API call sites (or update them deliberately) and PPV equality
must use the new named fields, never reinterpret the C++ object as native
bytes. Do not add a second PPV list to `Operation`, `Project`, or `SubProject`:
that would split state that the original stores in the same native list slots.

## Cleanup and ownership limit

The model's current C++ cleanup is explicit: `Operation` holds
`shared_ptr<vector<RecoveryEntry>> recoveryB` and optional
`shared_ptr<vector<bool>> recoveryFlags`; `Project` and `SubProject` hold
shared pointers for their respective root recovery-A list. Destruction of
these C++ holders releases their vectors. This is current source behavior, not
proof of the original Qt object's destruction semantics.

The retained native evidence locates the list fields and shows OPI
initialization, plus the lazy creation of the type-4 data root. It does not
establish the complete deleting-destructor path for OPI `+0x20/+0x28`, nor the
data-root/DB-handle deleting-destructor path for `+0x19a8`. In particular, it
does not show whether the native point-list destructor also deletes pointed-to
`BC` records or merely releases the QList container. Do not claim those
cleanup details from QObject ownership or the field offsets alone.

To close that limit, the smallest additional static evidence is bounded
disassembly of the OPI deleting-destructor/vtable cleanup target(s) for
`0x1421247d8` (including the base-deleting-destructor path) and the data-root
DBH destructor/cleanup target reached when the root at `+0x19a8` is released;
include the QList destructor/free helper only if the caller shows it is
responsible for point elements. No such cleanup receipt is present in the
retained sources inspected for this audit. Until that is recovered, preserve
the existing shared-pointer model ownership and do not add speculative custom
deletion.

## Integration boundary

This ownership finding removes the earlier assumption that PPV must necessarily
use unrelated, parallel point-list owners, but it does not authorize wiring the
parser yet. `RecoveryEntry` cannot currently represent the native PPV key, the
shared `recoveryFlags` insertion behavior must remain index-aligned as observed,
and public Save plus load integration need their own reviewed change. Keep the
public PPV guard until the model extension, actual loader binding, count
propagation, and paired Save behavior are integrated and validated. Do not
place PPV rows in the separate KEY recovery vectors while simultaneously
maintaining a second PPV copy.

No source, test, build, GUI, or mutable REA changes were made for this audit.
