# CP83 SW2SP three-witness fidelity repair

## Corrected outcomes

The source and focused test now encode the exact one-value `SW2SP.DAT` outcomes established by the independent PE review:

| Input bytes | Correct extracted value | Bounded implementation |
|---|---:|---|
| `0xA\r\n` | 10 | Exact token under the existing one-value SW2SP gate |
| `0X10\r\n` | 16 | Exact unsigned uppercase-prefix token under the same gate |
| `08\r\n` | 8 | Exact leading-zero decimal-fallback token under the same gate |

Signed `+0x10`, `-0x10`, `+0X10`, and `-0X10` remain zero. The new success cases do not apply to other file names or multi-value requests. `SPRING.INI` remains on its separate count path. No general parser conversion was attempted.

## Exact PE qualification

Original QtCore identity is SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The independent review at `analysis/resume83-sw2sp-parser-independent-review.md` re-read the PE section/export tables and compared bounded DUMPBIN `/disasm:bytes` instruction bytes against the image with zero mismatches. Its captures and region hashes are recorded there.

- Export resolution identifies `0x6706A650` as `QChar::toLower() const`. `getInteger` calls it before automatic-prefix selection at `0x670C53BF`, before validating the hex prefix at `0x670C556B`, and before classifying hex digits at `0x670C559D`. Thus uppercase `A` in `0xA` is normalized to lowercase `a`, and uppercase `X` in `0X10` is normalized to `x` before selecting base 16.
- For a leading zero, the lookahead path checks `QChar::isDigit` / `digitValue` around `0x670C5417–5446`. It selects octal only if the second digit is in `0`–`7`; when it is `8` or `9`, control falls through at `0x670C5448` to base 10. Therefore `08` is decimal 8.
- `QTextStream::operator>>(int&)` stores the getter result at `0x670C6166–616A` on success. The original `FUN_1410da690` reader initializes the destination, requests the SW2SP integer, ignores QTextStream status, and returns true after successful QFile open. The exact one-value call binding is retained in `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` and summarized in `analysis/resume67-sw2sp-locale.md`.

This is offline static binary/control-flow evidence, not an execution of the original application. ACP, CRT locale, or host runtime behavior does not affect these ASCII QtCore conversions.

## Superseded historical claims

The following receipts remain useful as chronology but their listed expectations must not be used as the oracle for these cases:

- `analysis/resume67-sw2sp-locale.md` and `analysis/resume67-sw2sp-locale-implementation.md`: unsigned `0X` was incorrectly described as decimal-prefix zero. The sign-prefixed cases remain zero; unsigned `0X10` is 16.
- `analysis/resume74-sw2sp-next.md` and `analysis/resume74-sw2sp-uppercase-hex-failure.md`: `0xA` was incorrectly treated as a zero-digit hex failure. Qt case-folds the hex digit, so the value is 10.
- `analysis/resume80-sw2sp-octal-invalid-next.md` and `analysis/resume80-sw2sp-invalid-octal-implementation.md`: `08` was incorrectly treated as invalid first octal digit. Base selection chooses decimal for this two-character numeric prefix, yielding 8.
- `analysis/resume83-sw2sp-parser-generalization-plan.md`: its proposed state machine repeated these three incorrect rules. Do not use that plan's affected parser contract as the corrected Qt oracle. Its process-scope cautions remain relevant: preserve the one-value SW2SP gate, keep other callers separate, and do not claim codec or multi-value equivalence.

The independent evidence and its explicit corrected table are in `analysis/resume83-sw2sp-parser-independent-review.md`.

## Frozen candidate and validation boundary

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` SHA-256: `C64BC75B5718B3F13C59193BF7793A54D632440B588CFA7ED1C96076935DD5D3`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp` SHA-256: `37EC076ED80F3F8D768A5E6F5FE754C166B17F43500044C926C64066E0599D06`

Static review and `git diff --check` completed without diagnostics. No build or tests were run in this lane; root owns build/integration validation. An original-process comparison of these three fixtures is still pending, so the source-derived outcomes should not be described as dynamically verified.
