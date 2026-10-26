# CP232 frequency-text owner-command contract review

Result: **PASS for the CP231 extraction contract**, with the qualifications below. This is a static review of the proposed command against the existing callback, not acceptance of an implementation or a new test run. At review time `frequencyTextEdited` still contains its lexical gate and direct control/table access; the new command is not yet present in `GuiModel`.

Reviewed `AGENTS.md`, `TAKEOVER.md`, takeover/import summaries, `analysis/resume231-editor-next-leaf.md`, CP137 frequency implementation/review/proposal qualifications, and the current `simcontrol300_owner.cpp`, `gui_model.cpp`, `engine.hpp`, `model.hpp`, Qt adapter and frequency tests.

## Required contract

The proposed `bool GuiModel::applyEditorFrequencyTextIfControlMatches(const def::model::Cls_1401e7b10* expected, int row, const String& deliveredText)` is an appropriate value-command boundary. `m2m::String` already aliases `qt::String` through `model.hpp`. Return admission/completion status only; do not return control/storage aliases or preconvert the text in the caller.

The owner must retain its present short-circuit preflight before calling the command: binding identity; row 0–5; KnownC locale; AwaitingNativeBoundaries state; present/complete pages; matching source model/remesh; then engaged cached text with content equal to the delivered text. Do not insert `pageState()`, which would introduce an earlier control query.

The command must perform these operations in this order:

1. Reject invalid row and excluded text before geometry/model access. Preserve the existing lexical/range gate: zero-length is admitted; otherwise at most ten UTF-16 units, first unit ASCII `1`–`9`, all units ASCII digits, magnitude at most `INT32_MAX`. This gate is bounded candidate admission, not recovered native validator semantics.
2. Call `requireGeometrySyncAvailable()` exactly once, then resolve `step ? &step->sim_ctrl : nullptr`. This reproduces `simCtl()`'s guard-before-lookup contract. Propagate guard/conversion exceptions.
3. Refuse null current control or mismatch with `expected`, comparing the expected address without dereferencing it. Null expected with null current control must refuse. This address token does not provide lifetime ownership, ABA prevention or general step-generation coverage.
4. Read `frqncy_cols` into `stride`, then preserve the short-circuit predicate order `frqncy_rows != 6 || stride != 1 || frqncy.ncomp != 6`.
5. Call the existing `qt::toInt(deliveredText)` with ignored validity. Only after conversion read the live `frqncy.values`; refuse null without allocation/repair.
6. Store only `values[row * stride] = static_cast<double>(converted)` and return true. The owner maps false to its existing UnsupportedSnapshot result and true to Completed.

The current adapter declaration defaults `ok` to null. `numberToCLocale` fails on zero-length content, and `toInt` consequently returns integer zero; the double cast produces positive zero. Canonical positive int32 values convert exactly to double. Keeping the real conversion inside the command preserves its placement after control/stride/shape checks and before the live values-pointer read. No enabled-widget/combo predicate, same-text shortcut, cache rewrite, Fill/Apply/Save call or unrelated row read belongs here.

## Qualifications and corrections

No blocking correction to CP231 is required. Preserve the following precision when implementing/reporting:

- An unengaged `optional<String>` cache is unknown. A null `String` and a nonnull empty `String` compare equal by content (`qt4.hpp`); do not add a nullness-sensitive equality requirement. Neither should be normalized or rewritten by the command.
- A missing/replaced current step must refuse without touching the old/replacement table. Existing tests retain the old allocation while constructing a replacement, so that concrete mismatch is meaningful; it is not proof against arbitrary address reuse.
- Native handlers retain their row-times-stride index before conversion. The existing reconstructed callback retains stride and evaluates `row * stride` at the final store. The extraction preserves that source-level contract; admitted row/stride are bounded and unchanged, so the numeric address agrees. Do not claim instruction-level equivalence or conversion-provider/reentrant side effects.
- This C++ internal method does not change `moproj2mst.h`, its API version 1 or the compatibility C ABI. Stopping/cache1 behavior is independent and must remain untouched.

## Validation boundary

Existing frequency tests cover all six rows, int32 maximum, positive-zero bits for null/empty text, preservation of other raw row bits and cache nullness, cache/Apply non-events, repeated explicit delivery with a noncustom combo, excluded spellings, locale/cache/binding refusal, missing/replaced control, shape/pointer refusal, and partial/expired owner gates. Keep them and add the direct-command cases proposed by CP231. The parked-viewer test must use valid row/text and null expected so it proves barrier precedence over control lookup; invalid input must refuse before that barrier. Root's focused group remains frequency_edit, viewer_box and stopping_cached.

Retained native comparison is qualified by CP137: original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, QtCore4_64.dll 4.8.4 SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, and six handler spans/hashes in `resume137-editor-next.md`. This review does not establish fresh original execution, real widget delivery/validator acceptance, QString lifetime/refcount behavior, QLocale/provider effects, complete editor recovery or public Save readiness.

Only this report was written. No source/test/build/shared recovery state, original process, GUI or REA state was changed; no build or tests were run.
