# CP80 SW2SP invalid-octal first-digit case

Implemented exact bytes `08\r\n` as zero only under the existing one-value `SW2SP.DAT` profile. The original base-0 getter selects octal from the leading zero, rejects `8` before accepting any octal digit, and returns conversion failure. The `int&` operator writes zero and sets stream status; the SW2SP caller ignores the status and reports success after opening the file.

The new focused test asserts success/value zero for the exact fixture and checks the same bytes remain unsupported for another path and a two-value request. Existing exact octal/binary witnesses and the other unsupported binary/octal assertions remain intact. This change does not implement general octal error handling, other invalid digits, partial-token recovery, or a public stream-status model.

Static source evidence is described in `analysis/resume80-sw2sp-octal-invalid-next.md` and retained QtCore disassembly; no original-process runtime comparison was performed. Static preflight reviewed the exact-byte/profile gate and the scoped regression. `git diff --check` completed without diagnostics. No build or full test suite was run; root integrates.

Frozen SHA-256:

- `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`: `DD14400F9937AF2424595F5787BE912016AFBF8AC5E285832C8A5DE211EF6BD2`
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_native_io.cpp`: `A5A50401F8535F26D8EAC58132FC77D6D9DA4121CB087FD9F86F38B1D6625DDA`
