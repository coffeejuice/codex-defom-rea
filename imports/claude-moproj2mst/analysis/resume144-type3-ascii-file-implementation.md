# CP144 private Type3 ASCII `.OPT` file slice

Added `parseType3OptAsciiFile` as a private, candidate-only file front-end over the existing scalar sequence, CP141 region-value helper, and CP143 region-row sequence. Its admitted byte profile is deliberately narrow: an absolute path to a successfully opened/read file, BOM-free seven-bit ASCII, LF separators only, no CR/NUL/control bytes except TAB, and a final LF for nonempty input. Empty files are admitted as a special case. Non-ASCII codecs/BOMs, CRLF/bare CR, unterminated final lines, relative paths, and read errors are rejected or reported as explicit candidate outcomes; these are not native parser failure claims.

The helper opens before setting `state.value48=true`, matching the observed open-success → optimizer `+0x48=true` boundary for this value projection. It consumes the first line separately, creates and discards the bounded ASCII trim result, then converts the original header with `qt::toInt` and discards the value. The remaining lines are applied in order: scalar lines through the existing dispatcher, `NUM_CON` blocks through the existing sequence helper, and `REGIONS` blocks through CP143's raw-row sequence helper. ENDDATA stops later mutations. Natural short counts preserve completed rows and return the EOF outcome. The original parser's observed bool return is not represented as candidate acceptance; its false result after parsing remains a separate fact.

The focused tests cover scalar + NUM_CON + REGIONS order with ENDDATA stopping a later scalar, open failure before `value48` mutation, CR/high-byte/unterminated profile guards, early EOF inside a REGIONS count, and empty-file open success. No OS read-error injection is attempted. If a byte read fails after open, this helper reports `ReadFailureUnsupported` after setting `value48`; because it preloads bytes before parsing, it does not claim that any native prefix rows would be preserved for a late read error. That entire failure profile remains unsupported.

Static preflight: `git diff --check` passed for the owned files. Existing CMake recursively collects `src/*.cpp` and `tests/doe/test_*.cpp`; no CMake/shared-header edit was made. No build/test, GUI, original runtime, or mutable REA action occurred. Public Type3/ShapeOptim admission and native optimizer ownership remain unchanged.

SHA-256:

- `source/apps/moproj2mst/src/ops/type3_opt_ascii_file.hpp`: `6E59F46B8ECF738CEF7035687893FBBCFA80B90A7EB8D9BB8999E015DE80542A`
- `source/apps/moproj2mst/src/ops/type3_opt_ascii_file.cpp`: `71B7B6F0DB71620EB121B05F18463DBF7197FBFEFDDE7CC9BA58A08A23CC8C87`
- `source/apps/moproj2mst/tests/doe/test_type3_opt_ascii_file.cpp`: `B997240B13C226D77AA946ED238A2440B1DFD440B4DD8001272AA772C637A0A1`
