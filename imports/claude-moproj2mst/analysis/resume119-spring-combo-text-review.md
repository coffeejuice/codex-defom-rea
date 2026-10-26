# CP119 SPRING combo text and SPRING2 BOM review

## Finding

The retained native combo path selects by parsing visible item text, not by querying Qt item data. Candidate numeric object-ID vectors are a useful projection for ordinary generated labels, but they do not yet model the native matching operation or its unsafe label boundary. Separately, the new SPRING2 UTF-8 BOM case is correctly refused by the candidate profile; available evidence does not establish that the original rejects BOM input.

## Native label matching

`analysis/resume119-spring-label-static.json` is tied to retained `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Its bounded captures identify `QComboBox::count`, `itemText`, `currentText`, and `setCurrentIndex`; no item-data accessor is present in this path. The comparator at `0x140ec8da0` loops through each combo index, gets `itemText(index)`, parses that text through `0x140ec9980`, compares the resulting integer to the requested ID, selects the first matching index, and otherwise leaves the current index unchanged.

The parser initializes its result to zero. Empty text returns zero. Nonempty text is converted with `QString::toLocal8Bit`, copied into a stack buffer using unchecked `strcpy`, then passed to `sscanf` with `%d`; the scan count is ignored. A failed conversion therefore leaves the initialized zero. These details are bounded by the parser capture `0x140ec9980..0x140ec9a47` (native-byte SHA-256 `8ee0571c2976dcf9e21b8c604a0dbf50f59276d4417b2db6cb22a39ed1783f07`).

The builder capture/report `analysis/resume119-spring-label-builder-static.json` proves the label prefix literal `"%-3d - "`, formats the signed object ID, then appends the object name. Therefore a generated label with a short, representable name parses back to its object ID. The normal candidate spans derived from `GuiModel::object(i)+8` can agree for that qualified case. They are not evidence of a native item-data lookup. Arbitrary names are not covered: the native copy is unbounded, while the object name layout and field alone do not prove NUL termination, encoding, or a safe total byte length. Editor300's bounded layout reading indicates a maximum 83-byte name field and a prefix of at most 14 bytes, but does not prove the contents are terminated or ASCII.

## Candidate impact and next narrow correction

Current `springComboItemIds` in `src/engine/simcontrol300_lifecycle.cpp` derives ordered IDs from model objects and comments that native code inserts item-data IDs. `SpringSelectionState`, `fillExtra3D` overloads, and `tests/s/test_spring_page_io_roundtrip.cpp` likewise describe explicit integer lists as item-data. Those explanations should be corrected. The page tests exercise numeric candidate membership and serialization; they do not create or parse native combo labels, test first-match index selection, exercise local-8-bit conversion, or prove a native widget population.

A narrow faithful extension should model the ordered labels (or an explicitly derived, validated parsed-label sequence) for each combo, and select the first label whose native-style leading `%d` parse equals the requested object ID. Bound the encoded label bytes before parsing: the native stack buffer has 208 bytes and the original uses unchecked `strcpy`, so a candidate implementation should reject unknown/non-ASCII labels or labels whose encoded length plus NUL cannot be proven to fit. Keep empty/failed parse-to-zero behavior only when it is explicitly represented by the admitted label profile. Do not claim to repair or reproduce native overflow behavior. If the project cannot yet supply safely bounded labels, keep the current ID-membership branch described as a candidate-only shortcut and refuse to call it native-equivalent.

CP115-118 page/order and Save fixtures remain useful for the narrow numeric-record and writer behaviors they actually assert. Their usual short canonical labels would produce the same ID values under the retained parser, so their chosen IDs are not invalid; however, any statement that they validate item-data matching or arbitrary object labels is unsupported. The current no-edit path still only needs to establish the selected row's two values; it does not establish selection behavior after a user click or the contents of a real Qt combo.

## SPRING2 UTF-8 BOM guard

The frozen reader calls `asciiBytes(*companion, false, true)` for `SPRING2.INI`. That path rejects a leading UTF-8 BOM with `M2M_E_UNSUPPORTED` before parsing. `tests/mdt/test_simcontrol300_spring_reader.cpp` writes BOM-prefixed bytes into its disposable fixture, expects the unsupported result and an empty output vector, and checks that both input sidecars remain byte-identical. This is a candidate admission guard with bounded cleanup assertions.

`analysis/resume119-spring2-normal-path.md` expressly excludes BOM and non-ASCII companion input from its source-closed canonical profile. Its retained original helper establishes a default `QTextStream` integer extraction loop and ignores conversion status, but the reviewed evidence does not qualify the original's BOM/codec handling. The candidate refusal is therefore safe as a scope boundary, not an assertion of original failure or parity. Keep BOM input outside supported claims until a retained source/runtime witness establishes the original behavior.

## Review limits

Read-only source and retained-artifact review only. No build, test execution, GUI control, original-process execution, or mutable REA action was performed. The exact candidate text is not yet native-equivalent for labels; SPRING2 BOM remains intentionally unsupported pending original evidence.
