# CP81: paired serialized-parent Save test and source release

Add one higher-level candidate test through the existing private full-conversion entry. It should prove that an existing unreadable object-property side file leaves its ObjPropertiesKey parent in the physical serialized project, omits only its Data child, and does not turn that local omission into a project Save failure. No new production hook or overlay manipulation is needed.

The plan below was prepared read-only. Root subsequently froze CP80 and authorized exclusive implementation of the new test file. The test is now authored and statically checked, with no build, executable test, GUI, original runtime or mutable REA action. The CP80 review correctly states that helper coverage alone does not empirically inspect a serialized parent or later Save effects; this authored higher-level case still needs root execution before that gap can be called covered.

## Exclusive ownership and bounded invocation

Assign one worker only `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst/source/apps/moproj2mst/tests/s/test_project_blob_lifecycle.cpp`. Keep the existing helper tests, shared test_save_io.cpp, private/public headers, production writer, coordinator and CMake untouched. Existing discovery should create target `m2m_s_project_blob_lifecycle`, CTest `m2m.s.project_blob_lifecycle`.

One TEST_CASE, suggested name `save_read_denied_object_blob_omits_data_but_serializes_parent_and_continues`, performs exactly two fresh conversions: unlocked control and read-denied case. Each conversion gets a new Rig/session, factory and scratch copy; no live Project is reused. Invoke `m2m::detail::convertDetailedWithServices` with M2M_FLAG_WRITE_SAVE, codepage 1252, a separate native-output.MST path, output-only ConversionDetails and the existing per-call ProjectOutputFactory.

Required includes resolve relative to the new test: `../support/test_paths.hpp`, `../../src/core/conversion_call_services.hpp`, `../../src/moproj/project_output_device.hpp`, plus existing public FileDevice/blob/XML headers and Testkit. Use only REQUIRE/CHECK/CHECK_EQ with one main; no raw assert, skip, diagnostic bypass or OK-or-unsupported allowance.

## Exact disposable fixture and target

Copy `M2M_ORACLE_B/133_B_DBSTEP_BOOL3_before` into a fresh unique child of configured M2M_TEST_SCRATCH. Use COG_P1_B1.moproj, active Boolean Task00002, and the inactive operation's side file `Operations/Task00001/ObjProperties00001.KEY`. The selected serialized path is:

`Project / Operation_List / Operation[child Id == 1] / Objects / Object[@Id == 1] / ObjPropertiesKey`.

The selected operation is HeatTransfer / SfOprHeatTransfer. It has one selected object with a Data child in the retained input; select by ID, not incidental child indices. Require exactly one matching operation/object/key so an absent target cannot pass as successful omission.

Read-only fixture inspection for this plan established:

| Input | Bytes | SHA-256 |
|---|---:|---|
| COG_P1_B1.moproj | 25,644 | 8e5e896cce67164430324cddc25324043449428b789d201bb3bef9c49cc89f79 |
| Operations/Task00001/ObjProperties00001.KEY | 169 | 16edca97bc62d6979c2db3af30917c13264bb54af7e2d0529aacecc734a458dd |

The original embedded selected Data was decoded offline with standard zlib: it is exactly those same 169 physical side-file bytes and hash. Read those expected bytes from the read-only fixture for assertions; root's usual fixture manifest binds them. No whole-project output hash or compressed-byte original oracle is invented here.

The native full-Save context sets nativeOperationBlobs before Open. `props_keys.cpp::storeBlob` writes embedded operation bytes physically and erases their overlay key on successful native open. This makes the disk branch available without clearing overlays. The target is in inactive Task00001, avoiding the active Task00002 saveCurrent output. At factory entry, strictly require the copied target bytes equal the 169 expected bytes. If the denied run still serializes Data, fail and investigate the actual virtual-source path; do not erase overlays or earlier effects to force a pass.

## Lock timing and real project output

Reuse the design of the existing delegating ProjectOutputDevice, with all output status values delegated to the real FileDevice. No scripted status 7, seed buffer, changed write chunking or output preflush belongs in this test.

At factory.open, after input Open and saveCurrent have completed but before project DOM/list serialization:

1. Require exactly the expected normalized project output path and one factory invocation. Record the copied target bytes before acquiring the denial handle.
2. Construct the real project output FileDevice and require actual opened state.
3. In the denied profile only, acquire a second Win32 handle on the side-file target with read/write access and share mode zero. Require successful acquisition, qt::fileExists true and an actual ifstream probe failing to open. The denied object is the side KEY, never the main project or MST.
4. Return a delegating adapter. Forward the real FileDevice aboutToClose callback to the outer Stream callback only when emitted by the real lower device. Delegate write/flush/close/opened/error unchanged.
5. Hold side-file denial throughout operationList/blobElement, the later sub-project DOM construction and physical project serialization/Close. At the real error query, record actual closed state, actual error and physical project bytes while the denial is still held. Verify later BWZ/INI/MST/remesh outputs are still absent at this point.
6. Release the side-file denial handle in the error-query observation after collecting that snapshot, before returning the actual error zero to the coordinator. This prevents this intentionally narrow read gate from also faulting a later MST reader. Teardown must release it on every exceptional exit as well.

The observation callback should record facts rather than execute throwing Testkit assertions inside a destructor/callback path. Keep observations and RAII ownership in per-call factory-owned state that outlives the returned device. No handles or callbacks escape conversion.

## Required serialized bytes and omission

Parse the bytes captured from the physical project after close, and separately read the same physical project after conversion. Require both reads to match and parse successfully, proving the assertion is on executed file output rather than only the in-memory DOM or copied fixture.

For both profiles require the uniquely selected Operation, Object and ObjPropertiesKey nodes. The parent key and object attributes must remain present. In control, require exactly one Data child and `qUncompress(fromHex(Data.text)) == expected169Bytes` with no decompression warning. In denial, require no Data child and no other children in that selected key. The candidate serializer's exact local omission form is `<ObjPropertiesKey/>`; readable-empty would instead contain `<Data>00000000</Data>` and must not be accepted as omission.

After checking control Data, remove only that Data from a parsed copy of the selected control Object. Serialize that selected Object subtree for each profile using the same XML writer and require equality. This checks that the intended difference is the Data child and that the parent/object metadata were not dropped or otherwise changed. It avoids making an unmeasured assertion that complete project files with different scratch roots are byte-identical. Do not use a global absence search: other objects legitimately retain their own Data nodes.

## Required ordering and Save result

Record ordered factory/open, real aboutToClose while open, real close returned and real error query; require one of each and closed state at the query. Side-file denial is held at project close/query and released before the converter resumes. Real project post-close error is exactly zero in both profiles, so projectWriteAttempted/Returned/Succeeded must all be true. A blob omission is not a failed project-output result.

Use the already qualified explicit remesh fallback `LOC_DEN_CTRL.DAT` with FILTER_DISTANCE 7 and INTERNAL_ELEMENT_SIZE_CONTROL_METHOD 4. Require BWZ/INI/MST/remesh paths absent before conversion and at the project error query. After both conversions require:

- Task00002/Boolean.BWZ contains WZD_BOOLEAN_CUTTER_OBJECT_NO and Task00002/DEF_MACHDIST_CTRL2.INI contains WZD_MACHDIST_OPERATION_BOOLEAN.
- MST contains END OF MASTER FILE and MST Attempted/Returned/Succeeded flags are true.
- LOCAL_REMESH.DAT contains the known KEYWORD_DRIVEN/method-4/filter-distance-7 records.
- Those four later output byte strings equal the fresh unlocked control's corresponding outputs.
- Result.code and details.returnedCode are exactly M2M_OK in both runs; saveCall is present and has the expected project/MST flags. No project-write error is expected from an omitted blob.
- After lock release, the target side file remains equal to the expected 169 bytes. The test denied reading, not rewriting or deleting that input.

These observations close the CP80 reviewer gap for the candidate's native full-Save parent serialization and later effects. The private entry uses the same run() as public C++ entry points, but this is not an executed C ABI or ordinary public-entry fault test. It is neither withBlobs archive support nor original-runtime parity. The known timer/DOE/Close acceptance qualifications remain separate.

## Integration gate

CP80 is frozen and root assigned this one test source. Static include/Testkit preflight is complete; the released source remains held unchanged for integration. Configure must discover the new target. Group it with the CP80 blob helper test, project-output device test and Save-I/O regression, then run the required full source-stable launcher and immutable source/artifact/fixture freeze. One new target with one case is the expected isolated test-count increment; reconcile other authorized lanes before asserting the final total. No runtime validation was performed by this worker.


## Released implementation receipt

Only new `source/apps/moproj2mst/tests/s/test_project_blob_lifecycle.cpp` and this report changed. Test source: 331 lines, one TEST_CASE and one Testkit main, SHA-256 `e46843638a2cd527ccc0c8c2454ea42743f3776e41d634f423aecc43017fa879`. The test follows the two-profile contract above: native full conversion, no status override or seeded output buffer, a strict read-denial probe, and physical parent/output checks. The selected operation also requires Type SfOprHeatTransfer and Name HeatTransfer.

The locked side-file handle is owned by a factory-local RAII object, retained through the actual output close/error snapshot and released at that error observation. Failed acquisition/probe or unreleased handle is a REQUIRE failure, not a passing diagnostic branch. Callback/destructor paths record observations instead of throwing Testkit assertions. Later converter stages run only after the denial has been released. The final selected-object comparison removes the verified control Data node from the parsed control tree only; it does not modify a fixture or any on-disk output.

Quoted includes were checked against each source-relative path and the existing component include directory. The new file has no unsupported REQUIRE_EQ, raw C assert, skip, status-alternative allowance or trailing whitespace. XML serialization source was inspected to confirm the exact empty-parent fragment `<ObjPropertiesKey/>\n`. These are static checks, not compiler or runtime acceptance.

Frozen CP80 baseline receipt records source tree `ec108f046e509be9dcf4181be5e1cd11303f3aa0b3a9b7906a5183dca3553c57` and source archive SHA-256 `486a24bfd4ce58dc75694d98b8231bec5b09a04a80c2395c30644ac1f9aeced1`. Inspected production dependencies remain unchanged: write.cpp `8af4be80324c39e1f8f452d7723bebd200e4b7372180d8e32c1e6ee32c2399a2`, conversion_call_services.hpp `8fc9fe687a5a3a187d9f913fc7ba1b5ae62bfdeb97975a9ebcedf1ffdf492a6a`, and project_output_device.hpp `586132f1b480112b5b3d4152b35aca508114fe65a93e786d0b11d2ba8cbec669`.

The source is released to root and will receive no further edits until results are reported. Root owns configure/discovery, focused execution and the exact full source-stable suite/archive. No source-bound runtime pass, public C ABI coverage, original parity or overall completion is claimed by this release.
