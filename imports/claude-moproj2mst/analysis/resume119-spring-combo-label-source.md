# CP119 SPRING combo label generation and lookup

## Finding

The two SPRING combo boxes are populated with visible labels constructed from each eligible model object's signed number and name. Later selection synchronizes by parsing those labels; the retained path does not select from QVariant item data. Candidate support should therefore be limited to labels whose exact generated text and local-byte parsing are safely known.

## Population path

The evidence is the retained original `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, with bounded disassembly in `analysis/resume114-spring-editor-setup-140ec8b00.asm` and the helper at `analysis/resume119-spring-label-builder-140ec98e0.asm` (native bytes SHA-256 `b2376f94e7b476ea5a1d5d98a6d3326babd96534ff6dc4b17ee7aac6debde0c0`). The setup clears both combo boxes, then iterates the GUI model's 1-based object lookup for indices `1..objectCount`. A null lookup exits the population routine. Objects for which `FUN_140834FA0` returns false are skipped; the predicate is the existing qualified non-kind-7 test. Each accepted object's name is read from the `+0x828` field, while its signed number is read at `+8`.

The label helper formats the signed number with the literal `"%-3d - "`, then appends the supplied name as a `QString`. The resulting label is inserted into each of the two combos. The initial placeholder is the literal `"None"` (four visible ASCII characters; five bytes including NUL), inserted at index zero in both combos. The captured calls are `QComboBox::addItem`; this is visible-text population. Neither the builder nor the later lookup path uses `QComboBox::addItem(text, userData)` or reads `itemData`.

The `+0x828` observation identifies the name source, not a safe C-string length contract. A separate bounded layout review reports an 83-byte name field, but does not establish termination or that all bytes are ASCII. Do not infer that every model name can be safely passed through the native parser from the field size alone.

## Selection lookup

The comparator `0x140ec8da0` (capture `analysis/resume119-spring-combo-label-140ec8da0.asm`) walks the combo from index zero to `count-1`. It obtains each visible `itemText(index)`, calls `0x140ec9980`, compares the parsed integer with the requested object number, and calls `setCurrentIndex(index)` on the first match. If no match is found, it leaves the combo's current index unchanged. Since `"None"` parses to zero, a requested zero matches the placeholder first.

The parser capture `analysis/resume119-spring-label-parser-140ec9980.asm` shows a zero-initialized result, an empty-text early return of zero, and otherwise `QString::toLocal8Bit`, unchecked `strcpy` into a stack buffer, then `sscanf` with `%d`; the scan result is not checked. Thus generated labels beginning with a decimal object number parse to that number for ordinary representable labels. The parser's permissive prefix behavior is real, but malformed or oversized labels are outside a safe candidate profile.

## Candidate-safe subset

For the current no-edit row-zero lifecycle profile, only the selected row's two IDs are looked up. A candidate may derive IDs from the qualified model-object sequence and treat them as an abstraction of parsing the generated labels only when each matching label is known to be ASCII, has a complete decimal prefix matching the signed object number, and fits in a bounded local-byte buffer including NUL. The helper's captured stack allocation is `0x48` bytes; avoid approaching that implementation boundary and use a smaller explicit candidate limit. A conservative 96-byte maximum including NUL is already used by the related selected-label evidence; it accommodates the 10-byte placeholder and the ordinary generated prefix plus name subset, while refusing unknown encoding, nontermination, or arbitrary long names. That limit is candidate admission policy, not evidence that the original safely handles every name up to that length.

Unselected rows are copied and serialized without combo lookup in this no-click profile, so requiring every row's IDs to occur in both combo catalogs would add a restriction not present in that path. Conversely, the current integer-span abstraction does not prove real combo construction or first-match behavior for duplicate parsed prefixes. Label parsing is first-match: if distinct visible labels parse to the same integer, the earliest label wins.

## Scope and qualification

These are static conclusions from the retained PE and bounded disassembly only. No original GUI/runtime execution was performed. The safe ASCII length subset is a reconstruction guard; it does not claim native behavior for non-ASCII local-code-page conversion, embedded NULs, malformed prefixes, unterminated name storage, or `strcpy` overflow. Candidate comments and test names should say “parsed label IDs” or “derived IDs for the admitted generated-label profile,” not “combo item-data IDs.”
