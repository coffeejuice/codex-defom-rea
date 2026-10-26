# CP100 canonical underflow source freeze

Actual remesh canonical underflow now returns native POSITIVE zero and errno34,
including negative mantissas, instead of refusing the range error. Later fields,
loaded/enabled and actual Session Open continue; source scaling falls back to1.
CP99's positive-exponent overflow result and all other range guards remain.

The evidence contract was frozen first:
`analysis/resume100-remesh-real-underflow-contract.md`, SHA-256
`76ed0b5281fefcd5ee3f612a69f713126cd318d8a4acb24c29ee4583f924cc6e`.
It includes lexer/status1, converter signed-zero paths/config64, fltin100h and
the outer XORPD sign-loss/errno22h branch. Root's CP99 archive release preceded
source work; only the approved source/new-test pair was edited.

## Exact source change

The private recognizer is now named `canonicalRealRange`. Its complete-token
gate, single nonzero digit, E/e marker after existing D/d normalization, capped
digit-only magnitude and strict >5200 bound remain unchanged. It now recognizes
exponent minus in addition to CP99's absent/plus exponent sign.

The added negative-exponent class is exactly
`[+-]?[1-9][eE]-[0-9]+` with magnitude>5200. It sets ERANGE and returns literal
+0.0 before host strtod, independent of the mantissa sign. An engaged optional
with zero remains a successful recognized result. CP99's positive class still
returns its fixed signed infinity. Every unmatched token follows the existing
ordinary conversion/hex/nonfinite/ERANGE guards; no general underflow parser,
denormal conversion or negative-zero normalization was added.

The helper rename and call-site variable rename are private only. No public
API/header, CMake, runtime, page/selected palette/count or writer changed.

## Six focused cases

New `test_simcontrol300_remesh_real_underflow.cpp` checks:

1. Exact +0 bits and errno34 at magnitude5201 and larger, both mantissa signs,
   multiple single digits including1/9, D/d, leading zeros and a long exponent.
2. Later method/boolean records and source scaling lower fallback1 complete
   for both input signs, with loaded/enabled and one close.
3. Magnitude5200, fractional/multidigit mantissas, trailing text and other
   nonfinite/hex/range profiles retain partial prefix and refusal.
4. Ordinary zero-mantissa/finite conversions retain their existing values and
   signed zeros, distinguishing them from the newly proved nonzero underflow.
5. CP99 positive-exponent classes still return exact signed infinity/errno34.
6. Actual Session Open consumes a scratch fallback and later setting into the
   SAME remesh owner, replacing its prior finite filter value with +0 without
   creating an editor factory.

Memory-file close captures errno before close effects. Disk fixtures stay
under M2M_TEST_SCRATCH with short ASCII process/case-specific names and a
current-directory restore scope. No original/user file is modified.

## Preflight, root dependency and release

Compared the approved source against CP99's archive: only the private range
recognizer and call-site naming changed (14 diff lines). Includes resolve,
Testkit macros are supported, delimiters balance, one test entry point exists,
and no C assert or trailing whitespace is present. No new include/CMake edit
is needed. No worker build, CTest, executable test, GUI/original execution or
mutable REA occurred.

New root target `m2m_mdt_simcontrol300_remesh_real_underflow`, CTest
`m2m.mdt.simcontrol300_remesh_real_underflow`, has6 cases. Root separately changed
the prior CP99 test's1e-9999/-1e-9999 guard fixtures to still-guarded fractional
1.5e-9999/-1.5e-9999; this worker did not edit that file. Independent pipeline
review has checked the native underflow sign-loss chain and has the exact
frozen pair for source review before root build; this receipt does not claim
completed source review or execution.

Frozen files relative to `source/apps/moproj2mst/`:

| File | SHA-256 |
|---|---|
| `src/engine/simcontrol300_remesh.cpp` | `74700a4f742bafd903472cd9cff6318c1577bcea9daeeff9337184fc891b05c0` |
| `tests/mdt/test_simcontrol300_remesh_real_underflow.cpp` | `9d632bb558053960de3eda83b37e5e76930007ef7e2ed04668df786022c31839` |

Both are released/frozen for review/root integration. This value/errno adapter
does not claim full legacy temporary/converter or FP-environment equivalence.
General real parsing and the independent selected editor constructor remain
incomplete.
