# CP89 implementation: bounded UTF-16LE `B_WEIGHT.DAT` read

Extended the existing `B_WEIGHT.DAT` BOM allowance in `asciiBytes` for exactly one requested double. After an `FF FE` prefix, the helper now requires complete 16-bit little-endian code units, rejects any code unit above ASCII `0x7f`, and converts admitted units to the existing ASCII numeric input used by `readReal`. The preexisting UTF-8 BOM + ASCII path remains intact. No other file path uses the BOM allowance.

Preserved the zero-count early return before file decoding. Requests with more than one value and other file names continue through the prior ASCII-only guard and reject UTF-16LE bytes. An incomplete code unit and non-ASCII UTF-16 code unit are explicitly rejected. No integer, Save/write, or later `STRETCH.DAT` double path changed.

Added a focused regression with exact UTF-16LE BOM bytes for `1.5\r\n`, expecting one-value B_WEIGHT read success and 1.5. It also checks zero-count open success/no decode, multi-value rejection, other-name rejection, odd payload rejection, and non-ASCII code-unit rejection.

Static preflight only: reviewed exact path/count selection, little-endian code-unit assembly, strict incomplete/non-ASCII rejection, and existing decode guards. `git diff --check` reported no diagnostics. No build, CTest, GUI, original-process run, or mutable REA action was performed; root owns integration validation. The original call and codec evidence is in `resume89-bweight-utf16le-bom.md` and the cited CP58/CP59 QtCore captures.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `E053D594C13BEACB7AFC66AB32F08A52731B751B226306CE4D2BD9419FE1F11D`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `C12774B5A7C991E6EFB64735892ACCFF07E92C13FB4EC95CD1FBD767B49340D5`
