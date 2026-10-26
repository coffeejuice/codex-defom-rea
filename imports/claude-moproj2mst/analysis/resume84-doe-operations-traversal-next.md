# CP84: Operations traversal limits and outer variable-ID finalization

Keep CP83's normal true result for populated Operations and clarify its implementation comment. A fake no-op loop cannot reproduce native QDom reference ownership. The next meaningful independent effect is the **outer InputTable reader's variable-number finalization** after normal child dispatch: source loops existing variables and repairs nonpositive numbers using a particular ordered, wrapping scan. This is currently absent and is reachable with Operations-only input and an actual Study that already owns variables. It requires no QWidget, QFileInfo, C-string codec or QString shared-null owner.

This evidence/design report was frozen before the authorized successor implementation. Only this report was written in this investigation. CP83 source remained held until root confirmed its archive. Read-only PE/DUMPBIN output was inspected without writing extra capture/database files; no build, tests or GUI control occurred.

## Operations: minimum faithful state and limits

The reviewer caveat in `resume83-doe-operations-source-review.md` is valid: candidate `status=true` skips internal native traversal. Its ordinary result and lack of payload application agree with original `0x140912a20`; native internal ownership is separate.

Bounded export/import/disassembly of original QtXml4_64.dll, SHA-256 `b324ee97b941a9093212f851fb9b02f7e48ccd3db54edad3dde11b3c483634df`, preferred base `0x61000000`, further qualifies the calls:

| QtXml source | Recovered state/ownership |
| --- | --- |
| firstChild `0x61002750..2791` | Reads source private+0x28; stores that existing child pointer in output wrapper, then refs child private+8 if non-null. Null source returns a null wrapper. |
| nextSibling `0x61002840..2881` | Reads source private+0x18; stores existing sibling and refs its private+8 if non-null. |
| toElement `0x61006120..6181` | Calls actual node-kind virtual+0x78. On true retains the same private node/ref; otherwise produces a null element wrapper. It does not inspect descendants. |
| isNull `0x61002be0..2be8` | Checks wrapper's pointer only. |
| assignment `0x61003790..37ed` | Ref incoming node first, deref old node, virtual-delete old node on zero if needed, then publish incoming pointer. |
| QDomNode/QDomElement destructor `0x61003e50..3e86` | Deref private+8; on zero calls private virtual deleting destructor with flag1. Both exported destructors share this entry. |

QtXml IAT `0x61043830/0x61043828` resolves to QtCore `QBasicAtomicInt::ref/deref`. None of the inspected firstChild/nextSibling/toElement/isNull paths directly calls an allocator. The ref helpers and virtual kind/deletion callees are separate ownership boundaries. A prior assumption of a fresh allocation at each wrapper construction would therefore be unsupported.

The native Operations normal sequence is: hold passed element -> acquire first child -> test current -> acquire element alias/null -> observe its nullness (no payload branch) -> acquire next sibling -> assign current (incoming ref before old release) -> release next temporary -> release element alias -> repeat; finally release null current and passed element. Raw text/comments/PI/CDATA participate in sibling order even though they become null element wrappers. Nested descendants are never traversed by this function.

To model literal native refs, one would need canonical private-node identities and ref cells plus typed virtual-kind/delete ownership, with current/element/next leases. Unknown ref counts or unknown virtual deletion must remain guarded, not become `shared_ptr::use_count()`. No current XML seam exposes this owner. `xml::Node` owns a vector of shared children; `childElements()` allocates a raw-pointer vector and filters non-elements. Neither is native QDom ref ownership. An O(1) pass over child NodePtr values could preserve order under a documented host abstraction, but would still not supply native counter/deletion or allocator-failure parity.

There is no payload callback or persistent Study effect to gain from adding such a loop now. Do not add a diagnostic event vector whose own allocations are presented as native effects, or inject arbitrary per-node failures as recovered behavior. Clarify the comment to say normal-return equivalence and keep this internal-fault/ref caveat. Actual native zero-ref deletion paths remain outside the claim.

## Independent outer ID owner and timing

Original GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Retained `wave2-doe-type2-session-load-dom-decompile.json` contains original `0x14090e080`, and bounded original disassembly confirms its tail:

1. After the child loop reaches normal completion, `0x14090ed14` calls manager vt+0x178 for a fixed outer count.
2. Each source-order index calls vt+0x1a0 at `0x14090ed54`. Null returned pointers are skipped at `0x14090ed62/6b`.
3. `0x14090ed75` reads variable number through `0x140100980`, a plain read of variable+0x1c. Numbers >=1 are preserved.
4. For a number <1, `0x14090ed92` calls manager vt+0x1e8; both SfMoDoeManager and DefDoeModel bind it to `0x14091da70` in retained `evidence/gui_mo/s1/vtables.tsv`.
5. `0x14090edae` calls `0x1409ff590`, a plain number store to that same variable+0x1c. The loop then advances once; it does not retry a newly assigned nonpositive result.
6. `0x14090edb8` returns the existing child-reader status after this finalization. Headers-only input can still return false while IDs have changed. Malformed XML returns before this tail; an existing thrown unsupported child guard also never reaches it.

Candidate `Study::vars`, `Var::number`, existing Model count/varAt and the actual owned Study already represent these owners/fields. No new data owner or QObject is needed. The branch does not change Var.index, ops, link, allocation or vector order. Empty DoeVariables with existing variables remains a separate index-finalization guard and is not relaxed in this increment.

## Exact next-number scan, including native edge cases

`0x14091da70` starts a signed32 candidate at0 and obtains a list view through vt+0x1d8 (`SfMoDoeManager -> 0x1400d7f20`, which copies the manager+0x38 pointer list). It scans in source list order, reading each current variable number. At `0x14091dae2/6` it compares that number with the current candidate; when number >= candidate it adds1 at `0x14091db02` and replaces the candidate. The `add eax,1` wraps at32 bits. It then returns that candidate. The function dereferences every scanned list entry; unlike the outer loop it has no null-pointer skip.

This is neither lowest-free-ID nor an overflow-safe mathematical maximum. It must be implemented as the same signed comparison and explicit uint32 increment/bit-preserving signed conversion, without C++ signed-overflow UB or positive clamping. Recompute for each encountered nonpositive outer variable against the **updated actual list**, rather than caching one maximum before the outer loop.

| Initial numbers in list order | Final numbers after normal outer pass |
| --- | --- |
| `[0,0,5]` | `[6,7,5]` |
| `[-9,-2]` | `[0,1]` (first result is zero) |
| `[INT_MAX,0]` | `[INT_MAX,1]` |
| `[0,INT_MAX]` | `[INT_MIN,INT_MAX]` (no retry of INT_MIN) |

A null outer entry is harmless when no nonpositive variable invokes the inner scan. If an inner scan reaches a null entry, guard at the original dereference boundary before assigning that current variable; keep earlier child effects. Do not globally validate/normalize the whole list before child dispatch or silently omit nulls from the inner scan. Duplicate Var aliases retain identity: once the first occurrence changes its number, the next occurrence sees the updated same object.

## Source-ready implementation and tests

Root confirmed CP83 archive and authorized exclusive `source/apps/moproj2mst/src/doe/doe_study.cpp` plus new `source/apps/moproj2mst/tests/doe/test_variable_id_finalize.cpp`. Announce this exact set before edits. The implementation should add a private ordered next-number helper and the outer loop immediately before current `readInputTable` status return, and clarify the Operations comment without adding a fake traversal loop. No headers, shared Session/writer/parser, CMake, Qt owner or public ABI change is needed.

Tests should enter `readInputTable` with Operations-only input on existing variables; check positive preservation, sequential updates, negative/zero corner case, list-order-dependent wrap and aliased pointers. Check header-only false status still commits IDs after header assignments, malformed XML leaves IDs unchanged, and earlier nonempty variable/region/constraint guards retain their partial child effects without finalization. Check outer null skip separately from an inner-null refusal. An actual Session-owned active Type2 Study fixture can own concrete plain Var records via its existing owner vector, parse Operations-only through `parseSubProject`/StudyIo, and confirm same-owner ID updates before ordered later sample effects. Source-kind/variable parsing remains guarded; this fixture supplies pre-existing owned records explicitly.

The existing CP83 Operations test that checks Var.ops/index preservation should continue to pass. Its existing variable number was0 and was not asserted; the outer reader will now source-correctly assign it. Operations child itself remains mutation-free; whole InputTable completion has this distinct later effect. Save completion, nonempty variable parsing, output/session application, region/constraint payloads and codec/QFileInfo/public Type2 guards remain unchanged.

Expected new target: `m2m_doe_variable_id_finalize`, CTest `m2m.doe.variable_id_finalize`. Static preflight must verify declaration headers (including qt4path for kSep), Testkit main/macros and resolvable includes. Root builds/tests after source release.

## Compact raw evidence identity

Read-only DUMPBIN used the immutable DLL/EXE paths, `/disasm:bytes` and selected function ranges. No additional raw files were written. Whole selected normal bodies/RET boundaries were inspected; adjacent leaf padding/instructions are not attributed to the procedure.

| Binary / selected range length | Raw bytes SHA-256 |
| --- | --- |
| QtXml firstChild 0x61002750,66 bytes | `a62cf3afe6f8d4df8edf8f1b9ab62b5efaf9571875ad91be29878f898ab81324` |
| QtXml nextSibling 0x61002840,66 bytes | `10fd4c2175e1cce7c3d12bd494b6928850acbbca2d456ccc962903d6d55edda9` |
| QtXml toElement 0x61006120,98 bytes | `0c51bd42e97afaa665b23f10c18b5f4dffebcdf80c808b1ece3551ba0b609508` |
| QtXml assignment 0x61003790,94 bytes | `4e9ae263e94105e931d0718ea2165a3c68f31c39edb993f13a2fb72cb6ee236f` |
| QtXml destructor 0x61003e50,55 bytes | `204a2b072202d0c20d1796271523b1755792fbe383d6d2f656e1a97ec1972104` |
| GUI next-number 0x14091da70,182 bytes | `6773949c529d48eb4907bea84103b3b3501bb53e95a5619b5c81b4018a0a2983` |
| GUI pointer-list view 0x1400d7f20,62 bytes | `3c9177a6b441014d245b4b3f5e49970631bb41b3b9611e89f994d3c793c69f5a` |
| GUI number setter 0x1409ff590,22 bytes | `d1c113d5159f972c143589452f2ad2ebb09e287ecae4697481662b81ce6827cd` |
| GUI number getter 0x140100980,14 bytes | `aa0064cab9b75339e0e750a487979be868dc766f587bcdb05e4df1648bcdc9ec` |
