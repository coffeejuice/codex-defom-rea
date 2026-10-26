# CP83: independent active DOE reader decision

The smallest source-ready branch is the public InputTable reader's **populated `Operations` traversal**. Original `0x140912a20` visits children and returns true without applying operation/variable mappings or accessing a Study. Candidate `readInputTable` refuses any element child under `Operations`. Removing that one refusal admits an existing source-closed decision in the actual lazy Study read, without QWidget, QFileInfo, shared-null, codec or notification ownership.

This report is frozen before the separately authorized source implementation. Only this report was written in the investigation; no source/build/test/GUI/database action occurred.

## Reconciliation of candidate branches

| Branch | Current implementation / earliest remaining dependency | Independent action |
| --- | --- | --- |
| B121 scalar variable | Isolated retained actual-Study prefix owns its concrete record and stops before SetOprHasDoe. Public nonempty DoeVariables still refuses. Receiver population/backend/icon/codec remain unresolved. | Keep those guards. Do not route diagnostic B121 model reads into public completion. |
| B122 geometry variable | Isolated record, binding and writer exist; public variable parser/GEO ownership remain guarded. | No source-closed actual geometry payload increment established by this audit. |
| Sample output modes | Current/fixed-mode0/1/2 helpers and immediate sequence already share the qualified writer. Mode1/2 original bytes are not retained; no missing mode-specific formatter was established (resume80-doe-mode1-next.md). | No duplicate helper or formatter change. |
| DoeOutput task/class data | Actual `.DS`/Session/Report factory prerequisites precede normal populated Task use. Existing-task class data handles zero-region/nonpositive-constraint state; first populated payload and later session application remain guarded. | Keep factory/lifecycle and region/constraint guards; empty OutputList traversal is already implemented. |
| Shape optimisation | Type3 Study ownership remains unsupported; ShapeOptim has a stale Study-name refusal in its folder routine. However original folder routine calls projectDir/QString path work before the name read, and native projectDir uses QFileInfo. | Not the requested independent active increment. Do not claim a full folder path is Qt-ownership-free. |
| InputTable `Operations` | Candidate guards populated elements, but original whole reader is a no-op traversal returning true. Actual zero-variable lazy read can reach it and then continue existing PAR effects. | Remove this one payload refusal; preserve order/status and all other guards. |

## Original proof and exact scope

Retained `analysis/wave2-doe-lazy-input-empty-readers-decompile.json` contains completed original `FUN_140912a20`; SHA-256 `8bbbb410969e3f95a33aa37eb123518c372f55d869e0ab21de9bbc39aab9c2bb`. Its retained export receipt SHA-256 is `aaf38a0a510d73526982c357628b1447c0f39ba2a8a2ba73bcfd7aba50d27af4`. The receipt records a prior read-only Ghidra export; no project or stale endpoint was opened here.

A new read-only DUMPBIN view of hash-verified original GUI range `0x140912a20..0x140912aef` confirms the complete normal procedure through RET. The binary SHA-256 is `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Selected 208 raw function bytes SHA-256: `a7456391b8e777c2586fef701520dde892c610f37fa3f01a730d91aff83d29a8`. Tool output was inspected without writing another capture file, honoring this lane's report-only ownership.

- `0x140912a37` initializes a local status byte to1.
- `0x140912a46` calls firstChild. The loop tests node null at `0x140912a52`, converts to element at `0x140912a69`, and calls its isNull at `0x140912a75`; that Boolean is not used for a branch or payload operation.
- `0x140912a88` advances to nextSibling; assignment/destruction calls update the traversal temporary. No child tag/attribute/name is read, no manager callback is called and no external Study state is stored.
- `0x140912ac7` copies the unchanged status for return; cleanup destroys DOM temporaries and the passed element; `0x140912aef` returns.

Normal traversal returns true for empty, populated, unknown-tag, nested, comment and text children. This is a baseline native no-op, not a proposal to discard unsupported mappings for convenience. Exception/allocator-fault parity inside native DOM temporaries is outside the existing candidate XML seam. There is no source reason to reject element payload only in this function.

The top-level B121 candidate-only model-prefix reader separately interprets Operations into a projection. That isolated parser does not redefine the native public `0x140912a20` behavior. The public increment must not copy those mappings, modify Var.ops, reindex variables, dispatch SetOprHasDoe or create any payload owner.

## Reachable owner and order

`parseSubProject` already handles a Type2 `SfDoeStudy` child through the actual `sub.ownedDoeStudy`, checks its raw alias and configured Project StudyIo, then invokes `readLazyStudy`. The latter updates the actual Study name, reads DoeInputTable.XML, invokes `readInputTable`, performs existing Samples_LHS/FF/USR attempts in order, then loads current DEF_DOE.PAR. Its caller ignores ordinary Boolean read results; a thrown unsupported error stops at encounter and retains earlier effects.

A fixture with empty DoeVariables and a populated Operations list therefore reaches the current candidate refusal without constructing a Report, delivering a variable notification or creating a QFileInfo/icon. The existing path/StudyIo/XML seams are reused. This branch itself reads no path, current codec or global shared-null count. Public conversion still selects its existing SUB index; selecting a Type2 SUB in a focused actual-Session fixture is explicit test setup, not a new C ABI selection feature.

The source change is exactly the removal of the populated Operations guard while leaving `status=true` at that encounter. Subsequent root children retain their current order. In particular: a later positive region/constraint payload still clears its known collection/count then guards; an earlier nonempty DoeVariables still refuses before Operations is reached; invalid XML still returns false. A successful Operations traversal is not public Save success. The later actual Type2 Save still writes immediate XML then stops at its existing refresh-owner boundary.

## Authorized implementation and focused tests

Proposed exclusive source set, to announce before edits: existing `source/apps/moproj2mst/src/doe/doe_study.cpp`, new `source/apps/moproj2mst/tests/doe/test_operations_reader.cpp`. No header, Session, parser, CMake, Qt owner, public ABI or shared writer edit is needed. Root has authorized this successor after this report freeze and owns the build.

Focused tests should cover:

1. Populated Operations containing plausible DoeOperation mappings, arbitrary nested elements and raw text/comments returns true while preserving actual existing variable ops/index and Study sample/region/constraint fields. A normal return must not be read as mapping application.
2. A real ConversionSession/live Project/owned active Type2 Study and RecordingStudyIo lazy read reaches populated Operations, continues the three existing sample write attempts in order, and loads the seeded current PAR. All input paths/overlay misses stay beneath the configured test scratch; record baseline seed writes separately from native-represented lazy effects.
3. Saving that same actual owner through the shared writer reaches immediate XML and the precise existing refresh-owner guard, preserving caller-owned XML journal/normal-return/boundary observations and prior DOM. It does not claim Save completed or native file durability.
4. Nonempty DoeVariables and populated region/constraint children still refuse at their original encounters; preserve earlier per-branch clear/partial effects. Populated Operations alone must not suppress those later guards.

Expected new discovery target `m2m_doe_operations_reader`, CTest `m2m.doe.operations_reader`. Use active Testkit checks and a main; resolve quoted includes before release. Root runs focused and grouped checks after the source freeze. No new original runtime acceptance claim follows from these synthetic, source-backed branch fixtures.
