# CP121 DoeTableRefresh receiver alias at Save dispatch

## Finding

No retained observation binds the DoeTableRefresh weak receiver selected by the dispatcher to the same table generation stored in main-window `+0x458` at an active Type2 Save. The constructor/getter establish a *potential* alias for one completed construction; the dispatcher evidence establishes a weak-guard selection mechanism. There is no static or retained runtime bridge joining those facts for a Save occurrence. Keep the post-XML public Save guard.

## What the static chain does establish

The original PE SHA-256 remains `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

- Getter `0x1400196d0` checks `mainwindow+0x458`; on null, it constructs `SfMoDoeTable` and stores the constructor return to `+0x458` only after return.
- That constructor registers its `this` under the global key `2signalDoeTableRefresh()` with slot `1slotUpdateUI()` before it returns. Therefore, when this exact construction completes, the registered pointer and returned/cache pointer originate from the same `this` value.
- The global dispatcher `0x1402cf3c0` performs suppression handling, snapshots the ordered keyed registrations, resolves a weak QObject guard per entry, skips null results and invokes non-null entries with DirectConnection. It does not consult `mainwindow+0x458` to select or validate a receiver. Its `invokeMethod` result is ignored.
- If the proper table receiver enters its ordinary metadata route, local method zero maps to `slotUpdateUI()`; that slot calls `QTimer::stop()` on its own `+0xb0`, then calls its own virtual `+0x1b8` refresh implementation and returns only after the nested call returns.

These are static relationships between instructions and object fields, not an observed instance identity at runtime. Registration precedes cache publication, and the selected destructor body has no explicit key-unregister call. Neither fact says which weak guards survive or what the dispatcher snapshot contains at a later Save.

## No additional alias source in retained records

The reviewed table PE receipt and bounded getter/constructor/destructor/dispatch/meta-call/slot captures contain no saved cache value paired with a dispatcher snapshot entry. The Save emitter passes the global owner and no-argument key; it carries no table pointer or constructor-generation token. The retained reports distinguish constructor-time potential identity from runtime liveness and explicitly note that the dispatcher never compares with `+0x458`. The newer CP118 producer inventory classifies static Geometry-editor key call sites, not DoeTableRefresh delivery, and does not add a receiver alias.

There is likewise no retained actual Save event log recording selected receiver, guard result, receiver entry or return. Thus no amount of pointer alias reasoning across separate static functions can produce the missing same-occurrence fact. A normal `invokeMethod` result alone would also be inadequate: the address/result would need to be correlated to the current cache generation and to actual slot completion. The timer call's static position proves order if entered; it does not prove runtime call or timer-state effect.

## Smallest decisive witness

Capture one active Type2 Save at the no-argument dispatcher call with:

1. the actual `mainwindow+0x458` pointer and constructor-generation/liveness identity;
2. the exact ordered DoeTableRefresh registration snapshot, each weak-guard result, and the resolved receiver/method for the entry invoked;
3. equality of that resolved receiver with the `+0x458` generation;
4. entry and normal return on that receiver through `slotUpdateUI()`, including its `+0xb0` timer pointer, `QTimer::stop()` return and `+0x1b8` refresh return;
5. return to the same Save continuation, which continues using its earlier cached manager.

This is the smallest same-occurrence alias/return witness; static constructor and Qt metadata receipts are not substitutes. No source, build, GUI, original process, or mutable REA operation was used in this lane.

## Evidence receipts

- Original PE: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 above.
- Table owner/metadata receipt: `resume65-doe-refresh-pe-receipt.json`, SHA-256 `BB41B752BF8F46F8B6449F596F118B639B4B501913D984B3362B59939F03520B`.
- Prior dispatcher/generation report: `resume119-doe-save-receiver-generation.md`, SHA-256 `BCCD30F706ED1C80E92C4A2E397603AC00569A8D4A69993D125AECB4AD5620AA`.
- Conditional slot/timer-return report: `resume120-doe-save-slot-normal-return.md`, SHA-256 `6301A97A7CD49994EDF1394FC600ED82B98624EFD2FD23FA5F4590919DF54259`.
