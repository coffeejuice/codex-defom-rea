# CP145: actual notifier entry before key/receiver admission

The smallest source-ready next **wired** effect is the actual shared notifier's suppression check and unsuppressed flag90 reset, followed by a guard before its unresolved key/map/receiver phase. It requires carrying a checked per-call Session/Project/SUB/StudyIo context through the actual lazy reader. The current constructor-only reader has no such context, so directly calling its existing diagnostic dispatcher would skip both an owner edge and the earlier key boundary. Receiver return and named-keyword continuation are not yet admitted.

Only this new report was written. No source/test/build, GUI, original process, mutable REA session or new runtime observation occurred. CP144 repair/build/freeze source was left untouched. This report uses retained original evidence and current canonical source; it does not request or authorize an original runtime probe.

## Exact invocation and return contract

The controlling retained native sources are `resume50-doe-variable-body-pe.md`/annotated reader, `resume51-doe-variable-notify.md` and its argument-order capture, `resume77-doe-setopr-slot-evidence.md`, the complete dispatcher captured in `resume64-boolean-event-route-1402cf3c0.asm`, and CP126/130's key-owner boundary. Original GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

At91095a the reader calls the installed derived manager vt+118=0d25a0 using the original row name. Its base call constructs/appends on that named Variable and unions operation IDs. The derived method then iterates the supplied keyword operations in their original order, including duplicates, rather than the deduplicated Variable ops union. For the admitted CP144 profile this is one operation1.

The call at0d2995 enters2cf3c0 with key `2signalSetOprHasDoe()`. Final argument-frame evidence corrects the older construction-order description: the logical slot arguments are **int operation ID first, bool true second**. Native key/receiver delivery is not inferred from creating equivalent host Strings or recording an attempt.

Dispatcher2cf3c0 checks suppression+141 at2cf3f1. Nonzero returns before flag90, key construction, map/list access or receivers. Otherwise it clears flag90 at2cf408, then constructs the key QString at2cf41f. Only after key construction does it select/default-insert the map list, take the ordered local snapshot and walk weak receiver guards.

Live receiver invocation uses DirectConnection1. The ordinary invokeMethod Boolean is ignored; false is neither parse failure nor a reason to undo earlier effects. Earlier completed receiver effects remain if a later receiver refuses/faults. Native argument temporaries/lists then unwind normally; the derived manager proceeds to the next supplied operation only after the current dispatcher returns. A host aggregate success flag cannot substitute for this sequence.

## Actual receiver ownership still missing

Normal SfMoDiagEditorModel constructor070f10 registers at071166 with receiver=this, slot `1slotSetOprHasDoe()`, the matching global signal key and flag0. Copy constructor0711a0 has the separate registration071a7f. Flag0 appends a new receiver/method pair; flag1 prepends. All matching registered models participate, not only the active SUB.

Qt metadata row23 binds `slotSetOprHasDoe(int,bool)` through static metacall0d8eb2 ->086e20 ->086ca0. Those are source-qualified static routes, not a live-instance census. The actual QObject weak guard, dynamic method state, complete registration order and each receiver's current backend/list state must hold at invocation.

Receiver086ca0 returns immediately for whole-model backend+18 null. With a nonnull backend it searches the logical list for the first matching operation ID; no match returns before QIcon allocation. A matched true receiver allocates an8-byte icon, constructs/configures its resource and transfers it through the actual backend/graphic item route. Those allocation/old-icon deletion/new-pointer or missing-graphic-item lifetime effects cannot be replaced with a Boolean hasDoe field.

The candidate Session-owned normal-SUB constructor history is useful but deliberately incomplete. `FlowReceiverPopulationPrefix::completeNativePopulation()` is false (`src/doe/flow_receiver_population_prefix.hpp`). It preserves ordered normal/copy/dead witness events without installing an actual complete registration list. `FlowReceiverWitness::backendWitness()` always returns nullopt (`src/doe/flow_receiver_witness.hpp:27`), meaning unavailable, **not qualified null**. A fresh constructor's native null store or a headless model does not close its later backend publication interval.

Thus there is no production fact permitting a null-backend or empty-receiver shortcut. Current diagnostic receivers and qualification strings are test profiles, not observed native receiver instances/returns. No receiver callback can be marked completed from the constructor projection alone.

## Key admission precedes population admission

CP126 proves the unsuppressed QString(char*) helper reads the actual process codecForCStrings field at QtCore RVA2f3e10, at instruction74126, before allocating/publishing source data. CP130 found historical null observations in a different editor process/stage, not this call. The SetOprHasDoe key is a distinct source occurrence from DoeTableRefresh and the icon resource. Do not reuse their data slots, allocation lengths or owner records.

Under the full native-data scope, unknown/non-null invocation codec/data state still prevents key construction. Map detach/default insertion, pointer-list COW/ref/liveness snapshot and complete receiver population are later independent dependencies. Under an explicitly assigned host value abstraction, a fixed key String still does not prove an actual complete population or backend witness.

Existing `dispatchSetOprHasDoePrefix` verifies its owner, checks suppression, resets flag90, then guards at Population when no registrations exist. Its isolated map/list/receiver adapter does not model this key QString's native data owner. Wiring it wholesale into the real reader and reporting its Population stop as execution of all earlier native stages would therefore overstate the source boundary. Keep its diagnostic/test use; introduce or refactor an explicit **pre-key** entry before any production binding.

## The current actual-reader context gap

`parseSubProject(Project&,Node&,Ctx&)` currently leaves Ctx unnamed (`src/model/subproject.cpp:366`). Its Type2 branch validates owned Study and configured Project StudyIo, then calls `readLazyStudy` without Session at384..388. `readLazyStudy` has the actual StudyIo but `InputTableSource` carries only a value-owned filename. The parser, Header helper and CP144 keyword shim receive Study, original row name and local dimension, with no trusted live Session handle.

Do not reconstruct that handle by following `Study.sub->project` after a separately retained Study may have outlived its Project. Do not create/rebind a notifier or use a retained shared I/O/manager lease as proof that Session still lives. The existing `bindSetOprHasDoeSessionPrefix` validates live Session Project, exact Project StudyIo and exact Session notification owner; this is the right identity pattern, but the real call chain must supply that Session first.

## Smallest source-ready bounded integration after assignment

Prefer one actual per-invocation context bridge and the pre-key effect, rather than another isolated helper that is never called:

1. At the actual Type2 parse call, consume existing `ctx.session` when available. Carry that borrowed live Session reference plus expected Project/SUB/Study/I/O identities through the synchronous lazy read and named InputTable context. It must not escape the read or be stored as a mutable Study/Session-global current-reader pointer.
2. Immediately after CP144 commits its canonical constructor prefix, validate Session.liveProject()==the expected Project; find owned SUB membership before dereferencing its saved pointer; require its owned Study/sub.doe/backlinks match this Study; require the same Project StudyIo used for this read; require its observed notifier equals the actual Session notification owner. Context validation cannot manufacture these owners or rebind I/O.
3. Enter a precise notifier-prefix function using `(operationId=1, hasDoe=true)` and the canonical owner. If suppressed, record/return the suppressed-callee outcome without modifying flag90/map/list. If unsuppressed, clear actual flag90, retain that store, then guard **before key construction**, with no empty key insertion or receiver snapshot/delivery.
4. The reader still stops. Even a suppressed callee return must stop at an explicit post-notifier named-keyword lookup boundary in this first increment; do not silently enable selector/text/reindex continuation. Bytes-only/source-less callers keep CP144's existing stop-before-notifier behavior when no actual context is provided.

The bridge must not set suppression to true to select an easy branch. A scoped production search found only suppression reads/diagnostic copies in DOE dispatch code; current default false does not supply a normal true producer for this call. Private tests may exercise true as an explicit profile, not evidence of the selected original path.

Suggested ownership after a separate root assignment:

- Root/shared bridge: `src/model/subproject.cpp`, `include/m2m/doe_activation.hpp`, `src/doe/activation.cpp`, private `src/doe/input_table_reader.hpp`, `src/doe/doe_study.cpp`, and the Header-helper context declaration if its existing signature is extended. The C ABI and legacy bytes-only public behavior must remain unchanged.
- Worker notifier/key boundary: existing `src/doe/variable_notify.hpp/.cpp` for a checked pre-key entry/outcome, and `src/doe/variable_record.cpp` / `keyword_reader_prefix.hpp/.cpp` only for forwarding that per-call context and replacing the precise post-constructor stop. If root factors the shared suppression/flag implementation with `DoeTableOwnerState::enterRefreshNotifierPrefix`, assign that disjoint shared file explicitly rather than duplicating or misnaming a refresh receiver event.
- NEW focused test `tests/doe/test_keyword_notifier_entry.cpp`, plus a unique receipt. No registration population installation, QWidget/backend/QIcon source, Save writer or CMake edit belongs in this slice.

This is a source-ready value/state increment **once that real context is assigned and threaded**. It is not presently callable with CP144's arguments alone. All identity validation occurs at the reached notifier encounter, preserving already-committed constructor/header/sample-invalid effects if the new owner/context guard refuses.

## Post-callback named lookup stays separate

After all actual messages return,9109c8 obtains keyword count for the original Variable name, subtracts1, and910a1b fetches that named record. The parser then writes selector at910a38, flags, nominal text and later scalar/function/enum stages. It uses the lookup result without a native null guard.

Never use CP143's borrowed record pointer as proof that this result is unchanged after callbacks. A future continuation must re-resolve the current actual named owner/count/last keyword at that encounter, preserve current content/lifetime and refuse undefined/unqualified null or reentrant deletion profiles. A,B,A still uses named A for Keyword while prior header/Description effects and selectedVariableIndex may remain on B; notifier return does not select A.

No qualified all-receiver return is available for the normal unsuppressed production branch. The static DirectConnection route and ignored Boolean establish ordering, not that a concrete callback returned. Therefore no selector/text commit, reindex, deferred links, later sample I/O or parser success is source-ready for public integration from current receiver facts.

## Focused validation and concrete missing observation

Tests for the proposed bridge should use an actual ConversionSession/Type2 lazy read and existing bound StudyIo. Assert owner/context identity refusal retains the already-appended keyword; the same canonical notifier's flag90 changes only on unsuppressed entry; suppressed entry leaves it unchanged; no key/map/list/receiver/nominal-text effect is recorded; direct legacy bytes callers retain their old boundary; no sample/DOM/public Save continuation occurs. Include stale Project/SUB/I/O/notifier replacement and duplicate A,B,A selection/target invariants. Do not use a test-set suppressed flag, declared empty registration list or `optional(nullptr)` backend as native admission evidence.

Beyond that entry prefix, the smallest original witness is one actual reached invocation with exact key/argument order, actual codec/key return, ordered weak receiver population and first receiver's actual backend/logical item state, followed by its normal return/exception and post-callback named-keyword lookup. Runtime observation would require separate root serialization and the existing GUI/control release policy; it was not performed or requested here. If such a witness is unavailable, keep Key/Population/Backend/CallbackReturn guards rather than rescan static imports or infer emptiness from constructor history.
