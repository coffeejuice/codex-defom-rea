# CP75: next SW2SP numeric-prefix edge

## Selected exact input

The next source-closed one-value witness is `SW2SP.DAT` bytes `2,345\r\n`. The original default-C `QTextStream` integer getter accumulates the first decimal digit `2`, sees comma as a non-digit delimiter, follows its C-number-options unget branch, and completes extraction with value 2. The comma remains unread. The one-value Extra3D sidecar reader ignores the remaining stream contents/status and returns success after QFile opens. Current candidate only admits its exact `1,234` prefix and otherwise rejects comma after the parsed digits, so this is a concrete candidate divergence.

## Retained evidence

- QtCore image SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
- `analysis/resume73-sw2sp-next.md` qualifies the locale branch: the original default stream and constructed C locale both have zero number-options. When `getInteger` sees a non-digit, the comparison at `0x670C5712–575C` jumps to unget at `0x670C5780–578B`, bypassing the conditional group-separator consumption path. The accumulated digit count is nonzero, so the parsed-number path succeeds. The comma is therefore a delimiter, not a grouped-number separator.
- `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` shows the decimal accumulator and non-digit decision; `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` shows the successful `int&` assignment without conversion error.
- `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` binds `FUN_1410da690` to QFile/QTextStream integer reads and shows it does not inspect QTextStream status. `analysis/resume67-sw2sp-locale.md` and `analysis/resume69-sidecar-int-next.md` bind the reachable Extra3D Open case to one requested `SW2SP.DAT` integer.

## Minimal candidate-only proposal

Add one adapter case writing exact bytes `2,345\r\n` to `SW2SP.DAT`, passing a one-element span, and asserting success with value 2. The position immediately after the successful extraction is after the first digit, with comma left unread; the current outer API does not expose the cursor, so the test should assert only the caller-visible value and success. Retain path and count guards; do not infer general group-separator acceptance, arbitrary digit lengths, repeat extraction after comma, or behavior under a non-C number-options profile from this exact fixture.

This is retained static QtCore/control-flow evidence, not a new original-process comparison. No source, test, build, GUI, or mutable REA state was changed for this audit.
