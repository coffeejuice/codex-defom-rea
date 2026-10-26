# CP82 next two converted step consumers: DB read entrypoints

Migrate readDbStep and readDbEntry in db_load.cpp to the existing bool-result
withStep seam. They form one small synchronous group, already have CP80 entry
guards and have independent normal DB fixture coverage. No access-layer/header
widening is needed. Keep the other db_load.cpp consumers explicitly legacy.

This began as read-only preparation while CP81 was frozen. Root subsequently
authorized the three-file source lane, requiring the normal false-path baseline
before production conversion. The staged baseline and final implementation
receipts below record that sequence. No worker compiler/build/test execution,
GUI or mutable REA action was performed.

## Exact proposed exclusive write set

- source/apps/moproj2mst/src/engine/db_load.cpp: include gui_model_access.hpp and
  convert only readDbStep/readDbEntry to bool-returning withStep callbacks.
- source/apps/moproj2mst/tests/mdt/test_db_load.cpp: normal false/true-resetViewer
  controls and missing-step state-preservation assertions in existing DB cases.
- source/apps/moproj2mst/tests/mdt/test_viewer_box.cpp: extend the actual parked
  entry matrix to both resetViewer values for both DB entrypoints.
- This report for implementation/static-release hashes if assigned later.

No engine.hpp, gui_model_access.hpp, native DB layout, reader/receipt, CMake or
public C ABI file needs to change. Root must freeze CP81 and assign the source
lane before implementation.

## Source-qualified behavior to preserve

Current db_load.cpp has these ordered bodies:

| Entry | Existing order |
|---|---|
| readDbStep, native0x140c7b870 | Model barrier; missing m.db returns false; resolve actual modelDbReader; missing reader returns false; lookup explicit requested step with query::step_index; missing index returns false; only then missing model step returns false; delegate to readDbEntry. |
| readDbEntry, native0x140c7b900 | Model barrier; missing DB/step returns false; when resetViewer is true, viewerClear then mirror-vector clear; write actual S.u1b58=1; resolve model reader; call native port fun_1401cc420(S,reader,index); nonzero status returns false; only normal success builds cache and furnace/primitive tables, reads entry step, rebuilds viewer and sets state100=1. |

The lower DB operation is the existing DEF_PRE/GUI twin, not a replacement
loader. No new PE/runtime claim is needed for a mechanical borrow migration.
Existing source qualifications remain attached to each helper and its @src map.

`query::step_index` in defdbio/src/db_query/queries.cpp:59 searches actual directory
records and returns-1 when absent. Do not substitute an operation ID, skip lookup
because the model step is null, or add a different extent/error policy here.
The physical receipt checks of the geometry query are a distinct bounded path.

## Minimal code shape

Each public entry immediately returns
`GuiModelAccess::withStep(m, [&](DB_STEP_DATA* nativeStep) -> bool { ... })`.
The access layer replaces that entry's standalone barrier call and performs the
same private check before obtaining the actual borrow. Its bool result policy
already supports these functions.

Replace only the callback body's m.step tests/dereferences/get() argument with
the supplied nullable nativeStep. Keep every other statement in its original
order, including the readDbStep lookup-before-null-step check and the optional
viewer/mirror prefix before u1b58. Do not move cache/table/state100 effects out
of the successful branch or add rollback/finally/error translation.

readDbStep still calls the real readDbEntry, so the inner call acquires its own
fresh checked borrow. Nested checks have no native effects. No pointer/reference
is returned, captured beyond the synchronous function, converted into a shared
owner, or stored as a new member. Locals for reader/index/native status remain
native-call locals. Both return types and public signatures remain bool.

## Borrow lifetime and limitations

The supplied pointer is the actual GuiModel unique step, which remains owned by
the live synchronous caller. The supported viewerClear path removes Views and
their representations; it does not reset/reassign GuiModel.step. If cleanup
throws at an unrecovered boundary, the callback propagates it before the later
DB writes, as today. The DEF DB-reader layer mutates/reinitializes that native
step's contents and does not own the enclosing C++ GuiModel unique_ptr. Cache
and the subsequent model-table/viewer calls remain in the same callback lifetime.

This migration does not add concurrency, reentrancy, asynchronous retention or
generation guarantees. A public raw field/pre-held pointer can still be changed
outside the supported entries. Do not claim that withStep pins a replaced native
step or revokes such aliases. CP79/80's durable parked barrier remains in force
and is never retired by external changes.

Keep modelDbReader's existing pre-effect guard and raw-reader API unchanged;
withStep must not be widened to pointer results merely to convert it. Likewise,
m.db, viewerMirrorPlanes, state100 and unconverted step uses elsewhere in this
file remain legacy surfaces in this checkpoint.

## Small focused regression matrix

1. **Parked model:** reuse the actual two-View/pending directory fixture. Call
   each DB entry with resetViewer=false and true, an actual in-range index/step,
   and require the existing unsupported error. Whole actual step bytes, u1b58,
   object/view counts, mirror state, Viewer, tables, timers and COW bodies remain
   unchanged. The true branch must not clear Views or mirror values; the false
   branch must not enter the native DB reader. Repeat after the existing external
   cache/version/Viewer changes if useful without duplicating the fixture.
2. **Normal DB success:** extend db_handle_open's last stored COG entry check
   and/or db_step_read_vs_key_export's step-2 check. Prime a normal model with
   the already covered true read, then repeat the same actual step with false
   so the latter uses established Views/Settings. Preserve expected
   object count, u744, nstart/tmax, material/object names and state100. Compare
   those meaningful outputs to the existing independent deformdb/KEY evidence;
   do not compare every byte of newly allocated pointers. Confirm the added
   false-path fixture's outcome against the unconverted baseline before claiming
   normal success: if a later viewer branch is unsupported there, retain its
   same refusal/partial effects rather than forcing this migration to repair it.
3. **Normal early false:** retain no-DB controls. With the real loaded reader,
   request missing step99999 and assert no model/Viewer/mirror mutation. For a
   deliberately absent model step, preserve the original normal false result
   and the source's lookup ordering; do not introduce an access-layer null throw.
4. **Failure order:** native nonzero read status must still preserve preceding
   clear/u1b58/native partial effects and omit later success writes. Do not
   fabricate an out-of-range reader failure fixture merely to test this. If no
   independently qualified DB failure input is already available, retain the
   literal source order and leave a new failure experiment queued.

Existing tests at test_db_load.cpp:176/212 already check true-branch success;
line232 checks missing-step false. The parked viewer matrix currently checks
false at its two DB calls. This group fills the complementary paths without a
new target or a second synthetic pending-state implementation.

## Raw-access, layout and ABI gates

After conversion, neither function body may contain m.step/model.step access;
the only native-step reference in those bodies is the callback parameter.
Use function-bounded balanced-brace extraction/source review for this check:
db_load.cpp contains intentionally unconverted raw uses, so a whole-file zero
claim would be false. Preserve CP81's whole-file zero-raw-step gate for the
already converted class_slots.cpp. Check callbacks do not retain nativeStep or
return a pointer/reference.

All GuiModel/native data declarations remain unchanged. CP81's compile gates
for DB_STEP_DATA size/offsets, unique_ptr step type, nonpolymorphic GuiModel,
void/bool-only callback results, C API version1 and six public function types
remain active. Public moproj2mst.h and src/dll/abi.cpp must retain their frozen
hashes. Root compares DLL export names/ordinals/forwarders to CP81 (RVA changes
are expected) and runs existing C ABI consumers; no new numeric GuiModel layout
constant should be invented for a function-body-only migration.

Focused targets are `m2m_mdt_db_load` / `m2m.mdt.db_load` and
`m2m_mdt_viewer_box` / `m2m.mdt.viewer_box`, followed by root's full source-stable
validation/archive. Worker performs static preflight only if implementation is
assigned. No C ABI version bump or new public operation is warranted.

## Current source binding

| Current source | SHA-256 |
|---|---|
| src/engine/db_load.cpp | 62dd38add3e0af1c00ed07827ff41cbf010064ca63cee6a085e4f9558b4d93a9 |
| src/engine/gui_model_access.hpp | e05944babf29b0477452ca497d7df836ac57fa822056088e4c693a079465fa26 |
| tests/mdt/test_db_load.cpp | d8914d203ce875ead2172ff0711a1d07f5a367df16f94a56b522f476481a5329 |
| tests/mdt/test_viewer_box.cpp | 128755279c3e6c5bac972a03b012f01e32617e3f7a2e55db2b917c7e3f6c71ed |

These paths are relative to source/apps/moproj2mst and identify the proposal's
starting baseline. Token measurement is unavailable.

## Authorized baseline snapshot before production conversion

Root agreed to build/run the added false-path control while db_load.cpp remains
unconverted. Only test_db_load.cpp was edited: db_handle_open first executes its
already covered true read, then rereads the same actual last stored entry with
resetViewer=false. It requires success and checks the actual step/Viewer
identities, independent deformdb object count and step number, state100, and
unchanged read-only DB bytes/stamp. Its outcome remains to be observed by root;
the assertion is not a passing-test claim.

Frozen test-only SHA-256:
`67184f31ea0a2e92c958cdaf96b2d44bf599aa87060e17cc6fabdb7fab4f3117`.
Production db_load.cpp remains at
`62dd38add3e0af1c00ed07827ff41cbf010064ca63cee6a085e4f9558b4d93a9`.
Target m2m_mdt_db_load / CTest m2m.mdt.db_load has7 cases. Static Testkit entry,
macros, quoted includes and whitespace passed. All source edits are held until
root returns the observed baseline result; no production conversion or parked
test extension has been applied yet.

## Observed baseline and completed migration

Root returned a source-stable baseline build and a passing7-case
m2m.mdt.db_load run (reported0.75s) while production db_load.cpp still had the
unconverted62dd38... hash above. Evidence:
`analysis/resume82-db-false-baseline-build.json`, SHA-256
`9439aff22feb3c3e9a0d9bb8c47b0f325b6b8eaaa334b6a4811cb2498430af42`.
Root then released source for the mechanical migration. This is baseline
evidence, not validation of the final migrated candidate.

Both DB entries now immediately return existing GuiModelAccess::withStep bool
callbacks. Their callback bodies use the supplied nullable nativeStep for the
same tests, u1b58 write and native reader argument. The original standalone
guards are replaced by the same private preflight inside the access layer.
The DB lookup-before-null-step check, optional viewer/mirror clear, reader/status
return and success-only cache/table/rebuild/state100 sequence are unchanged.
There is no catch, rollback, new default owner, retained step pointer or result
policy widening.

The baseline control remains intact. Additional normal coverage calls
readDbStep(...,false) on the same actual last stored entry and checks its
independent object/step/state output plus actual step/Viewer identity. A model
with the real DB owner but absent step returns false in both entries and both
reset modes, without creating a Viewer, clearing its mirror sentinel or writing
state100. Missing requested step99999 leaves the whole actual native step bytes
and Viewer unchanged under both reset modes.

The actual parked viewer matrix now calls both DB entries with both reset modes.
It retains a controlled external mirror sentinel as well as its existing whole
step/table/Viewer/timer/COW checks. This verifies that the true branch does not
clear state and the false branch does not enter the native reader while parked.
The external legacy vector mutation itself does not retire the persistent frame.

No new native-reader-error fixture was invented. Source-order checking preserves
the preceding effects and early native-status return; success-path writes still
follow that return. Existing helper/model/C ABI headers and ABI source retain
their frozen hashes, and CP81's compile-time callback/layout/signature gates
remain in the viewer test. No public operation, class storage or native layout
changed. Other raw members and unconverted functions in db_load.cpp remain
documented legacy surfaces.

## Final static preflight and frozen release

Balanced-brace function extraction verified zero `.step`/`->step` member access
in the two converted bodies and checked their ordered lookup/null/clear/mirror/
flag/native-status/success tokens. Both callback results remain bool and no
pointer is returned/stored beyond the call. Test files retain7 and39 unique
Testkit cases, main, supported Release-active macros, resolved quoted includes
and clean whitespace. No C assert/REQUIRE_EQ was introduced. The helper,
engine.hpp, public C header and abi.cpp hashes were explicitly rechecked
unchanged. No worker build or final runtime test was run.

Paths relative to source/apps/moproj2mst; final SHA-256:

| Released file | SHA-256 |
|---|---|
| src/engine/db_load.cpp | 82b2262da2c793d75ecf61af0cca9c59b77038752cfe836330a0ca7586d63920 |
| tests/mdt/test_db_load.cpp | db6436546a76ab0f4d45af16745f968a3d3d39df0fa39e3e47bd18f3a1e04183 |
| tests/mdt/test_viewer_box.cpp | 994aba8a3d2afc1c1aa271740ef611aa843c00c429b744fa9f19af4c0f6de94f |

Targets m2m_mdt_db_load and m2m_mdt_viewer_box, CTest m2m.mdt.db_load and
m2m.mdt.viewer_box. All three source/test files and this report are released
frozen for root's grouped build/review/test/ABI gates and archive. No full
encapsulation or revoked raw-pointer capability is claimed.
