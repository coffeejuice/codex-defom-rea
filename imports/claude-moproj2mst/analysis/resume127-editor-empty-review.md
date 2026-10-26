# CP127 independent review: empty Spring numeric transition

## Result

For an explicitly supplied zero-length visible String, the candidate now matches the bounded native value and ordering: the existing Qt adapter yields positive zero with `ok == false`, and the handler stores that result into the old row before the same-row comparison or next-row load. Null and nonnull-empty candidate Strings both have zero length and follow that case. The candidate correctly keeps its guards for other text/value profiles as admission restrictions, but those guards are not evidence that native Save refuses those inputs.

## Conversion and store order

The retained QtCore evidence in `analysis/resume126-editor300-next-cut.md` binds the original handler's five calls to the shipped `QString::toDouble(bool*)` entry at `0x6706c340`. Each call passes a null success pointer and stores the returned floating-point value unconditionally. For zero-length text the retained QtCore path returns positive zero; the handler's 0x40-byte record copy at `0x140ece257` happens before it compares the requested and stored row at `0x140ece287`. Consequently, an empty field participates in the old-item writeback on Save-current-item and on a delivered row change. A subsequent row load formats the saved zero into visible `0`; a same-row return leaves the existing empty/null visible String untouched.

The frozen candidate calls the existing `qt::toDouble` for each visible field. It admits conversion failure only when `String::isEmpty()`, then retains the returned value (the Qt adapter's +0) and the finite check. It writes the old record before checking `currentRow == requestedRow`, and on a click writes old row before the first and second target-combo searches. The new cases exercise all five fields, null versus nonnull-empty identity, row0→row1→row0 ordering, and an empty old value surviving a later target-label guard after the old write and first combo stage. This is consistent for the bounded empty-text case.

## Guard boundary for other inputs

The candidate still refuses nonempty failed conversions and any nonfinite result before mutating the stored row. Its tests cover space-only/tab-only, malformed text, overflow/underflow exponents, `nan`, `inf`, and embedded NUL; these checks preserve the candidate's supported subset. The native callback itself passes null for `ok` and contains no branch on conversion status or `std::isfinite`. It stores all five returned values and continues. Therefore these candidate refusals are reconstruction admission boundaries, not native refusal behavior. In particular, the handler source does not prove that native rejects nonfinite or malformed strings; the precise result for each text still depends on the Qt4 parser and any event/validator route that supplied it.

The QString conversion helper's retained evidence qualifies empty input, but the reports do not establish byte-for-byte results for every malformed/nonfinite spelling under the original process's codec/locale initialization. Nor do they show whether a real GUI validator permits the test's artificial text values to arrive at the callback. Do not describe these negative cases as native rejected-input tests or general input validation parity.

## Profile and failure-order limits

The new guard runs while constructing a private transition snapshot, before the helper writes any old-row fields. On a nonempty failed text it returns a preflight refusal with no row writeback; on a later target-label failure after a valid empty conversion, the earlier old-row write and first combo-index change remain. This distinction is internally coherent and tested. Native source shows unconditional conversion-result stores, so there is no source basis to infer that a native malformed-text case would stop before old-row writeback. The candidate profile is intentionally narrower.

The unchecked/zero-row Apply branch remains independent: it does not read these numeric controls at all. Public Session/palette construction, real QLineEdit validator behavior, actual itemClicked delivery, arbitrary invalid text results and same-run GUI acceptance remain unqualified. CP127 source-owner validation was static preflight only; I ran no build, tests, GUI, original process or mutable REA action.

## Frozen files

Reviewed against source-owner receipt `analysis/resume127-editor300-empty-numeric.md` (SHA-256 `11567a8c597a06df87cd0f828961e8893e701691e0eb3600978fca153db246f7`):

- `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp`: `7cccbd614ebda370eae85f313a4cb1f321be080dba827ce8976bb318d68dec69`
- `source/apps/moproj2mst/tests/s/test_spring_page_io_roundtrip.cpp`: `fead4fe7e6caadc6aca22ff824b349c307cfc06e104ebadf5b70eeb4a27e22bb`
