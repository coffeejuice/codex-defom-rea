# CP83 next editor cut: one-point all-zero 3D path table

The next source-qualified advanced-page increment is the actual model-owned
type7a path with ndim1, one axis point and four dependent values, all equal to
zero. Original `14089baf0` checks the axis first and then all FOUR dependent
values; its true result makes `140892370` return false and the time callback
skip `140874630`. This is a bounded table-predicate extension, with no path
evaluation, copy, transform or position mutation.

Only this report was written. CP82's three source files remain frozen. No
build/test execution, GUI/original execution or mutable REA was performed.

## Reconciled boundary

Current `advancedTimeProfile()` at simcontrol300.cpp:438 already admits ndim0
constant +0 and -0. Its live-model checks are used by Fill and before Apply's
first store. CP82 added four cases, and the file now contains 77 cases. Its
independent source review agrees with that bounded contract; this report makes
no new execution/acceptance claim. The older broad movement6 exclusion in the
wave2 advanced contract is therefore stale for constant zero, but remains true
for every table in the current candidate.

The stopping warning/default-No, partial node-fill, environment curve ownership,
ACVCOF and TIMEID omissions in older audits are already implemented. This
proposal does not repeat them. The new table branch extends the existing actual
GuiModel object owner and requires no runtime observation registry or fabricated
GUI defaults. The public mode300 constructor/private-palette gate remains
earlier in the lifecycle; a helper increment does not enable that public route.

## Exact native table path

CP82 pins the caller chain `140ec7500 -> 1408bfe60 -> 140892370`. The actual
object's movement owner is object+2160; its path is movement+448/object+25a8.
The predicate is `14089baf0`, with these ordered effects:

| Native range | Read or decision |
|---|---|
| `14089bb26..bb47` | Iterate dimensions in signed order, starting at zero |
| `14089bb49..bb70` | For each dimension, iterate its signed point count |
| `14089bb72..bb95` | Read axis[i][j]; UCOMISD with literal zero, JP rejects NaN, JE continues |
| `14089bb9f..bba9` | Only after all axes, call value-count helper `14089bc90` |
| `14089bbb7..bbca` | Iterate that signed count from zero |
| `14089bbcc..bbe9` | Read dependent values in order; nonzero or unordered rejects |
| `14089bbf1..bbf7` | Return true after all comparisons succeed |

The count helper begins with ncomp (path+40), then for ndim!=0 multiplies by
each signed n[i] through 32-bit IMUL at `14089bd3b`, replacing a negative final
count with zero. For the selected shape, exactly 4*1=4 values are visited with
no arithmetic overflow. It is not a three-component position-only predicate:
ignoring dependent component 3 would falsely admit a moving path. Axis zero is
also necessary; zero dependent values alone do not qualify. The constant field
at path+8 is ignored on this table branch.

The literal at `1421ad1d0` is exact +0.0. Both signed zeros in axis/values compare
equal; NaN fails. There is no tolerance and no write to the curve. The inherited
`defcmd::ifd::path::table_all_zero` has similar broader behavior, but calling it
would admit general shape traversal. Keep this GUI-qualified bounded branch.

## Proposed exact C++ admission

Retain the CP82 constant branch unchanged. For a nonconstant path, initially
admit only the actual embedded DYNARY satisfying:

- type==0x7a, ndim==1, n[0]==1, ncomp==4;
- axis[0] and values nonnull; n[1]==n[2]==0 and axis[1]==axis[2]==null;
- u68==null, excluding an additional owned/provider object from this first cut.

These are candidate supported-profile guards, not extra tests claimed to exist
in native `14089baf0`. They bound one already represented allocation shape.
The candidate's actual movement default initializes type7a for 3D; its existing
DYNARY template creates ndim1/n0=1/ncomp4 and allocates owned arrays. Explicit
tests should establish those fixture facts before filling their samples. A
type7a keyword alone or an unrelated page snapshot must not synthesize this
owner or its array pointers. The existing candidate assumes valid live DYNARY
allocation metadata for its supported model-owned arrays; this does not claim
that arbitrary nonnull pointers prove allocation extent.

After those guards, compare axis[0][0] with zero, then values[0..3] in source
order; any `!=0.0` refuses. Do not read the scalar path.value, normalize signed
zeros, call a curve evaluator or copy data to a new path. Keep larger/empty
tables, other types/components/dimensions, malformed/null-pointer shapes and
nonzero/unordered samples guarded. Apply must rerun the same live predicate so
changes after Fill cannot be hidden by a remembered successful profile.

## Exclusive write set and meaningful tests

The smallest source set is the same three existing files, only after root's
CP82 freeze/release:

1. `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: extend only the existing
   private live-profile guard with the selected shape and ordered comparisons.
2. `source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp`: document this
   additional bounded profile; no new public API.
3. `source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp`: focused
   Release-active tests plus the existing fixture expectation amendment below.

Recommended cases: actual allocated type7a one-point +0/-0 axis/value data passes
Fill and Apply with pointer/data/position bits unchanged; nonzero/subnormal/NaN
axis refuses even when all values are zero; each of the FOUR dependent components
independently makes a previously filled page refuse before Apply time stores;
and valid other shapes stay refused. A NaN scalar `path.value` with all traversed
table samples zero should pass, proving that inactive scalar storage is ignored.
Keep mixed-object/kind7 and live-recheck coverage from CP82.

One existing test must change coherently: `advanced_unrecovered_profiles_refuse_without_stores`
currently obtains a freshly constructed default type7a table and expects
blanket movement6 refusal. Make that actual table nonzero before the refusal
assertions; do not leave a now-supported default shape as its rejected fixture.
The CP82 `advanced_zero_path_table_still_refuses_without_entering_table_traversal`
uses ncomp1, so it remains refused. Retain it (with a name/comment clarification
if useful) as proof that the increment is not general table admission.

No CMake, view-manager/runtime, source registry, public session, palette, widget
counter or path evaluation files need edits. Existing root target remains
`m2m_mdt_simcontrol300_snapshot` / CTest `m2m.mdt.simcontrol300_snapshot`.

## Evidence receipt and next dependency

Static original: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Bounded views used the already verified DUMPBIN 14.44.35228.0, SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Native half-open ranges and retained source receipts:

| Range or file | SHA-256 |
|---|---|
| `14089baf0..14089bbf8` (264 bytes) | `56e6b1e7879acafaf419ece4d90ceec2050b772f29c2f97b7ebf152e48ffab5f` |
| `14089bc90..14089bd5a` (202 bytes) | `5ae2599c33f066b2c5d1a08d24baf761fa0472ff982331b463e6e7b6ffb597ea` |
| `source/port/defmodel/src/table/dynary_template.cpp` | `e8170c39949a9fdc9b8e2c776b87bc95106a3ed18d69be890c0fded3792ab2ef` |
| `source/port/defmodel/src/llobj_ctor/obj_defaults.cpp` | `21adcd05b33c2810f1303ca8e7fd576a91720d48933b41c7c8af0f70e0edc7e4` |
| `analysis/resume82-editor-zero-source-review.md` | `ef4ed0edc58e073ab161765de9ab16191bab51b7fa06a51cd4e36dd79c986af7` |

CP82's selection report retains the caller/predicate hashes and original scope.
No new raw capture artifact was needed. After this proposed increment, general
all-zero tables still need shape/extent and count-arithmetic qualification;
nonzero movement paths still need the actual copy/evaluate/rotation chain.
