# Checkpoint 59: PPV text decoding beyond UTF-16LE BOM

## Qualified result

The original PPV reader uses `QFile::open(ReadOnly)` followed by an unconfigured default `QTextStream` and `readLine()`; see `analysis/resume48-ppv-reader-receipt.txt` and the Checkpoint 58 call-path qualification in `analysis/resume58-ppv-qt-codec.md`. For the SHA-256-qualified original QtCore DLL (`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`), static evidence establishes:

1. If the input begins with UTF-8 BOM `EF BB BF`, QTextStream's active BOM helper resolves codec MIB `0x6A` (106, UTF-8). If it begins with no recognized BOM, that helper returns the current codec unchanged. The default stream constructor has already called `QTextCodec::codecForLocale()` and stored that codec before reading begins. Thus BOM-less UTF-8 bytes are **not** selected as UTF-8 by the BOM detector; their decoding follows the locale codec. `ANSI` here must not be treated as a proven specific Windows code page: the original process's effective codec/code page is not established by this static call path.
2. The stream decoder dispatches the chosen codec's virtual byte-to-Unicode conversion routine. The static generic path does not identify the concrete locale codec nor establish how that codec treats malformed/truncated byte sequences. No precise replacement, omission, or error result is qualified here.
3. Embedded CR behavior is qualified for this QFile path. `ReadOnly` is the only mode passed to `QFile::open`; the QIODevice `Text` flag is not set. `QIODevice::isTextModeEnabled()` reads the Text bit, so the stream decoder's separate Text-mode CR removal path is inactive. `readLine()` mode 2 treats LF preceded by CR as a line ending and omits that CR. A CR not serving as the CRLF line ending is processed as an ordinary character and is retained in the returned line.

These are static findings; no GUI, REA session, app run, source change, or mutable fixture was used.

## Evidence chain

- `analysis/resume48-ppv-reader-receipt.txt`: original `FUN_140d4b400` constructs QFile from QString, opens with mode `1`, constructs default `QTextStream(QIODevice*)`, and invokes `readLine()`.
- `analysis/resume58-qt-stream-init.asm`: default stream initialization calls `QTextCodec::codecForLocale()` at `0x670C2285` and stores its result at private offset `0x38` (`0x670C228A–0x670C2291`); it also sets auto-detection at private offset `0x88` to true.
- `analysis/resume58-qt-stream-decode.asm` (`0x670C2D77–0x670C2DD3`): on first read, auto-detection is cleared and the initial byte buffer is passed to `QTextCodec::codecForUtfText(QByteArray, currentCodec)` at `0x6714E050`; its result becomes the active codec. When no BOM matches, the helper returns the existing locale codec unchanged. A null-codec fallback calls `QTextCodec::codecForLocale()` at `0x6714DF70` (`0x670C2DC6`), but the normal default constructor already supplied a codec.
- `analysis/resume58-qt-codec-for-utftext-overload.asm`: `EF BB BF` selects MIB `0x6A` at `0x6714E0E1–0x6714E0F7`; if no BOM condition matches, the helper returns its second argument (`rdx`) at `0x6714E0FC–0x6714E0FF`. This establishes that BOM-less bytes do not select UTF-8 through this helper.
- `analysis/resume59-qt-codec-for-locale.asm`: the locale codec routine returns a cached codec, initializing it via helper `0x6714BB40` when necessary. The static capture does not show a stable literal codec name or process ACP value. Runtime locale/code-page qualification is still required before claiming which ANSI codec was active in the original process.
- `analysis/resume59-qt-codec-decode.asm`: stream decoding dispatches through a codec virtual method (`[r10+0x18]` at `0x6707247E`). This qualifies codec-dependent decoding, not malformed-byte semantics for an unknown codec.
- `analysis/resume59-qt-iodevice-mode.asm`: `QIODevice::isTextModeEnabled()` reads bit 4 of its open-mode state. With original QFile mode `1`, Text is false. `analysis/resume58-qt-stream-decode.asm` gates its CR filtering pass on that flag (`0x670C2E2F–0x670C2E53`).
- `analysis/resume58-qt-readline-inner.asm`: in readLine mode `2`, LF is recognized at `0x670C31A8`; the immediately preceding CR is recognized at `0x670C31AE` and excluded from the result. A non-LF character follows the ordinary path, so an embedded CR is not stripped by this line-ending logic.

All `resume59-qt-*.asm` captures are bounded DUMPBIN views of the same SHA-256-qualified QtCore image. `resume59-qt-locale-imports.txt` is only a filtered import listing; it is not used to infer which Windows API or code page was used for this process.

## Remaining runtime qualification

To decide whether BOM-less PPV text was decoded as CP1252, another ANSI page, or a UTF-8 locale codec, observe the original process's active QTextCodec/Windows ACP at the read. A generic statement that it uses `codecForLocale()` is supported; a specific code page is not. To characterize malformed-byte output, test malformed sequences against the exact codec selected in that same process, or source-qualify that codec's conversion implementation and flags. Do not use a candidate host's locale as a proxy for the original process.

