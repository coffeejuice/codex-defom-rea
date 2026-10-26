# CP84 bounded 1D zero-path loop source freeze

The actual advanced-page path guard now checks positive-length type7a
one-dimensional/four-component tables. Every axis sample is checked before
all 4*n0 dependent samples. Both signed zeros pass; nonzero/NaN still refuse
before Apply's first model store. The actual object-owned curve and positions
are never copied, evaluated, allocated or changed by this guard.

The approved evidence/implementation plan is
`analysis/resume84-editor300-next.md`, SHA-256
`eef855188042565d6c6dda74674aad45ca914586af75cf4118ad86a29f7fd1c1`.
It pins the original axis/value loops and signed32 value-count helper through
CP83's full native byte receipts. No new original capture was required.

## Exact production change

`advancedTimeProfile()` keeps its existing actual GuiModel object scan,
null/kind7 skip, and scalar ndim0 zero branch. The table profile retains
type7a/ndim1/ncomp4, actual nonnull axis0/values, zero inactive counts, null
inactive axes and null additional u68 owner. Its n0==1 test is replaced by
0<n0<=INT32_MAX/4. This bound is checked before any array read.

After admission it visits all axis0 entries, computes the now-proven safe
signed32 value count 4*n0, and visits all values in native order. Maximum
admitted n0 is 536870911 and value count is 2147483644, so the multiplication
and loop increments do not overflow. No native wrapped-count profile is
silently admitted. Unsupported shapes remain profile refusals, not rewritten
native behavior. Fill and Apply still independently check the current model;
no remembered pointer or successful predicate result was added to AdvancedPage.

The header describes this broader bounded table profile. No signature, CMake,
shared runtime, public session, widget/palette/counter, path-evaluation or
lifecycle source changed. The existing nonzero lifecycle fixture remains
unsupported without another amendment.

## Focused tests and safe allocation

Four new registered Testkit cases were added:

- Two- and three-point tables are allocated on the SAME embedded object
  DYNARY through the real dynary allocator, then populated with mixed signed
  zeros. Fill and changed-time Apply preserve array identities, every sample
  bit, NaN inactive scalar bits and position words.
- Nonzero, subnormal, infinity and NaN at later axis points refuse through
  both Fill and live Apply before the time store, retaining input bits.
- Every dependent value after the first point, including the last point's
  fourth component, is independently exercised with the same rejecting values.
  Returning those samples to signed zero makes Apply complete.
- After Fill, the SAME embedded path is resized/reallocated from two points
  to three through its real allocator. Apply consumes current arrays; a later
  final-component change refuses, and restoring zero permits completion.

The fixture helper frees the previous actual arrays before changing shape and
allocating the replacement; it checks allocation success. It does not simulate
capacity by only changing a count. In particular the CP83 one-point guard test's
unsafe prospective n0=2 assertion was removed. It now checks zero, negative and
two overflow-count inputs before restoring its real one-point metadata. These
profiles refuse before array access. Null arrays and other component/type/
dimension guards remain; the ncomp1 regression is unchanged.

## Preflight, release and limits

Compared the three files with CP83's archived source. All quoted includes
resolve, added macros exist in Testkit, delimiters are balanced, there is one
Testkit main, no C assert, no trailing whitespace and no remaining
`n0=2; refused()` undersized-array fixture. No new include or test registration
is needed. No build, CTest or executable test was run by this worker.

Root target `m2m_mdt_simcontrol300_snapshot`, CTest
`m2m.mdt.simcontrol300_snapshot`, now has 85 cases (81+4). Root owns grouped
build/validation and archive. The three source files are frozen/released:

| File relative to source/apps/moproj2mst | SHA-256 |
|---|---|
| `include/m2m/engine/simcontrol300.hpp` | `d0702d081a2074bc0e6d1646f56846665c3caf6f97e79442b455031a2d0447a2` |
| `src/engine/simcontrol300.cpp` | `9c4cc19de5d61573365dd8a5d1d3bc089a85b2ece8515676e28a7ae3e93f78ee` |
| `tests/mdt/test_simcontrol300_snapshot.cpp` | `d7896740ea11d305e1edec438d684f0e13b9c6bb976bafea7846c7d133994f93` |

No GUI, original execution or mutable REA occurred. Empty/negative/overflow
tables, other component/type/dimension profiles, null/extra-owner states and
nonzero/unordered data remain guarded. The supported valid-array profile uses
actual DYNARY owner/allocation metadata; arbitrary nonnull pointers are not
claimed to prove capacity. Public editor300 remains behind its earlier native
widget/private-palette dependency. Broader zero-table profiles or the nonzero
copy/evaluate/rotation owner chain are the next independent path dependencies.
