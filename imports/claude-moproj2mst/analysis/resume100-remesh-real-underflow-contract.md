# CP100 canonical real underflow contract

The public remesh reader has a source-qualified counterpart to CP99 overflow:
after existing D/d normalization, complete `[+-]?[1-9][eE]-[0-9]+` tokens with
exponent magnitude>5200 return POSITIVE zero and errno34. Even a negative
mantissa loses the sign in the outer native underflow wrapper. Do not reuse
the host strtod negative-zero result or broaden general underflow parsing.

Only this evidence report was written. No source/test edits, build/test,
GUI/original execution or mutable REA. Source ownership remains conditional on
root's CP99 archive release and approval of the exact pair below.

## Native chain and sign distinction

Reuse CP99's actual kind10 route, signed mantissa/exponent lexer, fixed nonzero
single-digit normalization offset0 and magnitude cap5201. Exponent minus at
141487241..7246 sets the exponent sign local to -1. After parsing the magnitude,
141487336..733c negates EBP. For the selected class EBP becomes -5201; the
strict compare/JL at141487365..736b reaches141487714.

That branch zeros the extended significand/exponent and returns lexer flag1.
Final sign insertion at141487755..776e can still put the mantissa sign in the
extended zero. The conversion core's zero path (141491934..19aa and packing
141491ecf..1f23) produces a signed double zero; configuration at143b5fae8 is64
(on-disk bytes40000000), selecting its double-width store.
No nonzero power/interpolation result is needed for this class.

`141486ccc` maps lexer's bit1 to result flag0x100 at141486d52..6d61. The outer
wrapper's overflow test does not select that flag. At14147595c it tests bit8,
loads the zero payload, and XORPD-clears XMM6 to +0 at141475967. UCOMISD finds
either signed payload equal to zero, so it calls the same errno accessor as
CP98, writes0x22 and returns that cleared XMM6. The path does NOT reload the
negative payload from141475980. Thus `-1e-9999` returns bits0000000000000000,
not8000000000000000, in this normal CRT/locale/TLS value profile.

CP99 overflow remains distinct: positive exponent>5200 still returns signed
infinity. A zero mantissa, fractional/multidigit significand, exponent5200,
nearer double underflow, denormal output, trailing text and special spellings
are not newly admitted. The existing ordinary finite parser still handles its
old finite cases; an unmatched canonical recognizer is not a blanket refusal
of every syntactically unusual token.

## Public behavior and exact source/test proposal

The existing actual Session/project Open remesh call can now store +0 to
FILTER_DISTANCE and continue later settings/loaded/enabled instead of refusing
ERANGE. The source scaling comparison maps that zero to its lower fallback1,
regardless of mantissa sign. No selected palette/getter/count input is involved.

Proposed exclusive pair after root release:

- Existing `source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp`: extend
  the private canonical recognizer to distinguish exponent minus, preferably
  rename it `canonicalRealRange`. Preserve CP99's entire positive class. For
  the newly proved negative-exponent class, cap magnitude at5201, require the
  complete digit suffix and magnitude>5200, then set ERANGE and return literal
  +0.0 before host conversion. Do not derive its sign from the mantissa.
- NEW `source/apps/moproj2mst/tests/mdt/test_simcontrol300_remesh_real_underflow.cpp`:
  dedicated Testkit cases for exact +0 bits and errno34 for both mantissa signs,
  D/d markers, leading exponent zeros and long magnitudes; later fields/scaling1;
  retained unqualified range/spelling guards; actual Session Open; and positive
  overflow preservation. Use the existing scratch/current-directory pattern.

No header/CMake/shared runtime/page/selected palette changes. Root or a separate
owner must amend CP99's two old underflow guard tokens in
`test_simcontrol300_remesh_real_overflow.cpp`: replace1e-9999/-1e-9999 with
still-guarded fractional1.5e-9999/-1.5e-9999. Worker should not write that prior
test without explicit ownership. CP98 hex and integer cases stay unchanged.

Tests should distinguish a nonzero single-digit underflow (new +0 withERANGE)
from ordinary zero-mantissa signed zero, whose existing parser path remains.
Do not claim FP environment/temporary architecture equivalence beyond this
value/errno adapter or execute original code to establish the result.

## Retained PE receipt

Imported original EXE SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Verified DUMPBIN14.44.35228.0 SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Preferred-VA half-open byte ranges:

| Range | SHA-256 |
|---|---|
| `141487336..141487371` | `63d5d0481e2cb5dd40084377ea52104f3ebc23fd4ba0ba1656f6d2f17f603c9c` |
| `141487714..141487724` | `d7f7d6d6d7c60a71277994d815e855698ea0c4d0f152f39ab53bec82f8ae4feb` |
| `141486d52..141486d61` | `1f0018cd0b68ca9f64a7b87ef8c811f9d5381e0f3d6190c46d9d643aee816e2d` |
| `14147595c..14147599c` | `7b0a25e8d1a649c04929c0688d3593a1377f113298253546a54e76298a1ed91b` |
| `141491934..1414919aa` | `116949b33eb66debffd02494b8ff8dee5605c11de744d7704e5712a5868965cc` |
| `141491ecf..141491f23` | `3cb062c0b8d07cd567356bcf3b1721f91017401525795fbd4672d4631faa8c00` |

Full lexer/fltin/outer-wrapper and callsite receipts are retained in
`analysis/resume99-remesh-real-overflow-contract.md`, SHA-256
`0b8e94a08ef219bc7a2e9571f4d1c7ce22eb2bdc5e6da1f14f6f39eb0848a9cb`.
No separate raw artifact was written. General real conversion and editor300
construction remain incomplete after this proposed narrow public branch.
