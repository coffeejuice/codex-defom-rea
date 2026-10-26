# CP210 outer-loader Frida independent review: PASS for one controlled original trial

**PASS for exactly one fresh owned original fixed-command trial**, with the reviewed pre-resume five-byte patch gate and independent host validation mandatory. This supersedes earlier HOLD verdicts in this report. The new gate resolves the concrete native patch-mode blocker by refusing to resume unless every entry is an E9 rel32 replacing exactly its first complete five-byte instruction and every remaining qualified prefix byte is unchanged. No original execution or GUI control occurred during this review.

This is a technical release recommendation for one bounded observation, not proof of original neutrality or a successful Open/Save/Close lifecycle. Root owns the method/release decision and guarded release changes. No fallback mode, automatic retry, extra hooks or broader original interface is covered. Keep the current false guards until root records its release decision; any substantive observer change reopens review.

## Exact reviewed identities

| Artifact | SHA-256 |
|---|---|
| `admin/original_outer_loader_frida_probe.mjs` | `9e584f00ad1b6eeef6a384e069cf6d3e075060a6de062220488d5fcf829cbe58` |
| `admin/test_outer_loader_frida_probe_fixture.mjs` | `69eeb32bb3d581c8243f16edae7c547674538f53c86a95941972c9c3802d6d82` |
| `admin/test_outer_loader_native_patch_gate_fixture.mjs` | `eea1f09fc940193ae372de35e5e593582baf22fe9d12fce4b7012a4dca6e4854` |
| `admin/owned_suspended_process.py` | `8f770388862580700063254663ac029eaf45ee269a3528aec3e61f946c32411d` |
| Exact-shape native fixture executable | `6f1ab2fa4bdf47a80ceda881f293e522333fae551a8c30a9e1d15600b473a83c` |
| Copied original PE | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |

Target is original installation v11.0.2 `DEF_GUI_MO_64.exe`; retained PE version-resource qualification is null, distinct from installation version. No reconstructed executable is involved. Applied reverse-engineer-anything artifact/runtime evidence qualifications and read the subtree instructions, retained CP209/CP210 main/loader evidence, revised runner/fixtures, raw receipts and `resume211-frida-window-review.md`. Only this report was changed. Root owns recovery/status/shared-state operations.

## Why the final gate closes the earlier HOLD

All six embedded prefixes and direct callers were independently matched to the exact PE in the preceding review. Main calls at `1c8f`, returning at `1c94`. Helper continuations are `1cf11`, `1d08a`, `1d0bd`, `1d0d8`, `1d0fb` for duplicate/factory/cast/Project/acceptance. Every entry begins with a complete five-byte RSP-relative MOV store; none requires a relative-address fixup. The static reviewer conditionally passes five-byte windows and reports no explicit incoming interior reference in its wider tested windows, without claiming indirect-target absence.

After installing hooks the actual generated agent calls `Interceptor.flush()`, reads all qualified prefix bytes again, and requires opcode E9, five-byte redirect, and byte-for-byte unchanged suffix after byte five. It records signed rel32 and destination. Any mismatch sends installation-failure and throws before handler-ready. The actual host independently validates all six ordered names/RVAs/addresses, before/after bytes, lengths, opcode, suffix, signed displacement and recomputed destination, then refuses resume if any failure exists. The message handler also immediately requests owned termination on installation failure. There is no allowed fourteen-byte or alternate redirect mode.

This makes the earlier fourteen-versus-sixteen-byte factory uncertainty inapplicable to a resumed trial: every larger patch or suffix alteration aborts before resume. Likewise the original shared RTTI PUSH sequence and Project SUB/immediate store stay at their original addresses after the first five-byte store. There is no need to qualify their displacement under this admitted patch mode. The exact-shape native fixture now executes all six original prefix shapes, including RTTI and Project, and checks caller stack/nonvolatile registers plus synthetic RCX/RDX preservation. The same first-instruction relocation mechanism is exercised with real pinned Frida 17.23.2.

Private Frida trampoline bytes/unwind registration were not inspected. The receipt establishes the entry redirect and unchanged suffix, plus executed native callback/state behavior; it does not establish every private implementation detail. This residual uncertainty is acceptable for a single owned scientific trial with fail-closed readiness and mandatory first-AV parity. Original-specific scheduling, shared RTTI effects and neutrality remain outcomes to assess from that trial, not facts inferred from the fixture. RTTI has 1,017 static explicit entry callers; its caller filter does not prevent global instrumentation effects.

## Independent fixture and receipt verification

Independently reran the actual-code mock fixture: **PASS, 78 assertions**. It extracts the actual generated callbacks, host pair/AV qualification, patch gate and completion predicate. It covers ordered five-way failure/all-success, AL behavior, normal versus missing return, wrong callers, nested selected and unselected outer calls, pending reentry, wrong thread/frame/path-pointer relation, dropped/reordered events, identity/count mismatch, event/frame caps, late helper errors, partial/overflow transport and completion failures. Patch-specific cases reject unsupported opcode or changed suffix at every site, incorrect destination, missing receipt and absent/failed patch readiness.

Independently checked all nine retained native raw helper/message/agent sets under `analysis/resume210-native-patch-gate-2026-10-26T12-27-02-390Z`, source/executable hashes, agent hashes, raw Windows absence JSON, same-device Frida absence, closed handles, detach, drained streams and helper exit. Confirmed all six native fixture prefix strings equal the original observer's prefixes.

| Trial | PID | Result |
|---|---:|---|
| Unhooked native baseline | 30324 | Natural AV, zero hook events, baseline patch gate deliberately false |
| Duplicate failure | 35200 | Three sequential events, outer zero, admitted six-site patch gate |
| Factory failure | 24348 | Four sequential events, outer zero, admitted six-site patch gate |
| Cast failure | 21652 | Five sequential events, outer zero, admitted six-site patch gate |
| Project failure | 31940 | Six sequential events, outer zero, admitted six-site patch gate |
| Acceptance failure | 23280 | Seven sequential events, outer zero, admitted six-site patch gate |
| All-success | 9444 | Seven sequential events, outer `0x100`, admitted six-site patch gate |
| Intentional pre-resume failure | 15636 | No resume/events; owned termination `0xe201` and exact cleanup |
| Project suffix corruption | 42288 | Agent rejects patch before readiness/resume; no events; owned termination `0xe201` and exact cleanup |

All seven natural children exited `0xc0000005` without termination. Six instrumented children match baseline first-AV module/offset/operation/address. Their events bind exact primary TID and textual creation identity, helper entry RSP = outer entry RSP - `0x1e0`, ordered count/sequence snapshot, no rejected callbacks/state cap/overflow, and expected normal-return values. Independently recomputed E9 signed displacements/destinations and unchanged suffixes for every admitted native receipt. Synthetic AV is a write to `0x58`, distinct from CP205's original read path. Neither synthetic baseline nor fabricated host records establish original CP205 parity.

Only one real pre-resume exception and one real rejected-patch cleanup boundary are covered. Every possible OS/timeout/late-stream scheduling fault is not covered; the actual completion predicate remains conservative on those outcomes. The earlier seven-child NOP fixture remains supporting callback/cleanup evidence, superseded for prologue qualification by these exact-shape receipts. No new native child was launched by this independent review.

## Attribution and acceptance boundary for the one trial

The repaired observer poisons an active frame on every outer reentry before caller filtering. Helpers require unique owner, exact caller, expected order, no pending reentry and exact RSP relation. Missing leave is incomplete, never inferred zero. Bounded frame/thread/event/transport state, first-AV snapshot parity, raw receipt retention and late-error rejection remain required.

Runtime QString characters remain unverified. This is adequate for identifying the first prerequisite failure of the unique **fixed owned CLI Open invocation**, with exact command/cwd/fixture identity, primary thread, corrected caller and main-local pointer relation. Exact main bytes supply RDX = main RSP+`1918`, RCX = main RSP+`40`; their difference `18d8` is independently enforced. This does not prove actual filename characters, encoding conversion or derived DB identity. No parser/allocation subcause may be inferred from one failed outer predicate.

A complete original result must pass the live patch gate before resume and retain exact identity, unique paired invocation, ordered prefix, CP205 qualified first-AV core/backtrace parity, natural `0xc0000005` exit, unchanged fixture, raw receipts, script/session cleanup and dual independent PID absence. Otherwise retain tentative observations and classify incomplete/divergent without a causal acceptance claim. The source still has literal false review guards and unconditional incomplete status; root may make only the recorded release changes needed to permit this reviewed single trial and completion through the already-tested prerequisite conjunction. Record hashes of that released runner and generated agent. No original execution follows from this report alone.
