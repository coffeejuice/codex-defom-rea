# Checkpoint 60: next source-qualified Extra3D integer mismatch

## Selected branch

The smallest current mismatch is the exact lowercase hexadecimal token `0x10` in `SW2SP.DAT`:

- The original Extra3D fill reads one integer from `SW2SP.DAT` through the native QFile/QTextStream helper. The source-qualified decompile is `analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json` for original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; it contains generic integer reader `FUN_1410da690`, which opens QFile ReadOnly, constructs QTextStream, and delegates to the integer stream helper. The Extra3D field binding is retained in `analysis/wave2-save-diffs-300-support-140ec8110.c` and summarized in `analysis/resume54-numeric-path-gap.md`: the SW2SP.DAT fill site requests one integer through this reader.
- QtCore's retained `QTextStream` integer getter is `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`, QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. With default base 0, it recognizes `0x` and sets radix 16 (`0x670C53D3–0x670C53E5`). Its digit loop accepts decimal digits and accumulates each by shifting the current value four bits (`0x670C5590–0x670C55B3`). The exact ASCII token `0x10` therefore yields 16; the end/CR delimiter is ungotten after the two digits, matching the already accepted one-token `0x2
` seam.
- The candidate binds `SW2SP.DAT` to `readInts` in `source/apps/moproj2mst/src/engine/simcontrol300_io.cpp:105`. `readInts` calls `readDecimalInt`; its only hexadecimal exception is the exact token `0x2` in `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`. For `0x10`, the special case misses, decimal scanning stops after `0`, and the following `x` is rejected. The candidate regression currently includes `0x10` among the unsupported integer inputs in `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`.

This is a deterministic radix difference for ASCII bytes and does not depend on ACP or numeric locale.

## Minimal next implementation and fixture

Extend the existing source-qualified lowercase `0x` branch in `readDecimalInt` only enough to accept token `0x10` (end-of-input or ASCII-whitespace delimiter) as value 16. Add a one-element candidate-only test using exact `SW2SP.DAT` bytes `0x10\r\n`; assert successful `readInts` and `value[0] == 16`. Remove `0x10` from the unsupported list because that expectation is the current mismatch.

Keep `0X10`, `0x20`, other hex values, signs before radix prefixes, binary/octal forms, overflow, and malformed-token/partial-row status guarded. The shared getter disassembly makes a broader hex implementation plausible, but this action is deliberately limited to one already-discriminating exact token until the full boundary/status branch is reviewed independently. No source or test file was changed for this audit; root can assign the narrow implementation separately.

## Validation boundary

The original behavior is statically source-qualified from the retained original executable decompile and QtCore disassembly. No original runtime read, GUI, mutable fixture, build, or test was performed. The exact original runtime state is unnecessary for this ASCII radix case; the surrounding QFile/stream state and one-integer field binding are statically tied to the call path.
