# CP125: actual notifier scalar prefix before guarded refresh

The real Session-owned Type2 XML/refresh path now represents the native unsuppressed notifier flag90=false store before its unsupported key/receiver boundary. Suppression leaves flag90 unchanged. The refresh entry remains noreturn, so this change does not release active Save, deliver a QObject slot or call later PAR writers.

Qualification: `analysis/resume125-doe-refresh-next.md`, SHA-256 `CC07ED6A9397B2484DA903BD4C1D2792566297163C7BD5FE3B0B272A71CAC787`. The retained complete native0x1402cf3c0 body reads suppression+141 at2cf3f1, jumps to epilogue for nonzero, otherwise stores flag90=0 at2cf408 BEFORE2cf41f key QString construction or2cf454 map lookup. The actual Save wrapper passes global0x143d76700 and the exact no-argument DoeTableRefresh key at0x1400d369d after either normal XML writer Boolean. Native dispatcher return, key population and receiver effects remain separate from the XML result.

## Source change and ownership

Only existing `source/apps/moproj2mst/src/doe/table_owner_state.cpp` and NEW `source/apps/moproj2mst/tests/doe/test_refresh_notifier_prefix.cpp` changed in this source lane. The cpp explicitly includes the existing SessionNotificationState definition. No public/shared header, ABI, CMake, shared writer, Session, CP124 XML file or existing test changed.

`DoeTableOwnerState::requireRefreshDelivery(actualSession)` retains its existing scope check before any mutation. State::matches still requires unreleased scope, same Session identity, actual ModelManager owner backlink, and matching pointer/control-block identities for ModelManager, WidgetRuntime and notifier. It then locally locks that existing weak notifier owner, without a StudyIo lazy getter or replacement owner. If suppression is false it writes flag90=false on that actual owner. If true it makes no scalar store. Existing main-window/table diagnostics still refuse in both branches, and the source comment explicitly marks key construction/delivery and post-suppressed-callee public continuation as unimplemented.

No key insertion, SessionNotificationAttempt, registration snapshot, receiver lookup, table allocation/publication, timer state, widget state or current Project selection was added. Constructor/cache/deletion diagnostic record methods are unchanged. A foreign Session or released admission scope refuses before touching either notifier. A later unsupported error retains the reached store; no rollback is added. Public retry starts another Save/XML occurrence, rather than certifying a prior callback completion.

The source state begins with flag90=false, and no production true producer has been established. Tests setting true or suppression explicitly are diagnostic initial scalar profiles, not an original-runtime occurrence witness. The same actual Session/Study/I/O ownership is nevertheless exercised through the production C++ Save entry. No Close-specific progress/busy token is reused for DOE.

## Focused tests and static checks

New target `m2m_doe_refresh_notifier_prefix`, CTest `m2m.doe.refresh_notifier_prefix`, has five Testkit cases:

1. Actual Session/Project/Type2 SUB lazy-read retained Constraints then Save: all three normal XML outcomes (success, explicit open-false, explicit post-close error7) reach flag90 reset and existing refresh refusal. XML normal-return/boundary and accepted/prior recording bytes survive; old SUB DOM, owned records/unknown+10, sample state and later sample-write count stay intact. Faults are bound to exact path/ordinal0.
2. Suppressed modeled branch retains each initial flag value, keys/attempts and unknown receiver/timer state; public continuation still refuses after recorded XML return/boundary.
3. Interrupted XML serialization after open/count mismatch preserves flag90=true, truncation and cleanup, without normal writer return or refresh boundary.
4. Foreign Session and released admission scope refuse before notifier stores, without manufacturing receiver death.
5. Diagnostic field-initialization receipt alone does not dispatch/reset; a subsequent actual-owner refresh entry performs the reset and remains guarded.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_refresh_notifier_prefix.cpp` passed. Direct trailing-whitespace/final-newline checks covered both source files, including the untracked test. `git diff --check -- source/apps/moproj2mst/src/doe/table_owner_state.cpp` passed. No worker compiler/build, executable, CTest/full suite, GUI/application control, original runtime or mutable REA session was used. Root owns compiled/runtime validation.

Root was notified before and at release that `tests/doe/test_table_owner_state.cpp:117` has the one obsolete expectation of flag90 remaining true after unsuppressed requireRefreshDelivery. Root owns the narrow correction. This worker did not edit that file. Constructor/cache/registration receipt events themselves continue to preserve notifier fields.

## Source freeze and next dependency

Source hashes sent to root before writing this receipt; all source held for grouped validation/archive:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/table_owner_state.cpp` | `AAD5EB5382EDF7DF98ADD74576FFAB8B89738BBFF0CD0E6CCC2427C113EE466E` |
| `source/apps/moproj2mst/tests/doe/test_refresh_notifier_prefix.cpp` | `8C1789FF7E61E3C47E52DF121C686C5AE739858986CC650BBEEBCA941CDB1B04` |

The next unsuppressed native edge is key QString construction0x1402cf41f, followed by actual map/default insertion, ordered registration snapshot and guarded receiver delivery. The known table slot then needs actual QTimer.stop(table+B0), actual virtual refresh/UI/current-project work and normal dispatcher return before cached-manager PAR calls. Suppressed callee epilogue does not itself implement the caller continuation. No guessed registration, QString shared-null/codec owner, callback-success flag, public Type2/DBLIST release or native allocation/fault parity is introduced.
