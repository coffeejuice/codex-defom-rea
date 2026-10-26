# CP125 `SPRING2.INI` signed low-dword witness

## Change

The bounded companion flag parser now accepts the exact canonical token `-2147483649` when it ends at whitespace or EOF and maps it to `2147483647`. It keeps other negative overflow magnitudes, signed-positive overflow, suffix-attached variants, radix/leading-zero inputs, wider accumulation, BOMs, and non-ASCII data guarded.

The focused one-row reader regression uses a complete `SPRING.INI` record and `SPRING2.INI` bytes `-2147483649\r\n`. It asserts successful read, one flag of `2147483647`, and byte-for-byte preservation of both inputs. Existing refusal checks now retain `-2147483650` and `+2147483648` outside this exact branch.

## Source qualification

Original `DEF_GUI_MO_64.exe` SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Shipped QtCore SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

The same QtCore operator `QTextStream >> int` is called for companion flags by `FUN_1410da690` (`analysis/wave2-save-diffs-300-support-1410da690.c`). The getter accumulates the decimal magnitude with 64-bit multiply/add (`analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`, `0x670C56F6–5704`), applies the negative sign in the completion branch (`0x670C5799–57C7`), and writes the 64-bit parsed result on success (`0x670C5969–5979`). `-2147483649` fits in that signed 64-bit result, so it is successful conversion. The `int&` operator stores the low dword (`analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`, `0x670C6166–616A`), yielding `2147483647`. The companion reader maps that destination to the record flag and ignores stream status.

This is a static source-qualified arithmetic result, not an original-process differential run.

## Static preflight and limits

`git diff --check` passed for the two owned files. No build or tests were run; root owns integration. No shared header, page, public Session, GUI, or REA state changed.

## Frozen hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `28ADEB83E070F3EE69467D1CF43DADD3DCC8706653BEA6B630A482AC60118E07` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `87AEC195A6D86C21C1A20E1F567879CD6252038CA3C621178E07917F82DC675C` |
