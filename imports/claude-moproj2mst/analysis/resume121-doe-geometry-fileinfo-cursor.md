# CP121 Geometry QFileInfo completion and first floating conversion

## Finding

The exact source-closed cursor is: **a successful native `QFileInfo(QString)` return is followed by `path()`, then GeoOperation `toInt`; the first `QString::toDouble` is conditional on entering an UpperCorrespondence or LowerCorrespondence PointPair child loop.** This permits a useful *conditional* no-correspondence projection to avoid any assumption about the process default numeric locale. It does not establish that a converter-side InputTable parse can claim the native QFileInfo constructor completed. Consequently, no complete private or public Geometry child admission is justified by the retained static evidence alone.

## Constructor normal-return boundary

For the retained QtCore 4.8.4 PE (SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`), private constructor `0x670b8030` begins by setting the ref word at private+0 to zero (`0x670b8052`). It constructs QString path ownership (`0x670b8059` / `0x670b8066`), releases the temporary shared-null reference (`0x670b806c..807c`), initializes cache storage, resolves and stores a file engine through `0x670f0080` (`0x670b808e..8093`), builds further cache fields, clears the default-object bit (`0x670b80e9`), initializes additional private state, and reaches its normal return at `0x670b812d..813f`. On that normal return, the retained getter dispatches to the file-entry parent calculation, as documented in CP113.

The entry body is bounded in `analysis/resume79-doe-fileinfo-private-670b8030.asm`; its receipt is `analysis/resume79-doe-fileinfo-static.json`. The FuncInfo map `analysis/resume79-doe-fileinfo-unwind.json` identifies cleanup states during construction. In particular, unwind action `0x6718a060` releases the engine at private+0x50, and later state transitions also destroy initialized cache/path members. This establishes cleanup ordering on unwind; it does not establish that allocations or resolver work always return normally. The outer QFileInfo constructor is therefore complete only along the conditional normal-return edge. The converter has no observed native call or completion token for this edge, and CP113's lexical path projection deliberately reports only the value *if* it returns normally.

## Numeric cursor and locale relevance

Original GUI PE SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Reader body evidence in `analysis/resume116-doe-geometry-payload-boundary.md` gives the call order:

1. `QFileInfo(QString)` at `0x140911542`, followed by `path()` at `0x140911559`.
2. `GeoOperation` attribute conversion with `QString::toInt(ok=null, base=10)` at `0x1409115da..0x1409115ea`.
3. Path/name attributes, keyed row lookup/append, tail flags and MorphMethod effects.
4. The child loop. Only on a recognized UpperCorrespondence/LowerCorrespondence child does the reader split its text, access tokens 0–3, and call `QString::toDouble(ok=null)` four times. Upper's first call is `0x1409121f5`; Lower's corresponding first call is `0x140912543` in the retained reader. Both then construct QPointF values and append one pair.

At QtCore wrapper `0x6706c340` (`analysis/wave2-doe-qstring-double.asm`), the first parse uses a default QLocale. Its private data reaches `QLocalePrivate::stringToDouble` at `0x6704c340`; only if that attempt fails does the wrapper construct explicit locale `(1,0)` and retry. The default profile can be produced from QSystemLocale/cache state (`analysis/resume101-doe-system-locale-6704cd30.asm`), whose concrete result for the original process is not retained. The order is closed; the first parse's result is not.

Therefore, a Geometry node with no correspondence child does not traverse `QString::toDouble`, and a no-correspondence projection need not assume or emulate process default locale. That narrows one dependency only. It does not eliminate the earlier native QFileInfo side effects or prove the constructor's normal return; nor does it by itself close all remaining state transitions required for a full child parse. `GeoOperation::toInt` is a separate base-10 conversion and must retain its own source-qualified semantics; it should not be mislabeled as QLocale::toDouble.

## Safe conditional scope and remaining guard

The safest private slice remains the CP113 path-value projection: named-source filename in its qualified absolute drive/UNC profile, exact Geometry attributes, and output comprising the conditional file-entry parent and literal `parent + "/" + attribute` joins. The projection is value-only; it must not represent QFileInfo wrapper/private construction, handler/ref/cache effects, success status, row ownership, or full child admission. A further no-correspondence candidate may build on those values only when its API explicitly states the assumptions: normal QFileInfo constructor return and no Upper/Lower PointPair children. It should stop before any floating conversion and remain disconnected from public parsing unless those assumptions are independently witnessed by the caller contract.

This report does **not** recommend releasing the existing public guard or admitting a complete private Geometry node. Current source passes named InputTable context into `parseInputTable`, but the parser still leaves it unused; the Geometry branch refuses before the QFileInfo ownership boundary (`source/apps/moproj2mst/src/doe/doe_study.cpp:427–431`, `source/apps/moproj2mst/src/doe/variable_record.cpp:183–188`). There is no native constructor-completion witness there. If later evidence supplies one, an isolated no-correspondence cut can be evaluated without requiring locale data; the first PointPair remains guarded until the original default locale/result is bound or runtime evidence is permitted.

## Evidence and limits

Reviewed retained captures: `analysis/resume79-doe-fileinfo-private-670b8030.asm`, `analysis/resume79-doe-fileinfo-unwind.json`, `analysis/resume113-doe-fileinfo-path-static.json`, `analysis/wave2-doe-qstring-double.asm`, `analysis/resume101-doe-system-locale-6704cd30.asm`, and `analysis/resume116-doe-geometry-payload-boundary.md`. These are static image/function captures, not observation of a current native process. No GUI, mutable REA, source edit, build, or test was performed.
