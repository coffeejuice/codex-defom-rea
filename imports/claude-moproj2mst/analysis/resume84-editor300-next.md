# CP84 next editor cut: bounded positive-length zero path table

The next source-qualified increment replaces CP83's fixed one-point restriction
with the native one-dimensional axis/value loops for actual type7a path data.
Admit a positive point count only while four times that count fits in signed
32 bits. Check every axis sample first, then all four dependent components per
point. Retain the existing owner/shape guards, signed-zero behavior, live Apply
recheck, and refusal of nonzero/unordered samples. No path evaluator is needed.

Only this report was written. CP83's four source/test files remain frozen. No
build/test execution, GUI/original execution or mutable REA was performed.

## Current code and audit reconciliation

CP82 recovered the constant-zero predicate. CP83 now recovers the actual
type7a ndim1/n0=1/ncomp4 zero table, with axis-first comparisons and all FOUR
values. `advancedTimeProfile()` is used by both Fill and the pre-store Apply
profile check. The header and 81-case snapshot test file reflect that boundary;
the 28-case lifecycle fixture was changed to an explicitly nonzero table. This
report does not claim a new build/test acceptance result.

Thus the earlier audit's blanket movement6 exclusion is stale for constants and
one-point tables. Stopping/default-No and environment retained-curve recovery
also already exist. The remaining narrow limitation at this predicate is now
the point-count equality test, not missing page/model ownership or missing
native code. General dimensions, other path/component types, malformed metadata
and actual nonzero path evaluation remain separate gaps.

The original mode300 advanced Apply call chain still reaches the predicate via
`140ec7500 -> 1408bfe60 -> 140892370`. Successful all-zero comparison means the
last function returns false; the caller skips `140874630` and its copy,
interpolation, transformation and position writes. This is reachable original
conditional behavior and a candidate internal-helper continuation. It does not
close the earlier public editor300 widget/private-palette lifecycle gate.

## Exact retained native contract

CP83 pins the complete `14089baf0` predicate and `14089bc90` value-count helper.
No further decompilation or runtime capture is required for the selected loop.
For ndim1, the native ordering is:

1. Load n[0] and iterate axis[0][j] from j=0 while j<n[0]
   (`14089bb49..bb9d`). UCOMISD/JP rejects unordered; zero of either sign
   continues. Any nonzero axis exits before the value-count call.
2. After the whole axis, call `14089bc90` (`14089bb9f..bba9`). It starts with
   ncomp and multiplies each active n[i] with 32-bit IMUL. A negative wrapped
   result is replaced with zero. This exact wrap behavior is not a capacity
   proof or a reason to admit extreme shapes into the candidate.
3. Iterate all resulting values in contiguous order
   (`14089bbb7..bbef`), with the same zero/unordered comparison. Return true
   only after both loops succeed (`14089bbf1..bbf7`).

For type7a/ncomp4 and positive n0<=INT32_MAX/4, the native product is exactly
4*n0, with no wrap. The upper point bound is 536870911; the largest admitted
value count is 2147483644. Loop increments terminate within signed32 range.
Reject overflow profiles before reading arrays; do not reproduce a wrapped
count by then traversing billions of possibly unrepresented axis elements.
Those profiles remain unsupported, rather than acquiring invented behavior.

The scalar path.value, inactive axes and transform/position data are not native
inputs on this table branch. Keep the constant branch unchanged. No epsilon,
normalization, sorting, early allocation or curve copy belongs in this predicate.

## Exact implementation cut and owner boundary

Use the existing actual GuiModel object, its embedded movement at object+2160,
and the SAME DYNARY at object+25a8. Preserve these CP83 table-profile guards:
type7a, ndim1, ncomp4, nonnull axis0/values, zero n1/n2, null inactive axes and
null u68. Change only n0==1 to 0<n0<=INT32_MAX/4. These are candidate supported
profile guards, not additional tests attributed to native `14089baf0`.

After the guard, loop over axis0[0..n0), compute the proven nonoverflowing
4*n0 count, then loop over values[0..count). Each sample uses !=0.0 so NaN and
nonzero values refuse while both signed zeros pass. Keep source order rather
than interleaving a point's axis and dependent values. Fill and Apply continue
to call the same live predicate; never retain the pointer or a prior successful
zero result in AdvancedPage.

This valid-profile boundary uses the already represented DYNARY owner and its
matching allocation metadata. `dynary_alloc()` allocates axis arrays from n[i]
and the value array from `dynary_value_count()`. Tests must use that actual
embedded owner/allocator when changing the valid point count. A nonnull raw
pointer by itself does not prove capacity; this proposal does not add a new
registry or pretend that arbitrary mismatched count/pointer fixtures are valid.
The current one-point code has the same live-model validity premise. Null and
overflow profiles must refuse before any possible array traversal.

## Exclusive source/test set and required reconciliation

After root freezes/releases CP83, only these three files are needed:

- `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: replace the point-count
  equality with the bounded positive count, then implement the two ordered loops.
- `source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp`: document the
  supported positive-length 1D/four-component table profile.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp`: add focused
  allocated multi-point cases and amend the old point-count refusal fixture.

The critical existing fixture is
`advanced_one_point_path_refuses_other_shapes_and_missing_arrays`: it temporarily
sets n0=2 while still holding a ONE-point allocation, expecting the equality
guard to refuse. Once n0=2 is supported, leaving that assertion unchanged would
cause an out-of-bounds read. Replace it with a pre-read overflow-count refusal
(INT32_MAX/4+1 or INT32_MAX) and/or a negative count, then restore n0=1. Do not
merely change its expected result or let it traverse the undersized arrays.
The existing n0=0 and ncomp1 refusals remain. Snapshot/lifecycle nonzero fixtures
continue to refuse; lifecycle needs no further change.

Meaningful focused cases:

1. Allocate actual type7a n0=2 and n0=3 tables with four values per point; set
   mixed signed-zero samples, plus NaN in inactive scalar storage. Fill and
   changed-time Apply succeed with pointer/sample/position bits preserved.
2. A nonzero/subnormal/NaN at a later axis point refuses even with all values
   zero; a nonzero/NaN in the LAST point's fourth dependent component also
   refuses. This distinguishes complete loops from CP83's fixed first sample.
3. After Fill, resize/reallocate the SAME embedded path through its actual
   allocator to another valid all-zero length. Apply uses current arrays and
   succeeds; a subsequent late sample change refuses before time/model stores.
4. Zero/negative/overflow counts and missing arrays refuse before traversal;
   restore transient test metadata before owner destruction. Keep other type,
   dimension, component and kind7 guards, plus CP82 signed-constant tests.

Root target remains `m2m_mdt_simcontrol300_snapshot` / CTest
`m2m.mdt.simcontrol300_snapshot`. No CMake, public session, view-manager,
palette/counter or path-evaluation file needs an edit. No test execution is
claimed by this report.

## Evidence receipt and next dependency

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The retained preferred-VA half-open native ranges are:

| Range | SHA-256 |
|---|---|
| `14089baf0..14089bbf8` | `56e6b1e7879acafaf419ece4d90ceec2050b772f29c2f97b7ebf152e48ffab5f` |
| `14089bc90..14089bd5a` | `5ae2599c33f066b2c5d1a08d24baf761fa0472ff982331b463e6e7b6ffb597ea` |

Full caller/source qualification is retained by
`analysis/resume83-editor300-next.md`, SHA-256
`476b5140de3c943660ec4d3c8da8e5718f6411f5e78778c7ae781470a1d8bbfc`.
The candidate allocator/count implementation is
`source/port/defmodel/src/table/dynary_template.cpp`, SHA-256
`e8170c39949a9fdc9b8e2c776b87bc95106a3ed18d69be890c0fded3792ab2ef`.
No new raw artifact was written.

After this increment, empty tables, additional components/types/dimensions,
overflow profiles and nonzero movement still remain guarded. A broader zero
predicate can be qualified separately; the next behavior-changing path is the
actual nonzero copy/evaluate/rotation owner chain, which remains unrecovered.
