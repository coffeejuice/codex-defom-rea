# CP82 SW2SP binary prefix before invalid bit

Added exact `0b12\r\n` handling for the reachable one-value `SW2SP.DAT` read. The original Qt binary loop accepts the first bit `1`, accumulates value 1, then rejects and ungets `2`. Because at least one bit was accepted, the getter succeeds with the accumulated prefix; the caller reads only one integer, so the remaining character does not change the field value.

The candidate branch requires exact bytes through EOF and the existing one-value SW2SP profile. The focused test contrasts `0b10\r\n` -> 2, `0b12\r\n` -> 1, and `0b2\r\n` -> 0; it preserves guards for other-path and multi-value requests. It does not generalize binary parsing, suffix treatment, or additional invalid-bit tokens.

Evidence: original QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; retained `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` shows the binary range test, multiply/add, unget, nonzero-digit success, and low-level result store; `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` shows successful integer assignment. `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` binds the caller to one requested integer and ignored stream status. Static evidence only; no original-process runtime comparison.

Static preflight reviewed the exact token/profile gate and focused test; `git diff --check` produced no diagnostics. No build or tests were run; root owns integration validation.

Frozen SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `11029B4288B830444A11973059F247BE27435C9CA0D610E9E82224C00CEDC780`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `A7FB77A62B146B3C67CFB2E7165286457F5E0031577B4A8CDDE1DDAF9A2CA275`
