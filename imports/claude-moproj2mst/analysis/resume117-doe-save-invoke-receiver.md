# CP117 DOE Save invoke receiver and main-window table binding

## Static receiver binding found

The retained static chain binds a **constructed `SfMoDoeTable` instance** to a global registration for `2signalDoeTableRefresh()` / `1slotUpdateUI()` and maps that slot to the first QTimer/table-refresh calls. It does not bind the registration selected by the dispatcher to the live main-window `+0x458` cache in a particular Type2 Save occurrence.

Original GUI PE SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

1. Main-window getter `0x1400196d0` checks `mainwindow+0x458`. On a miss it allocates `0xb8`, invokes table constructor `0x1400eb6f0`, then stores the returned pointer into `+0x458` only after constructor return (`resume65-doe-refresh-getter.asm`). A cache hit returns the existing pointer.
2. Constructor `0x1400eb6f0` completes initial setup/refresh, then calls global registration function `0x1402cfbe0` at `0x1400ebc5c`. The call passes global owner `0x143d76700`, key string `2signalDoeTableRefresh()`, the constructor's table `this` pointer, and slot string `1slotUpdateUI()` (retained `resume65-doe-refresh-ctor.asm` / owner report). Thus a registration made by this constructor identifies the eventual getter return/cache value for that construction generation. Registration happens before the getter publishes `+0x458`.
3. Dispatcher `0x1402cf3c0` obtains the key's registration list, snapshots it in order, and resolves each stored weak QObject guard. On a live entry it passes the resolved object to `QMetaObject::invokeMethod` at `0x1402cf6c6` with DirectConnection. It invokes the pointer recovered from the registration; the dispatcher does not load or compare main-window `+0x458` at that call site. The invoke result is stored but untested (CP116).
4. If the selected receiver is this table, its metaobject slot resolves to `slotUpdateUI()` at `0x1400f4e10`. That body loads the same receiver's `+0xb0` timer, calls `QTimer::stop()` at `0x1400f4e25`, then synchronously invokes virtual `+0x1b8` at `0x1400f4e38` (`0x1400f47e0` on the identified primary vtable). The refresh resolves the then-current main-window Project/active SUB; it receives no saved-SUB/manager argument.

## Where the same-occurrence proof ends

The native Save emitter `0x1400d369d` passes only the global owner and a no-argument key. No pointer to the selected Save SUB, its `+0x20` manager, the Study, the cached table, or a callback generation is included. The static constructor/getter chain proves a potential registration origin; it does not tell which registration the runtime snapshot holds at this Save, whether its weak guard resolves to the table generation currently in `+0x458`, or whether `invokeMethod` resolves the expected method and synchronously returns after slot completion.

This distinction matters because constructor registration precedes `+0x458` publication; the global dispatcher walks its own registration population rather than consulting the cache; and destroyed/stale receivers may remain as weak registrations until visited. Static code qualifies the liveness check operation but not the live pointer or list state for this occurrence. It also does not establish the receiver's refresh-time current Project/SUB/manager, which may differ from the manager cached by Save. Record that refresh context separately; native Save later uses its earlier cached manager and does not test equality.

The candidate mirrors none of these runtime objects: `DoeTableOwnerState` stores diagnostic Session-scoped identities and constructor/cache metadata, but not the global QObject registration list, a real main-window widget, QPointer guard, QTimer or actual meta-call. Its `requireRefreshDelivery` remains the first active empty-variable Type2 Save guard after XML normal return. Adding the static pointer relation as a candidate success token would not prove invocation or return.

## Exact runtime witness if this edge is reopened

One correlated Save trace must capture:

- pre-XML Save Project/SUB/Study/I/O/manager pointers;
- at `0x1400d369d`, `mainwindow+0x458` and the exact ordered registration snapshot for the key (including each receiver identity and weak-guard resolution);
- the actual receiver pointer and method passed to `QMetaObject::invokeMethod`, proving whether it is the live `+0x458` table generation or a no-invocation/other-target route;
- for that table pointer, its `+0xb0` timer identity and normal return from the `QTimer::stop()` call, then same-object `+0x1b8` refresh and normal slot return;
- the refresh-time current Project/SUB/manager and the subsequent Save continuation's original cached manager.

A normal return from the dispatcher or `0x1400d3200` is insufficient because the dispatcher ignores the invoke result and can return without slot entry. An invoke result alone is insufficient without matching the chosen receiver to `+0x458` and observing slot return. No such per-occurrence capture exists in the retained evidence reviewed here.

## Disposition

No new helper/test is source-closed. Keep the active public Save/output guard after the InputTable XML effects. The next decisive evidence is the correlated runtime witness above; no further static caller search can populate the missing current registration/cache/liveness values.

Read-only retained PE/source evidence only; no source edit, build, GUI/original launch, or mutable REA database access.
