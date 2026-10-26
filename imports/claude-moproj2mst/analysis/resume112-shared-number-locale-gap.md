# CP112 shared numeric conversion boundary

## Reachable gap

The smallest source-qualified shared-engine discrepancy found is
`QString::toDouble` locale selection. The original QtCore wrapper at
`0x6706c340` calls `QLocalePrivate::stringToDouble` first with the default
`QLocale()` (`0x6704c340`). If that attempt reports failure, it creates the C
locale and retries through the same helper. The bounded original captures are
`analysis/wave2-doe-qstring-double.asm` and
`analysis/wave2-doe-qlocale-double.asm`, both from retained QtCore 4.8.4;
`analysis/wave2-doe-qt-static-receipt.json` binds their byte ranges and hashes.
`analysis/wave2-doe-lifecycle.md` records the wrapper interpretation.

The current shared implementation in
`source/apps/moproj2mst/src/qt4/number.cpp` does not have this two-locale
sequence: `toDouble` calls `numberToCLocale` and its C-locale token conversion
directly. This makes the reconstruction narrower than original Qt on a host
whose default QLocale recognizes spellings not accepted by the C locale. It is
not a disagreement demonstrated on the active system, and no specific
non-C-locale output is asserted here.

## Public reachability

This seam is not theoretical or isolated to formatting. The DOE InputTable
parser in `source/apps/moproj2mst/src/doe/doe_study.cpp` converts each numeric
cell token with `qt::toDouble` (`:165`) and appends the returned value even
when the conversion fails. Consequently, an input accepted by original default
QLocale but rejected by the candidate's C-only path can change a public loaded
sample value to zero in the candidate. The retained source note
`analysis/wave2-doe-findings.md` already states that failures become zero.
The B121 DOE geometry-prefix reader also uses the same helper for
`nominalValue` (`doe_study.cpp:596`), with a subsequent validity and exact-value
gate. These establish reachable consequences without asserting a concrete
cross-locale case.

## Why this is not yet a safe correction

The retained native wrapper proves *attempt order*, not the full default
locale policy needed to implement it. `QLocale()` is environment dependent;
the current evidence does not bind a captured locale configuration to the
original call or provide source-qualified candidate semantics for locale
decimal/group separators, signs, digits, spacing, and fallback acceptance.
The bounded `0x6704c340` disassembly shows the internal `QLocalePrivate`
conversion branch but does not, by itself, recover how the default locale is
selected/populated or every conversion rule. The existing DOE regression is
explicitly C-locale and documents this limitation in
`analysis/wave2-doe-regexp-space-contract.md` and
`analysis/wave2-doe-findings.md`.

Implementing the host's current locale via a platform API would silently
substitute Windows behavior for Qt 4.8.4's exact default QLocale behavior.
Broadly changing the current parser or using a locale-independent heuristic
would likewise invent accepted forms. No source or test change is therefore
justified from the present evidence.

## Smallest resolving action

First recover the default `QLocale()` initialization source path for the
original process and bind it to a known locale. Then compare a tiny set of
discriminating strings against the retained QtCore function through an
authorized read-only/original-runtime probe (for example, decimal-comma input,
grouped digits, and a C-locale spelling). Record returned value and `ok` for
both default-locale attempt and C fallback. Only after that, add a narrow
locale-profile helper and tests for the recovered profile, leaving all other
locale profiles guarded. If the allowed probe cannot establish the default
locale state, the exact blocker is the missing original locale object/config
capture, not lack of an apparent consumer.

This was a source/evidence audit only. No code or test was changed; no build,
GUI, original runtime, or mutable REA database was used.
