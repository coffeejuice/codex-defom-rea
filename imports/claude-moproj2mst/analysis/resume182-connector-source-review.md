# CP182 connector independent source review

## Verdict

**PASS for source semantics; one remaining planned test-coverage gap.** No blocking implementation defect found in the released source. The explicit two-genuine-allocation B-origin separation assertion requested by the plan review is absent. Compiled focused/full regression, comparison and freeze acceptance remain the parent's responsibility; this static review ran none of them.

Read `resume182-connector-next.md`, `resume182-connector-plan-review.md` and `resume182-connector-implementation.md` before reviewing the implementation. Their qualification is static selected-path evidence in a retained unwind region, not concrete allocator contents, live occurrence, all callers, exception/cleanup behavior or QObject completion. No new native behavioral claim is made here.

## Exact cut and symbolic provenance

`task_factory.cpp:536-596` implements four separate nonthrowing publications. Read creates allocation-bound EAX B at `BeforeFlags20And` / RVA `0x131227`; AND advances to `BeforeFlags20Or` / `0x13122c`; OR advances to `BeforeFlags20Write` / `0x13122f`; only the final instruction publishes memory and advances to `BeforePrivateReturnSetup` / `0x131232`. There is no return-pointer setup or completion publication.

`doe_task_runtime.hpp:30-64` retains private allocation identity, offset `0x20`, width 32, read RVA `0x131224`, original knowledge masks and Read/And/Or stage. Defaulted equality compares all these fields. Production creates unknown B with zero source masks, and guards compare the complete expression against that unknown origin at every cursor, so forged input knowledge is refused. Identity remains stable across continuations; no intermediate read is performed. Distinct simultaneously retained allocations have distinct private pointees and therefore distinct expressions by construction.

The transforms preserve `(B & 0xfffffcf0) | 0x60`. Final known-one/known-zero/unknown masks are `0x60` / `0x30f` / `0xfffffc90`; AND's zero mask is `0x30f`. Memory stays disengaged through OR and must exactly equal EAX after the store. The adjacent `private24` zero cannot seed B. EAX is modeled as a dword expression; no full numeric allocator value is fabricated.

## Guards, budgets and refusal preservation

Every current CP182 phase, including terminal reentry, invokes `qualifyRetainedZeroPrefix` and checks canonical QVector/second QList/version provenance, all thirteen prior zeros and exact symbolic prefix before publishing anything. The shared qualifier checks pending Report ordinal/pointee, allocation-time private identity, sizes/address points/written flags, root association, first QList/QString pointee and control-block pairing, incomplete base and both unavailable backlinks. `TaskAllocationStore::ownsWidgetRuntime` also compares the associated root's pointee and control block. No root count prerequisite or reference event is added.

Invalid instruction stops above 4 refuse before initialization. Existing batch stops 1..6 remain separately controlled; CP182 stop 0 preserves CP181 and stops before the read, with default 4 admitting all four instructions. Unchanged targets refuse without replay; raised targets resume. Exact phase/expression/memory checks precede each instruction, so later refusal preserves accepted partial state and cursor. Terminal state is validated before its stop diagnostic. Unknown phases take the existing unsupported fallback without mutating the frame.

## Test audit and remaining gap

The owned factory tests add all five CP182 cuts, same/different/custom-key reentry, incremental resume versus the same uninterrupted full frame, default ConversionSession progression, full/partial pure known-bit transforms, invalid stop values, missing private state, symbolic field/phase/ordinal corruption and all thirteen missing/nonzero predecessor fields. The existing owner/root/binding/version/completion matrix and root pairing matrix include the five new cuts. `Cp180Snapshot`, frame/private tuples and predecessor rewinds include/reset both symbolic slots; root deltas and available baselines are retained in refusal snapshots.

**Coverage gap:** the plan review's explicit distinct-allocation origin separation check is not directly asserted. `doe_task_factory_independent_frames_share_root_symbols_and_accumulate_symbolic_deltas` at `test_doe_task_factory.cpp:1455` creates two genuine allocations, but only checks their shared QList/QString symbols and deltas. Add assertions that each EAX/stored expression uses its own allocation identity and that the two expressions differ. The source is correct by static inspection; this is an acceptance-coverage item, not evidence of a runtime defect.

Cross-test search found no stale ordinary CP181 `0x131224` terminal expectation outside the owned factory file. `test_doe_session.cpp:1008` intentionally expects `0x131185` because its native factory has no matching WidgetRuntime root; it cannot reach CP182. Remaining factory `0x131224` expectations are deliberate CP181 stop-0 coverage. Ordinary owned tests now use `0x131232`; earlier seam diagnostics remain intentional.

## Reviewed source SHA-256

All four files match the implementation handoff receipts:

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `fe6d7ba01361a13272e7bb311b279c346acb06ada0891589d81037e547055756` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `c89fd9e4087b0959e3ea003d16339febd41fd1d9421aac4120eb913e71c6424e` |
| `source/apps/moproj2mst/include/m2m/doe_task_factory.hpp` | `c37d45580af418035251f945b9b9a432b8a9ce180273438d66b6de37ed828410` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `f4fcbd528d5dea46c8e7e3d71c831273361b616e17a983bfd9584c80fe96282e` |

Only this report was written. No source/test edits, build, executable generation, executed tests, GUI-control session, live REA/database operation, deployment or shared recovery-state changes were performed.
