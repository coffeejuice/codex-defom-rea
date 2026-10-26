# CP120 SPRING placeholder label reconciliation

The placeholder is **`0   - None`**, not bare `None`. The earlier `resume119-spring-combo-label-source.md` sentence claiming that the bare text is inserted directly was incorrect and should be superseded.

## Call evidence

In the retained `DEF_GUI_MO_64.exe` setup capture (`analysis/resume114-spring-editor-setup-140ec8b00.asm`), `0x140ec8b65` stores integer zero at `[rsp+0x48]`. At `0x140ec8b70–0x140ec8b7c`, the code passes literal address `0x142b14ee0` to `0x140f34710`, constructing a `QString` in the temporary beginning at `[rsp+0x40]`. At `0x140ec8b90–0x140ec8ba7`, it then calls `0x140ec98e0` with the output temporary at `[rsp+0x50]`, the address of the zero integer at `[rsp+0x48]`, and the constructed `None` string at `[rsp+0x78]` (as the fourth argument). This is the same prefix builder used for model objects.

The bounded builder capture `analysis/resume119-spring-label-builder-140ec98e0.asm` loads the signed integer through the supplied pointer, formats it with literal `"%-3d - "` at `0x142b14ff8`, appends the supplied name `QString`, and returns the result. For zero, the width-three integer contributes `0` plus two padding spaces; the literal suffix contributes another space before the hyphen. Appending `None` therefore yields exactly `0   - None`. The setup then appends that built result to its label list at `0x140ec8bd1`; later it inserts the accumulated list into both combos at index zero. The `None` literal is the name component, not the final combo label.

## Consequences

The placeholder visible text parses as integer zero under the separately captured `itemText`/`toLocal8Bit`/`sscanf("%d")` lookup path. Correct the earlier report's statements that the placeholder text itself is `None`, that it has five bytes including NUL, and that it is passed directly to `addItem`. The list insertion is text-only; there is no QVariant item-data value in this population/selection path.

This conclusion is bounded static evidence from the retained image (SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`); no original GUI execution was performed. It establishes the generated QString content given the captured literal and formatter, not translator or runtime display transformations beyond those calls.
