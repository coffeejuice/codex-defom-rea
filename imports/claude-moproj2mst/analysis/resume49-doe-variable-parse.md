# B121 DoeVariable parse: smallest source-qualified boundary

## Conclusion

The retained original decompile identifies the first variable-list dispatch point but does **not** include the callee body for `0x14090ee20`. It therefore proves the XML type/tag discriminator and outer read order, but not the variable node's internal type test, attribute order, nested `Keyword`/`KwOperations` parsing, insertion point, or its own failure mutations. Do not implement B121 variable semantics from the XML sample alone. The smallest safe source increment is the already bounded outer `DoeVariables` dispatch with exact successful earlier-header mutations and parent failure epilogue preserved; a real B121 variable parser still needs its own callee evidence.

Evidence reviewed: `evidence/gui_mo/s1/mdt/DOE/d0.c` (`0x14090e080` caller), `analysis/wave2-doe-evidence.json` and its qualification, `source/apps/moproj2mst/src/doe/doe_study.cpp`, `source/apps/moproj2mst/src/doe/activation.cpp`, and the retained B121 `DoeInputTable.XML`. The evidence qualification says these are retained decompiles/artifacts and the DOE decompiles were not independently byte-validated in that pass; XML is artifact evidence, not a general parser specification.

## Exact outer discriminator and B121 order

Original `0x14090e080` opens the path through `QFile` and `QDomDocument::setContent`. Open failure records the formatted `Failed to open file, %1` application error and returns false. A `setContent` failure closes the file and returns false. On successful parse it closes the file before walking root child elements in document order.

For each child element, original code compares its tag name against the retained Qt string constants. The relevant exact dispatch is:

- tag `DoeVariables` → `0x14090ee20(element, study-name, Study)`;
- tag `Operations` → `0x140912a20(element, Study)`;
- `RegionsOfInterest` → `0x140912b00(element, Study)`;
- `Constraints` → `0x140912d10(element, Study)`;
- common mesh path/original name, do-positioning, and ROI type are read from attributes and immediately stored by setters `0x1409170b0`, `0x1409170f0`, `0x140917150`, and `0x140917130`.

For the retained B121 XML, observed child order is `DoeCommonMeshPath`, `DoeCommonMeshOrigName`, `DoeDoPositioning`, `DoeRoiType`, `DoeVariables`, `Operations`. Thus those four common fields are mutated before the variable reader is called. The `DoeVariables` node contains a `DoeVariable` child; the next `Operations` node contains an `Operation`/`DoeOprVariable` mapping, but it is reached only if the variable reader returns true. The XML sample shows fields `numSamples=3`, `varId=1`, `dimension=3`, `name=Transfer Time`, nested `Keyword` (`TLOC`, nominal `0.1`, type/parameter attributes and data), and a nested `KwOperations` id 1. Those are input observations; they do not establish callee read order or type semantics.

The outer loop assigns each child handler's return to `cStack_34c`. If a handler returns false, it releases the current child element and breaks the loop. **It does not immediately return.** After the loop (including after a false result), the function enumerates the Study's variable collection via virtual calls at offsets `+0x178`, `+0x1a0`, and conditionally calls the vtable slot `+0x1e8` plus helper `0x1409ff590` for entries whose `0x140100980` result is below 1. Retained evidence does not fully name those virtual methods; describe this only as a post-loop variable finalization/reindex pass, not as a specific numeric reindex algorithm. It then returns the last child result.

`readLazyStudy` calls `readStudyElement`, reads the InputTable, and explicitly ignores the Boolean result from `readInputTable`; it then attempts the three mode sample-load operations and the current `DEF_DOE.PAR` read in order. Thus a native `readInputTable` Boolean false after successful file access is not itself a thrown conversion error: common-field writes and any variable mutations already made remain, the outer finalization pass runs, and lazy loading continues. Failures thrown by a child, in contrast, unwind this sequence. Whether the native B121 `0x14090ee20` succeeds or fails on the valid retained row is not established by the artifact-only evidence, and no such result is invented here.

## Current implementation boundary

Current `doe_study.cpp::readInputTable` mirrors the common fields and no-child `DoeVariables`/`Operations` cases. At a nonempty `DoeVariables` it throws `M2M_E_UNSUPPORTED` before calling an implementation of `0x14090ee20`. This conservatively prevents invented variable state, but it is a different control outcome from an original child returning Boolean false: the exception skips outer post-loop finalization and prevents `readLazyStudy`'s later sample-write/read attempts. The report should keep that limitation explicit; do not reinterpret the guard as evidence that the original parser returns false.

The first variable-content guard for B121 is therefore after the four header mutations and at the `DoeVariables` tag dispatch. The general `ownedDoeStudy`/alias/`p.doeIo` check in `parseSubProject` is earlier only when the required owner wiring is absent. The Report QObject constructor prerequisite is a separate earlier lifecycle requirement and supplies none of the missing `0x14090ee20` semantics.

## Recommended bounded helper and evidence threshold

A bounded helper is justified for the **outer** parser only: keep a single ordered traversal that (1) applies known header setters as encountered, (2) hands `DoeVariables` to a per-variable child parser, (3) hands `Operations` to its independent mapper, and (4) on a Boolean child failure performs the observed post-loop pass and returns that Boolean. Use a result type that distinguishes a native false return from an unsupported/unqualified branch exception; do not turn “not reconstructed” into native false. This refactor can preserve known outer ordering without claiming B121 support.

Do not add a `DoeVariable` parser based on the retained row or field names yet. To cross that boundary, source-qualify `0x14090ee20` itself: exact child tags and accepted/rejected variants; which attributes are read and in what order; all allocation/list append and index updates; nested Keyword/geometry branches; return conditions; and state retained on each false path. Separately recover `0x140912a20` for the operation map before claiming the full B121 XML branch. For the B121 keyword path, reconcile the variable node with the `0x140916530` keyword parser and its known uncertainty (TLOC group 6 status is uninitialized in the retained analysis); do not use `temp.KWD` or sample hashes as a substitute for those decisions.
