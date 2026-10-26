# CP99 public remesh canonical real overflow contract

One further public reader branch is source-qualified: after existing D/d-to-E
normalization, a complete token matching `[+-]?[1-9][eE][+]?[0-9]+` with exponent
strictly greater than5200 returns signed infinity and sets errno34. Restrict the
increment to that class. Do not generally admit host strtod overflow, underflow,
INF/NaN spellings, hexadecimal input, fractional/multidigit mantissas or trailing
junk. No selected palette observation is needed for the actual remesh Open call.

Only this contract was written before source work. No source/build/test,
GUI/original execution or mutable REA. Bounded immutable PE/DUMPBIN inspection
closed the native path below. Root has frozen CP98 and approved the exact
two-file successor after this evidence freeze.

## Reconciliation and public effect

CP98 already fixed decimal integer saturation and errno; retain it unchanged.
The CP98 terminal matrix concerns disabled removal plus earlier returned Save
failures. Production already ignores the remove/write/close results correctly;
that proposal is an acceptance matrix case, not a missing production decision.

`originalReal()` still refuses every nonfinite or ERANGE host result. Thus
`FILTER_DISTANCE 1e9999` stops actual Session Open today. Native instead stores
+infinity, continues later fields and returns loaded=true/enabled=true. A
`SCALING_FACTOR_EACH_LAYER_INWARD` overflow likewise returns infinity before the
existing original comparisons clamp it to10 (positive) or1 (negative). This is
an actual public setting-reader continuation through existing
`moproj/read.cpp -> Session::loadProjectRemesh`, not a page-helper extension.

## Exact lexer/status/value/errno chain

TokenReader kind10's jump entry at RVA906e24 is906d75. That body normalizes the
exponent marker, calls `1414759b8` with an endptr local, and stores XMM0 into the
same actual field. The wrapper clears locale argument R8 and jumps to
`14147583c`. After whitespace handling it calls `141486ccc` at141475916.

`141486ccc` invokes lexer `141486ea4`, then its internal extended-to-double
conversion. It maps lexer's return-bit2 to result flag0x80 at141486d42..6d52,
independent of that local conversion result. The selected complete lexical
class has one nonzero significand digit, no decimal normalization offset and a
positive explicit exponent:

- initial sign handling is141487148..7164; digit1..9 enters state3 at7074;
- one digit is stored by14148701f..704d; E/e/D/d enters exponent state6 via
  141486fe8..700d;
- positive exponent digits use141487184..72c4; optional plus is723c..7255;
  leading zeros are handled in71e4..7208;
- exponent accumulation above0x1450 is capped to0x1451 at141487294..72ae.
  With the single-digit integral mantissa there is no decimal-offset change;
- `cmp ebp,1450h` / JG at141487359..735f reaches141487724, which constructs
  extended infinity and returns flag2. It bypasses the power-table path;
- final extended result/sign stores and return are141487755..779c.

The outer wrapper sees overflow flag0x80 at141475940. It loads +HUGE_VAL from
143b5f970, bits7ff0000000000000, and for a leading minus XORs sign mask
1421d3d30, bits8000000000000000. It calls the same errno accessor used by CP98,
writes0x22 at141475978, and returns the selected infinity in XMM0. Native
endptr progress is unused by the reader after the complete token was consumed.

The normal converter call remains part of native local work; for this route
the subsequent lexer-overflow flag selects the fixed outer result. This does
not authorize copying arbitrary converter output/flags from a host CRT. In
particular the native wrapper's underflow/zero branch has distinct behavior and
is not included. Preserve the existing normal CRT/locale/TLS success profile.

## Minimal source/test contract and ownership

Root approved these exclusive files only:

- Existing `source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp`: recognize
  the complete canonical class after existing exponent normalization, before
  host strtod. Accumulate/cap the positive exponent at5201 without host integer
  overflow, requiring every remaining character to be a digit. On qualification
  set ERANGE and return the fixed signed infinity. Otherwise retain all current
  real guards and ordinary conversion behavior. Do not clear errno first or
  invoke host overflow conversion for the newly supported class.
- NEW `source/apps/moproj2mst/tests/mdt/test_simcontrol300_remesh_real_overflow.cpp`:
  dedicated Testkit cases through read/loadRemesh and actual Session Open.

No header/CMake/build/GUI. Independent reviewer owns only its report. Root will
change the stale existing real-refusal token1e9999 to still-guarded1.5e9999 after
source release; CP98's late-hex prefix case remains valid and unchanged.

Focused cases must verify signed infinity bits and errno34; mantissas1..9,
positive sign/optional exponent plus, leading exponent zeros and long exponent
strings; later fields/enabled continuation and source scaling clamp10/1;
actual Session Open using a scratch fallback; and retained refusal of underflow,
INF/NaN, hex, trailing text, fractional/multidigit mantissas and exponent5200
range profiles. Avoid mistaking finite ordinary tokens for refused profiles:
small exponent values remain accepted by the existing ordinary parser.

## Native receipt

Imported original EXE SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
DUMPBIN14.44.35228.0 SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Preferred-VA half-open byte ranges:

| Range | SHA-256 |
|---|---|
| `140906e24..140906e28` | `83f93fdbe04e8c9f2f6c51d34ff8b921bc0c330422d00fe3ebef3d623077bb01` |
| `140906d75..140906daf` | `bcd8b0e3ddb5fb658afec1b40896b82eb96420010a04880ccc5a2ffd3e357ce3` |
| `14147583c..1414759c0` | `42baa7021035b346b0695f2dc5020381da562f042e63bcd0892979ff76bf188e` |
| `141486ccc..141486d95` | `54916ad926e930ab4851f1ad09aaf3cc781a4d93eb200cf51b11c47c10f025f3` |
| `141486ea4..14148779d` | `89b29985132ef432b85a6f949f1837b73ea2ab4b9b8baf14ba7c5f38aea1f4f5` |
| `143b5f970..143b5f978` | `9402bb655bc3b7331a00f1219ef647565bbe517c74aa0e41026885785867d1cd` |
| `1421d3d30..1421d3d38` | `e6ad6c9a3a3b7658c35bacf6553fcb8ffe34387534a648fe18f875b8f7a86ddb` |

No new raw artifact was written. This is a bounded public numeric increment,
not general CRT real-conversion recovery or editor300 constructor completion.
