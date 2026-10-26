# Checkpoint 70 histogram owner and initial-state boundary

## Retained evidence inspected

This pass used only the retained static PE captures and contracts; it made no REA import or metadata change, opened no GUI, and touched no source or build output. The target identity remains `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

The relevant retained sources are `analysis/resume56-graph-manager-wrapper.asm`, `analysis/resume51-factory-virtual-call.asm`, `analysis/resume50-histogram-construction-function.asm`, `analysis/wave3-close-graph-manager-constructor-contract.json`, and `analysis/wave3-close-graph-manager-cleanup-contract.json`. The last complete static reference search remains bounded by `resume60-graph-owner-xref.md`; the REA xref attempt timed out before producing references (`resume66-histogram-indirect.md`).

## What the first dispatch would see

The constructor contract says manager base construction initializes its `+0x10` QList and sets manager `+0x18` to null, then creates/caches a base `SfGraphFactory`. The derived `SfPstGraphManager` construction subsequently creates `SfPstGraphFactory`, deletes/replaces the base child in the recorded order, and commits the derived pointer to manager `+0x18`. Thus, after successful full construction, the guarded graph collection at `+0x10` begins empty and `+0x18` references the derived factory. This is the established constructor state; it does not establish that a call to `c15910` occurs at that point or that the list remains empty later.

For any `c15910` invocation with a nonnull `+0x18`, the wrapper forwards its second argument to the factory virtual `+0x60` (`c36b80`). That wrapper passes the argument through helper `0x1412c0730`, moves the helper's RAX into RDX, then dispatches factory virtual `+0x58` (`c36bd0`). `c36bd0` constructs `SfGraphHistogramItem` on its conditional branches and returns the constructed pointer; `c36b80` and `c15910` return their virtual result in RAX. The caller-supplied argument's identity and the helper's semantics are not recovered in the retained bounded evidence, so they cannot be labeled as an owner or scene.

## Ownership result and open edge

This establishes initialization of the manager's factory/list fields and the dataflow internal to the possible factory call. It does not recover the caller to `c15910`, the timing of its first invocation, or any post-return write of the graph pointer. No bounded capture places that pointer in `QGraphicsScene::addItem`, manager `+0x10`, or another owner field. Existing nearby addItem callsites remain unbound to this result. The exact missing link is still: caller/dispatch into `0x140c15910` → first use of its returned RAX.

Consequently, the native lifetime owner remains undecided. The native manager list's guarded/nonowning representation and unlink-before-delete behavior are separately established, but they do not decide scene ownership. `PostGraphSceneState::items_` as `unique_ptr` therefore remains unvalidated by these artifacts. The next evidence must come from an already analyzed root-owned static xref/callgraph or a new bounded capture that binds a concrete caller and follows only its first returned-pointer consumer; absent that edge, retain the uncertainty.
