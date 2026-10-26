# Resume 50: PPV point lookup, duplicate flags, and list ownership

## Scope and evidence

This is a source-qualified static contract for the PPV lookup and flag-list
mutation paths. No executable, source, build, GUI session, or mutable REA database
was changed or used.

Immutable target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (image
base `0x140000000`). Retained decompilation `evidence/gui_mo/s1/mdt/L0/A/f0.c`
has SHA-256
`a873ed9deb32842d55f45bc51f7f8166766979dfbd2c1dbe2d99c2aab99e80ed`.
The reader receipt is `analysis/resume48-ppv-reader-receipt.txt`, qualified to
`evidence/gui_mo/s1/mdt/L0/A/b0.c` SHA-256
`cb28de6852d5b6279719d2576759a9dcc0f640e436bcdf846caa691622c58698`. The
caller ownership interpretation is recorded in
`source/docs/re/moproj2mst/mdt/L0/spec_A.md`.

## FindOrAddPoint contract

`FUN_140d4af00` receives a point-list reference and four values. It constructs
a 24-byte point value with fields at offsets `+0` and `+4` (integers), `+8`
and `+0x10` (doubles), then passes that value and the list to
`FUN_140d4bab0` (`0x140d4bab0`). The returned search result is tested for
non-emptiness. If empty, it appends the value using `FUN_140067140`, reads the
new list size, and returns `size - 1`. If nonempty, it returns the index from
`FUN_140d4b8c0`; it does not append another point on that branch.

Therefore the helper has **deduplicate-if-found** control flow: an item the
lookup considers equal is not added, and the existing item index is reused.
This does not establish what equality means. The retained qualified decompile
does not include the bodies of `0x140d4bab0` or `0x140d4b8c0`; the bounded raw
PE ranges are respectively `[0x140d4bab0,0x140d4bb61)` and
`[0x140d4b8c0,0x140d4b9e0)`. No retained source-qualified evidence establishes
whether equality compares all four fields, compares doubles exactly or with a
tolerance, or uses another criterion. Thus duplicate suppression is settled
only relative to the native lookup predicate; field equality is unknown.

## Duplicate shared rows and flag insertion

The reader calls `FindOrAddPoint(sharedList, ...)` for a row classified as
shared. If its flag-list argument is non-null, it then calls
`FUN_140d4bb70(flags, returnedPointIndex, flag)` unconditionally for that row,
including when `FindOrAddPoint` returned an already-existing point index.

`FUN_140d4bb70` is a `QList` insertion path, not an indexed replacement:

- on an unshared list it calls `QListData::insert(listData, index)` and writes
  the flag value into the newly created slot;
- on a shared list it detaches through the retained helper and writes the value
  at the resulting insertion position.

Consequently, for repeated rows whose point resolves to the same index, each
row inserts another flag at that index. Existing entries at that index and
later positions shift right; the prior flag is not overwritten. This is the
native mutation visible in the retained `0x140d4bb70` body. It does not by
itself prove whether such repeated flags are later consumed positionally or
whether repeated rows are expected in valid input.

## Ownership and call-site destinations

The PPV reader's arguments are distinct list roles, not aliases inferred from
the file name:

| Reader argument | Native destination established by caller | Mutation here |
|---|---|---|
| `param_1` | Operation-local point list at `OPI + 0x20` | Cleared after successful file open; receives non-shared points. |
| `param_3` | Current subproject database point list A, obtained by `0x14005a400` from `db + 0x19a8` | Not cleared by this reader; receives shared points. |
| `param_4` | Operation-local shared-flag list at `OPI + 0x28` | Not cleared by this reader; each shared row inserts at the resolved point index when non-null. |

For a row whose token 1 says `true`, the shared path is selected only when
`param_3` is non-null. Otherwise the row is added/looked up in the local list.
The local path does not mutate `param_4`. The operation-local list and flag
list are owned/reached through the OPI call arguments; the shared point list is
reached through the current subproject's DB object. The retained evidence does
not establish that the shared DB list is the same object as KEY NRECVL/ERECVL
`RecoveryEntry` state. Do not alias or serialize them as the same model field
without separate layout evidence.

## Current reconstruction boundary and smallest safe integration

Current C++ source still refuses PPV load in `src/engine/loader.cpp` and PPV
save in `src/engine/save_current.cpp`; `RecoveryEntry` remains a two-field
`{int,double}` type used by the KEY recovery serializer. It cannot encode the
native PPV point shape or this separate flag-list mutation.

The narrow safe boundary is a separate PPV state/API with (1) a four-field
point type, (2) distinct operation-local points, subproject-shared points, and
operation-local flags, and (3) a lookup result carrying the existing/new point
index. Preserve the native append-on-miss and flag insertion-at-returned-index
behavior. Keep loader/save unsupported guards intact until equality predicate
and model ownership are established. Do not broaden `RecoveryEntry` or use its
NRECVL/ERECVL serializer as a substitute.

Smallest next evidence query: obtain a bounded disassembly or qualified
decompile of `0x140d4bab0` and `0x140d4b8c0` from the hash-verified PE, including
their direct helper calls, to identify the lookup predicate and matched-index
calculation. Separately inspect only the native root-list accessor/layout
around `0x14005a400` if implementation needs to decide whether the DB-owned
point list has an existing C++ model counterpart. The current evidence is
enough to implement conditional deduplication and insertion semantics, but not
to reproduce point equality or safely connect the lists to current C++ state.

## Status

- **Settled:** point record shape; append on lookup miss; reuse matched index
  without append; shared rows insert a flag at the returned index; duplicate
  shared rows therefore insert rather than replace; local/shared/flag argument
  destinations and their clear/mutation behavior.
- **Unknown:** native point equivalence predicate; exact matched-index helper
  semantics; whether repeated shared rows are valid and how downstream code
  interprets the resulting shifted flags; mapping from native DB point storage
  to any current C++ state type.
- **Safe action:** keep the current unsupported guards and introduce no source
  changes until the bounded lookup-body and model-layout evidence is available.
