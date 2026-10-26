# CP94 implementation receipt: exact SPRING invalid-count boundary

Implemented the one source-qualified `SPRING.INI` failure case from `analysis/resume94-spring-invalid-count-plan.md`.

For exact ASCII bytes `x\r\n`, `readSprings` now opens and decodes the file, leaves the output empty, and returns false. This models Qt's failed integer extraction writing zero and the original count-first wrapper returning based on the initially nonempty stream; the outer helper then skips allocation because the count is nonpositive. All other inputs continue through the existing strict `readDecimalInt` path. Positive count `1\r\n`, `x` without CRLF, other invalid byte `?\r\n`, and codec failures remain guarded. The test also calls public `fillExtra3D` and requires that the scalar form remains available for the false/empty spring result.

This changes only the assigned native IO source and focused test. No headers, CMake, shared Session, GUI, fixture corpus, or mutable REA state changed.

Static review only: no build or test was run; root owns integration validation. `git diff --check` produced no diagnostics for the assigned files (the imported subtree files are untracked in this checkout, so the command had no tracked diff to inspect). The changed source/test regions were inspected directly. Original-process comparison remains pending.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `2CA5E748B1DE801B9E1C74692DDB2B17E9975069BE8F76DCA71FE146C038D4E0`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `31083994AE1B834C7ACCF6FA0D5AA4561DA9070A11506A33916054857F2A0FA2`

