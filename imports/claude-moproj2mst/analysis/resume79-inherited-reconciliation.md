# CP79 — inherited queue reconciliation: PPV continuation and Report path

## Disposition

The inherited RINHERIT queue should distinguish completed helper work from lifecycle behavior that is still guarded. PPV is no longer wholly unsupported: bounded reader/writer helpers are connected to public Open/Save paths, and the exact KEY-failure continuation has a candidate regression test. Report's .DS derivation is implemented and tested as a pure helper, but Report Open still stops before the document owner is created or adopted. Neither item is fully closed as original-equivalent lifecycle behavior.

## PPV: helper integration and remaining boundary

analysis/resume47-inherited-next.md correctly identified SimCtrl.PPV as a reachable inherited recovery-list omission at the time. Later work integrated the bounded profile:

- loader.cpp loadSimCtrl checks for the operation's SimCtrl.PPV after the KEY path. If present it calls readPpvOpenSequenceFromDisk; UnsupportedInput and OwnerUnavailable retain an explicit unsupported path. Other reader results are ignored, consistent with the qualified native caller's ignored parser return and subsequent count update.
- save_current.cpp saveSimCtrl attempts KEY first, then runs the PPV writer for nonempty local recovery-B, while returning the KEY result and ignoring PPV open/post-open writer status. The local-empty/root-only gate is preserved.
- The parser, file decoder, Open sequence, Save operation adapter and text sink have independent focused tests. Public lifecycle tests cover supported local Open-to-Save, root-only no-write outcome, PPV Save-open failure, and KEY-open failure followed by PPV rewrite.

The CP68 case native_simctrl_key_flush_failure_still_attempts_local_ppv is also present in tests/s/test_save_io.cpp. It checks that a successful KEY open followed by the injected flush failure still reaches the PPV path and retains the KEY writer result; it avoids asserting uncertain KEY residue. Thus the earlier CP66/67 KEY-open/flush continuation proposal is already implemented as a candidate test. This review did not run it; do not represent it as independently validated or original-runtime parity.

Remaining PPV boundary: Open admits only a source-qualified byte/codec profile and continues to refuse unsupported decode/owner states. Public Open/Save parity for other locale encodings and malformed input is not established. Existing unit tests cover parser partial-row mutation/count update and Open-failed count behavior, but the reviewed public lifecycle suite does not exercise a malformed PPV row through public Open-to-Save. This is now the smallest executable RINHERIT acceptance increment: extend the disposable public PPV lifecycle fixture with one valid local row followed by a malformed row; require Open/Save to continue, and assert the first row survives and is normalized on Save. This checks the already-integrated ignored-reader-result boundary without changing production behavior or relaxing decode/owner guards. Keep the fixture scratch-only and leave original project inputs untouched.

## Report: path helper versus public owner gap

reportDsPath(Project.path) is implemented in src/ops/report_ds_path.cpp and has focused tests in tests/s/test_report_ds_path.cpp for normal, multi-dot filename, and dotted-parent cases. Repository-wide call-site search finds no production consumer beyond its definition/declaration: it is not connected to Report Open, Save, or Close.

The public gap remains concrete. ReportClass::loadOperation reaches loadClassData; loadClassData refuses at the Report document read (SP+0x30, 0x1400b5ad0 / 0x140643100) before any .DS read. The corresponding Save branch retains the document/viewer guard. resume66-report-owner.md and resume71-report-owner-edge.md establish that the candidate has no adopted SP+0x30 owner and that the first native transition occurs inside a main-window-parented QObject constructor. Current retained evidence does not close its shared QObject/TLS/QThreadData/parent-child effects or the Close release order. The pure .DS helper therefore settles only path derivation; it is not a safe partial Report lifecycle integration.

No production Report edit is presently source-closed. Reopening that lane requires the actual parented QObject owner and matching SubProject/Project Close-release contract; wiring only the path helper would cross the existing unsupported boundary without a valid document object.

## Next bounded task

Add the public malformed-row PPV Open-to-Save regression described above in the existing PPV lifecycle test file, then let root validate it in the normal frozen build. It is bounded to already-integrated public behavior, gives the inherited PPV queue a meaningful end-to-end acceptance case, and leaves the unresolved Report owner guarded.

## Scope

Read-only source/test review plus this analysis note. No source or test edits, build/CTest, GUI control, original execution, or mutable REA action was performed.

