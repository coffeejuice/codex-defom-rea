# CP83 next converted consumers: object deletion and PDIE re-pick

Select deleteObject and repickPdie in cogging_save.cpp. They are two contiguous
void-result consumers of the actual model step, fit the existing withStep
contract and already have a focused deletion/PDIE regression. Their migration
needs no helper/header/result-policy widening and no public API/layout change.

This began as read-only preparation while CP82 was frozen. After its archive,
root approved the three-file source lane. The completed implementation/static
release below records that successor. No worker compiler/build/test execution,
GUI or mutable REA action was performed.

## Proposed exclusive write set

- source/apps/moproj2mst/src/engine/cogging_save.cpp: add private gui_model_access
  include and convert only deleteObject/repickPdie to void withStep callbacks.
- source/apps/moproj2mst/tests/mdt/test_cogging_save.cpp: retain existing deletion
  regression and add a small direct re-pick branch matrix/null-step controls.
- source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp: extend the actual parked
  entry matrix with direct repickPdie and no-op/PDIE/non-PDIE deletion variants.
- This report for final implementation/hash receipt if root later assigns it.

Keep cogging_save.hpp, engine.hpp, gui_model_access.hpp, engine_internal.hpp,
native model code and CMake unchanged. Root must archive CP82 and reserve the
three source/test files before implementation. Other cogging_save.cpp consumers
(router, model-size lookups and later geometry code) remain explicitly legacy.

## Exact source behavior and evidence

`deleteObject`, source @src140c7ca50, currently performs:

1. CP80 model barrier check, then n<=0 returns.
2. Count actual objects whose kind is not7; no step/count0 returns.
3. On actual S, call step_delete_object(S,n,&nullHit).
4. If nullHit, raise the existing unsupported corruption error after those
   already committed effects. Do not roll them back or move the error earlier.
5. Only if actual current PDIE equals n, set PDIE0, then call repickPdie.

Native-twin source `defmodel/src/gui_twins_delete/delete_object.cpp:31` establishes
the nested order: find object; absent returns; clear BC refs; remove inter-object
pairs; make object empty; optional u3bf8 payload vslot; trim empty tail. The
missing-object regression's no-op is therefore intentional, not an error to fix.
Preserve that optional callback and every partial effect. Do not replace deletion
with container erase or invent transactional model rebuilding.

`repickPdie`, source @src140c7b630, currently performs:

- Count non-kind7 objects through def_pre::step_count_objects (menu56_common.cpp:173).
  This count is not simply stored S.u1378.
- Count<2 selects first real object; count==2 selects the rigid search regardless
  of the current PDIE; count>2 checks the actual current PDIE object and only
  requests rigid search when that object is absent/kind7.
- Rigid search uses step_first_rigid (menu56_common.cpp:205). A positive result
  writes that ID; no rigid ID falls back to first real object.
- The fallback uses first_real_object_number (gui_twins/step_utils.cpp:22).
  No actual step suppresses the setter rather than creating a default model.

These source helpers read actual object lists/IDs. Keep them and the existing
guarded w.object/w.pdie/w.setPdie calls; do not substitute raw object arithmetic
or a rule that always picks a rigid object for count>2.

## Small mechanical shape and ordering

Each function returns existing GuiModelAccess::withStep(w, callback), with a
void callback parameter DB_STEP_DATA* nativeStep. A return of a void expression
preserves the public void signature. Replace only the selected bodies' w.step
tests/get()/dereferences with nativeStep. Delete's explicit top-level guard is
replaced by the access layer's private preflight.

Keep all n/count/rigid/first predicates and all setter/native-call ordering
inside the callback unchanged. deleteObject still invokes the real repickPdie
on its PDIE branch, so that inner function obtains a fresh checked borrow.
No pointer/reference is returned or saved beyond the synchronous scope. The
actual GuiModel unique step owns the same native object; native object deletion
changes its contents rather than reassigning the enclosing unique_ptr.

The initial borrowed pointer precedes only pure count reads before native
deletion. After deletion, retain model-based PDIE/accessor calls exactly as today;
do not rewrite them to use stale object pointers or a snapshotted old primary
die. Optional native payload callbacks retain their existing behavior and order.
This migration adds no reentrancy/concurrency or arbitrary external-owner
replacement guarantee; any such raw-alias effect remains outside the access
seam's supported lifetime contract.

repickPdie currently lacks an explicit entry preflight. Routing its mutation
capability through withStep makes a parked model refuse before even its count
query, including a branch that could otherwise return without a setter. This
is the existing adapter parked-model policy, not a newly claimed native error.
Ordinary nonpending empty/null-step behavior remains unchanged. Value-only pdie
and objectCount diagnostics remain available.

## Existing coverage and exact small additions

`test_cogging_save.cpp:182 delete_object_repicks_pdie` creates three objects with
kinds1/0/0, PDIE3 and independently checks:

- delete0 and missing5 leave three objects/PDIE3;
- delete3 trims to2 and selects rigid2;
- delete2 leaves1 and selects first real1;
- changing the remaining object to kind7 makes deletion a count0 no-op.

Retain those expectations. The target currently has3 cases, including the
independent vdie42 cogging Save geometry comparison. Do not add a second copy of
that large fixture.

Add one direct repick matrix using tiny real model/object lists:

| Input | Expected normal result |
|---|---|
| Actual step, no counted objects, a nonzero PDIE | First-real result0 is written. |
| One counted object | Its first-real ID is selected. |
| Two counted objects with one rigid | First rigid is selected even if current PDIE points to another valid object. |
| Two counted objects, no rigid | First-real fallback is selected. |
| More than2 counted objects, current PDIE valid/not-kind7 | Current PDIE is retained; no forced rigid selection. |
| More than2 counted objects, current PDIE missing or kind7 | First rigid, or first-real fallback when no rigid, is selected. |
| Missing actual step | No allocation/default/setter; existing null behavior returns normally. |

Use the existing real get_or_create/kind fields and source helpers, no fabricated
layout proxy or new model owner. The counted-vs-stored distinction should be
explicit in a kind7 row. Existing source evidence supplies these expectations;
root may run the added normal matrix against the unconverted baseline first if
a separate dynamic baseline is required, as CP82 did for its new false-read path.

In the existing actual parked viewer fixture, require refusal of direct
repickPdie and deletion of0, missing ID, current PDIE and non-PDIE. Preserve whole
step bytes, object/view counts, primary-die value, timers, index globals, model
tables and COW bodies. The existing non-PDIE check already exercises deletion's
earliest guard; the additional direct repick check validates its new access-layer
preflight. Do not invoke optional payloads or deliberately corrupt a list merely
to force a new nullHit experiment.

## Static, layout and ABI gates

Use function-bounded balanced-brace extraction to require zero `.step`/`->step`
member accesses in both converted bodies and check native delete/nullHit/PDIE0/
repick order plus count/rigid/fallback order. Keep the file's unconverted router
and geometry uses on its explicit legacy inventory; a whole-file zero claim
would be false. Callback result remains void, with no captured borrow escape.

No member declaration/type/order/access label, native class layout or public
signature changes. CP81's compile gates for void/bool access, rejected pointer/
reference results, native DB_STEP_DATA size/offsets, public step type and the six
C ABI declarations/version remain active. Public C header and abi.cpp hashes
must remain unchanged. Root compares DLL exports (names/ordinals/forwarders,
ignoring expected RVA changes), runs existing C ABI consumers and then completes
the full source-stable validation/archive.

Focused targets: m2m_mdt_cogging_save / CTest m2m.mdt.cogging_save and
m2m_mdt_viewer_box / CTest m2m.mdt.viewer_box. Worker performs static preflight
only if a source successor is assigned. No reader continuation/cursor retirement
or full raw-alias encapsulation is enabled by this group.

## Read-only starting source binding

Paths relative to source/apps/moproj2mst; SHA-256:

| Source | SHA-256 |
|---|---|
| src/engine/cogging_save.cpp | 816a907d7b7a82776d6c8911a9f89158209f5475327271fe2debe06d9ac48076 |
| tests/mdt/test_cogging_save.cpp | 2a8ab9979f7189bde22749916dc9c0d4692ec11ec0e940afd5ea03a410ebfa31 |
| tests/mdt/test_viewer_box.cpp | 994aba8a3d2afc1c1aa271740ef611aa843c00c429b744fa9f19af4c0f6de94f |
| src/engine/gui_model_access.hpp | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| include/m2m/engine.hpp | 77153701684b2644d0b470c23ba110c9ce51ad8cbabbc82ab008b8b9c174128f |

## Completed migration and focused checks

Root approved the source lane after the plan freeze. Both functions now return
existing withStep void callbacks and use the supplied actual nullable nativeStep
for their step tests/native calls. deleteObject's standalone preflight is replaced
by the access-layer check. Its n/count checks, actual native deletion, nullHit
error, current-PDIE predicate, zero write and nested real repick call remain in
the same order. repick's counted-object/rigid/fallback logic and actual model
accessor/setter calls are unchanged inside its callback. It now refuses a parked
model before the first count read.

No helper/header/API/result-policy change was needed. Other raw step uses in
cogging_save.cpp remain legacy; no pointer/reference is returned or retained by
either converted callback. No payload callback, partial-effect error or native
object cleanup was removed/repaired.

The cogging target now has4 Testkit cases. Its original deletion/PDIE and
independent Save geometry cases remain intact. One new direct repick matrix
checks10 small actual object-list inputs: empty, one, two with rigid, two without
rigid, more-than2 valid deformable PDIE retained, missing/current-kind7 PDIE,
no-rigid fallback, stored3/counted2 distinction, and all-empty kinds. Stored
object count/kinds and actual step identity remain unchanged. A separate actual
null-step control returns normally from re-pick/delete without allocating a
step or Viewer.

The39-case viewer target extends its existing parked matrix with deletion0,
missing99, actual PDIE, actual non-PDIE and direct repickPdie refusal. Its full
step/table/Viewer/mirror/timer/COW invariants remain around those attempts; value
diagnostics remain usable. No second pending fixture or new CMake target was
introduced, and this worker did not run the new tests.

## Frozen release and validation limit

Balanced-brace extraction checked zero raw step member access in both converted
bodies and their ordered count/rigid/fallback/native-delete/nullHit/PDIE0/repick
tokens. Static preflight passed for4/39 unique Testkit cases, entry points,
Release-active supported macros, quoted includes and clean whitespace. No C
assert/REQUIRE_EQ, catch/rollback or borrowed pointer ownership transfer was
introduced. Helper/model/C ABI source hashes were rechecked unchanged, preserving
the existing compile-time callback/native-layout/C-signature gates.

Final paths relative to source/apps/moproj2mst; SHA-256:

| Released file | SHA-256 |
|---|---|
| src/engine/cogging_save.cpp | 58918c5f9ac62d21a6a637ed5af0784cd1c932fc96b9de66de44a268f5aceb49 |
| tests/mdt/test_cogging_save.cpp | 7904836d3ec38843a283e4c8e2fa0d36cf87c2785afb8181f98eb0922c0b6db8 |
| tests/mdt/test_viewer_box.cpp | f6f12fb475836c482fdde3e74ab7d92dcd357ad6479419091352289705022d0d |

All three files and this report are released frozen. Targets
m2m_mdt_cogging_save / m2m.mdt.cogging_save and
m2m_mdt_viewer_box / m2m.mdt.viewer_box. Root owns review, build, focused/full
tests, ABI comparison and archive. No worker compile/runtime result or full
encapsulation claim is made; token measurement is unavailable.
