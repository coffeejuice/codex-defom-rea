# CP126 empty active-cogging diagnostic classifier correction

## Change

`admin/run_converter_tests.py` now routes output lines through `gap_events()`. It omits only a full-line, whitespace-tolerant match of the empty summary `refused (active cogging operation):`; all other existing `EVENT` matches are unchanged. In particular, a nonempty active-cogging refusal list still matches and remains a gap event.

Focused regression coverage was added to `admin/test_run_converter_tests.py` for the empty summary, nonempty refusal names, other skip/note/count diagnostics, and replay of the retained CP125 `mdt_write` log. The CP125 replay now yields only the genuine `SKIP optional imported PACK/pack.dat header check: M2M_INSTALL_FIXTURE unset` event.

## Verification and expected count change

Command run from `admin/`:

```text
python -m unittest test_run_converter_tests.GapEventTests -v
```

Result: 4 tests passed. One initial focused run failed because the test fixture said `1 case skipped`, which the existing regex intentionally does not recognize; the fixture was corrected to `2 cases skipped`, and the rerun passed. No full CTest suite or converter runner was run.

The retained CP125 full-run receipt `build/test-runs/1792922590847127500/receipt.json` records 313 tests, including six passed-with-logged-gaps suites and one difference-bearing suite. It contains 21 gap-event lines total. `m2m.mdt.mdt_write` has two lines: the empty refusal summary and the real optional PACK skip. After this classifier correction, that row has one event instead of two; expected total event count is 20. Its status remains passed-with-logged-gaps while `M2M_INSTALL_FIXTURE` is unset, so the six-suite gap count does not change in that CP125 replay. The four-lane CP124 focused PACK run separately showed the assertion passes with the fixture bound, but this source patch does not alter runner environment policy.

Raw CP125 log was left untouched at `build/test-runs/1792922590847127500/m2m.mdt.mdt_write.log`, SHA-256 `f11d515aabeb39af846e638b316b93eb32dbf36bacaf967c4b7b1801a9dc5467`. Its first line ends at the colon with no refusal names; it then reports 27 comparisons, zero Beta cases, and all six test cases passing.

## Frozen write set

| File | SHA-256 |
|---|---|
| `admin/run_converter_tests.py` | `a86a842fbdc31bd5b2ff780872cf7ff558fd7defbf4b9d68d15b649673109b4e` |
| `admin/test_run_converter_tests.py` | `f4970c11714b6a6ab1b8c27ab3a55811db345cc919f19024c5d201b3a59a7513` |

Only these two admin Python files and this report were edited. No C++ source, CMake, build artifact, fixture, GUI, original executable, or mutable REA state was changed or used by this classifier patch. Test execution only read the retained CP125 log.
