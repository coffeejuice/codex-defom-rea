# CP187 connector independent source review

**PASS — no blocking source defect found for the bounded RET transition.** This is source acceptance only. No build or tests were run by this reviewer; focused/full regression and candidate receipts remain integration gates.

Reviewed `resume187-connector-next.md`, its independent plan review and implementation report against `source/apps/moproj2mst/src/doe/task_factory.cpp`, `include/m2m/doe_task_runtime.hpp`, `include/m2m/doe_task_factory.hpp`, and `tests/mdt/test_doe_task_factory.cpp` (paths relative to `source/apps/moproj2mst` where abbreviated). Read the retained static-source qualification in `resume163-connector-qobject-static.json` and the bounded epilogue/caller listings. They are unwind-region evidence, not complete logical-function or runtime-success proof. The listings retain `c3` at RVA `0x13123f`, CALL at `0x131417`, continuation XOR at `0x13141c`, public store at `0x131425`, and backlink store at `0x131431`.

## Contract and implementation

`ReturnToCaller` is prepared before publishing the new frame. The retained slot qualifies its allocation owner and control block, Report ordinal, task/private allocation identities, QtCore hash, initializer `0x131160`, CALL `0x131417`, fixed target `0x13141c`, and slot displacement zero relative to initializer entry S. Resume rejects missing/corrupt provenance without recreating it. The separate saved-RBX/RDI records and historical stack relations remain required.

At CP186, phase-derived `returnBoundaryRva()` is `0x13123f`, live RSP is S and restored RDI must name the saved incoming-RDX-origin token. CP187's only instruction mutation is the nonthrowing RSP assignment to S+8 followed by the phase assignment to `BeforeCallerXor`; the derived boundary becomes `0x13141c`. The symbolic slot is retained. Private RAX, restored task RBX, and restored saved-entry RDI remain separate representations. There is no invented native pointer, parent identity, duplicated mutable RIP, or return fallthrough to `0x131240`.

Before either the predecessor transition or terminal refusal, the factory validates retained roots/control blocks, private allocation/address points, exact sparse zeros including private+8, version argument, historical symbolic flags, RAX/RBX/RDI results, stack displacements, and return-slot provenance. Terminal reentry validates before checking the instruction budget. The terminal budget check always refuses before any further instruction or phase-array access; it cannot replay RET. No caller XOR, public `[RBX+8]` store, private `[RAX+8]` backlink, cache insertion, QObject/base completion or parent inference appears in the new transition.

The default stop is 9, with 0–8 retained and values above 9 rejected before initialization. Explicit CP186 rewind restores phase and RSP only; retained slot and register records are still validated. Earlier flags rewinds reset only the corresponding modeled progress and cannot repair return provenance.

## Test-source coverage and compatibility scan

The three CP187 tests cover both predecessor and terminal corruption (70 variants at each cut, across Report, Doe and unknown keys), exact predecessor/terminal register and boundary results, budgets 0–9 at the terminal, CP186 rewind/resume, and every earlier flags stop with corrupt-slot refusal. Snapshots retain return-slot fields and owner control block alongside private fields, expression state, roots, reference observations, construction state and allocation counts. Comparing the original snapshot after rewinding phase/RSP establishes that the successful return changed only those progress fields and the derived boundary. Negative assertions retain private+8 zero, absent publication/backlink flags and incomplete QObject/base state.

Invalid-budget fixtures now start at 10 and retain maximum-size rejection. Earlier CP183–186 instruction-specific tests retain explicit stops rather than depending on the new default. A recursive scan of the other application tests and the converter's `tests`/`source` test files found no stale references to CP182–186 terminal addresses/phases outside `test_doe_task_factory.cpp`; no other-test default blocker was identified. Execution remains required to establish passing regression behavior.

Only this review document was written. No source edit, build, test execution, GUI, runtime, deployment or REA database operation was performed.
