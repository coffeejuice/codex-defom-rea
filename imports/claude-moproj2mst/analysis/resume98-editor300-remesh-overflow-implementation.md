# CP98 public remesh overflow source freeze

Actual remesh decimal integer overflow now returns the original signed
saturation result and sets ERANGE=34 instead of reporting Unsupported.
Existing method clamp, boolean stores, shared local reuse, later fields and
Session Open consume that result through their existing public path.

The evidence contract was frozen before source work:
`analysis/resume98-editor300-remesh-overflow-contract.md`, SHA-256
`a60f05f86b215ea82dd567218ba777c6793dd42d1b6a36307d439f562d1439da`.
It pins kind5 dispatch, the signed base10/null-endptr wrappers, original
conversion core's saturation/errno branch, and remesh's subsequent clamp.
CP97 write freeze was respected; root explicitly released it before these edits.

## Exact production change

Only `originalInt()`'s range-exceeded branch changed. It assigns errno=ERANGE
and returns INT_MIN/INT_MAX according to the parsed sign. The existing digit
bound prevents host arithmetic overflow. The native converter also caps the
return at those signed limits, rather than returning a modulo low word.

Ordinary parsing, signs/whitespace, no-digit0, exact negative endpoint and
prior errno on in-range input remain unchanged. Early return on overflow does
not leave trailing digits in the file: the existing token reader consumed the
whole token before conversion, and the original has no endptr consumer. No new
floating-point parsing, locale, token-size, path, allocator or stream profile
is admitted. The normal current C-runtime errno adapter is used; native TLS/
locale allocation-failure behavior is not expanded.

## New dedicated focused target

Added `tests/mdt/test_simcontrol300_remesh_overflow.cpp` with five Testkit cases:

1. Both exact signed endpoints, both one-past-range values, explicit plus,
   leading zeros, trailing text and very long decimal prefixes. Completed load
   clamps to method5/0 and sets enabled. Memory-file close captures errno34 on
   overflow and unchanged EDOM for in-range conversion, before any close effects.
2. Overflowing method/boolean records followed by finite filter/scaling values
   all complete on the same RemeshState. Both saturated signs yield true booleans.
3. A missing next integer reuses the saturated shared local, not the clamped
   method: negative overflow gives method0 but still sets the following boolean
   true. Both input signs retain range errno and complete.
4. A later still-unsupported hexadecimal real preserves the newly saturated
   method prefix while retaining the old no-enabled-assignment refusal.
5. Actual Session/Open with scratch XML and fallback LOC_DEN_CTRL.DAT consumes
   both overflow signs and a later scalar into the SAME Session-owned remesh
   object. It needs no editor factory or palette observation.

Scratch files use the existing configured M2M_TEST_SCRATCH helper, ordinary
ASCII names, process/case-specific directories and a current-directory restore
scope. No original/user files are touched by the test design.

The CMake test glob already registers the new file. Root target is
`m2m_mdt_simcontrol300_remesh_overflow`; CTest name is
`m2m.mdt.simcontrol300_remesh_overflow` (5 cases).

## Static checks, external dependency and release

Quoted includes resolve through the actual test/source/include directories.
Testkit macros are supported, delimiters balance, there is one Testkit main,
and no C assert or trailing whitespace is present. Production diff against
CP97's archived source is only the overflow branch plus its source explanation.
No build, executable test, CTest, GUI/original execution or mutable REA was run
by this worker.

Root owns and has amended the stale existing case in
`test_simcontrol300_remesh.cpp` that expected overflow refusal. This worker did
not edit that file. Its old real hex/INF/range/locale guards remain. The
independent pipeline specialist has verified the original binary/range hashes
and signed/native errno contract and is reviewing the frozen source/test pair
before root build; this receipt does not claim that source review or execution
has already passed.

Exclusive frozen files relative to `source/apps/moproj2mst/`:

| File | SHA-256 |
|---|---|
| `src/engine/simcontrol300_remesh.cpp` | `620e2064ce565316cde6adbcf283d6aec5d6bbcdfcce2aaadeaa4799b572ef6a` |
| `tests/mdt/test_simcontrol300_remesh_overflow.cpp` | `0383902c09cdb3717a8a445495d15a158be4a33c9094d8bfb40372438fd62c28` |

Both files are released for review/root integration and remain frozen. No
shared header/CMake, selected editor palette/count, public constructor guard or
page Fill source changed. This closes one actual public remesh numeric branch;
it is not an editor300 Open-to-Save completion claim. Remaining floating
conversion profiles require their own original CRT source qualification.
