# CP77: main-window Qt address lineage closes, current area value does not

Static PE evidence closes the primary-address lineage of the owner of `+3c8`: the selected application-frame SfMoMainWindow object is the same primary-address QMainWindow/QWidget/QObject instance through its constructors. It is not the MainView, QPaintDevice secondary subobject, Boolean editor or area allocation. This permits a correctly typed **conditional identity binding**, but supplies neither an actual candidate constructor cursor nor a current area value. Public first-Open behavior remains guarded.

Only this report and bounded unique immutable-PE evidence were written. No source/build/tests/GUI/native execution/mutable REA. Original EXE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.

## Exact owner and primary Qt alias

1. Retained application1410 body loads RCX=`&[rsp+40]` at140001b34 and invokes SfMoMainWindow ctor10490 at1b39. The selected window is an application-frame object; no Boolean heap allocation owns this field.
2.10490 preserves/reloads that `this` without adjustment and calls base140c98b00 at104d2. The base preserves the same incoming pointer and passes it unchanged as RCX atc98b47 to IAT141e096b0 atc98b4f. The import is exactly `QMainWindow(QWidget*,QFlags<WindowType>)`.
3. New QtGui export resolution binds that constructor to653ca3f0. It retains incoming `this` in RBX at653ca40b, allocates its separate private block0x218 at653ca40e..413 and constructs that private block at653ca425. It then passes original RBX unchanged to QWidget protected ctor65070f00 at653ca442..445. The private allocation is not the window object's address.
4.65070f00 retains incoming `this` in RDI at65070f21. Its first base call at65070f27 passes that unchanged RCX to IAT65696a30, positively resolved as protected `QObject(QObjectPrivate&,QObject*)`. Its QPaintDevice base construction instead uses `this+10` at65070f2e/32. Primary and secondary vtables are installed separately at65070f3f and65070f49. QWidget private state is read through `this+8` at65070f5b, then init6506e4d0 is called.
5. QMainWindow later installs its primary and `+10` tables, invokes its private initializer653c9850 at653ca464 and returns the original RBX at653ca46a. SfMainWindowBase and derived SfMoMainWindow subsequently replace their own primary/secondary tables. None of this selected chain adjusts the primary `+3c8` base address.

The actual Qt QObject receiver identity and native window field owner therefore use the same primary address on this constructor path. An eventual candidate window value may bind one WidgetState to its canonical primary owner and keep paint-device views at their appropriate secondary relationship. A MainViewState's WidgetState cannot stand in for this object. Merely allocating such a projection does not execute QMainWindowPrivate, QObject/QWidget init, native registration, class/vtable effects or callbacks.

## The initializer is not a pure prefix ending at184fa

New direct vtable pointer reads from1420fb108 establish:

- `+2c8` at1420fb3d0 ->140010aa0.106d0 calls this slot at106e6 **before** the null area leaf.10aa0 does not use the window address for a field store: it calls140040810 with global143d768f0 at10ab0. New bounded body40810 calls808270/808860/8085c0 andcc9be0 on that global context. These descendants are not recovered as neutral here.
- `+1b8` at1420fb2c0 ->140caa9e0.106d0 calls it at10774, then calls QWidget::hide on its result at10784 and connectSlotsByName on the window at1078f, all before1080c ->184f0. The bounded caa9e0 body requests0x28 storage, constructs a **QToolBar** via IAT141e0bcd8 atcaaa1c, performs additional configuration/callback-capable work, and calls QMainWindow::addToolBar via141e0bb00 atcab151 before returning. It is not an empty getter or a preexisting MainView alias.

There are still earlier QMainWindow/QWidget construction effects and other initializer/helper calls before the source null store. The address lineage does not authorize omitting them, moving184fa to Session allocation/Open, or assuming they cannot reenter. The selected184fa leaf remains a single callback-free store once reached; its caller's surrounding effects remain the exact reachability dependency.

## Source-closed transition and remaining witness

The newly source-closed transition is an **identity projection**: for an admitted actual main-window construction occurrence, its primary QObject/QWidget role and its `+3c8` cell share one canonical window allocation/lifetime; its private block and `+10` paint-device base are distinct. CP75's shared cell can carry this identity without importing a captured raw pointer or inventing fields. No fresh GUI capture is needed to implement this conditional type/address relationship as an isolated primitive.

This is not a new public field-state transition. A candidate constructor cursor reaching184fa could commit null with its owner/occurrence guard, but the currently retained identity evidence does not execute the preceding Qt/global/toolbar effects. Alternatively an actual observed getter read18477 can populate the current value directly. CP76's bounded original-probe survey found no such owner/value operand observation. CP75's original ctor return (same owner before selected CLI Open) establishes an earlier initializer traversal only; later setup can have populated the area already.

Exact remaining witness: actual primary window owner/generation and reached18477 invocation, RAX==saved`[rsp+50]`, raw8-byte `owner+3c8` value, caller`[rsp+48]` and executed compare/branch, as detailed in CP76. For initializer timing instead, use selected184fa/18505 with caller continuation10811 and ordered occurrence. Nonnull area still requires actual allocation/lifetime-to-typed-area mapping; area/impl registrations and first current-editor adoption are later effects. No timer/HWND/postedWake input supplies this field value.

No new initialization/Apply activation follows from this lane. The smallest independent implementation opportunity is the isolated primary-owner/WidgetState identity bridge with all native field/liveness state unknown, stopping before base/private constructor effects. Root must assign ownership/interface work before implementing it; this report changes none.

## Evidence receipt

- `resume77-boolean-primary-lineage-imports.json`: selected immutable EXE/QtGui import identities and positive primary-vtable slot values.
- `resume77-boolean-mainwindow-qt-export.json`: exact QMainWindow exported entry and binary hash.
- `resume77-boolean-qmainwindow-ctor-static.json` /653ca3f0 asm: primary pointer/private block/QWidget call.
- `resume77-boolean-qwidget-base-static.json` /65070f00 asm: primary QObject and secondary paint-device roles. This bounds the already retained constructor body with a unique lane receipt; no runtime observation is claimed.
- `resume77-boolean-mainwindow-prefix-static.json` /10aa0 andcaa9e0 asm, and `resume77-boolean-early-init-static.json` /40810 asm: selected initializer descendants and unresolved preceding effects. Unwind-region captures are bounded bodies, not complete transitive callgraphs.
- Retained `wave3-root-scene-order-window-base-140c98b00.asm`, CP71 constructor/initializer, CP75 owner-return receipt and CP76 survey retain their original qualifications. Source remains frozen.
