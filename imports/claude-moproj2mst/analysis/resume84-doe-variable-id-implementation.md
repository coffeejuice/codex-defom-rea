# CP84: outer InputTable variable-number finalization

The public InputTable reader now performs the original outer variable-number pass after normal child dispatch. Existing positive numbers remain unchanged. Each nonpositive variable gets the result of a fresh ordered scan of the current actual variable list, with source 32-bit wrapping arithmetic. The change does not parse/create variables, reindex them, apply Operations payloads or advance any Qt/notification/Save guard.

## Qualified behavior

Frozen evidence/design: `resume84-doe-operations-traversal-next.md`, SHA-256 `295cee7f9f048650188b5521a8319cc6b26ab609327868d37a21945bfc7a698c`. It includes original PE hashes, source-tail addresses, helper/getter/setter bodies, selected raw-byte hashes and the QtXml traversal limitation.

Original `0x14090ed14..0x14090edb3` gets manager count once, visits each source-order varAt, skips null outer results and preserves numbers >=1. For each number <1 it calls vt+0x1e8 (`0x14091da70`) and stores the result with `0x1409ff590` into variable+0x1c. Its status return follows this pass, so header-only/empty-root input can still return false after IDs change. Parse failure or a thrown current unsupported payload never reaches the pass.

`nextVariableNumber` starts signed32 candidate0 and scans all current `Study::vars` in order. When a number >= candidate it stores that number plus1 with explicit uint32 wrapping and `std::bit_cast<int32_t>`. It does not choose the lowest free ID, clamp to1, sort, deduplicate, cache a mathematical maximum or retry a newly assigned nonpositive result. Earlier assignments are visible to each later scan. Examples protected by tests are `[0,0,5]->[6,7,5]`, `[-9,-2]->[0,1]`, `[INT_MAX,0]->[INT_MAX,1]` and `[0,INT_MAX]->[INT_MIN,INT_MAX]`.

The inner source getter dereferences each list entry. The candidate raises its explicit unsupported null-dereference guard at that encounter rather than omitting it or inventing a native crash result. It preserves completed child effects and does not perform the current ID store. An outer null pointer with no invoked inner scan is skipped as in the source. Duplicate Var aliases retain identity.

Actual `Study::vars` and the existing plain Var number field are used directly; no new owner, counter snapshot or lifecycle token was introduced. The existing model/container seam remains a normal-field projection: native QList reference/allocation-fault parity is not asserted, and these synchronous operations require stable list ownership during the call. There are no Qt widget/path/resource/QString global-count operations in the new decision/store path.

## Source changes and hashes

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `35430131f31569ff30f424b1c69a7e479c6a5477d67e9f1bad2c5838e30c1861` |
| `source/apps/moproj2mst/tests/doe/test_variable_id_finalize.cpp` | `397186b24c270754393eb8108cbf92e968a8b53e014a8588632b1100b8ce0d92` |

The source also clarifies CP83's Operations comment: candidate models its normal true result and no payload application; it does not perform native QDom child/ref traversal or reproduce internal faults. No fake no-op loop or diagnostic traversal ledger was added. The existing CP83 test remains unchanged; its ops/index preservation assertions still apply, while whole-reader completion can now update a nonpositive variable number in the distinct outer pass.

Nonempty DoeVariables, empty DoeVariables with existing variables (separate index-finalization dependency), populated region/constraint payloads, Type3, DoeOutput/Report, codec/QFileInfo/shared-null, notification and public Type2 completion guards remain unchanged. No header, CMake, shared parser/writer/Session, original fixture or public ABI file changed.

## Focused tests

New target `m2m_doe_variable_id_finalize`; CTest `m2m.doe.variable_id_finalize`. Eight cases cover:

1. Per-assignment recomputation and preservation of existing positive duplicate numbers, vector/alias identity, ops and indexes.
2. All-negative inputs assigning zero once and a second explicit read revisiting that zero.
3. Signed32 overflow and source-list-order dependence, including later rows after wrap.
4. Duplicate aliases and harmless outer null entries on a positive-only list.
5. Inner-null refusal after child header effects and before the current number store.
6. Header-only and empty-root false statuses still finalizing; malformed XML returning before mutation.
7. Existing variable/region/constraint payload guards retaining their earlier child effects and never reaching finalization.
8. An actual ConversionSession/live Project/active Type2 owned Study with concrete pre-existing plain Var owners; Operations-only lazy read through `parseSubProject` updates those same records, preserves their indexes/aliases, then continues existing LHS/FF/USR and current PAR effects in order.

The active-owner fixture explicitly supplies pre-existing records via the Study's existing concrete plain-Var owner vector. It is not evidence that public variable parsing or native notification ran. Its selected SUB1 is explicit test setup, not a public C ABI selection change. All recording paths/fallback probes are under configured `M2M_TEST_SCRATCH`; seeded writes are counted separately. No real file writes or native durability are inferred from recording results.

## Static preflight and release

The eight-case test has one Testkit main, supported active CHECK/REQUIRE macros, no C assert/REQUIRE_EQ and no trailing whitespace. All quoted includes resolve against the source/test directory or actual include roots. Declaring headers are explicit: qt4path for kSep, bit/cstdint for wrap conversion, memory/initializer_list/limits/string/utility/vector for used standard declarations, and tests/support/test_paths.hpp for scratch configuration. Existing source diff whitespace check passed.

No worker build, compiler invocation, CTest, original executable, GUI control or mutable REA action occurred. These tests are authored expectations pending root validation. The two source/test files are frozen and held through root grouped build/tests/archive; root also owns existing Operations-reader and DOE-study regression checks.
