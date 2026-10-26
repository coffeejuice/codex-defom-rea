# CP94 implementation plan: nonnumeric SPRING count maps to zero

## Objective

Close only the bounded `SPRING.INI` extraction case from CP93: an existing ASCII sidecar whose first byte is `x` and whose contents are exactly `x\r\n`. The original failed integer extraction leaves the initialized count at zero; the helper returns success because the stream was not at end before extraction; the outer spring reader then takes its nonpositive-count return without allocating or reading records.

The candidate currently throws `M2M_E_UNSUPPORTED` from its strict generic integer parser, aborting the enclosing `fillExtra3D` call. The public regression should assert that `fillExtra3D` returns a page for this exact malformed count and that direct `readSprings` returns false with an empty output vector.

## Source-qualified behavior

Original evidence:

- `analysis/wave2-save-diffs-300-support-1410dab40.c`: opens `SPRING.INI`, constructs a QTextStream, calls `FUN_1410da800` for one count, copies the count only when that helper returns true, evaluates `count > 0`, and allocates/reads records only for a positive count. The final return is the resulting count/record status.
- `analysis/wave2-save-diffs-300-support-1410da800.c`: initializes the integer destination to zero; records initial `atEnd`; extracts while not at end; increments the requested-value index regardless of extraction result; returns true iff the stream was not at end before extraction.
- QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`: retained `getInteger` and `operator>>(int&)` evidence, summarized in `analysis/resume84-sw2sp-parser-plan.md`, qualifies zero written on integer getter failure and status update. The outer spring reader ignores that status. This is a static prediction, not an original-process run.
- `analysis/wave2-save-diffs-300-support-140ec8110.c` lines around 139–141: the caller loops over spring rows only when `FUN_1410dab40` returns true.
- Candidate public call chain: `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp` `fillExtra3D` invokes `io.readSprings(springs)`; a false/empty result allows the scalar form to return. `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` currently routes the count through strict `readInts` and throws for `x`.

For the selected exact fixture, the only observable contract to assert is zero count, false `readSprings` result, empty output, and a non-null public Extra3D form. The Qt status and cursor are not exposed by the public adapter and are intentionally not asserted by this fixture.

## Minimal implementation approach

**Exclusive production file:** `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`.

Keep the existing `readInts` parser and all other paths unchanged. Add an internal `readSprings`-only recognition for the exact file payload `x\r\n` (ASCII bytes, no BOM) after successful file open. Map this one profile to the existing zero-count result: leave `output` empty and return false. For every other payload, retain the current `readInts` call and the current positive-count unsupported error.

Prefer a small private helper/reuse of the already-open file bytes if the local adapter structure permits it without changing the generic `readInts` contract. If exact recognition requires a second read after `readInts` throws, scope the reread strictly to the matching unsupported exception and exact bytes; do not swallow file-open, path, decoding, or unrelated parse errors. A refactor of the generic parser, a general “failed count means zero” policy, and any positive-count admission are out of scope.

The guard must remain explicit for:

- positive counts, including `1`;
- negative counts and all overflow/radix forms;
- other invalid payloads (including `?`, `x` without the selected CRLF framing, and high bytes/BOM);
- missing/unreadable `SPRING.INI`;
- row fields, `SPRING2.INI`, allocation, and loop behavior.

If implementation discovers that exact payload recognition cannot be kept independent from these cases without altering the generic reader, stop and revise this plan rather than widening the profile.

## Focused regression

**Exclusive test file:** `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp).

Add a release-active test using the existing `Rig` fixture:

1. Write `SPRING.INI` as exact bytes `x\r\n`.
2. Call `readSprings` with an initially nonempty vector; require false and an empty vector.
3. Call `fillExtra3D(*rig.io, NumericLocale::KnownC)`; require a value (the other sidecars are absent, which follows the already tested fallback path).
4. Write `SPRING.INI` as `1\r\n`; continue to require `M2M_E_UNSUPPORTED`.
5. Retain existing zero-count and extreme-count tests unchanged. Add no assertions about raw QTextStream status/cursor, since they are not exposed by this adapter and a fresh original-process comparison has not occurred.

No CMake, headers, shared Session, UI, fixture corpus, or mutable REA files should change. Root owns build/integration validation after the grouped CP93 build and source freeze.

## Acceptance boundary

A successful patch should make only the exact malformed count case return the original false/empty spring result, preventing its error from aborting `fillExtra3D`. It must not claim positive-count spring support or full spring Open→Save parity. Before implementation, verify the CP93 grouped build has frozen and root has accepted this plan.

No source or test was edited while preparing this plan. No build, test, GUI action, or mutable REA action occurred.

