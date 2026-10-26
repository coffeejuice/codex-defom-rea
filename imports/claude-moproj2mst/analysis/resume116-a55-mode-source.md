# CP116 — A55 mode and CG-toggle source audit

## Finding

The retained evidence does not resolve the original A55 process's `MoInputMode` or prove that its CG toggle ran. The two changed bytes are consistent with the retained original CG slot, but that consistency is conditional. Keep the historical difference as unresolved; do not label it as either an editor300 fix or a proven mode mismatch.

## What the original receipts establish

- The raw pair is exact and narrow: `Task00002/SimCtrl.KEY` is 2,146 bytes on both sides and differs only at offsets 1,439 and 1,455 (`0` candidate, `1` retained original Save). The row follows `SOLMTD 2 4`, so its tuples are `[0,0,0,0]` and `[0,1,0,1]`. The retained original and candidate hashes are respectively `79f9ec098a22b46f60e23072751a23fa036459f4db02f719e56e6421f243f79b` and `2bee048e6645fd36416d1b7582883c65a9727e5756715302a928ca3f1656854e` (`wave2-save-diffs-reference-inspection.json`, `wave2-save-diffs-rs55-capture.json`).
- `evidence/gui_mo/oracle_b/_tools/rs_run.out` says the A55 case completed Save and Close in its runner. The retained runner action is fresh-process Open→Save→Close, but the log has no PID-bound INI, mode value, mode-change event, active tree operation, editor identity, widget state, or callback record. `original_process_executed: false` in the retained candidate capture confirms that the byte-level candidate run did not supply a paired original execution trace.
- The available generic settings backup has `[Setting] MoInputMode=1`, but no capture metadata binds it to this process. The A55 `.moproj` does not carry the global mode. The candidate folder sweep passed no `OriginalSettings`, so its default mode was 0. This establishes a candidate-side mode input, not the original input.

## Static original decision path

For the retained original binary (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`), factory `0x1405ff0b0` chooses editor100 for mode 0 and editor300 for nonzero mode. In the editor300 path, iteration fill `0x140ec19f0` reads the current solver; on solver 2 it calls `setChecked(true)` on the CG radio (`0x140ec21b6`). Setup calls `connectSlotsByName` at `0x140f21d67`, and metadata method 64 routes `toggled(bool)` to `on_m_rbtnDefSolCG_toggled(bool)` / `0x140ed01b0`.

That slot accounts for the two stores if it is delivered with `true` and the relevant parameter conditions hold: S2 at `SC+0x2f8` becomes 1 when truncated S2 is `<=0`; S4 at `SC+0x308` becomes 1 unless truncated S4 is 1 or 5. Thus the input four zeros become `[0,1,0,1]`, with S1 and S3 unchanged. This source chain establishes a possible cause, not A55's actual route. The evidence does not bind the factory mode, first active operation, editor setup/fill completion, or the radio's unchecked→checked transition and slot delivery. `setChecked(true)` alone does not prove a signal if the radio is already checked.

The static material is `wave2-save-diffs-original-iteration-fill.asm`, `wave2-save-diffs-original-meta-dispatch.asm`, `wave2-save-diffs-original-slot-identity.json`, and `wave2-save-diffs-diagnosis.json`. These are PE/decompile receipts, not a runtime trace. The generic settings file and historical screenshot do not close those gaps.

## Smallest missing observation

One fresh original Open→Save on an immutable A55 copy needs a passive, process-bound record of: (1) the actual INI path/hash and `MoInputMode` at startup plus any mode setter before Open completes; (2) the first active operation and its Task00002 `SOLMTD`/count/parameter values; and (3) CG checked state immediately before and after initial iteration fill, whether method 64 is delivered, and resulting model values. Retain raw Task00001 and Task00002 KEY hashes after Save. This distinguishes mode 0 from editor300, and distinguishes `setChecked(true)` from a delivered unchecked→checked callback. Without this witness, the raw pair cannot answer whether original A55's bytes came from fresh Open or session history.

No GUI control, mutable REA database, source edit, build, or test was performed for this audit.
