# CP116 — Remesh default-handler unwind projection review

## Result

No blocker found. The private projection matches the retained original PE metadata and branch arithmetic. It accepts only explicit hypothetical inputs, does not read the supplied addresses, and does not change any public handler, service, or termination route.

## Original PE confirmation

The original `DEF_GUI_MO_64.exe` hash is `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. I independently mapped its exception directory and confirmed the exact table entry bytes `400447016505470198c04703` and unwind bytes `010903000901bc0002300000` at the report's RVAs. The function range is `[0x141470440, 0x141470565)`, unwind-info RVA `0x0347c098`. Disassembly at the function entry confirms `push rbx; sub rsp, 0x5e0`; the capture call returns at `0x141470485`. The fallback branch loads RIP from `[B+0x5e8]` and sets RSP to that same address, with no additional increment.

The unwind info decodes as version 1, flags 0, prolog length 9, three code slots, no frame register, `UWOP_ALLOC_LARGE` with operand `0xbc` scaled by 8 (`0x5e0`), then `UWOP_PUSH_NONVOL` for RBX. Given a supplied unmodified body RSP `B`, runtime-entry projection restores the caller's RBX word from `[B+0x5e0]`, uses the return word at `[B+0x5e8]` as caller RIP and exception address, and computes caller RSP `B+0x5f0`. The explicit no-entry hypothesis preserves the original fallback `B+0x5e8` and makes no RBX-restoration claim.

## Source and test review

`projectOriginalDefaultHandler` rejects any other captured RIP, unknown/modified stack-state hypothesis, invalid/alignment/overflowing B, unknown lookup, absent or byte-different metadata, contradictory no-entry metadata, or missing required words. It decodes only metadata already required to equal the exact retained constants. It treats zero-valued supplied words as valid values distinct from absent optionals. Arithmetic checks accommodate the full `B+0x5f0` result. No address is dereferenced; no Rtl, capture, filter, process, or native termination API is called.

The tests cover exact decoded fields, distinct supplied words, present zero values, both RSP formulas, unchanged exception address, each metadata-byte mismatch, and unsupported inputs/overflow. The no-entry case is explicitly a hypothetical lookup result: the test and comments do not claim the original image lacks a runtime-function entry at this PC. Public/native context capture, actual lookup, filter behavior, and process termination remain outside this projection.

No source edits, build, GUI, mutable REA action, or original/candidate execution was performed during review.
