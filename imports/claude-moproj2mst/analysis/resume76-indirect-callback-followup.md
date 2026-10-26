# CP76 follow-up: indirect callback target and branch condition

## Result

The QGraphicsItem destructor branch is not source-closed as a scene unlink. The instruction at `QtGui4_64.dll+0x5589b2` loads qword `[0x656974e8]` into `r8`, then at `+0x5589b9` calls `[r8]`. In the original on-disk image, the qword stored at RVA `0x6974e8` is `0x00000000008d3156`; with preferred image base `0x65000000`, that is VA `0x658d3156`. The target falls inside `.rdata`, at bytes beginning `DF 07 3F 64 65 73 74 72...`, part of a decorated symbol-name string (`?destroyed@QAbstractDeclarativeData...`), not a code range. Thus these static bytes do not identify a callable implementation. I do not explain away this apparent pointer/string mismatch: whether runtime initialization/rebasing or some other representation changes the effective target is not established by the retained static view.

The branch condition is explicit: destructor reads the private pointer from `[this+8]`; tests bit 23 of dword `[private+0x13c]`; only when set does it call `QObjectPrivate::get(this-0x10)`; if that result's `[+0x78]` field is non-null, it passes `this-0x10` in `rdx` to the indirect call and then clears `[result+0x78]`. No retained evidence binds this flag/pointer state to the histogram instance, or proves the indirect callback removes a graphics item from a scene. Accordingly, the earlier conclusion remains: helper c1c690 invokes the Qt base destructor, whose conditional path performs associated-object teardown, but histogram scene unlink and ownership are not proven.

## Evidence and limits

- Original DLL SHA-256: `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
- Qt destructor capture: `analysis/resume76-qgraphicsitem-dtor.asm`, range `[0x65558950,0x65558a20)`.
- c1c690 import/call chain and original executable hash are in `analysis/resume76-graph-item-base-dtor.md`.
- PE section mapping from the original DLL: `.rdata` RVA `0x696000`, raw offset `0x695000`; RVA `0x6974e8` maps to raw offset `0x6964e8`; qword there is `0x8d3156`. RVA `0x8d3156` maps to raw offset `0x8d2156`, within `.rdata`. The bounded raw bytes and calculations are in `analysis/resume76-indirect-target-bytes.txt`.
- DUMPBIN runtime function range for the destructor is `[0x65558950,0x65558f4c)` (from the DLL `.pdata` row); only the entry branch was needed for this question. This report does not claim to have semantically resolved the remaining destructor body.

## Exact missing edge

Need runtime-resolved value of `[0x656974e8]` and its callable target, plus the histogram object's relevant private flag and associated pointer state (or graph-specific caller evidence that directly adopts/removes it). Static raw data and this bounded disassembly are insufficient to close either edge. No source, build, GUI or mutable REA state was accessed or changed.

