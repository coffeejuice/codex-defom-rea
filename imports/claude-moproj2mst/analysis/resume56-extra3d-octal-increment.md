# Checkpoint 56: bounded Extra3D octal increment

Added one exact base-0 token case to `readDecimalInt`: `010`, delimited by end-of-input or ASCII whitespace, advances the cursor and returns 8. It follows the retained QtCore getter's base-zero leading-`0` branch and base-8 multiply/add path (`analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`, `0x670c57cc` and `0x670c5848`). Existing decimal parsing and the exact prior `0x2` branch are unchanged.

Added `native_extra3d_sw2sp_reads_the_qualified_base0_octal_token` to `tests/mdt/test_simcontrol300_native_io.cpp`. The test reads `SW2SP.DAT` bytes `010\r\n` as 8 and checks that adjacent octal forms (`011`, `02`) plus unqualified hex/binary forms remain unsupported. The older unsupported list now uses `011` in place of the newly supported exact token.

This does not implement general base-0 conversion. Other octal tokens, radix prefixes, signed radix forms, overflow, and malformed-token/partial-row status remain unqualified and guarded. No header, CMake, GUI, or mutable REA state changed. No build or test was run; the source/test/doc are frozen for the coordinator's combined validation.
