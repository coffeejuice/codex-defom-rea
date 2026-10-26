# CP187 R300 BCC scoped step implementation

`saveBcc` now confines the borrowed step pointer to an immediately invoked nullable callback containing the two synchronous `captureKey` calls. Slot filters, one model lookup per admitted slot, model/step-null refusal, object/mesh checks, path construction, ascending iteration, writer indices and RzElmCon-before-BccDef order remain unchanged. Acquisition stays after the object/mesh checks. The pointer has no use after the callback, and `captureKey` invokes its writer directly before returning. No geometry guard or aggregate Save admission was added.

## CP186 reconciliation

Before edits, both exclusive files matched their entries in `build/wave3-checkpoint186-verified/source.zip` byte-for-byte:

| File under source/apps/moproj2mst | CP186 and initial live SHA-256 |
|---|---|
| src/engine/save_current.cpp | 35e58afda87b9beab10c39b74197dd6a0bde0c99627351a7e8db01df2212f3ab |
| tests/s/test_save_current.cpp | 5c965f9622c72e1970ea1ebdf9f7a466db2ade6584519741a6fe42549934410e |

The frozen source already contains the CP186 `saveInterObject` immediate callback and its direct helper fixture. The only production delta is the `saveBcc` callback. Original CRLF production and LF test line endings are retained.

## Fixture acceptance and direct helper test

The standard corpus scan found no object simultaneously declaring DefineBCC=1, ObjectType=5 and HasMesh=1. R46 `46_MO2_Boolean_Oprations/MO2_Boolean_Oprations.moproj`, operation 2 (`SfOprHeatTransferExpress2D`), slot 0 supplies the existing inherited type-5 meshed workpiece with DefineBCC=0. The initial focused run failed the actual loaded mesh requirement: inherited loading uses `GuiModel::loadObjectKeyGeometry`, which explicitly deletes the imported mesh before `llobj_copy_keep_number`. The XML declaration is therefore not evidence of a loaded mesh. The corrected fixture imports the real copied `Objects/Object00001.KEY` (RZ 1204 nodes, ELMCON 1104 elements) through the existing full `GuiModel::loadObjectKey` API at object number 1 after normal operation loading. Its `key_import_object` reads through a constructed scratch step, selects a non-kind-7 object, gets/creates the destination, and copies with `llobj_copy_keep_number`. The test requires the actual post-import mesh predicate and object-list identity. The operation loader explicitly refuses a preexisting inherited BCC definition; the test sets the admission flag only afterward. No zero-filled object or mesh is introduced.

The new Testkit case `save_bcc_step_scope` requires the loaded slot, actual engine object, actual mesh predicate, kind !=7 and object-list number-1 identity before setting DefineBCC. Other slots' DefineBCC flags are cleared in this discarded project. It calls `saveBcc` directly through a test-only forward declaration, asserts exactly two captured paths (`RzElmCon00001.KEY`, then `BccDef00001.KEY` in the task directory), nonempty bytes and no deletes. Separate disabled-flag and wrong-type probes assert no effects. A noncopyable RAII detached-step guard invokes only `saveBcc` while detached, requires no effects, then restores and asserts the original model, step and object identities plus mesh presence. This is a synthetic helper fixture; it does not assert BCC content matching a native run.

## Source qualification and preflight

Read `source/docs/re/moproj2mst/s/spec.md` sections 1–2 and the actual `task_key_writers.cpp` bodies: both writers open text output, emit the banner, resolve objects by number, then require kind !=7 and mesh. RzElmCon writes nodes/elements and returns 0; BccDef writes edge/face and local BCs and returns its block result. The existing port closes return-3 files where the native implementation leaks a handle. These existing qualifications are unchanged; this patch establishes neither complete error-path parity nor fresh native byte equality.

Static preflight confirms existing Testkit `main`, supported REQUIRE/CHECK macros, no C assert, no new includes, and resolvable helper/model APIs. Paths use public taskDirAbs plus explicit filenames because slotKey has anonymous-namespace linkage. No missing-file probe was added. No build or test was executed by this owner. Root's first focused run failed the initial fixture mesh assertion, prompting the full-object import correction described above. Root must rerun the corrected test. No GUI, REA database, deployed binaries or shared state were changed by this owner. Root retains focused/integrated build, regression and freeze verification.

Final source SHA-256: `46265debeae2d39625cdc4e33e08970eab0bbe8540b6752a1015fa36f3f1ae36`.
Final test SHA-256: `0854785340253d8b78d396945a66e75f3cb56bbba2797aefb135e7cdc368e9ee`.

The two source files are released for root integration and independent review.
