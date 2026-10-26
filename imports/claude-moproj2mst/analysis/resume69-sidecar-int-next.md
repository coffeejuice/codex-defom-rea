# CP69: next reachable sidecar integer fidelity branch

## Call path

`SW2SP.DAT` is read during the original Extra3D Open/fill path: `FUN_140ec8110` invokes `FUN_1410da690` with a one-element integer destination, then formats the returned `int` into the enabled line edit with `QString::setNum(int, 10)` (`analysis/wave2-save-diffs-300-support-140ec8110.c`, literal binding in `analysis/wave2-save-diffs-300-extra3d-keywords.json`). On Save, `FUN_140ec9040` reads that line edit text and passes it to the Extra3D `QTextStream` writer (`analysis/wave2-save-diffs-300-support-140ec9040.c`, writer contract in `analysis/wave2-save-diffs-300-extra3d-increment.json`). This makes the one-int sidecar parser reachable in Open→Save.

The current implementation reaches `readDecimalInt` from `NativeExtra3DIo::readInts`. Its decimal branch calls `qt::toInt`, which enforces signed 32-bit range in `source/apps/moproj2mst/src/qt4/number.cpp`. That is a different range contract from the original `QTextStream::operator>>(int&)` path.

## Best next branch: signed decimal narrowing past INT_MIN

QtCore's `QTextStreamPrivate` constructs this stream with `QLocale::c()`; CP67's retained constructor/table evidence establishes C-locale signs (`+`, `-`) and group separator comma. The getter is `QTextStream::getInteger` at `0x670C5210`; its decimal loop seeds a digit accumulator and repeatedly computes `value = 10 * value + digit` at `0x670C56F6–5704` without an `int32` range check. For a recognized C-locale negative sign, the completion path negates the positive magnitude at `0x670C5799–57C7`. It stores the resulting 64-bit value at `0x670C5969–5979`. `QTextStream::operator>>(int&)` then copies the low dword to the requested `int` at `0x670C6166–616A`.

A minimal one-int SW2SP witness is exact bytes `-2147483649\r\n`. The stream recognizes `-`, takes decimal mode, accumulates magnitude `2147483649`, negates to `-2147483649` (still representable in its 64-bit accumulator), and narrows low 32 bits `0x7fffffff` to `int 2147483647`. The conversion has digits and a C-locale CR/LF delimiter, so the getter takes its successful parsed-number return path. The outer `FUN_1410da690` initializes the destination to zero, performs the requested extraction, does not inspect QTextStream status, closes, and returns true after a successful QFile open. Thus the Original Open branch enables the field with integer `2147483647`; Save can serialize the displayed decimal value. Current `qt::toInt` rejects the input as outside int32 and `readDecimalInt` throws `M2M_E_UNSUPPORTED`, so it cannot reach that Open→Save state.

This is the strongest next candidate-only test seam: `SW2SP.DAT` containing `-2147483649\r\n`, one-element `readInts`, expected result 2147483647. Keep it scoped to SW2SP's one requested value while overflow-status and repeated-extraction semantics are developed; do not apply the same rule to counts that control allocation without separate analysis.

## Other inspected branches; keep bounded

- **Signed leading-zero decimal:** because the original sees the C-locale sign before a radix prefix, `-010` selects decimal and should yield `-10`, unlike unsigned `010` which selects octal and yields 8. Current `readDecimalInt` rejects multi-digit leading-zero signed decimals. This is also source-supported, but it is secondary to the range-narrowing branch above.
- **Grouping:** this stream's separator is comma. The getter has a dedicated grouping path and compares separator/group-size metadata before consuming it (`0x670C5706–578B`). Do not accept arbitrary commas or infer all grouping placements from the separator alone; group-size validity still needs its own bounded matrix.
- **Uppercase hex digit:** lowercase `0x` selects the hex loop, which recognizes `0`–`9` and lowercase `a`–`f`; uppercase `A`–`F` do not increment the digit count (`0x670C5590–55E6`). Therefore `0xA` has no accepted hex digit: `operator>>(int&)` writes zero and sets stream error status. The original one-int reader does not inspect this status and still returns true after file open, so its caller-visible one-int destination is zero; the candidate currently throws. This is a separate status/failure branch, not permission to accept uppercase hex digits or continue parsing a following field.
- **Positive overflow:** similarly, `2147483648\r\n` produces low32 `0x80000000`, i.e. `INT_MIN`, if confined below 64-bit overflow. Very long decimal strings can wrap the 64-bit accumulator and need a separate bound; do not generalize past the shown finite range without qualification.

`SPRING.INI` uses an integer count before allocation, so the SW2SP witness should not be reused as a spring-count fixture. A successful low32 count can change allocation/iteration behavior in `FUN_1410dab40`; that reader requires its own risk-bounded contract. The parser analysis here applies only to values requested by the SW2SP one-int call.

## Evidence boundary

QtCore identity: `evidence/original-binaries/QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Relevant retained disassembly: `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm` and `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`; C-locale construction/data: `analysis/resume67-sw2sp-locale.md` and its bounded `resume67-*.asm` captures. This is static source evidence, not a fresh original application run. No source/build/GUI/REA changes were made.
