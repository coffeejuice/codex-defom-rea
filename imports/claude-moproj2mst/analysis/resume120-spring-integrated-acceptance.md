# CP120 integrated SPRING acceptance review

## Outcome

No actionable defect was found in the frozen private label transition or the bounded `SPRING2.INI` reader when evaluated against the retained contracts and the explicit candidate admission guards. The CP119 text-label implementation corrects the former integer/item-data mismatch. The follow-on ASCII `x` companion cases are supported by separate retained QtCore static evidence in `analysis/resume120-spring2-malformed-flags.md` and are still narrower than arbitrary malformed input.

This is a static acceptance review, not build or runtime parity evidence. It does not admit the public mode300 Session palette or native Qt widget path.

## Label and click transition

Reviewed `simcontrol300_io.cpp` (frozen SHA-256 `493f6e2bf21b62c377be13a8829ee1acb849da36a639f5715258ed7eee9b33dd`), `test_spring_page_io_roundtrip.cpp` (SHA-256 `8c7b177283317028998c9952251a13dfff5d1a827e0d04908c526b5871c41c9e`), and `resume119-spring-label-event-implementation.md`.

- `springLabelId` models the source-qualified leading signed decimal prefix, failed conversion as zero, and explicit <=63 ASCII byte admission (excluding embedded NUL). It does not claim to reproduce unbounded native `strcpy`, arbitrary local-codepage conversion, or overflow.
- `transitionSpringItem` commits the old current row’s parsed combo labels, five finite numeric values, and checkbox state before the same-row comparison. On row changes it scans the ordered labels and stops at the first matching parsed ID; a missing match keeps the old combo index. The loaded check state is exactly `payload.flag == 1`.
- `setSpringListCurrentRow` only moves the list-current row. The explicit event transition is separate, matching the retained `itemClicked` connection rather than treating programmatic movement as a click.
- Tests cover duplicate/prefixed labels, failed parse to zero, missing-match retention, programmatic movement followed by Save, 0→1→0 writeback, same-row writeback, null callback, finite edit admission, selected flag normalization, raw unselected flags, and the first-combo/old-row partial stage before a later guarded second-combo failure.

These assertions align with the bounded original helper sequence in `resume119-spring-itemclick-transition.md`. Test-provided labels remain synthetic; no native widget or original runtime run is claimed.

## SPRING2 reader and Save effects

Reviewed current `simcontrol300_native_io.cpp` and `test_simcontrol300_spring_reader.cpp` against `resume119-spring2-normal-path.md`, `resume120-spring2-malformed-flags.md`, and `resume121-spring2-partial-parse-implementation.md`.

- The companion reader zero-initializes the count-sized flags, parses only as many values as requested, leaves EOF-short suffixes zero, and does not modify either input file. Candidate BOM refusal remains explicit and tested; the original BOM/codec behavior is not asserted.
- Canonical complete/short/empty/trailing input is covered. The narrow `1x` and `x` cases preserve the source-qualified QTextStream cursor/destination behavior: a decimal prefix succeeds and leaves `x` unread; a failed extraction stores zero and leaves the unread character for a subsequent requested extraction. Other malformed suffixes, radix forms, overflow, non-ASCII bytes, and BOMs remain refused.
- Save tests verify primary `SPRING.INI` attempt before companion handling; for all-zero flags, readable deletion is attempted even after primary open failure; for nonzero flags, primary open failure suppresses companion write; and companion open failure does not roll back the successful primary output. Status remains qualified to open outcomes; later stream/close errors and original same-run behavior are not claimed.

The original application captures establish the helper loop and ignored extraction status; the additional QtCore captures used for `x` behavior are identified in the CP120 malformed-flags report. No discrepancy was found within those captured cases.

## Limits

No build, tests, GUI control, original process, or mutable REA operation was run. The report does not claim full parser parity outside canonical ASCII plus the specifically captured `x` branches, nor public lifecycle acceptance.
