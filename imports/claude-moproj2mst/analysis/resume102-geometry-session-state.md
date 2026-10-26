# CP102 registered Session geometry ownership and remaining occurrence

The actual registered Session retains operation geometry through its Mo child
after Project storage is released. A new focused test now covers that owner
chain and final teardown with nonempty retained geometry. No production source
changed. This is adapter ownership/cleanup, not native normal Close, query
failure recovery or a later CP97 barrier continuation.

## Actual Session path versus original Close/query state

core/convert.cpp creates one ConversionSession and, for WRITE_SAVE, registers
the actual Mo DbHandle before Project construction/read/selection. Save calls
saveCurrent/saveCurrentOperation through the existing source-qualified chain.
There is no normal-Close callsite after Save. ConversionSession's destructor
expressly performs adapter exception/final cleanup, not original Close effects.
releaseProjectAfterCloseEffects requires diagram null binding but does not
decide Close, perform nested Save or certify native notifications.

releaseProjectStorage captures actual Display, removes ProjectScope, retains
the actual DB child across project_.reset and clears its moProject backlink.
With actual registration, ModelManager still owns that DbHandle, which owns
the same MoModelSlots capsule and its Operations/GuiModels. Those models need
not die at Project reset. Session member order destroys ModelManager before
registry/scene/Display leases. DbHandle's adapter destruction releases its
capsule; capsule cleanup destroys Operations before manager tables. Models
therefore run their real destructors while Display/registry remain available.
This differs from older unregistered two-model Session tests.

CP97's barrier stays private model metadata until model teardown. It has no
resume/reset/retirement path; recorded pointers are never dereferenced on
retry. CP98 found no query/cleanup discrepancy under the serialized closed
query contract, and no original reachable exception/SEH witness. Session
destruction does not turn that missing failure witness into native Close.
The process/main owner, current busy state, question/nested Save and accepted
notification/clear sequence remain CP98 Close dependencies.

## Nonempty Save effect already qualified; missing occurrence remains

Cogging Save has a source-qualified nonempty effect: Save-current base vt+68
invokes the class saveNames vt+d8; Cogging14075c260 calls geometryStep140762ec0,
which reads cached viewer geometry140766200 for objects1/2/4 before placement
140763e50 and manipulator updates140762f30. Billet dimensions come from the
existing float surface box, not a new refresh inserted before the read.
42_vdie's independent original Save oracle checks billet Length1112.5595703125
and Width/Height/Dia404.32000732421875 plus nonempty manipulator coordinates.

That existing cogging_save regression uses a standalone Project/ProjectScope,
not an actual registered Session Save-to-Close observation. Static XML
inspection found no DBLIST/ReportList occurrence in42_vdie.vdie.moproj, so
that specific parser guard is not a demonstrated blocker for a future Session
Save integration test. Such a test must still drive the actual Open/selection/
activation/Save chain and record any earlier constructor/owner refusal; it
must not silently reuse standalone ownership as full Session acceptance.

No production writer0 of S.u1b58 was found in the examined managed path:
native step construction sets1 and MO readDbEntry's admitted process5 path
sets1. The CP97 actual-N flag0/high-version/cache0 state is explicitly supplied
by focused geometry fixtures. Existing copied KEY evidence does not establish
an optional geometry-list cross-section OBJ_MESH2D with live node/element owner
and populated arrays; a primary3D DIEGEO surface is different storage. Thus
the missing original occurrence is still a managed Open/Save session that
actually produces that optional mesh/flag0 state and reaches the query, then
an accepted Close on the same real process/main owner. No new native geometry
effect or barrier continuation is justified from current evidence.

## Authorized new test and static release

Root authorized only new
`source/apps/moproj2mst/tests/mdt/test_registered_session_geometry_lifetime.cpp`.
It has two Release-active cases under the new auto-discovered target
m2m_mdt_registered_session_geometry_lifetime / CTest
m2m.mdt.registered_session_geometry_lifetime. No CMake/shared edit was needed.

Both create actual Session registration/construction/selection and require the
DbHandle's capsule to equal Project.modelSlotsOwner. Two actual Operation-owned
GuiModels import retained42_vdie Object00001.KEY and require a nonempty primary
surface. The nominal DB/project identity is C:/PROBLEM/G102 (no project files
are generated); fixture input remains read-only.

The explicit-release case null-binds the diagram and calls the adapter storage
release. It requires ProjectScope restored/moProject null, while registered
child/capsule/two models and nonempty geometry remain alive, with no model
deletion yet and the capsule still at Operations storage boundary. The second
case throws a dedicated test sentinel before explicit release to exercise the
Session destructor's adapter unwind; this is not an injected native query
failure and supplies no original exception witness.

Both observe Display/registry/nonempty geometry/current-Project state immediately
before each real GuiModel destructor, then require every weak model/child/
capsule/Display/registry observation to expire after Session destruction.
Temporary diagnostic strong locks end before Session destruction; no external
strong manager/model/Display lease masks the owner chain. No native Close
effect driver, dialog, original process or private frame accessor is invoked.

Static checks passed: two Testkit cases, actual quoted/native include resolution,
run_all main, no REQUIRE_EQ/C assert, declared owner/release checks and no
production/shared modifications. Tests were not built or executed by this worker.
Root owns focused/grouped build and runtime acceptance.

## Frozen SHA-256 receipt

| File | SHA-256 |
|---|---|
| source/apps/moproj2mst/tests/mdt/test_registered_session_geometry_lifetime.cpp | 8b3b7972a735cfb7a8dd9f354a3c45510edb38efd0d830fab9e1484d64844eb3 |
| source/apps/moproj2mst/src/engine/conversion_session.cpp (unchanged) | f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604 |
| source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp (unchanged) | 46d4aaaba8128fc78d3d19b51158b319ec5959009eba48184194958e25a32e31 |
| source/apps/moproj2mst/src/model/project_owned_storage.cpp (unchanged) | cb21a607853408ebaae8050037c468f61f51b230a1f96c6705497fd0c9165797 |
| source/apps/moproj2mst/src/engine/model_manager.cpp (unchanged) | 76d7ec9f605fff55d8c7acb694ce1d66337aa43571277fd971883fb8649b3cd4 |
| source/apps/moproj2mst/src/engine/viewer_camera.cpp (unchanged) | 9deecc6d53f2dcd2c3d305ce46845bda9714f5e724d820e762206717992666a0 |
| evidence/gui_mo/s1/corpus/42_vdie/Objects/Object00001.KEY | 4ebb00899af733c12f34c6522ba8048928f9eede7dd2cffd241f39a59f6cdaf4 |

Retained qualifications: resume98-geometry-preinstall-failure-audit.md,
resume98-geometry-continuation-frontier.md, resume98-close-main-publication.md,
resume81-cogging-refresh-save-order.md, resume47-geometry-mesh-fixtures.md and
MDT L1B/V source contracts. Worker performed no build/test, GUI, original
execution, shared REA database access or REA mutation. Source/test is frozen
for root validation; no production change is proposed by this report.
