# CP63: nonnull status ownership and message-area delivery

The nonnull path needs an actual main-window layout/status-item owner before it can safely advance. CP62's supplied main-present observation does not supply that owner. The smallest closed next prefix is the layout's qualified null status-item read followed by the allocation guard; an existing item requires its virtual widget lookup and QStatusBar cast before showMessage may execute. The later message-area receiver has a separate process-global duplicate-message cache, which must not be stored on the status bar or cleared as normal Close cleanup.

This report and17 uniquely named bounded ASM captures are the sole writes. No source/shared/CMake changes, build, tests, GUI, target execution or mutable REA occurred. Root may build independently. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtGui4_64.dll SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`. DUMPBIN14.44.35228.0 from MSVC14.44.35207 supplied offline disassembly. Runtime population/parenting and arbitrary reentrant effects remain unobserved.

## Ordered nonnull status contract

CP62 establishes2d0bd0's fresh global-main lookup. Nonnull calls QMainWindow::statusBar at2d0c08. That getter loads main->private(+8)->layout(+1e0) and calls653ccd80. The layout stores a status **item** at+4f8, not a bare QStatusBar pointer:

1.Layout item null:653ccd8c ->653ccdb0 returns null without an item virtual or cast.
2.Item nonnull:653ccd98 invokes item virtual+60, then653ccda9 invokes imported QMetaObject::cast using QStatusBar's static metaobject. Thus a supplied has-status boolean cannot stand in for this item/widget/type chain.
3.Getter result null:653ca63f allocates0x28 bytes;653ca654 calls QStatusBar(main), then653ca668 calls QWidget::setSizePolicy with the compiled value0x0d and653ca673 calls QMainWindow::setStatusBar. The ctor's QWidget parent argument is the actual main. Construction/installation cannot be skipped by inventing a cached child.
4.Existing valid result returns directly from the getter. The same returned object receives QStatusBar::showMessage(message,timeout0) at2d0c3e. Status-bar identity must remain the one obtained at this lookup, not a later replacement observation.

QStatusBar ctor65406d50 allocates its private object and invokes QWidget construction. It writes private+1f0=0,+1f8=0,+200=0 before later size-grip construction, virtual visibility and setup calls. The initial+1f8 null store is not proof of the current timer at late Close, especially after those calls and intervening application activity. No candidate native status owner or completed QStatusBar construction is present in current Session interfaces.

showMessage first assigns its argument to status-private+1e8 through QString::operator=. For fixed timeout0, it then reads timer+1f8. Null skips deletion; nonnull calls deleting virtual+18 at65406a92 and only stores+1f8=null after return at65406a97. Positive-timeout code constructs QTimer with this status bar as QObject parent, replaces its vtable with6570bd98 and stores it in+1f8. Its deleting slot is65406ac0: call imported QTimer destructor, then conditionally free allocation for flag&1. QTimer/QtCore QObject teardown and callbacks remain separate dependencies. A raw owner pointer remains visible during the deleting call; shared_ptr::reset alone is not a qualified replacement for that visibility/order. This status timer is distinct from Session diagram singleShots and their dispatcher.

After the timeout branch, showMessage tail-jumps65406480. It is **not** a12-byte complete function: the first unwind record ends6540648c, followed by chained records through654065c2. The complete retained body:

- Derives nonempty-message state from private+1e8, then visits private+1e0 status-item list. It stops at the first null or permanent entry; eligible earlier widgets receive conditional visibility/attribute effects. Empty-list knowledge does not close the tail.
- Unconditionally emits exported QStatusBar::messageChanged at6540656f, then queries QAccessible::isActive. Active accessibility invokes updateAccessibility; afterward it computes a message rectangle and calls QWidget::repaint. Actual receivers, widget flags, accessibility and paint effects remain guarded.

Only after this entire status operation returns does2d0bd0 construct QColor(GlobalColor raw2), then2d0f10 delivers `2signalShowMsgInArea()` with QString/QColor/bool(false). Its shared notifier2cf3c0 suppression occurs after all preceding status effects. Preserve suppression-before-flag90 reset, map insertion/snapshot, guarded receiver resolution, DirectConnection1 order and non-rollback behavior. Suppression cannot authorize skipping status owners, timers or messageChanged.

## Message-area receiver and process-global cache

Direct retained constructord15050 calls QTextEdit(parent), sets readOnly=true and registers `1slotShowMsgInArea()` against that exact key atd150c3, before returning. RTTI at vtable1429fd408 identifies SfMessageArea. Lazy main getter018c50 allocates this receiver with parent=null at018c8f and stores it into main+3e8 only after the ctor/registration returns. The raw main cache and notifier guard are not proof of QObject parent ownership. Later reparenting/installation or explicit main teardown of this receiver remains unexpanded.

The PE scan found one direct E8 constructor caller, plus the emitter and another producer for three identical key literals. These exact direct-pattern scans are not complete indirect-call/registration coverage or evidence that a current receiver exists. The map cannot default to known empty merely because a fresh candidate Session has no MessageArea field.

Qt metadata is bound independently of string naming. Native static-meta initializer141d5ff40 assigns string table142a69bb0, data table142a69ae0 and extra data142a69d08 to runtime metaobject1446b8190. The runtime object resides in zero-fill, so reading its apparent file offset is invalid. Data header revision6 contains7 methods. Local method2 is `slotShowMsgInArea(QString,QColor,bool)`; metacall dd6390 delegates to QTextEdit, then local dispatcherdd60d0. Jump-table entry2 selectsdd61a0, copies QColor, and callsd151f0 atdd61e5. Registration strips the marker/parentheses to the bare method; Qt reconstructs this three-argument overload, not a zero-argument method.

d151f0 initializes process-global QString1446b3bb0 under flag1446b3bb8 bit0 and passes cleanup callback141dfb720 through14146ed38. The callback destroys that QString; the exact CRT exit-delivery mechanism is not closed here. On normal Close's false Boolean it skips the clear virtual, compares incoming message with the global QString, and:

- Equal message: returns without color, append, cache assignment or cursor work.
- Different message: setTextColor, append, assign the global cached message **after append returns**, obtain text cursor, movePosition(raw operation0xb,mode0,count1), setTextCursor, ensureCursorVisible, destroy cursor.

Initialization precedes comparison, so unknown/uninitialized cache cannot be treated as a known repeated-message skip. With Boolean true the clear virtual precedes comparison; that alternative is excluded from this fixed Close call. Qt append/cursor effects can reenter; the cache is still old during append. Multiple registered MessageArea instances share this same process cache, rather than independently appending the same message. Actual delivery/population remains necessary. Normal Close contains no direct reset of this cache in its inspected tail; absence of arbitrary computed writes elsewhere is not claimed.

## Current owner mismatch and smallest safe increment

Session exposes ModelManager, MainViewState, WidgetRuntimeState and shared notification state. None represents the native global main's QMainWindowPrivate layout, layout status-item+4f8, actual returned QStatusBar/private fields, status timer, main+3e8 MessageArea or global last-message QString. A diagnostic observation label or a separately allocated state holder does not establish those identities or constructor effects. CP62's latched nonnull guard must remain intact until a source-qualified same-object binding exists.

The first isolated increment should be a typed main/layout status-slot owner, bound to the live Session plus an explicitly qualified actual native main/layout identity. Model Unknown versus KnownNull versus KnownItem, with no default-null inference. Consume the layout slot once. KnownNull implements653ccd80's null leaf and stops before outer getter allocation653ca63f. KnownItem stops before virtual+60/cast until the actual item/widget/type owner is represented. Do not accept an arbitrary caller-supplied QStatusBar result, construct a duplicate status owner, or proceed to message assignment/timer cleanup without this chain. Future root binding must connect it to the actual main ownership interface; the current source does not provide that interface.

Focused tests should distinguish unknown and qualified-null slot; refuse foreign Session/main/layout; retain consumed slot identity through retries and external replacement; preserve all Project/current/OPI/notifier/timers; and stop before allocation/item virtual work. Later nonnull-driver tests must inspect message assignment before timer deletion, old timer visibility until deletion returns, post-return null store, messageChanged/accessibility/repaint before ShowMsgInArea, and process-shared duplicate-cache ordering across two receivers. A known-null timer must be a current same-owner fact, not an old constructor default. No source or tests were created in this lane. Public Close and nonnull status effects remain guarded.

## Evidence inventory and bounds

All new raw files are `analysis/resume63-close-status-*.asm`. Native bodies use unwind bounds unless noted. The `qt-65406480.asm` file is only the first unwind fragment; `-contiguous.asm` is an exploratory slice that ends mid-epilogue; only `-chained.asm` contains complete65406480..654065c2. `141d5ff40.asm` is an initializer slice: the relevant leaf ends at141d5ff79; its trailing neighboring partial procedure is not evidence of additional metadata initialization. Neither short/slice capture is called whole-function closure.

| Raw file suffix after resume63-close-status- | SHA-256 |
| --- | --- |
| 140018c50.asm | 973bec5950ef26eb381250046a858302f73c5299b3e4f412691a0db12d5ded1c |
| 1402d0f10.asm | bee0db456cc60e33f8ccd1f40e7f973b459c2065c0d238bbfdbd5aab7a5a1e7a |
| 140d15050.asm | c7537b80a479f4f150fce089110d516d3d0688a45b060b74acf24ffeb2ba845a |
| 140d151f0.asm | 50e628aa87a2b83ee3b3bee5ac01289c025c6f1c6432c8893ec0870273065298 |
| 140dd60d0.asm | a8640ad94988247ee73d9caabb57e2ec155bc642e8547d3b10124a91530225ad |
| 140dd62d0.asm | d034ba449c70dee60e2b0ed4a32c0b153c1179ef7a5cd8a6128eed714fe1f695 |
| 140dd6390.asm | 3d5019df8e760200b43608d89d96fb19b81d70725da1f6bbd25a1adf6e341597 |
| 14146ed38.asm | f9060c16f1dcc608ab20d268d05af1c900d6ef3077cce939cce5dc7e6ecc4af9 |
| 1416b00d0.asm | 4ee7bca44707c70ae96fd281dad9480db03c1fe954ca3e878b4946e2963710ec |
| 141d5ff40.asm | 073394c84340c75a9286b33c2aa422350e891337d118db83151db83a874598c4 |
| 141dfb720.asm | 66ae5c977f19d5ec4df9d04607dadd2989b4b05a0d82f667f5693577e67eef51 |
| qt-653ccd80.asm | 783abfcc6bc1be85e637b10e69ca66d7c6eb64b5be979d5706378db3f593054b |
| qt-65406480-chained.asm | d2973df465109b390c4b7bcb124fa0a9685350cc5148cde91cbcb7697088c2eb |
| qt-65406480-contiguous.asm | c98e117adcc24ebad581b158215ac4ed8949f183bf32379119cf81dada415a46 |
| qt-65406480.asm | ac68eed4d2754686f11add55de2d265431ef4e2503151d72b38ad08a48741043 |
| qt-65406ac0.asm | a2414cf1ac1311c619a9ee82168332d6d58089261ff3bce1036687a08b900a4e |
| qt-65406d50.asm | 901091316d82d88b2e2a9545f3239632820400048d13e29c476178c3ff022814 |

Key code hashes: layout getter653ccd80..653ccdb7 `e580f3f1e92d76d91515599a77898e74709184a9b4493962c5526a66136965af`; chained status tail65406480..654065c2 `b71b741a9424695380807b4976d0141fea020cc47647ed7f040c2ca1732aa9aa`; timer deleting wrapper65406ac0..65406af0 `373958c7eef6e689cb12ee1e5c95eecb42c0ed8319f984601b9e431931c76698`; handlerd151f0..d15309 `84b80ac99f9bafc280de6c6b882d47931b1cfa7fc06d79ab2c24926e6980be14`. Bound Qt imports/exports identify QMetaObject::cast, QString isEmpty/assignment/equality, QTimer destructor, messageChanged, QAccessible APIs, repaint, QTextEdit color/append/cursor APIs and the QColor copy constructor. Existing CP62 captures supply the enclosing statusBar/showMessage bodies. Retained L4/wa2.c was only a location aid; metadata/direct code/imports support the behavioral claims above.
