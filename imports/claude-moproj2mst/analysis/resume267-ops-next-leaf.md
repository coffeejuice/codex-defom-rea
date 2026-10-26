# CP267 registered operation leaves outside the audited lanes

## Result

No smallest independently implementable non-editor operation leaf is currently promotable from retained native evidence plus an actual-caller test. CP263 already reconciled the operation-dispatch gaps; CP266 adds no new Open/PostLoad/Save caller occurrence that closes them. The exact blockers remain occurrence/owner binding, not absence of candidate code. Preserve current unsupported guards.

This is a read-only audit. Only this report is written; no source, tests, fixtures, shared state, builds, GUI, original process, or mutable REA state was changed or used.

## Scope and checkpoint reconciliation

Excluded per task: Editor300, active DOE work, Boolean operation work, and current scene camera. CP263 (`analysis/resume263-operation-dispatch-next-leaf.md`) examined the reachable registered operation candidates and found no promotion. CP266's DOE state, Boolean timing, and Close-owner notes remain separate lanes; they do not add an operation caller witness. Repeating completed Boolean postLoad/Save dispatch or DOE Group 6 work would duplicate the earlier checkpoints.

## Remaining reachable candidates and exact blockers

| Candidate and source | Retained native basis | Why no actual-caller test can yet promote it |
| --- | --- | --- |
| Cogging Save relative `MAN_COUPLE.DAT` lookup: `source/apps/moproj2mst/src/engine/cogging_save.cpp` (native Save path `0x140762f30`; event pump `0x140098129`) | CP256/CP263 static Save evidence; candidate explicitly distinguishes project-folder resolution from the native CWD-relative `QFile::exists` at line ~580. | Need one fresh original Save interval joining event delivery/handler, each CWD change, project and active-operation identities, and CWD plus relative-file existence at the exact Cogging lookup. `SaveEntryEventPrefix` in `src/engine/save_event_prefix.cpp` only tests an injected schedule; it cannot establish Qt queue delivery or the resulting CWD/operation. Actual-caller regression must exercise the same Save occurrence and assert the lookup outcome and following effect.
| DoeOutput Save slot: `source/apps/moproj2mst/src/engine/save_current.cpp` (native slot `0x140e2e730`, dispatch `0x140052ec0`) | CP117/127 dispatch mapping; current guard at lines ~779–791 requires actual active Type2 Study owner and excludes unresolved warning/modal or Control/Regions enumeration paths. | Need one same-occurrence Type2 Save binding the actual notifier key/map/list and live table/timer receiver through callback/dispatcher normal return, plus selected DoeOutput identity and DBLIST process-global `+0x1c` admission/lifetime. A fabricated callback-success bit or `.DS` helper test is not an actual caller witness.
| DOE TLOC group 6 / Add Study: `source/apps/moproj2mst/src/doe/doe_study.cpp::keywordValues` | CP256 static review establishes no production caller for this helper; original group-6 edge `0x140916530` retains unresolved status storage. | Require ordered original Add Study → actual TLOC variable-edit caller → group-6 handler occurrence, with input/status edge, output pointers, caller mutations, and later Save output bound together. The helper is currently test-only; guessed status initialization cannot establish reachable dispatch.
| Other operation-specific loader paths (notably MultiBlow postLoad and FormingExpress): `source/apps/moproj2mst/src/engine/l5_slots.cpp` / class operation sources | CP257/CP263 static reachability notes. | MultiBlow lacks actual missing-object/slot-creation Open evidence; FormingExpress lacks a retained actual Open case. Candidate model-prefix tests do not establish native caller admission or failure prefix.
| RFORMAT external conversion boundary | CP256 format audit. | First identify and hash-bind the actual `DEF_CVT3` artifact, then recover its invocation, path/encoding/grammar and failure sequence. DEF_PRE command strings do not prove the converter's contract.

The cited call edges establish possible reachability, not that the required owner, branch outcome, or callback occurred in one native operation interval. Current scene radius/box and selection work are explicitly outside this audit; Close/destruction and property paths are not substitutes for an operation-specific Open/PostLoad/Save leaf.

## Recommended next action

Wait for a retained, occurrence-correlated native witness that closes one row above. Of the listed candidates, the Cogging lookup has the smallest potential downstream surface, but its deciding input is native CWD/event ordering; no resolver-only production change or synthetic schedule test is justified before that witness. If evidence arrives, implement only the exact guarded leaf in its listed source and add a regression through the actual Save caller that asserts the native branch result and subsequent effect ordering.

## Evidence consulted

- `analysis/resume263-operation-dispatch-next-leaf.md` — prior reconciliation of Cogging, DoeOutput, DOE group 6, format and loader candidates.
- `analysis/resume266-doe-state-next-leaf.md`, `analysis/resume266-boolean-timing-next-leaf.md`, and `analysis/resume266-close-owner-next-leaf.md` — recent checkpoints that do not close operation caller evidence.
- `source/apps/moproj2mst/src/engine/cogging_save.cpp`, `save_event_prefix.cpp`, `save_current.cpp`, `src/doe/doe_study.cpp`, and `src/engine/l5_slots.cpp` — current candidate guards/caller seams.

