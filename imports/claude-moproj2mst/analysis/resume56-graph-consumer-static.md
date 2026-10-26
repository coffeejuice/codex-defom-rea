# Checkpoint 56 bounded graph-factory consumer search

## Identity and search bounds

Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` (67,611,136 bytes). Read-only PE byte searches and bounded DUMPBIN were used; no Ghidra/REA session, GUI, build, or source changes.

The code-byte scan covered the PE `.text` raw range `[0x400, 0x1e01800)` (RVA `[0x1000, 0x1e022a7)`). It searched x64 indirect-call forms for `[reg+0x60]`: `FF 50..57 60` and `FF 90..97 60 00 00 00`. There were 217 generic matches. Such opcode matches alone do not identify a factory object; most were not analyzed. No 8-byte absolute reference to vtable slot address `0x14284d128` exists in the image. The factory vtable itself has the expected bytes at raw `0x284b928`: its `+0x60` slot points to `0x140c36b80`; `+0x58` at raw `0x284b920` points to `0x140c36bd0`.

## Relevant manager-dispatch candidate

One candidate is `0x140c15951`. Its complete bounded routine `[0x140c15910,0x140c15963)`:

- reads a manager object's `+0x18` factory field and returns null if absent;
- loads that factory object's vtable;
- invokes virtual `+0x60` with the second argument;
- stores and returns the virtual result in RAX without another operation.

This makes `c15910` a manager-to-factory dispatch path. Combined with the retained factory wrapper at `c36b80`, it yields the flow:

`manager+0x18` → factory `+0x60` (`c36b80`) → factory `+0x58` (`c36bd0`) → constructed graph pointer → returned unchanged through `c36b80` and `c15910`.

Neither the wrapper nor this manager routine calls `QGraphicsScene::addItem` or inserts the result into manager+`0x10`; those operations are not present in the bounded paths. The result is handed back to the caller of `c15910`.

## Exact remaining edge

A byte scan found no direct relative CALL to `c15910` in `.text`. The next needed reference is therefore the code/data xrefs to function entry `0x140c15910` (including indirect/vtable dispatch), followed by one bounded view of each relevant caller to track returned RAX. That caller may perform scene insertion, manager guarded-list insertion, another owner store, or pass the pointer onward. Until that edge is bound, scene lifetime ownership remains unresolved and the current scene `unique_ptr` representation is neither validated nor refuted.

The scan is intentionally bounded to common x64 call encodings with displacement exactly `0x60`; it does not prove absence of unusual instruction encodings, thunks, dynamically synthesized calls, or semantically equivalent calls through another register/offset. A Ghidra xref query to `0x140c15910` is the narrow next probe, not a request to broaden the graph audit.
