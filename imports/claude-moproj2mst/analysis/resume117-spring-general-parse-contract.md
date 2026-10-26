# CP117 SPRING general positive-count parse contract

## Decision

The original reader is structurally a count-driven sequence of Qt stream extractions, not a line parser. A safe next candidate can generalize from the exact count-one/count-two fixtures to a bounded family of complete ASCII rows while keeping the original field order, absent-companion behavior, and stop-after-count behavior. This report defines that candidate subset; it does not claim parity for malformed rows, arbitrary Qt numeric spellings, present `SPRING2.INI`, allocation failures, or non-ASCII codecs.

## Source-qualified original behavior

The retained PE decompiles are tied to `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; Qt behavior is tied to `QtCore4_64.dll` SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

`FUN_1410dab40` opens `SPRING.INI`, constructs one `QTextStream`, reads the count, allocates the `count*4` auxiliary array and `count` records, then reads optional `SPRING2.INI`, then processes each record in this exact extraction order:

1. integer `firstObject`;
2. doubles `firstValue`, `secondValue`;
3. integer `secondObject`;
4. doubles `direction[0..2]`;
5. auxiliary integer flag.

`analysis/wave2-save-diffs-300-support-1410dab40.c` and `...-1410da800.c` qualify count read/order. `...-1410da8f0.c` shows the double group wrapper zeroes its requested output slots, tests `atEnd()` before extracting, and returns the initial non-end result. `...-1410da690.c` shows the optional file's auxiliary array is zeroed first; if it is absent or cannot be opened, the main caller zeros all flags and continues. If it opens, it extracts up to `count` integers while not at end and ignores conversion status. The main loop has no final EOF check, so tokens after the last requested record are not consumed or validated.

The main wrappers do not check stream status. They write fields group by group. If a later group begins at EOF, the reader returns false after earlier fields have been written. If a group begins with data but fails partway through, its wrapper can still report true and leave zero-initialized destinations for unfilled fields. The record allocator's initialization and allocation-failure cleanup are not qualified. Consequently, partial rows and malformed extractions are excluded from this candidate contract; candidate failure must discard temporary records rather than expose partially filled output.

The stream's default locale is C, established by the shipped QtCore `QTextStreamPrivate` constructor; the spring path has no `setLocale` call. See `analysis/resume67-sw2sp-locale.md`. This qualifies ASCII decimal point `.` and grouping separator `,` independently of process ACP/locale. It does not qualify the behavior of every possible token spelling.

## Recommended candidate grammar and exact gates

Admit only `SPRING.INI` in the existing ASCII-only file profile, with a positive bounded count, absent `SPRING2.INI`, and exactly `count` complete records. Use Qt-like whitespace token boundaries (ASCII space, tab, CR, LF, vertical tab, form feed); records are groups of tokens, not physical lines.

For the first generalized profile, accept these deliberately narrow numeric spellings:

- integers: optional `+` or `-` followed by `0` or a nonzero decimal digit and decimal digits, with value representable as signed 32-bit; reject multi-digit leading-zero, `0x`/`0b`, grouping commas, and overflow forms. This avoids claiming base-0 and narrowing edges that were qualified for other specific paths but not yet independently mapped for every SPRING field.
- doubles: optional sign, decimal digits with an optional `.` and fractional digits (at least one digit overall), and optional `e`/`E` exponent with optional sign and at least one exponent digit; require a finite result. Reject grouping commas, `nan`/`inf`, malformed exponent, non-finite overflow, and non-ASCII bytes.

The ASCII-only codec gate is important: it admits the known writer spellings and does not infer Qt's default-codec treatment of arbitrary UTF-8/ANSI bytes. Preserve exact path matching and the absent-companion check. The accepted value of each row's flag is zero.

For the count boundary, use an explicit candidate safety ceiling (recommend `count <= 256`) before allocation, with checked multiplication and allocation failure returning false. This ceiling is an implementation resource policy, not an original-format limit; report larger counts as unsupported. It gives a finite refusal boundary while supporting counts beyond the exact one/two fixtures. Existing count≤0 behavior remains as currently qualified. Do not infer original overflow or cleanup semantics from this ceiling.

Once the declared rows have been parsed, ignore any remaining ASCII suffix, including whitespace and extra tokens. The original reader never extracts beyond the requested count and performs no trailing-EOF validation. Keep rejecting any suffix that violates the ASCII codec gate. This matches the original read stopping point without implementing unqualified later conversions.

## Partial effects and public state

Build records in a temporary vector. Return success and publish the vector only after every required field in every row has parsed. On EOF or invalid numeric token before completion, return the same guarded failure used by the existing reader, with output empty. This intentionally avoids exposing the original reader's partially written allocation: the public Open caller constructs list items only when the native read returns true, and the original allocation's unfilled bytes are not qualified.

The page/list layer must receive ordered typed records, one per source row, and preserve row order. Initial Open selects row zero after resetting its current-row marker to `-1`; selection callback reads each row's object IDs against combo item data and loads its scalar/vector values. Save first commits visible fields to the current item's record, then snapshots all item payloads in list order. Multirow parity therefore depends on root/editor's page ownership and Save integration, not only this reader.

## Focused test matrix

Add tests at the native reader boundary, then a public page roundtrip only when the page owner admits the same typed records:

- count three with three distinct complete ASCII rows, absent companion, assert exact values and order; this proves the grammar is not a count-specific literal table;
- count at the selected cap succeeds with complete supported rows; cap+1 is refused before allocation;
- spaces/tabs/newlines mixed between tokens and finite decimal exponent forms are accepted;
- one trailing ASCII token after the final declared field is ignored, matching the original extraction count;
- count zero/negative retains the already-qualified failure behavior;
- partial EOF after each group and within a double group returns failure and leaves output empty;
- malformed integer, leading-zero multi-digit integer, hex/binary integer, comma-grouped number, malformed exponent, non-finite number, numeric overflow, non-ASCII byte, and present `SPRING2.INI` remain refused;
- exact CP115/CP116 one/two-row fixtures continue to parse.

The UI test should choose distinct object IDs that exist in both populated combo lists, verify initial row-zero load, switch rows and back, then Save/Open and compare ordered values. Missing combo IDs remain outside this contract because the original selection callback leaves an unmatched combo's prior index unchanged.

## Evidence limits

Static evidence establishes the reader's extraction sequence, count-controlled loop, absent-companion zero flags, no trailing extraction, group-level writes, C-locale stream default, and public list item ownership/selection order (`analysis/resume114-spring-positive-count-static.md`). It does not establish original runtime behavior for arbitrary malformed tokens, all Qt numeric spellings in SPRING, allocation limits, allocation failure cleanup, present `SPRING2.INI`, ACP/default-codec handling of non-ASCII, or page control events under an original process. The proposed grammar and 256-row ceiling are candidate admission rules; they must be labeled as such until differential original-process evidence expands them.

No source, test, build, GUI, runtime, or mutable REA state changed for this report.
