# CP202 synthetic visibility fixture review

Decision: **PASS for a guarded installation attempt on one separately authorized, owned original held suspended.** This is not a full fixture lifecycle PASS, proof of original interception, or permission to resume after an installation/readiness failure. The revised original runner must retain its existing independent review gate until the coordinator completes its integration review.

Reviewed the preflight report, current agent/runner, both retained JSON results, the static candidate qualification and conditional plan review. Recomputed all four retained hashes: agent `86fbc278d6015b04db91204600e939729eea7497e52d4b0d61c3a903ea67a034`, current runner `038827368e64fbedceb1d9b02b7ea41897ee1bece734a4a166e65243df4baec8`, attempt 1 JSON `a3c61d9116546b32079ca3c64e25801355c469ee71b7b7b0805abff31d31df7f`, current result JSON `0988708a188dec60ce4c2801973b2e91f5cbde6f562ec292a79ec8da9a6593af`. Attempt 1 records the same agent hash and a different historical runner hash; the current runner is not represented as the exact source of attempt 1.

## What passed

The agent's 38-byte candidate is exactly the six load instructions qualified at original RVA `0x1547e` through `0x154a3` in `resume202-visibility-hook-alternative.md`: `488b842450020000488b8020040000488b8c2450020000488b8920040000488b000fb6542474`. The synthetic wrapper stores the actual NativeFunction main argument at post-allocation RSP+`0x250`, the byte at RSP+`0x74`, and uses main+`0x420` -> dock -> vtable. This is an executable fixture, not merely byte comparison.

Both owned Python children (PIDs 9352 and 36272) report Frida package/runtime 17.23.2, x64 and QJS. Their two hooked callback observations recover the allocated main/dock pointers and raw bytes in order 0,1 from callback RSP. All six baseline/hooked/restored calls per child return the requested value and retain the expected post-block RAX=vtable, RCX=dock and EDX=0/1. Returning through the wrapper and successfully writing output through R8 provide additional limited execution evidence. This does not test arbitrary register/flag preservation, unwind behavior, exceptions or every ABI state.

The retained 64-byte before/patched/restored strings show an actual entry patch and exact byte restoration. Both listeners detached and both restored executions passed before host teardown. Observed changed offsets 0..6 are byte differences only; they do not establish Gum's full overwrite, relocation or trampoline length. No trampoline body was captured and separate Gum build identity was not exposed.

## Lifecycle qualification

Both result outcomes remain `FAIL / incomplete`: attempt 1 timed out script unload at 2000 ms; attempt 2 timed out session detach at 5000 ms while the primary child thread remained suspended. A Promise.race timeout does not cancel the underlying operation or prove its eventual outcome. Listener detach/restoration is distinct from script unload/session detach. Neither successful unload nor successful pre-termination session detach is established.

The helper receipts show no resume, retained-handle termination success, wait-result 0, exit-code 57857, verified exit, handles closed and helper exit 0. Fresh Windows CIM and same-device Frida queries found each owned PID absent, without recorded verification errors. These are positive termination/absence receipts, not evidence that a timed-out detach settled. No GUI session was acquired and no app-control release claim follows.

The fixture runner also incorrectly tests `!session.isDetached` as a boolean property in its final cleanup guards. Frida 17.23.2 exposes a method; the truthy function object can skip those final calls. Consequently the records cannot support a claim that post-termination unload/detach was retried or settled. This does not negate the in-agent detach, restoration, execution checks or held-handle cleanup.

## Mandatory conditions for the next runner

The original runner must use the checked `session.isDetached()` method, terminate through the retained process handle before potentially hanging final Frida teardown, retain bounded errors as uncertain outcomes, and keep observed PID absence separate from settled session cleanup. The inspected `admin/frida_original_save_probe.mjs` already implements these conditions in `sessionDetached`, `settleSessionCleanup` and `cleanup`; no additional original-runner source change is required by this fixture result. Do not copy the fixture's property guards. If the fixture runner is reused or represented as lifecycle-ready, repair those guards first and keep historical failures intact.

Before the one permitted original resume, retain fresh exact image/module identities, actual module-base+RVA prefix checks, all four EXE listener receipts, active module-observer continuity, script readiness and the existing late-Qt readiness contract. Installation error, script error, incomplete readiness or setup budget exhaustion must take the no-resume held-handle termination lane. Do not weaken six-hook coverage because this fixture passed. A live original installation attempt remains necessary: the fixture uses page+`0x40`, not original alignment/ASLR placement, original unwind metadata, branch entries or surrounding production code, and does not call the original dock virtual or Qt.

The full preflight lifecycle gate remains incomplete. The successful executable layout test plus independently verified bounded owned-child termination is sufficient for the next guarded installation attempt, whose failure cannot resume startup. It is insufficient for claims about delivered Show, signal/slot delivery, Save, accepted Close or the cause of the earlier interception failure.

Read-only evidence/source review; only this report was written. No process, GUI, original executable, REA state, build, deployment, parent DMO or Claude state was changed. No reconstructed DEF_PRE instance was used.
