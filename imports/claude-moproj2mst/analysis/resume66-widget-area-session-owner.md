# CP66: widget-area adoption, current editor and first Open emission

Static evidence now distinguishes the native main-window area field, its property-stack page, the area's impl receiver and that impl's current editor. These are four linked identities rather than aliases for the existing candidate MainView/diagram receiver. The adoption order is concrete: new editor layout/parent/show and Enter precede the impl current-guard assignment; the outer QScrollArea replacement follows. Boolean's Enter hook is an empty leaf, so that selected hook cannot supply the missing first Apply. Actual first fresh-Open UpdateData emission/current-editor population remains unbound. Keep the CP65 helper frozen and the public PostPending/timing guards.

No source, builds, tests, GUI, original application or mutable REA was used or changed. Only this report and unique bounded static evidence were added. Immutable original EXE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

## Main window owns a property stack, distinct from MainView

New `resume66-widget-area-adoption-pe.json` resolves the ordinary PE imports used by retained CP65 caller `140018790`:

| Native site | Established type/operation |
| --- | --- |
| `1400187d2`, IAT `141e0bb50` | `QStackedWidget` constructor, with parent argument null. The resulting stack is stored at caller parent `+480` at `14001882c`. |
| `140018888`, IAT `141e0bb58` | `QStackedWidget::addWidget` for the earlier central widget resolved through `1400138e0` and its virtual slot. |
| `1400188af` | Lazy area getter `140018460`, whose completed result is cached at the same parent `+3c8`. |
| `1400188ba`, IAT `141e0ba18` | `QWidget::setMinimumSize` on that area (400x300 operands). |
| `1400188c8`, then `1400188df` | Reuse lazy getter, then `QStackedWidget::addWidget` on parent `+480` with that area's pointer. This is not a QSplitter adoption. |

The known source order gives the same cached area to both calls when the first constructor completed nonnull. Actual Qt child/stack insertion effects, failure behavior and current-index changes still require their own continuation/admission. Do not infer a nonzero property-stack index solely from having an allocated or inserted area.

`resume66-widget-area-owner-links.json` independently preserves two different original metadata objects/classes:

- `1421705f0` -> strings `14216f2d0 = SfMoMainWindow`, table `14216e830`.
- `14216ae80` -> strings `14216ad50 = SfMoMainView`, table `14216ac60`.

Main-window primary table `1420fb108` begins with metadata accessor `1400dcb20`, captured here. It uses the dynamic metadata override when present, otherwise `1421705f0`. Retained `1400db070` is the separate MainView metadata accessor. These native types must not be collapsed into the candidate `MainViewState` just because both are Session-lived. The getter's parent `+3c8` and stack `+480` need their own actual main-window component/binding.

## The actual property-stack index query

Retained main registration `140011512` registers `2signalGetPropertyStackWidgetIndex()` to the main-window receiver's bare `slotGetPropertyStackWidgetIndex`, flag0. `resume66-widget-area-main-metadata.json` binds derived metadata local23 to the zero-argument method. Its static-metacall jump entry at `1400dc95c` targets `1400db63f`; call `1400db644` invokes `1400320c0`, then stores the returned int through the caller's nonnull return slot at `1400db66a`.

New captured `1400320c0` reads **main-window `+480`** at `1400320ce`, then calls IAT `141e0ba38` at `1400320d5`, independently resolved to `QStackedWidget::currentIndex()`. It does not read Boolean's candidate `shown` flag or the area's current-widget guard. CP64's native query dispatcher `1402cf740` explicitly invokes raw type1; this is a synchronous returned-value query, not a timer/dispatcher-readiness query.

The selected Save/reselection branches initialize their int to0 before this query and branch on the actual result. Both an installed current editor and the surrounding admitted nonzero stack-index path are therefore necessary before treating the corresponding UpdateData emission as reached. Missing query population cannot become a manufactured nonzero index or an empty registration list.

An overload boundary remains explicit: main registration `140011362` spells `1slotShowWidgetArea()` while the **derived** metadata local25 is `slotShowWidgetArea(QWidget*)`. Its local case calls `1400322a0`; that function eventually asks `140018ae0` to select index1, but it must not be assigned to the registered zero-argument emission just from the shared bare name. Inherited/default-argument method resolution must be qualified before claiming that exact ShowWidgetArea emission changed the stack page. Captured locals26/27 represent different Reshow methods. No overload guess is used to resolve first Open.

## Area/impl registration and incoming Boolean widget

Retained `wave3-editor-area-constructor-140d14d30.asm` proves area impl0x48 construction `140dad210`, area `+28` store, and registration call `140d14ddc ->140d14e90`. The registered UpdateData receiver is **this impl**, whereas the ShowWidget receiver wrapper is the area. CP63/64's exact registry literals and receiver setup remain authoritative.

The impl constructor `140dad210` stores its area pointer at impl `+28` at `140dad290`; current `+18`, saved `+20` and pending-delete `+38` are separate guarded-pointer members. The selected constructor explicitly assigns null to those guards at `140dad2d0`, `140dad2e3`, `140dad2f6` after their constructors. This is construction-point evidence only, not permission to reset a later live impl.

The same impl constructs a QTimer stored at `+40` and connects `2timeout()` -> `1slotTimerDeleteWidget()` at `140dad3a2` using raw connection type3. That timer is for delayed widget deletion; it is not the diagram's1000ms Boolean/modelUpdated timer. Raw type3's normalization is a separate Qt registration continuation, not an observed timeout. Widget lifetime/cleanup must preserve this separate ownership.

**Constructor-end placeholder qualification:** area constructor `140d14d30` does not stop after impl initialization/registration: it calls `140d14f70(nullptr)` at `140d14de8`. The impl constructor sets `+11=1` at `140dad3bd`. In `140dadc20`, null incoming with this flag admits `140dae030 ->140dad540`; that helper allocates0x38, calls the generic widget constructor `140377950` at `140dad578`, constructs its layout/UI and returns the widget at `140dadc0a`. The caller then runs the normal adoption/current-guard tail on that result. Consequently, the initial null `+18` store is **not** proof of a null current guard after area construction; the successful straight-line path attempts to install a generic placeholder. Actual allocation outcomes, lifetime/reentrant changes and native callbacks remain qualified.

The new placeholder capture also proves two direct `WidgetDockHideBtn` emissions at `140dad92a` and `140dadbf6`, with both exact key literals preserved in `resume66-widget-area-placeholder-pe.json`. No complete receiver effects are asserted for those emissions. This is another reason a returned area cannot be treated as receiver-neutral or already bound to a Boolean editor. CP65's opaque constructor-return seam correctly leaves those internals guarded.

On the selected Boolean factory route, `1405f4756` calls the actual factory and collects its returned widget. When nonnull, `1405f499f` synchronously emits exact `2signalShowWidgetInWidgetArea()` with the widget argument. A null factory result follows a distinct branch; neither case proves runtime invocation on first Open by itself.

Area metadata `140d488b0` cases0/1 invokes wrapper `140d14fc0`. The new wrapper capture proves it first calls impl `140dae230`, then `140d14f70`. The first call optionally saves/hides the old current widget when its bool is true and sets impl flag `+10` to the inverse of that bool. The second calls impl replacement `140dadc20` before Qt take/setWidget. That initial mode is distinct from directly applying an old Boolean editor.

## Exact current-editor adoption boundary

Selected `140dadc20` compares incoming raw widget identity with the current live guarded pointer using `140dae910`; the new leaf capture proves it returns **incoming != guard target**. A same-current incoming pointer skips distinct-old replacement work. Native QObject guard assignment wrapper `140cbc920` calls imported `QMetaObject::changeGuard` only when the stored raw value differs; the exact import is retained here.

For an admitted new incoming widget, the tail order is:

1. Configure its existing layout when present, with the source-specific geometry/margin calls still qualified.
2. Add it to the impl layout at `140dae18c`.
3. At `140dae1a6`, call imported `QWidget::setParent(incoming,impl+28 area)`.
4. At `140dae1b4`, call imported `QWidget::show`.
5. Cast the incoming widget to the expected editor base. On a valid cast call editor virtual `+1c8` at `140dae1f9` (Enter).
6. **Only afterward**, assign impl current guard `+18` to incoming at `140dae213 ->140cbc920 ->QMetaObject::changeGuard`.
7. Return to `140d14f70`; `140d14f96` calls imported `QScrollArea::takeWidget`, then `140d14fa6` calls `QScrollArea::setWidget(incoming)`.

Thus area current adoption is not a constructor side effect, and Enter must not observe a prematurely committed current guard in a staged projection. Qt layout/parent/show/scroll-area calls can dispatch transitive effects and remain guarded unless admitted; the exact final immediate viewport parent is not claimed from the EXE alone.

New vtable reads bind Boolean primary `142cd9b18+1c8` to `140131150`, and `+1d0` to `140131160`. New bounded Enter capture has `mov [rsp+8],rcx; ret` at `140131150..140131155`; later bytes are alignment padding. Enter itself has no Apply/addObject call. This is a complete tiny leaf, not a transitive claim about the preceding `show`/setParent/layout calls. Leave's previously qualified empty leaf remains separate.

Distinct-old replacement has earlier removal/detachment/hide/15ms timer/deferred-delete behavior, preserved by `wave3-editor-native-owner-contract.json`. Revoking old **current** admission must occur at the original transition even while its pending allocation lives. A candidate class-registry shared alias cannot represent that distinction.

## First Open emission remains a conditional reachability result

The actual shown-editor UpdateData emission is `1405f4524` in the selected non-Process Object branch of `1405f36d0`, after the property-stack query and before Leave `1405f4721` and new factory creation. With actual nonzero stack index and an admitted live impl/current Boolean widget, CP64 proves this direct emission invokes Apply synchronously. With index0, the selected path uses propsToEngine before factory instead; that is not this UpdateData/Apply invocation. No broader absence of engine mutation is claimed for the propsToEngine branch.

Open's explicit first-row click and a delivered modelUpdated callback can reach this branch. The first click can also create/fill the first editor through its factory, after which a later click may Apply the old filled snapshot. Static construction/adoption order does not establish that the initial Open refresh already installed that editor before the explicit click, that ShowWidgetArea selected the page, or that a second timer/callback actually ran before Open returned. Empty Boolean Enter closes one possible direct Apply route but does not close show/event-filter effects or callback counts.

The minimum missing observation remains the first admitted `1411e2870` invocation's actual editor/impl identities and bounded stack, correlated with initial Open selection, parent stack-index query, ShowWidget delivery/current-guard commit, Open return and modelUpdated nesting. It must await the existing controlled-session release gate. A timer request, cached getter return or historical screenshot cannot supply those values.

## Safe integration successor

The current candidate Session still has no typed area/impl/current-widget population; the CP65 isolated getter is not wired into it. The next source increment can stage adoption under explicit actual owners and an observed old-current profile, preserving the layout/parent/show/Enter/current-guard order and stopping at each unresolved native callback. A null-old-current test profile is bounded candidate work only; it must not be inferred from completed area construction because of the placeholder path above. It must keep these admissions separate:

- main-window parent `+3c8`/`+480` construction and page membership;
- area `+28` impl and its actual UpdateData registry entry;
- current editor lifetime/guard commit after Enter, with old admission revoked on replacement;
- actual property-stack index and admitted UpdateData notifier prefix.

Only their composition may mint CP64's receiver suffix admission. Do not mark the existing Session MainView as the main window, convert a private Boolean selection snapshot into a native widget owner, select a page from editor `shown`, or enable first-Open/PostPending while filling these records.

## Retained new evidence

- `resume66-widget-area-adoption-static.json`: wrapper `140d14fc0`, raw/current comparison `140dae910`, guard assignment wrapper `140cbc920`.
- `resume66-widget-area-adoption-pe.json`: exact import symbols, Boolean vtable slots and selected registration literals.
- `resume66-boolean-enter-static.json`: explicit16-byte leaf candidate at `140131150`; RET and padding qualified above.
- `resume66-widget-area-main-metadata.json`: selected main-window metadata methods and main registration literals.
- `resume66-widget-area-main-dispatch-static.json`: one selected static-metacall unwind region `1400db1a0..1400dcb14`; common epilogue `1400dc8f4` ends RET `1400dc8fc`, and jump-table data from `1400dc900` are not treated as executable cases.
- `resume66-widget-area-stack-static.json`: property-index query `1400320c0` and distinct Show/Reshow helpers `1400322a0`, `140032350`.
- `resume66-widget-area-window-meta-static.json`: actual main-window metadata accessor/metacast `1400dcb20`, `1400dcb80`.
- `resume66-widget-area-owner-links.json`: main-window/MainView static objects, exact query import and selected jump-table entries.
- `resume66-widget-area-placeholder-static.json` and `resume66-widget-area-placeholder-pe.json`: constructor's null-incoming placeholder helper and its exact WidgetDockHideBtn keys.

Each disassembly receipt records original hash, exact DUMPBIN command, per-file SHA-256 and bounds; metadata/import receipts state their immutable file-read method. Existing CP65/CP64 and wave3 source qualifications remain attached. No claim is made that selected unwind regions or carrier scans constitute a full callgraph.
