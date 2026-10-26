# CP165 editor300 StepSize frontier

## Finding

The ordinary owned type-0x80 one-dimensional table profile (var0=1/2, ncomp=1, 1..4096 samples) is already present in the current converter source. It is therefore not a valid “next” increment. The smallest source-ready extension beyond it is **not established by retained evidence**: the next boundary is how Fill's definition callback handles multidimensional tables and nonzero variables other than var0=1/2, including their copy/storage meaning. Preserve the current refusal for those shapes until the exact missing native witness below is acquired.

Only this report was written. No source, test, shared plan/state, fixture, build, GUI, app-control, or live REA mutation occurred. The original is DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; this report does not claim a reconstructed DEF_PRE_64 instance or public Open/Save acceptance.

## Current source and retained evidence

Current `source/apps/moproj2mst/src/engine/simcontrol300.cpp` SHA-256 is `67b144f6df3e95843c82eae1a77389e5cfab8b65e2534419d3eae4e7f9d34dae`. `tableStepCurve` admits only type 0x80, ndim 1, ncomp 1, var0 1 or 2, positive n0 through 4096, no inactive counts/axes or u68, with aligned nonoverlapping object/axis/value ranges. `supportedStepCurve` composes this with the legacy constant profile. Fill/Apply/lifecycle guards, alias handling and constructor copy checks exist in the adjacent implementation; this read-only pass did not alter or execute them.

The retained CP156 contract `analysis/resume156-editor-stepsize-table-next.md` SHA-256 is `ecb28b90422c8a3a5865208c1aa57cf03b399e45638a9b90e49a513961853e43`. It binds Fill `[140ebb950,140ebc1e2)`, Apply `[140ebc1f0,140ebd178)`, definition callbacks, getter/setter and backed refresh by range hashes. Its source-backed conclusion is that getter `14089b4d0` maps ndim0 to definition0 and otherwise recognizes var0=1/2; setter `14089b450` writes ndim0 for definition0 and ndim1 with var0=1/2 for definitions1/2. It does not allocate, evaluate, or rewrite table data. For other multidimensional/variable states, the setter may collapse dimensionality or select constant while retaining counts/pointers, changing deep-copy semantics. The report explicitly keeps those shapes out of the accepted profile.

## Exact missing witness and guard

To extend the profile, recover one complete original trace from entry into Fill through its definition dispatch, temporary curve copy, and the matching Apply tail for a real `DYNARY` with ndim>1 or a nonzero variable other than var0=1/2. The trace must establish, from the original code and actual called copy/allocation routines:

1. which definition combo index is selected and what the setter changes;
2. whether dimensional counts, axes, values and ownership are preserved, replaced, or left stale;
3. whether Fill/Apply copies or evaluates those arrays, and exact ordering relative to mode/scalar writes and validation returns;
4. the concrete source/destination shape and alias/extent preconditions that make the path safe to model.

The retained evidence does not show those shapes are source-ready, nor does it establish a smaller independently meaningful partial transition. Keep unsupported shapes refused before new effects; do not infer safety from the accepted ndim1 case or reinterpret the resource cap as a native limit. Do not add an invented table format, widget/dialog state, or rollback.

## Acceptance if the witness is obtained

Bind exact native byte-range and retained decompile hashes for the newly followed callback/copy path, then have an independent source review reconcile it against CP156. A bounded implementation would need tests for the exact witnessed shape, bit-preserving data and metadata behavior, any partial Apply return, alias rejection before effects, and unchanged constant plus ordinary ndim1 var1/2 cases. Keep a negative test for adjacent unwitnessed shapes. Constructor tests must traverse the actual temporary-copy/backed-refresh/restore chain; direct Fill alone is insufficient. No test or source change was made in this investigation.
