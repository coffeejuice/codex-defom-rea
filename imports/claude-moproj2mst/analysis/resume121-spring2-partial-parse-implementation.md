# CP121 `SPRING2.INI` bounded malformed-prefix implementation

## Change

The companion reader now models the source-qualified ASCII `x` cursor cases from `analysis/resume120-spring2-malformed-flags.md` while keeping all other noncanonical content guarded.

- For a token such as `1x`, the companion parser consumes the valid decimal prefix `1`, leaves `x` at the logical cursor, and returns flag 1. If the loop requests another flag, the same `x` causes a failed extraction result of zero without advancing, matching the getter/operator/reader sequence.
- For a token starting with `x`, the attempted extraction returns zero and leaves `x` unread. Further requested values repeat the zero result from that same character.
- The per-row request count still bounds extraction; any suffix after the requested flags remains unexamined.

This is limited to ASCII `x` at the first invalid character, either after a valid in-range canonical decimal prefix or as the first character of the next requested extraction. Other malformed suffixes, radix-prefixed values, leading-zero forms, overflow, BOMs, and non-ASCII bytes remain refused. The shared ASCII decoder's UTF-8 BOM rejection remains in force for `SPRING2.INI`.

The focused test asserts `1x` gives `[1,0]` for two rows and `[1]` for one row, `x 2` gives `[0,0]`, and malformed `bad`, `0x1`, overflow, non-ASCII, and UTF-8 BOM inputs remain unsupported. Success and refusal cases verify the reader does not modify either input file.

## Static preflight and limits

`git diff --check` passed for the two owned files. The implementation uses no exposed stream-status object; it reproduces only the source-visible cursor and destination values for the named ASCII branches. It does not generalize group separators, sign-plus-invalid cases, all invalid ASCII characters, malformed radix prefixes, or overflow. No full build or tests were run; root owns integration validation. No page, header, public Session, GUI, or REA state changed.

## Frozen hashes

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `6522174069B42690DB1938E80552F89807934707822D36E0E4743E683E825D22` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `B34B5F84B1895992CABC2BFA31447F0FBAF9627035654CBF1A473632F31EEBAF` |

The original executable and QtCore identities, relevant instruction addresses, and static output deductions are in `analysis/resume120-spring2-malformed-flags.md`. No original-process runtime comparison was performed.
