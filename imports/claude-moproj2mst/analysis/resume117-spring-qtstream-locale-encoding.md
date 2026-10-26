# CP117 SPRING QTextStream locale and encoding contract

## Finding

The recovered SPRING reader constructs a `QTextStream` over each opened
`QFile` and leaves both locale and codec settings untouched. Those defaults
are distinct: Qt 4 initializes the stream's numeric locale to `QLocale::c()`,
while its text codec comes from `QTextCodec::codecForLocale()`. The numeric
parsing rule is source-closed; byte decoding can depend on converter startup
and host configuration. Current candidate acceptance is therefore safe only
for its explicitly enumerated ASCII byte fixtures, not arbitrary SPRING files
or every fresh-process codec configuration.

## Original reader and numeric parsing

The retained original decompile `analysis/wave2-save-diffs-300-support-1410dab40.c`
shows `FUN_1410dab40` opening `SPRING.INI` read-only and constructing one
`QTextStream(QIODevice*)` used for the count and all row fields. There is no
`setLocale` or `setCodec` call in this reader. The integer and real wrappers
`FUN_1410da800` and `FUN_1410da8f0` extract through that stream; their
`atEnd()` and zero-initialization boundaries are detailed in
`analysis/resume116-spring-multirow-parser.md`.

The immutable QtCore capture
`analysis/resume66-qtextstream-constructor.asm` shows the public
`QTextStream(QIODevice*)` constructor delegating to private constructor
`0x670c2b60`. In
`analysis/resume67-qtextstream-private-ctor.asm`, that private constructor
calls `0x6704d5a0` with language `1` and country `0` to initialize the stream
locale. `analysis/resume67-qlocale-c-factory.asm` binds that same pair to
`QLocale::c()`. The default numeric interpretation for SPRING's QTextStream
extractions is thus C locale. This is not the separately constructed
`QLocale()` used by `QString::toDouble`.

The candidate reader in
`source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` parses numeric
tokens with an ASCII scanner and `qt::toDouble`, which uses the C numeric
profile. That numeric choice matches the original stream default for the
qualified records. It does not by itself reproduce QTextStream's permissive
tokenization, partial extraction, or failure cursor semantics; those remain
outside the accepted exact-byte profile.

## Byte codec remains process-dependent

The same default stream constructor does not pin a codec. The retained Qt
contract in `evidence/gui_mo/s1/s1-02/spec.md` records that these writers and
readers never call `setCodec`, so Qt uses `QTextCodec::codecForLocale()`.
Startup code may set that codec from the application's language setting;
otherwise the Windows system ANSI codec is used. The chosen codec is process
configuration, independent of the stream's explicit C numeric locale.

For ASCII bytes, the language codecs and the ANSI codec preserve the ASCII
characters used in the currently admitted rows. The current candidate also
rejects non-ASCII bytes through `asciiBytes` and admits only exact complete
one- or two-row ASCII spellings with absent `SPRING2.INI`. Its two-row forms
and exact writer bytes are listed in
`analysis/resume116-spring-count-two-writer.md`; the bounded parser reasons
are in `analysis/resume116-spring-multirow-parser.md`. These exact fixtures
are safe from codec variation because no non-ASCII character needs decoding.

No claim follows for non-ASCII payloads, arbitrary high-byte whitespace or
digits, BOMs, or other codec-sensitive spellings. A profile intended to
generalize beyond ASCII must capture the original process's startup language
setting and resulting `codecForLocale()` identity (or retain a same-process
runtime witness). A Windows ANSI code page alone does not establish that the
application did not install one of its language codecs.

## QString output formatting is a separate path

The retained original writer `FUN_1410db030`, in
`analysis/wave2-save-diffs-300-support-1410db030.c`, formats double fields
using `QString::number(double, 'g', 6)` and integers in base ten. The retained
QtCore captures `analysis/wave2-save-diffs-qt-number-format.asm` and
`analysis/wave2-save-diffs-qt-number-set-format.asm` qualify this formatting
path as C-locale number conversion with six significant digits. The concrete
ASCII serialization contract is recorded in
`analysis/resume116-spring-count-two-writer.md`. This writer formatting does
not reveal the codec selected for a fresh process when strings contain
non-ASCII data.

Likewise, do not infer that every `QString::toDouble` call uses C locale:
the retained `analysis/wave2-doe-qstring-double.asm` shows that API trying
the process default `QLocale()` first and C locale only as a fallback, while
`analysis/resume116-qt-system-locale-profile.md` records the missing runtime
profile witness. That path is separate from the SPRING QTextStream reader.

## Safe parser profile and blocker

The safe source-qualified profile is the current narrow one: exact complete
ASCII `SPRING.INI` byte strings enumerated by `readSprings`, one or two rows,
finite accepted values under the candidate's C parser, and no `SPRING2.INI`.
The original stream's C numeric locale supports that numeric interpretation;
ASCII-only bytes avoid uncertainty in `codecForLocale()`.

Generalizing to other numeric spellings or byte encodings needs more than
knowing the stream's locale. It needs bounded evidence for the original
QTextStream token/extraction behavior and, for non-ASCII, the actual codec
profile. No such evidence is claimed here. No source, test, build, GUI,
original-runtime, or mutable REA changes were made for this report.
