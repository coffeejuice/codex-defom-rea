# CP92 implementation receipt: STRETCH three-double ASCII path

Added a focused public `fillExtra3D` regression for the source-qualified `STRETCH.DAT` call sequence. With bytes `1\r\n0.25\r\n0.5\r\n`, it checks the one-value integer read yields stretch kind `1`, then the separate count-three double read restarts at byte zero and populates the displayed values as `0.25` and `0.5`. A second fixture `1\r\n0.25\r\n` checks the third double remains zero after EOF. A three-value UTF-8-BOM `STRETCH.DAT` read is still rejected by the existing decoder guard.

No production source change was needed: the current `readInts` and `readDoubles` methods open the file separately, initialize outputs, and the double loop already follows the bounded valid-token/EOF behavior. No BOM support was widened to the three-value path; no malformed token behavior was newly admitted.

Static preflight only: `git diff --check` reported no diagnostics. No build or test execution was performed; root owns validation. The original caller/count and Qt stream state evidence are recorded in `resume92-stretch-multiread-contract.md`.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` (unchanged): `C043D36FF5FE5BF54809C7316DB01B3AA90F1783D6F848D807B468271F4755C7`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `E76EFDE7E7EB12F7E67ACA0CB42339E023E99714FA68B720BB981A18A983B115`
