# CP183 connector independent source review

**PASS — no source blocker identified for the selected one-instruction cut.** The implementation matches `resume183-connector-next.md` and its independent plan review. This is a static source/test review; no build, test execution, GUI interaction, or REA database operation was performed.

## Identity and boundary

`doe_task_runtime.hpp:120–122` adds `returnPrivateIdentity` as the same opaque sparse-allocation token already used by `allocatedPrivateIdentity`. `task_factory.cpp:603` copies that token without dereferencing it, converting it to an integer, or publishing a native address. The new `BeforePrivateEpilogue` phase and diagnostic text identify RVA `0x131235`, exactly after the qualified `mov rax,rbx` interval `[0x131232,0x131235)`. The flags expression remains retained as prior symbolic evidence; it is not interpreted as a concrete allocator value.

The accepted transition changes only the return token and phase. There is no saved-register restoration, RSP adjustment, RDI restoration, `ret`, QObject completion, task/private backlink publication, map insertion, or TaskInfo construction. `createTaskBase` still initializes before requested-key lookup and refuses at the retained boundary.

## Guards, budgets, and reentry

The shared flags path at `task_factory.cpp:550–592` requalifies the allocation/owner/root prefix before the CP183 move and again at the terminal cursor. Its existing helper checks the retained Report ordinal, task/private sizes (`0xc0`/`0x88`), allocation identity, qualified address points, matching root objects and ownership control blocks, committed bindings, and incomplete/backlink flags. The flags path additionally checks the QVector/second-QList bindings, exact version/caller provenance, all thirteen sparse-zero fields, and exact allocation-bound EAX/private20 expressions including unknown origin and stage.

The top-level guard at lines 181–183 forbids a return token at every earlier cursor. At ordinal five, lines 588–589 require the token to equal the allocation identity. Invalid or missing terminal tokens therefore refuse without replay; valid terminal frames reach the stop before any mutation. Unknown cursor values reach the existing unsupported boundary without executing this move.

The internal flags stop accepts 0..5 and defaults to five; ordinal six and larger refuse before allocation. Ordinals 0..4 preserve the CP182 cursor/stop sequence. The separate zero-store stop remains 1..6. Stop checks precede each admitted instruction, and terminal reentry with either the same key or another key cannot advance beyond `0x131235`.

## Tests and implementation report

The four CP183 cases at `test_doe_task_factory.cpp:5040–5144` cover the successful identity move, complete predecessor-state comparison after removing exactly the two admitted effects, terminal same/different-key reentry, nineteen malformed predecessor variants, missing/foreign terminal identity, and rewind/resume through every CP182 stop. Existing CP182 tests continue to exercise all symbolic-expression stages, sparse predecessor corruption, invalid stops, and null private state. Earlier diagnostic rewinds clear the added token; `frameValues` includes it, so snapshot comparisons cannot silently omit it. The shared snapshot also compares sparse fields, version provenance, bindings/control blocks, counts/caches, completion flags, and reference deltas.

All four current source/test SHA-256 hashes match `resume183-connector-implementation.md`:

- `task_factory.cpp`: `1f35a50476e65a906cd2931e7ad90ca8791fe35023653b42ee3f96cd6438b1ce`
- `doe_task_runtime.hpp`: `78876f1b91f05ef1c3a0afe361de0854509d14225c00448e35a0c41462eeb0fd`
- `doe_task_factory.hpp`: `5b3fdf93ae055a3f19a38339efa808f52656a6155a30d85015a4fa9d7f5435aa`
- `test_doe_task_factory.cpp`: `56485a6b77cd4bab4f6074f10f956def61847a140d15757e90378bd97c6d46bd`

## Blockers and qualification

None found in the reviewed source. Integration build/test results and comparison/freeze gates remain the root worker's responsibility. This verdict does not establish live constructor behavior or claim that the unmodeled epilogue has executed. Only this review report was written.
