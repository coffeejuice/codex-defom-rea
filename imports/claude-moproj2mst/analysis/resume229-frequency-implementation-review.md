# CP229 frequency implementation review

**PASS — the bounded frequency command extraction preserves the reviewed source contract. No implementation defect found.** This is an independent source/test review, not a new original-behavior validation or a build/test receipt. Root owns integration, the complete suite and source freeze.

Reviewed `analysis/resume229-editor-next-leaf.md`, `analysis/resume229-frequency-contract-review.md`, `analysis/resume229-editor-frequency-command-implementation.md`, the CP222 alias/closure qualifications, and the working diff/current bodies in the four assigned files. Accumulated unrelated CP225–CP228 changes in the shared files were distinguished from this frequency leaf.

## Preserved behavior

- `simcontrol300_owner.cpp:401` retains binding identity (including OPI lifetime), row/selection ranges, constructor state, page completeness, source model/remesh identity and cached combo equality in the same short-circuit order. Row validation precedes indexing. Normal command failure returns `UnsupportedSnapshot` before changing the enabled optional; unknown remains unknown. Repeated same-index events still execute.
- `gui_model.cpp:234` validates row/selection before exactly one `requireGeometrySyncAvailable()` call. It neither catches the barrier exception nor declares `noexcept`. Thus a valid delivery reaches the parked-barrier exception before nullable step lookup, expected-token comparison or table access, including null/mismatched expected tokens. Invalid direct arguments return false first. This ordering is established by source inspection here.
- Current control is resolved as `step ? &step->sim_ctrl : nullptr`, then explicitly checked for null and equality. Expected is used only as a pointer identity token, never dereferenced. Both-null and live-current/null-expected refuse. This preserves staged raw-address identity; it supplies neither generation identity nor ownership closure.
- Stride must equal one before the custom branch for every valid row/selection. Custom succeeds without evaluating rows, component count or values storage, and performs no table write. Non-custom checks rows, components and storage in order, then stores exactly one double. Row zero selection one stores `-1.0`; all other non-custom selections store positive `0.0`.
- The command returns before the caller updates input-enabled state. Therefore the single non-custom model store still precedes widget disable; custom enables only on success. Combo and text caches are untouched.

## Test strength and limits

The added direct-command case tests null expected with a live control, a different live control, both-null after removing the current step, invalid row/selection, stride refusal on custom, individual malformed table fields with custom success/non-custom refusal, unchanged bits after failures/custom, and exact one-cell `-1.0`/positive-zero writes. Existing caller cases cover every row and selection for missing/replaced control and stride refusal, all rows for each malformed table field, unknown-enabled preservation, cached text, invalid/cache-mismatched delivery, unconstructed/partial/expired OPI, repeated delivery and no unrelated I/O effects. Pointer/table fixture mutations are restored before destruction. The test retains its Testkit entry point and supported assertions; no C `assert` or unsupported macro was introduced. Existing CMake discovery/linkage remains applicable.

The added test does not dynamically exercise a parked geometry barrier, and its direct invalid-argument sample is narrower than the full caller range matrix. These are coverage limits, not observed implementation defects: the source order is explicit and the existing caller matrix exercises the extracted command. An existing general barrier regression alone cannot prove dynamic precedence in this new command/caller; any integration report must retain that distinction unless frequency-specific parked cases are run. No build or test was executed by this reviewer. The implementation owner's reported 1/1 focused pass is not re-certified here.

## Scope and source identity

No C ABI file, `simcontrol300.cpp`, or `test_simcontrol300_stopping_cached.cpp` is changed by this leaf; the latter two also have no working diff. The frequency path introduces no Stopping continuation, cache1 access, coordinate query or publication. Retain the stopping-cached target in root's integration group. No whole-application parity or CP222 closure claim follows.

Reviewed SHA-256 values match the implementation report:

- `engine.hpp`: `C1CD02921A03F96394892DA266A2463CFC265149B3597442F4D3EACDC0F25DFD`
- `gui_model.cpp`: `B3460A4BF2F3874688FBE43326AE87CDB325C60CA35EE09A7B85AD26CC1DF002`
- `simcontrol300_owner.cpp`: `15CA48461D6F48B2F2CBD085C971E45AAC43C076CAC4C182EECD0133B373FEB6`
- `test_simcontrol300_frequency_index.cpp`: `197CA0CF660A360A9198EB4AD100E9B12D5FBF61D94D33BA64BA48ACDFFF1A8F`

Only this report was written. Source, tests, builds, shared recovery state, originals, GUI and REA state were not changed.

## Supplemental parked-barrier assertion review

Root added a frequency-specific assertion at `test_viewer_box.cpp:2843` inside `geometry_low_query_model_barrier_stops_next_view_retries_and_owner_bypasses`. It calls the command on the mutable parked `model` with valid row-zero/custom selection and null expected token. The local `refused` helper requires an `Error` with `M2M_E_UNSUPPORTED`, so a normal false return fails the assertion. This is meaningful dynamic coverage of barrier precedence over expected-null rejection and custom success, once root runs the source-stable test. The initial draft called the non-const command through `const pendingModel`; review caught that compile defect and root corrected the receiver to `model`. The corrected assertion has no remaining source-level defect.

The test does not supply invalid direct arguments, missing current step or mismatched non-null tokens while parked, nor exercise the editor caller under a barrier. Those broader cases remain supported by source order rather than dynamic coverage. The earlier coverage-limit paragraph describes the original four-file implementation test; this supplement closes its parked-command/null-token gap at source level without claiming execution. Source-stable focused validation and the full freeze remain owned by root.

Supplemental file SHA-256: `B9AA543ECFE58B3D22FCA90A62DD4A7BF462986C38CE2B02A5FDB303972CC937`. Overall result remains **PASS** after correction, with no unresolved defect found. This reviewer edited only this report.

Root subsequently reported successful focused build `cp229-viewer-barrier-fixed-focus` and three focused CTest targets passing (10 + 13 + 56 cases). That is root-provided integration evidence, not a test execution by this reviewer. The repaired frequency/barrier assertion is included in the reviewed source above; complete-suite validation and freeze remain separate root gates.
