# CP195 connector implementation

The converter now models only JE at QtCore RVAs `[0x131438,0x13143a)`. Known-null P selects the taken successor `0x131449`; known-nonnull P selects fallthrough `0x13143a`. Both stop before their successor instruction. Unknown parent nullness remains fully admitted at CP194's existing `0x131438` guard without mutation or guessed path.

The implementation follows `resume195-connector-next.md` and its PASS review. The task owner released implementation after the CP194 freeze at `build/wave3-checkpoint194-verified`, identifying source SHA-256 `2ca0040ec38962efe2cd8eb01010448d824a02e42df9701e4acd95a9d490ff77`. No shared freeze/build/state files were edited in this worker lane.

## Evidence and bounded effect

The already-read `resume163-connector-qobject-static.json` qualifies the retained disassembly as an unwind-region view of a read-only original, not a complete logical-function or native-runtime proof. The qualified QtCore digest is `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; its caller-listing digest is `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9`. The listing was rechecked at preferred VAs `0x67131438`, `0x6713143a`, and `0x67131449`: JE is `74 0f`, the fallthrough begins a load, and the taken successor begins a call. Image base subtraction is `0x67000000`.

Two separate terminal phases and a `ParentBranchPath` marker record the qualified branch decision. Their derived cursors are `0x131449` for TakenNullParent and `0x13143a` for FallthroughNonnullParent. Admission binds the phase and marker to the exact saved/restored parent refinement and revalidates CP194's TEST outcome, including ZF. Unknown nullness cannot qualify either branch terminal.

The new JE transition changes only its path marker and phase. It preserves current TEST flag expressions, RDI and all other modeled registers, stack slots, sparse memory, links, owner/control-block qualifications, counts and incomplete state. No load, call, TLS/current-thread action or task completion has been implemented. The diagnostic instruction budget extends to 0..17, default 17; budget 16 retains CP194. Unknown P validates the full CP194 prefix before returning the existing guard at `0x131438`, preserving the retained prototype.

## Focused coverage and static checks

Complete snapshots now include the branch marker. New tests exercise both known branches, exact cursor/path mapping, full non-control-state equality, consistent rewind/resume, all valid terminal budgets and same/different keys, plus invalid-budget refusal snapshots. The default Session test exercises unknown P under all admitted budgets, full snapshot equality, no successor prediction/marker, one retained prototype and zero tasks. A 27-case corruption matrix checks the unknown predecessor, both known predecessors and both branch terminals with smaller/full budgets; it covers wrong/missing marker, phase-only and marker-only rewinds, forged unknown successors, saved/restored RDI and expired/foreign/aliased owners, stack-slot mismatch, TEST/ZF disagreement, inherited links/spill/registers and completion flags.

The retained 118-case inherited-corruption matrix now additionally checks both CP195 branch terminals with both smaller and full budgets. Earlier cuts keep their original budgets. Invalid-budget probes were advanced to 18 while 17 became admitted.

Static preflight passed: 19 phase/stop entries for the straight-line prefix plus two terminals; budget range/default; JE-only assignments; unknown guard; exact successor mapping; existing Testkit main; supported CHECK/REQUIRE assertions; no C assert or new quoted includes. No build, executable test, full suite, GUI, original execution, REA database mutation, deployment or shared-state update was performed. Build/test/freeze claims remain the integration owner's responsibility.

Released files: `source/apps/moproj2mst/src/doe/task_factory.cpp`, `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp`, `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp`, `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp`, and this report.
