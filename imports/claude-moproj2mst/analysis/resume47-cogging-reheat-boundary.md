# Resume 47: Cogging one/two-reheat boundary

## Original behavior, qualified

The retained evidence identifies the `masterWrite` call path and the faulty native loop without supporting any deterministic bytes from a multi-reheat run:

- `analysis/closure-audit.json` C10 is the controlling inventory entry. It classifies the port as an explicit refusal versus original array behavior, lists native addresses `0x14076ac03`, `0x14076ac72`, `0x1407617e0`, cites retained static source, and explicitly warns not to treat undefined output as reproducible without capture.
- `source/docs/re/moproj2mst/mdt/spec.md` section 3 is the retained address-level analysis. The operation writes the Simulation Data header, SIMLST count and lines, then `NoReheats`, then iterates REHEAT blocks. Its analysis of `0x14076ac03..0x14076ac72` says the record pointer is accumulated: at zero-based loop index `i`, the destination is `n - 1 + i(i + 1)/2` elements from the start (each REHEAT record is 400 bytes). Therefore, for `n=1`, the sole destination is element 0. For `n=2`, iteration 0 uses element 1, so it labels the second reheat's data “Reheat 1”; iteration 1 uses element 2, outside the two-element array. The first bad index is deterministic from the analyzed pointer arithmetic; the value read at that out-of-bounds address and resulting output/crash are unspecified. Do not claim what bytes follow or that the application necessarily crashes.
- `analysis/wave3-cogging-verified-1407617e0.asm` is direct retained disassembly for the Process Data function. It shows the path calling `0x14076a8c0`; it does not contain the faulty loop body. The exact loop explanation is retained in the spec and referenced by C10, not freshly extracted in this audit.

This is static evidence only. No native run establishes stable output for `NoReheats >= 2`.

## Current candidate behavior

In `source/apps/moproj2mst/src/datafiles/mdt_write.cpp`, `masterWrite` (around lines 404–440) validates that a positive `nReheats` has at least that many `reheats` elements, totals and emits SIMLST, writes `NoReheats`, then throws `M2M_E_UNSUPPORTED` for `nReheats >= 2`. This check prevents the port from entering the native unsafe loop. Since `writeMdtBytes` exposes a returned string only on success, callers receive no partial bytes when this exception is raised, even though its internal `Out` has already been appended through `NoReheats`.

For one reheat, the port emits `reheatWrite(reheats[0], 1)`. That writer emits the `Reheat 1` banner, `NoPass`, three Heat Transfer blocks in array order, then each pass. Process Data is appended afterward by `writeMdt`. These current source behaviors are deterministic and testable without claiming native multi-reheat output parity.

## Smallest deterministic boundary test

Use the existing public in-memory seam `datafiles::writeMdtBytes(const CogModel&, Project::CogGlobals&, header)` from `include/m2m/datafiles.hpp` or `src/datafiles/readers.hpp` (confirm the declaration location in the active source tree). A single focused test can exercise both sides with default-valued data; no engine model, external corpus, file write, or application capture is needed.

1. Create default `data::CogModel one` and default `Project::CogGlobals globals`. Set `one.cog.nReheats = 1` and `one.cog.reheats.resize(1)` because the model default count is 1 but the vector is initially empty. Keep each reheat's `nPass=0`. Call `writeMdtBytes(one, globals, knownHeader)` and assert success. Assert `SIMLST` with total 0 appears before `NoReheats 1`, which appears before `Reheat 1`, which appears before `Process Data`; assert exactly one `Reheat 1` marker and no `Reheat 2`. A minimal ordering assertion can compare the four marker offsets in the returned bytes. Avoid checking unneeded formatting or nondistinct default fields.
2. Create default `data::CogModel two`, set `two.cog.nReheats=2`, resize `two.cog.reheats` to exactly 2, keep both `nPass=0`, and call the same seam. Assert it throws `m2m::Error` with code `M2M_E_UNSUPPORTED` and message containing `NoReheats 2`. This verifies the current candidate boundary guard, not original-equivalent behavior. Do not assert output bytes, partial bytes, crash, or a specific original read value.

The element count must equal `nReheats` in both fixtures so this test reaches the intended one-versus-two boundary rather than the earlier `M2M_E_INPUT` size guard. `Project::CogGlobals::unit` defaults to 1, within `writeMdt`'s accepted unit range. If the local header path differs, use the existing test's `datafiles::writeMdtBytes` include already exercised in `tests/mdt/test_cogging_save.cpp`.

## Scope of the assertion

The one-reheat test pins a defined valid branch. The two-reheat test pins only the current explicit refusal. Static source establishes the native loop's first out-of-bounds index for two reheats, but not the subsequent memory contents or observable bytes. Any native capture is a separate evidence task and must preserve crashes, variation, or successful odd output as observed data without making a guessed deterministic contract. No source, tests, builds, application processes, or REA state were changed for this note.
