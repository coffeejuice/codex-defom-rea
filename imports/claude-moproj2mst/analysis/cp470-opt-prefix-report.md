# CP470 fixed .OPT stream insertion prefix

The retained original writer inserts three fixed NUL-terminated ASCII literals in this order, each followed by the QTextStream `endl` manipulator:

1. `DEF_OPT       \t1.0`
2. `TYP_OPT       \t1`
3. `GRD_TYP  \t\t10`

It next calls optimizer slot `+0x138`, inserts literal `KRG_MAX       \t`, inserts the returned int operand, and calls `endl`. It then calls helper `0x14091e0b0`, inserts literal `OPR_NO        \t`, inserts that int operand, calls `endl`, and calls a standalone `endl` before moving to the `+0x128` getter. This is the source-closed call and insertion order, including the distinct blank-line manipulator. It is not a guarantee about final newline bytes, flush success, codec, number formatting or physical file contents.

`analysis/cp470-opt-prefix-verify.py` maps the five literal addresses through original EXE PE sections and compares all 82 bytes including NUL with fixed expected data and hashes. The receipt `analysis/cp470-opt-prefix-byte-verification.json` binds original EXE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and the CP465 bounded writer disassembly. CP465's complete `0x140907a10..0x140908627` byte comparison binds the cited call instructions. The writer invokes QtCore QTextStream const-char insertion, int insertion and `endl` through separate imports; their complete implementation bodies are not recovered here. CP117's prior Qt default-state evidence identifies C numeric locale, precision 6 and codec-for-locale dependence, without closing the device output.

The existing CP466/468 prepared sink stores header and scalar-prefix entries as generic strings, so it cannot distinguish a literal insertion, a prepared integer operand, and an `endl` attempt. A private typed-prefix emitter may add those three event kinds and preserve the exact order while accepting already-prepared integer values. It must not claim native getter fault timing, Qt formatting, flush success or exact `.OPT` bytes. Public Type3 Save remains guarded. No original process, GUI control or mutable REA session was used.
