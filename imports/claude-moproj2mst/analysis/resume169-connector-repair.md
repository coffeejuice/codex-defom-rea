# CP169 connector source and test repair

The CP169 refusal matrix now reconstructs the accepted CP168 predecessor before every fault: `BeforePrivateZero50`, known-zero `private40`/`private48`, and unobserved `private50`. Each injection is applied after that reset. The matrix checks the intended CP169 `CP168 state` guard and the captured state after refusal; the wrong-phase case enters the earlier `private+0x48 zero` guard and checks that diagnostic. The one-time store and reentry tests remain in place.

The pending QObject prefix now retains `allocatedPrivateIdentity` when its separate private object is allocated. Every retained constructor admission, including the first QList shared-null binding, compares the current private pointee against that independent allocation-time pointer before mutation. This replaces the prior comparison of two expressions for the same current pointer. The CP169 replacement test copies all qualified private fields into a second, live private object before swapping it into the frame, so its refusal isolates identity rather than malformed fields. The original private object stays alive through the assertion, preventing pointer reuse from obscuring the test.

After the independent re-review found a gap at the first QList admission, a separate test now reaches `BeforeFirstSharedQList` with an unpaired root, associates the correct root, and replaces the private object with a copied, qualified object. It requires the allocation-time identity diagnostic before private `+0x18` binding, symbolic QList reference increment, or phase advance, and checks retained fields, deltas, counts, cache, and completion state after refusal. The root association itself stays one-shot and owner supplied.

The private `+0x50` write remains a single zero observation. Its admission still precedes the `+0x58` stop, and no cache, backlink, QObject completion, or unqualified effect was added. `TaskAllocationStore` and its widget-owner pairing were unchanged.

## SHA-256 after repair

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `2dccdc0011a96cdfc7951cea4863e81333858611da6dcf0dc9cf29db96ec5bc0` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `9e27b8ec4a0ea145c696daac27ce8d84aad2037d8decb0b6cc6cc56fcc89b023` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `790e2ea1be45b7e3af8dface63e65286fa2601aa58a402c9bab6dd02405a3c2d` |

## Verification and limits

Static review confirmed that no comparison to `pending->qobjectPrefix->privateState.get()` remains in the factory guards, that the first QList admission checks allocation-time identity before its binding/commit/phase writes, and that the CP169 matrix resets the phase and `private50` before its switch. `python admin/check_continuation.py` returned `CONTINUE` with no errors. Per the assigned boundary, this owner did not build, run focused or full tests, perform GUI control, or start live REA or original-process work. Consequently these changes need the integration owner's focused test and source-stable gate before CP169 acceptance. No GUI-control session was acquired, so no release operation was needed.

The connector source/test owner releases `task_factory.cpp` and `test_doe_task_factory.cpp` again to the integration owner. The header's earlier identity field is unchanged in this follow-up. No shared plan, state, CMake, or `task_runtime.cpp` file was edited.
