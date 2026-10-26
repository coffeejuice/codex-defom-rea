# CP66: signed/uppercase fallback at reachable Open→Save numeric readers

## Reachable callsites and API split

The converter's reachable Extra3D integer sidecar read is `NativeExtra3DIo::readInts` → `readDecimalInt` in `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`. On the original, the matching `FUN_1410da690` constructs `QFile(QString)`, opens ReadOnly, constructs `QTextStream`, and extracts requested integers with `QTextStream::operator>>(int&)` (`analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json`). The initial Extra3D fill binds this reader to `SW2SP.DAT` and requests one integer (`analysis/resume54-numeric-path-gap.md` and its cited `0x140ec8110` fill evidence). This happens in the Open path before later Save handling.

This is not the CP64 `QString::toInt(base 0)` path. CP64's uppercase/sign result cannot be transplanted into the stream reader. The stream's original getter is QtCore `QTextStream::getInteger` at `0x670C5210`, captured in `analysis/wave2-save-diffs-qt-textstream-getinteger-complete.asm`; `operator>>(int&)` is at `0x670C60E0`, captured in `analysis/wave2-save-diffs-qt-textstream-read-int-complete.asm`.

A separate reachable direct QString conversion in the retained SimCtrl.PPV reader passes explicit base 10 to each `QString::toInt` call (`analysis/resume48-ppv-reader-receipt.txt`). The current converter's `qt::toInt` wrapper is also fixed to base 10 (`source/apps/moproj2mst/src/qt4/number.cpp:462–475`). No identified Open→Save QString caller supplies base 0.

## Stream fallback for one candidate-guarded input

The retained getter's base-0 dispatch recognizes lowercase `0x`; it does not recognize uppercase `0X` (`0x670C53D3–53E5` in the capture). After the leading zero, uppercase `X` falls through QChar digit classification; the non-digit/negative digit value branch selects decimal base at `0x670C5417–5448`. The getter ungets the lookahead and then the leading zero (`0x670C5458`, `0x670C54D5`) before the decimal parse. The decimal path accumulates the `0`, then compares `X` against the stream's locale grouping separator; when `X` is not that separator, it ungets `X` and completes the conversion with value zero (`0x670C56D8–5704`, separator path `0x670C5706–578B`, successful result path `0x670C5790` onward). `operator>>(int&)` stores that value and does not set conversion-error status on the success path (`0x670C6125–6127`, `0x670C6166–616A`).

For SW2SP's one-integer request, therefore, input bytes `0X10\r\n` are statically predicted to produce integer zero and a successful first extraction if the stream locale's grouping separator is not uppercase X. The trailing `X10` is left unread, but the sidecar reader requested only one integer and stops. This is a concrete candidate mismatch: current `readDecimalInt` rejects uppercase `0X` as unsupported, while the original getter has a decimal-prefix fallback under that locale condition.

Signed candidates are less clean: `+0x10` and `-0x10` bypass prefix dispatch and are checked against QLocale's positive/negative sign characters before decimal parsing. The sign recognition and grouping-separator checks are explicit in the getter (`0x670C545F–54D0`, `0x670C5639–5667`, `0x670C5706–578B`), but their exact behavior depends on the locale associated with the QTextStream. Do not claim signed-token results without qualifying those QLocale values.

## Boundary / next evidence

`FUN_1410da690` contains no explicit `QTextStream::setLocale` call. The retained static code does not yet establish the process/default QLocale's grouping separator for the original run. Thus `0X10`'s fallback branch and decimal prefix are source-qualified, while the exact final output/status remains conditional on the default stream locale. The smallest next dependency is a static trace of QtCore's QTextStream private constructor/default QLocale initialization sufficient to show its grouping separator, or a read-only observation of that locale in the original process. Keep uppercase and signed forms guarded in the candidate until that dependency is resolved; do not infer `QString::toInt(base 0)` behavior for them.

No source, build, test, GUI, or mutable REA action was performed.
