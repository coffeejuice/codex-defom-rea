# Checkpoint 67 offline histogram caller search

## Bounded offline method

I used the immutable retained PE only; its SHA-256 remains `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (67,611,136 bytes). I did not open a REA target, modify analysis state, access a GUI, edit source, or build.

This pass parsed the PE section headers and searched 4-byte-aligned signed-relative dword entries in `.rdata`, `.data`, `data`, and `.rodata` for targets `c15910` (RVA `0x00c15910`), `c36b80` (`0x00c36b80`), and `c36bd0` (`0x00c36bd0`). The exact selected section ranges and scan formula are preserved in `analysis/resume67-histogram-offline-raw.txt`. The scan found zero aligned relative-data entries for all three targets.

This adds a check for conventional relative function-pointer/data-table encodings to the prior complete-image literal VA/RVA checks and `.text` direct/common RIP-relative scans. It does not claim that all possible data encodings were searched.

## Ownership result

No indirect caller or consumer of the manager wrapper's returned RAX was recovered. The statically bound path still ends at `c15910` returning the factory's `c36b80` dispatch result; `c36b80` returns the `c36bd0` construction result. There is no bounded pointer flow onward to `QGraphicsScene::addItem`, manager+`0x10` registration, or another owner store. Nearby addItem sites remain unbound and cannot resolve ownership.

The exact unresolved edge is a caller/dispatch of `0x140c15910`, followed by the first RAX consumer. The relative-data scan does not identify such a caller and cannot rule out unaligned or encoded tables, computed/runtime-generated dispatch, or a separately loaded module. Scene ownership and the source `PostGraphSceneState::items_` ownership representation therefore remain undecided. The previous REA request timed out before producing xrefs (`resume66-histogram-indirect.md`), so this offline result should be treated as a bounded negative check rather than a substitute for a completed analyzer xref.
