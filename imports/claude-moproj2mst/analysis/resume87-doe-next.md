# CP87 proposal: exact variable-child dispatch

The smallest next integration is to narrow the public DoeVariables refusal to **actual direct `DoeVariable` rows**, rather than every element child. Original `0x14090ee20` ignores other children, then performs the existing reindex and outer ID finalization. This uses the actual Study/Var owners already present and introduces no QWidget, QFileInfo, codec, geometry or notification dependency. Recognized variable payloads remain guarded before parsing/allocation.

This is read-only preparation while CP86 source is held. Only this proposal was written; no source/build/test/GUI/database action occurred. Source implementation requires root's archive/release and exclusive assignment.

## Original evidence and exact behavior

Original GUI SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Retained `wave2-doe-lazy-input-empty-readers-decompile.json`, SHA-256 `8bbbb410969e3f95a33aa37eb123518c372f55d869e0ab21de9bbc39aab9c2bb`, contains completed `0x14090ee20`:

- FirstChild/nextSibling traversal considers immediate children in order.
- At `0x14090ee9f` it converts the current child to an element. Non-elements skip the payload block.
- At `0x14090eef6` it compares the direct element's tag with literal `0x1427bb508`. An immutable PE read in this lane verified that literal is exactly `DoeVariable`.
- Only a matching tag enters the large attribute/constructor/keyword/geometry body beginning `0x14090ef30`. The entire body ends before sibling advancement `0x140912858`; an unknown element is not recursively searched for nested variables.
- At exhaustion it invokes manager vt+0x1e0 reindex and returns its initially true status after an empty deferred-link loop on the no-recognized-row branch. CP85 implements that actual-record reindex; CP84 implements the distinct outer number pass after remaining child dispatch.

Thus a list containing unknown direct elements, case variants, text/comments/CDATA, or a nested DoeVariable under an unknown wrapper has zero recognized rows. It behaves like the currently admitted empty-element list. A direct exact DoeVariable anywhere in the list is still a real payload row and must hit the existing unsupported boundary. Seeing ignored siblings before that row does not justify returning success or finalizing early.

Native DOM/QString ref/allocator behavior remains outside the existing value seam, as in CP83–86. Do not create a traversal ledger, inject artificial failures or claim that filtering names reproduces native temporary ownership. This increment qualifies the row decision and existing persistent finalization effects only.

## Current source and next change

`readInputTable` currently tests `!e->childElements().empty()` before calling `reindexExistingVariables`. Replace that blanket admission test with an ordered direct-child scan that skips nonmatching kinds/tags and throws the same existing DoeVariables payload error at the first exact direct `DoeVariable`. Only after the whole no-recognized-row scan finishes should the existing reindex call run. Keep the outer ID pass at the end of the top-level reader.

Do not invoke B121/B122 detached/retained readers, parse headers, allocate a record, set sample invalid flags, resolve LinkTo, update operation mappings or dispatch SetOprHasDoe. There is no new owner interface. Actual pointers/index/number fields remain on the same Study; recognized payload construction remains incomplete.

Order matters for existing partial effects:

- Unknown-only children allow existing-record indexes to finalize, then later top-level children execute, then normal outer completion can repair numbers.
- A matching direct variable after unknown siblings refuses before reindex and outer number repair, retaining only previously completed top-level effects.
- A later populated Constraints guard keeps earlier reindex/region-name effects but still prevents the outer ID pass.
- Repeated no-row DoeVariables siblings each reach reindex; the outer number pass still occurs only once after all siblings.

## Branch reconciliation after CP86

Region-name loading is now source-backed in the existing canonical Study name projection. Geometry use remains behind the output's first-region payload guard; a name or native append's null payload argument is not a geometry readiness token. Populated Constraints has only a count projection and still needs a concrete record/owner contract before payload import. Normal DoeOutput/Report/task and selected-session application remain guarded. Type3 optimisation Study construction and ShapeOptim file/geometry paths retain their owner/QFileInfo dependencies. The variable no-row dispatch is smaller and source-closed without any of those owners.

## Proposed exclusive files

After root confirms CP86 archive:

1. Existing `source/apps/moproj2mst/src/doe/doe_study.cpp`: narrow only the DoeVariables child-dispatch guard, retaining its error and finalization ordering.
2. New `source/apps/moproj2mst/tests/doe/test_variable_child_dispatch.cpp`: focused dispatch/owner/order tests.
3. Existing `source/apps/moproj2mst/tests/doe/test_empty_variable_reindex.cpp`: remove only its now-obsolete `<Unknown/>`-only refusal expectation. Its direct nonempty DoeVariable and other payload guards remain intact.

No header, CMake, Session/shared parser/writer, Qt owner, public ABI or original fixture edit is needed. Inspection-time source hashes remain CP86's frozen `doe_study.cpp` `f193ee8eeb0de9b7a4510edbde9909c727df662758a1d0d96a4bf6c40cab5fc5` and empty-variable test `f3ab3583044e49e33bb6dbd5c3bcfb416ad07eb51ad79145d75750df3ff5254c`.

## Focused acceptance expectations

Expected target `m2m_doe_variable_child_dispatch`, CTest `m2m.doe.variable_child_dispatch`:

- Unknown/case-variant/raw-only child lists return true without creating variables or prefix owners. A nested recognized tag under an unknown wrapper is ignored; a direct exact match still refuses.
- Existing concrete records retain identity and receive CP85 indexes then CP84 IDs for a no-recognized-row list; ops/links/sample state stay unchanged.
- Ignored siblings before an exact row do not cause early finalization; headers already reached remain, index/number/sample state stays unadvanced.
- A later constraint guard preserves earlier no-row reindex/region-name state and prevents IDs; repeated lists do not repeat the outer number pass.
- Actual ConversionSession/live Project/active Type2 owned Study through `parseSubProject` reads a no-row list, continues sample effects in existing order, and creates no variable notification/task/UI effects.
- Same-owner Save with zero variables serializes the existing Study data then retains the exact refresh-owner guard; ignored unknown metadata is not promoted into variable records or successful Save. A pre-existing-variable fixture still has its current variable-dimension reconciliation guard.

Use explicit test scratch/declaring includes and active Testkit main/checks. Root owns builds and existing reindex/ID/region/Operations regressions after the source freeze. Source-backed synthetic input establishes this bounded dispatch; it is not fresh original GUI or public conversion parity evidence.
