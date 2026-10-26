# CP162 SPRING2 BMP decimal digit category

The distinct increment is the original Qt **BMP decimal-digit category**, decoded from explicit UTF-16/32 BOM companions. It is source-ready for a bounded decimal-entry profile. The retained DLL contains 230 BMP code units for which QChar::isDigit is true, in 23 ten-digit blocks; 220 are non-ASCII. Every block's digitValue sequence is exactly 0 through 9. This is a recovered category, not a proposed list of additional literal tokens or the host's current Unicode classification.

The decisive distinction is radix entry: **U+0660 U+0661 U+0660 produces decimal 10**, whereas replacing those characters with ASCII `010` before the current base-0 reader would produce octal 8. Preserve the original code units until the numeric mode is selected. CP160's consumed-sign failure remains independent.

## Retained target and reproducible category

The original QtCore4_64.dll SHA-256 was rechecked as `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Caller identity remains original DEF_GUI_MO_64.exe SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

`analysis/resume63-qchar-number-class.asm` binds QChar::isDigit at `0x67069E70` to property category byte **4**, using the two-level index at RVA `0x1F4050` and 20-byte records at RVA `0x1FF7B0`. QChar::digitValue at `0x67069F00` reads the packed word at record+4, shifts its low byte left eight, sign-extends the 16-bit result, then arithmetic-shifts right ten. It is not a C-library digit conversion.

`analysis/resume162-spring-unicode-digit-evidence.py` reproduces these original instructions over the retained PE bytes without loading the DLL. It scans all 65,536 BMP units, verifies each recovered ten-unit block maps to 0..9, and emits `analysis/resume162-spring-unicode-digit-evidence.json`. The exact inclusive blocks are:

```text
0030–0039  0660–0669  06F0–06F9  07C0–07C9  0966–096F  09E6–09EF
0A66–0A6F  0AE6–0AEF  0B66–0B6F  0BE6–0BEF  0C66–0C6F  0CE6–0CEF
0D66–0D6F  0E50–0E59  0ED0–0ED9  0F20–0F29  1040–1049  17E0–17E9
1810–1819  1946–194F  19D0–19D9  1B50–1B59  FF10–FF19
```

For each block, `digitValue(unit) = unit - blockStart`. Do not substitute modern Unicode Nd ranges: the original table is the category authority. In particular, U+00B2 has digitValue 2 but category 6, so it fails isDigit; U+2160 has category 5 and digitValue -1. Numeric-looking characters are not interchangeable with this category.

## Byte decoding is closed for the four well-formed BOM profiles

The earlier `resume58-qt-stream-init.asm` zeroes the read conversion state at private+0x40 (`0x670C229F–22B1`) and enables Unicode autodetection at private+0x88. `resume58-qt-stream-decode.asm`, `0x670C2D77–2DA6`, replaces the locale codec using codecForUtfText before decoding; `0x670C2DDF–2E04` passes the initial state to the conversion wrapper. The wrapper `resume59-qt-codec-decode.asm`, `0x67072460–248A`, dispatches through codec vtable slot +0x18.

The previously retained BOM selector `resume58-qt-codec-for-utftext-overload.asm` checks the UTF-32 BOMs before UTF-16, avoiding the FF FE 00 00 overlap. New offline captures close the actual Unicode conversion route:

| BOM bytes | MIB | Codec vtable | Endian field at object+8 | Conversion route |
|---|---|---|---:|---|
| `FE FF` | 0x3F5 | 0x67245A90 | 1 | slot +0x18 -> 0x6714F930 -> 0x6714F1C0 |
| `FF FE` | 0x3F6 | 0x67245AC8 | 2 | same UTF-16 route |
| `00 00 FE FF` | 0x3FA | 0x67245B38 | 1 | slot +0x18 -> 0x6714F9B0 -> 0x6714F570 |
| `FF FE 00 00` | 0x3FB | 0x67245B70 | 2 | same UTF-32 route |

`resume162-spring-codec-ctor-6714c840.asm` binds those vtables and endian fields at `0x6714C99E–C9D8` and `0x6714CA2E–CA68`. The four name/MIB leaf captures bind the table roles to UTF-16BE/LE and UTF-32BE/LE. `resume162-spring-codec-mib-6714d0d0.asm` initializes the builtins at `0x6714D12E` and compares each object's virtual MIB (+0x10) against the request at `0x6714D2E0–D2FD`.

In `resume162-spring-codec-body-6714f1c0.asm`, `0x6714F28C–F2C4` assembles a UTF-16 unit as low-byte + high-byte*256 for endian 2, or first-byte*256 + second-byte for endian 1. `0x6714F2C4–F311` suppresses the initial U+FEFF and stores subsequent units unchanged. Thus FF FE 61 06 and FE FF 06 61 both emit the one QChar U+0661 after the BOM is removed.

In `resume162-spring-codec-body-6714f570.asm`, `0x6714F640–F67F` collects four bytes; `0x6714F6D3–F720` assembles the selected endian value and suppresses initial U+FEFF. `0x6714F722–F763` assembles later values; `0x6714F763–F792` writes a BMP scalar below 0x10000 as one UTF-16 code unit. Thus FF FE 00 00 61 06 00 00 and 00 00 FE FF 00 00 06 61 both emit U+0661. Supplementary values follow a different surrogate-pair branch and are outside this category.

These are immutable PE deductions, not original codec execution. Only complete units, BMP non-surrogate scalars and an initial matching BOM are proposed. Truncated units, invalid UTF-32 values, repeated/embedded BOMs, surrogate handling, UTF-8 non-ASCII decoding and BOM-less ACP interpretation remain guarded. No startup-codec witness is needed for an explicit recognized BOM.

## Numeric mode, cursor, status and count

The PE-checked original integer evidence remains `wave2-save-diffs-qt-textstream-getinteger-complete.asm` and `wave2-save-diffs-qt-textstream-read-int-complete.asm`, qualified by `resume83-sw2sp-parser-independent-review.md`.

At `0x670C5371`, automatic base selection tests **literal ASCII U+0030**, not digitValue==0. A first non-ASCII decimal digit falls through `0x670C545F–54D0`: it is neither an ASCII C-locale sign nor ASCII zero, but isDigit succeeds and selects decimal. The decimal loop at `0x670C5669–5704` uses isDigit/digitValue for the first and subsequent units. Mixed digit blocks therefore work in decimal. A recognized ASCII sign also selects decimal, independently of digit spelling.

For the following table, offsets are logical UTF-16 code-unit positions after the BOM has been removed. They are not the QIODevice's physical position: QTextStream can read ahead. For the proposed BMP-only profile, an equivalent raw-file boundary is BOM length + offset*2 for UTF-16, or BOM length + offset*4 for UTF-32.

| Decoded units | Rows | Flags | Cursor/status deduction |
|---|---:|---|---|
| U+0661, EOF | 1 or 2 | `[1]` or `[1,0]` | Successful extraction to offset 1/EOF, fresh status 0. For two rows the last slot stays pre-zeroed; no second extraction. |
| U+0660 U+0661 U+0660 CR LF | 1 | `[10]` | Decimal, successful; CR ungot at offset 3, status 0; row cap stops. |
| U+0660 U+0661 U+0660, EOF | 2 | `[10,0]` | First succeeds at offset 3/EOF, status 0; no second extraction. |
| U+0661 U+FF12 CR LF | 1 | `[12]` | Mixed-block decimal; CR unread at offset 2, status 0. |
| U+0661 SPACE `-` U+0968 CR LF | 2 | `[1,-2]` | First leaves SPACE at offset 1; second skips SPACE, consumes ASCII minus and Unicode digit 2, leaves CR at offset 4. Both successful; status 0. |
| U+0661 `q`, EOF | 2 | `[1,0]` | Original decimal prefix leaves q at offset 1/status 0; next getter fails without advancing and writes zero/status 2. This malformed-suffix composition need not be admitted by this increment. |

The SPRING2 helper `wave2-save-diffs-300-support-1410da690.c` increments the requested-row index after every attempted extraction, ignores status, and returns true on successful open. Its `atEnd()` guard uses unread input, as established in `resume120-spring2-malformed-flags.md`. With CRLF still unread, a further requested extraction can consume only whitespace and fail at EOF/status 1; the resulting zero must not be confused with an EOF guard that skipped extraction. The source-only value projection may leave both externally equal.

## Proposed admission and test contract

Keep this local to the **SPRING2 companion**, selected by the existing supported one/two-row SPRING primary profiles. Keep primary counts/row fields, SW2SP, B_WEIGHT, other sidecars and public Save/ABI gates unchanged. `NativeExtra3DIo::readSprings` is the existing private acceptance seam; success there does not establish full-project public Save parity.

Decode explicit UTF-16LE/BE or UTF-32LE/BE into preserved BMP code units. Permit existing ASCII units and the 220 recovered non-ASCII Nd units; keep non-ASCII whitespace and other Unicode categories excluded. Enter the new bounded decimal route only where the original does: an unsigned first digit that is non-ASCII Nd or ASCII 1..9, or an ASCII sign followed by Nd digits. Require complete digit runs with existing ASCII whitespace/EOF termination and an int32 result for the initial increment. Mixed scripts and non-ASCII leading zeroes are included because they use the same recovered decimal loop. Unsigned multi-unit tokens beginning with ASCII zero stay on their existing separately bounded base-selection routes; do not normalize them into this route. Existing overflow and malformed-marker families remain separate gates.

This is a deliberate category/profile boundary, not original-format validation. Do not implement it by replacing all Unicode digits with ASCII before the base-0 helper. Either preserve code units through mode selection or explicitly parse the new profile as decimal after proving that entry branch.

Recommended focused tests:

- Exhaust all 220 non-ASCII category members under all four BOMs as one-unit EOF companions, checking the exact digit flag and byte-for-byte preservation of both files. Include count-two EOF zero fill on representatives from every block. This verifies the recovered category rather than a few named scripts.
- Under all four BOMs, assert U+0660 U+0661 U+0660 -> 10, mixed U+0661 U+FF12 -> 12, and the two-flag U+0661 SPACE minus U+0968 -> `[1,-2]`. The receipt retains exact encoded byte strings and SHA-256 for these 12 fixtures. Also retain ASCII `010` -> 8 as the decisive non-normalization control.
- Exercise ASCII-sign +/minus with non-ASCII zero and nonzero Nd, EOF and tab/CRLF boundaries, and mixed ASCII/Unicode decimal digits after an unambiguous decimal start. Keep CP160 zero-digit sign tests unchanged.
- Refuse BOM-less high bytes, UTF-8 BOM plus non-ASCII payload, incomplete UTF-16/32 units, U+00B2 despite its digitValue 2, U+2160, U+2212, surrogate units, supplementary decimal digits and an unsupported count-three primary. Preserve both files on refusals. These are candidate exclusions, not claims that original Qt rejects all excluded profiles.
- Keep initial ASCII-zero + Unicode digit, overflow, malformed attached suffix and non-ASCII whitespace combinations refused unless separately admitted from their original branch. Do not accidentally broaden these through a general decoder change.

At inspection, production reader SHA-256 was `26CFABD97925362007D8FEC1EFBFFF6880C975E765B8060F123D3B51B014DE96`; test SHA-256 was `55C52C1F64672654FA00ED36C86BAADBE76020CECFBABE9246925D349056BF4B`. These are read snapshots, not a CP161 freeze. Current `asciiBytes` rejects all decoded units >0x7F; it must not be broadened globally to implement this companion-only profile. No source or test was edited.

## Evidence hashes and validation boundary

Evidence JSON SHA-256: `310778828c17bbd74939f0ac54f6beb9dba3694f315aea5b926343c8bd466faa`.
Extraction/verification script SHA-256: `514268853b5c3e2b7fc4c4fe2bad4b4d4909f6f38554ed6ac7a69db57e12c90a`.
The ordered category membership bytes (`<uint16 codeUnit,uint8 digitValue>`, little-endian) SHA-256 are `b1fd7aea2c6f249f6a2cc2140442a76c4beb5a9b4c817055c2de79a37fc2a095`.

| Original raw PE region | SHA-256 |
|---|---|
| isDigit RVA 0x69E70–69EAA exclusive | `c20ee47cd2d4508d79e4b7a509405f0358cf5770b13c165d9bc09e371bc4db2c` |
| digitValue RVA 0x69F00–69F3F exclusive | `57ebc867504d56727b3026d20ce90ba4f4cb3a82a127df2d666d4af874646336` |
| BMP property-index envelope RVA 0x1F4050–1FF7B0 exclusive | `9d11e8674a46b37cf7ba1c2ee20371710bd4b83f63bd7ec54d8177a937550664` |
| BMP property-record envelope RVA 0x1FF7B0–203978 exclusive | `fd34cc7f21df9c41daedcc5497f3f39cc4f31aa166db037c09fcc9a66af71026` |
| UTF-16 decoder RVA 0x14F1C0–14F389 exclusive | `09560c73c29beaa394852a2637637f855ee43788f687dcf09f8ea67db5b6ed38` |
| UTF-32 decoder RVA 0x14F570–14F804 exclusive | `9e6980d3b8cc4653645b39015965ceae05cd9b55bf1ee7548a144da48bc0eb5f` |

Fourteen new bounded DUMPBIN captures and their per-file hashes are retained in the evidence JSON and `resume162-spring-codec[-body/-ctor/-mib]-static.json` receipts. The verification script compared 1,137 address-prefixed instruction records (3,871 emitted prefix bytes) against the PE, with zero mismatches; continuation-line bytes are not counted in that prefix check. Raw region hashes bind the complete decoder bodies. No absent mutable engine state was inferred.

No immutable witness is missing for the proposed well-formed four-BOM BMP/decimal profile. Original-runtime differential corroboration and public lifecycle acceptance remain unperformed. This task used offline reads and diagnostic captures only; it did not execute the original DLL/app, control a GUI, access live REA/mutable databases, edit CP161 source, build or run CTest.
