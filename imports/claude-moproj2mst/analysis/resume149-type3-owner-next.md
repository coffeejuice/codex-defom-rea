# CP149 Type3 owner audit: getter evidence and remaining integration gate

## Finding

The retained original evidence identifies the reachable ShapeOptim consumer of `SUB+0x28` more precisely than the earlier owner report: `FUN_14013d740` invokes an ordered set of virtual getters on that pointer and copies the results into ShapeOptim's fields and ordered geometry-name list. This is enough to specify what the consumer asks for, but it does not bind the opaque allocation at `SUB+0x28` to a constructible candidate object or establish the values those getters return after `.OPT` parsing. The private CP144 parser therefore remains disconnected; keep Type3 Open and Save guarded.

## Reachable owner chain

For the retained `DEF_GUI_MO_64.exe` (SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`), `analysis/resume125-type3-optimizer-owner.md` establishes that SUB constructor `FUN_1400b5470` separately creates the Type2/3 DOE manager at `SUB+0x20` and, for Type3 only, allocates a `0x50`-byte object using `FUN_140917ff0` and stores it at `SUB+0x28`. Type3 `SfDoeStudy` parsing obtains both, parses manager XML/InputTable through the `+0x20` object, constructs the `<Control>/<project-base>.OPT` path, and calls `FUN_140908630(path, optimizer-at-+0x28)`. That parser call establishes an actual path from project Open to the owner pointer, but the constructor and parser bodies are not retained; their initialized member state and parsing effects remain unknown.

The consumer in `evidence/gui_mo/s1/mdt/L7/d0.c`, `FUN_14013d740 @ 0x14013d740`, gets the active SUB and calls `FUN_1400b5ab0(SUB)` before dispatching through the returned pointer's vtable. The getter sequence is visible in that decompile:

- `vt+0x138`, then direct helpers `0x14091e0b0` and `0x14091e0f0`, then `vt+0x128` and `vt+0x118` populate ShapeOptim fields at `+0xcc`, `+0xd8`, `+0xd4`, `+0xd0`, `+0xc0`, `+0xc4`, and `+0xe8` (the last is derived from whether `+0xd0 > 0`). The `+0xc4` value is derived from `+0xc0` with the explicit threshold/modulo branches shown at `d0.c:1043–1058`.
- `vt+0x28` supplies a count. For each index, `vt+0x30` supplies a row pointer; `d0.c:1065–1118` dispatches on the row tag and copies row values into the checkbox/value/limit fields. The special tags are `0x3f2`, `0x3f4`, `0x438`, and `0x439`; the fallback path copies a string-like value and numeric fields. This is the ShapeOptim projection contract, not evidence for optimizer row allocation or row meaning. In the `0x439` branch the decompile tests the existing `OB+0x74` field when setting `OB+0x9e`; do not silently reinterpret that observed expression as a test of the just-copied `OB+0x78`.
- `vt+0xc0` supplies the geometry-name count. The caller iterates from zero to count minus one, calls `vt+0xd8(owner, QString-out, index)`, and appends each returned name in order to `OB+0xf0` via `FUN_140004b90` (`d0.c:1120–1125`). Then `vt+0x148` is stored at `OB+0xe0` and `vt+0x158` at `OB+0xe9` (`1126–1129`).

This gives the exact observable calls and copy order in the consumer. It does not prove that `DefDoeModel` in `evidence/gui_mo/s1/vtables.tsv` is the concrete class installed by `FUN_140917ff0`; the retained constructor body needed to bind that vtable is missing. A slot list by itself cannot establish getter semantics or the owner's member layout.

## Candidate boundary and precise missing evidence

The current candidate has no separate Type3 owner: `SubProject` contains the Type2 `doe` alias and `ownedDoeStudy`, and `makeSubProject` initializes them only for `type == 2`. `parseSubProject` rejects Type3 `SfDoeStudy`; `ShapeOptimClass::loadClassData` rejects `sp->type == 3`; and its Save route calls `saveAfterFiles` before `saveOperationFiles`, with the former refusing Type3. These guards avoid treating the `+0x20` DOE manager or CP144's `Type3OptimizerScalarState` as the opaque `+0x28` object. The downstream `listF0_` consumer also forms per-name `.GEO` paths, so populating only scalar parser values would still not implement the later Save/Open effects.

The smallest missing immutable evidence bundle for actual-owner integration is:

1. The bounded body of `FUN_140917ff0`, including its vtable assignment, defaults, direct constructors/allocations and failure branches; plus the Type3 SUB teardown path that releases `SUB+0x28`.
2. The bounded body of `FUN_140908630` and direct state-mutating helpers sufficient to connect the admitted `.OPT` records to those getter results, including the failure/partial-mutation paths. CP144's successful full-read ASCII/LF value parser does not close that mapping or the native streaming error prefix.
3. A concrete vtable binding from the constructor to the getter targets, and getter target bodies (or equivalent bounded observations) for the fields/list consumed above. In particular `vt+0xc0/+0xd8` must be tied to actual ordered names before the existing `listF0_`/GEO flow can be enabled.
4. The native ShapeOptim save effects for updating the optimizer and writing each GEO before relaxing the candidate's Save guards; the current candidate order (`saveAfterFiles`, then `saveOperationFiles`) is not a substitute for implementing those bodies.

Thus the reachable path is established, but the optimizer's concrete identity, ownership, parser-produced state, getter values and Save effects are not. The current private parser may be tested as a bounded value helper; it cannot yet be attached to `SubProject` or used to admit actual Type3 Open/Save.

## Qualification

Read-only review of retained PE-derived decompiles/specs, the CP125/CP144/CP145 receipts, and current candidate source. No source/test edits, build, GUI/original execution, or mutable REA action was performed.
