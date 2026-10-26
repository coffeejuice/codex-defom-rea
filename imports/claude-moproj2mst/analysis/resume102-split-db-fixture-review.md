# CP102 independent review: build an on-disk split DB for `dbOpen`

## Verdict

Yes. A disposable **format-valid** two-part chain can be made under `M2M_TEST_SCRATCH` from the retained `.DB` format and passed through the real `engine::dbOpen` path. The current corpus lacks a naturally split, 2-GB-limit production artifact; therefore label the fixture “synthetic, format-valid two-part chain,” not a captured or native-generated 2-GB split database. No in-memory patch to `DbHandle::reader` is needed.

## Recommended deterministic fixture

Use the existing `deformdb::Writer` to create two small complete DB files named `X.DB` and `X25.DB` in the same scratch directory. Both can be one-step, header-only databases built from a real COG step header copied with `deformdb::Database::open/read_step`; set the primary's directory `entry.step` to `26`, and the predecessor's last directory `entry.step` to `25`, then append each with `Writer::append_step(step, true)`. Header-only avoids inventing MAR/A payloads while leaving valid headers/directories for `dbOpen`'s synchronous cache build. `deformdb::Writer` emits ordinary format records and the candidate DB reader consumes those files through `load_db_info` and `setup_part_chain`.

Then create a real `engine::DbHandle`, set its path to the scratch `X.DB`, and invoke `engine::dbOpen(handle, ctx, true, false)` (or `engine::openProjectDb` on a minimal Project if testing that caller too). This invokes the actual production `dbOpen`; it must not seed the answer, part count, `u330`, directory rows, or chain globals directly. The existing CMake graph already includes `deformdb_core` transitively through `defdbio`, so the test target can use `deformdb::Writer` without a shared CMake change.

Expected candidate observations for the bounded default-answer contract: warning names the “2G Split DBs” prompt and records default Yes; after `dbOpen` returns, the reader is loaded, its chain count is 2, the part order/names are `X25.DB` then `X.DB`, and the merged directory count is 2. A one-part control with only `X.DB` checks no warning and one chain part. Keep the control separate or reset the process-global DB part table between runs.

The lower-level format helpers support this recipe. `has_previous_part` checks `abs(first directory step) >= 2`, looks for `<base><abs(step)-1>.DB` (then `<base>-<abs(step)>.DB`), and verifies that the predecessor's last step is the requested step or one less (`source/port/defdbio/src/db_query/db_chain.cpp`). The native Open caller asks after `load_db_info` succeeds and before cache construction (`source/apps/moproj2mst/src/engine/db_load.cpp::dbOpen`). `source/docs/db-format-spec.md` documents the record/index/directory scheme; existing `test_db_directory_receipt.cpp` and `test_db_query.cpp::cog_part_chain` exercise the same predecessor step relation, though those tests currently patch an in-memory row or directly call the chain helper rather than public `dbOpen`.

## Qualifications

- The two writer-produced files are genuine valid DB-format files and the Open branch is reached from disk through `dbOpen`; their tiny header-only split is synthetic and is not evidence of a real database crossing the 2-GB split threshold.
- Reusing a retained step header supplies a plausible cache payload; for semantic consistency, keep its step metadata compatible with the primary step or document that the fixture tests only split-chain Open dispatch/cache setup, not solution continuity across parts. Do not assert simulation results from this fixture.
- Static source qualifies Yes as the default button; it does not qualify timer/event deliveries during `QMessageBox::question`'s modal `exec`. The candidate warning is its configured default-answer behavior, not proof of full modal-loop parity.
- Direct `dbOpen` covers the target production branch without a GUI, but does not prove the full `.moproj` outer Open lifecycle. `openProjectDb` can add the caller path if the test is intended to cover path derivation too.

This was a read-only review. No source, test, build, GUI, original execution, or mutable REA operation was performed.
