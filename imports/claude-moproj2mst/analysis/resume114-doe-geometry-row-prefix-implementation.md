# CP114 Geometry row receiver prefix

The isolated receiver now mutates the actual `HeaderVariableRecord` row owner, `Var::files`, through the normal row insertion, flag and MorphMethod sequence. It has no public parser caller. The existing Geometry refusal before QFileInfo remains in place, and successful execution of this helper proves neither Geometry import nor public Save completion.

## Evidence and owner

`analysis/resume114-geometry-row-contract.md` (SHA-256 `13BBC51079A75FD60920A76A0EC4A2A9FE614D6A427CABADAA21F029D0BC2D80`) qualifies the original GUI PE `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`:

- Reader `0x140911f99` supplies the already selected Var, operation/object integers and joined nominal/upper/lower paths to `0x1408175c0`.
- Lookup compares upper, lower, GeoOperation and GeoObject; nominal is excluded. The first match returns without append or row replacement.
- On miss, constructor `0x140818790` initializes three strings, false flags and two correspondence containers. Insertion assigns nominal/upper/lower, writes both key integers, then appends to Var+0x28.
- The reader then sets both flags on `count-1`, even if an earlier row matched. MorphMethod follows on the variable itself through `0x140817890`.
- Upper absence means nonempty upper path; present exact `false` is false and every other present value is true. Lower absence retains true even for an empty lower path; present exact `false` is false.

The parent selected the existing `Var::files` as the single logical row owner and added optional `GeoFile::geoOperation` / `geoObject` fields to the shared model. This lane did not edit the shared model. Disengaged values identify legacy projections whose original keys are unknown; the helper refuses an owner containing any such row before performing a mutation. This whole-list admission bound is reconstruction policy, not a native input-validation branch.

`HeaderVariableRecord` remains concretely owned by `Study::headerVariableOwners`, with the existing borrowed alias in `Study::vars`. There is no parallel Geometry row list or derived deletion through `Var`. This is a host value/lifetime projection: correspondence containers, native QString reference counts, QList allocation and failure/unwind parity are not represented or claimed.

## API and order

Private `src/doe/geometry_row_prefix.hpp` declares:

```cpp
GeometryRowPrefixResult applyGeometryRowPrefix(
    HeaderVariableRecord&, const GeometryRowPrefixInput&);
```

The input owns the three path values and explicit operation/object/MorphMethod integers. Optional UpperDefined/LowerDefined string values preserve absent versus present-empty. These explicit values do not certify native numeric conversion, QString ownership or QFileInfo construction. CP113's path-value helper may supply the path values conditionally; it supplies no public import capability.

`variable_record.cpp:39` checks all existing keys, scans in list order, constructs/assigns/appends a row only on a miss, then writes tail UpperDefined, tail LowerDefined and variable `geoType` in that order. It returns `appended`, `matchedRowIndex`, `tailRowIndex` and `nextBoundary == GeometryRowPrefixBoundary::CorrespondencePayload`. The index observations describe reached state and are not native return values. Nominal on an existing match remains unchanged. The host local row owns assigned values until append; host allocation exceptions propagate. No exception translation, rollback policy or native allocation-failure equivalence is introduced.

There is no XML traversal, correspondence child consumption, GEO payload I/O, notification, sample update or deferred-link resolution in this helper. The existing B122-specific `GeometryVariable` metadata/writer remains separate and is not populated. The helper accepts a concrete receiver reference; the caller must separately establish the actual selected live Study owner before any production binding.

## Tests and static validation

New `tests/doe/test_geometry_row_prefix.cpp` has six Testkit cases:

1. A real public header read creates the concrete Study record; CP113 path values then populate its existing files owner, preserving unrelated header fields.
2. Two rows followed by an earlier-key duplicate retain the original nominal and modify the tail flags, followed by MorphMethod.
3. Each of the four key components participates; zero/negative integers remain explicit inputs, and null/empty string content compares equally.
4. Empty paths and absent/present-empty/exact-false attributes preserve the asymmetric defaults and case-sensitive comparison.
5. Either or both unknown legacy key fields cause refusal before flags, list size or MorphMethod change, including when an earlier row already matches.
6. Public Geometry still refuses after the reached fresh header prefix, with no row append.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_geometry_row_prefix.cpp` passed. Direct checks on all three changed source/test files found final newlines and no trailing whitespace; tracked source `git diff --check` passed. No worker build, compiler invocation, CTest, GUI or mutable REA operation ran. Root target: `m2m_doe_geometry_row_prefix`; CTest name: `m2m.doe.geometry_row_prefix`. Runtime validation remains root-owned.

## Source release

Source frozen and released to root before report completion:

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| src/doe/variable_record.cpp | `62C06BD5CB971E22E078D2FC469095B72766D54FD0A015F240501F3671FA5AC1` |
| src/doe/geometry_row_prefix.hpp | `28ADCA09276E876618CD039AFB42B1A4D4BDFD935B758A9F47478936A5514BED` |
| tests/doe/test_geometry_row_prefix.cpp | `39C7228D2CFAD346E13463A34682D2156E77235697843EB079CD9BEFCC06559E` |

Parent-owned shared `include/m2m/mst/doe_model.hpp` observed hash: `698B0A7F24F0F83DAAE87E8B5EA94F4C9EA38846268C131B67DAAED54CC7E5D4`. No CMake, shared writer, public header, ABI or plan edit by this lane.

## Next dependency

Before public integration, bind the actual selected record and qualified QFileInfo/path/conversion occurrence at Geometry entry. The next unimplemented receiver effect after this prefix is the ordered Upper/Lower correspondence traversal and PointPair storage through `0x140818000` / `0x140818080`, including its row target and malformed-input effects. Recover those exact container element types, append decisions and tail selection before adding correspondence fields or a public call. GEO payload import and active Save reconciliation/refresh delivery remain independently guarded. `admin/check_continuation.py` reports CONTINUE with 15 outstanding items; this source release is not objective completion.
