# CP233 Save/lifecycle next leaf assessment

## Decision

No new minimal Save raw-step/raw-object alias migration is justified by the current source and test seam. The immediately executable residual Save issue is a witness blocker: completed Open → Save → Close behavior still lacks a direct fresh-original lifecycle witness, and the current Save folder sweep is explicitly diagnostic rather than acceptance evidence.

## Checkpoint context

`analysis/takeover-state.json` identifies CP231 as frozen with 369 bound tests and records Save, accepted Close, and all 15 workstreams/eight gates as open. `analysis/wave3-checkpoint231-validation.json` reports 362 clean passes, six logged gaps, one logged difference, and no failed/incomplete/skipped cases. These are reconstructed test results; they do not supply original lifecycle evidence.

## Save path review

The most compact-looking Save helpers still own externally visible sequencing, so extracting them as model operations would not be a behavior-neutral alias migration:

- `source/apps/moproj2mst/src/engine/save_current.cpp::captureKey` accepts a callback that writes a scratch file, then conditionally reads/removes it and appends `FileEffects`. Moving native serialization behind the model would alter callback/output ownership and can change whether effects are recorded after open/write failure.
- `writeTaskKey` branches on `nativeTaskKeys`; in native mode it calls `beforeNativeKeyWrite`, opens/truncates through `CoggingFile`, returns the configured failed-open value if opening fails, erases the overlay only after successful open, streams bytes, then closes. This ordering is a precise failure contract and a native writer callee remains the serialization authority.
- `saveInterObject` first returns false for a missing step; otherwise it can rebuild contacts and update contact flags before deriving the path and attempting the native writer. Existing tests cover this order and exact bytes. A copied value operation cannot replace the borrowed step needed by the serializer.

These dependencies are documented in `analysis/resume222-save-alias-ledger.md` and directly visible in current source. Do not infer that a generic pointer/callback elimination is safe.

## Executable increment and focused seam

The best bounded next step is evidence acquisition, not a source edit: obtain a fresh owned original run of the selected lifecycle `open A → save A → close A`, recording completion/accepted-close outcome plus the resulting project/key/study files and cleanup. Compare it to the reconstructed run without treating `s.save_folder` diagnostics as a pass criterion. Existing reconstructed test seams include `tests/s/test_save_current.cpp` (`save_interobject_step_scope` and exact 631-byte interobject-key check) and `tests/s/test_save_io.cpp` native byte/order/denial cases, but neither substitutes for the missing original lifecycle witness.

If a source migration is required before that witness exists, the exact blocker is lack of a qualified one-value Save consumer: every located candidate either mutates/interrogates multiple fields, hands the step to an external native writer, or couples it to ordered file effects. A proposed new API would need native callee evidence showing which owned input fully replaces the step alias and a focused test that proves byte identity plus the original failure prefix. No such candidate/test pair is currently established.

## Limits

This was a read-only source/evidence review. No source, tests, build, lifecycle runtime, original app, GUI, or REA session was changed or exercised. The report does not claim CP231 tests are fresh-original acceptance evidence.
