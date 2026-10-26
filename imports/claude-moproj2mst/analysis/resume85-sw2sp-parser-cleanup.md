# CP85 cleanup: remove dead SW2SP integer literals

## Change

Removed the `allowSw2spOneIntProfile` parameter and its flag-guarded exact-token ladder from `readDecimalInt` in `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`. The sole fallback call now uses the two-argument function. The one-value SW2SP route remains before the fallback and still uses the CP84 parser. The focused test file did not assert the obsolete internal flag or its branch layout, so it was not changed.

The reachable generic behavior was kept: lowercase `0x` parsing, the legacy signed/uppercase `x/X` zero-prefix fallback, the exact `010`→8 branch, strict decimal validation and `qt::toInt` range handling. In particular, `010` had no `allowSw2spOneIntProfile` condition in the previous source and therefore was not part of the dead ladder; it remains in place for generic callers. No filename, count, ASCII decoder, path, file-open, or `SPRING.INI` logic changed.

## Static reachability proof

Before cleanup, source search found one `readDecimalInt` invocation: the fallback in `NativeExtra3DIo::readInts`, passing `false`. `readInts` routes only `values.size() == 1 && path.u16() == u"SW2SP.DAT"` to the parser before reaching that invocation. Thus no current execution could satisfy any `allowSw2spOneIntProfile` condition. The in-scope Extra3D caller in `simcontrol300_io.cpp` requests `SW2SP.DAT` into `std::array<int, 1>`; the original caller count qualification is recorded in `resume71-sw2sp-multivalue-boundary.md`. `SPRING.INI` still falls through with the strict generic behavior because it does not match the exact filename.

This is a static call graph/source argument, not a runtime test. I did not build or run CTest; root owns integration validation. `git diff --check` emitted no diagnostics. No test was added because CP84's tests assert externally visible parsing and path/count gates, not the removed internal branch layout.

## Hash

SHA-256 `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp`:
`7899DEFD95F9B6F853AFCF5096A91326AD07BF182855440E383424C129C97A0C`.
