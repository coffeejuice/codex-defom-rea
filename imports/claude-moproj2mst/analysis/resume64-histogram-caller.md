# Checkpoint 64 histogram consumer search

## Search result

The immutable original PE `evidence/original-binaries/DEF_GUI_MO_64.exe` was rehashed: SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, size 67,611,136 bytes. This was a bounded read-only PE-byte/DUMPBIN audit. No GUI, source edit, build, or mutable REA operation was used.

The factory table bytes bind `SfPstGraphFactory` slot `+0x58` (`0x14284d120`) to `c36bd0` and slot `+0x60` (`0x14284d128`) to `c36b80`; see the raw receipt `analysis/resume64-histogram-caller-raw.txt`. The method chain remains `c15910` manager wrapper → factory `+0x60` (`c36b80`) → factory `+0x58` (`c36bd0`) → histogram constructor result. The two recovered RIP-relative refs to the table start are its constructor/destructor vtable writes (`c36b36`, `c36b5e`), as captured in `resume63-graph-adoption-xref.asm`.

A bounded whole-image search found no direct relative CALL/JMP, absolute-VA pointer, or common RIP-relative reference to manager wrapper `c15910`. Its 32-bit RVA appears only in the `.pdata` runtime-function record. A separate whole-image scan found no literal slot-address `0x14284d128`; the `.text` scan found 217 generic `call [reg+0x60]` opcode candidates, but none can be attributed to the concrete factory without receiver/vtable dataflow. No recovered candidate connects the factory result to QGraphicsScene::addItem, manager+`0x10`, or another owner store.

## Ownership conclusion

The requested consumer-to-scene edge was not recovered. The graph constructor result is returned through the factory and manager wrapper, but no bounded static callsite consumes the returned RAX. Nearby `QGraphicsScene::addItem` imports remain unbound to this value and cannot establish ownership. Therefore this pass closes the listed PE-byte search without resolving native scene ownership; the `PostGraphSceneState::items_` lifetime representation remains unvalidated.

The complete search receipt records the checked encodings and their limits. It does not rule out computed or dynamically populated dispatch, unusual instruction forms, or references from separately loaded modules. Resolving those would require the root-owned static-analysis xref/callgraph view to be open and to return an indirect caller; there is no evidence in the retained bounded captures that justifies expanding the source model.
