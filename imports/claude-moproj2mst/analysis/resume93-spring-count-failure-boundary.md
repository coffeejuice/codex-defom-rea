# CP93: SPRING count-first failure boundary

## Finding

The original count-first reader has one small, source-closed failure case that does not enter allocation or record parsing. For an existing, successfully opened ASCII `SPRING.INI` containing the single byte `x`:

1. `FUN_1410dab40` opens the file ReadOnly, constructs one QTextStream, and calls `FUN_1410da800(..., count=1, stream)`.
2. `FUN_1410da800` initializes its integer destination to zero, observes that the stream is not at end, invokes `QTextStream::operator>>(int&)`, and returns true based on that initial not-at-end result. It does not check conversion status.
3. The Qt integer getter has no numeric prefix at the first byte, so extraction fails. The int operator leaves the destination zero and sets an error status. The wrapper still returns true.
4. `FUN_1410dab40` copies zero to the count, evaluates `count > 0` as false, skips both allocations and the row loop, closes the stream/file, and returns false.
5. The public Extra3D fill caller invokes `FUN_1410dab40`; the spring-item loop runs only when that result is true. A false result leaves the scalar page path available.

The candidate's `NativeExtra3DIo::readSprings` currently delegates the count to `readInts("SPRING.INI", count)`. For the same byte, the generic strict decimal branch in `readDecimalInt` throws `M2M_E_UNSUPPORTED`. This escapes `fillExtra3D`, unlike the original zero-count/false result. This is a concrete malformed-count Open-path difference, distinct from the already covered valid multi-double profiles.

## Smallest safe source/test cut

A narrowly bounded fix/test can recognize only the one-byte ASCII `x` count after a successful `SPRING.INI` open and map the failed extraction to zero, then return false from `readSprings` with empty output. It must not proceed to allocation or row parsing. Test both direct `readSprings` outcome and the public fill caller's non-throwing scalar-page outcome if the existing test seam can construct that path.

Keep all positive counts unsupported. In particular, do not reuse the SW2SP parser profile for `SPRING.INI`: positive count values size a `count * 4` auxiliary array and a per-record `0x40` allocation and control repeated reads. Overflowing or otherwise large counts require separate allocation bounds. The record loop reads one integer, two doubles, another integer, and three doubles per record from the open `SPRING.INI` stream; its optional `SPRING2.INI` integer array is initialized/zero-filled when that helper fails. That is a separate larger contract.

## Evidence and exact bounds

- Original reader: `analysis/wave2-save-diffs-300-support-1410dab40.c`, including the initial integer call, `count > 0` branch, allocation calls, optional `SPRING2.INI` load/zero fill, six per-record extractions, and cleanup/return.
- Integer wrapper: `analysis/wave2-save-diffs-300-support-1410da800.c`; it zeroes destinations, checks `atEnd`, extracts while not at end and within count, increments the extraction index, and returns based on the initial at-end state.
- Public caller: `analysis/wave2-save-diffs-300-support-140ec8110.c` around lines 139–141; only a true result enters the spring item loop.
- Qt source qualification: QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The retained `getInteger` and `operator>>(int&)` captures cited by `analysis/resume84-sw2sp-parser-plan.md` establish zero-on-getter-failure, low-dword storage on success, and status assignment. This prediction is static PE analysis; no original process was run.
- Candidate: `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`, `readInts` and `readSprings`; caller: `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp`, `fillExtra3D`.

No source or test was edited; no build, test run, GUI action, or mutable REA action occurred. The exact original runtime result still needs process comparison. The proposed `x` fixture is intentionally limited to the zero-count failure edge; it does not qualify positive count parsing, allocation bounds, other malformed tokens, BOM/codec behavior, or row partial-read semantics.

