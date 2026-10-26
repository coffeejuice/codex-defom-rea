# CP122 `SPRING2.INI` comma-delimiter implementation

## Change

The bounded companion flag parser now models the retained C-locale comma delimiter case. For an ASCII companion value `1,234`, it parses the decimal prefix `1`, leaves the comma at the logical cursor, and returns that first flag. If another flag is requested, the parser sees the same comma, returns zero, and does not advance; further requested slots remain zero. This matches the retained getter/operator/reader deduction for counts one through three.

The new branch is state-gated: a comma-leading retry is accepted only after a valid decimal prefix in this same companion read established the pending comma delimiter, and only while comma is followed by an ASCII digit. Comma-first data and malformed forms such as `1,,2` remain guarded. Existing CP121 `x` cursor behavior, canonical companion values, EOF-short zero fill, BOM rejection, and non-ASCII rejection remain intact.

Focused tests cover `1,234` with counts one, two, and three, expecting flags `[1]`, `[1,0]`, and `[1,0,0]`. They also reject comma-first and repeated-comma variants and verify that the reader does not modify either input file.

## Static preflight and limits

`git diff --check` passed for the two owned files. The implementation represents the visible values/cursor behavior needed by the bounded reader; it does not expose or reproduce QTextStream's sticky status field. General grouping acceptance, unrelated punctuation, locale-dependent bytes, malformed radix/overflow cases, and other comma forms remain outside scope. No full build/tests were run; root owns integration. No shared page/header, GUI, or REA state changed.

## Frozen hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `B091D8F35FCBDB0E7D02E9BFA9E3F7BFBF7A144CA1146DE8759621740A11FC3B` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `2B7F448F0BCCDDCA60051CB53D5403DA47EBD32501B2CFD1473FEAF6FB49730D` |

The PE and QtCore source qualifications and output deductions are recorded in `analysis/resume122-spring-stream-next.md`. No original-process comparison was run.
