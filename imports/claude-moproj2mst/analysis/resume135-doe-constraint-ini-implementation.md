# CP135 private owned-Constraint DEF_DOE.INI serialization

The existing private planned DEF_DOE.INI writer now accepts coherent actual Study-owned zero-valued Constraint records and emits the native NUM_CON section. It preserves the two-pass ordering, original-index numbering quirk and category1 literal0.0. Public active Save still stops at its existing refresh guard; no native output device, Type3 parser or callback continuation was added.

Evidence contract: `analysis/resume135-doe-save-next.md`, SHA-256 `959F638FB196F45CCB412C2D0EB594AF354B1306499F7DC03A5D00702478FC6E`. Original GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The retained native913270 writer, actual manager getter slots/literals and independently instruction-qualified two-pass window are documented there. Its5206-byte normal body SHA is `a13035e57e52524905a23dc5fb18d479c0d904c7bd769e6c597f0f9d0bc7e0b8`.

## Source and value contract

Only existing `source/apps/moproj2mst/src/doe/doe_study.cpp` and NEW `source/apps/moproj2mst/tests/doe/test_def_doe_constraint_order.cpp` changed. The writer signature is unchanged. No public/shared header, CMake, Type3 owner/parser, Session, shared Save writer, refresh implementation or old test was edited.

The existing early positive-constraint refusal is replaced with an explicit bounded owner admission: nonnegative compatibility count equal to concrete owned-vector size, no null records, category0/1/2 only, current zero-value profile. Invalid/unrepresented host ownership refuses before publishing any new planned write, retaining prior caller FileEffects. No second record registry or native data/ref owner is created. sourceField10Bits remains unknown and unused.

The NUM_CON section is inserted after DOE_DBS/run paths and before REGIONS. Positive count adds a blank line, exact NUM_CON plus two TABs and count, then rows. Category2 is scanned first in original list order and printed with original index+1, while a separate counter starts1 and increments for each category2 row. The second scan keeps category0/1 mutual encounter order and numbers them from that counter. Each row has the native two/one/four/six/four-space layout. Categories2/0 use the existing numerical-zero formatting; category1 emits literal0.0. Thus IDs[1010,200,1081,205,1012] emit IDs[200,205,1010,1081,1012] with numbers[2,4,3,4,5], including duplicate4. Empty coherent count emits no new bytes.

This is the existing buffered FileEffects planner's value abstraction: one completed write is appended after serialization/Text-mode CRLF translation. It does not execute QFile open, QTextStream endl flushes, partial native writes, error/close observation or native allocation/COW/refcount behavior. It is not wired to the later native post-MST stage and is not moved ahead of refresh. A failed host construction does not append an incomplete new plan; prior effects remain. No original allocator-failure schedule is claimed.

## Focused tests and static preflight

Target `m2m_doe_def_doe_constraint_order`, CTest `m2m.doe.def_doe_constraint_order`, contains six cases:

- Actual ConversionSession/Project/Type2 SUB lazy input imports concrete records, then the private writer plans exact full mixed-category bytes with run paths, NUM_CON and following REGIONS. It asserts the duplicate row-number quirk, source spacing/CRLF, same Study/record identities, unknown+10 and no executed StudyIo writes.
- Duplicate category2 records retain distinct owners, encounter order and independently routed ObjectNo/RegionNo values.
- Without category2, the shared second-pass counter starts1 and interleaves category1 literal0.0 with category0 numerical0.
- Empty owned count preserves the prior complete planned bytes.
- Count mismatch, null record, unknown category, negative count and zero-count/nonempty-owner states refuse before appending a new plan, retaining a seeded prior effect.
- Explicitly planning the INI on the actual Study does not release engine::saveProject: immediate XML still records normal return/refresh boundary, then main-window+458 refuses; old SUB DOM/later sample-write count remain and the private plan is not replayed.

`python admin/check_test_source_preflight.py source/apps/moproj2mst/tests/doe/test_def_doe_constraint_order.cpp` passed. The quoted includes, Testkit main/macros and required qt4path declaration resolve. Direct trailing-whitespace/final-newline checks passed for both files, including the untracked test. `git diff --check -- source/apps/moproj2mst/src/doe/doe_study.cpp` reported no whitespace errors. Current source defaults and cleanPath separator normalization were inspected for the exact byte expectations. No worker compiler/build, executable or CTest/full suite was run; root owns validation. No GUI/original process or mutable analysis database was used.

## Frozen source

Source released to root before this receipt and held for grouped build/test/archive:

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `00B2E7AED3D47728F6A54D2EB70BF09B3B93076711FB29C8731FB591394D7423` |
| `source/apps/moproj2mst/tests/doe/test_def_doe_constraint_order.cpp` | `DFDC42542CC70A359C4A255E575DA9ECBC6D7B917A9451644ED2B70B1853BE19` |

Existing zero-constraint oracle tests remain valid; no obsolete positive-constraint INI assertion was found or edited. The exact next public active Save dependency is unchanged: invocation-bound refresh key codec/data owner, actual map/list/receiver/timer and normal dispatcher return. A separately faithful output-device adapter is still required before this private planned INI can represent native post-MST I/O.

## Focused build fixture correction

Root's first focused preflight failed in the new test fixture only: `build/logs/resume135-final-focused-preflight-02.log:51-54` reports C3668/C2259 because MinimalViews::rect omitted its item-index parameter. The fixture at test line86 now matches the actual `moproj_write.hpp:43` interface exactly: `moproj::RectF rect(const SubProject&, int) override`. Production source and test assertions are unchanged. The prior test hash4F2342E5719298164EF6810097A6CF64EF1731C7913C60346A6C5D088E54D4D6 identifies the failed initial fixture, not the active release above. The failed log/results were preserved.

The actual base signature was inspected, include/Testkit preflight rerun successfully, and direct whitespace/final-newline checks passed. No worker build or executable/test run was performed. Corrected test source was frozen/released to root before this receipt update; root owns compiled/runtime rerun. The initial static preflight checked includes/main/macros and did not catch C++ virtual signature compatibility.
