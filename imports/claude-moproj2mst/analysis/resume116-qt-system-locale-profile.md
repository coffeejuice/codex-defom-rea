# CP116: no retained fresh-process QLocale profile witness

## Finding

The static chain confirms how the default profile is selected, but no retained
artifact records the actual `QSystemLocale` values used by an original fresh
process at a `QString::toDouble` call. I found no source-closed fixed profile
for the default `QLocale()` constructor and no retained process capture of its
selected numeric fields. The missing input is the same-process default locale
provider result/profile, not merely a Windows code page.

## What the retained PE establishes

The immutable QtCore 4.8.4 static evidence is bound in
`analysis/resume101-doe-system-locale-static.json`,
`analysis/resume101-doe-system-producer-static.json`, and
`analysis/resume101-doe-platform-query-static.json` to
`QtCore4_64.dll` SHA-256
`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
Together with `analysis/wave2-doe-qstring-double.asm`, it shows:

- `QString::toDouble` at `0x6706c340` asks `QLocalePrivate::stringToDouble`
  to parse using the default `QLocale()` first; only failure reaches an
  explicit `(language=1,country=0)` C-locale retry.
- Default `QLocale` construction at `0x6704d4f0` consults the cached default
  profile and, when needed, resolves one through `0x6704cd30`. That resolver
  obtains the `QSystemLocale` provider (default provider factory
  `0x6704cc10`, vtable `0x671a8d60`), queries locale components, and maps the
  returned values to the shipped locale table.
- The default-provider query dispatch at `0x6707ff80` is stateful. For
  example, the retained CP101 trace shows query 4 reaching `0x670803ee` and
  using cached/native provider data for a character later copied into the
  locale record. A nondefault provider can be installed at a separate global
  location. The code therefore does not imply a fixed C profile.

These are code/data-flow facts. They identify the runtime dependency but do
not yield the provider's values for a particular process. The bounded static
table extraction in `analysis/resume101-doe-static-locale-digits.json` lists
zero-digit fields for its recovered table domain; its own qualification does
not cover every mutable provider or every numeric field used in parsing.

## Retained runtime artifact check

I checked the original-process receipt and evidence under
`build/reference-harness/billet-training-004/captures/original/` and
`.../evidence/`. The receipt records a fresh process and `windows_ansi_codepage`
1252, but also says the inherited environment was not fully captured. Its
outcome is a crash during Open, before the project Save path; it contains no
`QSystemLocale` query results, active locale index, decimal/group/sign fields,
zero digit, or `QString::toDouble` call result. The runtime harness therefore
does not witness this profile. ANSI code page 1252 is not a language/country
or QLocale numeric profile and cannot fill this gap.

Retained corpus output also does not supply the input profile. In particular,
the old note in `evidence/gui_mo/s1/s1-02/spec.md` infers a C locale from
formatted decimal dots on the author's machine. That is not evidence for the
default `QLocale()` used by `QString::toDouble`: CP67's
`analysis/resume67-sw2sp-locale.md` explicitly binds the default
`QTextStream` locale to `QLocale::c()` for its reader/writer path, while
QString numeric formatting/conversion calls have their own locale selection.
An output spelling produced by that C-locale stream cannot identify the
default input-conversion profile. I have not reused it as proof.

## Exact unresolved runtime input

To close one fresh-process numeric profile, an allowed original-runtime
witness must record, in the same process and before/at `QString::toDouble`:

1. whether an application/default-locale override is already installed or the
   system provider supplies the default;
2. the `QSystemLocale` query values used to choose the language/script/country
   row (or the final selected locale record identity); and
3. the actual numeric fields consulted for `stringToDouble`, including the
   zero digit, decimal separator, group separator, signs, and number options,
   plus `toDouble` value and `ok` for discriminating inputs.

For the public recovery question, at least one actual call site and parsed
input must be bound to that process capture. Do not infer these fields from
`GetACP`, ANSI code page 1252, file spellings, or a separate locale probe. Until
the witness exists, retain the C-only candidate helper as a narrower,
qualified implementation; do not claim equivalence to default-locale
`QString::toDouble`.

This was read-only evidence inspection. No new PE capture, source/test edit,
build, GUI action, original process, or mutable REA database access occurred.
