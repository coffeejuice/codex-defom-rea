# CP114 Qt default-locale numeric path

## Recovered wrapper decision

The retained QtCore 4.8.4 PE resolves the outer `QString::toDouble(bool*)`
branch precisely. In `analysis/wave2-doe-qstring-double.asm`, entry
`0x6706c340` constructs a default `QLocale` at `0x6704d4f0`, obtains its
private data through `0x67049340`, and calls
`QLocalePrivate::stringToDouble` at `0x6704c340`. It records that attempt's
success in a local byte. If successful, the wrapper returns that parsed value
and writes true to the caller's `ok` pointer when present. If it fails, the
wrapper constructs a locale with `(language=1,country=0)` using
`0x6704d5a0`, gets its private data, and makes a second call to the same
`stringToDouble` routine. The exported `QLocale::c()` factory at
`0x67001130` calls that constructor with the same `(1,0)` pair; see
`analysis/resume67-qlocale-c-factory.asm`. This makes the fallback explicitly
the C locale, not a retry using another spelling rule inferred from the host.

The fallback contract is therefore source-closed at the wrapper level:
**default-locale parse first; if it fails, C-locale parse; return the first
successful result, otherwise the C parse result and its success state.**
This does not close the content of either parser for all locales.

## Where the default profile comes from

The default `QLocale` constructor in
`analysis/resume67-qlocale-default-ctor.asm` delegates to
`0x6704cd30` when the cached default profile is absent/stale. The bounded
`analysis/resume101-doe-system-locale-6704cd30.asm` shows that this resolver
obtains a `QSystemLocale` provider through `0x6704cc10`, issues several
provider-vtable queries for locale components, and uses the returned language,
script, and country values to select or fall back among locale data records
at `0x671b2040` (record stride `0x90`). The provider and cache are process
state; the constructor does not hard-code `(1,0)` for ordinary `QLocale()`.
`(1,0)` is the separately constructed C fallback.

The retained table inspection in
`analysis/resume101-doe-static-locale-digits.json` samples the selected table
domain's zero-digit fields and explicitly qualifies that domain; it is not a
complete recovery of every locale record's decimal/group/sign/number-option
fields or a capture of which profile the application selected at a particular
run. Retained CP67 evidence proves `(1,0)` is the C locale for the separate
default `QTextStream` construction in its Extra3D case; that result does not
make the default `QLocale()` used by `QString::toDouble` C locale.

Thus the missing state is now precisely identified: **the default
`QSystemLocale` query result/profile for the converter's original process and
the corresponding full numeric record/rules.** The PE establishes that the
profile is provided dynamically. Static inspection cannot supply the concrete
query result for a fresh process, and a profile from another run or host would
be an assumption.

## Can a parsing slice be implemented without runtime locale assumptions?

Only the explicit C fallback is independent of the runtime locale, but current
`qt::toDouble` already parses through its C-locale helper. That alone is not a
source-closed replacement for the original wrapper: when a default locale
accepts an input, its first result wins even if the C-locale parser would
accept that same text differently. A helper that unconditionally accepts
candidate C forms would change the ordering and could change the value or
success result for a non-C default profile. Emulating default parsing by
`std::locale`, Win32 locale APIs, or a guessed English profile would substitute
another implementation for Qt's selected record and parser rules.

No general or useful public parsing slice is closed without either (a) an
explicitly bound locale profile in the public call contract, or (b) a fresh
original-process witness that binds the `QSystemLocale` query result and all
numeric fields consulted by `stringToDouble`. The existing conversion ABI has
no such locale-profile argument. The correct action under the present contract
is to retain the C-only helper and its existing guard/qualification, not to
infer runtime locale.

## Smallest next evidence action

If original runtime observation becomes allowed, record the locale component
values returned to `0x6704cd30` and the selected table record for one fresh
original process, then evaluate just a few discriminating inputs through
`0x6706c340` with `ok` captured: a C-style decimal, that profile's decimal
separator, and a grouped form. Bind the witness to the exact QtCore hash and
process locale state. A source-only route would instead need a complete
bounded trace of the `QSystemLocale` provider vtable's query implementation
and all `stringToDouble`-consulted fields, plus a source-backed stable rule for
the system locale configuration; the current retained captures do not close
that provider/configuration chain.

Evidence reviewed: `analysis/wave2-doe-qstring-double.asm`,
`analysis/wave2-doe-qlocale-double.asm`,
`analysis/resume67-qlocale-default-ctor.asm`,
`analysis/resume67-qlocale-c-factory.asm`,
`analysis/resume101-doe-system-locale-6704cd30.asm`,
`analysis/resume101-doe-system-locale-static.json`, and
`analysis/resume101-doe-static-locale-digits.json`. Their receipts bind the
captures to the retained QtCore 4.8.4 image; no new disassembly was needed.
No source/test change, build, GUI, original process, or mutable REA database
was used.
