# CP114 — Native Geometry row construction and owner contract

## Object chain

In the `0x14090ee20` Geometry branch, the reader first obtains the selected Var from the SfMoDoeManager. `0x14090f0c5` gets the manager variable count through slot `+0x178`; `0x14090f0ef` calls slot `+0x1a0` with `count-1`, storing the returned Var pointer at `[rsp+0xa8]` (`0x14090f0f5`). Geometry metadata is applied to that Var, whose geometry-row list is at `Var+0x28`.

## Native row key and call arguments

At `0x140911f99`, the reader calls `0x1408175c0` on the selected Var with:

| Argument | Native source/value | Row field/role |
| --- | --- | --- |
| `RCX` | selected Var `[rsp+0xa8]` | owner of the row list |
| `EDX` | parsed `GeoOperation`, `[rsp+0x210]` | row key at `+0x1c` |
| `R8D` | parsed `GeoObject`, `[rsp+0x21c]` | row key at `+0x20` |
| `R9` | joined `NominalGeoFile` path, `[rsp+0x230]` | nominal string at `+0x00` |
| stack `+0x20` | joined `UpperGeoFile` path, `[rsp+0x250]` | upper string at `+0x08` |
| stack `+0x28` | joined `LowerGeoFile` path, `[rsp+0x248]` | lower string at `+0x10` |

`0x1408175c0` checks existing rows by exact upper string (`row+0x08`), lower string (`row+0x10`), GeoOperation (`row+0x1c`), and GeoObject (`row+0x20`) (`0x14081762c..0x1408176b3`). Nominal path is **not** in this key. On a match it returns without updating/inserting that row. On a miss it allocates `0x38` bytes (`0x1408176ce`), calls row constructor `0x140818790`, copies nominal/upper/lower strings and writes the two integer keys, then appends the row pointer to this Var's `+0x28` list (`0x140817720..0x1408177b6`). This corrects the earlier typo `0x1408178790`: the actual constructor target is `0x140818790`.

## Constructor defaults and field initialization

The bounded constructor body `0x140818790..0x140818807`:

- default-constructs QString members at offsets `+0x00`, `+0x08`, and `+0x10`;
- initializes container members at `+0x28` and `+0x30`;
- sets byte flags `+0x18` and `+0x19` to zero;
- does not initialize `+0x1c` or `+0x20` in the visible body; insertion writes both before list append.

The reader then gets `count-1` from the Var's geometry list and calls `0x140817f40` / `0x140817fa0`, which bounds-check and write entry flags `+0x18` / `+0x19`. On the normal append path this is the new row. On the dedupe path it is still the list tail, which can differ from the matched row; this is observed order and must not be silently corrected by an implementation.

The byte supplied for `UpperDefined` (`+0x18`) starts false. When the XML attribute exists, it is false only when its string is exactly `"false"`; any other value yields true. If it is absent, the code sets it true iff the joined upper path is nonempty. The `LowerDefined` byte (`+0x19`) is initialized true immediately before its attribute test. When present, it is false only for exact `"false"`; when absent, a nonempty lower path sets it true, while an empty lower path leaves it at the preinitialized true value. These are the observed branches, including the asymmetric missing-attribute fallback.

After flag setters, `0x140817890` writes parsed `MorphMethod` to the Var's `+0x20` field (`0x1409111ffe`). It is variable-level state in this path, not a member of the `0x38` row. Upper/Lower `PointPair` children are traversed later and submitted to Var methods `0x140818000` and `0x140818080`; they are separate row data effects after the insertion/flags/morph sequence.

## Destruction and append order

Manager destruction owns the Var records; Var destruction reaches `0x140817d10` for its geometry list. That routine walks rows, calls wrapper `0x140818810` with delete flag 1, and then erases the corresponding pointer from the list. Wrapper `0x140818810` calls row destructor `0x140818850` and `operator delete` when flag bit 0 is set. The row destructor destroys list members `+0x30`, `+0x28`, then QString members `+0x10`, `+0x08`, and `+0x00`. The Var list wrapper is destroyed after row traversal. This closes normal-path ownership for the row and all fields allocated by the row constructor.

The retained range establishes the allocator-null branch and ordinary normal insertion order, but this report does not claim complete C++ exception/unwind cleanup parity for failures during QString assignment or list insertion. Native exception tables and injected allocation failures were not inspected.

## Implementation boundary

The candidate's legacy `Var::files` / `GeoFile` can carry three paths and two flags, but it has no per-row GeoOperation/GeoObject keys or correspondence collections. Appending an unkeyed `GeoFile` would lose the native dedup key and make `geometryVars(opId,objId)` unsourceable; the existing `GeometryVariable` metadata projection is B122-specific and does not own the row. The source-closed implementation contract therefore needs a typed Var-owned row representation for key, paths, flags, and correspondence lists before any actual append/dispatch integration. Keep the public pre-QFileInfo refusal until a separately assigned owner lane integrates that typed representation and the conditional path projection.

No new isolated helper/test was added here: a path-only helper is owned by the CP113 DOE lane and cannot represent this row key/append/tail-flag contract. A pure row parser alone would not exercise the owner mutations or payload-loading edge.

## Evidence and limits

- Original GUI PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- Reader argument setup and branch: `analysis/resume50-doe-variable-body-pe-annotated.asm`, `0x14090f0c5..0x14090f0f5`, `0x1409117a3..0x140911ffe`.
- Row insertion body: bounded original PE `0x1408175c0..0x1408177d0`, previously captured in `analysis/resume105-geometry-receiver-methods.md`.
- Constructor: bounded original PE `0x140818790..0x140818810`.
- Flags: bounded original PE `0x140817f40..0x140817f90` and `0x140817fa0..0x140817ff0`.
- Destruction: `analysis/resume91-doe-variable-lifetime2-140817d10.asm` plus bounded original PE `0x140818810..0x1408188af`.
- Static disassembly only; no runtime, GUI, mutable REA, source edit, test or build.
