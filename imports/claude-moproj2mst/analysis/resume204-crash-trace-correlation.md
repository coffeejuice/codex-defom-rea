# CP204 crash trace correlation: dock and slot events

## Question and conclusion

This read-only correlation asks whether CP202's partial Frida trace can eliminate or rank the four CP204 conditional startup routes into `QListData::size()`: the CLI loader's owner-derived wrapper, the recent-path fixed-image wrapper, the notifier snapshot path reached from `postOpen` or selected setup, and the conditional pre-Open constructor chain. It cannot eliminate any of them. The observed Hide events and one visibility slot entry establish that some dock visibility work occurred during the instrumented process's short observed interval. They do not identify a `QListData::size()` caller, receiver, or route. At most, they give weak phase context consistent with startup having progressed into dock visibility handling before the trace ended.

No route ranking follows from event ordering alone. The four paths are conditional static instruction paths; none is bound to the trace PID by a captured caller frame or exception context. The route table's order in CP204 is a provenance/discriminator organization, explicitly not a likelihood ranking.

## Evidence identity and observation window

CP202 identifies PID 20712 as the original `DEF_GUI_MO_64.exe` on Sim2, with the expected original image identity (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, 87,420,928 bytes). The Frida message stream has 272 sequenced messages: module identity/readiness, observer readiness/arming, then dock events and one prepared visibility request/slot entry. Dock observations span 364–763 ms. Seven are labeled `Hide` (Qt event type 18).

At seq 185 (521 ms), the observer records a prepared visibility request at app RVA `0x1547e`, raw value 0, and `boolQualified: true`; seq 186 is a Hide for dock `0x3379390`, and seq 187 records slot entry ID 1 with value 0. Dock events continue afterward through seq 272. The trace report records no `busy_return`, `auto_save_call`, or `show_signal_call` messages. The Hide and slot are therefore concrete callback observations, but not a decoded Save, accepted Close, main-window show completion, or crash stack.

The capture terminated with `target_exited_or_detached` / `process-terminated`, `captureComplete: false`, `coverageComplete: false`, all six hooks listed as missing in final coverage, and `observerContinuity: false`. Readiness metadata and individual callbacks do not establish continuous coverage. The separate baseline crash record has no native stack or fault context; matching module/RVA in CP203 establishes agreement on the fault location, not identity of the caller for this instrumented run. Interceptors can also perturb timing.

## CLI order and overlap with the trace

The retained static CLI sequence is `main` → loader (`0x1ca30`) → `postOpen` (`0x2a0e0`) → recent-path update (`0x1dc00`) → splash finish → `showMaximized` → timer registration → `QApplication::exec`. This is instruction order, not a runtime trace of PID 20712. CP203 expressly says the launch receipt does not prove that the process reached the loader call or completed Open.

Three of CP204's four candidate routes overlap parts of that sequence:

| Candidate route | Static location in CLI/startup | What Hide/slot observations contribute |
|---|---|---|
| Loader wrapper `P+58h` → common wrapper `4ae0` → size (`4aee`) | During loader, before `postOpen` and recent-path update | Nothing caller-specific. A later observed dock Hide does not prove the loader call succeeded, and does not show whether a fault occurred before or after it. |
| Recent-path wrapper at image RVA `3d77548` → `ada30` → size (`ada85`) or common wrapper (`4aee`) | Recent-path update, after `postOpen`, before splash finish/show | Weakly compatible with having reached later startup UI work, but no observed CLI boundary joins the Hide to this call. It neither proves nor disproves either size site. |
| Notifier local snapshot → size (`4aee`, or conditional `818909`) | `postOpen` path, or selected constructor setup whose connection to CLI invocation remains unresolved | Hide/slot does not identify notifier state, snapshot loop, branch conditions, or size caller. No evidence supports selecting either notifier call. |
| Conditional pre-Open constructor local → direct size (`4fc35`) | Pre-Open setup, before loader in the described main-rooted candidate chain | Later dock handling could be downstream if this path returned, but the partial stream has no entry/return marker for this chain and is not chronologically tied to the baseline crash. It cannot eliminate the route. |

The fact that dock events are observed at 364 ms and later is not a reliable “after show” marker. Qt dock lifecycle events can occur during construction, visibility changes, layout, or other framework work. The trace does not hook the CLI loader, `postOpen`, recent-path update, splash finish, `showMaximized`, `QApplication::exec`, or any of the four size-call continuations. Event timestamps cannot supply those missing boundaries.

## Exclusion and ranking assessment

**Exclusion:** none. Hide events do not imply that the four conditional size calls were bypassed. A route can execute and return before the observed Hide; it can execute later; or a different path can produce the Hide. Conversely, if the crashing instrumented PID terminated during the trace, the partial capture does not locate termination relative to any size call because no exception event, fault registers, app return address, or trustworthy fault stack was retained.

**Ranking:** none defensible. The trace is not a sample of the faulting instruction's dynamic caller. Event-type count, ordering, thread ID, dock pointer, prepared request, and slot value do not discriminate the four operand provenances (owner-derived, fixed-image, notifier-stack, or constructor-stack). In particular, a Hide is not evidence that the fixed-image route ran, and the one `slot_entry` is not evidence that a notifier-list size check ran. A route's static proximity to UI setup is not a likelihood estimate.

The narrowest supported contextual statement is that, during this instrumented run, the observer saw dock event traffic and a `false` visibility request/slot around 521 ms, after the earliest dock event at 364 ms and before the last at 763 ms. This supports that some dock visibility machinery was active in the observed interval. It does not establish the application had passed every CLI startup stage, nor that any particular conditional QList call was reached.

## Evidence needed to discriminate routes

CP204's retained-exception alternative remains the useful discriminator: for a matching fault, retain exception access type/address, loaded module identities and bases, `RIP`, `RCX`, `RSP`, plus at least a readable `0x40`-byte fault-stack prefix. At QtCore size entry, the immediate return can separate the direct sites (`ada8b`, `81890f`, `4fc3b`) from common wrapper `4af4`; for `4af4`, the wrapper caller continuation can further narrow provenance. A QtGui return or invalid stack needs fuller unwinding. No new original run or GUI action is part of this audit.

## Sources and limits

Sources: `analysis/resume202-original-probe-attempt2-trace.md`; raw stream `analysis/resume202-passive-oracle-run-2026-10-26T04-45-03-155Z/frida-messages.jsonl`; `analysis/resume204-crash-candidate-slice.md`; `analysis/resume203-cli-open-static-semantics.md`; and `source/apps/moproj2mst/src/cli/main.cpp` (converter CLI only, not the original DEF GUI CLI). The converter's `wmain` parses converter arguments and invokes `m2m_convert_w` or isolated-save processing; it does not establish the DEF GUI startup order. The applicable original-app order here comes from CP203's qualified original-PE static evidence.

This report correlates retained evidence only. No source edits, builds, original execution, GUI acquisition, mutable REA operation, or fixture mutation occurred. The capture's partial-coverage and timing limitations remain material.
