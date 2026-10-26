# CP148 addendum: decimal low-dword token at EOF

Added the focused EOF-terminated companion case requested by independent review. With the existing one-row primary fixture, exact `SPRING2.INI` bytes `18446744073709551617` (no trailing newline) produce flag `1`, and both input files remain byte-identical after reading. This complements the CRLF-terminated decimal wrap matrix and covers the parser's end-of-input terminator branch.

Static preflight found no trailing whitespace. Production reader was not changed; no build or CTest was run. Root owns validation.

- Reader: `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` — `50BC8B56431F31E2B2D8EA005A7D6C55FF7B0D6DDFF161004E455E8CC22E3B58` (unchanged)
- Focused test: `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` — `58810845CF508ABA550EA26AA0BC8BD3359FDF89AD420A6A78E08CB902DCE27D`
