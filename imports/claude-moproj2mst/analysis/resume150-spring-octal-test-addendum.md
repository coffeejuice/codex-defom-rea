# CP150 octal boundary and whitespace test addendum

Added two focused count-one reader assertions to the existing SPRING2 complete-octal test matrix:

- `017777777777\r\n` maps to `INT32_MAX` (`2147483647`), the largest admitted octal value.
- `011\t` maps to `9`, covering a tab terminator in addition to CRLF and EOF.

Both assertions use the existing one-row primary fixture and verify exact byte preservation of both input files. No reader code changed.

Static preflight: `git diff --check` is clean. No build or CTest was run, per assignment.

SHA-256:

- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp`: `4611B787F5C532E36B00B3D29A4C610C069DE96BD70EFCE78E7213ED19C68ABD`
- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` (unchanged): `C4340EA223AB9B6D701EB770328E5C888C7F0609BD07FEE6F17BA5A762FC968C`
