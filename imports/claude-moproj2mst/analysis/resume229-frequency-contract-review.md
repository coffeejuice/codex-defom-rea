# CP229 frequency owner-command contract review

**PASS — the proposed leaf preserves the current callback contract, subject to the explicit failure and custom-result qualifications below.** This is an independent source/design review, not an implementation or runtime validation.

Reviewed `resume229-editor-next-leaf.md`, `simcontrol300_owner.cpp::frequencyIndexChanged` (lines 401–427), the complete `test_simcontrol300_frequency_index.cpp`, `gui_model.cpp::simCtl`/`requireGeometrySyncAvailable` (lines 208–218), and the CP222 alias/closure qualifications. The contract here is the current reconstructed source; no new claim about original-binary validation follows from this review.

## Preserved event and command order

The caller must retain its existing short-circuit guards in their current order: binding identity (including the OPI lifetime guard), row 0–5, selection 0–2 for row 0 or 0–1 otherwise, constructor state `AwaitingNativeBoundaries`, complete pages, source model/remesh identity, and the cached combo equal to the delivered selection. In particular, validate row before indexing the cached combo. A repeated delivery of the same index remains an event and must repeat its model store/widget projection; do not add change detection.

The command may repeat row/selection admission before calling the geometry barrier. It must then perform exactly one barrier check, resolve `step ? &step->sim_ctrl : nullptr`, refuse null or a different control, and read/check `frqncy_cols == 1`. The expected pointer is only the legacy equality token; never dereference it. Do not add `noexcept`, swallow the barrier exception, or perform an early expected-null check before the barrier.

After successful identity/stride admission, custom means row 0 selection 2 or rows 1–5 selection 1. It succeeds without consulting rows, component count, or values storage and performs no model write. Only after command success does the caller set that row's input-enabled optional to `true`.

For every non-custom choice, check `frqncy_rows == 6`, then `frqncy.ncomp == 6`, then non-null `frqncy.values`. Write exactly `values[row * stride]`: `-1.0` only for row 0 selection 1, positive `0.0` otherwise. Complete that store before assigning input-enabled `false`. Leave combo, cached text, all other doubles and unrelated model/I/O state unchanged. Failure must preserve an unknown optional enabled bit as unknown, rather than assigning `false`.

## Clarifications needed in implementation

1. “Map failure to UnsupportedSnapshot” applies to a normal `false` command return. A parked geometry barrier currently throws `Error(M2M_E_UNSUPPORTED, ...)` from `simCtl()` even when the current control or expected token is null/mismatched. Preserve this exception and its precedence; do not translate it to `UnsupportedSnapshot`. Invalid event input still refuses before reaching that barrier.
2. A boolean result is sufficient if `true` means **admitted/completed**, including the custom branch that wrote no value. The caller already knows row/selection and can derive the enabled bit. No distinct custom enum is necessary. A result meaning only “double was stored” is insufficient: custom must be successful so that its widget is enabled. If a richer result is chosen, it must represent successful custom explicitly.
3. Null current control must fail even when `expected == nullptr`; do not use equality alone. A non-null current control with null expected must also fail. Resolve and compare before any stride/table access, so mismatched control refuses even if the replacement table would otherwise be malformed. Raw address equality is the present staged behavior; this leaf does not supply CP222's future generation identity or ownership closure.

## Regression implications

Existing tests cover six-row automatic/custom semantics, exact row-zero sentinel/positive-zero bits, preservation of non-addressed double bits and cached text, repeated delivery, stale/missing current control, bad stride for every choice, and custom success despite each malformed table condition. They do not directly establish barrier precedence or direct command admission because the command does not yet exist.

Add direct-command cases for invalid row/selection before a parked barrier, valid choices through a parked barrier (including null/mismatched expected tokens and missing current step), null expected with a live control, both current and expected null, mismatched expected control, all custom selections with absent/malformed storage, and non-custom one-cell writes. Keep caller checks/projections covered separately. Do not call `pageState()` after parking merely to inspect effects: that accessor itself calls `simCtl()`; retain the already-obtained page reference instead. Restore mutated fixture table pointers before fixture destruction as the existing test does.

The existing test uses a Testkit `main`, supported `CHECK`/`CHECK_EQ`/`REQUIRE`, and no C `assert`; CMake discovers `tests/*/test_*.cpp` and links the ordinary target to `m2m_core`/`testkit`. No new test target is required when extending this file. Keep the Stopping cached regression in root's focused integration group. No source, tests, build artifacts, shared recovery state, originals, GUI, or REA state were changed by this review; only this report was written.
