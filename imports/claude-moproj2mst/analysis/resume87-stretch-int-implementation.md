# CP87 implementation: `STRETCH.DAT` one-int Qt parser route

Extended the existing private Qt 4 base-0 integer route to accept exactly one requested value for either `SW2SP.DAT` or `STRETCH.DAT`. This follows the shared original `FUN_1410da690` caller evidence in `resume87-stretch-int-successor.md`. No helper conversion logic changed. All other filenames and spans of zero or more than one integer remain on their prior branches; ASCII decoding, open behavior, the later three-double `STRETCH.DAT` read, and `SPRING.INI` count path are unchanged.

Added a focused release-active adapter test for exact `STRETCH.DAT` bytes `0X10\r\n` → 16. The same test asserts that a different filename retains the legacy zero result for this uppercase-prefix input and that a two-value `STRETCH.DAT` read remains outside the new parser route.

Static preflight only: `git diff --check` reported no diagnostics. No build, CTest, original-process comparison, GUI or REA mutation was performed; root owns build/integration validation.

SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `C60B3291B6306B256699BA671AFC8BD466242E8DD51D5BCE7F44226F802A96E5`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `08EEBC85A6DE0684FDD9A89EC292BCD44071845F56D6E58D2EF72DB1023E7E46`
