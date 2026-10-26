# CP81 independent review: serialized project blob lifecycle test

## Review result

The new `tests/s/test_project_blob_lifecycle.cpp` is well-scoped for the candidate behavior it asserts. Each profile creates a fresh scratch copy of `133_B_DBSTEP_BOOL3_before` and a fresh `convertDetailedWithServices` invocation. It selects the Task00001 HeatTransfer object-property side file by the copied fixture path and verifies its input bytes before arming any lock. The assertion targets one operation/object/key by IDs and requires those nodes to exist, so a missing path cannot pass as successful omission.

The denied profile creates the actual project `FileDevice` in `Factory::open` first, then acquires the side-key read denial. In `convert.cpp`, `saveCurrent` and step-2 effects precede native project writing; `Factory::open` is the project writer entry after that work and before `saveProjectFileBody` builds the serialized operation list. Therefore the side lock cannot interfere with project/input Open or step 2, while it is in place for the later `operationList` -> `blobElement` read. The factory independently checks file existence and verifies an actual `ifstream` probe fails. It does not clear Project overlays to force the disk path; the final Data assertion will detect if the path is still resolved virtually.

The output adapter delegates the real `FileDevice` and does not alter chunking, flush, or returned error. Its `aboutToClose` callback records that the project file is still open and forwards the real callback to the stream. At the post-close error query it captures the closed state, actual error and physical project bytes while the side denial is still held, then releases the denial before returning the real error to conversion. This is the right ordering to test “omit only embedded Data, complete project write, then continue later Save stages.” The lock release is RAII-protected on conversion/test failures; a strict held-lock check follows conversion.

Assertions are constrained to observed effects: project writer succeeds with real error zero; the parsed physical output preserves ObjPropertiesKey but omits its Data child only in the denied run; control Data decompresses to the fixture side bytes; Boolean BWZ/INI, MST and remesh outputs complete and agree between fresh copies; the side KEY remains unchanged. Comparing the selected control Object after removing its control Data node against the denied Object avoids comparing complete project files across different scratch paths. No project residue under a failed write is inferred.

## Limits and unqualified assumptions

- This runs the candidate detailed converter's Open and full Save path through a private project-output service. It is not the original executable, C ABI, GUI interaction, or fresh original fault execution.
- It observes the real project file's QFile-like Close and error query. It does not execute or prove normal GUI Close. `ConversionSession` teardown on return is candidate adapter cleanup, not native GUI Close.
- The test uses the fixture's current side-key bytes as expected input and checks the known length; root's fixture manifest supplies the durable fixture hash. It does not hard-code compressed Data bytes or assert a whole-project hash, appropriately avoiding a synthetic encoding oracle.
- It requires a real shared-read denial on this Windows host. The setup uses REQUIRE and a direct `ifstream` probe, so a lock failure is a test failure rather than an alternate pass. The project output itself is opened before the lock; no output status fault is injected.
- Open/Save completion and continuation are tested, but property/MST/remesh success comes from this specific corpus profile. The test is not general failure-matrix coverage for those later writers.
- No build or test run was performed during this review; root owns execution and any source-stable validation.

I found no lock-timing, fixture-isolation, data-omission, or output-status assertion that needs correction before root validation. The only wording to preserve in future reports is “project-file Close/error query,” not “native GUI Close.”

No source/test/build/GUI/mutable REA changes were made for this review.
