# CP112: pure main busy decision prefix

Implemented native01daf0's exact partial-read decision in three new exclusive source/test files. It creates no application/main/widget owner, reads no native field or Qt object, performs no state mutation, and supplies no public Close admission. It is a deterministic model of the integer comparisons after their inputs become available. This closes a distinct gap between CP112's recovered actual getter chain and the existing Close entry helper's supplied mainBusy300 Boolean.

Only the announced files and this receipt were written. No shared source/header, CMake, plan, build, test execution, GUI, original execution, REA retry or mutable REA database. Original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No reconstructed executable/deployment was inspected or changed.

## Source contract

Native main table1420fb108+300 resolves14001daf0. It reads main+480 and calls imported QStackedWidget::currentIndex at14001db05.14001db0b compares the returned int with **exactly2**; equality jumps directly to true. Only the non2 path calls1400327c0 at14001db15. That leaf compares DWORD main+270 with **exactly1** at1400327ce and returns the corresponding Boolean.01daf0 returns false only when both exact tests fail.

`evaluateCloseMainBusyDecision(optional<int32_t> currentIndex, optional<uint32_t> mode270)` models those comparisons:

| Inputs available | Decision |
| --- | --- |
|currentIndex missing, even with mode270=1|NeedsCurrentIndex|
|currentIndex=2, mode missing or any value|Busy|
|currentIndex!=2, mode missing|NeedsMode270|
|currentIndex!=2, mode=1|Busy|
|currentIndex!=2, mode!=1|NotBusy|

The missing-first-read rule matters: algebraically reordering the OR could report Busy from mode270=1 while skipping the native Qt getter. The helper instead retains the first unavailable read. Conversely, it does not require mode after the currentIndex=2 short circuit. Negative currentIndex values, including native layout's initial-1, are compared normally; other nonzero index/mode values are not coerced to Boolean true. It does not validate page ranges, normalize main mode or repair original behavior.

All values are explicit pure inputs. They carry no proof that the actual main/stack/layout/private owner exists or that a source occurrence ran. No prior decision, constructor literal, Save answer or writer status is cached or consumed.

## Focused tests and static preflight

New files under `source/apps/moproj2mst/`:

- `include/m2m/engine/close_main_busy_decision.hpp`
- `src/engine/close_main_busy_decision.cpp`
- `tests/mdt/test_close_main_busy_decision.cpp`

Five Release-active Testkit cases cover missing first input despite later busy mode; currentIndex2 short circuit with unavailable/conflicting modes; missing second input for every tested non2 value; exact-state cases including-1,1,2,3 and signed/unsigned extremes; and a new unresolved attempt following a prior NotBusy result, which must not inherit that earlier result. These are declared mathematical input cases, not native owner or runtime captures. The function is noexcept and has no allocation/effect dependencies.

Static preflight checked the Testkit entry point, resolvable m2m/testkit includes against actual CMake include roots, supported CHECK/TEST_CASE macros, and absence of C assert/unsupported REQUIRE_EQ. It did not compile or execute the tests. Existing CMake globs discover both files; no registration edit is required. Root owns build/validation. Expected focused target: `m2m_mdt_close_main_busy_decision`; after root's configured build, use `ctest --test-dir <build> -R '^m2m_mdt_close_main_busy_decision$' --output-on-failure`.

## Integration boundary

Public Close remains unchanged. A future caller may use this projection only as part of the actual native2ca920/2ca840 route: global+9 short-circuits the main query when nonzero, and a null current main bypasses virtual+300. A present main still needs its actual derived lifetime and the real main+480 getter return before the01daf0 comparison can advance. CP112's constructor/publication/private-layout chain is not implemented by supplying optional values.

NotBusy is only the virtual predicate result. It does not discharge busy-warning behavior, manager can-close, modal delivery, nested Save, display callbacks, true-busy/Tree effects, destruction, false-busy or status. In particular CP110's project/MST writer failure retains Project attachment and can still return native answerNo10000; it supplies neither currentIndex nor mode270. A later FileCloseAll must obtain fresh inputs and execute its guards, including any nested Save. This helper must not be called from adapter catch/destruction to imply accepted native cleanup.

## Frozen read/write receipt

DUMPBIN14.44.35228.0, immutable PE section mapping, end-exclusive complete native function spans:14001daf0..14001db3c SHA-256 `9fde859043f5266fd0d9107338d3d28a1dd5634080b8211f4ce2919e489249f4`;1400327c0..1400327ef `96132f3a2d0722d406ac97a470810eae94df865fcd9a50534df37cf6b07f280e`.

New file hashes:

| File | SHA-256 |
| --- | --- |
|close_main_busy_decision.hpp|`d499e8dbd0500c47d6f08bdd3ab80b5490fceb74eeeee851350704c6c0d06d12`|
|close_main_busy_decision.cpp|`beb5a904f10aea980c5115a5cdae044935f191487b6397c03bf93234e48ab5ed`|
|test_close_main_busy_decision.cpp|`56858798d38f8fa6e197d6b47c5f3986eabc51b1051685a0076d67493233adb7`|

Retained evidence: resume110-close-after-save-failure.md (`818298bbcb5f73fe11d1c98fd2c9754ea5d79ef6f99d61394ee8137e7a57aa0c`), resume112-close-main-stack-producer.md (`b2136c02934c08a9fed39cf5e4267d4a99b579cdafeaa57906c9fd569caa07cd`). Source/API review also read existing close_dialog_answer.hpp/.cpp, close_entry_prefix.hpp/.cpp and CMake's m2m_core/test globs. This source receipt is not a tested candidate archive or original runtime witness. Ownership is released with these files frozen for root's integration build.
