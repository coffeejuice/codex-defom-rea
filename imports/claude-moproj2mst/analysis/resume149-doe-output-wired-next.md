# CP149: wire the existing Output directory attempt and actual operation-model lookup

The next source-ready increment belongs in the existing `saveCurrentOperation` DoeOutput branch: execute the already-qualified immediate `Control/Regions` attempt, preserve a failed attempt at the warning-modal boundary, and on success execute the existing current-operation model getter before stopping at Regions enumeration. This wires an existing effect into its actual Save caller. No new isolated helper, viewer snapshot, task selection, successful public Save or cleanup implementation is needed.

This report is read-only. No source, test, build, GUI, runtime original or REA operation was performed. Root must assign the shared caller before implementation. The CP142 private helper and historical CP143 reports remain unchanged.

## Two corrections to the historical CP143 interpretation

`resume143-doe-output-owner.md` correctly preserves mkdir-failure modal continuation, but its model identity and one Save-order sentence need correction from the bounded native caller and current source:

- The first model lookup after mkdir is **0x14005f770**, not 0x140d0d670. Native 0x140e2e730 calls its current-operation accessor 0x1405ef2b0 at 0x140e2e76b, passes that result to 0x14005f770 at 0x140e2e773, and saves the result at stack+0x30. A null result at 0x140e2e77d destroys the local QString and jumps to the callback epilogue at 0x140e2ec26. It returns from the entire callback; it does not merely skip Regions cleanup and continue with Common_Mesh deletion or `.DS` output.
- Current-operation Save precedes project-file InputTable/refresh. Candidate `engine/save_project.cpp:68` calls `saveCurrentOperation`; `saveProject:74` first calls that whole `saveCurrent` path, then reaches `moproj::saveProjectFile` at line79. Thus the historical claim that public refresh precedes the DoeOutput Save class branch is incorrect. Native order in `wave2-doe-lifecycle.md` already records the correct relationship.

The later 0x140d0d670 call is at 0x140e2eb79, after directory cleanup, Common_Mesh handling and task-related probes, within the eligible geometry-output branch. It obtains the display's ROIs model and stores its marker. Requiring that later ROI-model result as the first lookup would move a later dependency ahead of an earlier source-closed prefix.

## Exact native order at this cut

1. `0x140e2e752 -> 0x140e33210` derives the active subproject Control/Regions path and calls shared directory helper `0x14081afb0(path,true)` at `0x140e333ac`.
2. The helper immediately attempts QDir::mkpath. With warnings enabled, failure enters the one-button modal warning; after that modal returns the helper returns true. The caller does not use the physical mkdir Boolean as a terminal Save result. CP142's private `WarningModalRequired` correctly stops before that unrepresented return, as qualified by `resume143-doe-output-mkdir-continuation.md`.
3. After helper return, the callback constructs a local QString and gets its actual current operation and its operation model at `0x140e2e76b/773`. A null model returns the callback as described above.
4. For a nonnull model, it re-fetches the active subproject at `0x140e2e79d`. With that subproject present it obtains Control at `0x140e2e7bd`, constructs the Regions path/QDir/list inputs and calls cleanup `0x14081b400` at `0x140e2e8f9`. The first enumeration/path-object construction is the next unimplemented effect, before any deletion can be claimed.
5. Common_Mesh removals, ROI choice/cached geometry gating, selected session/task probing, current-operation property application and `.DS` output follow. Native geometry later calls `0x140d0d670`; current-operation helper `0x1400543a0` is at `0x140e2ebe4`, then class vt+0x80 at `0x140e2ebfc`.

A failed mkdir cannot simply fall through to step3 in the candidate: actual modal delivery and return are missing. Nor can the warning be classified as original abort. The preserved partial effect is the failed immediate attempt plus an explicit pending warning boundary. No modal dismissal token is invented.

## Existing owner and implementation seam

`engine/loader.cpp:168` already implements `opModel(Project&, Operation&)` for native 0x14005f770/0x14005f820. It asks the actual OpClass class-model slot first; otherwise it uses the operation's `engineModel`, lazily calling the same class's createModel when absent. This is an actual producer, not an arbitrary supplied GuiModel pointer. DoeOutput inherits the base class-model null return and has its own createModel at `ops/noop.cpp:203` for 0x140e32a30. That method uses its real current operation/project and creates the model with the operation's dimension. It is already the Open-side model producer.

The existing `saveCurrentOperation` preconditions require a real OpClass, `ob->current == &op` and a nonnull operation project. Add the bounded active-project identity/owned Type2 Study checks required by CP142 before its attempt. The mkdir helper validates actual active SUB, owned Study, alias and backlink and derives the path from that owner. Do not allocate another Study or infer one from a document snapshot.

After successful attempt, call `opModel(p, op)` on that same operation. Preserve an existing model's identity; allow only the existing qualified lazy producer when missing. Do not substitute `displayRoiModel`, `Session::ModelManager::currentModel`, or `Study.outputDocument`. ModelManager's current model is a separate database/current-MO owner, not this GuiModel. No new manager selection or binding is established by this prefix.

On a nonnull return, guard before Regions QDir construction/enumeration and all deletes. A native null return ends the callback; preserve that behavior only for a faithfully reached existing getter return, without adding an injectable null profile or promoting a corrupt owner to native success. Earlier invalid/current-class cases retain their existing no-dispatch behavior. Exceptions during the existing getter retain the directory attempt already reached; do not roll it back or invent native allocation-failure parity.

For non-immediate/recording FileEffects, retain the pre-attempt guard from CP142. Queuing `mkdirs` and then asserting the actual attempt happened would be incorrect. Root's real native Save path already sets `saved.step2.nativeDirectories=true` in `convert.cpp:339`; the record-only route needs its own separately qualified device behavior before this effect can be admitted.

## Remaining lifecycle and manager barriers

The candidate retained B120 active DoeOutput Open still stops while importing `.DS` at DBLIST/ReportList. Even a synthetic/empty document profile encounters the later selected-session/global-task application guard in `DoeOutputClass::loadOperation`. Wiring the Save prefix does not make those Opens return. A direct test of the real engine Save dispatcher is valid coverage of this bounded caller, but must not be described as successful original or end-to-end retained Open-to-Save acceptance.

The `.DS` Save owner still needs the actual active subproject's lazy document, its selected SfTaskDoe and linked manager/model updates. Existing Study-owned `outputDocument` is a load-side projection; it cannot be copied into a new Save document or treated as a callback-return proof. Likewise an empty region list or scalar ROI value does not qualify a cached common-mesh object or a cleanup implementation.

No optimisation/task effect can be moved ahead of the pending cleanup or modal boundary. Later Type2 DEF_DOE.INI/design/task work follows the project/main-MST sequence. Type3 owns a separate optimizer and `.OPT` output. Existing private INI/PAR and Type3 helpers do not authorize these later calls here. For non-DoeOutput active operations, the existing Type2 XML/refresh and nonempty-variable Save guards remain independently earlier in their own paths.

## Exact implementation proposal for root assignment

Reserve existing `source/apps/moproj2mst/src/engine/save_current.cpp` and NEW `source/apps/moproj2mst/tests/doe/test_output_save_prefix.cpp`, plus a unique implementation receipt. Reuse existing `src/doe/output_regions_mkpath_prefix.hpp/.cpp` unchanged. No Study/Session/model/shared header, loader, emitter, C ABI, output-data, geometry, SPRING2 or CMake edit is required by the proposed source cut. If an old guard assertion needs repair, root should own that separate test change.

The shared branch should replace the blanket DoeOutput refusal with: validate the actual bounded owner; call the existing attempt helper on the supplied `fx`; on failed result refuse explicitly at warning-modal delivery; on success call existing `opModel`; preserve faithfully reached null return; otherwise refuse explicitly before Regions enumeration. It must not invoke propsToEngine, `.DS` serialization or the generic base save sequence after that guard. Do not catch and erase the reached attempt.

Focused tests should invoke **engine::saveCurrent/saveProject**, using actual registered DoeOutput OpClass/current operation and active flow selection, caller-owned SaveEffects and an owned Type2 Study. A test of only the private mkdir helper is insufficient.

- Success: under checked test scratch and native-directory profile, reach one successful immediate Regions attempt, preserve the selected operation/model identity, then receive the exact cleanup-enumeration guard. Verify no project write attempted, no XML/refresh journal event, no queued writes/deletes or later task mutation. An existing sentinel entry in Regions remains because cleanup was not reached.
- Failure: make the exact Regions target a regular file. Retain the failed attempt and blocker; receive the warning-modal guard. Verify no new model construction after that failed attempt and no project/cleanup/task effects. If Open fixture construction already needs a model, record its identity before the call and verify it is unchanged.
- Lazy model: where the existing actual class fixture can faithfully leave engineModel absent, success runs the existing getter/producer before the cleanup guard. Test no arbitrary model argument and no ROI named-model creation. This is existing candidate value/owner semantics, not Qt allocation parity.
- Ownership/no-dispatch: a mismatched Study/active SUB refuses before new I/O; current-class mismatch follows the existing dispatch precondition. Do not synthesize original null-return conditions through corrupt pointers merely to cover an unreachable test branch.

These are implementation-ready acceptance criteria for a wired partial effect. Root retains grouped build/full-suite authority. This report requests assignment, not execution of source changes.

## Compact evidence receipt

Original GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Bounded read-only DUMPBIN `/DISASM:NOBYTES /RANGE:0x...,0x...` decoded the actual entries and source branches; no rel32-only inference was used for the corrected gate.

| Original byte range | SHA-256 |
| --- | --- |
| 0x140e2e730 through RET 0x140e2ec2f, 1280 bytes | bbbb74ac8c45bb9cd45aa70276e2f7fdb8874dc4db5d2c44a86a263ae2d96a5e |
| 0x140e33210 through RET 0x140e333d0, 449 bytes | 65e093c667ef45866e075500419115d7690d04cf24ae2473a90ff1592c05a769 |

| Current candidate file | SHA-256 |
| --- | --- |
| src/engine/save_current.cpp | ba944dade590fd73cbe16c95a2a83d7f50868b2f1b05370bd477f7611d7f9812 |
| src/engine/save_project.cpp | 90a0ddbf6b4c92b4f4fe8bc106aa2e9248f31c84d2a0981d0ee10eda0baf2337 |
| src/engine/loader.cpp | 746c6855de4831c15d8ad04f01df859d1e48a6ee5a1b16fd327c994d0fad9e7c |
| src/doe/output_regions_mkpath_prefix.cpp | f6b57d765e271745254d582a98f594875a05981aae4a5098962e6980a57bfab7 |

The retained `wave2-doe-lifecycle.md`, `resume122-shapeoptim-mkpath-original.md`, CP142 plan/implementation/review and CP143 qualification remain evidence for shared directory-modal behavior and private effect limits. CP149's corrections apply to the first model lookup, whole-callback null branch and placement before project-file Save; they do not broaden later cleanup/task/document qualification.
