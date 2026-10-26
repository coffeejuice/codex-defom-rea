# CP211 outer debugger decision-core independent review

**PASS for the bounded synthetic decision core; HOLD for any native child or original execution.** The 68/68 fixture result is bound to the checked observer and fixture source hashes and the retained JSONL bytes. It is useful evidence that the actual `OuterObserver` state machine rejects the listed malformed/missing states and classifies synthetic stage sequences. It is not a native debugger validation, original-process observation, nor independent replay of the saved run.

## Scope and checked artifacts

Read the converter subtree `AGENTS.md`, takeover state, CP210 outer-capture plan, CP210 debugger hardening/review, and CP211 runner/fixture/review artifacts. Read the actual `admin/original_outer_loader_debug_probe.py`, `admin/test_outer_loader_debug_probe_fixture.py`, `analysis/resume211-outer-debug-fixture.json`, and `.jsonl`. This was read-only. No original, GUI, native child, build, source fixture, shared recovery state, or app-control session was touched. No GUI-control release is claimed or needed for this offline review.

The core SHA-256 is `f009bcb84470c673b515bbaa18e1bfb4e27c43722ff449c99f420b6420707946`; fixture source is `316f36eba9d807230bd198af430ec853f4c7962586f707e0fcf123fb001d2a3b`. The saved receipt binds both hashes and raw JSONL SHA-256 `8e33784ad5af3a85fa5b6ca6f1f51f343cc0fa2e00c701357e3df0b146b96a32`. I independently recomputed those hashes, parsed all 939 JSONL records, verified the receipt's raw hash, and matched its 68 unique result names to 68 ordered `case-start`/`case-end` pairs. The JSON receipt itself currently hashes to `88337485eceefe1d5cabe4f819e87d16cf7d1ff893cf21d889ea64092231c7ef`; that digest is not embedded in another receipt, as expected.

The saved receipt reports Python 3.12.10 x64, 68/68, elapsed 0.0 seconds at the timer's resolution, and explicitly says there was no native API or original observation. I did not rerun the fixture because its runner rewrites the shared retained receipts. Therefore the runtime result is verified for internal consistency and source binding, not independently reproduced.

## Decision-core comparison with CP210

The principal CP210 decisions are represented coherently in the code:

- Fixed selection is exact B98 path plus guarded caller marker `base+1c94`, nonnull RCX/RDX, and a recorded entry RSP. CP210's stale `1c99` continuation is rejected; `1c94` agrees with the corrected CP209 `1c8f: call 1ca30` evidence. The core still does not re-disassemble those bytes itself.
- The stages are ordered `1cf11`, `1d08a`, `1d0bd`, `1d0d8`, `1d0fb`. Duplicate identity uses full nonzero RAX; acceptance uses `RAX & 255`; full RAX is retained. A witnessed failure disables DR2 and requires the selected return; all five successes followed by zero stays an unknown later route.
- Stage hits require same registered TID generation, expected RSP (`E - body_delta`), and successful guarded return-marker read at E. Return requires `E+8` and the same marker. Recursive/overlapping calls, lost stages, read failures, unacknowledged debug state, unexpected DR6 causes and changed ownership fail closed.
- Debug-state proposals use DR0/DR1 plus staged DR2, leave DR3 disabled, reject any nonzero initial debug-register baseline, and disarm decisions until exact readback acknowledgment. These are state-machine checks only; the class does not touch Windows contexts.
- Event sequence, process creation identity, elapsed timestamp and fixed budgets are checked. A cap invalidates at the cap, matching CP210's “reaching cap means incomplete” rule. The adapter-provided timestamp does not implement a deadline while a native wait is blocked.
- AV disposition is `DBG_EXCEPTION_NOT_HANDLED`; first/last chances must be ordered and pair-identical. CP205 matching requires the expected QtCore fault fields/return sequence and exactly one closed zero-return occurrence on the AV thread, with supplied module/unwind qualification booleans. `cp205_input_contract_matched` is correctly qualified as a predicate result, not proof those booleans came from native evidence.

The negative fixtures materially exercise the shared core: all five routes, wrong/prefix/case paths and caller, partial/out-of-order stages, recursive and cross-thread overlap, TID reuse, all six nonzero debug baselines, malformed registers/context, event/time/cap errors, AV chance/pair/association ambiguity, serializer/write propagation, and closed CLI/guard behavior. Most are decision checks, not Win32 behavior. Several displayed “null route” cases use synthetic stage values and synthetic RSP delta `0x48`; neither is an original observation.

## Receipt limitation

The JSON-to-JSONL binding is sound: the JSON names the observer and fixture hashes and hashes the complete raw JSONL; recomputation agrees. The JSONL preserves emitted event envelopes, acknowledged proposals, successful selected/scalar/return observations, decisions, case outcomes, and summaries.

The phrase “complete per-case input records” is broader than what is serialized. A failing `hit` emits the event envelope and an `incomplete` reason, but generally does not persist the supplied CONTEXT, DR6/readback, guarded-read result, path, or marker that caused rejection. For example, `wrong-readback`, `bad-stage-rsp`, and `selected-read-error-not-zero` do not include those attempted values in their JSONL records. The fixture source hash preserves the deterministic test generator, so this is not a source-binding gap; it does mean the raw receipt alone is not a complete replayable input transcript for all 68 tests. Narrow that claim or amend the fixture before relying on raw records as full reproduction inputs. This does not invalidate the reported pass count, but weakens independent auditability of individual negative inputs.

## Exact blockers before native/original use

The runner has literal `ORIGINAL_RUN_ENABLED = False` and `ORIGINAL_READY = False`. Even if changed, `run_original()` unconditionally raises HOLD because no native launch path exists. The CLI accepts no PID, attach, arbitrary executable, command, address, or path mode. I found no bypass to original execution in this source.

The remaining gates are concrete:

1. Implement and independently review a separate fixed `DEBUG_ONLY_THIS_PROCESS` child transport with pinned target identity, current copied-fixture/manifest preflight, loaded-module identity and byte spans, qualified loader breakpoint, and original body-frame delta. CP211's `0x48` is explicitly synthetic; CP210 requires independent prologue-derived frame relation and byte checks.
2. Implement bounded QString and guarded process-memory reads, actual per-thread `SetThreadContext`/`GetThreadContext` readback/restoration, RF/restart and ordinary-state preservation, creation/exit/TID-generation handling, and test staged native hits plus a normal entry/return pair on a disposable child.
3. Implement qualified native x64 unwind and module/closed-occurrence association to bind the selected Open to CP205. Raw stack-word scanning and synthetic qualification flags cannot satisfy this.
4. Implement exact-handle ownership generations, every event continuation and file/event handle closure, pending-event failure drain, real Win32 failure/partial-mutation injection, deadline/error handling, exact-child termination, and independent post-debugger process-state checks. Forced cleanup must not be reported as natural capture completion.
5. Persist actual native events/continuations and AV core/stack/chances/exit/cleanup receipts with source, runtime, target/module and fixture-before/after bindings; independently review final transport/observer/fixture hashes and readiness before any one-shot original run.

The target constants pin the stated original path/hash and the proposed B98 command/fixture digest, but this synthetic run did not verify the currently installed target, PE version resource, loaded modules, fixture contents, or before/after equality. The retained qualification says installation v11.0.2 and null PE version resource; it is not a new executable version-resource verification. CP207's CP205 parity is prior evidence and is not inherited by this new runner.

**Disposition:** decision-core semantics and synthetic cases PASS with the receipt-input caveat above. Native child readiness and original observation remain HOLD. This review does not authorize changing either guard.
