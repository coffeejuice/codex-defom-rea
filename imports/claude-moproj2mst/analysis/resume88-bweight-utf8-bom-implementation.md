# CP88 implementation: bounded B_WEIGHT UTF-8 BOM read

`NativeExtra3DIo::readDoubles` now allows a leading UTF-8 BOM only when the request is exactly one value from `B_WEIGHT.DAT`. `asciiBytes` strips the three recognized BOM bytes using unsigned-byte comparisons, then applies its existing high-byte rejection to the remaining payload. Thus this implements the qualified BOM followed by ASCII numeric text while malformed/non-ASCII payload bytes, BOM-less non-ASCII data, other filenames, and non-one-value requests remain rejected by the previous guard. The integer path and all Save/write paths are untouched.

Added a focused release-active adapter regression using exact bytes `EF BB BF 31 2E 35 0D 0A`; it expects `readDoubles(B_WEIGHT.DAT, oneValue)` to return success and 1.5. The test also checks that a BOM followed by non-ASCII malformed payload remains unsupported, the same BOM input on another filename remains unsupported, and a two-value B_WEIGHT request remains unsupported.

Static preflight: reviewed the explicit unsigned prefix-byte comparison and exact path/count admission; `git diff --check` reported no diagnostics. No build, CTest, GUI, original-process run, or mutable REA action was performed. Root owns build/integration validation. The scope and original evidence chain are in `resume88-bweight-utf8-bom.md`.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `C63D62C31658A1E68504D2669CF80E75E992E85EB9FB25D8E10ABEC5718548CC`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `336277438B47F5C6056124FA4F12C40DAE910D446558CF52BB19ADCB3EB7B2C0`
