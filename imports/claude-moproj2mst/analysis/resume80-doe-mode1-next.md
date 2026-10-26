# CP80: mode-1 DOE sample serialization boundary

## Conclusion

There is no mode-specific serializer implementation gap. Native fixed-mode writer `0x1400d7790` selects `invalid[mode]` and `tables[mode]`, then calls the same `0x140915360` sample-table writer with that mode. The candidate likewise routes mode 0, 1, and 2 through one `writeSampleTable`/`serializeSampleTable` formatter. Its existing immediate-sequence test already exercises mode 1 with a populated synthetic table, verifies `Samples_FF.par` appears in source order, and checks exact candidate bytes `1\r\n1\t20\t\r\n`. No source change is justified for mode 1's shared formatting path.

The missing evidence is an **original valid mode-1 table/file**. A read-only inventory of retained oracle evidence finds no `Samples_FF.par` or `Samples_USR.par`. Every retained `DEF_DOE.PAR` begins with mode `0`; the mode-0 B121/B122 `DEF_DOE.PAR` and `Samples_LHS.par` files have fixture-backed byte comparisons in CP77. Historical absence of FF/USR does not identify whether those modes were invalid, nor does it supply their rows, decimal formatting, or output bytes.

## Native and candidate evidence

- `analysis/resume64-doe-sample-writer.asm` selects a dynamic mode index against the invalid-state array and table array, then calls the shared `0x140915360`; the public arm supplies mode 1 at `0x140099d7c` for `Samples_FF.par` (`analysis/resume63-doe-public-save-order.md`). This establishes same writer and selected mode/table, not actual valid mode-1 output bytes.
- `doe::serializeSampleTable` emits the mode header, one-based rows, tab after every value and CRLF. `qt::formatDouble` is six significant digits. The mode0 fixture-backed test protects byte parity only for the retained mode-zero rows.
- `tests/doe/test_doe_sample_immediate.cpp::doe_sample_immediate_sequence_writes_current_then_modes_zero_one_two` uses synthetic integer rows for all modes and verifies mode 1's fixed FF output and candidate byte layout. This catches accidental mode/index/path drift, but integer `20` does not qualify the original formatter's mode-1 decimal results.

## Smallest safe next step

Keep the shared mode-indexed helper and existing public guard unchanged. If exact original mode-1 byte parity is required, first obtain a retained original case/capture with mode 1 marked valid and populated. It should include the contemporaneous sample manager's mode-1 rows/validity, resulting `Samples_FF.par` (or `DEF_DOE.PAR` when current mode is 1), bytes and hash, plus evidence that the writer reached that file. Compare candidate output byte-for-byte against those captured bytes using the same parser/formatter fixture pattern as CP77. Until then, report mode 1 as source-backed through the shared writer and candidate-tested with synthetic data, without claiming original mode-1 bytes.

The helper remains detached from public Save. Native order is InputTable XML -> `DoeTableRefresh` -> current table -> fixed modes 0/1/2. The actual refresh receiver and cached Study/I/O lifetime remain guarded. `SetOprHasDoe` is a separate B121 variable-reader dependency; this serializer does not bypass that load boundary or establish a valid public Study.

## Review scope

Read-only inspection of retained DOE fixture paths, `resume64-doe-sample-writer.asm`, `resume63-doe-public-save-order.md`, `resume64-doe-sample-save-binding.md`, `resume71-doe-sequence-static-review.md`, `doe_study.cpp/.hpp`, and the existing immediate sample test. No source edits, build/test run, GUI, or mutable REA action occurred. This report is the only written file.
