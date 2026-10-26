# CP94 independent SPRING invalid-count source review

No blocker was found in the frozen implementation. The change admits only exact ASCII `x\r\n` through `readSprings`, returning false with an empty spring vector so the scalar Extra3D form can remain available. It does not admit populated spring records or general malformed integer input.

## Frozen comparison

I compared the two current files directly against their members in `build/wave3-checkpoint93-verified/source.zip`. The production delta is confined to `readSprings`; the test delta retains the positive-count guard and adds one bounded regression. Current SHA-256 values match the owner release:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `2ca5e748b1de801b9e1c74692ddb2b17e9975069be8f76dca71fe146c038d4e0` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp` | `31083994ae1b834c7accf6fa0d5aa4561da9070a11506a33916054857f2a0fa2` |

CP93 receipt records source tree `36e2a39398cf469ce9932a5e81e331c2da7cac81dc49a863672fe621a5f48468` and archive `8859fa3beecd2cb7ca735eb7827d529f8005a0bf36c612626e5af74616ca2d52`. The archive hash was independently recomputed for this review. This is a comparison baseline, not a CP94 full-build binding claim.

## Native ordering and adapter result

Retained `wave2-save-diffs-300-support-1410da800.c` initializes the requested integer destination to zero, checks initial `QTextStream::atEnd`, and attempts extraction while not at end. Its return is based on initial availability, not extraction status. For `x\r\n`, retained Qt `getInteger` evidence shows the invalid first character is ungot and getter code 2 returned. The int extraction operator writes zero; because the restored `x` means the stream is not at end, a fresh stream acquires status 2. The logical cursor remains before `x`.

In retained `wave2-save-diffs-300-support-1410dab40.c`, the spring wrapper calls that helper, copies the zero count, then tests `count > 0`. Both spring-record allocations and subsequent record reads are skipped. It closes/destroys its file and stream and returns false. The caller in `wave2-save-diffs-300-support-140ec8110.c` constructs spring rows only on true. This uses the stream helper `1410da800`, not the separate file-wrapper helper `1410da690`.

These are offline source/PE-qualified conclusions for original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore4_64.dll SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Relevant retained instruction records are `wave2-save-diffs-qt-textstream-getinteger-complete.asm` and `wave2-save-diffs-qt-textstream-read-int-complete.asm`. No original process was run during this review.

The adapter reproduces the externally relevant false/empty outcome, using exact recognition rather than exposing or emulating mutable QTextStream cursor/status. It clears output first, opens `SPRING.INI` once, applies the existing default ASCII decoder, then returns false for the selected bytes. There is no probe/reopen sequence. File lifetime remains RAII; this review does not claim native destruction order equivalence beyond the preserved single-read adapter path.

## Guards and test strength

For every other payload, the new local fallback matches the former `readInts(SPRING.INI, count[1])` path: one successful open, default ASCII decoding, zero-initialized count, empty input returning zero, and generic `readDecimalInt` at cursor zero. SPRING did not match the existing one-value SW2SP/STRETCH private Qt-parser route. Missing/unopenable files still return false; count <= 0 still returns false; positive counts still reach the same unsupported populated-reader error. Codec/high-byte guards remain active because no BOM allowance is passed. Generic `readInts` and its path/count gates are unchanged.

The new release-active test starts with one spring record and requires false plus empty output for exact `x\r\n`. It separately requires `fillExtra3D(..., KnownC)` to return a value. It requires unsupported errors for `x` without CRLF, another invalid token `?\r\n`, and positive `1\r\n`. Existing zero and extreme-count coverage remains. There is no fixture-output bypass, optional success branch, or debug-only assertion.

`fillExtra3D` permits the scalar page when `readSprings` is false/empty, so the public C++ assertion checks the intended continuation. It does not establish GUI list/checkbox state, original runtime behavior, allocation-hook observations, C ABI behavior, arbitrary malformed-count support, positive spring support, or complete Open/Save parity. “Allocation skipped” refers to native spring-record allocation branches, not absence of all file/string allocations.

## Validation boundary

This was read-only source and retained-evidence review. No source/test edits, builds, test executions, GUI operations, or mutable REA actions occurred. Root reported grouped build and focused `simcontrol300_native_io` success; I did not independently execute those checks or establish the final CP94 source/binary/full-suite binding. Full frozen acceptance remains root-owned.
