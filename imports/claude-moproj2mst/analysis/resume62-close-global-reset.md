# CP62: Close global resets and the post-busy status boundary

The inspected normal-Close tail does not justify a blanket process-global reset or Session destruction. After the false-busy setter, its next effect is a fresh global-main lookup for status reporting. A qualified null main skips both the status bar and message-area notification. A nonnull main enters Qt child/timer and notifier effects that remain distinct from Project cleanup and application-owner teardown.

This report and eight uniquely named bounded ASM captures are the sole writes. No source/shared/CMake changes, builds, tests, GUI, target execution or mutable REA operations occurred. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`. Both are retained original artifacts. DUMPBIN14.44.35228.0 is installed under MSVC14.44.35207. Static direct-body evidence does not establish arbitrary transitive callback coverage or current runtime observations.

## Direct tail after the false setter

Normal0299a0 first sends DeleteWidgetInWidgetArea at029c8d, then calls2ca9f0(false) at029c9b. CP60/61 qualify its ordered+9/counter resets, late main busy predicate and conditional TreeEnableSelection delivery. Once that setter returns:

1.029caa ->024d80 constructs a QString for literal1421000d8, `All projects have been closed.`.
2.029ce4 calls2d0bd0 on owner143d76350 with that message, integer timeout0, Boolean false.
3.2d0bef freshly obtains the current global main pointer via2c8090(global143d766b8). Null branches2d0c01 ->2d0c6b, skipping every status-bar, color and message-area effect. This must be a new lookup occurrence: earlier false-setter facts are not current observations after synchronous TreeEnableSelection receivers.
4.Nonnull main calls imported QMainWindow::statusBar at2d0c08. Then QStatusBar::showMessage(message,0) at2d0c3e. The alternate timeout==-1 branch maps to0, but normal Close already supplies0.
5.2d0c4e constructs QColor with Qt::GlobalColor raw value2.2d0c66 calls2d0f10 with the message, that color and the address of the false Boolean.
6.2d0f10 constructs QGenericArguments in QString/QColor/bool order and seven empty arguments.2d11c4 sends exact `2signalShowMsgInArea()` through shared notifier2cf3c0. Its suppression, flag90, ordered population/liveness and direct callback contracts apply. No proven-empty binding for this key is established here.
7.On normal return,029cf2 destroys the temporary QString and029cf8 loads true. The wrapper031f10 calls0299a0 and immediately returns, without an additional direct owner-delete or global-reset call.

The status path runs before ShowMsgInArea suppression is tested. A suppressed shared notifier therefore does not authorize skipping QMainWindow::statusBar or showMessage. The message text itself is emitted after earlier cleanup effects and is not evidence that those phases occurred in the candidate.

## Status reporting can change owners and timers

QtGui export QMainWindow::statusBar at653ca610 loads the actual main-window layout and checks its current status-bar result through653ccd80. A missing result takes allocation/constructor path653ca63f/653ca654, configures the widget and installs it via653c9c50. Thus this getter can allocate/parent/install a new status child. It cannot be replaced by a read of an invented existing pointer. Layout/child constructor/event effects and exact installation ownership are not closed by this bounded wrapper alone.

QtGui QStatusBar::showMessage at654069d0 first assigns the message to status-private+1e8. For timeout0,65406a07 takes the nonpositive branch. It reads the existing timer at private+1f8; nonnull invokes its deleting virtual+18 at65406a92, then stores null at65406a97. It then tail-calls65406480. The timer identity/destructor/Qt descendants and that later status update helper remain dependencies. Positive-timeout allocation/start paths exist but are outside this fixed Close timeout0 profile.

These bounded bodies establish why the nonnull status route remains guarded. They do not authorize changing the Session diagram timer queue: the status timer is a different owner, and its deletion is neither a blanket event purge nor dispatcher destruction.

## Which resets are actually established

| Boundary | Source-qualified state change | Required qualification |
| --- | --- | --- |
| False busy setter | global+9=0,143d91230=0; +A/+B=0/0 or1/1 from main busy; pending tree Boolean matches request | CP60/61. Tree delivery and receiver mode are separate from these bytes. |
| Earlier post-manager-clear02a0e0 | main+3c0=0; calls013d40(main,mode0,showBusy=false) | Native029a93 enters this before the late false setter. It is not another post-false reset. |
| 013d40 null-current-Project mode0 branch | writes application process type5 through2cb780 to global+0xc at013e61 | Old process type first selects03aeb0/03b230/03b590/03b920 callbacks. Actual manager/current Project lookup follows. Do not skip those effects and implement just the final store. This application process type differs from MoInputMode/cogging fields. |
| Project-specific cleanup | 0982c0 ->0aab30 calls12f1190(Project+60), then returns true | Detailed payload below is feature-specific, not demonstrated engine-wide global reset. |
| Final reporting | Fresh main lookup; null skips, nonnull status and shared notification; temporary QString release; bool true | No direct extra application-wide reset in the inspected epilogue/wrapper. Unknown notifier recipients are not excluded. |

There is no evidence here for clearing all engine globals, Fortran units/caches, notifier maps, graph registries, process remesh storage or all pending events. Absence from selected direct bodies is not whole-program exclusion of such transitive behavior. Preserve individual established mutations and guard unknown effects instead of adding broad reset work.

## Earlier Project feature cleanup, newly bounded

0aab30 passes Project+60 to12f1190, then unconditionally loads true.12f1190 visits29 fixed names in order, using the same owner and12f0da0 for each, with the final invocation as a tail jump. The sequence starts `2D_DEF_GUI_F2`, `2D_DEF_GUI_PRE2`, `2D_DEF_GUI_PST2`, continues the 2D machining/heating/microstructure/simulation/preform names, then 3D equivalents, and ends `3D_DEF_WZD_COG3`, `3D_DEF_GUI_SHAPEROLL3`, `3D_DEF_GUI_ROLL3`, `3D_DEF_GUI_EXTRUSION3`, `3D_FURNACE`. Exact addresses/order are retained in the capture.

12f0da0 initially calls12e76c0 with this feature name, an initialized output byte and fourth argument0. The following instructions compare the output byte and the returned state byte against r9b, skipping at the corresponding equal branches; the callee's register/data contract is not closed here. On continuation it calls12e5780; negative result skips. It then accesses an owner+1458 keyed structure, conditionally removes its entry and invokes the entry's deleting virtual. Further context/provider/error paths follow; only its success path at12f100b stores the returned state byte false. This is not a call to zero the entire Project/context/global region.

The feature-name vocabulary and keyed state strongly suggest license/feature-release work, but a complete licensing semantic identity, external provider behavior and all map/payload cleanup are not established here. That interpretation must not be promoted to a no-op or successful feature check-in contract. The candidate Project has no source-qualified owner projection for this native Project+60 context, its+1458 entries or per-feature flags. Keep the actual Project-cleanup seam guarded. The unconditional true return from0aab30 does not permit skipping its preceding effects.

## Project Close versus Session/application teardown

Earlier retained contracts distinguish real model/Project destruction from native main/view-manager QObject ownership. `wave3-normal-close-diagram-lifetime-contract.json` qualifies Project diagram-null rebind while the receiver remains main-window-owned; this does not cancel registered singleShots. `wave3-close-shared-view-owner-teardown-contract.json` qualifies MainView private storage release, ViewManager guard-member destruction versus target destruction, and Qt base destructors. App wrapper order does not establish descendant callback/destruction order.

Current `ConversionSession::~ConversionSession` expressly performs adapter cleanup: release Project storage, destroy diagram receiver/dispatcher, release MainView native owner and adapter task allocation/factory scopes. `releaseProjectAfterCloseEffects`/`releaseProjectStorage` are narrower Project storage transitions. Normal Close must not call the Session destructor as an implementation shortcut: it would replace retained main/receiver/process owners with adapter teardown, before their native boundaries have been established. CP59/61 global helpers remain declared isolated state projections, not a production owner or a shared actual true/false lifecycle binding.

## Smallest implementable next contract

Add an isolated status-entry continuation at exact `0x140029ce4:0x1402d0bd0(normal-close-status)`. Bind its progress to the live Session/actual manager identity, and require fresh occurrence-qualified global-main presence evidence. Missing evidence stops before the lookup outcome is assumed. Qualified null main reaches `NullMainStatusSkipReached` without changing status text, color, notifier+90/map/population, tree mode, Project/current, receiver or pending timers. A qualified nonnull main must stop before QMainWindow::statusBar at2d0c08 until its actual layout/status-child owner and Qt effects exist. Do not infer null from a null model or reuse a MainView diagnostic lease as the native global-main alias.

The continuation must stop before the caller's native QString temporary-release/public-return boundary, or clearly separate an adapter-owned string release from a native Qt refcount/destructor claim. It must not report full Close success, repeat earlier false writes, bypass TreeEnableSelection/earlier teardown or clear process globals. A later nonnull status driver should retain status-child lookup, message assignment, timer deletion, null store, status-tail call and ShowMsgInArea stages in that order, with no rollback of committed effects.

Focused future tests: missing versus qualified null/non-null main; late occurrence and Session mismatch; null skip leaves all actual Session/manager/notifier/timer state unchanged; repeated attempts retain a consumed lookup outcome; changed later global-main facts do not replay a completed status invocation; nonnull guard precedes any child/status/log mutation. A separate nonnull fixture must require status-bar effects even with notifier suppression. No tests were created or run in this evidence lane.

## Retained raw evidence

All new captures use `/nologo /disasm /range:<entry>,<end-minus-one>` and end-exclusive unwind bounds. No decompiler names are used as sole behavior evidence.

| New raw capture under analysis/ | Code bounds | Raw artifact SHA-256 |
| --- | --- | --- |
| resume62-close-140031f10.asm | 140031f10..140031f28 | 9afe5812ed9d8126d71122b60ba5ad6b1154135ad93963e8e22c74d9736cc9a5 |
| resume62-close-1402d0bd0.asm | 1402d0bd0..1402d0c70 | eea6c0b6de70c74c9190f62366e7f854187ac7246259e87622624095ca28a1ec |
| resume62-close-1402d0f10.asm | 1402d0f10..1402d11d3 | bee0db456cc60e33f8ccd1f40e7f973b459c2065c0d238bbfdbd5aab7a5a1e7a |
| resume62-close-1400aab30.asm | 1400aab30..1400aab4e | 1b217bd31bc746c0e5ca43b654dec504fb6d6030ab501b99fb85ef249ceb3f9f |
| resume62-close-1412f1190.asm | 1412f1190..1412f134e | 4d478ed18d6bd7606f4f017afc6dec9e6d0655c5aab2925cf7572858c5d143dc |
| resume62-close-1412f0da0.asm | 1412f0da0..1412f105f | e40971943e0104a0af1142870e1b579a70e82479ee2fc698e19236119071a93b |
| resume62-close-qt-653ca610.asm | QtGui653ca610..653ca686 | 5042132e48600edf5683cc86e799cb592bb55f93d78b5017c93d916473c2ff09 |
| resume62-close-qt-654069d0.asm | QtGui654069d0..65406aba | 4c1e830019ac4247454b0efce93187876c468b84131e1a57273a59a799942747 |

Code-byte SHA-256:2d0bd0 `dd2b8b8ab71a2e3c4b929b606b849f0d8de29abd08925c265eee5e8749cef785`;2d0f10 `40708a63facfb8e15f886a8d08fe2ec36ad136432e51d49a9cdba9b6471ea41f`;12f1190 `95532f69394a3f61e1eeb5cd30ff19a2cdc261bd535e67af06dc5fbbf2aff574`;12f0da0 `4510480e79a2108f866cc8d297de121bc75ad4ee42dc9045cf3bfa7a22e3c167`;Qt statusBar `35de2cdad5e969d9d7dfd441005fdb7fe269cdd95a08fdc7a025df236eafe797`;Qt showMessage `49265ae80cb189c722bb11a7125d2a395264e141de45db6f3b23e44b31f26b65`.

PE imports bind141e089d8=QMainWindow::statusBar,141e089d0=QStatusBar::showMessage,141e0bfb8=QColor(GlobalColor). QtGui imports additionally bind65696780=QString::operator=,65697c88=QTimer constructor,65697c68=QTimer::start and65696718=QObject::connect. Earlier process-type evidence is `wave3-closure-next-140013d40.asm`, `wave3-closure-next-1402cb780.asm` and `wave3-closure-next-close-global-reset-static.json` (artifact SHA `6da570e7280fd899803f2c49cda37d1f0a2b501a9c98482dad3471443fcfa6a8`). Remaining unknowns are explicit dependencies above; normal Close and process teardown parity remain unclosed.
