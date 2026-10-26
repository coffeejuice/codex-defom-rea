# CP91 implementation receipt: NBC two-double ASCII row

The retained native reader and CP90 source already implement the qualified ASCII two-value path, so no production source change was needed. Added a focused test for the reachable `NBC.DAT` count-two call: valid bytes `1.5\r\n2.5\r\n` yield `[1.5, 2.5]`; a one-value file `1.5\r\n` yields `[1.5, 0.0]`, preserving the original zero initialization and EOF/partial-value behavior. This exercises the candidate loop against `FUN_1410da510(..., 2)`'s source-qualified atEnd/extract/index ordering.

The same test confirms codec boundaries remain unchanged: UTF-8 BOM input stays unsupported for `NBC.DAT` count two, and UTF-8 BOM input stays unsupported for the three-value `STRETCH.DAT` double read. CP90's all-BOM decoder remains limited to one-value `B_WEIGHT.DAT`; no malformed, BOM-bearing multi-value, or STRETCH count-three behavior was admitted.

Static preflight only: `git diff --check` reported no diagnostics. No build or test execution was performed; root owns integration validation. No native source file changed in this checkpoint.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` (unchanged): `C043D36FF5FE5BF54809C7316DB01B3AA90F1783D6F848D807B468271F4755C7`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `9CAE0E8493ED99D11655B8BDEE0DF246A5568AF54376CE5BA7F46C0376CB065D`
