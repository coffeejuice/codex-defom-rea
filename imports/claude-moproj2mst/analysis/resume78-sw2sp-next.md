# CP78: next exact SW2SP base-0 token

## Selected input: binary `0b10`

The next useful one-value fixture is exact bytes `0b10\r\n` in `SW2SP.DAT`. Original Qt `QTextStream::getInteger` selects base 2 and returns 2; the CR delimiter is ungot after the final bit. Current candidate integer parsing does not admit the binary prefix and rejects this file.

## Source qualification

- Retained QtCore SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
- In `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`, the automatic-base dispatch at `0x670C53EF–5405` recognizes lowercase `0b` and selects base 2. The binary loop at `0x670C58E2–592F` requires the `b` prefix and accepts bit values only 0 or 1. Its accumulator update at `0x670C5942` is `value = 2 * value + bit`; bytes `1`, `0` therefore produce 2. The first non-bit is ungot at `0x670C5954–595F`, and a nonzero digit count proceeds to the successful value store at `0x670C5969–5979`.
- `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` shows the successful getter value is assigned to `int&` at `0x670C6166–616A` without setting stream error.
- The original sidecar binding in `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` and `analysis/resume67-sw2sp-locale.md` establishes that Extra3D Open requests one `SW2SP.DAT` integer through this QFile/QTextStream path. Save consumes the parsed field later as described in `analysis/resume69-sidecar-int-next.md`.

## Minimal test proposal

Write exact bytes `0b10\r\n` to `SW2SP.DAT`, request one integer, and assert successful read with value 2. Keep path and one-value gates. This token establishes neither general binary parsing nor malformed/no-bit conversion status, binary overflow, uppercase `B`, signed prefixes, or multi-value continuation; those remain outside the proposed test.

This is static shipped-Qt evidence, not an original-process runtime comparison. No source, test, build, GUI, or mutable REA state was changed for this read-only audit.
