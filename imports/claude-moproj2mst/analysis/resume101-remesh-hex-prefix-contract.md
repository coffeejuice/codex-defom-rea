# CP101 public remesh 0x prefix contract

The current explicit 0x/0X guard can be replaced by one original-backed result:
after the existing optional sign/whitespace handling, a leading0x or0X parses
only the decimal zero and stops numerically at x/X. It returns that signed zero
and preserves errno. It does NOT interpret a C99 hex float. Thus0x1p2 yields
+0, -0x1p2 yields -0, and a huge exponent in the suffix does not cause ERANGE.
Keep every other real guard and the existing ASCII/token/stream profile.

Only this evidence report was written. No source/test edits, build/test,
GUI/original execution or mutable REA. Root has reported CP100 archived; exact
source/test ownership still requires agreement before writes.

## Exact native lexical/value route

Reuse the kind10 call route and normal KnownC/TLS profile pinned by CP99/100.
Initial mantissa sign is retained in the lexer sign word. Reading0 from its
initial/sign state goes through1414870b2..70bb into zero-prefix state1.
At1414870c0..70c6 the lexer records found-digit=true, but the count of nonzero
significand digits remains0.

For the next x/X byte, the decimal separator and digit/sign tests fail. The
exponent tests141486fe8..7002 accept D/E/d/e, never x/X. They jump through
141487140 to the finish path1414872d2. Found-digit is true, so this is not the
no-digits/status4 path. Zero significand count branches141487303..7305 to
14148773a, writes extended zero and returns status0. Final sign insertion
preserves a negative mantissa as extended negative zero.

The zero converter paths/config64 already pinned by CP100 produce double +/-0
with converter return0. `141486ccc` therefore leaves all result flags0.
The outer strtod wrapper bypasses invalid/overflow/underflow handling and loads
the payload directly at141475980. No errno store is reached. This differs from
CP100 underflow, whose flag100h selects a sign-clearing XORPD branch and ERANGE.

The original computes string length/normalizes marker bytes before lexical
conversion, but does not numerically parse the suffix after x/X. The endptr
points at x/X; TokenReader's complete-token cursor is already advanced and
does not use that endptr to replay suffix bytes. Any suffix within the existing
visible ASCII token bounds has the same result, including bare0x, a valid
hex-float spelling or non-hex text. No new binary/codec/oversized-token profile
is admitted by this prefix result.

## Proposed minimal implementation and public effects

Use the existing hex-prefix conditional in `originalReal()` after normalization
and canonical range recognition. Instead of nullopt, return +0.0/-0.0 according
to the existing mantissa sign, BEFORE the ordinary parser clears errno/calls
host strtod. Do not invoke modern hexadecimal conversion or clear errno on
this new branch. Preserve CP98 integer and CP99/100 canonical range behavior.

Actual Session Open already calls this reader before editor construction.
FILTER_DISTANCE receives signed zero; source scaling comparisons choose1;
later records and loaded/enabled continue. No palette witness or public widget
gate changes are required.

Proposed exclusive pair:

- Existing `source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp`.
- NEW `source/apps/moproj2mst/tests/mdt/test_simcontrol300_remesh_hex_prefix.cpp`.

No header/CMake/build/GUI. New tests should prove both zero signs, uppercase/
lowercase prefixes, bare/hex-looking/non-hex suffixes, huge suffix exponents,
preserved nonzero errno, range-errno carry from preceding integer overflow,
later fields/scaling1 and actual Session Open. Preserve nonfinite and other
range/mantissa refusals. The current strict ASCII/size guards must still reject
unqualified bytes even if a line begins with this prefix.

Four prior root-owned fixtures currently expect hex refusal and need a separate
amendment after release: original remesh guard list; CP98 late-real prefix case;
CP99 other-real guard list; CP100 other-real guard list. Replace that input with
a still-guarded NaN or fractional huge exponent, or remove its obsolete list
element. Do not erase the retained prefix/no-enabled assertions. This worker
should not edit those files without explicit ownership.

## Native byte receipt

Imported original EXE SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Verified DUMPBIN14.44.35228.0 SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Preferred-VA half-open ranges:

| Range | SHA-256 |
|---|---|
| `1414870b2..141487106` | `6dffed7f8b47b29157b6932beaa4967214e2c8445af239e1eb12eb1b0550aa09` |
| `141486fe8..141487012` | `3bd61d9e9fcacfd433864a18e0531b9db86b8b3c011a1f8a6a56e9fede556c19` |
| `1414872d2..14148730b` | `b57c9a3573a699132849ac03a89e1ce697401eaef8e92971e2e68788c5d7283d` |
| `14148773a..141487746` | `8a619d3d4b7bdf9486cc05c931c0a9c906e06d735d888daf87bcccd9ae403a26` |
| `141486d1c..141486d78` | `220d609d8f9c41629a4f3f4331327325023203f2e5a64e83af7512f87fc5afed` |
| `14147592a..141475985` | `1c282cdd4ec4fda664a6784ca4a46a207976a65aaabd8b240c94ce3f270ce331` |

Full caller/lexer and signed-zero converter/config receipts remain in
`resume99-remesh-real-overflow-contract.md` and
`resume100-remesh-real-underflow-contract.md`. No new raw artifact was written.
This is one existing guarded prefix class, not general CRT real conversion or
editor300 constructor completion.
