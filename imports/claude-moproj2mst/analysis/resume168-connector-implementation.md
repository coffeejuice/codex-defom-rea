# CP168 connector implementation: private `+0x48` zero store

## Result

Implemented the selected normal-path store at QtCore RVA `[0x1311bb, 0x1311bf)`. The constructor path already executed CP167's `+0x40` zero in the same `initializeNativePrefix()` call. It now validates that retained boundary, records `private48 = uint64_t{0}`, advances to `BeforePrivateZero50`, and throws at the existing constructor cut before the independent `+0x50` store. It does not create `TaskInfo` or mark QObject completion.

`SparseQObjectPrivate::private48` is optional and starts disengaged, preserving unknown allocated bytes as unknown. The CP167 predecessor guard now also refuses any prior `+0x48` observation before writing `+0x40`. The CP168 admission guard checks the exact pending Report owner and private identity, allocation extents and address points, address-point markers, the previous `+0x40` zero, no previous `+0x48` observation, incomplete QObject/base state and unavailable backlinks. It also rechecks `TaskAllocationStore::ownsWidgetRuntime()` and the bound QList/QString root identities, QtCore hash and symbol RVAs `0x2f0a50`/`0x2f0b30`. Existing symbolic reference deltas are retained and not repeated.

Tests now expect the first factory call to cross the CP167 and CP168 stores and stop at `0x1311bf`. Focused coverage checks both zero observations, retained identities and root deltas, same/different-key reentry, the actual `ConversionSession` owner pairing, and refusal before mutation when `+0x48` is already observed at CP167 entry. The CP168 pre-write matrix covers missing/nonzero `+0x40`, already-observed `+0x48`, foreign/aliasing QList and QString bindings, expired/unavailable/foreign factory root, both uncommitted reference markers, wrong phase/private or task address point, missing task address-point marker, and completed/backlink state. It verifies fields, phase, bindings, commit markers, address-point markers, completion/backlink flags, deltas, caches and retained counts remain unchanged. The test file uses Testkit entry points and macros; no C `assert` checks were found.

## Verification and scope

This was a source-only implementation pass. `git diff --check` did not report whitespace errors for the active parent checkout, and a static scan found no C `assert` in the test file. No build, focused tests, full regression, original comparison, GUI control, deployment or live REA session was run. The independent source review reported that CP167 had passing predecessor gates before this patch; runtime acceptance of the repaired CP167 boundary and CP168 remains unverified until the authorized integration owner runs the focused and checkpoint gates.

No GUI-control or live REA session was acquired, so there was no app-control release operation or receipt to report. No GitHub transport was used.

## SHA-256 receipts

Hashes are for the files at implementation handoff, before any subsequent edits.

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `750722b3bec3551b87124747bbb45235001dd699eb25e9d935b670ecf7f2cdcb` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `156a52a6f4d14f92497400a10f9448f143adb1b99a4b0904ed1cc3da96a04f1f` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `1ef0cb811cfeb2fe405a3a8688bf3fa7a05d39d51638de348b219e76ff69b3c2` |
| `analysis/resume168-connector-next.md` | `f94c4ca9ce70b2c34358956640a28e311c157e66512c9e15adc9c3306beac9b8` |
| `analysis/resume168-connector-plan-review.md` | `bc8cd6c1ac17ca322561e157772626f024e0f1e0a6066853753849ead47bc37d` |
| `analysis/resume168-connector-source-review.md` | `98432d86418b97a5e6ad554b25f89d78827eea93b466035c3fb17548dd380fcf` |
