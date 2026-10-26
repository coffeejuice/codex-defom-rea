# CP64: Boolean UpdateData event reachability

The widget-area UpdateData emission is a **synchronous invocation**, once its real notifier gate, receiver registration and current editor admit it. It does not itself post an event or await a dispatcher wake. A diagram timer can cause the upstream reselection that emits UpdateData, but timer registration and delivery are separate from this direct notifier leg. Static evidence still does not establish which first emission finds an installed Boolean editor on fresh Open. Preserve the public PostPending/timing guard.

## New original-PE evidence

All reads used immutable originals, with no process execution, GUI control, mutable REA, source changes, builds or tests:

| Binary | SHA-256 |
| --- | --- |
| `evidence/original-binaries/DEF_GUI_MO_64.exe` | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |

New captures are `analysis/resume64-boolean-event-route-static.json` (`1402cf3c0..1402cf738`, `1402cf740..1402cfacd`), `analysis/resume64-boolean-qt-invoke-static.json` (`670a83f0..670a85cc`, `67121de0..67122522`), and `analysis/resume64-boolean-qt-method-static.json` (`671201d0..67120969`); ends are exclusive. Each receipt records the selected DUMPBIN artifact, exact command, bytes, SHA-256 and tool hash. These are PE unwind-region bounds, not whole-program or arbitrary transitive-call closure.

`analysis/resume64-boolean-event-route-pe.py` reproduces bounded ordinary import/export and five literal reads into the unique `analysis/resume64-boolean-event-route-pe.json`; it refuses to overwrite its output. No target code is loaded or executed.

## Registration is not delivery

The retained widget-area constructor `140d14d30` creates `SfWidgetAreaImpl`, stores the actual impl at area `+28`, then calls registration `140d14e90` at `140d14ddc`. Its registration site `140d14f08` supplies notifier global `143d76700`, the actual impl, signal literal `1429fd038 = 2signalUpdateDataInWidgetArea()`, slot literal `1429fd020 = 1slotUpdateData()`, and fifth argument byte0 to `1402cfbe0`.

`1402cfbe0` is a custom named-receiver registry. It stores the receiver guard and stripped bare method name; flag0 uses append-after-dedup against guarded receiver identity plus method. Its flag is an insertion/dedup policy, **not** Qt AutoConnection. Flag nonzero's insert-at-front branch does not change the emitter's raw Qt invocation type. Registration does not invoke Apply and is not an event-loop registration. Existing source qualification is retained in `wave3-editor-native-owner-contract.json`, `wave3-close-view-manager-notifier-registration-contract.json` and CP63 `resume63-boolean-inherited-1402cfbe0.asm`.

The registry's actual population and actual suppression byte are runtime inputs. A constructor's known registration instruction does not prove that constructor has already run at a proposed Open point, that its receiver remains alive, or that no other receivers can run first.

## Emission directly invokes the registered receiver

The selected `1402cf3c0` body establishes this order:

1. At `1402cf3f1..1402cf3fa`, read notifier `+141`; nonzero skips this entire dispatch. No missing value is filled with zero here.
2. For the admitted branch, clear notifier `+90` at `1402cf408`, resolve the signal key from the map at notifier `+88`, and copy the selected receiver list at `1402cf461`.
3. Iterate that local list in order. Resolve each guarded receiver through `140c1aef0`; an expired entry is removed from this local list and iteration counters are adjusted. This is not evidence of removal from the stored registry.
4. Obtain the live receiver's metadata, convert the stored bare method to bytes, and build the generic argument vector.
5. At `1402cf6bd`, set **r8d=1**, then call IAT `141e05658` at `1402cf6c6`. The original import is `QMetaObject::invokeMethod(QObject*,char const*,Qt::ConnectionType,QGenericArgument,...)`. It returns a bool stored at `1402cf6cc`; this body does not branch on that bool before continuing the receiver list. Preserve that behavior rather than inventing a failed-invoke exception or early return.

The return-argument query sibling `1402cf740` also gates on notifier `+141` at `1402cf771`, copies the keyed list and explicitly passes r8d=1 at `1402cfa52` to IAT `141e05348` at `1402cfa5b`. This is the registered synchronous property-stack-index query mechanism; the caller's initialized index0 remains meaningful when no receiver writes it. It is not a dispatcher-readiness query.

The import/export chain and native branch make the synchronous conclusion independent of the textual enum name:

| Address | Native evidence |
| --- | --- |
| IAT `141e05658` -> QtCore export `670a83f0` | No-return-argument invokeMethod wrapper. It calls `67121de0` at `670a85bf`, passing the connection type through. |
| IAT `141e05348` -> `67121de0` | Return-argument overload, also the common named-method resolution path. |
| `67122488..6712249b` | Passes selected connection type to QMetaMethod invoke `671201d0`. Method/argument resolution can reject a mismatched name/signature before dispatch; registration alone does not force success. |
| `67120460`, `67120534..6712054b` | ebx is1. Explicit nonzero type bypasses the Auto0 thread-comparison conversion and is retained in r10d. |
| `67120685..671206a8` | For r10d==1 and available static metacall, calls the metacall immediately at `671206a4`, then returns. |
| `671206ad..671206f6` | Otherwise the same type1 branch immediately calls dynamic metaobject callback at `671206d8`, or receiver vtable `+10` qt_metacall at `671206ee`, then returns the result-derived bool. |
| `671206fb` onward | Type2 and other branches are separate. The selected type1 branch does not enter the later event-allocation/post calls at `6712088e`/`6712092c`. |

Consequently, delivery of **this emitted UpdateData invocation** does not require HWND, serial, postedWake, a timer ID, or a posted-event dequeue. A receiver's callback can still cause its own nested work, and the real guard/population/metadata must still be bound. This is not a claim that all notification effects are represented by the candidate.

## From receiver metadata to Boolean engine mutation

The retained `SfWidgetAreaImpl` qt_metacall `140deb760` dispatches its seven local cases to `140deb5a0`; local case3 calls `140dae340` at `140deb64d`. That method resolves the **current** guarded widget at impl `+18`, casts it, and on a valid result calls virtual `+1c0` at `140dae3ab`. CP63 direct PE reads bind Boolean's installed table `142cd9b18+1c0` to Apply `1411e2870`.

CP62 retains actual engine-add sites `1411e2955` for Workpiece and `1411e2bc6` for Cutter, in that order and conditional on each combo's Add New Object text. A synchronous Apply can still perform no add when its data/selection guards do not admit one. Neither a receiver registration nor a successful method lookup establishes the Add New branch.

CP63 also excludes an ordinary automatic setup Apply slot under the known static metadata: Boolean and SfWidget add zero local Qt methods; QWidget/QObject have no `on_*` candidates. setupUi's `connectSlotsByName` is registration, not activation. Dynamic metadata overrides, arbitrary receiver populations and transitive constructor events remain qualified.

## Lifecycle reachability and the first-Apply boundary

| Selected route | Established synchronous portion | Remaining condition |
| --- | --- | --- |
| Open refresh -> explicit row0 selection | `038950 ->d3b300 ->d3b7d0(false) ->038850 ->048640 ->class+158=5f36d0`. For non-Process Object and queried old widget index!=0, `1405f4524` emits the exact UpdateData key `142497798`; `1405f4721` later emits Leave key `1424977b8`, then factory creates/fills replacement and ShowWidget follows. Apply, when admitted, finishes inside the UpdateData call before this Leave/replacement. | The first initial selection/refresh must already have installed an old Boolean editor for this shown-editor branch. Index0 takes the distinct propsToEngine path. This lane does not prove that first old-editor population or the exact number of initial callbacks. |
| Timer-triggered reselection | After an actual modelUpdated callback reaches the selected current model/item and same-current Type0 branch, the UpdateData leg above is direct. | Registering a1000ms timer is not delivery. Receiver lifetime, current model/item, active project/loading gates, native timer delivery and nested callback order remain inputs. |
| Save's direct widget update | `1405ef53f` calls widget update; a nonzero property-stack index causes UpdateData emission at `1405f22af`. The selected receiver Apply finishes before propsLoad `1405f22c4`; outer Save then unconditionally calls propsToEngine `1405ef54a`. | Actual shown Boolean editor, admitted notifier/receiver/metadata and valid data/selection are needed. This proves a conditional synchronous Save Apply, not that no earlier Apply occurred. |
| Save entry's event pump | `140098129` calls flags0 processEvents before current-operation Save at `1400981e1`. | It may dispatch already eligible callbacks but does not wait1000ms or prove a specific callback occurs. An Apply during the pump needs its actual callback context; no separate UpdateData queue entry should be invented. |

The new literal evidence independently distinguishes UpdateData at `142497798` from nearby Leave literals `142497750` and `1424977b8`; nearby notifier emissions cannot be classified just by their common dispatcher address. Existing route source qualifications remain in `wave3-boolean-reselection-contract.json`, `wave3-save-widget-order-contract.json`, `wave2-db133-open-lifecycle.json` and `wave3-boolean-open-sequence-contract.json`.

The event-loop registration belongs upstream: retained `wave3-boolean-timer-contract.json` qualifies nonnull `170430` attachment's `QTimer::singleShot(1000,this,1modelUpdated())` at `17085f`; Qt `67134d90 ->67134870` allocates a dispatcher-child QSingleShotTimer and registers the interval via `6712b1e0`. Its timeout connection raw type3 is normalized to Auto0 by `671315c0`; absence of an actual dispatcher returns no registered timer ID. Timer event `67134ca0` kills a positive ID, activates timeout and deletes the timer. Only then does the selected `modelUpdated` metadata route reach `183160`, fit, resolve the then-current model/item and perform selection. Raw constructor0x4a posting is a different path and cannot stand in for this timer event.

## Decision and safe next action

A synchronous **conditional** Apply path is now proven through the actual notifier implementation at both reselection and Save. A synchronous **first-ever fresh-Open** Apply is not proven: the missing fact is which admitted emission first encounters the actual installed Boolean editor, including effects of the earlier selection and any delivered/nested callbacks. The decisive observation remains the first actual `1411e2870` invocation's editor/current-widget identity and bounded native stack, correlated with Open return, Save entry and modelUpdated nesting. Capture must await the existing owned-session/app-control release gate.

Safe candidate work can preserve this direct invocation boundary without fabricating an event: given a qualified shown-editor/current-owner/notifier state, test Apply completes before Leave/replacement Fill and before Save propsLoad. A deliberately absent/expired receiver or suppressed notifier must not become a fabricated Apply. Root owns any source/test task; this lane adds none. The public constructor PostPending continuation remains guarded, and native timer/HWND/serial/postedWake observations remain mandatory where their own upstream path requires them.
