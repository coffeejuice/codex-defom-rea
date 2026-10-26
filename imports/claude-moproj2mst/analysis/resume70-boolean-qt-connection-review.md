# Boolean receiver and connection audit

The CP70 payload recovery closes the selected Show overloads, but does not establish a first fresh-Open Apply. The next conditional receiver prefix is small and exact: the area's one-QWidget clone supplies false, so `0x140dae230` stores delete mode `impl+0x10=1` without a preceding Qt call in that branch. The candidate can represent that effect only after actual receiver/impl admission. It must then stop before unimplemented replacement effects; for a distinct live old widget the first such Qt call is **layout removal at `0x140dadc91`, before pending-copy `0x140dadcc3`**.

This read-only review wrote only this report. No source, build, executable test, GUI, original execution or mutable REA changed. Its evidence is retained original PE captures and current candidate source, not new runtime observations.

## The payload closes arity, not receiver admission

Reviewed `resume70-boolean-first-open-apply.md` and its generic-argument/metadata receipts against the selected retained emitter/wrapper bodies:

- Factory result lives in local `[rsp+0x98]`. The argument helper `0x140025080` forwards the type name and data address to imported `QGenericArgument(char const*,void const*)`.
- Reached `ShowWidgetInWidgetArea` site `0x1405f499f` carries one `QWidget *` argument; the selected area metadata clone `slotShowWidget(QWidget*)` supplies bool false at `0x140d48923`.
- Reached `ShowWidgetArea` site `0x1405f4e62` also carries one typed widget argument, not an empty vector merely because the application key ends in `()`. A null pointer value, if the site is reached with it, still has the QWidget argument type.
- The custom registry strips slot text to a bare method name. Qt's named invoke constructs the signature from emitted argument names and retries normalized signature lookup. Under the recovered static metadata, the main-window receiver therefore selects `slotShowWidgetArea(QWidget*)`, local25, through `0x1400db694 ->0x1400322a0`.

The earlier CP66/67 dependency on finding an inherited zero-argument Show slot is closed for these selected emissions. No zero-argument slot search or key-string arity heuristic is needed. Actual dynamic metadata overrides and an unsuccessful invoke remain separate inputs. Source argument storage must stay alive through the selected synchronous calls; the pointer-to-local-data vector is not a queue-owned serialized payload.

## Actual registration and receiver state needed

The following are distinct contracts. A single `connected=true`, typed owner lease or editor `shown` value cannot replace them.

| Reached operation | Actual receiver/binding required | Connection and state boundary |
| --- | --- | --- |
| Property-stack query | Actual SfMoMainWindow registered for `2signalGetPropertyStackWidgetIndex()`; actual `+0x480` QStackedWidget | Return-argument custom dispatcher `0x1402cf740` invokes raw type1. Query `0x1400320c0` reads actual stack currentIndex. Caller-initialized0 is retained if nothing writes it; neither a cached area nor editor `shown` certifies nonzero. |
| ShowWidgetInWidgetArea | Actual SfWidgetArea receiver; its actual `+0x28` impl; incoming factory QWidget lifetime and emitted argument | Registration helper `0x140d14e90` registers the area itself at `0x140d14eb8`; its one-argument clone calls wrapper `0x140d14fc0` with false. This is a custom named-method receiver entry, not an independent QObject signal connection with an inferred Auto mode. |
| ShowWidgetArea | Actual SfMoMainWindow receiver and its `+0x478/+0x480` owners, same emitted widget | Registration `0x140011362`; selected metadata and dock/helper effects precede `0x140018ae0(1)`. A request for index1 does not establish page membership/currentChanged effects or the value after callbacks. |
| UpdateDataInWidgetArea | Actual live SfWidgetAreaImpl registered at `0x140d14f08`, current guarded widget `+0x18`, valid native editor cast and correct Boolean widget data/model | Exact key `2signalUpdateDataInWidgetArea()` maps to bare `slotUpdateData`. `0x140dae340` reaches current editor virtual `+0x1c0`; Boolean's actual table resolves it to Apply `0x1411e2870`. Saved `+0x20` and pending `+0x38` cannot authorize Apply as current. |
| Deferred old-widget deletion | Actual impl timer `+0x40`, QObject parent/affinity, exact timeout registration and pending guard | Constructor connects timeout to slotTimerDeleteWidget at `0x140dad3a2`, raw type3. Its normalization/delivery is a separate Qt contract; it is not the diagram's1000ms timer and does not follow from an UpdateData emission. |
| Upstream timer-caused reselection | Actual diagram/editor receiver, pending timer identity/readiness and current model/item at callback execution | A timer can cause the selection that emits synchronous UpdateData. Timer creation/start is not delivery. A Save-entry nonblocking poll is an opportunity, not proof a timeout occurred before Open/Save. |

For the custom dispatcher, preserve the actual **shared SessionNotificationState** population and source order: suppression `+0x141` first; only unsuppressed dispatch clears `+0x90`, resolves the keyed map and snapshots the ordered receiver list; expired guards are removed from the local copy; live receivers resolve their own metadata and are invoked with raw type1. The returned invoke bool is ignored by the original receiver loop. A method-lookup miss is not permission to throw an invented native failure or skip later receivers.

Registration flag0 means append-after-dedup by receiver guard and bare method, not Qt AutoConnection. The selected raw1 invoke path bypasses Auto thread comparison and calls metacall synchronously. It requires no new HWND/postedWake/timer-readiness observation for **that direct leg**, while actual source receiver lifetime, metadata, caller gates and transitive callback effects remain mandatory. The receiver callback may reenter; synchronous is not equivalent to callback-free.

The source construction instruction alone does not establish that the receiver was already registered at a proposed first-Open point, is still alive or is the only receiver. Full typed QObject/native-owner lifetime and the same Session/runtime/thread must be bound. MainView/diagram receiver identity cannot substitute for SfMoMainWindow, area or impl.

## Exact smallest conditional prefix

The selected one-widget area delivery enters `0x140d14fc0`, loads its impl and calls `0x140dae230(false)` before `0x140d14f70`. The false test at `0x140dae24a` jumps over current-guard resolution, saved-guard assignment and hide. It reaches `0x140dae296` and stores byte1 at **`0x140dae2b2`**, then returns. Consequently the bounded effect is:

```text
admitted actual one-widget clone call
  -> same live area's actual impl
  -> impl.deleteMode10 = true
  -> preserve current18, saved20, pending38 and widget lifetimes
  -> stop at replacement continuation 0x140d14f70 ->0x140dadc20
```

This is the first source-qualified persistent receiver-field effect in the selected clone path. It must preserve the committed mode if the following replacement refuses. It requires no claim that a placeholder is present, a timer is due, a current index changed or Apply ran. The bool-true two-argument method has preceding save/hide effects and must remain separately guarded.

After this prefix, if the actual old current is live and differs from incoming, `0x140dadc91` calls QLayout removal **before** testing delete mode and copying old current to pending at `0x140dadcc3`. Thus an implementation cannot jump directly from setting mode to CP68's pending-copy observation and claim an executed prefix. With unavailable current/guard/cast/layout identity it stops sooner. With null or same current, use the actual branch and its later effect boundary; do not infer either branch merely to avoid layout removal. A uniform initial implementation may safely refuse at replacement entry rather than speculate about those branches.

No source change is proposed to the meaning of `observeInitialFlags`. It deliberately refuses replacing already known initial flags and therefore is **not** the execution API for a later Show-mode store. A future source task would need a distinct source-bound effect/admission method/cursor, actual same area/impl/occurrence validation and retained state across the next guard. It must not loosen immutable initial observations or replay the mode prefix to manufacture a desired profile.

The public path is earlier still: inspected Session has no canonical typed main-window/area/impl owner or installed area receiver population. Therefore no public caller currently qualifies the above conditional effect. Binding diagnostic owner storage under Session may preserve identity and unknown fields, but cannot certify constructors or register fake QObject receivers. The current owner/adoption and raw0x4a PostPending guards remain necessary.

## Candidate skeleton audit

CP68 `WidgetAreaWindowOwner`, stack, scroll-area, impl and child records correctly distinguish initially unknown fields, canonical control blocks, diagnostic admission and actual native-alive observations. Impl current, saved, pending and scroll content are separate. `requireAdoptionEffects()` always refuses. No source in this snapshot binds these records to Session or dispatches the recovered clone into them.

The three guard-write observation APIs are not an executor: `observePendingCopyAtDadcc3`, `observeCurrentClearAtDadfec` and `observeCurrentStoreAtDae213` require caller witnesses of those source writes. They cannot serve as success callbacks for omitted layout/parent/show/deleteLater/Qt effects. Their selected ordering remains correct: old-current clear precedes new Enter on delete mode; hide-only retains old current until post-Enter store; QScrollArea take/setWidget follows the impl store. Boolean Enter itself is an empty leaf, but prior Qt effects may reenter and are not thereby closed.

`BooleanSynchronousUpdateData` is likewise a conditional receiver suffix. It consumes a qualified emission and invokes the existing actual operation hook before Leave/refill, but its weak opaque aliases are caller admissions. CP68's typed liveness/kind observations alone cannot mint its `receiverRegistrationAdmitted`, `methodResolved`, current-widget match or emission occurrence. A distinct native Boolean child must also identify the correct filled widget snapshot and operation/model; private class snapshot storage reused across replacement is not proof of native widget lifetime/current identity.

## First Open remains unknown

The selected outer factory sequence emits UpdateData **before** Leave/new factory/show delivery when its actual property index is nonzero. Once actual impl/current Boolean receiver admission is established, that emitted Apply is synchronous. Starting at actual index0 instead takes the earlier propsToEngine branch; later successful adoption and page1 request may make another admitted selection eligible. Static order does not show that another selection/timer callback actually ran before fresh Open returned.

The minimum causal observation remains first actual `0x1411e2870` entry with native editor identity, impl current guard, stack/index-query result and bounded caller stack, correlated with Open return, the factory/adoption commits and any modelUpdated nesting. Keep requested timer IDs, due state, delivered count and original Open/Save boundaries separate. The CP70 Save-dispatcher report narrows the selected explicit opportunity to Save's zero-timeout poll; it supplies no observed due set. No GUI acquisition or runtime probe is authorized in this lane.

## Focused acceptance contract for a later source task

Use actual canonical receiver/impl aliases with declared diagnostic fixtures until real binding exists. Test that the one-widget type remains QWidget* even for an explicitly observed null value; clone false sets mode from either known prior value; saved/current/pending/lifetimes/index stay unchanged; and refusal before replacement preserves that mode. Test missing/foreign/expired receivers, unknown metadata, alternate control blocks, suppression-before-flag-reset and ordered local receiver snapshots independently. A method miss must retain the original continue behavior once the surrounding dispatcher is implemented.

For a distinct placeholder, require layout-removal completion before any admitted pending copy. Verify unknown/null/same/different current remain distinct, no timer start/deletion/show/Enter/index completion is inferred and no Apply occurs from owner creation, mode commit or page-selection request. Complete effect tests and first-Open capture are separate acceptance layers; a passing conditional prefix test cannot release public timing/PostPending guards.

## Review hashes and evidence scope

Original EXE/QtCore hashes and exact retained disassembly ranges remain attached to CP64/66/70 receipts. No new disassembly files were produced. The reviewed positive payload report SHA-256 is `6520a67f382ae57402629674c196c7bc0278190b8ab4076c5aa1d0cad464d8be`; payload literal/import receipt `1e8f6962dfba0590bb9592647971cda57653f6d42e961f1bc83b969ec29951d5`; selected area metadata receipt `0aa288e1aa831968d2bce8ac50c5145c3387c682126d0b4553425732fa70468d`.

| Reviewed source under `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `include/m2m/engine/widget_area_owner_state.hpp` | `6e4e4ca3c8b2fe373f8044f7db63b63dd1974006cf0443e5e2e60aa4651788a3` |
| `src/engine/widget_area_owner_state.cpp` | `b77f84438ccc562174ee831bc15435ded548ce34245ae3964043663d5cf98b19` |
| `include/m2m/engine/boolean_lifecycle.hpp` | `976cd0c9b2388d3e14de53c906ebfa027f09dbe2c78699247198d289fabce69a` |
| `src/engine/boolean_lifecycle.cpp` | `7340522bfce32301d5cbbfc970575aca0ce2fc0534d43a5bb43c4ebfadcfdd76` |
| `include/m2m/engine/conversion_session.hpp` | `6b05d5d085ff23b42e7a3f1c784ea5e92576945504e471d1dda29d44abb17a99` |
| `src/engine/conversion_session.cpp` | `99731a89662bac4b9162c22f0bbb649da3e65c64a5431c96048060774b4ab8a3` |

These are review snapshots, not tested build freezes. Token measurements are unavailable.
