# Checkpoint 71 histogram indirect-owner lane

## Alternate dispatch routes checked

This pass used the verified immutable PE and retained static captures only; it made no REA import or metadata change, opened no GUI, edited no source, and ran no build. Binary SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

I checked the routes that could hide the missing caller from a direct call scan:

- **Manager/factory vtable or function-pointer slot:** the complete image has no 8-byte absolute pointer equal to `0x140c15910`, and the target RVA `0x00c15910` appears only in the `.pdata` function record. Thus no stored absolute vtable/function-pointer entry to `c15910` is present in this PE. The known `SfPstGraphFactory` slots instead point to `c36bd0` (`+0x58`, graph factory) and `c36b80` (`+0x60`, wrapper), as shown by the raw table bytes in `resume64-histogram-caller-raw.txt`.
- **Direct code and common RIP-relative references:** the complete `.text` scans documented in `resume60-graph-owner-xref.md` found no E8/E9 target, absolute reference, or common RIP-relative LEA/MOV/FF reference to `c15910`.
- **Relative data table:** the offline aligned signed-rel32 scan across `.rdata`, `.data`, `data`, and `.rodata` found no entries to `c15910`, `c36b80`, or `c36bd0` (`resume67-histogram-offline-raw.txt`).
- **Generic virtual-call candidates:** 217 `call [reg+0x60]` forms occur in `.text`, but none has a retained receiver/vtable flow establishing it as an incoming call to `c15910`. The `+0x60` dispatch *inside* `c15910` is the manager-to-factory edge, not an incoming caller.
- **Registry route:** no retained Qt meta-call/registry record identifies `c15910`; no direct code target, absolute table entry, or aligned relative table entry points to it. A runtime-computed or string-based path without an application static target remains outside these byte scans.

## Pointer and initial-state boundary

The full manager constructor contract establishes `+0x10` as a newly initialized empty QList and replaces the base factory at `+0x18` with `SfPstGraphFactory` during derived construction. That is initial construction state only; it does not establish the first call time or prove that a call to `c15910` occurs after construction.

Within the known wrapper chain, a nonnull manager `+0x18` dispatches through factory slot `+0x60` to `c36b80`. That routine sends its second argument through helper `0x1412c0730`, then dispatches factory slot `+0x58` to `c36bd0`. The constructor result returns through both wrappers in RAX. No bounded function carries this RAX into `QGraphicsScene::addItem`, manager `+0x10`, or another owner store.

## Result

No indirect caller/adoption path was recovered. The specific unresolved edge remains `caller or registry dispatch -> c15910 -> first post-return RAX use`. The retained evidence rules out the listed static vtable/function-pointer and table encodings in this PE, but not computed/runtime-generated dispatch, unusual data encodings, or references from another module. The scene lifetime owner and `PostGraphSceneState::items_` unique ownership therefore remain unvalidated; nearby unbound `addItem` sites do not change that conclusion.
