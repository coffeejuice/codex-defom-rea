# CP156 RINHERIT — document slot boundary

## Finding
No source-ready inheritance integration boundary is established. The smallest missing witness remains native lifetime dataflow for the active SubProject's `+0x30` document slot through accepted Project Close and QObject teardown. Keep the public Report guard. The existing post-save viewer callback cannot substitute: it runs after the document-sensitive save callback and does not call the document accessor or document methods.

## Qualified facts
- `analysis/takeover-state.json` RINHERIT entry is ready, but implementation/integration are partial and validation incomplete. Its CP145 next action explicitly says same-document slot, accepted Close release, and QObject teardown remain missing.
- Original PE: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- `evidence/gui_mo/s1/mdt/L7/s0.c` SHA-256 `66149a...f5c2e935`: `FUN_1412adf90` calls Report `vt+0x80` (`FUN_1412aeb30`) before `FUN_1412af0c0`; the first resolves active SubProject via `FUN_1412b0f70`, obtains its document through `FUN_1400b5ad0`, derives the `.DS` sibling and calls `FUN_140645030(document,path,1)`. This establishes order and args, not callee effects.
- `evidence/gui_mo/s1/mdt/L7/d3.c` SHA-256 `6f6f7205103782d7ae11c35bf2f33245433b60b6bf8b09dea29c2572bed19582`: `FUN_1400b5ad0` reads/reuses SubProject `+0x30`; if empty it allocates `0x70`, calls `FUN_140643100(raw, mainWindow, mode, mode!=2, 0)`, then publishes with `FUN_140cbc920`.
- `analysis/resume145-report-document-edge.md` records the bounded CP145 call/order review. `analysis/resume63-report-viewer-bounded-capture.txt` SHA-256 `0a0b5b87c1f034e2f0207f23864222098aeb2577af6d5f95def9d49f536b5559` is the viewer-body capture: callback `0x1412af0c0` iterates a returned collection but does not access `0x1400b5ad0`, `0x140645030`, `0x140645dc0`, or `0x140644fd0`.

## Next evidence increment
Obtain a bounded native SubProject destructor / accepted-Close trace covering writes or clears of `+0x30`, `0x140cbc920` release semantics, deleting-destructor path, and ordering relative to QObject parent teardown. If immutable PE evidence cannot establish same-instance identity, the smallest substitute is an original Open→Save→accepted-Close trace binding Project, active SubProject, document pointer, parent and destruction order. Also query `0x140645030` body separately to establish Save effects. Until then do not add a Report document owner or infer mutations from viewer iteration.
