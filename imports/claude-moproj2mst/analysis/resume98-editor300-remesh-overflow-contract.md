# CP98 public remesh integer overflow: source-qualified continuation

The public remesh reader has a source-closed decimal integer overflow branch
independent of the selected editor palette. Original kind5 conversion saturates
to signed32 MIN/MAX and sets errno=0x22; it does not refuse. Remesh then clamps
method into 0..5 or converts the result to a boolean, continues later fields,
closes its input and returns success. Current `originalInt()` instead returns
nullopt on overflow, making actual Session project Open stop prematurely.

Only this evidence report was written at this point. No source/test edits,
build/test execution, GUI/original execution or mutable REA. Bounded static
DUMPBIN/PE reading inspected the imported EXE; the original was not executed.
Root approved a source/new-test successor but requested a CP97 write freeze,
which remains in force until released.

## New evidence beyond CP88

CP88 correctly left remesh numeric extensions pending CRT qualification. CP98
resolved the actual integer route inside the original EXE, rather than using
host atoi behavior or applying unrelated QTextStream/BOM recovery:

| Edge | Native effect |
|---|---|
| remesh `1400ab323` | Calls token reader `1409068f0` with kind5 and shared local destination |
| `140906c0b..6c33` | Index kind-1 into jump table RVA906e00; kind5 entry RVA906e10 contains906cbd |
| `140906cbd..6cd2` | Pass complete current token to `1414737ac`, store returned EAX to destination |
| `1414737ac..37b2` | endptr=null, base10; jump to signed wrapper `141475c44` |
| `141475c44..5c6a` | Signed flags0, selected CRT locale, call conversion core `1414759c0` |
| `141475b11..5b90` | Accumulate decimal digits; track unsigned overflow and sign flags |
| `141475b92..5bb8` | Enforce signed limits: positive<=7fffffff, negative magnitude<=80000000 |
| `141475bba..5bbf` | On range overflow obtain errno pointer through141474134 and store0x22 |
| `141475bd0..5bdb` | Select signed saturation magnitude7fffffff or80000000 |
| `141475be6..5c01` | Apply negative sign and return saturated EAX |
| `1400ab328..ab393` | Store returned method, clamp negative to0 or >5 to5; continue loop |

Exact strings `2147483648` and `-2147483649` therefore return2147483647 and
-2147483648. Exact in-range -2147483648 is accepted without a new range error.
The signed wrapper's flags distinguish it from adjacent unsigned conversion;
there is no modulo-low-word interpretation for this kind5 path. For very large
decimal prefixes the overflow flag also leads to the same signed saturation.
There is no endptr consumer, and the token reader already advances over the
whole token before conversion, so remaining digits do not become later fields.

The errno getter `141474134..4153` returns current CRT thread-data+10, falling
back to static143b5e2c8 if thread data is absent. The incremental candidate
contract preserves ERANGE=34 through the existing C-runtime errno adapter; it
does not expand TLS/locale allocation-failure behavior. Existing ordinary ASCII/
KnownC/token-size/stream ownership restrictions stay in force. No floating
hex/nonfinite/range branch is qualified by this integer proof.

## Public owner and minimal implementation

`moproj/read.cpp` calls actual `Session::loadProjectRemesh()` after XML read and
current-directory selection, before editor/page construction. The Session's
one RemeshState is the mutation destination. This is already a public caller,
not a new isolated helper or symbol-only registration.

Exclusive source set after root's freeze release:

- Existing `source/apps/moproj2mst/src/engine/simcontrol300_remesh.cpp`: change
  only decimal integer overflow from nullopt to signed saturation plus errno34.
  Keep whitespace/sign/prefix parsing, no-digit0 and ordinary success behavior.
  Do not clear prior errno on in-range conversion. Existing method clamp,
  boolean conversion, shared last-int reuse and later record loop stay intact.
- NEW `source/apps/moproj2mst/tests/mdt/test_simcontrol300_remesh_overflow.cpp`:
  dedicated Testkit cases through read/loadRemesh and actual Session Open.

No header/CMake/shared owner changes or worker build. Root will amend the stale
existing test that currently expects integer-overflow refusal; this worker will
not edit that old test file. Its real hex/INF/range and locale refusals remain.

Focused cases: both signed overflows give method5/0 and completed load/enabled;
large prefixes/signs/trailing text saturate; in-range endpoints do not set a new
range error; boolean conversion remains true for either saturated sign; missing
following integer reuses the saturated shared local; later finite scalar fields
still apply. Capture errno at the memory file close boundary for integer-only
cases; later real conversion intentionally has its existing errno behavior.
An actual minimal disk Project Open must consume an overflowing fallback file
and retain method5/0 plus a later field in its actual Session-owned RemeshState.
Keep earlier partial effects if a later still-unsupported real token is reached.

## Static receipt

Original `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
Verified DUMPBIN14.44.35228.0 SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.
Preferred-VA half-open byte receipts:

| Range | Bytes | SHA-256 |
|---|---:|---|
| `140906e10..140906e14` |4| `b6a2dabf9aebe63e803e3179fa462f00d959570b0a54ff0b7b6dd7bc97932a96` |
| `140906cbd..140906cda` |29| `89df24e96ccf55e1cbc9950efdfe86bef45c2e78e9781925830d1648c2f027cf` |
| `1414737ac..1414737b7` |11| `063981f545db857689bf874438c79672d79e2876797be335f9c0917d9ca9fe85` |
| `141475c44..141475c74` |48| `629a19022f00de3f87d45e062fdc5b6ceeb4115cbe5d8280730d08526b585aa5` |
| `1414759c0..141475c42` |642| `f120a6e2141b2a448c558e2925a9e4277deb58e94add5d82f259ab9903dd0192` |
| `1400ab328..1400ab395` |109| `d8a8ddbdfb13d2be07c9db3f3a5520f86f737971cb22dfafcdd326977266a30b` |

The retained reader/token captures remain
`analysis/wave2-save-diffs-300-remesh-reader.asm` and
`analysis/wave2-save-diffs-300-remesh-token-dispatch.asm`. New bounded views
were inspected without adding a separate raw file. The old CP88 numeric
unknown is now closed for this signed integer branch only; selected palette,
page Fill, floating conversions, special namespaces and allocation failures
remain separate dependencies.
