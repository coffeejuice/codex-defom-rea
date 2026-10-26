# Independent review: CP75 DB directory receipt

## Scope and frozen identity

I reviewed `analysis/resume75-geometry-directory-receipt.md`, the six released source/test files, their allocation/retirement callsites, and the target registration rules. The six source/test SHA-256 values match the receipt exactly. No source edits, build, tests, GUI, fixture mutation, or REA operation were performed.

## Allocation, replacement, retirement, and owner generation

The source search found exactly two production `new DbDirRecord[...]` sites:

- `defdbio/src/step/db_info_read.cpp`: the full-load path converts positive local `nrsets` once to `size_t`, uses that value as the `new[]` operand and publishes the same value immediately afterward. The allocation becomes queryable only after record population and the existing cleanup tail calls `allocationReady`.
- `defdbio/src/db_query/db_chain.cpp`: multipart prepending first updates native `uac8`, uses that updated count as the `new[]` operand, and publishes it. It fills and copies entries, deletes the old allocation through the common helper, then marks the new one ready. Retirement matches the deleted pointer, so deleting the saved old pointer does not clear a different replacement receipt.

All production directory `delete[]` operations route through `defmodel::delete_dir_records`, which retires by pointer identity before deletion. The full-load path deletes the old pointer before allocating the replacement. The multipart path allocates/copies before deleting the saved old pointer. `db_info_reset` retains its native nonzero-`uac8` gate; when `uac8 == 0`, the pointer and receipt remain. `db_info_init` revokes the observation before native pointer initialization and does not free a leaked pointer as a repair.

Owner binding is at `DbHandle` construction after its unique reader is constructed. `DbHandle` destruction retires the matching owner, and `Cls_1401dbfe0` destruction retires the reader independently. Because the reader is a `unique_ptr`, direct replacement destroys the old reader and removes its entry. The process-wide monotonic serial supplies both owner/allocation generations; it fails closed at exhaustion. The actual receipt stores owner, reader, allocation, count and generations outside native objects.

## Layout and public geometry path

No native layout header or `DbHandle` data member changed. `db_load.cpp` adds constructor/destructor sidecar hooks; no C ABI changes. The receipt hooks occur in the allocation/reset lifecycle. The public geometry/query implementation in `loader.cpp` has no receipt call and remains guarded by the existing model/DB/reader checks. Thus this release does not yet make a directory read reachable through the receipt or change geometry-query behavior.

The static CMake glob uses `CONFIGURE_DEPENDS` for `tests/mdt/test_*.cpp`, so the new test file maps to target `m2m_mdt_db_directory_receipt` and CTest name `m2m.mdt.db_directory_receipt` without a hand-edited target entry. Real-load fixture copies are placed below the configured scratch root; no fixture file is modified in place.

## Focused test sufficiency and remaining gap

The ten cases cover unregistered/synthetic state, logical-count drift, ready gating, replacement generation and old-pointer retirement, actual delete/reset, the native zero-count reset edge, owner-address reuse, direct reader replacement, pointer mismatch/init revocation, full-load/header-only/reload/missing-file behavior, and a real multipart replacement. The two fixture-driven cases call the real `load_db_info`, `setup_part_chain`, and `prepend_previous_part` paths rather than a copied parser/allocator. The CMake registration and Testkit entry point are present. No tests were run under this read-only review.

One specific test gap remains: the receipt states that a failed full reload *after header fields change but before successful directory replacement* keeps the old allocation/receipt. The current integration test covers header-only retention and a missing-file reset, but not a parse/unsupported-family/read error after header mutation. This does not invalidate the pointer-identity bookkeeping for the implemented hooks, but a focused regression for that retained-old-storage error path would strengthen the contract before a later geometry query is allowed to consume the receipt.

## Finding

No blocking mismatch was found for this receipt-only release: both real allocations and all production retirement routes are instrumented with pointer identity; owner-generation lifecycle is covered; native layout and public geometry behavior remain unchanged. Keep the future geometry slice guarded until its strong-owner/reader match is implemented and the pre-replacement reload-error receipt behavior is covered or explicitly accepted.
