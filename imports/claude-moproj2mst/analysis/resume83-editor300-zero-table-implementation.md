# CP83 one-point zero-table source freeze

The actual advanced-page live path predicate now admits the bounded type7a
one-point/four-component all-zero table as well as CP82's constant signed-zero
branch. It reads axis[0][0] first, then all four values, and preserves the curve
and position data. General shapes and nonzero/unordered samples remain guarded.
No curve evaluation, allocation or synthetic observation was added.

Source qualification and the approved three-file contract are in
`analysis/resume83-editor300-next.md`, frozen SHA-256
`476b5140de3c943660ec4d3c8da8e5718f6411f5e78778c7ae781470a1d8bbfc`.
The selected original predicate uses axis-first UCOMISD/JP comparisons and the
native count helper computes four dependent values for this shape.

## Exact changes and guards

`advancedTimeProfile()` retains its actual GuiModel object iteration and
null/kind7 skip. For movement6, ndim0 still checks only its scalar, including
signed-zero admission and NaN rejection. Otherwise the selected table profile
requires type7a, ndim1, n0=1, ncomp4, nonnull axis0/values, zero inactive counts,
null inactive axes, and no additional u68 owner. These are bounded candidate
profile guards, not claims about additional native branches. The resulting
array reads cover exactly one axis and four values. Their comparisons admit
both signed zeros and refuse every nonzero or NaN. The inactive scalar value
is not inspected. Larger/empty/other-component tables remain unsupported.

Both Fill and Apply continue using the live model predicate; Apply still
refuses before its first time/model store if current data has changed. No
public constructor/private palette guard, other page behavior, API signature,
CMake or shared runtime source was changed.

The old snapshot and lifecycle refusal fixtures now explicitly make their actual default
table's first value nonzero. Its guard expectation is preserved for the
unrecovered evaluator branch. The CP82 ncomp1 zero-table test remains refused
and was renamed/commented to describe its unsupported shape.

Four new Testkit cases cover:

- actual default allocated one-point 3D path, signed-zero axis/four values,
  ignored NaN scalar storage, unchanged allocation pointers/sample bits and
  three position words across Fill and changed-time Apply;
- nonzero, subnormal, infinity and NaN independently at the axis and each of
  all FOUR dependent components; live Apply refusal before time stores, then
  admission once all samples return to signed zero;
- type, dimension, point/component count and inactive-count guards, plus
  temporarily missing axis/value arrays; each refuses before an array read
  and restores the fixture metadata/pointers afterward;
- an object created after Fill qualifies through its current zero table, but
  a later fourth-component change refuses Apply; kind7 still takes the native
  skip without touching its nonzero curve.

## Static preflight and coordinated lifecycle fixture amendment

Compared all four files with CP82's archived `source.zip`. Quoted includes
resolve, delimiters are balanced, added macros are supported Testkit macros,
the file has exactly one Testkit main, and no C assert or trailing whitespace
is present. Existing target registration is automatic. No build or executable
test was run by this worker.

Target `m2m_mdt_simcontrol300_snapshot`, CTest
`m2m.mdt.simcontrol300_snapshot`, now contains 81 cases (77+4).

Repository-wide test reconciliation found one further fixture amendment outside
the initial three-file set: `test_simcontrol300_lifecycle.cpp:787`, case
`initial_editor_unrecovered_live_advanced_state_stops_after_earlier_page_stores`.
It creates a fresh movement6 default all-zero type7a table and expects refusal.
Root explicitly extended this worker's exclusive set for that one fixture.
The actual path's type7a/ndim1/n0=1/ncomp4 and both array pointers are verified
with REQUIRE before making values[0]=1. Its earlier-page stores, advanced
refusal and skipped-later-I/O expectations remain intact. No other lifecycle
test changed. Its target `m2m_mdt_simcontrol300_lifecycle` / CTest
`m2m.mdt.simcontrol300_lifecycle` still has 28 cases. This amendment prevents
the test's blanket old exclusion from becoming stale; it remains a nonzero
evaluator refusal, not a new production restriction.

## Released source hashes

Files are relative to `source/apps/moproj2mst/`:

| File | SHA-256 |
|---|---|
| `include/m2m/engine/simcontrol300.hpp` | `6f5ce9552c6c63998f22ae5841d70c83bf38df44d71b6207191b83952aba2e1e` |
| `src/engine/simcontrol300.cpp` | `31fd1ea3e3ba1b2ee004267378e9369d993e548e55296bb8f21a08a6ff12f454` |
| `tests/mdt/test_simcontrol300_snapshot.cpp` | `ffb4fae05f86c5dfff10f0f17c81e400a2030c55df2163d4d64c712aff211bef` |
| `tests/mdt/test_simcontrol300_lifecycle.cpp` | `1dec8b6827017706802b599ba388983628b6d591cc6fe77dff79080596122337` |

These files are frozen for root's grouped build/test/archive. No GUI, original
execution or mutable REA operation occurred. Public editor300 remains blocked
by its earlier widget/private-palette dependency. The next path-specific
dependency is another qualified table shape or the actual nonzero copy,
evaluation and rotation chain, neither enabled by this increment.
