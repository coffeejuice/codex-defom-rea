# CP465 Type3 SUB+0x28 owner and accepted Close

## Finding

The normal native SUB destructor and its Project-list disposer are already closed. Neither establishes release of the Type3 optimizer published in raw `SUB+0x28`. The accepted-Close reports close a conditional static path through Project and SUB destruction, but the same-instance admission and independent optimizer release route remain unresolved. This evidence does not justify adding a SUB-owned smart pointer, a destructor-order rule, or public Type3/Close behavior.

No new production integration is source-closed here. The smallest safe follow-up remains a private, detached value operation that does not claim native ownership; adding a private owner field or destruction action would still invent behavior. Keep actual Type3 Open/load/Save, optimizer lifetime, and accepted Close guarded.

## Established native edges

The immutable PE identity is `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

- `FUN_1400b5470` separately allocates/publishes the DOE manager at `SUB+0x20` and, for type 3, the optimizer at `SUB+0x28`. The constructor's unwind actions free only in-flight allocations. For an optimizer constructor throw, the unwind map does not invoke a destructor on the already-published manager at `+0x20`; it also does not model destruction of the optimizer object whose constructor did not complete. This is local unwind evidence, not a process-wide leak claim.
- Complete normal SUB destructor `0x1400b58d0..0x1400b59de` destroys guarded `+0x48`, guarded `+0x30`, raw `+0x0`, and then its remaining members. It never loads `+0x20` or `+0x28`. The SUB deleting wrapper `0x1400ac330..0x1400ac366` calls that destructor and conditionally releases the SUB allocation.
- Project-list cleanup `0x1400a80e0..0x1400a818a` iterates Project `+0x58`, calls the SUB deleting wrapper for each nonnull entry, then destroys the list. Project destructor `0x14008a3c0` calls this cleanup.
- The accepted-Close work reports conditionally connect Close admission to a fresh manager-child traversal, reread of the Project slot after feature cleanup, Project deletion, the Project-list loop, and SUB deletion. Thus the normal Close route can reach the known SUB destructor for the Project actually reread at the relevant point, subject to each callback/guard returning as expected. Failed-Open disposal and selected-subproject removal are separate routes; their order does not prove accepted Close.

## Exact unresolved edge

No retained evidence here traces the typed optimizer value from publication at `SUB+0x28` to any registration, alias owner, independent deletion, or destructor dispatch. The normal destructor's omission cannot prove that no other component releases it. Conversely, the optimizer's own virtual deleting entry `0x140918fc0` and internal destructor chain only show what happens if invoked; they do not identify an invocation from the containing SUB or accepted Close.

The accepted-Close chain is conditional native static control flow, not a same-instance witness that the current candidate creates the same application root, manager, Project, and Type3 SUB occurrence. Existing callback-sensitive rereads and guards remain prerequisites. No direct +20/+28 order can be inferred from the Project list cleanup or Close chain.

## Source-closed decision

Do not add native ownership to the candidate SUB yet. A candidate RAII owner or custom destructor would select cleanup that the original bytes do not establish; adding deletion of `+0x28` on Close could create double deletion if an independent native release route exists. Likewise, preserving the raw pointer forever would be an unsupported leak claim.

A source-closed increment would require one of:
1. immutable static evidence tying the same published optimizer address (or its owning control object) to an independent release call and then locating that call on the Close/replacement path; or
2. a same-instance original lifetime witness after the separate app-control release prerequisite is satisfied.

Until then, keep the detached Type3 scalar/value projection private and separate from actual `SUB+0x28` lifetime and public Close.

## Byte-bound evidence reused

No new original bytes were captured in CP465. The following prior receipts bind every cited native range to the PE SHA-256 above:

| Range | Receipt SHA-256 | Retained evidence |
|---|---|---|
| `0x1400b5470..0x1400b566c` | `cc0dd5062b9b3f9e8ffef32bda76f376675110c6af0c3e043cfa4f805304c9fb` | `analysis/resume142-close-sub-disposer.md` constructor/unwind table |
| `0x1400b58d0..0x1400b59de` | `2b35e06273c333eb3c9ee445daf5e8d27c2e54245c22236345e3bc2474beb3de` | `analysis/resume142-close-sub-disposer.md`, `analysis/resume160-type3-sub-destroy-static.json` |
| `0x1400ac330..0x1400ac366` | `ed4d13f5c1eb45d0762a74fdc00fdd3cd65df6fb8e5fdc9c846023c2afeef5ab` | `analysis/resume142-close-sub-disposer.md` |
| `0x1400a80e0..0x1400a818a` | `17d446ec2ab4fca352f9353858a184ae63a07c6d1f1e60df0c8931b89c1d9fa4` | `analysis/resume142-close-sub-disposer.md`, `analysis/resume160-type3-project-dispose-static.json` |
| `0x14008a3c0..0x14008a466` | `d33ef435aea6da822f0aa985431d82cba18f8a18374e724d0d76030e986614f9` | `analysis/resume142-close-sub-disposer.md` |
| `0x140918fc0..0x140918ff6` | `4141f39c4bb0c0541294a30f206dea14ad9807a25e9a4abd318a81a1c4d39d73` | `analysis/resume160-type3-sub-destroy-static.json` |

Accepted-Close prerequisites and same-instance limitations are retained in `analysis/resume142-close-sub-disposer.md`, `analysis/resume143-close-indirect-owner.md`, `analysis/resume153-accepted-close-trigger.md`, and `analysis/resume160-inherit-close-next.md`. CP449's published-consumer audit is `analysis/resume449-type3-published-owner-edge-audit.md`.

## Scope

Static read-only reconciliation only. No original process execution, GUI/app control, mutable REA database, source/build/test/plan/state change, or deployment action.
