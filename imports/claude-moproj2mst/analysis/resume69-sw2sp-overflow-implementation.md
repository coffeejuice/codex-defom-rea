# CP69 implementation: scoped SW2SP signed low-dword case

Implemented the exact static QtCore case established in `analysis/resume69-sidecar-int-next.md`:

- Input: `SW2SP.DAT` bytes `-2147483649\r\n`.
- Scope: exactly one requested integer and path name exactly `SW2SP.DAT`.
- Result: integer `2147483647`.
- Cursor: advances over the numeric token and leaves CR/LF at the current position, matching QTextStream's unget delimiter behavior.

The branch is in `readDecimalInt` behind an explicit boolean passed only when `NativeExtra3DIo::readInts` receives a one-value span for `SW2SP.DAT`. It does not change the generic parser's int32 range behavior. Positive overflow, another negative overflow, an appended non-whitespace suffix, other filenames, multi-value requests, and `SPRING.INI` remain guarded. The existing C-locale signed `0x`/`0X` prefix fallback and lowercase unsigned hex/octal cases remain intact.

A focused adapter test reads the actual SW2SP sidecar, checks the exact overflow result and adjacent in-range `-2147483648`, and confirms those boundary negatives stay guarded for other conditions, including a SPRING count. No build or CTest was run; root validation remains pending.

Frozen file SHA-256 values:

- `src/engine/simcontrol300_native_io.cpp`: `5DEEFCB48D076B7284144ED0AD0693B91A3AF3EE91DB2CB406E5468ACC0B833D`
- `tests/mdt/test_simcontrol300_native_io.cpp`: `FA8315FCE9F1B30DE595AF3C8010B18FA03752B7B896C8328428C5AE1F22AC40`

No header, CMake, GUI, project fixture, or REA state was modified.
