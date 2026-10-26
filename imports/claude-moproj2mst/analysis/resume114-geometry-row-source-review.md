# CP114 Geometry row-prefix independent source review

## Review result

No source-level blocker found in the frozen typed row-prefix implementation against `analysis/resume114-doe-geometry-row-contract.md`. The model fields preserve the native four-part dedupe identity, nominal path is excluded from that identity, unknown legacy keys refuse before mutation, and post-lookup flag/MorphMethod updates follow the native tail/order contract. This approves only the isolated receiver prefix; public Geometry admission, correspondence consumption, payload I/O, and runtime equivalence remain outside this increment.

## Evidence

- `source/apps/moproj2mst/include/m2m/mst/doe_model.hpp:15-25` keeps the row in `GeoFile` under the existing `Var::files` owner. It adds `geoOperation` and `geoObject` as optional fields. The optional state distinguishes an unknown key in pre-existing projected rows from a known native-key projection; it does not add a parallel row collection.
- `source/apps/moproj2mst/src/doe/geometry_row_prefix.hpp:9-19,29-33` defines an owning input value with explicit operation/object/morph integers and absent-versus-present optional attribute strings. The API accepts a concrete `HeaderVariableRecord&`; its comment correctly bounds it to the existing row owner and calls out refusal for unknown legacy keys. `:21-26` marks the next boundary as correspondence payload rather than claiming it consumed that phase.
- `source/apps/moproj2mst/src/doe/variable_record.cpp:41-45` checks every existing row's two optional key fields before any write and refuses with `M2M_E_UNSUPPORTED` if either is unknown. Lines `47-53` perform ordered lookup on `before == upper`, `after == lower`, operation and object only. Nominal is explicitly excluded at `:50`; the check is consistent with the contract's first-match behavior.
- On a miss, `variable_record.cpp:54-65` builds a local row, assigns nominal/upper/lower plus both key fields, and appends it. On a match it does not overwrite the existing nominal. Lines `67-75` calculate `count-1`, set both flags on that tail, then assign `geoType`/MorphMethod and report the correspondence boundary. This matches the source-qualified native sequence, including a duplicate that matched a non-tail row.
- The focused test `source/apps/moproj2mst/tests/doe/test_geometry_row_prefix.cpp:61-85` directly covers a two-row list, a duplicate of row zero with a different nominal, preservation of row-zero nominal, and flag/MorphMethod updates on row one. Lines `87-106` cover each key component and string content equality; `:126-145` cover refusal with either/both key fields unknown and verify list size, flags, and MorphMethod are unchanged. `:147-155` confirms the public parser still refuses before appending a Geometry row.
- `source/apps/moproj2mst/CMakeLists.txt:130-137` discovers `tests/*/test_*.cpp` through `GLOB CONFIGURE_DEPENDS`; the new DOE test therefore needs no per-test registration edit.

## Scope and residual limits

The test's actual Study owner is constructed through `readInputTable` (`test_geometry_row_prefix.cpp:27-33`), then the isolated helper receives that record. The test does not establish a production Geometry caller or admission path. The helper intentionally stops before correspondence and does not populate `Var::geometry` metadata (`geometry_row_prefix.hpp:31-32`; implementation receipt `resume114-doe-geometry-row-prefix-implementation.md`, “Next dependency”). The current source-level review did not run a build or tests; root owns validation.

I did not find a native-contract mismatch in the requested areas: key composition/nominal exclusion, unknown-key refusal, tail flag target, MorphMethod order, or correspondence boundary.
