# CP87 independent review: repeated DB-inheritance fixture isolation

The current scratch fix is sound by source inspection. It atomically acquires an unused per-Rig leaf and skips old leaves, so a reused PID/test scratch parent cannot cause copy_file to encounter a prior destination. Reliable repeat runs do not require deleting earlier scratch or enabling overwrite. Root must rebuild/rerun to confirm the authored fix; this review ran no executable.

Reviewed source: `C:/Users/alext/Projects/codex-deform-rea/imports/claude-moproj2mst/source/apps/moproj2mst/tests/mdt/test_db_inheritance.cpp`, 220 lines, SHA-256 `ab3bbe2b9c87828df12224f646b4ebbf3bae102e35f8e6d719fab4d55a669d13`. Related owner receipt is `analysis/resume87-db-inheritance-scratch-fix.md`. Only this report was written by the reviewer; no source/test/build/fixture/GUI/mutable REA changes occurred. Findings were sent directly to root and io_order_audit.

## Failure and acquisition semantics

Root reported full-suite setup failures from copy_file because the destination already existed in reused scratch. The old allocator formed PID plus an in-process serial, then called create_directories, which succeeds when an old directory already exists. A later executable can reuse a PID and reset its serial; that does not make the old Inherited.DB or DieStress/selected.DB a fresh fixture. This is a source-supported setup collision, not evidence that DB behavior failed.

The new code first resolves the PID parent through existing scratchOutput containment and creates that parent. Each leaf is attempted with single-directory create_directory. Only its true result assigns dir and permits initialization/copy. Existing directories (false with no error) and existing-file collisions are skipped by advancing the atomic serial; unexpected errors throw a path-qualified filesystem_error. The allocator neither follows/reuses an existing leaf nor removes it to force success. Atomic serial also avoids a same-process counter data race, although current Testkit execution is sequential.

This is ownership by successful creation, rather than a check-then-create race. Simultaneous processes have different PIDs; same-PID leftovers are harmless. The existence search may take more attempts as old leaves accumulate and is not a measured constant-time allocation. No timing/savings claim follows. Ordinary filesystem failures still fail setup rather than creating an empty or reused fixture.

## What remains unchanged

Comparison with the actual CP86 archived file shows the text from initProject through the end of the file is identical. All model setup, borrowed DB handle association, copyDb, fallback KEY construction, query-cache setup and DB assertions are unchanged. The four Testkit cases remain four cases. With the fixture available, their five Rig instances each acquire their own leaf.

copy_file retains its default no-overwrite semantics. That remains a useful assertion that an acquired leaf was empty; replacing it with overwrite_existing would hide collisions rather than establish isolation. The fixture source remains read-only M2M_DB_FIXTURE/COG_P1_B1.DB and Objects/Object00001.KEY. Existing byte/timestamp immutability checks on copied DBs are not weakened. The pre-existing fixture-unavailable skip remains unchanged and must not be mistaken for successful DB acceptance; root's full fixture manifest/presence checks still apply.

## Cleanup and model lifetime

The current fix intentionally retains per-run artifacts. That is acceptable for reliable reruns because no old leaf is reused. Do not recursively remove the configured root or PID parent: it can contain earlier failure evidence or leaves owned by another active Rig.

Do not add `~Rig(){ remove_all(dir); }` as a cleanup shortcut. A destructor body executes before member destruction. Here Project p owns model state borrowing query, and query must outlive p. Removing files in the destructor body would run while those model/DB handles can still exist, risking Windows sharing failures and making cleanup depend on implementation timing.

If cleanup is later explicitly desired, an owned-path guard declared before query can be destroyed last, after p and query. It must own its path value, record only a leaf whose atomic creation succeeded, verify containment and remove only that leaf. Alternatively, an outer cleanup owner can run after the complete Rig scope exits. Preserve failed-run artifacts according to the runner's evidence policy; never treat handle closure/reset or source-fixture deletion as a way to make the test pass. No such cleanup change is required for this fix.

## Root validation

Rebuild the changed target and rerun its four cases, then repeat the full suite using the same configured scratch root. Preserve the normal fixture hashes and source/executable binding. The source fix should tolerate existing PID leaves without external cleanup; do not clear them merely to obtain the first pass. Required DB assertions and the exact complete suite remain in force. This review provides a static isolation/lifetime conclusion, not an independent run result or completion claim. Token measurements are unavailable.
