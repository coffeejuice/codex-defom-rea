# CP66 adoption source lane: report-only boundary

No adoption helper or test source was created. The assignment explicitly permits a bounded report when precise old-current/Qt callbacks cannot be modeled; that condition applies. Native order is established, but the existing candidate does not have the QStackedWidget/QScrollArea, area impl/current guard or placeholder replacement effects required to commit this prefix faithfully. Adding callback-return flags or a local pointer token would record admission without modeling those effects and could falsely certify a real current editor.

Only this report was written in this source lane. The proposed new `widget_area_adoption_prefix.hpp`, `.cpp` and test files remain absent/unclaimed for later implementation. CP65 lazy helper files and public Session/Boolean/PostPending paths are unchanged. No build, compiler, CTest, GUI, original runtime or mutable REA ran.

## Established native order

Source qualifications and exact receipts are in `analysis/resume66-widget-area-session-owner.md`; original EXE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

| Prefix | Native order and effect boundary |
| --- | --- |
| Main-window area page | `140018790` constructs QStackedWidget and stores parent `+480`; initial central addWidget at `140018888`; lazy area getter at `1400188af`; QWidget::setMinimumSize400x300 at `1400188ba`; cached getter again at `1400188c8`; addWidget(area) at `1400188df`. Calls and parameter identities are qualified, but actual Qt page/parent/current-index callbacks have no corresponding complete candidate component. |
| Area constructor tail | `140d14de8` calls `140d14f70(nullptr)`. Impl flag `+11` was set1 at `140dad3bd`; `140dadc20` null-incoming path calls placeholder factory `140dad540` at `140dae030`. That helper constructs a generic widget, UI/layout and emits WidgetDockHideBtn at `140dad92a`/`140dadbf6`. Constructor-end current cannot default to null from its earlier initialization. |
| ShowWidget mode prefix | Area wrapper `140d14fc0` calls impl `140dae230` before replacement. True mode can save current into `+20` and hide it; impl `+10` becomes inverse mode. The mode must be bound from the actual selected invocation, not guessed from the signal's bare name. |
| Distinct old-current replacement | `140dadc20` compares incoming against the actual guarded `+18`. A distinct live old widget is removed from layout at `140dadc91`. With impl `+10!=0`, copy old to pending `+38` at `140dadcc3`, detach at `140dadcde`, remove again at `140dadd04`, start timer15 at `140dadd1b`, hide at `140dadd35`, execute notifier/deferred-delete effects, then clear current through `140dadfec`. The other flag branch hides old at `140dae007`. Old lifetime/current status and receiver effects differ. |
| Incoming adoption | Configure existing layout when present; add to impl layout at `140dae18c`; setParent(incoming,area) at `140dae1a6`; show at `140dae1b4`; cast and Enter virtual `+1c8` at `140dae1f9`; **then** changeGuard(impl `+18`,incoming) at `140dae213`. |
| Scroll-area suffix | After impl returns, wrapper `140d14f70` calls QScrollArea::takeWidget at `140d14f96`, then setWidget(incoming) at `140d14fa6`. The native immediate final viewport parent is not closed by EXE call names alone. |

Boolean Enter resolves to the captured empty `140131150` leaf, so its own body needs no invented effect. That does not make preceding layout/setParent/show callbacks neutral. They may run code before current-guard commit. A staged implementation must preserve the original assignment order and must not introduce rollback or extra source-absent revalidation after callbacks as a substitute for their effects.

The actual property-stack query is separately bound: metadata local23 -> `1400db63f` -> `1400320c0`, which reads parent `+480` and calls QStackedWidget::currentIndex. An area current widget and a nonzero stack page are different state. The registered no-argument `slotShowWidgetArea()` is not automatically the derived metadata overload `slotShowWidgetArea(QWidget*)`; inherited/default-argument resolution remains qualified. No source increment here enables that emission or first-Open Apply.

## Why current candidate components are insufficient

`engine/view_manager.hpp` and `widget_events.hpp` provide WidgetState/WidgetRuntime, guarded parent/grid constructor stages, stack event dispatch and posted-event ownership. They do not expose a QStackedWidget page/current-index component, QScrollArea take/setWidget component, actual SfWidgetArea impl, or the old/current/saved/pending-delete guard collection above. The existing parent continuation enums explicitly stop at visibility/child/native-effect dependencies; they cannot be treated as completed generic QWidget::show/setParent operations.

`ConversionSession` holds MainView/ViewManager and the shared notifier, while native SfMoMainWindow and SfMoMainView have distinct metadata and fields. Its Boolean editor is still a private class-owned selection value reset by Fill, not a separately lived attached widget. CP64's synchronous suffix already requires externally admitted actual receiver/current-editor owners and does not manufacture these components. CP65's lazy getter deliberately guards impl/registration even after an observed constructor return.

Therefore neither an explicit `oldCurrent=null` flag nor an opaque placeholder identity supplies native replacement behavior. A null profile is a valid synthetic isolated scenario only when source-point admission establishes it; it is not the successful fresh area's default. Placeholder profiles require their own live guards, mode flags, pending lifetime and notifier/timer effects. No exact implemented prefix reaches an actual current-guard commit with those effects represented today.

## Minimal implementation dependencies and future focused tests

The next shared component increment must retain actual main-window/stack/area/impl identities and distinguish current `+18`, saved `+20`, area `+28`, pending-delete `+38`, timer `+40`, and flags `+10/+11`. Initial observations need exact old-current kind (unavailable/null/placeholder/other), old identity/lifetime and mode state at the selected call; an unknown population cannot become empty.

Before adoption can complete, source-qualified Qt stack insertion, incoming layout/parent/show and scroll-area continuations must either run their recovered candidate effects under those real owners or stop at the first unresolved effect. Placeholder replacement must preserve deferred ownership and actual source-order notifications. Merely accepting “callback returned” cannot be reported as candidate implementation of its effects. Surrounding ShowWidget emission, property-index query and first-Open event timing stay independently guarded.

Once these prerequisites exist, focused Release-active tests should establish:

- Parent `+480` owns the actual page population/current index; adding the area does not automatically select it.
- Incoming Enter observes the source-appropriate prior current state; current `+18` changes only after Enter returns. Callback throws before commit leave it uncommitted without invented rollback of earlier effects.
- Distinct placeholder/current replacement retains pending allocation while revoking old current admission at the native point; later timer/deleteLater delivery is explicit.
- Same-current, null incoming/placeholder creation, old-mode branches and allocation failures follow their separately qualified source paths.
- A second Session or same-address/new-lifetime owner cannot reuse adoption admission, and adoption alone cannot mint an UpdateData/first-Open emission.

No test source was added because there is no implemented adoption contract to validate beyond the already existing CP64/65 isolated tests. Root may allocate the missing shared owner/Qt effect work without changing this lane's frozen files. The public guard remains `Constructor raw0x4a posting requires the actual shared queue/dispatcher continuation`.
