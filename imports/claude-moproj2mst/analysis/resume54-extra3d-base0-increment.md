# Checkpoint 54: bounded Extra3D base-0 integer increment

`readDecimalInt` now recognizes the exact lowercase token `0x2` when it is followed by end-of-input or ASCII whitespace, returns 2, and advances the cursor past the token. This is the single qualified base-0 case from `analysis/resume54-numeric-path-gap.md`: the retained original integer reader at `0x1410da690` extracts with `QTextStream >> int`, and the original Extra3D fill reads one integer from `SW2SP.DAT`. The remaining decimal branch is unchanged.

Added `native_extra3d_sw2sp_reads_the_qualified_base0_hex_token` to `tests/mdt/test_simcontrol300_native_io.cpp`. It asserts `SW2SP.DAT` bytes `0x2\r\n` read as 2 and that nearby unqualified forms (`0x20`, `0X2`, `0b10`, `02`) remain unsupported. Existing decimal boundary cases remain in place. The test is not built or run here; root owns validation.

This is deliberately not a general Qt integer parser. Prefix variants, octal/binary behavior, signs on prefixed values, overflow, invalid-token/status continuation, and extraction after malformed input remain guarded or otherwise unqualified. No header, CMake, app, project fixture, or REA state was changed. The two source files and this note are frozen for root validation.
