# CP85 independent source review — null-array empty path

## Result

The frozen empty-path branch is narrowly implemented as specified: for the current object-owned type7a/ndim1/ncomp4 path, `n[0] == 0` is accepted only when both `axis[0]` and `values` are null. It reads no sample or scalar `value`, performs no allocation/evaluation, and continues the object scan. A zero count with nonnull payload remains refused. The existing common inactive-axis, dimension, component, signed32 count, and `u68` guards remain in front of the branch. Apply rechecks this predicate against the current model before its first store.

I found no discrepancy in the requested owner, cleanup, payload, or live-Apply behavior.

## Source and ownership

`GuiModel::objectAt()` returns the current `step->objects.at(no)` object. `advancedTimeProfile()` at `source/apps/moproj2mst/src/engine/simcontrol300.cpp:438-471` reads the embedded DYNARY at object `+0x25a8`; this is not a copied page curve. The scalar `ndim==0` branch remains separate. For tables, the common profile guards require type `0x7a`, `ndim==1`, nonnegative and signed32-safe `n[0]`, `ncomp==4`, no `u68`, and empty inactive dimensions/axes. At `n[0]==0`, the new branch rejects either nonnull active payload pointer and otherwise continues immediately. Thus it does not enter the positive-count array loops or inspect inactive scalar storage.

The native receipt in `analysis/resume85-editor300-next.md` says the axis loop skips at zero count and the value-count helper returns `4*0`; the dependent-value loop also skips. The pointer-null requirement is the candidate's owner restriction to an empty allocation profile, not a claim that original code dereferenced or checked those pointers. The retained candidate cleanup source has a reason to keep that restriction: `dynary_free_arrays()` treats `n[i]==0 && axis[i]!=nullptr` as a shared/borrowed marker and suppresses normal frees.

## Apply and fixture safety

`fillAdvanced3D()` and `advancedApplyProfile()` both call `advancedTimeProfile()` on the live `GuiModel`; `applyAdvanced3D()` gates before the first model store. The new positive-to-empty test first calls `dynary_free_arrays()` while positive counts still describe the owned arrays, then sets `n[0]=0`. It proves Apply uses that current empty owner, and separately checks that a later negative count or nonzero ndim0 constant is refused before time mutation.

The initial empty-owner test follows the same safe order: the actual embedded arrays are released while `n[0]>0`, then the count changes to zero. The test records null pointers, zero dimensions, NaN scalar bits and position bits across Fill/Apply. This both exercises no-sample behavior and leaves ownership metadata safe for ordinary DYNARY destruction.

The zero-count/non-null-payload test uses real one-point allocations. It temporarily changes only `n[0]` and, separately, one payload pointer to prove refusal. Before the owner is destroyed it restores the positive count and both original pointers. This is necessary because the free helper interprets a zero-count/non-null-axis pair as borrowed storage. The old one-point guard's zero-count rejection remains intact and exercises this nonnull profile.

## Remaining boundaries

The path's scalar `value` is intentionally not checked for `ndim==1`, `n[0]==0`; its NaN sentinel test verifies it stays unchanged. No sample read, curve copy, path evaluation, position write, or new allocation is performed. Nonnull/borrowed payloads, negative and overflow counts, other type/dimension/component shapes, extra owners, and nonzero or unordered samples remain guarded. Public editor300 remains behind earlier native widget/private-palette guards.

The DYNARY model does not store allocation extents. The admitted null-array empty profile is directly inspectable; positive-array profiles continue to rely on valid model-owned allocation metadata, as documented in the prior CP84 review. This report does not claim arbitrary pointer/capacity safety or native repeated-Open acceptance.

## Hashes and validation

Reviewed source hashes:

| File | SHA-256 |
|---|---|
| `include/m2m/engine/simcontrol300.hpp` | `f06a8a7ea6874cb00b327ff9cf8e6938199f939eac9f44def2dedb99b982ce4f` |
| `src/engine/simcontrol300.cpp` | `3ee26f182a1140d699d5035b00c7d587be4261f9e76d6569c055ef100c95ac5d` |
| `tests/mdt/test_simcontrol300_snapshot.cpp` | `9f5d57e268c33b0af4f56c27f84cea7a3aaad35463e136441a2372400444bf8f` |

The implementation report states static preflight only, with no build or CTest. I performed this review without source edits or test execution.
