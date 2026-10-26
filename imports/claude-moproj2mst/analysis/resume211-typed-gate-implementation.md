# CP211 typed-child gate implementation

## Change

Added `engine::detail::outerLoaderMoChild(DbHandle*) noexcept` as a candidate-only partial class projection. It returns null for null and Base handles and returns the identical pointer only for `Kind::Mo`. It reads no backlink, file path, read state, model ID, selection, or Project slots.

`ConversionSession::prepareProjectModel` calls this gate after `appendCreatedModel` and `registerModelSignals`, before publishing `p.dataDb`. Existing path assignment, append, and signal registration stay before the gate. The unexpected candidate mismatch throws the existing `M2M_E_UNSUPPORTED` internal invariant result. `initProject` and `commitProjectModelConstruction` remain later steps; the existing later ownership/backlink check remains unchanged.

The focused test checks null and Base rejection (including a Base with a populated Project backlink), identity-preserving Mo acceptance while unread and path-unset, and session ordering: two connections exist while the child is published with no Project backlink and no selected current model, followed by the existing construction commit establishing the backlink. This is a candidate contract test, not native runtime evidence.

## Source evidence

`analysis/resume211-outer-rtti-map.md` records the copied binary's RTTI descriptors `.?AVSfModelBase@@` and `.?AVSfMoModel@@`, a zero-offset `SfMoModel -> SfModelBase -> QObject` hierarchy, and a specific manager factory whose normal completed construction returns the Mo class. Its qualification explicitly leaves the original failing invocation's factory/cast reachability and failure cause unresolved.

`analysis/resume211-rtti-review.md` independently PASSes this fixed-image class mapping and candidate gate placement, and recommends kind-only semantics with null/Base/Mo internal cases. It expressly does not establish native factory/cast success. Candidate source evidence in `source/apps/moproj2mst/src/engine/conversion_session.cpp` shows model creation as `Kind::Mo`, path, append, signal registration, publication, and a separate post-`initProject` construction commit.

## Verification

Attempted Release build of `m2m_mdt_conversion_session` in `build/cp209-current-project` using CMake/Ninja. It failed before compiling the changed target because the configured MSVC invocation cannot find standard SDK/toolchain headers (`windows.h`, `cstdint`, `memory`, `cstddef`). Full output is retained at `build/cp209-current-project/cp211-typed-gate-build.log`. The focused CTest was not run because the executable was not built. This is an environment/toolchain setup failure, not a passing test result.

## Diff ownership note

This work edited `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp`, `source/apps/moproj2mst/src/engine/conversion_session.cpp`, and added the focused test in `source/apps/moproj2mst/tests/mdt/test_conversion_session.cpp`. The checkout already contained unrelated concurrent changes in `conversion_session.cpp`'s neighboring source context and in the test file (including current-project resolver tests); those are preserved and are not part of this CP211 patch. No original executable, GUI, deployment, shared plan, or native oracle was used.
