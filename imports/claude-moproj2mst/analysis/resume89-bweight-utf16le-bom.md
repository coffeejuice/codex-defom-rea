# CP89: bounded UTF-16LE BOM case for `B_WEIGHT.DAT`

## Selected reachable contract

The same one-value `B_WEIGHT.DAT` Open read from CP88 provides a next concrete codec case. Original `FUN_140ec8110` calls the double reader `FUN_1410da510` with count 1 at source line 112, using QString literal address `0x142b14eb8`; `analysis/wave2-save-diffs-300-extra3d-keywords.json` maps that VA to `B_WEIGHT.DAT`. A successful read is formatted into the weight control at lines 120–123. The candidate path is `fillExtra3D` → `readDoubles(B_WEIGHT.DAT, std::array<double,1>)`.

Original `FUN_1410da510` constructs a default `QTextStream(QIODevice*)` over a QFile opened ReadOnly, without a codec override. The retained QtCore 4.8 image has SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Its default stream constructor enables auto-detection; the first decoder call consults `QTextCodec::codecForUtfText`. `analysis/resume58-qt-codec-for-utftext-overload.asm` shows byte prefix `FF FE` selecting MIB `0x3F6`, the Qt codec identifier for UTF-16LE; the stream decode path is in `analysis/resume58-qt-stream-decode.asm`. The generic QTS character decoder then supplies decoded characters to `getReal` (`0x670C59B0`), which consumes the numeric token. The no-custom-locale/default C `QLocale` evidence is in `resume67-sw2sp-locale.md`; its decimal separator is `.`.

Thus exact bytes `FF FE 31 00 2E 00 35 00 0D 00 0A 00` decode to ASCII `1.5\r\n` after the BOM and produce double 1.5 on this one-value read. This does not require knowing the process ACP: the UTF-16LE BOM selects the codec, and the decoded numeric characters are ASCII. The candidate CP88 change accepts only UTF-8 BOM followed by ASCII; the UTF-16LE prefix and interleaved NULs still hit the high-byte rejection in `asciiBytes`, so this is an exact reachable refusal.

## Minimal implementation/test slice

Keep the existing explicit one-value `B_WEIGHT.DAT` gate. Extend only that branch to recognize `FF FE`, decode complete little-endian 16-bit code units whose values are ASCII (`<= 0x7f`) into the existing ASCII numeric parser, then keep the existing rejection for non-ASCII code units. The test fixture should write the exact 12 bytes above and assert `readDoubles(B_WEIGHT.DAT, oneValue)` succeeds with 1.5. Add negatives for odd trailing byte or a UTF-16LE code unit above ASCII; those malformed/non-ASCII results are not qualified and should continue to be guarded. Leave UTF-16BE, UTF-32, BOM-less UTF-16, non-ASCII UTF-16 content, other file names, and multi-value requests unchanged until separately qualified.

This would advance actual Extra3D Open behavior for one bounded input without generalizing the sidecar decoder. It does not change integer readers, the numeric parser, Save serialization, or host-locale behavior.

## Qualifications and open points

The codec BOM branch is source-qualified in the common QTextStream constructor/decoder, while its original reader binding is specifically `B_WEIGHT.DAT` count 1. No original-process execution was performed. The code evidence establishes UTF-16LE codec selection, not malformed/truncated UTF-16 replacement rules; the proposed implementation deliberately rejects those cases. The UTF-8 BOM CP88 receipt should describe the gate precisely: zero-value spans return after successful open before decoding; spans greater than one remain on the existing decoder and reject the BOM.

Read-only audit only. No source, test, build, GUI or REA state changed in CP89.
