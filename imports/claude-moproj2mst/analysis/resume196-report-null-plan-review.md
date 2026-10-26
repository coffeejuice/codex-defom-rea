# CP196 Report caller-nullness plan review

## Decision: PASS for the bounded source proposal

`resume196-report-null-binding-next.md` supports qualifying the actual Session-owned first pending Report token as statically null and reusing CP194/CP195 to stop before QtCore RVA `0x131449`. No additional instruction, parent-slot observation, helper effect or constructor completion is justified. This is approval of a plan, not a finding that the proposed private issuer already exists or that its implementation is verified.

## Evidence checked

I read the parent-slot issuer and QObject import-binding reports, the qualifications in the CP160/CP163 captures and prototype-constructor inventory, the public-QObject ownership contract, and the immutable-import qualification in `resume46-doe-input-imports.json`. I checked the relevant retained instruction lines rather than interpreting unwind regions as whole logical procedures.

The first nonnull `0xc0` allocation in `resume163-connector-prototype-140137610.asm` reaches `0x1407b89e0` at `0x140137644`. Its entry calls `0x14157ead0` at `0x1407b89f7`; that entry calls `0x141403210` at `0x14157eae7`. The contiguous TaskBase listing clears EDX at `0x141403222`, reloads RCX at `0x141403224`, and calls IAT `0x141e041c0` at `0x141403229`. No intervening instruction changes RDX. The retained PE receipt names the public QObject symbol; the ownership contract identifies actual export `0x671313d0`, whose bounded entry listing saves RDX into RDI at `0x671313eb`. The protected constructor is a separate entry.

Current production ownership in `conversion_session.cpp:162..172` constructs the allocation store, widget root and Native factory, then establishes the canonical store/widget pairing. `createTaskBase()` runs `initializeNativePrefix()` before inspecting the requested key. Its first pending allocation and fresh frame are therefore the existing source-model occurrence to qualify. Native profile, requested key or matching widget pointer alone would not distinguish this Session route from standalone fixtures. The proposed private Session-only operation supplies that distinction without claiming execution of the original caller.

I inspected current `doe_task_factory.hpp`, `doe_task_runtime.hpp`, `task_factory.cpp`, the allocation-store pairing/allocation definitions, and the focused task-factory tests. The proposed capability is new: current public `SavedEntryRdi` records accept synthetic nullness and cannot by themselves authenticate production provenance.

## Admission and propagation

The plan requires a privately constructed immutable capability, weak owner bindings with control-block comparison, exact factory occurrence, and a separately issued frame/token occurrence. Those requirements are sufficient at the normal C++ API boundary if implemented as written. Public strings, enums, aggregate field equality or copied diagnostic nullness must never substitute for the private issuance record. This is not a memory-corruption security boundary.

Fresh issuance before publishing `qobjectPrefix` is the correct cut. Existing unknown frames must not acquire the qualification after POP or TEST. Current POP copies both the token and its optional nullness; the retained validator compares the restored value to the saved token. TEST recomputes its outcome from that same token; known-null gives ZF set, SF clear and even parity. JE changes only path and phase, selecting `TakenNullParent` and derived cursor `0x131449`. The retained listing contains the helper call at that cursor, so stopping there admits no helper call. Default budget 17 already contains this JE transition; no eighteenth instruction is needed.

The proposal explicitly preserves unqualified standalone unknown behavior at `0x131438` and standalone synthetic null/nonnull diagnostics at `0x131449`/`0x13143a`. Parsed DOE projection remains a separately selected profile and supplies no Report caller qualification. This separation is consistent with the source and the existing tests.

## Concrete implementation acceptance checks

These make the plan's stated invariants reviewable; they are not missing original-runtime evidence needed to approve this bounded plan.

1. Record Session-source eligibility independently of the nullable capability handle. Removing both frame qualification and capability must refuse on the Session route, rather than silently downgrade to the unqualified diagnostic path. Test missing factory capability, missing frame binding and simultaneous removal. The plan already requires missing-capability refusal and rejects incompatible retained frames; enforce it before any allocation, cursor or refcount mutation.
2. Check the first Report occurrence explicitly, including ordinal zero and the pristine Session store at binding. Do not implement “current pending ordinal” as permission to qualify an arbitrary later Report after externally prepopulating the store. Test a preallocated Report and a wrong pending ordinal without mutation.
3. Keep a non-reissuable per-frame occurrence record outside public mutable provenance fields. Resetting/replacing a prefix, private allocation or saved token must not cause the fresh-frame block to mint a replacement automatically. Address equality alone cannot establish an occurrence after optional reset or allocator reuse. Bind after the frame has its stable published location, or use an independently stable occurrence identity without weakening exception safety.
4. Extend complete snapshots to include factory eligibility/capability identity and issuance state, frame/token source identity, weak owner control-block identity, and all fixed source/module fields. Existing `savedRdiValues()` and `frameValues()` do not yet include these proposed fields. Snapshot the damaged state immediately before refusal and require equality afterwards, including counts and factory maps.
5. Preserve the existing standalone CP194 and CP195 suites. Replace the two obsolete actual-Session unknown assertions near test lines 7826 and 7894 with source-qualified assertions, and audit earlier actual-Session integration assertions that may indirectly depend on the old terminal. Add qualified earlier-budget POP/TEST/JE comparisons, coherent rewind, partial rewind, invalid budgets, foreign/expired/copied capability, aliased control block, late binding and post-Session-scope tests. Valid capabilities in tests must come from a real `ConversionSession`; friend diagnostics may damage them but must not manufacture the positive case.
6. Run validation before fresh/resumed writes even with smaller budgets. A terminal frame reentered with any valid smaller budget remains terminal and unchanged. A coherent rewind may retain the same capability but must clear only the phase-dependent restored/test/branch observations together; it cannot repair source nullness or issue another capability.

The focused checks proposed in the plan cover these categories and the existing completion/successor guards. The implementation review should reject a partial matrix that omits the simultaneous downgrade or replacement cases above.

## Limits and hard stop

The retained reports record matching SHA-256 identities for `DEF_GUI_MO_64.exe` (`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`) and `QtCore4_64.dll` (`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`). I consumed those reports and bounded listings; I did not independently parse, rehash or execute either PE. Static import identity and the listed zero argument do not prove runtime loader resolution, caller reachability, native register capture or current thread state.

The historical ownership contract describes later thread/helper behavior, but CP196 supplies none of its required runtime inputs. Keep the exact stop before the call at `0x131449` (target RVA `0x2aca0`): no TLS query, helper/thread ownership, reference/counter effect, parent `+8` or `[rcx+0x40]` read, TaskInfo, cache publication or completion. Keep one pending prototype, zero completed tasks and incomplete flags. No unsupported runtime inference was found in the proposed bounded decision when read with its explicit qualifications.

Only this review file was written. No source, test, build, GUI, REA database, original artifact or deployment was changed; no tests were run.
