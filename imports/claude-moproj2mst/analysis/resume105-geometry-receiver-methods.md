# CP105 — Native Geometry receiver method follow-up

## Concrete receiver effect recovered

The original PE identifies `0x1408175c0` as a receiver-owned registry insertion/update routine, rather than a file loader. The reader calls it at `0x140911f99` with `[rsp+0xa8]` as receiver, GeoOperation and GeoObject scalars, and three QString-derived values (including the combined geometry filename paths).

In `0x1408175c0..0x1408177bb`, the method first gets the count for `this+0x28` (`0x1408175e0..0x1408175ee`) and iterates entries with `0x1402eee10`. It compares two strings (`entry+8`, `entry+0x10`) and two integers (`entry+0x1c`, `entry+0x20`) against the incoming values (`0x140817762c..0x14081776b3`). If no match exists, it allocates `0x38` bytes (`0x1408176ce`, allocator call `0x140146eabc`), initializes the object via `0x1408178790`, assigns three QString fields from incoming args (`0x140817720..0x14081774e`), writes the two integer fields (`0x140817759..0x14081776d`), and inserts it into the receiver's `this+0x28` list through `0x1402d0580` at `0x1408177ad..0x1408177b6`. The method then returns. Thus the Geometry child has a concrete owner/list mutation edge: a receiver owns a list of separately allocated 0x38-byte records keyed by the strings and integer pair. This still does not identify the receiver's concrete class or establish its destruction policy.

## Adjacent methods

- `0x140817840` reads `this+0x28` and returns its count through helper `0x1400004ae0`; this matches the count used at the beginning of `0x1408175c0`.
- `0x140817f40` checks index bounds against the same list and writes one byte at entry offset `+0x18`.
- `0x140817fa0` similarly writes one byte at entry offset `+0x19`.
- `0x140817890` directly writes an integer at receiver offset `+0x20`.

At reader callsites `0x140911fcd`, `0x140911fea`, and `0x140911ffe`, these are post-insertion indexed/receiver updates. They are not file effects.

## First file effect remains unresolved

The retained reader disassembly then starts Geometry child-node traversal at `0x14091201c`. Its prior fields include upper/lower/nominal file names and path concatenation (`0x140911862`, `0x14091189f`, `0x1409119a6`, `0x140911aea`), but the captures inspected here do not identify a QFile/QIODevice open or read after that traversal begins. The new result closes the row allocation/insertion edge only; it does not prove payload loading, receiver destruction, or final geometry owner lifetime.

## Next dependency

Inspect the bounded child loop after `0x14091201c` for the first file open/read and its connection to the newly inserted list record. Separately recover receiver construction and destructor/container destruction at `this+0x28`; do not infer those from the insertion helper.

## Evidence qualification

- Binary: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- Original bounded reader: `analysis/resume50-doe-variable-body-pe-annotated.asm`, call/argument setup at `0x140911f99..0x140911ffe` and child traversal at `0x14091201c`.
- Existing static capture: `analysis/resume103-doe-prune-fields-140817840.asm` establishes the count getter body.
- This follow-up used read-only DUMPBIN ranges on the hash-qualified original PE: `0x1408175c0..0x140817720`, `0x140817720..0x1408177d0`, `0x140817f40..0x140817f90`, `0x140817fa0..0x140817ff0`, and `0x140817890..0x1408178e0`. The captures establish local control/data flow only; names for the record fields and receiver class remain inferred from call arguments and offsets, not symbols.
