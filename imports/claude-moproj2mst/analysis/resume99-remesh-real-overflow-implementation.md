# CP99 canonical real-overflow source freeze

The actual remesh reader now admits only the proved canonical huge-positive-
exponent class and returns fixed signed infinity with errno34. Later fields,
source scaling clamps and actual Session Open continue. General nonfinite,
underflow, hex and other range/mantissa profiles remain guarded.

The contract was frozen before implementation:
`analysis/resume99-remesh-real-overflow-contract.md`, SHA-256
`0b8e94a08ef219bc7a2e9571f4d1c7ce22eb2bdc5e6da1f14f6f39eb0848a9cb`.
It pins the native lexical cap/overflow status, fltin mapping, fixed signed
HUGE_VAL result and errno store. CP98 archive freeze/release preceded source work.

## Exact production change

Added a private `canonicalRealOverflow()` value recognizer in the existing
remesh source. After the existing D/d-to-E replacement, it accepts a complete
token matching `[+-]?[1-9][eE][+]?[0-9]+` only if exponent>5200. Every exponent
character must be a digit; accumulation caps at5201 with bounded unsigned
arithmetic. No whitespace, trailing text, multidigit/fractional mantissa or
negative exponent is newly admitted.

On qualification it sets errno=ERANGE and returns signed infinity. It runs
BEFORE host strtod, so this new branch does not depend on host overflow
conversion, flags or traps. An unmatched token falls through to the unchanged
ordinary conversion/hex/nonfinite/ERANGE guards. CP98 integer saturation,
all reader/file owners, shared locals, native partial effects and Save behavior
are untouched. No public API/header, CMake, selected palette/count or page source
changed.

## New dedicated tests

Added `tests/mdt/test_simcontrol300_remesh_real_overflow.cpp` with five cases:

1. Exact signed infinity bits and errno34 for boundary5201, both signs,
   single-digit mantissas including1 and9, optional exponent plus, leading exponent zeros, D/d markers
   and a long exponent. Loaded/enabled and one close are retained.
2. Both overflow signs reach later method/boolean records; the same source
   comparison clamps positive scaling infinity to10 and negative to1.
3. Retain partial method effects then refuse still-unqualified exponent5200,
   fractional/multidigit mantissas, trailing text, underflow, INF/NaN and hex.
4. Existing finite decimal/D-exponent and zero-mantissa fallback conversions
   retain their expected finite results and completed loads.
5. Actual Session Open consumes scratch fallback overflow input and later
   method4 into its SAME remesh owner, without creating an editor factory.

Memory-file close captures errno before close effects. Disk tests use the
configured M2M_TEST_SCRATCH, short ASCII process/case-specific names and a
current-directory restore scope. No original/user input is modified by the
test design.

## Static preflight and root dependency

Quoted includes resolve, new assertions are supported Testkit macros, all
delimiters balance, one Testkit main is present, and there is no C assert or
trailing whitespace. Diff against CP98's archived source is the private
recognizer plus its invocation (25 added lines). No worker build, CTest,
executable test, GUI/original execution or mutable REA occurred.

The existing CMake glob registers target
`m2m_mdt_simcontrol300_remesh_real_overflow`, CTest
`m2m.mdt.simcontrol300_remesh_real_overflow` (5 cases).

Root separately amended the old real-refusal fixture from1e9999 to the still
guarded1.5e9999 in `test_simcontrol300_remesh.cpp`; this worker did not edit
that file. CP98's late-hex prefix case still applies unchanged. Independent
pipeline review has checked the native binary/range/status contract and is
reviewing this exact frozen source/test pair before root build; no completed
source-review/test result is claimed by this receipt.

## Frozen source

Paths relative to `source/apps/moproj2mst/`:

| File | SHA-256 |
|---|---|
| `src/engine/simcontrol300_remesh.cpp` | `a3f361099a8d0686825b191e8516220505f16c3228c1044e5cd4becdde970779` |
| `tests/mdt/test_simcontrol300_remesh_real_overflow.cpp` | `ebf581ebb0f60e4d18488b7dbb39140c0107bb9f6f9952883666697381f87f90` |

Both files are released/frozen for review and root integration. This closes
one actual public numeric decision without selected palette observations.
General CRT real conversion, additional lexical classes and the separate
constructor/page dependencies remain incomplete.
