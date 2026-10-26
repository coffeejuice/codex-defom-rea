# CP231: next editor native-alias leaf

## Recommendation

The next finite leaf is `StagedEditor300::frequencyTextEdited` in `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`. Its native effect is one bounded write to a single live `FRQNCY` row. Unlike the completed frequency-index command, this callback has a meaningful conversion point between table-shape admission and the values-pointer read. A command that accepts the already-converted integer would move `qt::toInt` too early, before the geometry/control/table checks. Keep the conversion inside the named model operation.

Proposed signature:

```cpp
bool GuiModel::applyEditorFrequencyTextIfControlMatches(
    const def::model::Cls_1401e7b10* expected, int row, const String& deliveredText);
```

It returns only admitted/completed status; it never returns a native control or array pointer. The expected control address remains the current staged identity token, compared without dereference; it is not a durable or opaque identity.

## Required order and behavior

Keep `frequencyTextEdited`’s outer binding, row, KnownC locale, ready-page/source model/remesh, and matching populated-text checks before the command. Move its existing canonical spelling/range validation into the model command, unchanged and before the geometry guard: empty is admitted; otherwise only canonical positive ASCII digits from 1 through `INT32_MAX` are admitted. This prevents direct callers of the public model method from using Qt's invalid-text-to-zero behavior as an unintended write route. A false result maps to the existing `UnsupportedSnapshot`.

Within the command, retain this sequence:

1. Validate row 0–5 and the canonical text profile before model access, matching the old callback's refusal-before-`simCtl()` behavior for invalid spelling.
2. Call `requireGeometrySyncAvailable()` once.
3. Resolve `step ? &step->sim_ctrl : nullptr`; refuse null or an address mismatch, including null current control with null expected identity.
4. Read `frqncy_cols` into `stride`, then keep the current short-circuit shape check order: `frqncy_rows == 6`, `stride == 1`, `frqncy.ncomp == 6`.
5. Call the existing `qt::toInt(deliveredText)` with ignored validity. Keep empty text conversion to integer zero.
6. Read the current `frqncy.values` pointer after conversion; refuse null without repairing/allocating storage.
7. Write only `values[row * stride] = static_cast<double>(converted)` and return success.

This preserves the callback’s unusual native order: the geometry/control/table checks precede conversion, while the values-pointer read follows conversion. Do not preconvert in the callback, do not fetch or retain `values`, do not add a widget-enabled/combo predicate, do not change repeated delivery, and do not catch conversion/guard exceptions. The Stopping/cache1 path is not involved.

## Files and tests

- `source/apps/moproj2mst/include/m2m/engine.hpp`: add the named declaration beside the other editor model commands. Keep the existing `String` alias already available through `m2m/model.hpp`.
- `source/apps/moproj2mst/src/engine/gui_model.cpp`: implement the command; include `m2m/qt4.hpp` directly if needed for `qt::toInt`. Return status rather than a pointer.
- `source/apps/moproj2mst/src/engine/simcontrol300_owner.cpp`: keep event/binding/cache checks; replace the local lexical validation and all direct control/table reads/writes with one call, so validation still occurs immediately before the old `simCtl()` point.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_frequency_edit.cpp`: add direct-command cases for matching control, live/null expected mismatch, absent current step, invalid row and invalid/overrange text refusal, shape mismatch, empty-to-positive-zero conversion, and one-row-only writes. Existing owner-event tests already cover the six rows, int32 maximum, empty/null distinction, cached text, repeated same-text delivery, wrong locale/bindings, stale/missing control, unsupported table shapes, and values-pointer refusal; retain them.
- Root’s focused regression group should include `m2m.mdt.simcontrol300_frequency_edit`, `m2m.mdt.viewer_box` (parked geometry refusal), and `m2m.mdt.simcontrol300_stopping_cached`. The command’s single guard before control lookup must remain testable in the parked-viewer fixture with valid row/text and a null expected token, using the established `refused` helper.

This requires edits to shared `engine.hpp` and `gui_model.cpp`; coordinate those hunks with their current source owner at implementation time. No shared files were edited in this read-only design task.

## Evidence and limits

The reconstructed-source contract is directly visible in `simcontrol300_owner.cpp::frequencyTextEdited` and the existing cases in `tests/mdt/test_simcontrol300_frequency_edit.cpp`. CP137’s implementation/review notes (`analysis/resume137-editor-frequency-edit.md`, `analysis/resume137-editor-frequency-review.md`) retain the original DEF_GUI_MO_64 v11.0.2 SHA-256, the six slot-map handler addresses (0x140ecfeb0 through 0x140ed0130), per-handler source spans/hashes, and the exact `QString::toInt(ok=null, base10)` ordering. They also cite QtCore4_64.dll 4.8.4 and its SHA for the empty-string conversion qualification. Those retained materials support the bounded source/native comparison; they do not establish a fresh original execution, native widget event delivery, Qt locale/provider side effects, ownership/refcount effects, or complete editor behavior.

No source, test, build, shared recovery state, original process, GUI, or REA state was changed for this report.
