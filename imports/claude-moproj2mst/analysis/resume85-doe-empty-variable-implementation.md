# CP85: empty DoeVariables reindex

The public InputTable reader now reindexes pre-existing variable records when an empty DoeVariables element is encountered. It preserves source list order, alias last-position stores and earlier index mutations before a null-entry fault guard. Nonempty variable payload parsing remains unsupported. The later CP84 number pass still runs only after normal completion of the entire child loop.

## Evidence and exact increment

Frozen evidence report: `resume85-doe-next.md`, SHA-256 `73788951f0545c9574cbbff6043035b113902b8401e1850786ac1cf4968726f6`. It reconciles active variable/output/optimisation guards and identifies the actual Study/Var fields and native timing.

Original DoeVariables reader `0x14090ee20` calls manager vt+0x1e0 at child exhaustion (`0x1409128bc`), then has no deferred-link iterations on the selected empty branch. Both native manager vtables bind that slot to `0x14091c7a0`. Its source-order loop writes variable+0x18 at `0x14091c800`; it does not skip null entries, clear variables, allocate new records or mutate numbers/ops/links. The 116 selected raw bytes have SHA-256 `4bbb5259e95301d3a816980bc15e98d5b8d7f5901fdc4fc85181b88043d84777` in GUI PE `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The private `reindexExistingVariables` helper borrows the actual Study's stable list via existing count/varAt model access and writes index in encounter order. On null, it reports the original dereference boundary as unsupported, leaving prior index/header effects intact and later entries untouched. Duplicate aliases receive each store, so `[a,a]` finishes with a.index1. There is no whole-list prevalidation, deduplication, owner replacement, retry marker or notification.

The native copied QList/reference ownership is still covered only by the existing candidate container abstraction. The new integer-field path does not claim native COW/ref/allocator-failure parity; it requires stable synchronous list ownership with no intervening callback. It adds no QWidget, QFileInfo, native QString/count or QDom ownership claim.

Inside `readInputTable`, the nonempty-element guard remains first. Empty/text/comment/CDATA-only child input then reindexes and sets the existing status true. Later child guards can still throw after that reindex: their original clears/count-reset happen, but the outer ID pass has not run. Earlier guards prevent the reindex. Normal completion reaches CP84's existing ordered number pass once, after all child readers, even when empty DoeVariables occurs more than once.

## Frozen files

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `fcbce2f2f613a0f81427dc320b78abf44ffba03cd8282697e86c2cf19817facf` |
| `source/apps/moproj2mst/tests/doe/test_empty_variable_reindex.cpp` | `38b0968e1559099c32ab8f3ebe0e53fddf2e71a23f002ea2a3b3359bd207235e` |
| `source/apps/moproj2mst/tests/doe/test_variable_id_finalize.cpp` | `cf37e8a4e1b07fcc30938976ca3a52e92579b23e93b2bbac197499753278fa64` |

The existing number-finalizer test changed only to remove its obsolete empty-DoeVariables refusal expectation. Its nonempty variable, region/constraint and number-order/fault tests remain. No header, Session, shared parser/writer, CMake, public ABI, Qt owner, native fixture or CP82 source file changed.

## Focused test target

New target `m2m_doe_empty_variable_reindex`; CTest `m2m.doe.empty_variable_reindex`. Eight cases cover:

1. Existing concrete record/alias identity, index0..n-1 and preservation of positive IDs, names, ops and links.
2. Duplicate Var aliases receiving their last index.
3. Partial indexes and earlier headers before null-entry refusal, with later siblings and number pass unadvanced.
4. Reindex before later region/constraint payload guards, their known clears, and unchanged nonpositive numbers.
5. Earlier region/nonempty-variable guards preventing reindex; unknown element children still conservatively refuse.
6. Repeated empty readers with text/comment/CDATA and a single outer number pass, protecting the native zero-first all-negative result.
7. Actual ConversionSession/live Project/active Type2 owned Study through `parseSubProject`, retaining the same pre-existing concrete records, reindexing then numbering them and continuing ordered LHS/FF/USR/current-PAR effects.
8. Same-owner engine Save still refusing at `InputTable variable dimension reconciliation remains unsupported`, preserving prior DOM and producing no XML output or additional sample writes.

The actual-owner fixture explicitly supplies pre-existing plain Var owners through the Study's existing owner vector. It does not claim public variable parsing, native notification or geometry loading occurred. Active SUB1 is explicit test setup; public C ABI selection is unchanged. Recording seed writes are separately counted and all fallback probes stay under configured test scratch. Recording success is not native durability evidence.

## Static preflight and release

All quoted includes resolve against their actual include roots/test directories. The new test has one Testkit main, eight active cases, supported assertions, no C assert/REQUIRE_EQ and no trailing whitespace. Standard declaration headers and explicit qt4path (`kSep`) and test_paths (`scratchRoot`) headers were checked. Existing source/test diff whitespace checks passed.

No worker build, compiler invocation, CTest, GUI/native app execution or mutable REA operation occurred. These test expectations await root validation. Source is frozen and held through root's grouped build/tests/archive. Relevant regression targets include `m2m.doe.variable_id_finalize`, `m2m.doe.operations_reader` and the existing MDT DOE-study tests. Nonempty variable notifications, populated region/constraint payloads, DoeOutput/Type3 lifecycles and public Type2 Save guards remain.
