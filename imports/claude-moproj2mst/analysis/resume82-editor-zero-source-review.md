# CP82 independent source review — movement-kind6 zero path

## Result

The frozen three-file change matches the retained source contract for the selected constant branch. I found no source-level discrepancy in the requested points: the guard reads the actual GuiModel-owned object and its embedded DYNARY at object `+0x25a8`; it admits only `ndim == 0` and a constant comparing equal to zero; it rejects table dimensions and all nonzero/unordered values; and Apply reruns the live guard before its first model or page store.

## Source trace

`GuiModel::objectAt()` in `source/apps/moproj2mst/src/engine/gui_model.cpp` returns `step->objects.at(no)`, so `advancedTimeProfile()` scans the current model object's actual storage. The offset relation is `movement+0x448 == object+0x25a8`; the native predicate evidence in `analysis/resume82-editor300-next-ready.md` independently names this same embedded owner. The guard at `simcontrol300.cpp:438-452` skips null and kind7 objects, looks only at movement kind6, and reads the embedded DYNARY. It returns false for any `ndim != 0` before evaluating the constant, so it does not traverse table axes/values. With `ndim == 0`, `value != 0.0` refuses nonzero values and NaN; both `+0.0` and `-0.0` compare equal and pass. The original bounded PE branch has the same order: test `ndim` first, then compare the scalar with literal zero; `JP` rejects unordered and `JE` accepts both signed zeros. It does not inspect array pointers on that constant branch.

`fillAdvanced3D()` at `simcontrol300.cpp:1138-1187` applies this guard to the live model while constructing the page snapshot. `advancedApplyProfile()` at lines 457-466 calls it again, and `applyAdvanced3D()` at lines 1191-1194 returns `UnsupportedSnapshot` before its first store if the current model no longer qualifies. The only preceding local work in Apply is read-only validation/text conversion. The outer empty-recovery Apply also prechecks the live guard before clearing its own recovery entries.

## Regression coverage

The four new cases in `tests/mdt/test_simcontrol300_snapshot.cpp` cover:

- both signed zeros, fill and Apply completion, bit-preserved path scalar, and unchanged object position words containing a NaN payload, negative zero and a finite value;
- positive/negative nonzero, smallest positive subnormal, both infinities and quiet NaN refused by Fill and by Apply after Fill, with the time store untouched; changing the same actual path to `-0.0` makes Apply complete;
- a valid allocated `ndim=1` zero-valued table remains refused and its arrays/time remain intact;
- a new later live kind6 object with a nonzero constant invalidates both Fill and Apply after an earlier supported zero-path snapshot, while changing the later object to kind7 follows the existing skip rule.

The C++ DYNARY layout has `ndim` at `+0x04`, constant `value` at `+0x08`, arrays from `+0x28`/`+0x48`, and size `0x78`. The table test allocates through the existing Testkit fixture helper after freeing the default arrays, then leaves that ownership to the ordinary DYNARY destructor. The constant branch intentionally does not require array pointers to be null, matching the original scalar branch's reads; the explicit table regression uses `ndim=1`.

## Limits and validation status

This review confirms the internal advanced-page branch only. It does not prove that public editor300 composition can reach it through earlier Open/widget/palette gates, nor does it recover table traversal or nonzero movement evaluation. The CP82 implementation report states that this worker did not build or run the focused target; this independent source review likewise did not build or execute tests. Root owns frozen build/CTest validation.

Reviewed frozen source hashes:

| File | SHA-256 |
|---|---|
| `include/m2m/engine/simcontrol300.hpp` | `d64f9245e8f8140a51aa56389698f78ae9e999286c83f471f02c6c5d70b93718` |
| `src/engine/simcontrol300.cpp` | `813e14c4157537beb9223a600644e93933c40826973d29be5d45977ce9da1b6b` |
| `tests/mdt/test_simcontrol300_snapshot.cpp` | `c49c53850276dcf8e8affd3cc378bf883864f1ef1cc430cd19d2ef9cdf6ea47d` |

The selected PE qualification is in `analysis/resume82-editor300-next-ready.md`; no new original runtime evidence was needed or acquired for this review.
