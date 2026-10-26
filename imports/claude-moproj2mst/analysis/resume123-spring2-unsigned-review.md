# CP123 independent SPRING2 unsigned-narrowing review

## Finding: the first value above `UINT32_MAX` is incorrectly refused

The newly admitted `4294967295` branch is source-consistent: original Qt's integer getter accumulates decimal digits in a 64-bit register, and `QTextStream::operator>>(int&)` stores the low 32 bits. The candidate maps `4294967295` to `-1`, and the new two-row test checks that outcome.

However, the refusal boundary at `4294967296` is not source-consistent. This value is only `2^32`, far below the signed 64-bit accumulator limit. The retained getter's decimal loop continues to multiply/add in `RDI` using 64-bit `LEA` operations (`analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`, `0x670C56F6–5704`). The positive-value path does not reject it: `test rdi,rdi; jle` at `0x670C57BB–57BE` is false for positive `4294967296`, then the getter writes its 64-bit result at `0x670C5969–5979`. The retained `int&` operator stores `EAX` to the 32-bit destination (`analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`, `0x670C6166–616A`), giving `0`.

Candidate `readSpringFlagInt()` uses a `uint32_t` fallback after signed `int` parsing fails; tokens outside `UINT32_MAX` throw `M2M_E_UNSUPPORTED` (`source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp:220–231`). The focused refusal matrix includes `4294967296` as unsupported (`source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp:325`). These lines conflict with the original static path. This is a precise boundary defect, not merely an untested hypothesis.

## Confirmed candidate case and adjacent risk

The two-row test for `2147483648\t4294967295` correctly expects `[INT_MIN, -1]` and verifies both input files remain unchanged. Positive decimal `4294967296` should instead produce flag `0` under the same original path. A minimal discriminating candidate-only case is a one-row SPRING fixture with `SPRING2.INI` containing `4294967296\r\n`; the candidate should publish one row with flag zero and preserve both inputs. Adding this case must not imply a bound for all larger values.

The static original accumulator also implies a potentially larger positive range before signed 64-bit overflow, but this review does not qualify the overflow/wrap boundary. Keep larger magnitudes and signed-overflow forms guarded unless they receive a separate bounded proof.

## Basis and limits

Original executable: `DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. QtCore: SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. The original Spring2 extraction loop writes each `int&` result, increments the requested index, and ignores QTextStream status (`analysis/wave2-save-diffs-300-support-1410da690.c`). This is retained static source evidence, not a fresh original-process run.

No source, test, build, GUI, or mutable REA state was changed.
