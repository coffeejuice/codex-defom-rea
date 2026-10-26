# CP83: populated Operations reader

The public DOE InputTable reader now accepts populated `Operations` lists as the original does: it returns true at that child without applying mappings or mutating manager state. Only the previous populated-element refusal was removed. All variable, region, constraint, output, Type3 and later refresh/Qt ownership guards remain.

## Source qualification

The frozen investigation is `resume83-doe-next-independent-branch.md`, SHA-256 `6cf8f1545299661762e8651bc6d83b63e0dfc14d53f10f5c50bf81586b73e499`. Original reader `0x140912a20` initializes status1, traverses QDom children with firstChild/isNull/toElement/nextSibling/destruction, and returns the unchanged status. No tag, attribute, Study callback or manager payload operation occurs. Fresh read-only DUMPBIN view confirmed the complete procedure through RET at `0x140912aef`; its 208 raw original bytes SHA-256 is `a7456391b8e777c2586fef701520dde892c610f37fa3f01a730d91aff83d29a8`. Original GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The retained Ghidra text/receipt are `wave2-doe-lazy-input-empty-readers-decompile.json` and `wave2-doe-lazy-input-empty-readers-receipt.json`, with hashes in the investigation. No database or stale endpoint was opened. Existing candidate XML traversal ownership is reused; native Qt DOM allocation-fault parity is not claimed.

## Changed files

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `4108b7363f7ce5119e8c2c4497b98b928d7346383e41316ab60b9bb07b0eceea` |
| `source/apps/moproj2mst/tests/doe/test_operations_reader.cpp` | `05066ecf7fd6979b7325b1e3ab0c42bf154f29e295df551ff5b6af0196ad6a99` |

No header, Session, CMake, shared writer, original fixture or Qt codec/path-owner file changed. Existing top-level B121 candidate-only model projection still has its separate mapping behavior; it is not used by this public reader branch.

## Focused tests

New discovery target: `m2m_doe_operations_reader`; CTest name: `m2m.doe.operations_reader`.

Four Testkit cases cover:

1. Populated Operations with plausible operation/variable names, unknown/nested elements, text and comment returns true. Existing variable ops/index, Study name, regions, constraints and sample state stay unchanged. Malformed overall XML still returns false.
2. Actual ConversionSession/live Project/active owned Type2 Study, reached through `parseSubProject` and real `SfDoeStudy`, reads the seeded InputTable, performs Samples_LHS/FF/USR simulated write attempts in source order, then reads and commits the seeded current PAR. The same Study owner/raw alias remains attached; no notification or Report factory shortcut is involved.
3. That same actual-owner read fixture enters `engine::saveProject`. Save must still stop at the precise main-window+458 refresh-owner guard after one normal successful XML write and the unconditional refresh boundary. Caller-owned SaveEffects retains the actual canonical XML journal, prior SUB DOM remains attached, and the original ignored Operations payload is not serialized as an applied mapping. No public Save completion or native durability is claimed.
4. Nonempty DoeVariables still refuses before Operations, and later populated RegionsOfInterest/Constraints still guard after their original clear/count-reset mutations. Operations cannot bypass them.

The recording fixture explicitly selects active SUB1; it does not change public C ABI selection. All recording paths and any fallback probes are under `M2M_TEST_SCRATCH` via the explicit `tests/support/test_paths.hpp` include. Input/DEF_PAR fixture seed writes are counted separately from subsequent lazy-read effects. The test uses the existing immediate XML simulation and existing refresh guard, not an original GUI process or a fabricated UI receiver.

## Static preflight and release

Quoted includes resolve against actual include roots or the test's directory. The test has one active Testkit main, four cases, supported CHECK/REQUIRE macros, no C assert/REQUIRE_EQ and no trailing whitespace. Existing tracked source diff whitespace check passed. `TextWriteAttempt.requestedBytes`, current parser declaration and caller-owned SaveEffects interface were checked against their current headers.

Root's first grouped preflight found the new test did not include the declaring header for `qt::kSep`. This exposed a limit of the initial include-resolution check: the includes that were present resolved, but a required declaration header was missing. The exclusive test now explicitly includes `m2m/qt4path.hpp`, whose line10 declares `qt::kSep`. The actual include path resolves; main/include/trailing-whitespace static checks were repeated. Only the test and this receipt changed for the repair; the production hash is unchanged. Root owns the rerun, and no passing build is claimed here.

No build, compiler invocation, CTest, GUI/app control, native target execution or mutable REA operation occurred. Root owns focused/grouped validation; the tests above are authored expectations, not reported passes. Source is frozen and held through root build/test/archive. Next guards are unchanged: nonempty variable notifications, populated region/constraint ownership, actual DoeOutput/Type3 lifecycles and refresh receiver/Qt ownership.
