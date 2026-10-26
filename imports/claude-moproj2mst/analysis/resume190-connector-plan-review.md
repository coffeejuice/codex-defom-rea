# CP190 connector independent plan review

## Decision

Pass for implementation of the bounded publication transition. No blocking plan defect found. This is a read-only review of `resume190-connector-next.md` against retained QtCore evidence and the CP189 source; compilation, tests and implementation review remain required.

## Evidence and instruction boundary

Read `resume163-connector-qobject-static.json` before interpreting its caller listing. It qualifies the capture as an unwind region, not a complete logical function or runtime proof. Independently rehashed QtCore as `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` and the caller listing as `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`. Read-only binary bytes at offset `0x130825` are `48 89 43 08 4c 8b e0 48 89 44 24 58 48 89 58 08`, agreeing with the retained listing.

The plan correctly selects exactly RVA `[0x131425,0x131429)`, `mov qword ptr [rbx+8],rax`. CP189's `EB 05` skips the null arm and stops at this store. Its qualified RAX is the allocation-bound private identity; restored RBX is the pending public Report task. Therefore the new observation must represent public task+8 receiving the private token. The successor cursor is `0x131429`. R12 assignment at `0x131429`, caller stack spill at `0x13142c`, and reverse private+8 store at `0x131431` are outside the cut.

## State, admission and refusal

The CP189 factory qualifies Report ordinal, public/private allocation sizes, address points, original private identity, matching live widgets and shared-null bindings, sparse zero fields and flags expression, initializer provenance, saved owners/control blocks, RAX/RBX identities, restored RDI, symbolic stack and CALL/RET lineage. The plan preserves these guards. A publication value plus marker is necessary: the existing boolean alone cannot distinguish a foreign or absent task+8 token on reentry.

`qualifyRetainedZeroPrefix` currently rejects either backlink marker unconditionally. The plan explicitly requires phase-sensitive admission for the new terminal, retaining strict refusal before publication and refusing reverse publication/completion everywhere in this cut. Implementation must also extend all early phase-dependent register/flags/stack guards to recognize the new terminal as returned, RDI-restored and XOR-completed. Adding it only to the loop table would fail reentry before terminal validation.

The proposed nonthrowing commit of token, marker and phase after complete qualification is appropriate. It changes only the public task+8 observation and derived cursor. `private08` remains qualified qword zero; `privateTaskBacklinkAvailable`, `complete` and `qobjectBaseCompleted` remain false. No allocation, reference delta, R12/spill observation, final task or cache may be manufactured.

## Acceptance gates

The proposed tests cover the required behavior. CP189 tests at `test_doe_task_factory.cpp:5988` onward provide corruption/refusal snapshots, cursor-only preservation and all-budget reentry/resume patterns. Extend the full snapshot helper with the new value observation; otherwise refusal snapshots could miss its mutation. For exact preservation comparison, consistently rewind the publication token, marker and phase before comparing with the predecessor, then restore all three.

Require one CP189-to-CP190 advance proving the correct published token and `0x131429`, unchanged private08 and all other represented observations. Require terminal reentry at budgets 0 through the new maximum and all three task keys, consistent rewind/resume, null/foreign/missing token and marker combinations, premature publication in earlier phases, wrong/invalid phase, existing owner/control-block and provenance corruption, and reverse-link/completion refusal with no further mutation. The current maximum is 11; CP190 should admit 12 and reject 13 and maximum `size_t` before allocation and at a retained prefix. Terminal validation must precede the stop and prevent `ordinal+1` overrun.

## Review boundary

Reviewed source hashes: `task_factory.cpp` `3fba67426cd5cfade4518c70864b1c599b2442ed8aa53954b8cb78ab2242a0f5`; `doe_task_runtime.hpp` `b5e30d49112f395229744ab216e687930f56639c2871c81d4b4d1785256381ac`; `doe_task_factory.hpp` `78583e17b7f3f4c71b0dacfc95e4a02686005524624d108ed177eaacc44a8aa2`; `test_doe_task_factory.cpp` `10af80fce384284eb140803732ad5dedba64fff0c8c670570a31605ce2b1c5e0`.

No source changes, builds, test execution, GUI, REA database, deployment or Git transport operation occurred. Only this review file was written. Static evidence supports the local instruction effect and ordering; it does not establish successful native construction, concrete pointers, prior task+8 contents, faults or concurrency guarantees.
