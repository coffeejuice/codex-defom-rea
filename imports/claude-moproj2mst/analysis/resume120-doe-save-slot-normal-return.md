# CP120 DOE Save slot and timer normal-return binding

## Finding

The retained static bodies close the **conditional code route** from a correctly selected live table receiver through its meta-call case to `slotUpdateUI()`, and from that slot through the timer-stop call and refresh virtual. They do not supply a runtime return receipt for the receiver or either nested call during an active Type2 Save. The exact unresolved edge is occurrence ownership: the weak-guarded registration chosen at the Save dispatch must be the currently published main-window table generation, and that same receiver must enter and return from the slot. Keep the active Save guard.

## Conditional static route

Original GUI PE SHA-256: `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.

1. The retained `SfMoDoeTable` metadata identifies local method 0 as no-argument `slotUpdateUI()`. Its qt_metacall delegates to the base, then for a local ID below 22 enters the static metacall; jump-table case 0 calls `0x1400f4e10`. This establishes the ordinary metadata route if the dispatcher selected that table and the invocation resolved to this metaobject path.
2. The complete slot body `0x1400f4e10..0x1400f4e43` loads receiver `+0xb0` and calls imported `QTimer::stop()` at `0x1400f4e25`; after that call returns, it loads the same receiver's primary vtable and calls virtual `+0x1b8` at `0x1400f4e38`; after that call returns, it executes the slot epilogue and `ret` at `0x1400f4e42`.
3. The identified primary vtable resolves `+0x1b8` to `0x1400f47e0`, whose bounded body has a normal tail and clears its in-progress byte on that path. This proves the target and code-order conditional on that vtable being active; it does not observe a runtime entry or return.
4. The Save signal dispatcher uses DirectConnection for live entries, but stores and ignores `QMetaObject::invokeMethod`'s Boolean. Its own normal return does not show which metacall case ran or whether the expected slot returned. No retained evidence logs the invoke's receiver, slot entry, timer call return, refresh return, or slot return for an actual Save.

The timer call is a call-site, not proof that a timer had been active or that stopping it caused a particular Qt event-loop state. A complete slot return on the expected direct-call route would imply the imported call returned far enough for the subsequent refresh and epilogue to execute; this pass has only disassembly, not that runtime return. Do not equate the constructor's separate AutoConnection from `QTimer::timeout()` with Save's synchronous global signal dispatch.

## Same-occurrence gap

The static constructor passes its `this` pointer to the registration at `0x1400ebc5c`, while getter `0x1400196d0` publishes the returned table at main-window `+0x458` after constructor return. The dispatcher selects from its own weak-guarded ordered snapshot and never consults `+0x458`. Registration is earlier than publication; the destructor body has no explicit key unregister call. These facts permit a static constructor-generation relation, but don't identify the live weak guard, registration snapshot entry, or current cache generation at Save time. The queued CP118 producer analysis concerns Geometry-editor actions and introduces no such DoeTableRefresh occurrence binding.

The smallest missing witness is one correlated Save event with: (a) `+0x458` and the ordered DoeTableRefresh registration entry/guard resolution; (b) the exact resolved receiver and method at invoke, matched to that cache generation; (c) entry and normal return of `slotUpdateUI()` on that receiver, including its `+0xb0` timer pointer, return from `QTimer::stop()`, and same-receiver refresh virtual return; and (d) return to the Save continuation using the previously cached manager. Neither a normal dispatcher return nor an invoke Boolean alone provides this sequence.

## Disposition and evidence scope

No further isolated helper or static call-site search can manufacture the missing same-occurrence runtime values. Preserve the post-XML public Save guard and earlier partial effects.

Read-only bounded evidence review only. No source edit, build, GUI/original execution, or mutable REA operation. Primary receipts: `resume65-doe-refresh-pe-receipt.json` SHA-256 `BB41B752BF8F46F8B6449F596F118B639B4B501913D984B3362B59939F03520B`; slot capture `resume65-doe-refresh-slot.asm` `A12B2571B28BA5CF842532FC23ACF407EBF2E4F9A64694EC4844B89B4BDA6762`; metadata route `resume65-doe-refresh-metacall.asm` `2D99395280F6FE08DCAE1309BA9D0136BA2FB5310842BBD87CCA40C1FD0D7BA2`; static metacall code `resume65-doe-refresh-static-metacall-code.asm` `7B6AC9FEED56A4BB7EB23260E81648686C7585C9BF43D2A9B2597428CFE12A61`; dispatcher `resume51-doe-variable-notify-dispatch.asm` `72E47AEEF793CF4E9347BCEAE891A6EE540CECDD0254EE19C7D9FF8360395957`; prior invoke-return analysis `resume116-doe-save-callback-return.md` `429FCB7543A72D68C7EE03BDF79F908459304C02D3E031C8BF5C1FFECFAEAFFD`.
