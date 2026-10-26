# Checkpoint 51 histogram graph pointer-flow owner trace

## Bounded pointer flow

Immutable target remains `evidence/original-binaries/DEF_GUI_MO_64.exe` with SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The prior PE parse bound `.?AVSfPstGraphFactory@@` to a vtable whose slot `+0x58` points at `0x140c36bd0`; this is the routine containing the two `SfGraphHistogramItem` constructor calls. It stores each constructor return in a local result, joins, and returns the graph pointer in RAX. The relevant bounded body is `analysis/resume50-histogram-construction-function.asm` `[0x140c36bd0,0x140c3712e)`.

A second vtable entry at `+0x60` points to wrapper `0x140c36b80`. Its bounded disassembly is `analysis/resume51-factory-virtual-call.asm` `[0x140c36b70,0x140c36bd0)`. The wrapper resolves an object through `0x1412c0730`, then executes `call qword ptr [rax+0x58]` on its factory argument; it returns that virtual call's RAX directly. Given the SfPstGraphFactory vtable binding, the virtual target can be `0x140c36bd0`, so the graph pointer flows from the constructor routine through this wrapper unchanged.

## Owner search result

No direct relative `CALL` to `0x140c36bd0` or `0x140c36b80` was found in the immutable `.text` scan; the visible edges are virtual dispatch through vtable slots. The constructor routine and its `+0x60` wrapper contain no call to QGraphicsScene::addItem (`0x141e08260`) or manager+`0x10` list insertion. The constructor returns a bare graph pointer to the virtual caller.

The exact unresolved edge is the call site invoking the SfPstGraphFactory `+0x60` slot (`0x140c36b80`) and its post-return use of RAX. That edge is indirect and cannot be selected from the function body alone. Until the caller is bound, there is no evidence whether it adds the returned graph item to a scene, inserts a QPointer in the manager collection, stores another owner, or performs more initialization first. Do not infer scene ownership or approve `unique_ptr` equivalence from the factory and destructor paths alone.

## Scope and artifacts

`analysis/resume51-factory-virtual-call.asm` is the newly generated bounded DUMPBIN capture. `analysis/resume50-histogram-construction-function.asm`, `analysis/resume49-histogram-constructor-body.asm`, and `analysis/resume49-histogram-vftable-slot18.asm` retain the adjacent factory, constructor, and deleting-destructor boundaries. All evidence is static and bounded; no mutable REA state, application execution, GUI, build, or source edit was used.
