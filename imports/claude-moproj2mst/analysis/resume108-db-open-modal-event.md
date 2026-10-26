# CP108 Open split-DB modal event boundary

## Static Open order

Retained original PE evidence orders the split-DB question early in File > Open. `0x14008a9b0` prepares project path/state and opens the data-root DB (`0x140cdcdc0` -> `0x140cdc400` -> `DBH::open 0x140cdc480`) before it walks the `.moproj` XML. When the DB reopens and `has_previous_part` (`0x140814e80`) is true, the question is “2G Split DBs,” buttons Yes|Default and No|Escape. The initialized answer is Yes. Yes calls `setup_part_chain` (`0x1408150f0`); after that normal return, license/cache handling completes and the loaded flag is set. The project XML read and post-open/active-subproject work follow.

The retained timer contract traces the successful normal Open route beyond this point: after XML loading and subproject activation (`0x1400a7250`), native view binding (`0x140038380` -> view `vt+0x1b8`, `0x140170430`) attaches the nonnull operation model and requests `QTimer::singleShot(1000, receiver, modelUpdated)`; the selected active-project route has two such registrations. Therefore these **later Open-created active-project timers** do not exist yet at the earlier split-DB question and cannot be among callbacks delivered during that question. This ordering is supported by `wave3-boolean-open-sequence-contract.json` and `wave3-boolean-timer-contract.json`.

The modal itself is still a real nested event-dispatch opportunity. Static Qt evidence for `QDialog::exec` shows `QEventLoop::exec(0x40)` repeatedly dispatching through the current-thread dispatcher (including waiting); using the default Yes answer does not short-circuit that loop or prove an empty event set. The Open timer contracts do **not** prove that the global dispatcher has no startup, preexisting, or unrelated queued work at this point. No exact timer/event ID, receiver callback, or elapsed time inside this `QMessageBox::question` was observed.

## Close consequences

The synchronous File > Open call chain does not directly invoke the normal Close root. On the default Yes route it continues through DB chain/cache completion and then project reading. Normal Close is a separate caller (`0x1400299a0`) with its own busy/can-close question and possible nested Save. The default Yes result for the split-DB question says only “load all parts”; it is not a Close decision and cannot stand in for the Close answer or modal schedule.

Because the DB question enters a nested event loop, retained static evidence cannot rule out unrelated/reentrant actions such as a window Close request delivered during that interval. It also does not establish that such a request occurs. Consequently, no accepted-Close callback, teardown, or Close-time Save effect can be attributed to the default-Yes Open branch from static source alone.

## Candidate relation and remaining witness

CP103's `dbOpen` test and CP105's new caller-level `openProject` test exercise the candidate's synchronous default-warning/chain/cache behavior. They do not display a dialog or prove original nested-loop delivery. The synthetic DB pair is format-valid but is not a captured 2-GB-split original project. The CP105 test also stops before subproject activation, Save, and Close, which correctly avoids claiming later callbacks.

The smallest missing runtime witness is one owned fresh original process, first project Open, with a valid split-DB input and passive event observation from immediately before `0x140cdc480` through its Yes return. Record: existing dispatcher/timer registrations and receiver identities at entry; every timer and relevant reentrant window callback delivered inside this modal interval (including an observed empty list); actual button/result; DB part chain/cache/load result; then whether Open completes and whether any Close root was entered before control returns. This one witness distinguishes the source-predicted later Open timers from any preexisting work. It does not need to run Save or Close unless a Close action is actually delivered; if one is, preserve that separate nested sequence. No new GUI session is authorized or used in this read-only lane.

## Evidence bindings

- `source/docs/re/moproj2mst/mdt/DB/spec.md` §§1.3–1.5 and §8: native DB open/dialog/caller order and split predicate.
- `analysis/wave3-boolean-open-sequence-contract.json`: retained fresh Open chain; DB/path Open precedes project walk, while subproject activation and view binding follow it. Its selected callgraph bounds are not whole-program negative proof.
- `analysis/wave3-boolean-timer-contract.json`: timer registration and callback receiver/current-model behavior; explicitly no original runtime timing or readiness observation.
- `analysis/wave3-directory-failure-dialog-contract.json`: static Qt nested-dialog event-loop capability; this is a Qt modal-loop contract, not a run of the DB question.
- `analysis/wave3-normal-close-guard-contract.json`, `analysis/resume60-close-schedule-next.md`: independent Close caller and unresolved modal timer/event partition.
- `analysis/resume103-integrated-grouped-repair4-tests.json`: CP103 direct split-DB candidate target passed in a grouped suite; not original GUI acceptance.
- `analysis/resume105-open-split-db-test-receipt.md`: CP105 caller-level candidate test scope and synthetic fixture qualification; not a test execution receipt.

Read-only evidence reconciliation. No code/build, GUI/original execution, or mutable REA database operation was performed.
