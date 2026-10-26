# CP269 `fillMeshing3D` owner implementation

## Result

Changed only the first admission term in `fillMeshing3D` from the raw nullable
step/dimension predicate to `model.dimension() != 3`. The term remains first in
the original short-circuit expression, before enabled scaling validation. The
scaling check and all later page construction, object enumeration, label sort,
initial-load transition, and remesh-control formatting remain in place.

Added actual-entry coverage for an ordinary 2D model and a model whose owned
step is reset. Both return `nullopt`; the 2D dimension and empty object count
remain intact, the missing step is not recreated, and every `RemeshState` field
is unchanged. The enabled scaling factor is finite (`2.25`), so scaling
validation cannot independently cause either refusal. A review caught and
corrected the initial test draft, which used infinite scaling and therefore did
not isolate dimension admission. This does not claim that externally supplied
page state can be observed: `fillMeshing3D` constructs its page locally.

Prior CP260 `applyMeshing3D`, CP263 `applyMeshingSelection3D`, and CP268
`transitionMeshingObject3D` dimension queries were preserved. No shared model,
header, C ABI, or other `simcontrol300.cpp` behavior was changed.

## Validation

- `python admin/check_new_cpp_bindings.py source/apps/moproj2mst/src/engine/simcontrol300.cpp source/apps/moproj2mst/tests/mdt/test_simcontrol300_meshing_selection.cpp` — passed, no issues.
- `python admin/build_candidate.py --label cp269-fill-meshing-finite-scale --target m2m_mdt_simcontrol300_meshing_selection` — exit 0; source stable during build.
- Exact CTest `m2m.mdt.simcontrol300_meshing_selection` — 1/1 passed (0.07 s).
- Build receipt: `analysis/cp269-fill-meshing-finite-scale-build.json`.
- Focused CTest log: `build/logs/cp269-fill-meshing-finite-scale-focused-ctest.log`.
- `git diff --check` — clean.
- No full regression build or suite was run in this owner phase.

## Hashes

SHA-256 after implementation:

| Artifact | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300.cpp` | `82b1777fe7845943ef7420b5d3a0c2e32bec780996ed8dc778a68c096975846c` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_meshing_selection.cpp` | `73b1ce3d01a7649c0d83b04d036bdde5c660090b82cd36f35215a2f5c250ffa6` |
| `build/candidate/bin/m2m_mdt_simcontrol300_meshing_selection.exe` | `b2b34fb3d1f85e531a898a8e3eece464bc28bb6917dd7a52118395e1522f08ec` |

## Limits

The check establishes the internal synchronous owner-query substitution and the
reconstructed actual-entry refusal behavior. It does not establish public
ordinary-MO UI reachability, native runtime parity, or uniform refusal for
parked 3D models. In particular, populated parked models still reach the
existing guarded `objectAt` path; empty parked models may fill without geometry
admission. The full application goal and previously recorded external GUI
release blocker are unchanged.
