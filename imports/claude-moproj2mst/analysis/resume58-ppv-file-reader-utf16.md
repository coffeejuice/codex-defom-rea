# Resume 58: PPV file reader UTF-16LE and CRLF increment

## Change

Expanded the isolated `readPpvFileLines` reader to accept:

- BOM-free ASCII, decoded byte-for-byte into UTF-16 code units;
- UTF-16LE input with the `FF FE` BOM, decoded into UTF-16 code units;
- LF and CRLF line endings, with CR removed only when directly before LF.

The implementation rejects other BOM/codecs, odd-length UTF-16 payloads,
unpaired UTF-16 surrogates, unsupported ASCII control/NUL bytes, and any CR not
directly paired with LF. On these cases it returns
`UnsupportedEncodingOrLineEnding` and clears any partial lines. A successful
byte read with unsupported content remains distinct from `OpenFailed`.

The updated Testkit cases cover ASCII LF and CRLF, UTF-16LE BOM with CRLF,
missing input, UTF-8 BOM, odd UTF-16 length, an unpaired high surrogate, and an
embedded CR. The test uses active `REQUIRE`/`CHECK` macros, with no C `assert`.
Only the assigned reader header/source/test and this analysis receipt were
edited. No CMake, public loader, or `readTextLines` change was made.

## Source qualification

`analysis/resume58-ppv-qt-codec.md` qualifies these two newly supported cases
from the retained original `QtCore4_64.dll` (SHA-256
`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`) and the
PPV call path in `analysis/resume48-ppv-reader-receipt.txt`:

- The original PPV reader opens QFile in `ReadOnly`, constructs a default
  QTextStream, and calls `readLine()`.
- Default stream initialization enables Unicode auto-detection. The retained
  codec helper maps initial bytes `FF FE` to MIB `0x3F6` (UTF-16LE), and the
  stream decode path consults that helper.
- `QTextStream::readLine()` recognizes LF preceded by CR and excludes both
  terminator characters from the returned line.

These are static binary qualifications, not runtime observations. The same
evidence leaves malformed/truncated UTF-16 behavior unresolved, so rejecting
those encodings is a deliberate guard rather than a claim about native error
handling. UTF-8 BOM behavior is also outside this increment and stays guarded.
The implementation continues to use project `readFileBytes` (overlay first,
then disk), not a literal QFile object; native open failure mappings beyond
the byte seam are not claimed.

## Validation status

No build or tests were run, per assignment. The root integration owner should
validate the focused test and combined Release candidate. The adapter remains
isolated and is not connected to public `loadSimCtrl`.
