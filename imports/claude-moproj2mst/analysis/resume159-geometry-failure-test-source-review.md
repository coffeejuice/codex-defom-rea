# CP159 independent failure-test source review

No blocker was found. The released test implements the accepted declared-host allocation-failure contract, including real detach observation, disarming and identity-conditioned fixture repair. It adds no production allocator/cleanup behavior and advances no CP157 publication guard.

Hash binding:
- Actual source `source/apps/moproj2mst/tests/mdt/test_geometry_clone_body_failure.cpp`: `5c88e50b420028393eaf445df131636fbeddb57cb3a1e1d90c78f4115cb1a9b3` (matches implementation receipt).
- `analysis/resume159-geometry-failure-test-implementation.md`: `df2c53142b053a028f1b28ad217cceaac4731ac671981e13024f103eb6db4c19`.
- Accepted review `analysis/resume159-geometry-plan-review.md`: `e385dd056d825e8ed34f457410af3e933cbd561a820649869ac3f777e29dff2d`.
- Included actual CowVector header remains `6928fd1bfeb9001e0ad31e560b527a8f26f8621c92c4f3345b1ecf764ab0a983`.

## Hook and real failure ordering

The test uses two actual handles sharing the default empty body. Static size checks qualify the8-byte handle and32-byte body. The only operation while armed is `first.detach()`. Replacement scalar new performs fixed primitive probe writes, reads the still-live old body's refs, and throws bad_alloc before calling malloc. There is no Testkit call, logging, dynamic probe storage or unrelated allocation in that interval. Normal unarmed allocation and unsized/sized delete use the paired host malloc/free mapping; sized delete forwards to one accounting path.

Both normal and unexpected catch paths disarm before assertions/rethrow, and the fixture guard also disarms on unwinding. The checks require the propagated exception, exactly one observed request32, refs1 inside that actual allocation, zero armed delete calls, both unchanged handles, retained refs1 and unchanged null payload fields. This observes the real header implementation's decrement-before-new behavior rather than a copied transition model. Allocate-first or added rollback changes fail its observations. An unobserved/elided allocation cannot satisfy the prerequisites.

## Fixture lifetime

`RestoreFixtureRefs` is declared after both handles and before arming, so it runs before either handle destructor. It repairs refs2 only if both current handles still name the saved actual host body, and only after the failure-state assertions. An unexpected successful detach therefore does not overcount the old body. On the specified throw path, the repaired refs2 permits second destruction to decrement to1 and first destruction to delete the body exactly once. No fake clone storage is freed and no incomplete constructor destructor is invoked. The saved probe.old pointer is never followed once disarmed during later fixture/global teardown.

The repair is explicitly test cleanup; neither production detach nor its exception path was changed. It does not conceal a production rollback because the refs1 observation/assertion occurs before repair. Existing native body32-throw versus payload-constructor-throw distinctions remain intact.

## Testkit, target and limits

Includes resolve through existing Testkit/defmodel roots; static inspection passed one case,8 CHECK/3 REQUIRE outer arities, and one `testkit::run_all()` entry. The test contains no C assert or forbidden allocation work while armed. No runtime result is claimed here.

The released location is **tests/mdt**, rather than the plan's proposed tests/model. The actual CMake packet/topic equation therefore yields target `m2m_mdt_geometry_clone_body_failure`, CTest `m2m.mdt.geometry_clone_body_failure`. This is a registration-name correction, not a source defect; root should bind focused validation to those actual names.

The receipt correctly confines claims to translated host CowVector failure order. It proves no original CRT handler/HeapFree/errno behavior, payload-constructor cleanup or scalar body publication. Source disposition: accepted, no corrective edit requested. Root owns compilation, focused/full tests and source-stable freeze.

Only this report was written. No source edit, compiler/build/test run, GUI/original execution or mutable REA occurred.
