# CP143: actual fresh A producer commits cache1 and remains parked

The private high-record geometry branch now performs the bounded fresh-A/empty-region producer from `resume143-geometry-a-producer-plan.md`. It installs an exact local barrier before its existing actual directory query, allocates through `bc::calloc_doubles`, publishes the actual step-owned pointer and records its successful extent, then commits cache1. The barrier remains installed before the caller arrays checkpoint, main assignment, node Stop, coordinate return or builder. No second getter or equal-width copy is admitted.

Production edits are confined to `src/engine/viewer.cpp`: one allocator declaration include; private frame allocation/cache stages and observational pointer/count; a fresh-empty-A eligibility predicate; and the bounded high-record branch. Existing low-record code and its exact postVersion<6000 retirement remain unchanged. High profiles outside the new admission retain their existing query-and-guard path. Public editor/import gates, CP141 no-copy predicates and CP139/140 builder/readiness guards remain.

Admission requires the existing actual Project DB owner/reader/current receipt/in-range index plus dimension3/u1b58=0/high version, positive header preconditions, cache0, null A, empty selected region and nonnegative maximum length <=INT32_MAX-2. These fields are rechecked inside the original initial borrow before install and after query. The count is formed only after the no-overflow guard. The actual GUI names predicate is qualified by these fields; the constant DEF_PRE names fold is not called.

Query result/outputs/postVersion and allocation pointer/extent are retained in the private frame. The actual step owns the allocation and frees it normally. The frame adds no allocation owner or general span capability. Failed allocation retains its normal diagnostic/null publication and stops before cache commit. Nonempty file data and all other unresolved branches stay guarded. A parked cache reset cannot reacquire or resume the reader.

NEW standalone `tests/mdt/test_geometry_a_producer.cpp` contains three Testkit cases/34 bounded profiles, with39 CHECK and21 REQUIRE callsites:

- Twenty-four admitted profiles combine directory versions6000/11000, maximum lengths0/1/4, optional widths0/4 and element metadata2/3. They use the existing actual owned Project/DbHandle/current-directory fixture pattern with a copied DB fixture. They check real zero-filled allocation extents2/3/6, query version mutation, cache1, unchanged nominal/capacity metadata and MAR, node timer count2 and byte-for-byte unchanged arrays timer. Negative arrays count is included because that caller checkpoint has not been reached.
- The same profiles repeat refresh/scene and object mutation probes, including a declared raw cache reset. They require one total COW, the same allocation and a sticky unknown scene; the source must not bypass unfinished assignment through cache1. Optional coordinate/connectivity pointers and contents remain unchanged. The allocated A is retained for the actual step's normal destructor.
- Nine guarded profiles cover absent header preconditions, negative/overflow maximum lengths, nonempty selected region, a preexisting allocated A pointer, out-of-range index and missing model DB owner. They preserve the old query/guard cohort, then restore the source fields and enter one fresh producer invocation. No huge test allocation or file-array decode is attempted.
- One cached-high control preserves the existing two-getter/known-scene path with no A allocation or directory version mutation, proving the new barrier is specific to the fresh producer.

Fixture cleanup restores nonowning model/node/element pointers before destruction, removes only its owned copied DB directory, restores module tables/timers and dispatcher hooks, and leaves step-owned fresh A for its real destructor. The preexisting-A refusal fixture explicitly frees/nulls its own test allocation before testing a fresh producer.

Static preflight passed: comment/literal-aware delimiter balance, project include existence, no trailing whitespace, unique test names, one Testkit main, no assert macro, and one outer argument for each CHECK/REQUIRE. Directory record, allocator and Viewer declarations were inspected. These are structural checks, not C++ compilation or runtime validation.

Only Viewer, the new focused test and this receipt were written for implementation. No shared header/CMake/old test edit, build/test execution, GUI/original launch or mutable REA occurred. Root owns target `m2m_mdt_geometry_a_producer`, suggested CTest `m2m.mdt.geometry_a_producer`, configured with `M2M_DB_FIXTURE` and disposable `M2M_TEST_SCRATCH` like viewer_box, plus regression/full validation. No test pass is claimed.

Frozen files:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/engine/viewer.cpp` | `08211fd9e400d1224ff9b8559fae33ef6b3be2b3e8b2574c459b1dd35abbedc3` |
| `source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp` | `f96d671f0c157478bba7a70cb4543c89ae39339ff131006da4973e34ac1dbfc8` |
| `analysis/resume143-geometry-a-producer-plan.md` | `e6611da487aa5664db877378d28552c82f90bf3e2253d446cecb290b35523a1d` |
