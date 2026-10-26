# CP186 connector independent source review

**PASS — no source blocker found for the bounded POP RDI cut.** This is a read-only source and test review against `resume186-connector-next.md`, `resume186-connector-plan-review.md`, and `resume186-connector-implementation.md`. Compilation, executed focused/full regression, original comparison, candidate freeze and integration receipts remain separate acceptance gates.

The reviewed implementation covers only `[0x13123e,0x13123f)`, `5f pop rdi`. The retained initializer/caller listings and their `resume163-connector-qobject-static.json` qualification describe unwind regions, not proof of complete logical functions or runtime constructor behavior. They support push RDI at `0x131165`, caller RDX-to-RDI at `0x1313eb`, the private call at `0x131417`, and the excluded RET at `0x13123f` / caller continuation at `0x13141c`.

## Provenance and transition

`doe_task_runtime.hpp:142-163` represents saved entry RDI independently from private RAX and restored task RBX. The saved record binds owner/control block, Report ordinal, task identity, allocation-time private identity, module hash, initializer/save/origin/move/call/return RVAs. Its address is an opaque symbolic token; null restored-token state denotes absence of POP, with no claim about native parent bits.

`task_factory.cpp:178-181` creates the saved record with the initial qualified frame, before publishing that frame. `:213-235` validates the retained record and both owner pointer and control block before resumed writes. It rejects missing or foreign provenance, premature restored tokens, incorrect live displacement, and wrong saved/return-slot displacements; it does not recreate or repair a persisted record. CP186 requires its restored token to identify its own retained saved record.

The instruction loop at `task_factory.cpp:590-676` keeps stops 0 through 7 and adds stop 8. Case 7 assigns only restored RDI and live RSP zero, then advances phase to `BeforePrivateReturn`. These scalar publications cannot allocate or throw. The saved slot/provenance remains intact. Historical RBX `[S+0x10] = [historical RSP+0x48]` with displacement `-0x38` remains independent from live RSP; CP185 is `-8`, CP186 is `0`. No flags or private memory store is added by POP.

## Retention and refusal

The successor allowances at `task_factory.cpp:187-196` retain valid RAX/private and RBX/task identities. The final loop revalidates the sparse stores, allocation identity, root/control-block bindings, version argument, flags-expression stage and incomplete/unpublished state before stop/refusal. Its terminal ordinal always meets the validated maximum budget of 8, so it throws before any later transition. No RET, caller `0x13141c`, task publication `0x131425`, private backlink `0x131431`, parent relation or QObject completion is admitted.

`createTaskBase` calls initialization before requested-key lookup. Thus same-key, different-key and unknown-key calls all validate the retained terminal before refusing, without replaying POP. Lower budgets preserve a validated CP186 terminal. Invalid ordinals 9 and maximum size refuse before mutation.

## Test-source coverage

Snapshot helpers at `test_doe_task_factory.cpp:72-145` include every saved-RDI provenance value, independent weak-owner/control-block comparison, restored token and live/slot stack fields. The CP186 tests at `:5442-5625` cover a 57-case corruption matrix at predecessor and terminal, all three key classes, exact success effects, RAX/RBX retention, unpublished state, all budget reductions, CP186-to-CP185 rewind/resume, every earlier flags stop, premature-token refusal at every earlier phase and corrupt-provenance nonrepair after rewind. The exact-effect test restores only phase/RSP/result before comparing the complete predecessor snapshot. The invalid-stop fixture at `:5030` admits 8 and retains 9 and maximum-size rejection. Existing explicit CP185 fixtures continue to select 7.

No source, build output, GUI state, deployment, REA database or shared recovery-state file was changed. No test or native executable was run. This review wrote only this report and establishes source consistency, not fresh native GUI lifecycle acceptance.
