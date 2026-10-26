# CP199 TLS-index CMP: bounded symbolic next increment

## Decision

A one-instruction **structural symbolic CMP** increment can be specified without inventing a TLS index, native module instance, observation, or issuer. Its source issuer must be the existing Session-owned WidgetRuntimeState root, with a new private immutable symbol/read schema and an exact Session grant. It is not implementable by reusing an existing TLS receipt: none exists. This plan permits a fresh unknown 32-bit read expression and flags only, ending at RVA `0x2961b` before JNE. Concrete fast-path advancement remains blocked by the runtime dword value at the selected read.

This is a proposed schema/implementation contract requiring review, not an authorization to treat an unknown symbol as an initialized TLS index. It makes no original-process execution claim. The abstract root represents the adapter's existing QtCore symbol namespace; its creation does not certify a physical DLL load, process identity, module base or initialization epoch.

## Inspected predecessor and evidence

Read `resume199-tls-index-owner.md`, both CP198 plan/review reports, the original static qualification `resume163-connector-qobject-static.json`, and `wave2-doe-task-public-qobject-ownership-contract.json`. These qualify retained original-image disassembly, bounded regions and selected ownership dependencies. They do not establish live Report execution, full logical-function boundaries, exhaustive alias enumeration or a same-thread TLS value.

The CP198 plans predate implementation. Current inspected source now contains the ten-stage callee prefix, `BeforeTlsIndexCompare`, nested generation 3, eight stack slots and symbolic SUB flags. `advanceCalleePrefix` stops at ordinal 10 before the global read. This source presence is not a CP198 freeze/review receipt. Reconcile the eventual frozen header/implementation/test pair and acceptance record before CP199 edits; do not implement against the planning documents alone.

Qualified instruction bytes are `83 3d cd 72 2c 00 ff` at QtCore RVA `0x29614`. Next RIP `0x2961b` plus signed displacement `0x002c72cd` resolves to `0x2f08e8`; opcode 83 sign-extends imm8 `ff` to the 32-bit operand `0xffffffff`. The instruction is `CMP dword [QtCore+0x2f08e8], -1`. The following JNE at `0x2961b` targets `0x296c0` and is excluded.

The evidence module identity is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The producer audit records PE file-backed `ff ff ff ff` at raw offset `0x2ee4e8` and initializer `TlsAlloc`/store at `0x29686/0x2968c`, plus teardown reset at `0x1a6215`. Those are initial-image bytes and static producer/reset candidates. They supply neither the running dword nor persistence to this frame. Do not seed the unknown expression from PE -1, constructor defaults, startup success, host TLS or an assumed absence of writes.

No new original-binary decoding was performed here; the instruction/image statements rely on the independently checked retained CP198/CP199 qualifications above.

## Real source issuer and lifetime

`ConversionSession` constructs the one `TaskAllocationStore`, shared `WidgetRuntimeState` and `TaskFactoryRuntime`; its constructor associates the allocations with that widget root, then calls private `bindSessionReportNullCaller` (`src/engine/conversion_session.cpp:163` onward). The latter is private to the Session workflow and rejects non-pristine/foreign roots. Its `SessionReportNullBinding` is explicitly a privately issued static caller qualification, not a captured native frame. The validator compares exact shared ownership/control blocks and Report/task/private/frame/caller occurrences.

`WidgetRuntimeState` in `include/m2m/engine/view_manager.hpp:487` already owns distinct QtCore QList/QString/QVector symbols on this same adapter root, with unknown absolute counts where appropriate. Its constructor (`src/engine/view_manager.cpp:839`) creates the existing `WidgetThreadState`. Its destructor explicitly distinguishes ending an adapter/module binding from releasing a physical module. The root therefore supports another *symbolic* module-global identity without pretending that Session startup observed its value. The allWidgets qualification is not a TLS qualification.

Proposed issuance has two levels:

1. WidgetRuntimeState privately owns one noncopyable immutable `QtCoreTlsIndexSymbol` for its lifetime: weak root identity/control block, private occurrence token, fixed module hash, RVA `0x2f08e8`, width 32 and evidence identity. Construct it with that root (bind the weak root after shared ownership exists, at the private Session binding step if needed). No value/default, loaded base, native-process ID, success bit, TLS generation or QThreadData pointer belongs in this symbol. A standalone WidgetRuntimeState may possess symbol metadata but gains no production Session grant from it.
2. After existing Session admission succeeds, the exact factory may request one immutable `TlsIndexReadExpression` from a private WidgetRuntimeState issuer. Restrict access to TaskFactoryRuntime and require the exact private Session capability/frame, current live allocation scope and original root; do not expose a public `observe(value, qualification)` constructor. Grant/bind the capability only through the existing private Session binding route after `associateWidgetRuntime`. The root retains/authenticates the issued read handle; the factory retains the exact issued transition handle, following CP198's identity pattern. Matching strings, numeric addresses, public field copies or newly allocated lookalike control blocks cannot authenticate themselves.

The read is fresh per executed instruction occurrence: `UnknownRead32(symbol, exact frame/call/read occurrence)`. It is not a timeless cached variable whose value is assumed stable across calls. Bind the existing thread owner for execution-context identity only; this does not make the slot thread-local or qualify a TLS getter result. Include the enclosing generation-2 call, nested generation-3 occurrence, Report ordinal zero, task/private/source-binding identities and read RVA. Store only weak root/thread/capability links as appropriate to avoid extending Session lifetime through diagnostics. Retaining a diagnostic expression must not authorize another read after root destruction or factory/allocation scope release. `releaseAdapterScope` must invalidate consumption even if external handles retain metadata. A fresh Session/root creates distinct symbol and read identities; no unload/reload or native lifetime claim is added.

The issuer certifies expression ownership and unknownness, not a runtime observation. No unavailable external observer is needed for this bounded abstract instruction. If project policy requires all memory reads to carry a concrete native observation, this alternative cannot proceed; retain `0x29614` until the exact observation described by the producer audit exists.

## Phase, arithmetic and boundary contract

Admission is exactly accepted CP198 terminal: phase `BeforeTlsIndexCompare`, cursor `0x29614`, completed prefix 10, RSP `S-0xa8`, current second-SUB flags from `0x29602`, generation-2 live `[S]=0x13144e`, generation-3 innermost `[S-0x70]=0x2acb6`, all eight issued stack writes and the original register/source/capability/history checks. No preexisting TLS read/CMP state is allowed.

Prepare the authenticated read expression and comparison record before any publication. Let `x` be that expression, width 32; the comparison result expression is `r = (x - 0xffffffff) mod 2^32`. CMP discards the result and replaces CF/PF/AF/ZF/SF/OF with the 32-bit subtraction functions of these same operands. ZF denotes `x == 0xffffffff`; it is not assigned true or false. CF is unsigned borrow, PF parity of result low byte, AF auxiliary borrow, SF result bit 31, and OF signed subtraction overflow. Neither 64-bit SUB functions nor TEST's forced CF/OF values are appropriate. Other represented flags are preserved; no unrepresented flag values are invented.

The CMP record must bind width, sign-extension rule, immediate, result expression, producer `0x29614`, memory-read occurrence and owner. Current-flags selection gains a CMP case which takes precedence over the retained second-SUB record. Both SUB records and parent TEST/JE stay historical. Do not erase CP198 history to make a later validator pass.

Commit one instruction, advance to a distinct phase such as `BeforeTlsIndexJne`, derive cursor `0x2961b`, retain RSP and all general registers/stack slots unchanged, and stop. No branch outcome/path token exists. Do not evaluate JNE, select `0x296c0`, enter initialization, return, reload at `0x2acb6`, call `TlsGetValue`, write the slot, assign private+0x40, increment a thread/QObject reference, publish a task or complete the factory. The index's concrete value and both branch alternatives remain unresolved.

Keep CP198's validator scoped to its immutable predecessor and add a successor validator rather than extending enum arithmetic past its ten ordinals. Update all phase-range/current-flags/cursor helpers deliberately; the new phase must not accidentally look like an eleventh CP198 stack instruction. Admission/commit must be failure-atomic. Terminal reentry authenticates the exact issued CMP state and stops without a second read or fresh occurrence.

## Required acceptance tests

- Actual Session-issued first Report reaches frozen CP198 then advances exactly one CMP to `0x2961b`. Assert private root/symbol/read ownership, exact hash/RVA/width/occurrences, unknown operand, six Sub32 flag functions and current CMP precedence. Assert CP198 SUB/TEST/JE history, all registers, stack slots/liveness, nested/enclosing calls, private fields, counts, cache and incomplete state are unchanged.
- Direct CP198-plus-CMP equals every valid interrupted-prefix/resume route. Repeated terminal resume preserves exact read/comparison handles and counts; no replay or new unknown is issued. A read in a distinct accepted frame/session must have a distinct occurrence rather than implied equal value.
- Complete snapshots include root/thread/control blocks, grant/capability, read and comparison occurrences, current/historical flags and every existing frame/private/publication field. Missing, foreign, copied, stale or same-address/different-control-block roots/handles; wrong symbol/hash/RVA/width/instruction; wrong immediate/sign extension; wrong read/frame/thread/call generation; wrong phase/cursor/RSP or malformed predecessor all refuse without mutation. Attempted public/synthetic issuance must fail or remain unqualified.
- Destroy/release the factory or allocation scope and test refusal despite retained diagnostic handles; use a new Session to reject old symbols/reads. Test root expiration independently. Preserve standalone unknown/non-null/synthetic predecessor guards.
- Coherent rewind must restore the issuer's trusted handle state with the full predecessor snapshot; partial phase/flags rewind cannot cause another read. Inject preparation allocation failure where existing facilities permit and require complete predecessor preservation.
- Assert no concrete dword, PE-derived -1, initialized-index receipt, JNE result, native loaded-module/base claim, getter/QThreadData result or later effect is introduced. Expression-evaluator unit tests may verify subtraction semantics over selected x values, but those examples are not production TLS observations or branch witnesses.

## Remaining blocker and smallest preparation

Concrete JNE selection still requires a qualified runtime read at this exact occurrence (or fully reconstructed producer plus justified no-reset/no-overwrite lifetime). The proposed owner does not close it.

For this structural option, the smallest preparation is a review of the private root issuer, trusted read-handle retention/invalidation and successor phase/flags schema against the final frozen CP198 source/test pair. No new native capture or loader reconstruction is necessary to describe an unknown expression. If the identity/lifetime review rejects using the existing adapter QtCore namespace, the exact missing source feature is a privately created, Session-bound symbolic module-scope token; add/review that inert token first, with no read/phase advancement. Do not substitute an invented physical module lifecycle.

Only this report was written. No source/test/build, GUI, native execution, mutable REA database, parent application, DMO, Claude, original artifact or deployment was changed.
