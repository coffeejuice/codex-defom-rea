# CP83 SW2SP octal low-dword narrowing

Implemented exact bytes `040000000000\r\n` as zero for a one-value `SW2SP.DAT` read. The octal digits after the leading prefix are `40000000000` = `4 × 8^10` = `2^32` (4294967296). The original getter accumulates in its 64-bit result register; `QTextStream::operator>>(int&)` stores the low dword, which is zero. This is a successful parse with digits, not a conversion-error result.

The candidate branch is exact to those bytes and the existing one-value SW2SP profile. Its test asserts zero and verifies other-path and multi-value requests remain unsupported. No general unsigned-octal parser or octal-overflow rule was added.

Evidence: original QtCore SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; the retained `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` octal loop initializes and accumulates through 64-bit `rdi` (`0x670C57CC–5969`, with digit accumulation at `0x670C5848`); `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm` copies the low dword at `0x670C6166–616A`. The Extra3D sidecar reader requests one `SW2SP.DAT` integer (`analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json`, `analysis/resume67-sw2sp-locale.md`). Static qualification only; no original process comparison.

Static preflight verified the octal arithmetic (`40000000000` base 8 equals 4294967296), exact-token/path/count gates, and focused test. `git diff --check` produced no diagnostics. No build or tests were run; root integrates.

Frozen SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `D091405C36AA0DA0CF13C72E6EFE16E9B47F0CC2F4E19FD9B4AC4868AE4DFDB5`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `F02B6BB68147229B4412CB562A40C3B461C244000E8E11B2849FE1C25664FE7F`
