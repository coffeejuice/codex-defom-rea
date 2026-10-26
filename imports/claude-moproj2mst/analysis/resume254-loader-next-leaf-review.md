# CP254 loader next-leaf review — PASS

The revised design resolves the three prior HOLD findings. No outstanding design defects found within its explicitly bounded current-provider/stable-storage domain. This approves a migration plan, not implementation or runtime acceptance.

The report now separates current C++20 RHS-before-LHS sequencing from retained native d0.c's second-control-before-virtual-call sequence. It preserves the current source sequence without presenting it as native timing equivalence, and grounds the production equivalence in the actual pure MultiBlowClass::v120() returning 2. Side-effecting/future providers and reentrant storage/barrier changes are explicitly outside that equivalence.

The unchecked second-access precondition is now explicit. The command's nullable no-op is identified as its value API contract rather than original null behavior, and the production relocation is restricted to valid synchronous stable storage after the first successful check. Do not claim that the direct absent-step test validates the old unchecked dereference. The command remains void, must not be noexcept because simCtl may throw, and must perform exactly one simCtl admission/write path. Retaining the caller's first check gives two current-source admissions total for this final segment without a third control lookup.

The test plan now accurately describes recorded save.writes path/byte checks and open.deletes, with no asserted full .moproj equality. It requires actual executed/pass/skip accounting for optional Oracle B 93/93D/94 cases and a narrow independent counted-provider caller test if those cases are unavailable/all skip. Ordinary command success, unrelated-field preservation, direct parked refusal with whole-step/effect checks, and absent-step no-recreation give meaningful coverage of the new owner boundary. A sentinel hook variation retains integration output checks without claiming dynamic intermediate ordering.

Retained decompile/spec and historical Save outcomes remain appropriately qualified. CP249 completed heating-unit work is now distinguished from the blocked broader projection. No C ABI/native layout, identity, original timing witness or wider alias-closure claim is introduced.

This supersedes the earlier HOLD after checking corrected sequencing, second-access precondition and harness evidence. Only this review report was written. No source/test/build, shared recovery state, original process, GUI or mutable REA changes were made.
