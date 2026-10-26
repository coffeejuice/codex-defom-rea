# CP65: no reachable `QString::toInt(base 0)` seam in current Open-to-Save source

## Result

I found no current converter call site that supplies base 0 to `QString::toInt`. The uppercase/signed base-0 contract from CP64 is valid for that Qt API but has no matching seam in the current converter Open→Save path, so no source implementation is warranted in this lane.

The current compatibility wrapper `qt::toInt(const String&, bool*)` has no radix argument. In `source/apps/moproj2mst/src/qt4/number.cpp:462–475`, its implementation calls `bytearrayToLongLong(buf, 10, &my_ok)` and checks signed `int` range. Therefore every current call through this wrapper is base 10. A concrete in-scope apply path is `source/apps/moproj2mst/src/engine/simcontrol300.cpp:706–715`, where deformation and thermal iteration fields use `qt::toInt` before updating the simulation control object. This is not a base-0 caller; an `0X10` test there would be testing the decimal wrapper, not CP64's base-0 parser.

## Original call-site distinction

The original SimCtrl.PPV reader contains several direct QString conversions and their retained decompilation explicitly supplies radix `10` (`analysis/resume48-ppv-reader-receipt.txt`, also summarized in `analysis/resume48-ppv-writer-contract.md`). Its writer uses QTextStream insertion. The original Extra3D SW2SP read path instead constructs QTextStream and extracts `int&` in `FUN_1410da690`; that stream path has its own `QTextStream::getInteger` implementation and is not equivalent to `QString::toInt` (`analysis/wave2-doe-sim300-extra3d-sidecars-decompile.json`, `analysis/resume54-numeric-path-gap.md`).

Thus CP64's uppercase `0X` acceptance and sign-before-prefix behavior must not be grafted onto either current `qt::toInt` callers (which are decimal) or `readDecimalInt` (which reconstructs the distinct stream extractor). `resume65-toint-callsite-evidence.txt` contains bounded current-source excerpts and identifies the original retained evidence used for this distinction.

## Smallest valid follow-up

Only proceed with CP64 grammar when a current source seam is added or identified that corresponds to an original direct `QString::toInt(..., base 0)` call. The bounded source search in this lane found no such explicit base-0 call, and the shared `qt::toInt` adapter currently exposes no way to request that radix. Any new API or call site would need its own original call-site qualification before implementation. No source/test edits, build, GUI, or mutable REA action was performed.
