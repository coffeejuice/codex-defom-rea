# CP168 connector independent source and test review

## Decision

**Block static acceptance pending one predecessor guard repair and focused refusal coverage.** The CP168 store itself matches the selected `[0x1311bb, 0x1311bf)` cut: it records a single known 64-bit zero at private `+0x48` and stops before the independent `+0x50` instruction. The current source and tests have a malformed predecessor state that can mutate before refusal, and the CP168 refusal test does not establish every requested invariant. No build or test was run in this read-only review.

## Source and first-call path

The qualified disassembly in `analysis/resume163-connector-qobject-67131160.asm:32-35` shows `xor edi,edi`, the `+0x40` store, the `+0x48` store, then the separate `+0x50` store. `SparseQObjectPrivate::private48` is `std::optional<std::uint64_t>` and starts disengaged (`doe_task_runtime.hpp:26-39`). The pending Report retains the private object (`task_factory.cpp:106-135`). CP167's guard and write lead to `BeforePrivateZero48` (`task_factory.cpp:201-225`); CP168 validates the same Report/private object, sizes, address points and written markers, associated root, qualified pointer-identical QList/QString bindings, both commit markers, known `private40 == 0`, absent `private48`, and incomplete QObject/backlinks (`task_factory.cpp:227-249`). It sets only `private48 = 0`, advances to `BeforePrivateZero50`, and throws before `0x1311bf` (`task_factory.cpp:250-260`). `ownsWidgetRuntime` checks both shared ownership and `.get()` (`task_runtime.cpp:30-37`). No CP168 path increments the root deltas, creates a TaskInfo/task, grows a map, or completes QObject.

Every admitted first-call expectation in the focused test file now says `RVA 0x1311bf`; fault fixtures that stop earlier expect `0x131185` or an earlier guard message. The positive cases check the new phase, both zero fields, the retained private and root bindings, unchanged delta on reentry, empty caches, one retained prototype, no task, and incomplete QObject/backlinks (`test_doe_task_factory.cpp:299-407`). The actual `ConversionSession` owner pairing is exercised at lines 343-367. Same-key and different-key reentry are exercised at lines 399-406. Since both stores run in one first call, the tests observe CP168's final boundary rather than a separately paused CP167 frame; the pre-write test reconstitutes that predecessor phase.

## Blocking findings

1. **The CP167 entry can mutate a malformed CP168 field state.** At `task_factory.cpp:207-224`, the `BeforePrivateZeroFields` guard requires `private40` absent but never requires `private48` absent. Starting with an otherwise qualified retained frame at that phase, `private40` absent, and `private48` already observed, it writes `private40 = 0` and advances to `BeforePrivateZero48`. Only the following CP168 guard notices observed `private48` (`task_factory.cpp:245-249`) and refuses. This violates the plan's requirement to refuse malformed phase/field state before mutation and undermines the one-time transition. Require `private48` absent in the CP167 predecessor guard, then add a test that captures the fields, phase, bindings, deltas, and counts before this refusal. The present CP168 corruption loop begins at `BeforePrivateZero48` (`test_doe_task_factory.cpp:425-430`), so it cannot detect this sequence.
2. **The CP168 refusal matrix and post-refusal observations are incomplete.** The ten-case loop at `test_doe_task_factory.cpp:409-451` covers missing/nonzero `+0x40`, observed `+0x48`, aliased foreign QList binding, foreign QString binding, false QList commit, wrong private address point, completed frame, wrong phase, and expired root. It does not directly exercise an unassociated or foreign factory root at the CP168 boundary, aliasing foreign QString binding, false QString commit, wrong task address point or written marker, or available backlink. The loop checks fields, phase, identities, deltas, caches, and counts after refusal, but does not check that the injected bindings, commit markers, completion and backlink flags themselves stay unchanged. The plan explicitly requires field, phase, binding, and delta preservation on each refusal. Add those cases and assertions at the CP168 entry. Earlier-phase root tests do not exercise this guard.

The source checks the omitted CP168 guard terms; finding 2 is a focused-test evidence gap, not a claim that those terms are absent from production. The model has no `private50` or `private58` observation, and the source stops before either store. The current tests therefore can prove the control boundary and absence of modeled later effects, but cannot inspect those native bytes directly.

## Acceptance state and receipts

CP167's final static review in `analysis/resume167-connector-source-review.md` reports its predecessor snapshot accepted and cites parent-reported passing runtime gates. Those receipts predate the CP168 source and test changes. CP168's implementation note says no build, focused tests, checkpoint regression, or original comparison ran; this review likewise ran none. Runtime acceptance of this CP168 snapshot remains with the integration owner after the static findings are repaired.

| Inspected file | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `1878f797d87810bcb347eb93eded5db4ba6bc1655211329a7b059b2576e6aa28` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `156a52a6f4d14f92497400a10f9448f143adb1b99a4b0904ed1cc3da96a04f1f` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `6270dab8d1e4960715f2bf55148f7343caf2be1ea33a9f8a5761c6fd09f7681c` |

Only this review note was written. No source, test, build, deployment, GUI-control, original-process, or live REA change was made.

## Re-review of repaired source and focused tests

**Accept this snapshot for static source and focused-test review.** The CP167 predecessor guard now rejects `private48.has_value()` before writing `private40` or advancing the phase (`task_factory.cpp:219-226`). The new malformed-predecessor case at `test_doe_task_factory.cpp:481-510` restores `BeforePrivateZeroFields`, leaves `private40` unknown, supplies an already observed `private48`, and verifies refusal with both fields, phase, bindings, markers, symbolic deltas, counts, and completion unchanged. This closes blocking finding 1 above.

The CP168 corruption matrix now has sixteen cases (`test_doe_task_factory.cpp:409-479`). In addition to the earlier field, binding, phase, root-expiry, and completion cases, it includes aliasing foreign QString binding, false QString commit marker, wrong task address point, missing task address-point marker, and each backlink flag. It captures and checks both bindings and commit markers, task address point and written marker, completion/backlink flags, fields, phase, deltas, cache sizes, and retained counts after refusal. The separate test at lines 513-559 reconstitutes the qualified `BeforePrivateZero48` frame with an unavailable or foreign factory root and confirms that CP168 rejects it without changing the retained frame or deltas. This closes blocking finding 2 above. The already reviewed positive first-call, same/different-key reentry, and actual `ConversionSession` cases remain present.

The source and test hashes match the implementation handoff:

| Inspected file | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `750722b3bec3551b87124747bbb45235001dd699eb25e9d935b670ecf7f2cdcb` |
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `156a52a6f4d14f92497400a10f9448f143adb1b99a4b0904ed1cc3da96a04f1f` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `1ef0cb811cfeb2fe405a3a8688bf3fa7a05d39d51638de348b219e76ff69b3c2` |

This is static acceptance only. The CP168 implementation report says its focused tests, source-stable checkpoint regression, and original-comparison gates have not run; this reviewer ran none. Only this review note was changed in this pass.
