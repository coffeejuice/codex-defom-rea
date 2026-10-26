# Active DOE notification to Save output

The integrated path to recover next is the actual Type2 `.moproj` Save branch, with its owned Study and shared Session/current-project binding. Another backend helper alone cannot produce its files. The selected original output path is `subProjectElement 0x140099040 -> manager saveInputTable 0x1400d3200 -> dimension reconciliation 0x1400d2b60 -> DoeInputTable.XML writer 0x14090a550 -> signalDoeTableRefresh`. Both the file effect and the later message matter.

Only this report and five uniquely named bounded PE evidence files were written. No source, shared interface, CMake, build, executable test, GUI, original application execution or mutable REA state changed. Public B121 remains guarded.

## Connection to CP60

CP60's prerequisite registry models the distinct `DiagModelCanAddItem` registration/removal before an actual model backend assignment. It does not install complete `SetOprHasDoe` population, publish a backend witness, finish live icon ownership or finish lazy variable parsing. With actual owners/population/backend effects eventually supplied, a successful scalar variable load still must deliver **every** operation notification, re-query the keyword afterward, perform selector/type/text setters, and reindex before Save consumes the Study.

The identified live `SetOprHasDoe` receiver allocates/replaces a diagram QIcon; it does not directly produce DoeInputTable.XML. This does not justify omitting it: actual callback/ownership ordering remains required before the parser continuation and Save. A constructor-null backend or an isolated diagnostic commit is insufficient at lazy delivery.

## Verified Save prerequisite: dimension reconciliation

`0x1400d3200` first calls **`0x1400d2b60` at `0x1400d3227`**, before constructing the output path or calling the XML writer. Its complete selected native body is retained in `resume63-doe-backend-dimension.asm`.

`0x1400d2b60` returns immediately when the manager variable list at `+0x38` is empty. Otherwise it snapshots that list, resolves the global main-window context (`0x143d766b8`), dynamic-casts the actual main window, then obtains its model manager (`0x1400133a0`), current Project (`0x140045780`) and operation manager (`0x1400a58b0`). Missing pointers skip the remaining reconciliation and return; they are native branches, not permission to use a substitute Project.

For each non-null variable in snapshot order:

1. Read operation ID at **Var.ops index 0**, via `0x140816ae0(var,0)` at `0x1400d2ca1`; an unavailable index returns -1. Resolve that ID in the actual current Project's operation manager with `0x140047dc0` at `0x1400d2cba`. A missing operation skips this variable.
2. Get dimension through **`0x14005f0f0` at `0x1400d2cde`**. That helper defaults to 3 if the operation has no base object (`0x140054e90`); otherwise it invokes the actual base object's virtual **`+0x190`**. Do not derive this value from a guessed class-name or the active Project geometry flag.
3. Compare with the variable's `+0x24` dimension at `0x1400d2d00`. **Equality jumps to the next variable** at `0x1400d2d07`, skipping keyword parsing and temporary data effects.
4. Mismatch first calls `0x1400d8040` at `0x1400d2d25`, whose leaf writes **Var.dimension `+0x24`**, then obtains the keyword list. Empty list or null first keyword skips further processing while retaining the dimension change.
5. For a non-null first keyword, collect its nominal/upper/lower strings, keyword name, parameter name, entity IDs and selector. Convert/format these arguments and call **`0x140916530` at `0x1400d30d2`**. This body contains no function/enum flag test before that call. A changed-dimension scalar keyword can therefore reach this parser too.
6. After return, apply parsed data only if both upper/lower result pointers are non-null; nominal application is additionally conditional on its pointer. Clean up locals and continue variable order. Earlier dimension and parser/file effects are not rolled back.

This refines the scope of the earlier reader result: **ordinary scalar-double XML reading** skips the function-only temp.KWD parser. It does not establish that **Save after an operation dimension change** also skips it. The old historical temp.KWD file remains an editing artifact rather than unconditional fresh Save evidence. The reached `0x140916530` temporary-file/parser/failure contract remains separately qualified, including the retained TLOC status uncertainty.

## Actual file and message order

After reconciliation, `0x1400d3200` constructs `<DoeStudies>/<study.name>/Control/DoeInputTable.XML` and calls **`0x14090a550` at `0x1400d349c`**. Its writer entry first calls **remove-empty-variables `0x14091c820` at `0x14090a5a9`**, before QFile construction/open. Thus cleanup can mutate the manager before a later file-open failure.

The QFile open receives WriteOnly|Text (2|0x10), verified in the entry window. Failed open sets the original global error string and returns false; it does not serialize the document. On successful open, the writer constructs the InputTable DOM, serializes it through QTextStream using `QDomDocument::toString(1)`, closes QFile, and checks `QFile::error`. Error zero sets the result true; nonzero preserves false and sets the global failed-write error. The writer windows here verify entry/open and final output/error ordering, not the full 15,145-byte variable/constraint DOM body.

Back in `0x1400d3200`, **writer Boolean is saved but does not gate the next call**: global messenger `0x1402cf3c0` receives exact **`2signalDoeTableRefresh()`** at **`0x1400d369d`**, then the wrapper returns the saved Boolean. Literal `0x142166b90` confirms the key. Thus a failed open/write can still be followed by this notification, and a later callback refusal must preserve already committed file/error/cleanup effects. No notification owner/population closure for this key is established by CP60's different key.

The original Type2 project-element caller proceeds to current/per-design sample writers after the input-table call; it does not turn its Boolean into an all-or-nothing Save. The earliest candidate Save guard remains before all of these native study effects in `moproj/write.cpp::subProjectElement`: `project file: DOE / optimisation study elements (0x1400d29d0, 0x140907a10)`. Current standalone `saveInputTable` separately refuses nonempty variables before reconciliation, and its no-variable implementation lacks this original post-write notification. Those standalone routines are not an integrated active-study Save implementation.

## Next integration boundary and tests

Root should prioritize the shared Type2 `subProjectElement` orchestration: require the actual Project-owned SUB/Study and existing StudyIo/Session notifier/current-project manager, append the native study node, then invoke original study output phases in source order with immediate effects. Keep Type3 independent. Do not simply remove the common guard or copy the historical files. A targeted audit of the native `0x140099040` Type2/3 arms should close exact caller order, arguments and ignored-return behavior before source wiring.

For this selected reconciliation/output branch, integration tests must demonstrate:

- Equal actual operation/variable dimensions skip `0x140916530` and retain variable state; unknown actual current-project/base-virtual ownership refuses at its reached boundary rather than using copied IDs.
- Mismatch stores dimension before keyword-list/null-keyword exits or parser refusal, and considers only the first referenced operation and first keyword. Retain prior mutations and temporary-file effects.
- Remove-empty cleanup precedes file open, including a failed open. Writer failure preserves native global error and still reaches DoeTableRefresh; a notification refusal preserves earlier output.
- One actual ordered effect receipt spans study-node append, input XML, current/per-design PAR outputs and later main Save outputs. Ignored native Booleans must not become early cancellation or output rollback.
- Public B121, backend ownership/population and active operation `.DS` guards remain until their distinct dependencies are actually integrated. CP60 map registration is not a substitute for these paths.

No successful active B121 Save, native callback closure, scratch parser equivalence, sample regeneration, complete DOM serialization or fresh independent parity is established here.

## Bounded evidence receipt

Original PE SHA-256 was rechecked as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. New captures use Microsoft dumpbin 14.44.35228.0. `resume63-doe-backend-pe-receipt.json` (SHA-256 `43ea6bb0b2e92465b4c5d6cfd9a25067e8f890c634d99a96f16f712802998b68`) identifies four bounded captures totaling 59,312 bytes:

| Capture | Native range | Scope / body SHA-256 |
| --- | --- | --- |
| `resume63-doe-backend-dimension.asm` | `[0x1400d2b60,0x1400d31f3)` | complete selected unwind region / `4ba0640f63710b92171f3431d2b72268c5f38e22ac5d11f4ed24a4fa888bf8d3` |
| `resume63-doe-backend-save-wrapper.asm` | `[0x1400d3200,0x1400d36d9)` | complete selected unwind region / `d46f4a5e25473ffa59c1cd951bb356ac27cb7b06451f2bc6957d3751777a300b` |
| `resume63-doe-backend-writer-entry.asm` | `[0x14090a550,0x14090a700)` | selected entry/open window, not full writer |
| `resume63-doe-backend-writer-tail.asm` | `[0x14090de68,0x14090e079)` | selected stream/close/error window, not full writer |

The writer's full selected unwind region `[0x14090a550,0x14090e079)` hashes to `c4876c6e6716ac05c064c859586cb8344a866791faf15494a57c324105f881be`; only the specified windows were newly disassembled/retained. Supporting leaf getter/store views were bounded read-only terminal selections, not a whole descendant callgraph capture.

Retained DOE decompilation `evidence/gui_mo/s1/mdt/DOE/d0.c` hashes `1ec4144d1bcdc367eeddf8add4f2368acfac4103d15f3d582564f5fff3a5e2f3`; `d1.c` hashes `dc1662ab5bde9b414407337b95a4d9f770efdbdf4d4f48b197c4d036575684b0`. Their provisional prose is not used as a runtime oracle; key calls/branches above are independently verified by PE/disassembly. Current `moproj/write.cpp` hashes `6df9a21b4dd9847b50ad17ddc3af48252ed49ce11369b36267abbc6f614d3299`, and `doe_study.cpp` hashes `988b9599de5d47426bef8fd34554b436636e0af2932ecbd1f43e1458629532e4`.

Token measurements are unavailable.
