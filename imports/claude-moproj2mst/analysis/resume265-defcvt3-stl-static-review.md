# CP265 DEF_CVT3/STL review — HOLD on converter scope

The inspected wrapper/child-prefix facts are sound static research, but the proposed next implementation/capture is not established as a dependency of the user's single-project MOPROJ→MST Open→Save→Close converter. The actual caller supplied by CP265 belongs to the separate DEF_PRE console File Input menu. Retain this evidence as broader DEF_PRE material; do not release a converter implementation or original-runtime capture on that caller alone.

Only this review was written. No source/test/build/shared state, original process, GUI control or mutable REA was changed or run.

## Static correctness

Independently recomputed the artifact identities: original x86 PE32 `DEF_CVT3.com`, 36,864 bytes, SHA-256 `8dd8b87c8b24cec933e98fee31946103d95cb0c09a6cb28119131aacc04652fe`; original `DEF_STL.exe`, 233,472 bytes, SHA-256 `7a4d8fe41b94709ac4b0094ce9a6ccb85cf0ae2c10fdc6785085c0b961e27a9a`. No product version is established for these utilities; they are not reconstructed DEF_PRE instances.

Independent bounded DUMPBIN inspection of wrapper `401170..401197`, `401c00..401d11`, `402d60..402e1f`, child `404900..404b68`, and writer prefix `4038c0..403905`, plus PE-mapped literal reads, corroborates the important claims:

- `SCRATCH.KEY` is tested by successful `fopen(...,"r")`; only a readable prior file triggers the ignored unlink result. That precedes the STL option comparison.
- The STL branch passes prefix and `DEF_STL.EXE"` to the helper. Literal `4064d4` is a quote plus NUL; the helper overwrites the initial concatenation and appends quote/prefix/suffix/quote, yielding the reported two leading and two trailing quote bytes. It calls the imported `system` slot and does not read the third argument. Windows shell interpretation remains a runtime question.
- Child config and prompt order agree with the report: filename token, object integer, tolerance float; first filename byte `0` exits; nonpositive object is reset; negative/zero finite tolerance receives the documented handling. Config-open failure after the initial INI test joins processing rather than ordinary prompting. Scanner failure/EOF/locale/unusual floating-input behavior is not recovered by this prefix.
- The selected writer requests literal `SCRATCH.KEY` with `"wt"` and reports a null-open failure. This is not proof that processing reaches that writer, complete KEY grammar, successful output, or reliable failure propagation.

No static contradiction was found in those selected facts. This review did not independently re-decode every startup/argv/CRT/helper instruction or the complete child geometry path. The CP265 report properly leaves the processing/output algorithm, shell/stdin behavior and actual original execution unproved. The existing `fi_stl_def_cvt3.json` explicitly calls its extra prompt answers guesses; its blank lines are not an accepted numeric-input contract.

## Actual caller belongs to DEF_PRE's console session

The established current source chain is:

```text
source/port/def_pre/main.cpp::main
  -> def::pre::main_m
  -> [only when no utility option handled the invocation] main_session
       source/port/def_pre/src/menu_shell.cpp:600–632
  -> menu_loop
       main menu option 2
  -> menu_file_input
       File Input option 5
  -> launch_dir0("DEF_CVT3 KEY STL")
       menu_shell.cpp:464–469
  -> def::cmd::launch_program(1, text, 0)
  -> if readable: read_key(...,"SCRATCH.KEY"); hit-return
```

This is a real production caller for the broader DEF_PRE console program. It is not a converter actual caller. Invoking its menu requires a distinct application entry/session and user input unrelated to opening an existing `.moproj` in the converter.

`source/apps/moproj2mst/CMakeLists.txt:72–96` adds and links `def_pre_core` because the converter embeds recovered engine helpers. Source uses `def_pre/menu56.hpp`, for example `GuiModel::objectCount`→`step_count_objects`, object creation→`step_first_deformable`, Cogging save→object count/first-rigid helpers, and viewer table finalization→`step_current_object`. Those helper calls do not enter the console menu. Linked library/object-code presence, shared launch infrastructure, or an executable string is not an executed call edge into `menu_file_input`.

Read-only source search found `menu_file_input` called from that DEF_PRE menu loop, `menu_loop` from `main_session`, and `main_session` from DEF_PRE's own main. No reference from `source/apps/moproj2mst` into those session/menu entry points or `DEF_CVT3`/`DEF_STL` launch text was established. This is an audited current-source reachability result, not a universal proof that arbitrary future/input-generated command strings can never name an external executable.

## Converter path checked

The selected converter entry is `src/dll/abi.cpp:146–154` (`m2m_convert_w`) or its UTF-8 sibling→`m2m::convert` in `src/core/convert.cpp`. The implementation constructs its own `ConversionSession`/Project, calls `openProject` around line 241, activates the subproject, performs Save/current-operation/project/property/MST work, and retains its separately recovered session/Close qualification. `src/moproj/read.cpp::openProject` performs suffix/project/DB/XML/remesh work. None of those inspected paths dispatches the DEF_PRE File Input menu.

Two superficially similar converter paths do not establish the missing edge:

- `src/engine/loader.cpp:510–511` guards a real operation-inheritance path whose original `14005bd30` runs **FIXGEO.EXE and DEF_GEOPRIM3.EXE** for 2D→3D inheritance. It is a distinct converter dependency with its own existing evidence/guard requirements; DEF_CVT3's STL console wrapper does not resolve it.
- `src/mst/emit_b_doe.cpp:184` emits `GFREAD` plus an output geometry filename into MST text. Emitting a future script command is not invoking DEF_PRE's menu or the external STL converter during this Open/Save/Close occurrence. Internal STL/GFREAD readers elsewhere in `defcmd` likewise are not evidence for this external wrapper's entry point.

## Required scope correction and next action

Revise CP265's opening “imported converter” wording to identify the **original DEF_CVT3 external utility**. Revise “selected next leaf” to “broader DEF_PRE console boundary, not currently established in converter lifecycle.” Preserve the useful static facts and guessed-fixture qualification, but withdraw the File Input 5 implementation/capture as the converter's selected successor.

The exact scoped next action is a dependency disposition, with no production edit or GUI capture: retain this report as external-utility evidence and return the converter worker to an established ABI/Open/Save/Close caller. Do not treat original File Input 5 runtime, missing-child experiments or full STL parsing as a blocker to converter progress. That requested occurrence would validate the separate console route, not repair the missing converter edge.

To reopen this utility as converter work, first supply an immutable source/native call chain from the selected original DEF_GUI_MO MOPROJ Open/Save/Close or operation-load/save occurrence to an actual **DEF_CVT3 KEY STL** launch, including the responsible operation/property slot, command construction, launch caller and subsequent SCRATCH.KEY consumer. Bind it to an existing eligible project/fixture and selected lifecycle stage. A shared engine library, generic launcher or separate DEF_PRE menu test is insufficient. If such evidence is absent, preserve the current “no converter dependency established” result rather than inventing reachability or expanding scope.

HOLD therefore applies to CP265's proposed converter next action, not to the inspected static wrapper/child-prefix evidence. No executable build, deployment, new GUI witness or mutable analysis database is needed to make this scope correction.
