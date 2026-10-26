# CP100 independent canonical underflow review

No blocker was found. The frozen change preserves CP99 positive-exponent overflow and admits only the new complete token class `[+-]?[1-9][eE]-[0-9]+`, after existing D/d normalization, with exponent magnitude strictly above5200. It correctly returns positive zero for either mantissa sign and sets errno34.

## Native value/status chain

I independently verified original EXE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and all six byte-range hashes in `resume100-remesh-real-underflow-contract.md`. Direct PE reads also confirm converter configuration `143b5fae8` is64 (`40000000` bytes), selecting double-width output. CP99's kind10 dispatch, lexer and wrapper route remains applicable.

For one nonzero integral significand digit the decimal normalization offset is zero. The exponent magnitude cap5201 is negated at `141487336..733c`; strict JL below -5200 at7365/736b reaches7714. That branch constructs extended zero and returns lexer flag1; final sign insertion can preserve a negative extended zero.

The converter cannot be skipped in this sign proof. Independently inspected zero path `141491934..19aa` takes the all-zero significand route to1ecf, packs the sign into double zero using the64-bit configuration, and returns code0 via EBX. The fltin layer maps lexer bit1 to flag0x100 at `141486d52..6d61`, without a competing overflow flag. The outer wrapper tests bit8 at `14147595c`, loads the signed-zero payload, clears XMM6 with XORPD at5967, and compares the payload to that zero. Either sign compares equal, so it writes errno0x22 at5978 and returns cleared XMM6. It bypasses the payload reload at5980. Thus negative selected inputs produce bits `0000000000000000`, not negative-zero bits.

The independently matched converter-zero range hashes are `116949b33eb66debffd02494b8ff8dee5605c11de744d7704e5712a5868965cc` and `3cb062c0b8d07cd567356bcf3b1721f91017401525795fbd4672d4631faa8c00`; outer zero-wrapper range hash is `7b0a25e8d1a649c04929c0688d3593a1377f113298253546a54e76298a1ed91b`. These are offline original-code conclusions, with no original execution or mutable REA.

## Frozen source and tests

| File beneath `source/apps/moproj2mst` | SHA-256 |
| --- | --- |
| `src/engine/simcontrol300_remesh.cpp` | `74700a4f742bafd903472cd9cff6318c1577bcea9daeeff9337184fc891b05c0` |
| New `tests/mdt/test_simcontrol300_remesh_real_underflow.cpp` | `9d632bb558053960de3eda83b37e5e76930007ef7e2ed04668df786022c31839` |
| Root-amended `tests/mdt/test_simcontrol300_remesh_real_overflow.cpp` | `859538a75d62ad8a497626e5a627080e46e66301f5f7b6b084cd3a9d417d088f` |

Direct comparison with `build/wave3-checkpoint99-verified/source.zip` shows only the helper rename, exponent-minus recognition, and ERANGE/+0 return branch. CP99's positive class and CP98 integer handling remain intact. The digit loop still validates the entire suffix after capping; its largest arithmetic temporary is52009. Empty suffixes, extra signs, trailing text, fractional/multidigit mantissas and magnitude5200 fail the new recognizer. The zero result remains an engaged optional, so the caller does not confuse it with an unsupported profile. The existing fallback, locale/path/ASCII/chunk limits and native reader ownership are unchanged.

Six new Release-active Testkit cases check exact +0 bits from a nonzero seed and errno34 before fake close effects, both mantissa signs, D/d markers, exponent zeros/long magnitudes, later settings and enabled assignment, scaling lower fallback1, retained refusals, ordinary signed-zero/finite fallback, and CP99 signed infinity preservation. The disk case reaches actual project Open with a Session-owned RemeshState and requires +0, later method4 and opened/enabled state before editor creation. Supported assertion macros, includes and run_all entry point are present; scratch writes stay beneath M2M_TEST_SCRATCH. Root's old-test diff changes exactly two now-supported underflow tokens to still-refused fractional forms.

No test admits general host underflow, denormals, arbitrary real syntax or unsupported range profiles. Ordinary zero-mantissa signed-zero checks describe retained fallback behavior, not newly generalized native conversion. The increment does not reproduce the full legacy temporary/converter architecture, FP environment flags, original CRT TLS identity, physical fclose errno, C ABI errno visibility, editor Fill/palette behavior or full Open/Save parity.

This report is the sole write. No source/test edits, build/tests, GUI, original execution or mutable REA occurred. Native inspection used bounded DUMPBIN14.44.35228.0 disassembly plus direct immutable PE reads. Evidence contract SHA-256: `76ed0b5281fefcd5ee3f612a69f713126cd318d8a4acb24c29ee4583f924cc6e`. Root owns focused/full execution and final frozen source/executable binding.
