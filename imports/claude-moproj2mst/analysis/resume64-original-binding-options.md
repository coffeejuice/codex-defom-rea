# CP64 original-reference binding options

## Conclusion

The retained material offers no positive, session-bound original reference for either proposed comparison target. A55's `MoInputMode` remains unbound, and PPV has qualified static reader/writer evidence plus candidate-only model tests, but no retained original PPV Save receipt. Neither gap can be repaired by treating a nearby generic settings backup, static decompile, or synthetic candidate test as an original runtime result.

## A55 settings mode

`evidence/gui_mo/oracle_b/_tools/settings_backup_l5s/DEFORM-MO.ini` records `MoInputMode=1` (hash `909d618a352e7f1c972a53b887aa5978e933d875ff4b260a8dd107fa4797b4af`). The A55 `rs_log.tsv`/`rs_run.out` entries document the interaction/close but do not identify the launched process's settings path/hash/value or a mode-toggle event. The backup is not named for A55 and contains no capture/session binding. Its timestamp precedes the run log, which is not proof it was the INI read by that process or that no action changed the mode. The A55 project itself does not store this global preference. See the detailed qualification in `analysis/resume63-a55-mode-qualification.md`.

**Minimum fresh original receipt:** before launching a pristine A55 copy, record the actual INI path and hash and the parsed `[Setting] MoInputMode`; after launch, record any mode setter/signal events before Open and the mode at first editor selection. Preserve the raw saved `Task00002/SimCtrl.KEY` and its hash. Also record current tree operation and whether the CG radio transitions false→true during editor300 fill; otherwise the two differing fields cannot be attributed to the hypothesized callback.

## PPV original reference

Retained PPV artifacts are source/decompile evidence, not a runtime oracle. `analysis/resume48-ppv-reader-receipt.txt` is a bounded static decompile of original reader `FUN_140d4b400`; `analysis/resume48-ppv-writer-receipt.txt` is a bounded static decompile of the original writer `FUN_140d4afe0`. The retained search of `evidence/gui_mo` found no `SimCtrl.PPV` fixture/output, and Oracle-B run manifests/logs do not record PPV bytes or a PPV round trip. Later CP57/63 records explicitly qualify PPV coverage as synthetic candidate contracts and state that fresh original PPV runtime was not executed. The candidate public loader/Save path remains guarded, so those candidate tests cannot stand in for original output.

**Minimum fresh original receipt:** use one disposable copy of a retained project with both recovery lists populated and a deliberately small, documented `SimCtrl.PPV` containing at least one valid row. Record input file hashes and full paths, original executable identity, settings/profile identity, Open result, parsed points and shared-owner state, then Save and capture exact `SimCtrl.PPV` and `SimCtrl.KEY` bytes/hashes. Add one absent-file/empty-list control to distinguish default behavior, and one malformed-row case only if the valid round trip completes. Keep each project and generated path under `C:\PROBLEM` and shorter than 100 characters. Preserve raw output before any interpretation; record process exit/modal outcome and verify no source fixture was modified.

## Qualification boundary

These are the smallest evidence-producing original comparisons, not authorization to acquire GUI control. The current user instruction requires a verifiable supported app-control release receipt before another GUI-control session. No GUI/app action, build, source edit, mutable REA operation, or original launch was performed for this audit. Until that gate and the above receipts exist, keep both the A55 mismatch and PPV runtime behavior unresolved at the original-reference layer.
