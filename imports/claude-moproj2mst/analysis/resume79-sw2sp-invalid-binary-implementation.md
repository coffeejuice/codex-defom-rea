# CP79 SW2SP invalid-binary failure result

Implemented the exact `0b2\r\n` one-value SW2SP witness as a returned zero. The original Qt getter recognizes the base-2 prefix, rejects the first bit because only `0` and `1` are accepted, and reports conversion failure. `operator>>(int&)` writes zero and sets stream status; the original SW2SP reader ignores that status and returns success after open. The candidate branch is bounded to the exact bytes and existing one-value `SW2SP.DAT` gate.

The focused radix test now contrasts valid `0b10\r\n` -> 2 with invalid-first-bit `0b2\r\n` -> 0. It retains unsupported checks for other invalid bits, uppercase `0B`, non-SW2SP path, and multi-value requests. No behavior for generic malformed binary inputs, subsequent extraction after sticky failure, or externally visible stream status is claimed.

Source qualifications are documented in `analysis/resume79-sw2sp-next.md` and the retained QtCore captures. Static preflight reviewed the exact bytes, profile gate, and negative scope; `git diff --check` completed without diagnostics. No build or tests were run; root owns integration validation.

Frozen SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `CCDBFCEC34D0186744302F3243E268DD2F4A28DA607617A7894046E26A560CD9`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `F16E9ACCA1BBA9F13E8443614A1112DFF9F79B93F707F9732D310CD45AEC3FBA`
