# CP212 independent CLI decoder recovery

The recorded `-f` argument is not a literal filename interface. Main decodes it as hexadecimal Unicode code units before calling the outer loader. The CP212 trial supplied the literal path, so the recorded acceptance failure is not evidence that the original rejected the intended `.moproj` filename. The literal also violates this decoder's group-length assumption; its exact runtime QString must remain unverified.

## Artifact and method

Read-only static examination of copied `evidence/original-binaries/DEF_GUI_MO_64.exe`, original DEFORM v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. This is the same identity recorded by the original CP212 trial. Microsoft DUMPBIN reports version 14.44.35228.0; installed tool directory is MSVC 14.44.35207. No original executable, GUI process, converter, build or test project was executed or changed. No REA session was acquired. Only this report and its three sibling disassembly files were written. Existing recovered source/notes were not treated as the oracle.

Raw bounded proof:

- `resume212-cli-decoder-independent-main.asm`: VA `140001bf2..140001cd4`.
- `resume212-cli-decoder-independent-helper.asm`: VA `1400011a0..140001404`.
- `resume212-cli-decoder-independent-scan.asm`: VA `14146df30..14146e009`.

All following instruction locations use RVA relative to image base `140000000`.

## Main and helper contract

Main constructs the option name from the ASCII string at RVA `1e15224`, whose bytes are `66 00` (`f`), calls `2c00` to obtain that option value, and passes its QString to `11a0` at call RVA `1c4d`. The helper result lives at main stack offset `1918`. Main calls QString::length at `1c78`, and only a positive decoded length enters the outer loader (`1c8f`, continuation `1c94`). This establishes that the loader receives the helper's result, not the original argument.

The copied PE import table independently binds these IAT entries:

| RVA | Imported QtCore4_64.dll operation |
|---|---|
| `1e04490` | `QString::operator[](int) const`, returning QChar |
| `1e04498` | `QChar::toLatin1() const` |
| `1e044a0` | `QChar::QChar(int)` |
| `1e044a8` | `QString::setUnicode(QChar const*, int)` |
| `1e044b0` | `QString::length() const` |
| `1e04488` | `QChar::QChar()` |
| `1e044c0` | `QString::QString()` |
| `1e059d8` | `QString::QString(QString const&)` |

`11a0` takes hidden QString-result storage in RCX and a pointer to input QString in RDX. It reads input length `n` at `11e2`. Empty input returns an empty constructed QString at `11f3..1222`. For nonempty input it allocates `2 * floor(n/4)` bytes (`1227..1253`), default-constructs that many QChars, then iterates while `i*4 < n` (`12c7..12d2`). Each iteration:

1. Reads input QChars `4*i + j`, for `j=0..3` (`12ed..132a`).
2. Converts each with `QChar::toLatin1()` and puts the low byte in a four-byte stack buffer (`130d..1325`).
3. Appends a NUL byte at buffer offset 4 (`132c`).
4. Passes the buffer, literal ASCII `%x` at RVA `1e151c4`, and the address of stack dword `54` to helper `146dfd4` (`1331..1342`).
5. Loads that dword without checking the scanner's return, constructs QChar(int), and stores its 16-bit value in output slot `i` (`1347..1365`).

At `137b..1395`, it gives QString::setUnicode the output pointer and exactly `floor(n/4)` as length. It deletes the temporary array, copies the resulting QString to result storage, and returns.

`146dfd4` is a varargs string-scanner wrapper: it homes the first vararg, passes buffer and format plus the vararg list and null locale through `146df30`, and selects scanner core `1480128`. `146df30` computes buffer length, constructs a string-backed stream descriptor (flags `49h`), and calls the core. The `%x` literal and output address establish scanf-family hexadecimal integer conversion; `%x` is not a strict four-digit validator. The native scanner core was not exhaustively recovered here, so exotic locale/input corner cases remain outside this report.

## Encoding and malformed inputs

The canonical safe input language is four ASCII hexadecimal digits per desired 16-bit code unit, concatenated with no separators, prefix, BOM or terminator. Digits spell the numeric code-unit value most significant nibble first: `0043` means U+0043 (`C`). This is not the hexadecimal spelling of UTF-16LE bytes (`4300` would request U+4300). The temporary QChar array is little-endian in Windows memory; that storage fact does not reverse the textual digits.

Non-BMP characters require two encoded surrogate units in UTF-16 order, for example `d83dde00` for U+1F600. The decoder does not combine or validate them itself; it constructs QChars and calls setUnicode. The adjacent encoder at `10a0` reads signed 16-bit words and formats `%04x` (literal RVA `20edb90`); sign extension can produce eight digits for units >= 8000. Thus this report does not claim that adjacent encoder round-trips all Unicode. The requested ASCII path is unaffected.

Malformed groups do not have a clean rejection path. Scanner assignment count is ignored, and the destination dword at stack offset `54` is not initialized before the first scan. A failed conversion can therefore retain a preceding successful value, or use an indeterminate prior stack value if no assignment succeeded. Prefix conversion, whitespace, optional signs and the usual hexadecimal prefix can also be accepted within a four-byte group; they are not canonical syntax.

For `n % 4 != 0`, allocation and final QString length use `floor(n/4)`, but the decode loop executes `ceil(n/4)` iterations. The last iteration requests QChars beyond the input's logical end and stores beyond the nominal allocated QChar array. Imported Qt bounds behavior and allocator effects were not examined; this is unsafe malformed-input behavior, not a defined truncation rule. No exact resulting string or universal crash claim follows.

## CP212 literal argument comparison

The retained `summary.json` in `resume210-original-outer-loader-frida-2026-10-26T12-41-01-511Z` records the exact argv suffix:

```text
-m open -f C:\PROBLEM\p202a\Gear_blank.moproj
```

Its `pathQString = 0x7df8e8` is only an address. The recorded `AL=0` acceptance result remains an observation of this fixed launch; decoded path content was not captured.

The literal has 34 QString characters, giving nominal output length 8 but nine loop iterations. Its complete groups and conventional `%x` predictions are:

| Group | Scan outcome | Stored low 16 bits |
|---|---|---|
| `C:\P` | successful hex prefix C | `000c` |
| `ROBL` | no assignment; retain preceding value | `000c` |
| `EM\p` | successful hex prefix E | `000e` |
| `202a` | successful full group | `202a` |
| `\Gea` | no assignment | `202a` |
| `r_bl` | no assignment | `202a` |
| `ank.` | successful hex prefix a | `000a` |
| `mopr` | no assignment | `000a` |

The remaining `oj` starts the unsafe ninth iteration. The table is a static prediction for complete chunks, not a capture of runtime QString contents. It explains why treating the literal path as the reader's input was unjustified. It does not authorize an original retry or establish that a correctly encoded launch completes Open/Save/Close.

## Canonical argument and test vectors

For `C:\PROBLEM\p202a\Gear_blank.moproj`, use this exact 136-character `-f` value:

```text
0043003a005c00500052004f0042004c0045004d005c00700032003000320061005c0047006500610072005f0062006c0061006e006b002e006d006f00700072006f006a
```

Independent offline Python arithmetic confirmed 34 UTF-16 units, 136 hexadecimal characters, and an exact UTF-16 code-unit round trip. This is a static encoder check, not original runtime verification.

For a separately prepared disposable clone at `C:\PROBLEM\p213a\Gear_blank.moproj`, the corresponding command is below. It is a prepared command only; it was not executed and does not grant runtime authorization:

```text
"C:\Program Files\SFTC\DEFORM\v11.0.2\DEF_GUI_MO_64.exe" -m open -f 0043003a005c00500052004f0042004c0045004d005c00700032003100330061005c0047006500610072005f0062006c0061006e006b002e006d006f00700072006f006a
```

The p213a argument also has 136 hexadecimal characters and decodes to 34 code units. Its argument length modulo four is zero, so the decoder allocates and fills exactly 34 QChars. By comparison, the CP212 literal argument length is 34, modulo four is two, allocation is eight QChars, and the decode loop attempts nine QChar stores. The nominal overrun is one QChar (two bytes), with the last iteration also reading input indexes 34 and 35 beyond the 34-character logical string.

| Input | Predicted result or qualification |
|---|---|
| empty | empty QString; main skips outer loader |
| `0043` | `C` |
| `0043003a005c` | `C:\` |
| `4300` | U+4300, demonstrating no byte swap |
| `00e9` | U+00E9 |
| `d83dde00` | UTF-16 pair D83D DE00 |
| `0000` | one U+0000 code unit; positive QString length |
| `0041zzzz` | A followed by retained A under normal scanner semantics |
| `zzzz` | uninitialized scan destination; value not established |
| `004` / `004100` | incomplete groups; out-of-range accesses/write, result not established |

No candidate source correction is justified by this launch alone. The immediate correction is to the invocation/evidence qualification. Exact original decoding and successful lifecycle behavior for the canonical argument still require separately authorized, qualified runtime observation.
