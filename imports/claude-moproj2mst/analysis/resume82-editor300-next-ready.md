# CP82 next editor branch: constant-zero movement path

The next bounded increment is the advanced-page time callback's constant-zero
path branch. Current `advancedTimeProfile()` rejects every live movement-kind6
object. Original `140892370` instead returns false when its actual embedded path
curve is identically zero, so caller `1408bfe60` skips the unrecovered path
evaluation/rotation call `140874630`. The smallest closed subset is
`DYNARY.ndim==0 && value==0.0`; both signed zeros qualify. Nonzero, NaN and all
table curves remain guarded.

This is a reachable branch of the original mode300 advanced Apply call chain,
and a bounded extension to its existing internal helper. It does not establish
that the current public Open path has passed its earlier editor/private-palette
constructor guard. No captured session is claimed to have this curve profile.

## Reconciliation with the old audit

`resume46-editor-settings-audit.md` predates multiple source increments. Its
movement-warning stopping refusal is no longer current: `applyStopping3D()` now
implements the source default-No skip of six limits, and focused cases cover
all six warning kinds. The stopping callbacks now retain partial scalar,
selector, changed-spin, coordinate-allocation/StateVar and formatting effects in
`StoppingFillReport`. Environment now has actual retained curve owners, ACVCOF
selection callbacks, TIMEID table projection and checked view-factor handling.
Meshing already remembers and applies its four scalar fields with original
label sorting. Reimplementing those old audit items would duplicate recovery.

The advanced guard remains current and explicit. `simcontrol300.cpp` function
`advancedTimeProfile()` rejects kind6 at object+21c8 before looking at its path;
both `fillAdvanced3D()` and `advancedApplyProfile()` use that guard. The header
still states that all valid kind6 objects are excluded. The existing test
`advanced_unrecovered_profiles_refuse_without_stores` verifies blanket refusal
using a freshly constructed object's table path. That table fixture remains
unsupported under this proposal, so its expectation should remain intact.

## Original values, owner and exact branch

Retained `wave2-save-diffs-300-apply-140ec7500.c` shows the advanced Apply calls
`1408bfe60` after its time/radio stores, once unconditionally and again if either
time changed. Its actual receiver comes from editor vt+198 then `140c7acd0`.
Retained `wave2-save-diffs-300-advanced-time-callback-complete.asm` shows that
callback reading the selected time through `1408dfc30`, scanning actual objects
by list position, skipping null/invalid-kind objects, and calling `140892370`
with object+2160. Only a true result reaches `140874630`.

`140892370` tests movement+68==6, then passes movement+448 to `14089baf0`.
Thus its data owner is the actual selected GuiModel object's embedded DYNARY at
object+25a8, not a cloned page curve or newly allocated observation. The
candidate already has this live model/object registry and DYNARY layout.

Bounded DUMPBIN against the pinned original proves the missing predicate edge:

| Native address | Effect |
|---|---|
| `14089bafe` | Compare path+4 (`ndim`) with zero |
| `14089bb02` | Nonzero jumps to the table traversal; keep this branch guarded |
| `14089bb09` | Read constant double path+8 |
| `14089bb0e` | UCOMISD against literal at `1421ad1d0`, verified bits `0000000000000000` |
| `14089bb16` | JP rejects unordered/NaN |
| `14089bb18` | JE selects true for +0 and -0 |
| `14089bb1a` | Nonzero returns false |
| `14089bb21` | Jump to `14089bbf1`, return AL=1 |
| `140892399..23a2` | True all-zero predicate makes movement predicate return false |
| `1408bff10..ff14` | False movement predicate skips `140874630` and advances scan |

No axis/value-array pointer is read in this constant branch; no allocation,
curve copy, interpolation, object-position write or transform occurs. There is
no epsilon comparison. Native signed zeros compare equal. NaN is not zero.
The existing inherited `defcmd::ifd::path::table_all_zero` describes a similar
full predicate, but directly reusing it would admit table traversal and require
more shape/pointer qualification than this increment. Use the bounded actual
GUI branch instead of broadening through that helper.

## Exact exclusive write set and regression boundary

Root approved these three existing files after the evidence selection:

- `source/apps/moproj2mst/src/engine/simcontrol300.cpp`: in the existing live
  guard, permit a kind6 object only when its actual path has ndim0 and value0.
  Keep null/kind7 skips and all other preconditions; update the callback comment.
- `source/apps/moproj2mst/include/m2m/engine/simcontrol300.hpp`: clarify the
  narrower supported profile; no signature or ABI change.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp`: focused
  Release-active Testkit cases using actual GuiModel-owned object/path storage.

Tests should prove positive/negative-zero admission through Fill and Apply;
preserved path and position bits; nonzero, infinity/NaN and even zero-valued
tables remain refused; a curve changed after Fill is rechecked before any Apply
store; and one newly unsupported live object cannot be hidden by an earlier
supported one. Existing table and kind7 regressions remain. No public/session,
view-manager, model schema, CMake or palette/counter changes are needed.

The supported branch proves the existing callback no-op claim for one additional
actual model profile. It does not claim path evaluation recovered or replace the
live Apply guard with remembered Fill state. Root owns build/test integration.

## Evidence receipt

Only this report was written during selection. No source/build/test/GUI/original
execution/mutable REA was performed. New static inspection used the verified
MSVC DUMPBIN 14.44.35228.0 (binary SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`).
Original `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Preferred-VA half-open byte ranges:

| Range/artifact | SHA-256 |
|---|---|
| `14089baf0..14089bb26` | `10e99fc7dbf8e2bbdcca41e6a884a99e662c345a049222b0ba79081554b1ea78` |
| `14089bbf1..14089bbf8` | `28e556ff73dfba50a7af2c06d9fa878e1e0e34f8623d3109ea456768835cfd49` |
| `140892370..1408923ab` | `6c4d64c08dea9d6d808caf69c96abd2160add0e2f465a3eda1095ce198abd000` |
| `1408bfe60..1408bff2c` | `440ddb108db67c7a6cd7feece1ced103922b05761f0304a1bb6a564ec98d4e4d` |
| `wave2-save-diffs-300-apply-140ec7500.c` | `f36256fe2a8bae92c2e23ebf8d656eb0ef45be2bc3486222eb91ff8bf9f6ded9` |
| `wave2-save-diffs-300-advanced-time-callback-complete.asm` | `638815eb8bd4dc205b694228dece57603f8f384a2a6abf1d785db0fdd340ef23` |
| `wave2-save-diffs-300-advanced-motion-predicate.asm` | `bf6ce10c799cd2b6133940ee5344e169e1e85fce81a9582c67e8aaafc43674f6` |
