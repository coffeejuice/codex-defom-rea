# CP199 symbolic TLS-index CMP: independent plan review

**PASS for the bounded structural plan. BLOCK source edits until CP198 is frozen, independently accepted, and the final header/implementation/test pair is reconciled.** The proposed private issuer can authenticate ownership of an unknown expression without inventing a TLS value. It cannot authenticate a native observation, initialized index, concrete branch, or actual module lifetime. No instruction, arithmetic-width, or boundary error was found.

Reviewed `resume199-tls-cmp-next.md`, SHA-256 `c22039c9f9e4c7fd29a79e398fc54a94222ab6b612272dedd12893f80fe8968f`. Read CP199's producer/owner audit, both CP198 plan/review reports, original `resume163-connector-qobject-static.json` qualifications and the wave2 public QObject ownership contract. Inspected the current Session, WidgetRuntime, factory/header and focused factory-test source. CP198 remained under its owner's edits during inspection; source presence and appended tests are not a stable acceptance or freeze receipt. This review neither runs nor approves that implementation.

## Owner and issuer assessment

The existing source has a real adapter ownership root: ConversionSession constructs and associates the allocation store, shared WidgetRuntimeState and TaskFactoryRuntime, then privately calls `bindSessionReportNullCaller`. That binding validates a pristine native-profile factory and the exact shared owner/control block. Its later frame binding validates ordinal-zero Report/task/private, saved-parent token, initializer-call and frame occurrences. Existing WidgetRuntimeState QtCore shared-null symbols and its existing WidgetThreadState support a symbolic namespace and execution-context identity. They do not establish native TLS initialization.

The TLS symbol/read issuer is **new proposed source**, not an already available receipt. The plan states this correctly. Its contract is sufficient because the private Session binding grants issuance, the WidgetRuntime root independently retains/authenticates the read, and the factory independently retains the issued transition. A new record containing matching hash/RVA/frame fields therefore cannot authenticate itself. A private constructor alone would be insufficient without these trusted retained handles and live-scope checks.

`UnknownRead32` means an unconstrained 32-bit value read at this modeled occurrence. Its owner attests the expression's provenance and lack of a concrete observation; it does not attest that an initialized TLS index exists. Fresh occurrence identities must not assert unequal numeric values across two reads, nor imply equal values from the same symbol. Binding the existing thread identifies the instruction's context; the index cell remains module/process-global. No new thread, physical module, loader epoch or initialized-index owner follows.

## Independent instruction and producer checks

Read-only PE parsing independently verified the original DLL SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`, CMP bytes `83 3d cd 72 2c 00 ff`, next RIP `0x2961b`, and resolved target `0x2961b + 0x2c72cd = 0x2f08e8`. The entire executed range is `[0x29614,0x2961b)`. Opcode 83 sign-extends `ff` to dword `ffffffff`; this is a 32-bit memory subtraction for flags, with no result write.

The same check found file-backed `ff ff ff ff` at raw offset `0x2ee4e8`. These are PE initial bytes, not this frame's running cell. The producer audit identifies imported TlsAlloc at `0x29686`, EAX store at `0x2968c`, and TlsFree/reset candidates at `0x1a620f/0x1a6215`. The retained initializer agrees on the allocation/store sequence. Those static producers establish neither execution time, allocation success nor no-reset/no-overwrite persistence. The audit's import and reset qualification is retained; this review did not rerun its complete import/direct-reference audit. Neither the PE sentinel nor the Session's allWidgets startup qualification may supply x.

For x in the unknown dword expression, `r = (x - 0xffffffff) mod 2^32` is correct. CF is unsigned borrow, PF parity of r's low byte, AF auxiliary borrow, ZF tests r equals zero, SF is r bit 31, OF is signed subtraction overflow. All six must use the same operand/result occurrence. ZF denotes x equals `0xffffffff`; no concrete flag or branch bit is available. The plan correctly preserves represented unrelated flags and historical SUB/TEST/JE records while selecting CMP as current.

Following bytes independently match `0f 85 9f 00 00 00` at `0x2961b`, targeting `0x296c0`. They are excluded. Reaching the distinct pre-JNE phase does not select either initialization route, return, execute the `0x2acb6` reload/getter, establish QThreadData, or write private+0x40.

## Predecessor and acceptance gate

Current CP198 source has exactly ten prefix stages, terminal `BeforeTlsIndexCompare`, RSP `S-0xa8`, eight stack writes, generation-2 enclosing return `[S]=0x13144e`, generation-3 innermost `[S-0x70]=0x2acb6`, and second-SUB current flags produced at `0x29602`. Its private `issuedCalleePrefix_` authenticates the immutable terminal handle. The proposed CP199 admission matches this structure and must retain this exact handle as predecessor history.

Current focused tests now include exact boundary slots/flags/nested returns, every split/resume route, coherent rewind, actual Session default terminal, copied issued-state refusal and corruption refusal. These were actively appearing during inspection. Their existence supplies a concrete test design to extend; it supplies no passing run or frozen-pair acceptance. Reconcile the final stable CP198 source, tests and receipt before starting CP199. Keep the ten-stage validator scoped to the retained predecessor; a successor must not index the displacement table or enum arithmetic at ordinal eleven.

The proposed CP199 tests cover the necessary distinctions: actual Session grant, fresh unknown read, six Sub32 functions, unchanged registers/slots/private/count/cache state, exact terminal reentry, copied/foreign/control-block substitutions, scope release/root expiration, coherent rewind and preparation failure. They must inspect trusted issuer state as well as frame state. A failed preparation which leaves a root-side authenticated read installed is a mutation even if the visible phase stays at CP198.

## Concrete implementation requirements

1. Resolve the new schema/API names in source before implementation review: private grant entry point, symbol owner, root-issued read retention, factory-issued CMP retention, and the exact liveness checks used by each validator. Public diagnostic fields and matching strings are never authorities. The plan already requires this; do not weaken it to a factory-local fabricated receipt.
2. Prepare symbol binding/read/comparison allocations and trusted-state changes before one nonthrowing commit, or use an unpublished preparation object. Root-side read retention, factory retention, flags selector and phase must publish atomically. Include all trusted handles in complete refusal snapshots and privileged coherent rewind. Partial rewind must not issue a second read.
3. At terminal reentry authenticate the same issued read/CMP and frozen CP198 history, then stop without allocating another occurrence. After factory/allocation release, retained diagnostic metadata must remain non-consumable even while the WidgetRuntime root survives. Test root expiration separately; diagnostics must not keep it alive.
4. Treat “unknown” as the full dword domain and preserve the exact result/operand dependency. Small evaluator examples may test arithmetic but cannot become production value observations, constraints or branch witnesses.

These are concrete implementation acceptance conditions, not evidence gaps requiring a new original capture for this bounded symbolic instruction. If the chosen implementation cannot meet the private root/grant/invalidation contract, stop before CMP and first add the inert Session-bound symbolic module token described by the plan. Concrete JNE selection remains blocked by the separately qualified runtime value requirement.

Only this review report was written. No source, test, build, GUI, mutable REA database, parent application, DMO, Claude, original artifact or deployment was changed.
