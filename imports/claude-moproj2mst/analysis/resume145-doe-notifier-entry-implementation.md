# CP145 actual lazy-reader notifier entry

The real Type2 lazy reader now carries checked per-call Session/Project/SUB/Study/I/O identities to the reached TLOC notifier entry. The actual canonical notifier applies suppression or the unsuppressed flag90 reset, then the reader stops before key construction or at the suppressed-return named-keyword lookup boundary. Constructor/header effects remain committed. Public Save and receiver delivery remain guarded.

Root assigned this wired increment from `analysis/resume145-doe-notifier-admission.md`. No worker build/CTest, original process, GUI or mutable REA action occurred. Only the assigned files below, the new test and this new receipt were written; no public record header, emitter, Save writer, CMake or additional shared file was edited.

## Frozen source release

All source was frozen and sent to root before this receipt was written. Paths are relative to `source/apps/moproj2mst`.

| File | SHA-256 |
| --- | --- |
| src/model/subproject.cpp | 9d2ea605eaeb092019405ecb5cbfaac81c2031aad1ed034da6f1fc722d1975d7 |
| include/m2m/doe_activation.hpp | dae01e454c9052b019d7d1e3db1cdd3d99fe171eb039f31e38ca5945011d86b6 |
| src/doe/activation.cpp | ffb76fcfb6c768bcf15ae77480ab781b5dca0c9594d1f42a6822b6131eb850d1 |
| src/doe/input_table_reader.hpp | 886a610a055953e8acf9f0f3bd45bd85e0d612dacf2a8fe5a6b50641595bdc75 |
| src/doe/doe_study.cpp | e70b1709e6b7ef229e3923271b5b579faec490d50aad80c64b342c77380a7180 |
| src/doe/variable_notify.hpp | 540418ab75fa1a3d90f7e263eee4ca9c32acb9a84079ebe4c7c63f313b72eb7c |
| src/doe/variable_notify.cpp | 299b520c47576880b619ea3764154ee545b91518ec62f0529d829df0524a807a |
| src/doe/variable_record.cpp | 829cfbca780a0f95ad0313c75580998d8ebc31fafb461b0cce601a8b338f3abf |
| src/doe/keyword_reader_prefix.hpp | a0fe2154c0c523e2387427490941d6f4629328417f2443cbd2f9d743fe060445 |
| src/doe/keyword_reader_prefix.cpp | 561840e9d75e9d99e6f96122d656668e7602ee26322a1c24e9855fd4cc3c6481 |
| tests/doe/test_keyword_notifier_entry.cpp | 5b71af91ff414fa37b50823f3e105036049f3a825574bf69febd4a8afe009192 |

## Actual bridge and compatibility

The actual `parseSubProject` Type2 call constructs `InputTableNotifierContext` from its existing Ctx.session, Project, owned SUB/Study and Project StudyIo. Context contains only borrowed pointers plus a weak StudyIo control-block identity; it is not stored on Study or in Session-global reader state. Its Session must remain live for the synchronous call. It is not an asynchronous resumption token.

The contextual `readLazyStudy` overload copies that context into the per-call value-owned InputTableSource after the actual XML read, then the parser carries its pointer only through that read/unwind. The public bytes-only InputTable entry remains source-less. The original four-argument `readLazyStudy` function is preserved and delegates with no context. `parseSubProject` with no Ctx.session also retains the prior boundary. No implicit Session lookup follows Study.sub.

The public `appendBoundedHeaderVariable(Study&,Node&,DeferredVariableLinks&)` declaration and symbol remain unchanged. It delegates to the new private `appendBoundedHeaderVariableFromSource` with nullptr; the named-source parser uses that private contextual entry. Geometry filename-value context remains present and its guard is not relaxed. No C ABI change was made.

## Reached notifier entry and partial order

CP144 still decodes only the exact retained TLOC profile, resolves the original row name, commits CP143's dimension/constructor/owned append/operation union and preserves A,B,A's named target versus last Header behavior. Only afterward does it consume an available notifier context.

New `enterSetOprHasDoeNotifierPrefix(context, study, int operationId, bool hasDoe)` is bounded to the actual TLOC call's int1/booltrue arguments. It validates:

- live explicit Session and matching expected Project;
- owned SUB membership before dereferencing the saved SUB alias;
- actual expected Study, owned Study/sub.doe and Project/Study backlinks, with Type2 SUB;
- current Project StudyIo matching both the captured pointer and weak owner/control block;
- the same Session ModelManager observed by that I/O;
- the actual Session canonical notification owner already observed by that I/O.

All these checks occur at the notifier encounter. A refusal keeps the previously committed constructor/header/sample-invalid effects and changes no notifier flag/map/key. A retained old I/O or Study lease cannot make a replaced I/O or removed SUB dispatchable.

Native suppression at2cf3f1 returns before key/map/receiver work and before flag90. The helper returns `SuppressedCalleeReturned` without a store; the reader then guards at post-notifier named-keyword lookup. Unsuppressed entry clears canonical flag90 at2cf408 and returns `KeyConstructionRequired`; the reader retains that store and guards before constructing the QString key. Neither path performs named lookup, selector/type/text writes, reindex, deferred links or later sample I/O.

No SessionNotificationState attempt, key insertion, registration snapshot, receiver visit or callback-return receipt is fabricated. Its existing generic attempts collection describes other qualified key/map events, not this newly reached pre-key prefix. The older diagnostic `dispatchSetOprHasDoePrefix` implementation and its test population adapter are unchanged and are not called by the real reader. This avoids skipping the earlier native key ownership boundary merely to report a Population stop.

An explicitly test-set suppressed state is a value profile, not a production suppression producer. The bridge does not set suppression, invent a null backend, construct a QString data owner or install an empty receiver list. The retained native argument-order correction (int before bool) is preserved in the new typed interface and actual call.

## Focused tests and static preflight

New target `m2m_doe_keyword_notifier_entry`, CTest `m2m.doe.keyword_notifier_entry`, contains five cases:

1. Actual unsuppressed Type2 lazy A,B,A read reaches the key guard, changes canonical flag90 true->false, preserves keys/attempt count, commits Keyword on A and Header/Description on B while selection remains1, retains sample payload and skips later I/O. Public saveInputTable still refuses before outputs.
2. Actual suppressed lazy read leaves flag90 unchanged and stops after suppressed callee return before named keyword lookup. Constructor text/selectors/data remain uncommitted.
3. Public bytes-only, original four-argument lazy read and Ctx.session-null actual parse retain their old stop-before-invocation behavior and do not touch flag90.
4. Wrong actual Session supplied to the wired path refuses only after the constructor prefix, leaving both Session notifier flags unchanged and preserving actual Study data.
5. Nine direct negative context profiles cover missing/mismatched Project/SUB/Study/I/O, replaced I/O with retained old lease, rebound notifier, missing weak I/O identity, removed SUB and wrong bool argument. They retain flag/key/attempt state and the canonical constructor owner.

Compile-time function-pointer typing asserts the entry's int/bool argument order. Tests never dereference removed SUB or expired I/O aliases and never claim a completed native receiver census or callback.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_keyword_notifier_entry.cpp` returned `ok: true`, no issues. Direct final-newline/trailing-whitespace checks passed on all11 source/test files. Ctx's complete definition is already included through model/common.hpp; private context/header include paths and the original Header wrapper were inspected statically. This does not replace C++ compilation or execution. Root owns the grouped Release build, focused tests and required full validation.

## Remaining dependency

Unsuppressed execution now stops at the exact key-construction boundary. Invocation-bound C-string codec/data/ref ownership, map/list/COW state and the complete actual receiver population/backend/callback return remain unqualified. Suppressed execution still stops before the original named-keyword count-1 re-fetch. No CP143 pointer is used as proof of a post-callback target. Public nonempty Save/global operation-manager reconciliation, prune/XML, DoeTableRefresh and later PAR sequencing remain guarded.
