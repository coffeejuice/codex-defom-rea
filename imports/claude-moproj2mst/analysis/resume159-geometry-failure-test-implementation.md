# CP159 body32 allocation failure test implementation

Added only `source/apps/moproj2mst/tests/mdt/test_geometry_clone_body_failure.cpp`.
SHA-256: `5c88e50b420028393eaf445df131636fbeddb57cb3a1e1d90c78f4115cb1a9b3`.

The test includes the real `defmodel/sdctl/cow.hpp`, statically qualifies the
8-byte `CowVector<double>` handle and 0x20-byte body, and invokes actual
`detach()` on two handles sharing a default empty body. A test-local replacement
scalar `operator new` records the requested allocation and old body's live refs
while armed, then throws `std::bad_alloc` before storage acquisition. The paired
unsized/sized delete accounting confirms there was no armed cleanup. Assertions
run after disarming and check one request of exactly 32 bytes, observed refs1,
propagated exception, unchanged identities, the refs1 undercount, and all three
payload pointers still null.

A guard declared after both handles disarms and repairs refs to2 only when both
still point at the saved body. This repair is test-fixture teardown only. It is
not a production rollback or a claim about original native allocation failure.

Preflight: Testkit provides `TEST_CASE`, `CHECK`, and `REQUIRE`; the source has
the required `testkit::run_all()` entry point and contains no C `assert`. The
quoted/angle includes resolve through the existing testkit and defmodel include
roots used by the project's app test targets. `CMakeLists.txt` globs each
`tests/*` directory and its `test_*.cpp` files, so this file is discoverable
without a CMake edit. No compile, build, test, GUI, mutable REA, or production
source change was performed; root owns focused compilation and validation.

**Ownership release:** the new test source is complete and released to root for
integration. This worker touched no other source file. The receipt is the only
additional file written here.

Scope remains limited to translated host `CowVector` failure ordering. It does
not establish native source-free behavior, constructor/payload allocation
cleanup, or CP157 scalar publication.
