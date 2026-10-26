# CP63: earliest Boolean Apply invocation

The static callgraph narrows the first engine mutation to applying an already installed Boolean editor. It does not locate that first invocation in runtime time: the initial Open selection and delivery of a later modelUpdated callback remain unbound. Keep the public constructor PostPending and Boolean timing guards.

The new finding is that the ordinary Boolean setup auto-connection route supplies **no Boolean Apply slot**. `SfObjectSetupBool` has zero local Qt methods, its intermediate `SfWidget` has zero local methods, and the retained QWidget/QObject methods contain no `on_*` names. `connectSlotsByName` registers matching `on_*` methods; it does not invoke Apply. This finding is qualified to the known native static metadata, not an uncaptured dynamic metadata override or arbitrary event/filter receiver population.

## New immutable evidence

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; preferred image base is `0x140000000`. No original process, GUI, mutable REA, source edit, build or test was used. `admin/capture_pe_ranges.py` retained exact selected unwind regions through DUMPBIN 14.44.35228.0. Unwind boundaries are region boundaries, not independent proof of an entire logical procedure.

New raw files and reproduction:

- `analysis/resume63-boolean-apply-first-static.json` and its four `.asm` captures: `1411e4f00..1411e4f4d`, `1411e4f60..1411e4faa`, `1411e4fb0..1411e4ff9`, `1411e2dd0..1411e37fa`, all ends exclusive.
- `analysis/resume63-boolean-inherited-static.json` and its three `.asm` captures: `140376bd0..140376c1a`, `140130740..1401307bf`, `1402cfbe0..1402cffca`, all ends exclusive.
- `analysis/resume63-boolean-apply-first-pe.py` produces `analysis/resume63-boolean-apply-first-pe.json`: file-backed PE VA spans, ordinary import symbols, selected vtable entries and explicitly limited carrier scans. It refuses to overwrite its result. It never loads or executes the binary.

Each disassembly receipt contains per-file bytes, SHA-256, exact command and tool hash; the PE receipt contains literal bytes/hashes and the original binary hash. No old capture was overwritten.

## Direct entry and metadata

| Original address | Established result |
| --- | --- |
| `142cd9b18+1c0 = 142cd9cd8` | The installed Boolean primary vtable holds `1411e2870` Apply. `+1b8` holds `1411e25b0` Fill. |
| `1411e4f00` | Metadata accessor checks QObject private `+28` and returns its nonnull override; otherwise returns static object `142cde640`. The override is a real qualification, not an assumed null. |
| `142cde640` | Static metaobject refers to parent `143d938d0`, class string `142cde618 = SfObjectSetupBool`, and metadata table `142cde5d8`. Table words are revision6 followed by zeros, including local method count0. |
| `1411e4fb0`, call at `1411e4fda` | Derived qt_metacall only calls `140376bd0`, then returns the inherited method id unchanged. No derived slot case invokes Apply. |
| `140376bd0`, call at `140376bfa` | Intermediate qt_metacall delegates to the imported Qt function through IAT `141e0ba60`; it has no local slot cases. The retained SfWidget metadata contract independently gives zero local methods and QWidget parent. |
| `1411e37df`, `1411e37ec` | setupUi first calls retranslation `1411e3800`, then ordinary import `QMetaObject::connectSlotsByName(QObject*)` at IAT `141e04120`. This is not an Apply invocation. |

The carrier scan found the sole encoded absolute pointer to Apply at `142cd9cd8` and no `E8 rel32` candidates targeting it in either executable file section (`.text` and `text`). This is only a search for those byte encodings. It does not decode all instructions, close indirect or cross-module references, or establish that Apply is never called by some other route. The positive vtable and widget-area route below are the relevant evidence.

The existing `analysis/wave3-editor-base-metaobject-contract.json` qualifies the `SfWidget` static-metaobject initializer `141cdf810..141cdf848`, strings `142328000`, table `142327fc0`, and parent QWidget. `analysis/wave3-editor-qt-base-automatic-slot-metadata.json` separately retains original QtGui SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887` and QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`: QWidget has23 local methods, QObject4; neither has an `on_*` candidate. The mechanism is qualified by `analysis/wave3-editor-setup-translation-connection-contract.json`: `67130b60` filters `on_*` names, calls registration `671306d0` at `67130e92`, and does not activate the receiving slot on that registration path. This closes an automatic Apply candidate, not all transitive constructor effects.

## Registered owner and concrete invocation path

`SfWidgetArea` constructor `140d14d30` allocates its `SfWidgetAreaImpl`, stores it at area `+28` at `140d14dd3`, and calls registration `140d14e90` at `140d14ddc`. This is the main widget-area owner, not a new per-Boolean event queue.

Registration at `140d14ef1..140d14f08` passes the area's actual impl (`area+28`) and these exact PE literals to notifier registration `1402cfbe0`:

- `1429fd038`: `2signalUpdateDataInWidgetArea()`.
- `1429fd020`: `1slotUpdateData()`.

The same selected registration body also registers ShowWidget, UpdateUI, LeaveWidget and DeleteWidget. A registration is not an emission or callback delivery. This custom notifier registry must not be relabeled a Qt queued connection merely from Qt-style strings.

The retained impl Qt dispatcher `140deb760` admits seven local method cases. Its static dispatch `140deb5a0` local case3 calls `140dae340` at `140deb64d`. That handler resolves the actual current widget guard at impl `+18`, performs the native cast, and only with a nonnull result calls current widget vtable `+1c0` at `140dae3ab`. With Boolean's installed vtable this invokes `1411e2870`. It does not apply every previously allocated editor or use a timer-registration owner as a substitute for the current widget.

Source qualifications: `analysis/wave3-editor-native-owner-contract.json`, `wave3-editor-area-constructor-static.json`, `wave3-editor-widget-slots-static.json`, `wave3-editor-widget-metadata-static.json`, and `wave3-editor-widget-delete-static.json`. The new raw registration capture does not make the registry exhaustive against dynamically composed names or arbitrary external receivers.

## Earliest call along each selected lifecycle route

| Route | Earliest statically identified Apply point and remaining precondition |
| --- | --- |
| First Boolean editor construction | Constructor `1411e2480` calls base, setupUi at `1411e2529`, then Fill at `1411e2533`. The selected ctor/Fill bodies have no direct Apply or `140057ba0` engine add call. The known static metadata supplies no automatic Apply slot. This does not prove neutrality of all Qt constructor/property events, dynamic metadata or transitive event filters. |
| Open synchronous refresh/reselection | `038950 -> d3b300` sets row0 current and explicitly calls `d3b7d0(index,false)`. `038850 ->048640 -> current class +158=5f36d0` reaches the non-Process Object branch. **If an old editor is already shown (queried index !=0)**, UpdateDataInWidgetArea invokes that old editor's Apply before Leave and factory replacement. Index0 takes `0543a0` instead. Whether an initial Open selection/callback has already installed that old Boolean editor remains unresolved. |
| Delivered modelUpdated reselection | Nonnull model attachment requests a1000ms single-shot timer. Only actual timer delivery through `67134ca0` and selected view `183160` reaches then-current model/item selection and the same shown-editor branch. Fit precedes resolving model/item. The old editor is applied before a new ctor/Fill, so a later callback must use the refill result, not replay stale Add New data. Delivery/readiness and nested callback order are not inferred from a request count. |
| Save entry and event pump | Main Save calls `processEvents(flags0)` at `140098129` before current-operation Save. This can service already eligible events; it does not wait1000ms or prove timer delivery. Apply during that pump requires its real callback context. |
| Direct Save widget update | Base Save `1405ef510` calls widget update `+d0` at `1405ef53f`. Nonzero widget index in base `1405f1e20` emits UpdateDataInWidgetArea at `1405f22af`, reaching current-widget Apply before propsLoad at `1405f22c4`. After returning, outer Save unconditionally calls propsToEngine at `1405ef54a`. This is a source-ordered conditional Apply point, not a proof it is the first Apply of the session. |

These retained route qualifications remain authoritative: `analysis/wave2-db133-open-lifecycle.json`, `wave3-boolean-open-sequence-contract.json`, `wave3-boolean-reselection-contract.json`, `wave3-boolean-timer-contract.json`, `wave3-save-widget-order-contract.json`, and CP62 `resume62-boolean-timing.md`/vtable/ctor/Fill/Apply captures.

Once Apply is invoked with valid operation/Boolean data, workpiece selection is examined before cutter selection. The two native engine add call sites are `1411e2955` and `1411e2bc6`, respectively, conditional on selected text matching Add New Object. Object allocation in the editor/UI constructor, posting raw0x4a and registering the diagram timer are distinct operations. None can be promoted to those engine add calls.

## Safe successor

No public timing increment is justified by this lane. A candidate-only regression can enforce the already recovered partial order: shown old editor Apply -> replacement Fill -> later Apply of the new selection; the first eligible Add New action must occur once, and a replacement filled with the resulting object must not create a duplicate. The candidate must still require explicit callback delivery/current-widget evidence and must not mark Open's timer due from requests alone. Root owns any implementation/test allocation; this task changes none.

The smallest original observation, when the existing app-control release gate permits an owned disposable session, is a first-hit Apply breakpoint at `1411e2870` with the actual editor/current-widget identity, return address and bounded stack, correlated with Open entry/return, Save entry/pump/direct update and modelUpdated nesting. The two add call sites identify whether that invocation actually creates an engine object; record selection/returned object identity at those source sites. The decisive missing fact is the causal context of that first invocation, not another global queue count or screenshot. Current event-delivery, native HWND/serial/postedWake and nested-callback guards remain unchanged.
