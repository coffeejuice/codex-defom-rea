# CP70: ShowWidget overload and first Boolean Apply

The selected factory path's ShowWidgetArea overload is now resolved: despite `()` in the custom registration/key literals, it sends one typed **QWidget pointer** and reaches derived `SfMoMainWindow::slotShowWidgetArea(QWidget*)`. The preceding area-delivery emission likewise supplies one QWidget pointer; SfWidgetArea's cloned one-argument slot supplies bool false, selecting delete-old mode. These static facts narrow the actual source order, but do not prove the first fresh-Open callback/Apply occurrence or implement its Qt effects. Keep first-Open/PostPending guards.

Only this report and unique bounded immutable-PE evidence were added. No source, compiler/build, tests, GUI, original execution or mutable REA was used/changed. Original hashes are EXE `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtCore `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

## Correction to the earlier overload dependency

CP66/67 left open whether registration literal `1slotShowWidgetArea()` meant a zero-argument inherited slot rather than derived metadata `slotShowWidgetArea(QWidget*)`. The missing input was **the emitter's QGenericArgument vector**, not another inferred method with zero parameters. The custom notifier strips a registered slot to its bare name, and Qt builds the invocation signature from the emitted argument types. The `()` in these application-specific keys is not an arity declaration.

The retained `wave3-editor-factory-delivery-1405f36d0.asm` establishes:

| Source site | Exact selected payload/order |
| --- | --- |
| `1405f4756..1405f4759` | Factory virtual+8 returns a widget into local `[rsp+98]`. Nonnull route emits ShowWidgetInWidgetArea at `1405f499f`; the alternative factory-result branch remains separate. |
| `1405f4770..1405f4787` | Builds a QGenericArgument using type literal `1424977e0 = QWidget *`, with data address `&[rsp+98]`. The following generic arguments are default empty arguments. |
| `1405f499f` | Custom notifier key `1424977f0 = 2signalShowWidgetInWidgetArea()` with that typed widget payload. |
| `1405f4c33..1405f4c4a` | Again constructs first QGenericArgument from `&[rsp+98]`, now with literal `142497868 = QWidget *`; subsequent arguments are default empty. |
| `1405f4e4c..1405f4e62` | Passes the first argument vector to key `142497878 = 2signalShowWidgetArea()`. This has one QWidget* argument even when its pointer value is null, not zero arguments. |

New `resume70-boolean-show-arguments.json` preserves those literal bytes and imports. New helper capture `140025080` forwards its type/data operands to IAT `141e04168`, explicitly imported `QGenericArgument(char const*,void const*)`. It then returns the constructed argument value. No inferred GUI action is used to identify the payload.

Retained registration `140011362` binds the actual main-window receiver to bare `slotShowWidgetArea` through `1402cfbe0`. CP64's qualified notifier body forwards the argument vector to raw invocation type1. Retained Qt `67121de0` appends actual QGenericArgument names between parentheses (`67121f02..67122178`), calls `indexOfMethod` at `67122190`, and, on a negative result, calls `normalizedSignature` at `671221a8` and retries lookup. New export receipt independently identifies `671211e0` as QMetaObject::indexOfMethod and `6711ff60` as normalizedSignature. Thus the emitted `QWidget *` spelling selects the normalized QWidget* method under the known static metadata profile.

SfMoMainWindow table `14216e830` local25 is `slotShowWidgetArea(QWidget*)`. Its selected jump-table entry targets `1400db680`, which reads the pointer argument and calls `1400322a0` at `1400db694`. This resolves the selected emission's derived overload without relying on a default/inherited zero-argument method. Actual dynamic metadata override, live receiver population, suppression and argument ownership remain runtime/source-bridge inputs.

For completeness, new bounded base metadata analysis identifies `SfMainWindowBase`: accessor `140cc74e0` returns its dynamic override or static object `1446b2670`. The object is not file-backed initialized data; initializer `141d51880..141d518b8` supplies QMainWindow parent, strings `142916840`, table `1429159f0` and extra data. That180-method table has no WidgetArea/PropertyStack names. This is a bounded static profile check, not an exhaustive absence claim about arbitrary runtime metadata. The positive QWidget* payload route above is sufficient and authoritative for this selected emission.

## Area delivery selects a concrete old-current mode

New `resume70-boolean-area-methods.json` and initializer capture `141d574b0..141d574e8` bind SfWidgetArea metadata to table `142a132a0`, strings `142a13310`, QScrollArea parent and two methods:

- local0: `slotShowWidget(QWidget*,bool)`.
- local1: cloned `slotShowWidget(QWidget*)`.

The selected one-widget payload reaches local1. Retained static metacall `140d488b0` case1 reads that argument, sets r8d=0 at `140d48923`, then calls `140d14fc0`. The wrapper calls impl `140dae230` before replacement. Its false-mode branch does not save/hide current via `+20`; it stores inverse mode **`impl+10=1`** at `140dae2b2`.

Consequently, when the actual current guard holds a live placeholder different from the new Boolean editor, this selected factory delivery uses the delete-old branch of `140dadc20`: remove current, copy it to `+38`, detach/remove/start15/hide, native notifications/deleteLater, then clear `+18` at `140dadfec`. Incoming layout/add/parent/show and empty Boolean Enter follow, and new `+18` is assigned at `140dae213`. Old allocation lifetime and pending guard remain distinct. If the current guard is null, same incoming, expired or changed by callbacks, the corresponding actual branch must be bound; do not manufacture the placeholder's existence from the mode alone.

The placeholder prerequisite is unchanged: area constructor `140d14de8` calls `d14f70(nullptr)`; initial impl `+11=1` can invoke placeholder builder `dad540`, which includes real QWidget/layout/UI work and two WidgetDockHideBtn emissions. A completed opaque lazy getter is not proof of a null old current. None of those effects is implemented by CP68 typed field observations.

## Property page selection after widget delivery

Derived ShowWidgetArea handler `1400322a0`:

1. Calls dock/helper effects `140cef3f0` and `140cf2c10` using the actual widget and main-window `+478` owner; their full callback effects remain a dependency.
2. Calls `140018ae0(1)` at `1400322e4`.
3. Checks widget/base cast; if successful reads byte `+34` through `14003f5e0`, otherwise its local default remains true.
4. If that flag is true, calls `140032600` at `140032339`, then returns.

New `140018ae0` capture proves it loads main-window `+480` and calls imported QStackedWidget::setCurrentIndex at `140018afd`, preserving the index operand. New import evidence identifies that function. This closes the **request to select page1**, not Qt's page/child/currentChanged effects or proof the index remains1 after callbacks.

The new `14003f5e0` leaf ends RET at `14003f5ed` and only reads byte `this+34`. Retained generic SfWidget constructor `140377950` initializes that byte1 at `1403779c4`; this is constructor-point evidence, not proof an actual live widget's field was never changed before Show. The conditional central-close path in `140032600` depends on actual main-window `+270` state, other stack/widgets, virtual calls and `140036580`; its final tree call at `1400326ae` is imported **QAbstractItemView::setSelectionMode(1)**, not an Apply call or an explicit TreeItemClicked emission. Its transitive Qt/callback effects remain qualified.

Another useful source point is initial main-window area setup: retained `140018790` calls `1400320f0` at `140018a93`. New captured `320f0` requests `140018ae0(0)` at `14003227f`, then calls32600. It chooses initial property page0 at that constructor stage; it does not prove the page still0 at a later Open click. Later ShowWidgetArea requests1. Current editor identity and property-stack index remain different cells; native query `1400320c0` reads currentIndex from actual `+480`.

## Immediate Apply timing and first fresh Open

The selected `5f36d0` outer source order remains:

1. Query actual property-stack index. For non-Process Object with old editor shown/index!=0, emit **UpdateData at `1405f4524`**, then Leave `1405f4721`.
2. Create/fill replacement at `1405f4756`.
3. Deliver ShowWidgetInWidgetArea and its adoption path.
4. Deliver ShowWidgetArea **with that widget payload**, requesting page1.
5. Emit UpdateTreeViewCheckButtons at `1405f505f`, then return.

Once step1's actual gate/receiver/current widget admits it, CP64 proves Apply completes synchronously within UpdateData. The workpiece/cutter Add New branches inside Apply retain sites `1411e2955`/`1411e2bc6`. The later Show handlers are not another explicit UpdateData call in this selected body, and Boolean Enter is empty. This does **not** prove they are transitively neutral: dock, layout, parent/show, stack currentChanged, filters or central-close descendants may reenter. No full negative callgraph claim is made.

If the first admitted tree selection starts at actual index0, the selected branch takes propsToEngine rather than UpdateData before factory creation. A later admitted selection can now find the installed Boolean editor and page1 and Apply it. Whether fresh Open supplies that earlier first selection and another immediate/queued callback before Open return remains unresolved. Requested1000ms diagram timers are not deliveries, and native Save's flags0 pump supplies no fixed wait. Only a causal first Apply/current-widget/callback observation can establish original first-mutation timing.

## Candidate owner state and exact integration prerequisites

CP68's typed owner skeleton is available: distinct window area/stack, scroll content, impl current/saved/pending guards, explicit unknown liveness/kind/mode and source-observed branch writes. It still unconditionally refuses adoption effects. Merely retaining those records under Session does not implement the following prerequisites:

| Needed before the selected public path can use CP64 Apply | Exact source-bound requirement |
| --- | --- |
| Actual ownership | Native SfMoMainWindow identity, actual `+3c8/+480/+478` fields, area/impl back-references, distinct placeholder/new widget lifetime and shared WidgetRuntime/thread. MainView and diagram receiver cannot substitute. |
| Registrations/payload | Actual shared notifier population, bare-method normalization, widget QGenericArgument data ownership, static/dynamic metadata binding, receiver liveness, suppression/flag90/list snapshot/order. The one-widget overload may now be projected for this qualified source path; the key's parentheses must not drive arity. |
| Old-current replacement | Actual source current/kind, clone false-mode `+10=1`, full selected removal/deferred-delete effects, explicit placeholder lifetime and current clear atdadfec; incoming callbacks and current store atdae213. CP68 observes these fields but does not execute their effects. |
| Stack/dock/central effects | Actual QStackedWidget page population and selection/currentChanged path, dock helpers, widget+34 and main-window+270 contexts, source virtual recipients and tree selection-mode effects. No page-selection completion or reentrant absence may be inferred from a requested index. |
| UpdateData admission | Actual eligible stack-index query and specific emitted occurrence, registered impl/current Boolean widget/cast, source Project/Operation/model binding and full enclosing driver ordering. Typed liveness alone does not identify an emission or complete a GUI callback. |
| First Open | Actual first factory/current adoption and the next callback/selection/Apply nesting relative to native Open entry/return. Preserve original timer/dispatcher observations rather than using elapsed time or output state. |

At this inspection, `conversion_session.hpp/.cpp` still had no widget-area owner member/accessor or actual area registration binding; only the staged Editor300/future-area boundary was present. If a Session holder lands concurrently, it should be read as storage retention until the prerequisites above are source-qualified. Inspected Session hashes were header `6b05d5d085ff23b42e7a3f1c784ea5e92576945504e471d1dda29d44abb17a99`, source `99731a89662bac4b9162c22f0bbb649da3e65c64a5431c96048060774b4ab8a3`. CP68 header/source hashes remained `6e4e4ca3c8b2fe373f8044f7db63b63dd1974006cf0443e5e2e60aa4651788a3` / `b77f84438ccc562174ee831bc15435ded548ce34245ae3964043663d5cf98b19`.

The smallest safe next increment is the selected **one-widget payload/clone-mode** projection with actual typed receiver/widget aliases, retaining gates at the first missing Qt effect. No inherited zero-argument-slot search is needed for this emission. Do not automatically select1, clear old, complete Enter/Qt callbacks or invoke Apply from typed record construction. Public raw0x4a PostPending remains unchanged.

## New evidence receipts and qualification

- `resume70-boolean-show-arguments.json`, generic-argument disassembly/receipt: exact emitter literals/import and helper forwarding.
- `resume70-boolean-invoke-exports.json`: Qt indexOfMethod/normalizedSignature exports; the actual signature-building/direct-invoke path remains in CP64's retained Qt captures.
- `resume70-boolean-area-meta*`, `resume70-boolean-area-methods.json`: area static accessor/initializer, two signatures and cloned one-widget mode, selection imports.
- `resume70-boolean-base-meta*`, `resume70-boolean-base-methods.json`: positive base-class accessor/initializer/table qualification, not a runtime object read. RIP-relative candidate scan is only a carrier search; disassembly confirms the initializer writes.
- `resume70-boolean-stack-selection-v2-static.json` and three captures: index setter, central-close helper and byte34 leaf. The first capture attempt retained the first two asm files before stopping on the unwind-less byte34 leaf; v2 supplies the complete receipt and bounded leaf qualification. No partial artifact was overwritten or silently treated as complete.
- `resume70-boolean-show-property-static.json`: selected initial page0 request body.

Disassembly receipts contain exact commands, hashes and unwind/leaf bounds. Tiny leaf RET/padding boundaries and dynamic static-object initializers are qualified above; raw metadata/import receipts record immutable file-read methods. This lane establishes no original runtime owner values or whole-program negative callgraph result.
