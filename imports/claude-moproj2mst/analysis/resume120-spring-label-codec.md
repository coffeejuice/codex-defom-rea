# CP120 SPRING combo-label local8Bit boundary

## Finding

The recovered selector converts each displayed combo label with
`QString::toLocal8Bit()`, copies the resulting C string into a fixed 208-byte
stack region with unchecked `strcpy`, and scans a decimal `%d` prefix. This is
not a bounded full-label parser and has no truncation step. For generated
short ASCII labels, the decimal prefix is invariant across the original
startup's supported locale codecs and Windows ANSI codec. Exact non-ASCII
conversion bytes and oversized-label effects remain process-dependent or
unsafe, so the existing candidate policy of at most 64 ASCII bytes including
NUL is appropriately conservative; it is not the native limit.

## Original conversion and parse path

The original binary is `DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
`analysis/resume119-spring-label-static.json` binds the retained DUMPBIN
captures and import identities. In
`analysis/resume119-spring-label-parser-140ec9980.asm`, function
`0x140ec9980` initializes its integer result to zero, returns zero directly
for empty text, and otherwise calls imported `QString::toLocal8Bit` at
`0x140ec99dd`. It obtains `QByteArray::data`, copies with `strcpy` into
`[rsp+0x40, rsp+0x110)` (208 bytes), and calls `sscanf` with literal `%d`
at `0x140ec9a0d`. The scan result is not checked. A nonempty label with no
decimal prefix therefore leaves and returns zero; a valid decimal prefix may
be followed by arbitrary suffix text. Leading zeros and a leading sign are
accepted by the `%d` prefix scan. This establishes parser mechanics, not
every C runtime overflow result for out-of-range numeric text.

The selector capture
`analysis/resume119-spring-combo-label-140ec8da0.asm` shows it visiting
`itemText(index)` in ascending index order, parsing each string with this
helper, choosing the first matching parsed ID, and preserving the previous
index if none matches. The builder capture
`analysis/resume119-spring-label-builder-140ec98e0.asm` shows prefix format
`%-3d - ` followed by the supplied object name. Neither path uses QVariant
item data. Full source qualification and frame reconciliation are in
`analysis/resume119-spring-itemclick-transition.md`.

## Codec qualification

`QString::toLocal8Bit` delegates through Qt's local codec to
`QTextCodec::fromUnicode`. Retained QtCore evidence in
`analysis/wave3-editor-observed-language-binding.json` identifies the
observed Windows local codec implementation as `QWindowsLocalCodec`, whose
`fromUnicode` calls `WideCharToMultiByte(CP_ACP, 0, ...)`. The same evidence
does not identify the numeric ACP of the original process or sample
conversion bytes. Also, the converter startup may install one of its
language-specific locale codecs before these calls; the lazy codec state and
that startup dependency are documented in
`analysis/wave3-editor-translator-codec-observation-contract.json` and
`analysis/wave3-original-manifest-codepage-qualification.json`. Therefore do
not state that the original label was encoded as CP1252 or even that the
Windows local codec branch was active for an arbitrary run.

For the bounded generated-label profile, the number prefix and separators
are ASCII. ASCII bytes survive the supported Qt locale codecs and Windows
ANSI codec unchanged, so the decimal prefix presented to `%d` is stable for
labels whose names are also ASCII. Non-ASCII names can encode differently,
expand to multiple bytes, or use replacement behavior depending on the active
codec; the retained captures do not establish the original run's exact
bytes. Text such as a non-ASCII digit is not admitted by the current
candidate profile and should not be assumed to parse as a decimal prefix.

## Bounds, overflow, and candidate admission

There is no `strncpy`, length check, or truncation before the 208-byte
`strcpy` destination. If the converted local8Bit label has at most 207
non-NUL bytes, its terminating NUL fits in this region. More than 207
non-NUL bytes writes beyond the local buffer. The disassembly proves this
logical overwrite boundary but cannot qualify which adjacent stack values
are corrupted or the eventual process outcome. A shorter wide QString may
also expand during local8Bit conversion, so UTF-16 character count is not a
substitute for checking encoded byte count.

The native object-name field layout gives a 0x54-byte stored name slot, but
the retained evidence does not prove every source name is terminated or
ASCII; the native builder's append does not impose a parser-side bound. The
existing <=64 ASCII bytes including NUL candidate admission is a stricter
policy that keeps conversion within the stack region and avoids locale
variation. It must remain described as candidate policy, not a native 64-byte
limit. Do not truncate labels to emulate the native behavior. Numeric prefixes
outside the representable `int` range are also not qualified as portable
`sscanf` outcomes and should remain excluded from generalized parity claims.

## Safe current conclusion

The current ASCII short-label cases can assert prefix parsing and first-match
selection without knowing the process ACP: the bytes relevant to their
numeric prefix are ASCII. For arbitrary Unicode labels, exact converted
bytes require a same-process observation of active Qt local codec/ACP plus
the result of `toLocal8Bit`; for oversized labels, source evidence only
establishes an unchecked write past the 208-byte region, not a deterministic
fault or partial GUI outcome. Keep both cases outside candidate admission.

No source, build, GUI, original-runtime, or mutable REA changes were made.
