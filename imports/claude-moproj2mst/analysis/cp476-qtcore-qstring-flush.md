# CP476 Qt QString stream and flush evidence

Static-only inspection of the copied `evidence/original-binaries/QtCore4_64.dll` (SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`). No application launch or runtime observation was used.

## QString insertion

The PE image base is `0x67000000`. The `QTextStream::operator<<(QString const&)` export at RVA `0xC4460` is VA `0x670C4460`; its `.pdata` runtime-function range is `[0x670C4460, 0x670C44A0)` (64 bytes), with raw SHA-256 `0e2c8574dbfe2913f988d39591d4d75ce0391b84988eebb77cc66058839ff4af`.

At `0x670C4481`, the relative call resolves to `0x6701CEE0` on the empty/null branch. At `0x670C4492`, it resolves to `0x670C3530` on the normal branch. The normal callee's `.pdata` range is `[0x670C3530, 0x670C37E5)` (693 bytes), raw SHA-256 `12765f7f7f6c4cc4e3282f64a8d763d7c49038251851d6eaf9fde603980409af`.

The normal helper appends UTF-16 QString content to QTextStream's internal QString buffer, applying field width/alignment/padding state. Its buffer-length check at `0x670C37B2` compares against `0x4000` QChars and may call `0x670C2F50` to flush. The helper does not itself visibly encode to bytes.

## Flush and write

The flush helper at `0x670C2F50` has `.pdata` range `[0x670C2F50, 0x670C30DE)` (398 bytes), raw SHA-256 `055b1cc507e8a003f8383a3974a309510ce0718e45edf0105a1a747408dce21c`.

Call anchors identify these operations:

- `0x670C3025 -> 0x6706D410`: exported `QTextCodec::fromUnicode(QChar const*, int, ConverterState*)`, producing a `QByteArray` from the buffered UTF-16 text.
- `0x670C3032 -> 0x67021650`: clear/reset the QString staging buffer after conversion.
- `0x670C3047 -> 0x670BB750`: exported `QIODevice::write(char const*, qint64)` with the converted byte array.
- `0x670C309B -> 0x670B5E00`: exported `QFile::flush()`, conditionally called after the device write.

The order is: check device/open/error/buffer preconditions; process configured text-mode/newline handling as applicable; convert buffered UTF-16 through the selected QTextCodec; clear the QString buffer; call QIODevice::write. If the write result is nonpositive, the stream status field at object offset `+0xCC` is set to `3`. After a positive write, a byte-count mismatch also sets status `3`. A QFile flush is conditionally performed afterward; a false flush result sets status `3`.

## Limits

This establishes static call ordering and the conversion API used. It does not establish the selected codec's identity, OS write/flush outcomes, or behavior of any particular filesystem/device. The value `3` is reported as the observed status-field value without assigning additional semantics.
