# CP101 legacy hex-prefix zero source freeze

The actual remesh reader now implements its original decimal-prefix result
for the existing guarded0x/0X class: return signed zero, preserve errno and
continue. It never invokes host hexadecimal conversion for that class. All
other real guards, canonical range behavior and public constructor gates remain.

Evidence was frozen before source work:
`analysis/resume101-remesh-hex-prefix-contract.md`, SHA-256
`fd738aaf44f5852c9ca8503b8ae3b8b2e81767a4881a96ca8337298ccf6343ce`.
It pins found-decimal-zero/status0, stop-at-x cursor, signed-zero converter,
flag0 payload return and no errno store. CP100 archive release and exact pair
approval preceded implementation.

## Production and focused tests

Only `originalReal()`'s existing hex-prefix refusal changed: it returns literal
-0.0 for a preceding mantissa minus, otherwise +0.0, before clearing errno or
calling strtod. Existing whitespace/sign indexing and D/d normalization stay.
Suffix text is not numerically interpreted; TokenReader already consumes one
whole token. No new token size/encoding, locale, stream or owner profile was
added. CP98 integer and CP99/100 canonical range paths are untouched.

New `test_simcontrol300_remesh_hex_prefix.cpp` has six cases:

- Exact signed-zero bits and unchanged EDOM for both prefixes/signs, bare0x,
  hex-looking/non-hex suffixes and huge suffix exponents.
- Prior integer ERANGE carries through a negative prefix-zero and later boolean
  record; method remains clamped5 and the read completes.
- Complete suffix consumption, later method assignment and scaling fallback1.
- Other nonfinite/fractional-range profiles remain refused after their prefix.
- A reached non-ASCII line guard remains before the field store.
- Actual Session Open consumes a scratch fallback into the SAME remesh owner,
  replaces its prior finite filter value with signed zero and applies method4
  without creating an editor factory.

Memory-file close captures errno before close effects. Disk tests use configured
M2M_TEST_SCRATCH, short ASCII process/case names and a directory restore scope.
No original/user input is modified by the fixture design.

## Preflight and root-owned fixture reconciliation

Quoted includes resolve, assertion macros exist in Testkit, delimiters balance,
one Testkit main is present, and there is no C assert/trailing whitespace. No
new include/header/CMake registration is needed. No worker build, executable
test, CTest, GUI/original execution or mutable REA occurred.

New target `m2m_mdt_simcontrol300_remesh_hex_prefix`, CTest
`m2m.mdt.simcontrol300_remesh_hex_prefix`, contains6 cases.

Root separately amended four old inputs, preserving their prefix/refusal checks:

| Root-owned test | Replacement for former0x1p2 |
|---|---|
| `test_simcontrol300_remesh.cpp` | `-INF` |
| `test_simcontrol300_remesh_overflow.cpp` late-real case | `NaN` |
| `test_simcontrol300_remesh_real_overflow.cpp` guard list | `1.5e9999` |
| `test_simcontrol300_remesh_real_underflow.cpp` guard list | `1.5e-9999` |

This worker did not edit those files. The independent reviewer has the exact
pair and root fixture changes; native prefix/cursor/sign/errno proof checked
so far, source review pending at receipt write time. Root owns build/archive.

## Frozen pair

Paths relative to `source/apps/moproj2mst/`:

| File | SHA-256 |
|---|---|
| `src/engine/simcontrol300_remesh.cpp` | `26791cd239c998016895ea83161cc79c430413eb47d4efc9059cb586471a34fe` |
| `tests/mdt/test_simcontrol300_remesh_hex_prefix.cpp` | `e7d5ac6f62a77d9d134680bac049f657165362eb52040c1abf91ee3ebc327fef` |

Both files are released/frozen. This value/errno/cursor adapter does not claim
full legacy temporary/FP-environment equivalence or general real parsing.
Nonfinite spellings, additional range/mantissa classes and selected editor
construction remain separate dependencies.
