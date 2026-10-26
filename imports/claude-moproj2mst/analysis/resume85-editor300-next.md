# CP85 next editor cut: null-array empty 3D path table

The smallest next internal increment is the actual type7a/ndim1/ncomp4 path
with zero points and null arrays. Original `14089baf0` returns all-zero for this
table without reading any sample: its axis loop skips and `14089bc90` computes
4*0=0, so the value loop skips too. The movement predicate therefore returns
false and the advanced time callback bypasses `140874630`. No evaluator,
allocation, position write or new runtime provider is required.

Only this report was written. CP84 source remains frozen. No source/build/test
execution, GUI/original execution or mutable REA occurred.

## Reconciled current boundary

CP82 covers actual ndim0 constant signed zero. CP83 added the owned one-point
four-component zero table. CP84 now covers positive point counts whose 4*n0
value count fits signed32, checking ALL axis samples before all dependent
samples. Its snapshot target has 85 cases recorded by its worker's static
receipt; this report does not claim a new execution/acceptance result.

The next retained restriction is n0<=0 and required nonnull arrays for every
table. It correctly refuses borrowed-array markers but also refuses the exact
null-array empty case above. Earlier audits' blanket movement6 exclusion,
stopping warning/default-No omission and environment-curve gaps are no longer
the current code boundary. The public editor300 route still stops earlier at
native widget/private-palette state; no public bypass is proposed here.

## Native proof and owner distinction

The original selected call chain is still
`140ec7500 -> 1408bfe60 -> 140892370 -> 14089baf0` on the actual object-owned
path DYNARY at object+25a8 (movement+448). Retained complete byte ranges establish:

- `14089bb49..bb70`: j starts at 0 and is compared with signed n[i]. For the
  only active dimension with n0=0, JGE skips its axis pointer load/read.
- `14089bb9f..bba9`: call the value-count helper after the axes. Its ndim1
  path multiplies ncomp4 by n0=0 through `14089bd3b`, returning zero.
- `14089bbb7..bbca`: value index0 is already >= count0; jump directly to
  `14089bbf1`, AL=1, RET. The values pointer is never dereferenced.
- `140892399..23a2`: all-zero true becomes movement-on-path false; caller
  `1408bff10..ff14` skips the unrecovered copy/evaluate/move call.

Native ignores the scalar constant field on this table branch. Its arbitrary
bits, including NaN, cannot become an input validation rule. Likewise this
branch reads no axis or value sample, so zero-point admission must not inspect
an invented sample or allocate a replacement zero point.

Candidate DYNARY cleanup distinguishes n[i]==0 with a nonnull axis pointer as
a shared/borrowed-array marker. This proposal keeps that representation guarded.
It admits the actual null-array owner, not every pointer payload on which the
native loop happens not to dereference. These extra restrictions bound the
candidate's supported owner profile; they are not attributed to native tests.

## Exact implementation and source files

Preserve the scalar branch and CP84's positive-table loops. For tables keep
type7a, ndim1, ncomp4, zero inactive counts, null inactive axes and null u68.
Refuse negative or signed32-overflow point counts. Then split:

1. n0==0: require axis0==null and values==null; qualify immediately without
   any sample read. Inactive axes must already be null from the common guard.
2. n0>0: retain both nonnull pointer requirements, then the existing axis-first
   and 4*n0 value loops with signed-zero comparisons.

Use the SAME actual GuiModel object's embedded DYNARY. No page-owned copy,
getter observation, count seed or derived empty object belongs here. Apply must
still rerun the live predicate before its first store, including after a curve
transitions from populated to empty between Fill and Apply.

Only the existing three files are needed after root's CP84 freeze/release:

- `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: partition the common
  valid-shape checks from empty/positive pointer guards as described above.
- `source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp`: describe null-array
  empty tables alongside the bounded positive table profile.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp`: add focused
  empty-owner and live-transition cases; clarify the old zero-count fixture.

No CMake, lifecycle, view-manager, palette/counter, public session or path
evaluation source requires an edit. Root target remains
`m2m_mdt_simcontrol300_snapshot` / CTest `m2m.mdt.simcontrol300_snapshot`.

## Focused tests and fixture reconciliation

Three useful cases are sufficient:

1. Free the actual default owned path arrays WHILE the original positive counts
   are still present, then set n0=0 while preserving type7a/ndim1/ncomp4 and
   null inactive arrays. Fill and changed-time Apply succeed with null arrays,
   scalar NaN bits, metadata and position bits preserved. Do not use the current
   positive-only `zeroPathTable` fixture helper without changing its contract.
2. Fill with an actually allocated positive-length zero path, then release its
   arrays through the actual embedded owner and change its count to zero.
   Apply uses the now-empty live owner and succeeds. A subsequent negative
   count, nonzero ndim0 constant or other unsupported shape refuses before
   time stores, proving that successful prior Fill is not cached admission.
3. Zero count with nonnull axis/value payloads remains refused. Use actual owned
   allocations, retain/restore their pointers and positive counts around the
   transient guard checks, and free them normally. Positive count with missing
   arrays, negative/overflow count and ncomp1 remain refused by existing cases.

The existing `advanced_one_point_path_refuses_other_shapes_and_missing_arrays`
sets n0=0 while retaining its real one-point axis/value pointers. Keep that
refusal expectation: it is a nonnull-array zero-count profile, not the proposed
empty owner. Add a comment identifying the distinction rather than deleting its
assertion. CP84's corrected overflow/negative fixtures remain valid, and the
snapshot/lifecycle explicitly nonzero fixtures still refuse. No test should
set count0 before releasing its actual arrays: that can mark them borrowed and
make cleanup skip ownership that the fixture intended to release.

## Evidence receipt and remaining branch

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Retained preferred-VA half-open ranges:

| Range | SHA-256 |
|---|---|
| `14089baf0..14089bbf8` | `56e6b1e7879acafaf419ece4d90ceec2050b772f29c2f97b7ebf152e48ffab5f` |
| `14089bc90..14089bd5a` | `5ae2599c33f066b2c5d1a08d24baf761fa0472ff982331b463e6e7b6ffb597ea` |

The candidate allocation/free/count implementation is
`source/port/defmodel/src/table/dynary_template.cpp`, SHA-256
`e8170c39949a9fdc9b8e2c776b87bc95106a3ed18d69be890c0fded3792ab2ef`.
Full positive-loop/caller qualification is retained in
`analysis/resume84-editor300-next.md`, SHA-256
`eef855188042565d6c6dda74674aad45ca914586af75cf4118ad86a29f7fd1c1`.
No new raw artifact was needed.

Other types/components/dimensions, borrowed/extra-owner states, negative and
overflow metadata, and nonzero/unordered samples stay guarded. The inherited
DEF_PRE `defcmd::ifd::path::place_on_path` is not a substitute for recovering the
GUI nonzero chain: it copies/converts the path, evaluates at current time,
computes delta, may translate geometry and writes current position through an
actual model owner. Those producer, callback and cleanup effects still need
GUI/native qualification before a nonzero profile can be admitted.
