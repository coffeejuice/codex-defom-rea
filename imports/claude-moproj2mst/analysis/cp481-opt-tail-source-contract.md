# CP481 complete ordinary Type3 `.OPT` tail

The retained original `DEF_GUI_MO_64.exe` has SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. CP465's complete writer range `0x140907A10..0x140908627` has raw SHA-256 `66b545af5736084402aa28577578924eced302d90646717bf943e81368313f26`; tail through return `0x140908489..0x140908627` has raw SHA-256 `5e8f612d429cd05bf376eab282697923c4cff65363df1d8386fc2c46ff44d038`.

After the REGIONS section, the writer inserts a standalone `endl` at `0x140908495`. It then calls optimizer slot `+0x148` at `0x1409084AD`, retains the double result in XMM6 before any EPS output, inserts literal `EPS_OBJ       \t` at `0x1409084C3`, inserts that double at `0x1409084D0`, then performs `endl` at `0x1409084E0` and another standalone `endl` at `0x1409084F1`. Only then it calls optimizer slot `+0x158` at `0x140908509`; its byte result selects `DB_PURGE\t\t0` when nonzero and `DB_PURGE\t\t1` when zero. Each branch has two `endl` calls. Finally it inserts `ENDDATA`, `endl`, explicitly closes QFile at `0x1409085A2`, and returns false after temporary cleanup. No ordinary stream-status or QFile error check changes this sequence.

The slot `+0x148` target `0x140918A20..0x140918A30` loads `owner+0x40` (raw SHA-256 `021f59bc8202976ab19d894c43b76e6844ba2f44d5a47c417de8917b6d23e82b`); `+0x158` target `0x140918A70..0x140918A7E` loads byte `owner+0x48` (raw SHA-256 `155e24c9f1327baaa55e6cfd5be5bcafea5b4b7949a3e9aa833dcf247912a8a8`). Candidate DefOpt view already exposes `value40()` and `value48()`.

PE literals include their terminating NUL, which QTextStream does not insert:

| VA | C string | Exact bytes including NUL | SHA-256 |
|---|---|---|---|
| `0x1427BA088` | `EPS_OBJ       \t` | `4550535f4f424a202020202020200900` | `bf6b99cbd2ebe36b7e06ce7cee8984a77399b7437c0f7d2b13477862de0031b0` |
| `0x1427BA098` | `DB_PURGE\t\t0` | `44425f505552474509093000` | `23730732934fbcaa1a8ff5bb1be72cbcf268b418a56ed78d76aa4f47936aa21c` |
| `0x1427BA0A8` | `DB_PURGE\t\t1` | `44425f505552474509093100` | `dda88fdd7aa9bf68affcb82e3176cbd1e431b56c9f707047caff38ec6d0b932e` |
| `0x1427BA0B8` | `ENDDATA` | `454e444441544100` | `49fd6690fd22f29c44f06872bf7c705953b26132fbde22b8b8045908dfe818d4` |

The typed-manager profile can also suppress artificial prepared `scalarPrefix`: CP465 shows OPR's blank `endl` at `0x140907C92`, then the `+0x128` getter at `0x140907CB5`, with no intervening insertion or helper. Its `value34` capture belongs immediately after that blank line and before groups. A complete typed tail suppresses prepared `finalLines`: ENDDATA `endl` at `0x140908598` is followed by QFile close with no intervening insertion. Generic/WithGroups prepared fragments remain compatibility seams.

This closes the ordinary private insertion-attempt order with prepared optimizer state. Native Qt encoding, physical writes and errors, exception cleanup, actual owner lifetime, public Open/Save/Close admission, and fresh original acceptance remain separate.
