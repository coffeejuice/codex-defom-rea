# CP101 independent remesh hex-prefix review

No blocker was found. The frozen change reproduces the original decimal-zero prefix result for a leading `0x`/`0X` after existing whitespace and optional-sign handling. It preserves the zero sign and prior errno, consumes the already-selected token, and continues later fields. It does not interpret hexadecimal magnitude or exponent.

## Native chain and exact admission

Original EXE SHA-256 independently matched `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. All six PE range hashes in `resume101-remesh-hex-prefix-contract.md` independently matched direct section-mapped bytes. I checked bounded offline lexer views and reused the independently checked CP99/100 kind10, converter-zero/config64 and outer-wrapper path.

The lexer recognizes initial decimal0 and enters state1. At `1414870c0..70c6` found-digit becomes true, while nonzero significand count stays0. The next x/X matches neither a decimal digit/separator nor the D/E/d/e exponent branch. `141487140` backs the numerical cursor up to x/X. The finish path therefore reaches zero-result773a, not no-digits7746: status0 and signed extended zero. The converter returns signed double zero/code0; fltin flags remain0. The outer wrapper reaches payload load `141475980` without its errno stores. This preserves negative zero, unlike CP100 underflow's flag0x100 route to freshly cleared positive zero.

The suffix is not numerically visited. Bare0x, hex-looking text, arbitrary visible ASCII suffixes and enormous suffix exponents have the same value/errno outcome within the existing token profile. TokenReader has already advanced over the complete token; its caller does not replay the strtod endptr at x/X. The candidate does not expose or claim a newly reconstructed numerical cursor object.

Matched zero-prefix range hashes include `1414870b2..141487106` = `6dffed7f8b47b29157b6932beaa4967214e2c8445af239e1eb12eb1b0550aa09`, zero status `14148773a..141487746` = `8a619d3d4b7bdf9486cc05c931c0a9c906e06d735d888daf87bcccd9ae403a26`, and outer wrapper `14147592a..141475985` = `1c282cdd4ec4fda664a6784ca4a46a207976a65aaabd8b240c94ce3f270ce331`.

## Frozen diff and four older fixtures

All files below are relative to `source/apps/moproj2mst`:

| File | SHA-256 |
| --- | --- |
| `src/engine/simcontrol300_remesh.cpp` | `26791cd239c998016895ea83161cc79c430413eb47d4efc9059cb586471a34fe` |
| New `tests/mdt/test_simcontrol300_remesh_hex_prefix.cpp` | `e7d5ac6f62a77d9d134680bac049f657165362eb52040c1abf91ee3ebc327fef` |
| `tests/mdt/test_simcontrol300_remesh.cpp` | `5aeab3c83ca5e7fe0b8956dae2dbceea335f7c1d71f9717d47f38f9e0166c566` |
| `tests/mdt/test_simcontrol300_remesh_overflow.cpp` | `039c68109302ff2a9b30c04fce928edba6e2c29cef379359e4756ebd74cd6068` |
| `tests/mdt/test_simcontrol300_remesh_real_overflow.cpp` | `13e8204bd60b78c25785582c13f8fb13788917cdbd60d8844cd410d0166a7e89` |
| `tests/mdt/test_simcontrol300_remesh_real_underflow.cpp` | `71d60f5a56975a53851048732ca2ffd025588dde7cd1365bb71d34286cdad9eb` |

Direct comparison with `build/wave3-checkpoint100-verified/source.zip` shows the sole production change is the existing prefix conditional's nullopt becoming signed zero before errno clearing/host strtod. The sign check uses the character immediately before prefix start, which is minus only when the existing optional-sign handling advanced over it. Bounds remain checked. CP98 integer saturation and CP99/100 canonical range recognition are unchanged.

The four root-owned older fixture diffs are exactly: original guard0x1p2→-INF; CP98 late-real0x1p2→NaN; CP99 guard0x1p2→1.5e9999; CP100 guard0x1p2→1.5e-9999. All other assertions remain, including prior method effects, no loaded/enabled assignment on a later unsupported value, and close count. The last two replacements duplicate an existing refused token in their respective lists; they add no distinct coverage. Hex behavior is now positively checked by the new file, so those duplicates are not an acceptance blocker.

## Tests and limits

Six new Release-active Testkit cases check exact +/-0 bits from a nonzero seed, lowercase/uppercase/bare/non-hex/huge suffixes, unchanged nonzero errno, carried CP98 ERANGE, later integer/boolean fields, scaling fallback1, existing nonfinite/range guards and reached non-ASCII refusal. The ASCII guard precedes numerical interpretation and inspects the reached visible chunk, so ignored suffix bytes do not bypass the candidate profile. That refusal is a reconstruction boundary, not a native claim that all such bytes fail.

The disk case reaches actual project Open with its Session-owned remesh state and checks openedOk, enabled, exact signed zero and later method4 before editor creation. Supported macros, includes and run_all entry point are present; all synthetic writes stay under M2M_TEST_SCRATCH. No optional success branch or fixture-output bypass appears.

The increment does not establish general CRT real conversion, FP-environment/temporary architecture equivalence, original TLS identity, physical fclose errno, public C ABI errno visibility, editor Fill/palette behavior or complete Open/Save parity. This report is the sole write; no build/tests, source edits, GUI, original execution or mutable REA occurred. Evidence contract SHA-256: `fd738aaf44f5852c9ca8503b8ae3b8b2e81767a4881a96ca8337298ccf6343ce`. Root owns focused/full execution and final frozen source/executable binding.
