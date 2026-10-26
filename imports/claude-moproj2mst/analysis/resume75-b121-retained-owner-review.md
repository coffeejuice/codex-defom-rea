# CP75 — Static review of the retained B121 scalar-prefix Study owner

## Scope and disposition

The new `readOwnedB121ScalarPrefix` path is a candidate-only adapter for the source-qualified prefix of the retained B121 TLOC branch. Its concrete ownership and retry model are internally coherent for the supported fresh mode-0 path: `Study` owns a retained state, that state owns the derived variable, and `Study::vars` is a non-owning alias. The helper stops by throwing at the unresolved `2signalSetOprHasDoe()` delivery boundary; it does not claim a successful native read or wire into public Type 2 parsing.

No blocking ownership/lifetime defect was found for the retained valid-row path. One boundary limitation should remain explicit: the owner is published before `read()` inspects the row, so some unsupported/malformed inputs can leave an empty retained-state marker even before the first native variable append. This is outside the supported fixture branch and is not tested; it must not be described as native malformed-input parity.

## Concrete owner and alias lifetime

`src/doe/variable_prefix.cpp` implements this ownership chain:

1. `readOwnedB121ScalarPrefix` first checks the live `ConversionSession::Project`, verifies that the `SubProject` address is present in `project.subs`, and checks active-subproject/type/project back-links, `ownedDoeStudy`/`doe` identity, and the Session-bound `StudyIo`, `ModelManager`, and notification-state identities.
2. The stack `B121ScalarVariablePrefix` allocates a `B121ScalarPrefixState`. After `requireFresh` succeeds, `retainOnStudy()` publishes that state into private `Study::retainedScalarPrefix_` before parsing begins.
3. `read()` allocates `ScalarVariablePrefixRecord` into `state_->variable_`, then appends its raw address to `Study::vars`. On the valid B121 path it reaches `readKeyword`, creates the typed keyword/list prefix, records the pending notification payload, and throws at that unresolved delivery.
4. Stack unwinding does not remove `Study::vars` because `retained_` is set. The Study-owned shared state keeps the derived record alive. The prefix state has no Study/Session back-pointer, so it does not create an owner cycle; a separately retained const diagnostic observer may outlive Study storage while still owning only the prefix record.

`B121ScalarPrefixState` is complete in `variable_prefix.cpp` when its `shared_ptr` control block is created, and its `unique_ptr<ScalarVariablePrefixRecord>` deletes the derived type directly. `Study::vars` remains a raw alias and is destroyed as a vector without owning/deleting the record. This deliberately differs from the other isolated complete-model helper, which stores a completed `Var` in `scalarPrefixVariableOwners`; `readOwnedB121ScalarPrefix` checks that vector is empty and does not use it.

The fresh-owner preflight is ordered before publication. Wrong Session/Project/SUB identity, inactive or non-Type2 subproject, broken Study alias, unbound I/O/manager/notifier, wrong `DoeVariables` tag, nonzero sample mode, preexisting sample rows/variables/other prefix owners, and retry with an existing retained prefix all refuse before this function mutates the retained owner or model. The focused tests cover representative cases from each group.

## Unwind and retry contract

For the valid retained row, the refusal is intentionally after native-prefix mutations: tests assert `Study::vars[0]` aliases the state-owned record, index remains `-1`, selected index is `0`, sample-invalid flags are set, operation 1 and the keyword prefix exist, nominal text remains unset on the keyword, and the pending notification contains `true`, operation 1 and the collected nominal string. They also assert no Study file I/O, directory creation, notifier attempt, XML write, or `sub.loaded` mutation occurred in this isolated call.

On a second call, `readOwnedB121ScalarPrefix` detects the existing retained prefix before constructing another driver. The test checks that the same state, variable address, keyword list and pending signal remain and no duplicate variable is appended. There is no reset/continue/success token, which is appropriate while callback delivery and later selectors/text/reindex remain unsupported.

The failure case with `varId=0` checks a different native-order boundary: the variable is allocated/appended and receives its zero number before the candidate's explicit unsupported guard. Because the retained state owns it, the raw Study alias remains valid after stack unwinding. This is evidence for retained prefix lifetime, not an assertion that the original application rejects `varId=0` there; the retained original body does not establish that validation.

When Session Project storage is released, the test proves that an escaped diagnostic `shared_ptr<const B121ScalarPrefixState>` can keep the record readable after the Study expires, then frees it when the observer is released. With no escaped observer, clearing subprojects reclaims Study and prefix state together. These are adapter lifetime tests, explicitly not native Close or QObject lifecycle replay.

## Guard and test isolation

`readOwnedB121ScalarPrefix` is declared in the private DOE prefix header and is not called by `readInputTable`, `readLazyStudy`, `parseSubProject`, or conversion dispatch. Repository search finds its production use only in its declaration/definition; the new tests call it directly. The public `readInputTable` nonempty-`DoeVariables` guard remains in `src/doe/doe_study.cpp`, and `writeInputTable`/`saveInputTable` retain their nonempty-variable refusals. `test_retained_scalar_prefix.cpp` explicitly checks the public reader and writer still refuse and produce no writes.

The new owner test uses a focused in-memory TLOC XML row rather than opening the retained B121 file. A separate existing model-prefix test reads the retained fixture and verifies its fields. The new test therefore validates Session/Study owner binding, partial state, unwind and retry mechanics against the expected row shape, but it is not an end-to-end fixture loader or public B121 conversion test.

## Boundary caveat

`readOwnedB121ScalarPrefix` calls `retainOnStudy()` before `driver.read()`. `read()` sets `started_` and then walks variable elements; its unsupported checks for missing/invalid name/sample/header attributes can throw before `state_->variable_` is allocated, and an empty/unknown-only `DoeVariables` list can throw at the “empty variable-list finalization” boundary. In those cases the Study can retain an empty/started prefix marker although no variable append occurred. The existing tests cover wrong root/mode/preexisting rows before publication and a post-append failure, but not these pre-append parse failures. Since this helper is deliberately candidate-only and refuses rather than reporting success, this does not corrupt the supported retained B121 row; it does mean no claim should be made that malformed/empty input leaves Study unchanged or matches native failure semantics.

The original PE receipt `analysis/resume50-doe-variable-body-pe-receipt.json` establishes that the valid fresh B121 branch mutates variable and keyword/list state before the manager emits `2signalSetOprHasDoe(true, 1)`. It does not justify publishing adapter state on every malformed/empty input. If callers need safe retry across those unsupported shapes, either preflight them before `retainOnStudy` (as a candidate-only scope check) or add tests and document the retained empty-state behavior explicitly; do not remove the pre-notification state retention for the valid branch.

## Review limit

Read-only source and test inspection only. No build/tests were run, no GUI/runtime capture or mutable REA operation occurred, and public B121 Type 2 activation/read/write remains guarded.
