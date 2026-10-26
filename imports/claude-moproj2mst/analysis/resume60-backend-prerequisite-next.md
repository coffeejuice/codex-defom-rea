# Backend prerequisite: removal and registration order

The bounded native removal path is now resolved. It **creates an absent notifier key before searching**, removes the selected registration-record identity from a local list, and writes the list back even when no receiver matches. It neither deletes that record nor clears the key. A faithful backend prerequisite driver can model this keyed state, but its actual same-notifier population/history must remain explicit and unavailable by default. Public B121 stays guarded.

Only this note was written. No source, shared interface, CMake, build, executable tests, GUI, original application execution, mutable REA session or deployment occurred. Analysis used bounded local PE/disassembly plus retained qualified decompilation. No cold Ghidra retry or active REA target was used.

## Verified native selection

Original PE `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256 was rechecked as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Microsoft dumpbin 14.44.35228.0 `/DISASM /RANGE` was used with the explicit ranges below. Ranges are PE unwind regions unless noted; they qualify selected body bytes rather than whole-program closure.

| Selected body | End exclusive | Bytes | SHA-256 |
| --- | --- | --- | --- |
| Removal `0x1402cffd0` | `0x1402d0211` | 577 | `78857d6bc7664f6d30598844c3867729bae842c0ef456bebdd18543411c2a862` |
| Map lookup/insertion `0x1402d03f0` | `0x1402d04d6` | 230 | `f9024e5d86fac2f76f35cf6979ed1c332c207cd6f78d9a311d4fbc4a57c2b3b8` |
| Pointer-list removal `0x141344d50` | `0x141344e72` | 290 | `e68e232fd737e71fddcd99096c6996fd9fe77fb0994b37369b47b41f4f1974b0` |
| Pointer-element destruction `0x14139a720` | `0x14139a754` | 52 | `edf55761c307e2ba09af8fd8dac54336c28b17d762b416d10370947f88727abc` |
| Guarded receiver equality `0x140067260` | `0x14006729a` | 58 | `795f2f14c85e6afda70c3daccfb12418658aeee3b11d9938608e841c76111857` |
| List copy `0x1402ef070` | `0x1402ef0c2` | 82 | `4e47ec14a92c305810bd004f4bc350a8a218365869033954decbd39c45fde72a` |
| List assignment `0x14015d5e0` | `0x14015d672` | 146 | `c42327e627da42fd2c70ab898981c7d8465aac613135537b37b0b34e44f24ae1` |
| Registration-record constructor `0x1402d0360` | `0x1402d0397` | 55 | `3fe995e434dcf202ae9734b85bb841b6a7fdcae422e0079ace29ce03dfb30b3a` |

Removal file offset is `0x2cf3d0`. Retained `wave2-doe-session-notifier-map-lifecycle-decompile.json` independently agrees with the confirmed control flow. Its receipt qualifies private static decompilation rather than a runtime oracle. Registration `0x1402cfbe0` remains the complete selected 1002-byte body captured by `resume51-doe-variable-notify-register.asm`, body SHA-256 `82c162c516ddaef1c539fd2a3caeae012c4781141b32a569f1daef25bef7dd7b`.

## Exact removal sequence

`0x1402cffd0(notifier, key, receiver, slot)` returns void. For the backend setter, receiver is the non-null whole-model QObject, key is exact `2signalDiagModelCanAddItem()`, and slot is exact `1slotDiagModelCanAddItem()`.

1. Construct key QString and invoke map `operator[]` on notifier `+0x88` at **`0x1402d002a`**. `0x1402d03f0` detaches the map as needed, searches it, and creates a default empty list when absent. Thus key insertion precedes slot parsing and receiver matching. Copy that list locally at `0x1402d0037`.
2. Normalize the slot: `mid(1,-1)`, find literal `"("` case-sensitively from position 0, truncate there, and fall back to the original slot if the result is empty. The known valid literal becomes `slotDiagModelCanAddItem`. Malformed/non-ASCII/empty method behavior is outside the proposed bounded driver.
3. Read local list count once at `0x1402d010d`, then scan ascending indices. `0x140067260` compares the entry's current guarded QObject pointer with the requested receiver; only a receiver match proceeds to case-sensitive bare-method equality at `0x1402d0172`. For this non-null receiver, expired null guards fail equality; they are not pruned.
4. On the **first** receiver+method match, capture that entry's registration-record pointer, call `0x141344d50(localList,&recordPointer)` at `0x1402d0189`, and leave the search. The helper removes **all occurrences of that exact pointer identity**, preserving survivors' order. It does not remove every distinct record with equivalent receiver/method. Distinct later matching records survive this invocation.
5. Whether matched or not, perform a second map `operator[]` at `0x1402d01d4`, then assign the local list back at **`0x1402d01e1`**. The key remains present even when empty. There is no map-key erase, Boolean failure return, invocation/delivery, suppression check, or `flag90` write in this body.

Pointer-element destructor `0x14139a720` sets EAX to zero and takes the no-delete branch unconditionally. Removal therefore removes pointer slots without deleting the pointed registration record. Preserve allocation identity/lifetime as a distinct record; do not replace this with automatic deletion of a receiver or metadata object. Native copy-on-write container allocation/refcount details and allocation-failure behavior are separate from this valid-data logical contract.

## Registration and backend-store ordering

For non-null backend, `0x140071b30` calls registration `0x1402cfbe0` at `0x140071b69`, with flag 0. That helper:

1. Constructs its local list, **allocates 0x10 bytes** at `0x1402cfc17`, and constructs the new guarded-receiver/QString record through `0x1402d0360`.
2. Normalizes the slot, assigns the supplied receiver and method into the new record, then checks map membership at `0x1402cfd8b`. If present, copy the current list; otherwise retain the local empty list.
3. For flag 0, search ascending receiver+bare-method pairs. Append the new record only if no equivalent pair exists. For flag 1 the separate path prepends without this dedup scan; the selected backend setter uses flag 0.
4. Assign the local list to `map[key]` at `0x1402cff24`, then re-read that map list and count before cleanup/return. The map entry is present even when an existing pair caused append to be skipped.

Allocation occurs **before dedup**, so the second same-pointer attachment still allocates/initializes a fresh record even if the first attachment already registered the pair. The successful duplicate path has no deletion of that unused record; do not invent cleanup or skip the allocation because the final list is unchanged. A bounded representation may retain these allocations under a separate adapter heap owner without claiming native Close cleanup. Null allocation leads into unguarded uses in the native body; it must not be generalized into a successful no-op.

For null backend, the derived setter calls removal `0x1402cffd0` at `0x140071b8e`. Both branches call base setter `0x14014c3f0` only **after** that helper returns, at `0x140071b9d`; the leaf store `0x14014c404` writes model `+0x18`. There is no native suppression bypass for either prerequisite. A registration/removal refusal cannot be replaced with a backend store or rolled back with prior effects.

Retained diagram `setModel` `0x140170430` first detaches the old model at caller `0x140170540`, then invokes the new setter at `0x140170581` and `0x1401705f4` around intervening view effects. Keep both registration/allocation occurrences. The existing BackendCommitPrefix's default refusal remains before entering either prerequisite; it correctly claims no internal allocation or map effect in that default branch.

The direct registration inventory has **another** CanAddItem site in copy constructor `0x1400711a0`, at **`0x14007185d`**. A bounded disassembly confirms flag 0, receiver=this, key literal `0x1421392e8`, slot `0x1421392c8`, same global notifier. The normal Open SUB constructor and Copy remain separate source paths. The inventory is finite direct-call evidence, not arbitrary dynamic-registration closure. Normal SUB witness history alone cannot certify this key's absent or complete list at later delivery.

## Exact minimal action

Implement an isolated `BackendSetterPrerequisiteDriver` for this **single exact key/slot**, backed by explicit typed state bound to the same actual Session notifier and actual model generations. Keep key presence and ordered record list unavailable by default. Use stable registration-record identities, guarded receiver generations and retained allocation receipts; do not use a set of `(receiver,method)` pairs that loses allocation or pointer-alias removal behavior.

At driver entry, validate actual owner/lifetime before representing native effects. For registration, the recovered successful-allocation prefix can allocate/init/retain the record and normalized fields before the first unavailable map-membership boundary; later refusal must preserve that prefix. For removal, unavailable map state stops before `0x1402d002a`; a qualified absent state inserts the empty key before method normalization. Once actual state is qualified, perform the recovered local-copy/search/writeback behavior. Only then return to BackendCommitPrefix's existing post-prerequisite generation check and leaf-store record.

Do not install a default empty key, infer closure from a textual qualification, mutate unrelated notifier flags/keys, publish the isolated backend value to FlowReceiverWitness, or call public B121. Production wiring still requires complete actual same-key registration/copy/removal history and surrounding diagram effects, actual QObject/backend lifetime, and SetOprHasDoe population. This audit resolves the selected valid-data helper behavior, not those ownership/history prerequisites.

Focused tests for that next isolated driver should cover absent-key removal insertion; present empty/missing-match writeback; stable survivor order; expired guards retained; first matching record plus all aliases removed while a distinct equivalent record survives; retained record allocation on both duplicate registration and map-state refusal; two attach allocations but one deduplicated entry; flag-0 append and separate unsupported flag-1 scope; suppression/flag90 unchanged; prerequisite effects visible before backend store; failed prerequisite preserving old backend; and Project release during the active prerequisite retaining its earlier effects while refusing the store. Keep the existing public B121 and Population regression tests.

## Evidence receipt and limits

| Retained file | SHA-256 |
| --- | --- |
| `analysis/wave2-doe-session-notifier-map-lifecycle-decompile.json` | `ef09c0cca9e3bcc1b4fe88f4228684ef7e199635ab7e5739f4039c6f669a841a` |
| `analysis/wave2-doe-session-notifier-map-lifecycle-receipt.json` | `c21f7e0672ff6007575b8874d6171744b8f3ee6bba5f2a1e28c48790f959967f` |
| `analysis/resume51-doe-variable-notify-register.asm` | `53c002b76cc2b5ce9ab41e1d9750ac660bb4956bcc855164f3a6e5f73a11300f` |
| `analysis/wave2-doe-session-notifier-direct-key-candidates.json` | `1a947bb2daf487a9e9d8aee1999d09675f2abb6202b3e526ef1847fcbc6cf0d9` |
| `analysis/resume59-doe-notify-next.md` | `746fff4d803ace31221b2ac58f4d4ad914031dc89451674201148997b4834e16` |
| `source/apps/moproj2mst/src/doe/backend_commit_prefix.cpp` | `cb3695f1d430ef37b4d77a89f1d276d77e8ee763acda32531d50644e9f813dc7` |

No fresh native delivery, native heap/Qt failure equivalence, complete map population, backend ownership, public B121 success, Save/Close parity or objective completion is established. Token measurements are unavailable.
