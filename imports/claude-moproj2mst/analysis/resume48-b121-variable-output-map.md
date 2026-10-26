# B121 variable-list parser and Save output map

## Scope and conclusion

This is a source-qualified continuation of `analysis/resume48-diff-b121.md`, after the common Report QObject prerequisite is assumed to be independently satisfied. It does not relax that prerequisite. The B121 historical pair remains a UI-transition reference, not pure Open–Save. Its expected after-state contains one `Transfer Time`/`TLOC` DOE variable, and the current source has no complete path through that variable-list reader/writer or the active DOE Save sequence.

The map below combines retained original decompiles (`evidence/gui_mo/s1/mdt/DOE/d0.c` through `d9.c`), source `analysis/wave2-doe-findings.md`, current DOE code, and B121 artifact hashes in `analysis/wave2-doe-evidence.json`. Decompiles are retained evidence with the qualifications already recorded there; they are not fresh runtime observations.

## First variable-list parse boundary

For a project that already contains Type=2 plus `<SfDoeStudy>`, `model/subproject.cpp::parseSubProject` checks that `ownedDoeStudy`, the `sub.doe` alias, and `p.doeIo` agree, then calls `doe::readLazyStudy`.

`doe/activation.cpp::readLazyStudy` performs this ordered prefix:

1. `readStudyElement(study, element)` reads the study name (`0x1400d2aa0`).
2. Read `DoeStudies/<name>/Control/DoeInputTable.XML`; on success call `readInputTable` (`0x1400d3970` → `0x14090e080`). The parent return is ignored in original-derived code.
3. For mode 0, 1, 2, call the original-bug-compatible sample-load writer for `Samples_LHS.par`, `Samples_FF.par`, `Samples_USR.par` (`0x1400d7a80`); then read current `DEF_DOE.PAR` (`0x1400d7820`). Reached only if InputTable parsing returns/throws according to the current branch.

In `readInputTable`, XML children are visited in document order. B121's first `DoeVariables` child contains `DoeVariable` immediately. The first variable-specific native reader is `0x14090ee20`; the current implementation guard in `doe/doe_study.cpp` sees nonempty children and throws `M2M_E_UNSUPPORTED` (`DoeVariables reader 0x14090ee20 has no oracle case yet`). This precedes operations-child parsing and precedes the existing-variable reindex/finalization case for an *empty* `DoeVariables`. The actual B121 variable node carries `name="Transfer Time"`, `varId="1"`, `dimension="3"`, `numSamples="3"`; its `Keyword` node names `TLOC`, has nominal `0.1`, `unitType="0"`, `parmType="double"`, parameter `-1`, `NominalData/string` text `TLOC     \t0.1`, and `KwOperations/KwOperation id="1"`. Reading just the header attributes is not equivalent to recovering the variable: original `0x14090ee20` consumes nested Keyword data, and `0x140912a20` later reads operation mappings.

This parse boundary is downstream of the common DOE owner/I/O check and upstream of save-time writer blockers. It is also separate from the Report QObject base and TaskInfo construction prerequisite.

## Native Save call sequence and B121 comparator inventory

The comparator reports **one differing file plus eight missing files**. The differing `COG_P1_B1.moproj` is described after the table; it is not one of the eight missing artifacts.

Original save evidence in `analysis/wave2-doe-findings.md` and retained `d0.c`–`d9.c` gives the following producer map. It describes eligible native branches and their ordering, not a claim that the current converter executes them.

| B121 expected artifact | Original writer path / native source | Content or gate | Current source blocker/status |
|---|---|---|---|

| `DoeStudies/DoeStudy1/Control/DoeInputTable.XML` | `0x1400d3200` → variable writer `0x14090a550`; native per-variable parse/serialization includes variable part `0x14090ee20`, keyword/operation mapping and XML stream. | 707-byte B121 oracle; includes variable attribute tuple, TLOC keyword/nominal, and operation id 1, alongside common headers. | `doe_study.cpp::writeInputTable` refuses nonempty `s.vars` at the variable part of `0x14090a550`; parser likewise refuses the first nonempty DoeVariables node at `0x14090ee20`. |
| `DoeStudies/DoeStudy1/Control/DEF_DOE.PAR` | Current sample writer `0x1400d75a0` / specified table writer `0x1400d7790` → `0x140915360`; called in active Type=2 save and again in `0x1400ba800`. | 1,396-byte mode-0, 101-line table, one sample column; first rows `1\t0.55605\t`, `2\t0.858065\t`, `3\t0.420254\t`. `numSamples=3` is variable metadata and is not necessarily the generated run count. | Sample table formatting is partially ported. The active Save driver does not call DOE save; design/sample state is not reconstructed from a manager. No solver/study run may be invented to get values. |
| `DoeStudies/DoeStudy1/Control/Samples_LHS.par` | Per-design table writer `0x1400d7790` during active sub-project Save; generator may be `0x14091d110` before output. | Same 1,396-byte SHA as `DEF_DOE.PAR` in this oracle; values are fixture output, not proof the sampler can be seeded or regenerated. | Per-mode save table helper exists, but driver/writer order and manager design state are missing. LHS algorithm/RNG is unported; do not hardcode oracle bytes. |
| `DoeStudies/DoeStudy1/Control/COG_P1_B1.DS` | `SfOprDoeOutput` vtable `+0x68` (`0x140e2e730`) → `+0x80` (`0x140e2fcd0`) during active-operation Save → session document `0x140e326a0` → writer `0x140645030`. | 2,013-byte UTF-8 SessionFile XML; populated from active sub-project session and DOE output task. | Current `SaveCurrent` refuses DoeOutput; full DOM refresh/default state via `0x14064a830` remains incomplete. This is distinct from `0x140907a10` `.OPT`. |
| `DoeStudies/DoeStudy1/Control/COG_P1_B1.MST.TEMP` | Selected Type=2 path `0x1400a20d0` → `0x1400ba650` → `0x140131530`. | 7,316-byte DOE sub-project MST; writes normal header, `*!SUBPRJ` block, selected sub-project operation walk/trailer. | Existing `mst::writeSubMst` is not connected; `walkOperations` still stubs a nonnull DOE manager map. It is distinct from main `.MST`. |
| `DoeStudies/DoeStudy1/Control/DEF_DOE_PARSER.INI` | Immediately after `0x140131530`, `0x1400ba650` calls `0x140915eb0`. | 38 bytes, text lines include mode `1`, template basename and main MST basename, newline terminated. | Standalone writer is ported/isolated-tested for the no-variable case, but `0x1400ba650` is not connected to conversion. |
| `DoeStudies/DoeStudy1/Control/DEF_DOE.INI` | `0x1400a20d0` → `0x1400ba800` Type=2 branch → `0x140913270`. Before this, `0x14091d110` ensures/resizes/generates the design rows. | 3,080 bytes. For B121: base DB `COG_P1_B1`, object 1, operation 5, ROI 0, DOE_DBS run entries. | `writeDefDoeIni` has a partial port, but current caller passes explicit `designCounts`; manager/session/object mapping and native design-state acquisition are not connected. |
| `temp.KWD` | Variable keyword parsing path `0x140916530` writes parsed nominal/upper/lower keyword text before reading it into a scratch dimensioned model; the historical file is associated with the earlier Add/Edit Variable action. | 13 bytes, exact retained text `TLOC    \t0.1\n`; SHA `15eefcf3997f7c89507af9d42db2e38a37e38f4f20d7f3b0467bd992455dc404`. | Current helper refuses TLOC/group 6 because native code consumes an uninitialized status. Historical presence does not establish this file is unconditionally written during Save. Preserve it as a variable-edit effect unless a Save path proves otherwise. |

The eight missing paths are `.DS`, `.MST.TEMP`, `DEF_DOE.INI`, `DEF_DOE.PAR`, `DEF_DOE_PARSER.INI`, `DoeInputTable.XML`, `Samples_LHS.par`, and `temp.KWD`.

### Differing `.moproj` and `.DS` path detail

The differing `COG_P1_B1.moproj` is the changed project-file row, separate from the eight missing paths. The before file is 27,053 bytes / SHA-256 `e5b529b6440ebd55bfbb701da26beae94a67c883f35c4c177a9a80caf530e523`; the oracle after file is 34,757 bytes / SHA-256 `fb271b8ef6ab67d14fd215827bccb3ac9e462f4d3ccd7ca878d39efe3d421c45`. The retained comparator gives one file-level difference but no byte offsets or content diff.

The 2,013-byte `COG_P1_B1.DS` is generated on active DOE operation Save through `SfOprDoeOutput` vtable `+0x68` (`0x140e2e730`) → `+0x80` (`0x140e2fcd0`), not through the optimization `.OPT` function `0x140907a10`. This path constructs `<Control>/<base>.DS`, obtains the active-sub-project session document via `0x140e326a0` → `0x1400b5ad0`, updates the DOE manager's values from that document, then writes through `0x140645030(doc,path,0)`. That file writer updates document path, refreshes its DOM through `0x14064a830`, conditionally saves linked documents, opens QFile WriteOnly without Text, selects UTF-8, serializes `QDomDocument::toString(1)`, closes/checks QFile error and returns void on success/failure. The required SessionFile DOM contents and defaults are not completely reconstructed. Current `SaveCurrent` still refuses the DoeOutput task branch. The DOE public Task/Report construction prerequisite is upstream and remains separate.

## Call ordering / first current Save blocker

For any current source attempt to Save an already-open Type=2 project, `.moproj` serialization reaches `subProjectElement` and refuses Type=2/3 before its native `<SfDoeStudy>` / InputTable / sample writers. This is the first Save-time DOE guard. If that common guard were later qualified, `saveInputTable`'s nonempty-variable guard remains next for the B121 variable state. Later sequence items are not interchangeable: main MST Save calls the selected DOE subproject path, then `.MST.TEMP`/parser INI, then Type=2 `DEF_DOE.INI`, sample output and directory steps; active operation Save separately generates `.DS` when a supported DOE output task is present. Original `0x1400a20d0` ignores helper booleans in its valid branch and returns main-MST result, so error ordering/partial output must follow native evidence rather than all-or-nothing output staging.

The historical checkpoint47 comparator does not exercise any of these branches: it opens the no-study `_before` project and has no Type=2 state to serialize. Its missing files represent state created by prior GUI actions. The after-state B121 folder is useful as a distinct training reference, but its expected sample and MST artifacts must not be copied as implementation output or treated as regenerated values.

## Current source decision

The next source-qualified variable implementation boundary, if selected, is the original `0x14090ee20` variable node and nested Keyword record after the current Type=2 owner checks. Its dependencies include variable ownership/indexing, `TLOC` keyword field/parameter extraction, nested `KwOperations` mapping at `0x140912a20`, and source-qualified sample/design state. The full B121 Save requires both the per-project `subProjectElement` Type=2 branch and independent producers for all files in the table. A successful Report QObject constructor alone satisfies none of these DOE Save branches.


