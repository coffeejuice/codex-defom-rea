# CP143 A-producer source review

No blocking defect found in the released Viewer fresh-A producer or focused test. The implementation performs the actual bounded high-record query/allocation/cache prefix and preserves its sticky stop before assignment. It does not admit reader completion, second getter, equal-width copying, low-frame retirement or a public editor/import gate. This is static acceptance for root validation, not a build/test pass.

Only this report was written. No source/build/test/GUI/app-control/original execution or mutable REA access. I compared the released source against frozen CP142 `source.zip`, read the actual allocator/query/free owners and step-access barrier, and independently checked the four native PE hashes in the plan. Original DEF_GUI_MO_64.exe v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Actual query and allocation order

The CP142 diff is confined to an allocator include, private frame metadata/stages, `qualifiedGeometryFreshEmptyA` and the selected high branch. Existing low branch, `completeInitialCoordinateRead`, CP141 width-mismatch predicate, null-companion path and builder/public guards are unchanged.

The preexisting actual Project strong DbHandle, exact model/reader match, current directory receipt and in-range actual `step.u740` checks remain before the directory record read/query. The new eligibility predicate additionally requires dimension3, null-name-state u1b58=0, current high version, positive header-length fields, cache0, null A, empty selected region and bounded nonnegative `u2b24 <= INT32_MAX-2`.

The frame records the actual model/step/view/item/geometry/mesh/node/element/DB/receipt identities and is installed inside the initial `withStep` callback **before** `step_dir_sizes`. That callback rechecks its actual step, node owner, reader, index, receipt and eligibility before installing. The strong DbHandle remains alive in the synchronous outer scope. `step_dir_sizes` reads the selected current directory record, writes the actual step version when needed and returns; the frame records result/output sizes/postVersion before any allocation. The post-query eligibility recheck rejects low version or changed prerequisites. The query is a direct synchronous read/setter, with no callback that can replace the step between those checks.

After the overflow guard, `u2b24+2` is a positive int32 count. Existing `bc::calloc_doubles` performs the zeroed count-by8-byte allocation and native-style diagnostic. The result is first published into actual `step.u1b78`, then recorded as a nonowning observational pointer/extent. The frame acquires no allocation owner and does not infer extent from nominal header/capacity fields. `DB_STEP_DATA::step_free_contents` frees/nulls this field normally; the frame does not free or dereference it during destruction/retry.

Only a nonnull allocation with still-empty selected region/cache0 commits actual cache11=1 and the corresponding stage. Allocation-null is retained as a committed publication/diagnostic, then parked before cache publication: this is a deliberate partial-effect guard, not a claim that the original native empty-region branch tested allocation success. No huge/OOM probe is required by this cut.

The native branch bytes agree:14082821b adds2 to u2b24;14082821e calls the allocator;14082822e publishes+1b78;140828248 tests selected length;140828265 writes cache1 and140828270 jumps to loader return. Native GUI predicate14082bf10 returns false for actual u1b58=0/dimension3/version>=6000. The implementation does not call the unrelated constant DEF_PRE names fold.

## Sticky boundary and unchanged continuation guards

The high branch always returns `HighVersionDbLoaderPending` after the initial callback. It has no arrays checkpoint, node helper/Stop, coordinate-return marker, builder call or frame-reset path. Existing retirement requires `NodeSkipLoopCommitted` and postVersion<6000; the new high stages cannot satisfy it. No parked `withStep` borrow is reacquired: subsequent refresh/scene/mutator entry sees the existing model-owned barrier before any native borrow or repeat COW/query/allocation. A raw cache reset or viewer replacement is not a retirement capability.

High records outside the producer eligibility retain the old query-and-guard behavior. Existing cache1-at-entry paths retain their old two-getter behavior. The new code does not generalize a preexisting A pointer into an owned span, decode nonempty regions, admit selected-width copying, relax low-frame checks or turn cache1 into completed scene readiness.

## Focused tests and path safety

The three Testkit cases contain34 bounded profiles:24 admitted combinations (versions6000/11000, maxima0/1/4, optional widths0/4, metadata2/3), nine refused profiles and one already-cached control. Success profiles verify actual small zero-filled extents2/3/6, post-query version/cache publication, unchanged MAR/+1b74/header metadata/optional arrays/step/DB identity, exactly one COW, node timer2 and byte-for-byte unchanged arrays timer. Negative arrays count is deliberately tested because its caller checkpoint remains unreached.

Retries, scene query, object mutation and raw cache reset assert sticky refusal and stable produced pointer/one COW. Refusals cover missing headers, negative/overflow count, nonempty region, existing allocated A, out-of-range record and missing model DB owner. Each restores fields and demonstrates a fresh producer call. The preexisting A probe uses the same actual allocator and explicitly frees/nulls only its own small allocation before the fresh profile. Candidate destructors leave newly produced A to the normal step destructor and reset nonowning optional/DB pointers. Cached control checks the preexisting completed two-getter path without A production or query version mutation.

Fixture input is read from configured `M2M_DB_FIXTURE` and copied to a PID/tag directory under absolute `M2M_TEST_SCRATCH`. The three tags are fixed local literals; no input-derived parent traversal is used. Existing CMake requires scratch beneath the build and gives each test a separate hashed child directory. Only the copied DB is loaded/mutated; the reference fixture is not changed. Filesystem cleanup is restricted to that derived owned directory. It uses an error_code while the DB owner still exists, so complete physical removal of an open file is not asserted by this static review.

Independent include/main/macro checks passed: all quoted/project/Testkit includes resolve, there are three unique Release-active Testkit cases and one `testkit::run_all` entry, with no C assert or unsupported REQUIRE_EQ. Failure/OOM and wrong/expired receipt integration are not separately exercised by this new test; the former is structurally guarded and the latter retains the unchanged actual receipt checks/regression cohort. Root must run the focused producer and existing Viewer/no-copy tests, then its source-stable full suite.

## Verified release identities and native qualification

| File | SHA-256 |
| --- | --- |
|source/apps/moproj2mst/src/engine/viewer.cpp|`08211fd9e400d1224ff9b8559fae33ef6b3be2b3e8b2574c459b1dd35abbedc3`|
|source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp|`f96d671f0c157478bba7a70cb4543c89ae39339ff131006da4973e34ac1dbfc8`|
|analysis/resume143-geometry-a-producer-plan.md|`e6611da487aa5664db877378d28552c82f90bf3e2253d446cecb290b35523a1d`|
|analysis/resume143-geometry-a-producer.md implementation receipt|`3c09bd8e27d9e9a597ac4c7c1dc3468a8924491b529196ae14cff3d338df14b5`|

Independent native half-open range hashes match the plan: loader140828060..1408283d5 `cb9d5ee4388c41ed115d75b236989b9f4cebf0bbe36ed3264d206508e70be796`; allocation/cache1408281fb..140828275 `286c53802c1db4aacf4fa6489fa9656c89577c20e3a037b246b0142f79b804f3`; allocator1409c83b0..1409c8408 `2ca64c1da768dd96662135394af3e2d8621a13a257c78ab42d2ebb046ea00ef4`; GUI predicate14082bf10..14082bf5a `c6e2177bc1357b7f5fe80b828e89967a50695d4d953c557bfa3aeea90eb06ace`.

Root targets: `m2m_mdt_geometry_a_producer`, `m2m_mdt_viewer_box`, `m2m_mdt_geometry_node_width_mismatch`; CTest names use `m2m.mdt.*`. No production source was changed by this review and no public Close or application-lifetime claim follows.
