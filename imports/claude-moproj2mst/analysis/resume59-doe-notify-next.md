# DOE notifier: next state transition after Session ownership

The next recoverable state transition is the **actual flow model's backend-pointer commit**, tied to its constructor generation and the shared notifier. Session ownership and normal SUB birth/death history now exist, but neither a complete `SetOprHasDoe` receiver population nor the current backend value at lazy B121 delivery is established. Keep public B121 parsing guarded. A constructor-only null or the Session's Boolean diagram binding cannot replace that evidence.

This is a read-only audit. Only this note was written. No source, CMake, shared plan, build, test execution, GUI, original application execution, mutable REA session or deployment occurred. The reverse-engineer-anything workflow uses retained source-qualified captures plus bounded local PE reads/disassembly; no cold analysis retry was attempted.

## Current source boundary

Checkpoint58's `ConversionSession` owns `FlowReceiverWitnessRegistry` on its existing `notificationState_`, binds it to the actual Project before `initProject`, and therefore records SUB0 before StudyIo exists. `makeSubProject` records each stable `&sub.flow` generation before the Project backlink and Type2 Study construction; the SUB lifetime hook marks that generation dead before its members disappear. Later normal SUBs use the same factory. This is real candidate ownership/order coupling, with explicit limits on native QObject construction and destruction.

`FlowReceiverWitness::backendWitness()` still unconditionally returns unavailable. The registry never installs `SetOprHasDoeBindings.registrations`. `bindSetOprHasDoeSessionPrefix` checks the actual live Session/Project/StudyIo/notifier relationship, but its new binding still has unavailable population. Thus the current unsuppressed native prefix is exactly:

`verify owner -> flag90=false -> M2M_E_UNSUPPORTED("2signalSetOprHasDoe() prefix: ordered receiver population is unavailable")`.

The actual suppression branch returns before flag reset or population lookup. The isolated scalar parser still commits its variable/keyword/operation prefix and stops before delivery. The public `readInputTable` still refuses nonempty DoeVariables with `DoeVariables reader 0x14090ee20 has no oracle case yet`.

## Newly verified backend setter

The immutable original `DEF_GUI_MO_64.exe` SHA-256 was rechecked as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. PE reads confirm the model constructor's secondary interface table `0x14213ba88`, installed at whole-model `+0x10`, has slot **`+0x08 -> 0x140071b30`**. The raw eight-byte slot hash is `754e29f58bd9fa9f1e775bbd4016806b9fbe213d2484a3987a27541345e1adea`.

Microsoft dumpbin 14.44.35228.0, `/DISASM /RANGE:0x140071b30,0x140071ba6`, confirms the complete selected body. Its PE unwind region is `[0x140071b30,0x140071ba7)`, file offset `0x70f30`, 119 bytes, SHA-256 `cbce8273c17bf7c90262da9c794e31f773cae6e0825c5122b7c345021f0a650a`.

The ordered body is:

1. Test the supplied backend pointer at `0x140071b3e`.
2. Non-null: compute the whole-model QObject as `interfaceThis-0x10`, set flag 0, and call registration API `0x1402cfbe0` at `0x140071b69`, with global notifier `0x143d76700`, key **`2signalDiagModelCanAddItem()`**, and slot **`1slotDiagModelCanAddItem()`**. Literal addresses are `0x1421396c8` and `0x1421396a8` respectively. The existing recovered registration contract normalizes the bare method and appends only an absent receiver/method pair.
3. Null: compute the same whole-model receiver and call **`0x1402cffd0`** at `0x140071b8e`, with the identical key/slot at `0x142139708`/`0x1421396e8`. This is the retained removal-API path; this audit verifies its call and arguments, not a complete new disassembly of that callee's mutation/failure behavior.
4. Both paths call base setter **`0x14014c3f0`** at `0x140071b9d`, with the original interface pointer and supplied backend.

The leaf setter is bounded directly by its return and padding, not unwind metadata: `[0x14014c3f0,0x14014c409)`, file offset `0x14b7f0`, 25 bytes, SHA-256 `7248c320c2788c4d4a4807f2e0f0391ab36e41cbe85350fc417704644922ef1e`. Its complete bytes are:

```text
48 89 54 24 10 48 89 4c 24 08 48 8b 4c 24 08 48 8b 44 24 10 48 89 41 08 c3
```

The sole store is **`0x14014c404: mov qword ptr [rcx+8],rax`**, hence whole-model `+0x18 = suppliedBackend`. No callback, allocation, icon or operation-field update occurs inside this leaf. The preceding registration/removal call belongs to the derived setter and must not be skipped to reach this store. A refusal there retains the old backend value and any earlier registration prefix effects.

## Why the diagram Session hook is insufficient

The retained, byte-verified `0x140170430` diagram `setModel` body demonstrates actual backend changes after construction. Its old-model branch invokes old interface `vt+8(null)` at `0x140170540`, after saving old view state. It writes the view's own attachment, then invokes the new model's `vt+8(viewThis)` at **`0x140170581`**. After the intervening holder reset and view `vt+0x2b8(0)` call, it writes the attachment again and invokes the same new-model setter at **`0x1401705f4`**. Selected view refresh, restart-icon and timer effects follow.

The model secondary-vtable dispatch above binds those calls to `0x140071b30`. This is stronger than an inference that published models probably acquire backends. A repeated same-model attachment does not justify collapsing both setter occurrences or the intervening effects. Null detach is a backend store, not flow-model receiver death; `SetOprHasDoe` constructor registration remains a distinct key/lifetime.

Current `ConversionSession::commitNonNullDiagramBinding` and `commitNullDiagramBinding` model the bounded diagram attachment/timer state. They do not call the native model setter or implement `DiagModelCanAddItem` registration/removal. Consequently they cannot certify a current `backendWitness()` by themselves. This audit does not establish every writer of model `+0x18`, backend lifetime/type, or a complete setter call history at B121 delivery.

## Smallest implementation and evidence boundary

An isolated follow-up can add a **per-generation backend-state commit record** to the existing flow witness owner. It must begin unavailable. It may record a known value only at a qualified actual `0x14014c404` commit for that exact live model generation, after the matching derived setter's registration/removal prefix completed. Record source occurrence and the actual backend identity; no synthetic backend object, copied model or active-only lookup is needed. A failed pre-store prefix cannot alter the backend witness. Any unqualified publication/write interval makes the current value unavailable again. Receiver death clears it and never transfers it to an address-reused generation.

This is a source-bound partial transition hook, not a replacement implementation of `setModel`. Before wiring a production caller, close the `0x1402cffd0` removal contract and supply the actual same-notifier typed `DiagModelCanAddItem` registration state; preserve both new-model occurrences and the old detach. Do not add an unrestricted `setBackend(nullptr)` whose caller can certify arbitrary lazy state.

Independently, a population bridge must use ordered constructor/death history from the same Session registry. The finite direct registration inventory identifies normal registration `0x140071166` and Copy registration `0x140071a7f`; it does not prove arbitrary dynamic-registration closure. A partial history can be represented diagnostically, but it cannot silently turn missing population into a qualified empty or complete list. A future closed fresh-history profile must retain dead entries, all normal models including inactive SUB0, separately sourced copies, native append/dedup behavior and later registration changes. A textual qualification alone is insufficient.

With those prerequisites actually supplied, the next dispatcher boundary is `BackendWitness`; known current null returns before logical-item lookup, while live backend plus missing item returns before icon work. A live matching item remains guarded before QIcon allocation for true or backend clear for false. Even a successful bounded notification must be followed by native post-delivery keyword lookup before scalar setters; do not reuse a pointer assumed unchanged across delivery.

## Focused test contract

Future tests should establish these independent transitions without claiming fresh-original parity:

- Match actual Session/Project/registry/notifier and the exact live model generation; reject a foreign owner, wrong model/control block, dead witness, released Project or reused address before the proposed backend store.
- Keep unavailable state across constructor publication unless its current value is source qualified. A qualified base-setter event stores non-null or null exactly once and retains its occurrence. A refused registration/removal prefix preserves the previous backend state and any preceding diagnostic/map mutations.
- Replay old detach, first new attach, intervening refusal and second new attach distinctly. If the intervening view effect refuses, preserve the first attach; never roll it back or claim the second occurred. Same-pointer repeated calls remain separate events.
- Population remains unavailable until its actual history is closed. Retain inactive models and dead snapshot entries in constructor order. A backend commit alone cannot lift Population refusal; that refusal still preserves `flag90=false`.
- With an explicitly closed diagnostic population, test unknown backend refusal, current null without item access, missing logical item, and live matching item icon guards. Preserve earlier receiver effects and int-then-bool argument order.
- Suppression requires neither population nor backend evidence and leaves `flag90` unchanged. If a later parser continuation is supplied, emit every operation notification first, re-query the last keyword afterward, then apply selector/type/text setters and finalization in native order. Refusal retains prior variable/keyword mutations and skips later setters. Scalar double never enters `0x140916530`/temp.KWD.
- Existing public B121 failure remains unchanged until the complete ownership/population/backend/continuation chain is integrated. This audit authorizes no public guard removal.

No tests were added or run in this read-only lane. Token measurements are unavailable.

## Retained capture and source receipt

Hashes below were read from current files during the audit. They identify evidence/source versions, not executable validation.

| File | SHA-256 |
| --- | --- |
| `analysis/resume51-doe-variable-notify-receipt.json` | `159d1e29113b720b20ba949b721bd4a9d7bb73aae31fff28c11c736a28ecf8ab` |
| `analysis/resume51-doe-variable-notify-model-ctor.asm` | `ece4c8d343dbf75c024c56faaeeafe921a75b31f197996c53455c429bb6e144d` |
| `analysis/resume51-doe-variable-notify-base-ctor.asm` | `d6a1ccd8fd74bcc9ba3f7bc8e1a34e947ce0d0aac12f226544acc97394a4ca14` |
| `analysis/wave3-boolean-timer-140170430.asm` | `990eeb72652151a1ea1120c2ac386dbd7cef20a718692a875003945998c40315` |
| `source/apps/moproj2mst/src/doe/flow_receiver_witness.hpp` | `2b5f788308d2f41dc9dcaa70d8c5557d1a65a6931ee6563d5eb8f79683459152` |
| `source/apps/moproj2mst/src/doe/flow_receiver_witness.cpp` | `53ca78012fb718c7818d334cec1138306387e9d3a6461c4dfbe9f66707decab1` |
| `source/apps/moproj2mst/src/doe/variable_notify.cpp` | `592ddf00afa584c67584ea01f2e3c8fd426b3e49e381cb5ec298b314637b6c83` |
| `source/apps/moproj2mst/src/doe/variable_prefix.cpp` | `648f27d48011d18a467806da90ceaab045811ff4998db8fe80e37926b47e759e` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `fb54fd5f79dbbe6660c0cc0284015bff8d33a8ae9495bf88de8b985b7f55a8fd` |
| `source/apps/moproj2mst/src/model/subproject.cpp` | `187ac92f56cc1ce9a4e1c08d3037c90fe98e1e17a8885340a78ca5901863977c` |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `988b9599de5d47426bef8fd34554b436636e0af2932ecbd1f43e1458629532e4` |
