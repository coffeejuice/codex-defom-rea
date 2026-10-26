# Checkpoint 58: PPV `QTextStream` codec and line handling

## Result

The retained original QtCore 4.8 binary proves both behaviors in this PPV read path:

- The default `QTextStream(QIODevice*)` state enables Unicode auto-detection. Its first read of bytes calls `QTextCodec::codecForUtfText`; that helper recognizes the UTF-16LE BOM `FF FE` and returns the codec resolved from MIB value `0x3F6` (decimal 1014, the UTF-16LE MIBenum). The BOM is therefore consumed through codec selection before decoding. This establishes UTF-16LE BOM detection for the original reader.
- `QTextStream::readLine()` handles CRLF as a line ending. In its line-mode-2 path, it recognizes LF preceded by CR and advances past the CR as part of ending the line. The CR from a CRLF terminator is not present in the returned line.

These are static conclusions from the retained original DLL, not runtime observations. No GUI, REA session, source edits, or fixture mutation was used.

## Qualified PPV call path

`analysis/resume48-ppv-reader-receipt.txt` records original GUI-MO `FUN_140d4b400` at `0x140d4b400` (decompile source SHA-256 `cb28de6852d5b6279719d2576759a9dcc0f640e436bcdf846caa691622c58698`). The function constructs `QFile` from a `QString`, opens it with mode `1` (`ReadOnly`), constructs `QTextStream(QIODevice*)` without setting a codec or disabling autodetection, and repeatedly calls `readLine()`.

The QtCore image used below is `evidence/original-binaries/QtCore4_64.dll`, SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

## BOM qualification

`resume58-qt-stream-init.asm` shows private stream initialization writing `1` to the byte at private-state offset `0x88` (`0x670C230F` onward). The stream reader at `0x670C2C40` tests this field at `0x670C2D77`; when enabled, it clears the flag and passes the initial byte buffer through `QTextCodec::codecForUtfText` at `0x6714E050` (`resume58-qt-stream-decode.asm`, `0x670C2D94–0x670C2DA1`). The resulting codec pointer replaces the stream codec.

The bounded helper capture `resume58-qt-codec-for-utftext-overload.asm` directly compares the initial bytes. At `0x6714E0AA–0x6714E0D6`, the two-byte sequence `FE FF` maps to MIB `0x3F5`; at `0x6714E0C6–0x6714E0D6`, `FF FE` maps to MIB `0x3F6`. The helper resolves these values through `0x6714D0D0` and returns that codec. This branch is reached by the stream decoder above, so UTF-16LE BOM selection is not inferred solely from the auto-detect flag.

The captured code also checks UTF-32 and UTF-8 BOMs, but those are outside this task’s PPV claim. The selected codec then decodes bytes through the stream’s normal codec path. The binary evidence does not establish how malformed or truncated UTF-16 input is handled.

## CR qualification

`resume58-qt-readline.asm` shows the exported `readLine` wrapper passing mode `2` to its internal line reader (`0x670C516A–0x670C5182`). In `resume58-qt-readline-inner.asm`, that mode compares each decoded character with LF (`0x670C31A8`) and checks the preceding character for CR (`0x670C31AE`). It therefore treats CRLF together as a line ending and excludes the CR from the returned line. The separate stream decoder capture is not used to claim how embedded CR characters are preserved.

The original `QFile::open` call is read-only and does not set `QIODevice::Text`. The observed CR handling belongs to `QTextStream::readLine` and its decoding path.

## Evidence and limits

New bounded DUMPBIN captures in this checkpoint:

- `resume58-qt-stream-init.asm`
- `resume58-qt-stream-decode.asm`
- `resume58-qt-codec-for-utftext-overload.asm`
- `resume58-qt-readline.asm`
- `resume58-qt-readline-inner.asm`
- `resume58-qt-codec-for-utftext.asm` (wrapper/context)

The captures use Visual Studio DUMPBIN 14.44.35228.0 against the SHA-256-qualified QtCore file above. Exact virtual-address ranges are encoded in each capture header. The resolution of MIB `1014` as UTF-16LE follows the Qt codec MIB assignment; the binary proves the BOM byte test and numeric MIB selection directly. There is no runtime confirmation for the particular PPV file, locale, or malformed-input cases.

