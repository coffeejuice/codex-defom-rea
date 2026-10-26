# CP80: next distinct SW2SP base-0 failure branch

## Selected exact input: invalid first octal digit `08`

Use exact bytes `08\r\n` in `SW2SP.DAT`. The original Qt getter sees leading `0`, selects its base-8 path, and rejects `8` as the first octal digit. With no octal digits accepted, the getter reports conversion failure. `QTextStream::operator>>(int&)` writes zero and sets stream status; the one-value SW2SP reader ignores that status and returns success after QFile open. The caller-visible result is therefore `readInts` success with value zero. The candidate currently rejects `08` as an unsupported unsigned octal form.

This is a distinct radix decision/failure branch after the valid binary witness and invalid binary-digit case. It does not propose general octal parsing or error recovery.

## Retained evidence

- Original QtCore SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
- `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` shows leading-zero base-8 dispatch at `0x670C57CC`. The octal loop classifies a character as a digit by subtracting `'0'` and checking `<= 7` at `0x670C5832–5843`; the accumulator/count update is at `0x670C5845–5852`. For `08`, the `8` fails that range test, is ungot at `0x670C585D–5860`, and the zero-digit path at `0x670C5865–587E` returns conversion failure after restoring the initial prefix character(s). No successful octal value is produced.
- `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` shows conversion failure causes `operator>>(int&)` to store zero and set stream status (`0x670C6125–6165`).
- `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` shows the SW2SP helper initializes the output, requests one integer, ignores stream status, closes the QFile, and returns true after a successful open. `analysis/resume67-sw2sp-locale.md` binds this helper to the reachable one-value SW2SP Open path.

## Bounded test proposal

Write exact bytes `08\r\n` to `SW2SP.DAT`, call `readInts` with one destination, and assert success with value zero. Keep existing valid `02`, `010`, `0b10`, and `0x` profiles unchanged. Do not infer handling for other invalid octal digits (`09`), an octal token with an accepted digit before the invalid one, multi-value continuation after the failure, or any stream status API from this exact case.

This is static retained QtCore evidence, not an original-process runtime comparison. No source, test, build, GUI, or mutable REA state was changed for this audit.
