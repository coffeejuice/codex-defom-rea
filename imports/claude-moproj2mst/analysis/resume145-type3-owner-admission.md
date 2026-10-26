# CP145 Type3 optimizer owner and Save admission crosswalk

## Finding

CP144's file parser remains a private value projection: it mutates an explicitly supplied `Type3OptimizerScalarState`, but it is not attached to a Project, SubProject, loaded optimizer, or ShapeOptim operation. No source-qualified actual-owner integration is safe yet. Keep the current Type3 Open and Save guards. The smallest supported cut is the existing standalone private parser/test contract; the next actual-owner change requires constructor/lifetime and consumer-state evidence, not an alias to the Type2 DOE Study.

## Original owner and Open path

For `DEF_GUI_MO_64.exe` SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`, retained `analysis/resume125-type3-optimizer-owner.md` and the original L5d/DOE decompiles establish:

- `FUN_1400b5470` creates two distinct objects for a Type3 SUB: the DOE manager at `SUB+0x20` and a separately allocated `0x50`-byte optimization object at `SUB+0x28` via constructor `0x140917ff0`. The `+0x28` pointer is published only for Type3.
- In the Type3 `SfDoeStudy` parse arm, native code obtains both objects, parses the XML manager and InputTable, constructs `<Control>/<project-base>.OPT`, then calls `FUN_140908630(optPath, optimizer+0x28)`. The parser's bool is stored but not used as an abort test in the retained caller path.
- The Type3 parser takes the optimizer pointer as an explicit argument. Its stack QFile/QTextStream are local; they do not own that pointer. On successful file open it updates the optimizer before reading the separate header, dispatches lines in order, closes stream/file, and returns the observed false byte. CP134 records this source order.

This establishes the slot and call graph, but not constructor defaults/body, destructor/deletion path, nested row container ownership, or the actual state written by `.OPT`. `analysis/resume125-type3-optimizer-owner.md` records the missing constructor/parser bodies and vtable/getter targets. The CP144 value-state defaults and vector fields cannot be treated as a demonstrated replacement for the opaque native object at `+0x28`.

## Candidate owner and Open boundary

Current `SubProject` contains `doe` and `ownedDoeStudy`; comments and `makeSubProject` bind these only for `type == 2`. A Type3 SUB has neither a Type3 optimization owner nor an alias field. `parseSubProject` handles `SfDoeStudy` for Type2 through the owned Study, but throws `M2M_E_UNSUPPORTED` for Type3. This correctly preserves the distinction between manager `+0x20` and optimizer `+0x28`. Wiring CP144 into `ownedDoeStudy`, `doe`, a Type2 `Study`, or `Project::doeIo` would attach the state to the wrong object and lifetime.

CP144 also admits only fully-read BOM-free ASCII/LF files, preloads the complete byte stream, sets the value-model `value48` only after a successful open, and reports read errors as `ReadFailureUnsupported`. It composes the private scalar/NUM_CON/REGIONS helpers for that finite profile. The original parser is streaming and can retain completed mutations before an I/O error; CP144 explicitly makes no claim about that late-error prefix. This restriction is suitable for the isolated candidate reader, not a general native Open replacement.

## Candidate ShapeOptim / Save path

`ShapeOptimClass::loadClassData` in `src/ops/report.cpp` returns for missing project/sub-project and then refuses `sp->type == 3` before mapping ShapeOptim properties from the loaded optimizer. The broader Type3 viewer/engine path is also guarded in `src/engine/shapeoptim_load.cpp`. The native later consumer calls `SUB+0x28` getters for objective/list values, produces ordered `listF0_` names, and uses them to build GEO paths; CP144 only populates a private scalar/row snapshot and provides none of that getter or list behavior.

On Save, `saveCurrentOperation` has a ShapeOptim-specific sequence: property synchronization, candidate `saveAfterFiles` (the native vt+0x80 work), then `saveOperationFiles` (native vt+0xe8 work). `ShapeOptimClass::saveAfterFiles` currently copies StopCritGroup values to candidate fields and then refuses Type3 because the optimization Study is unmodeled. That refusal occurs before the subsequent Opt-folder/ROI `.GEO` save path. Removing it after merely attaching CP144 would allow a Save with no native optimizer property update, no populated `listF0_`/geometry owner, and incorrect file continuation/order.

The retained native ShapeOptim Save slot map in `source/docs/re/moproj2mst/mdt/L7/spec.md` says vt+0x68 runs vt+0xd0, vt+0x80, then vt+0xe8; vt+0x80 transfers stop criteria and updates the optimization study, and vt+0xe8 creates `DoeStudies/<study>/Control/Opt` and writes one GEO for each ROI object. The current code still explicitly guards Type3 in `saveAfterFiles`, and the later Type3 GEO writer/reader remains refused. The private parser alone cannot authorize any of those effects.

## Smallest safe integration and exact missing dependency

No public or SubProject-owner integration increment is source-closed. Keep CP144 invocable only in its private tests/explicit value-state seam, and keep `parseSubProject`/`ShapeOptimClass` Type3 guards. A future owner integration needs, at minimum:

1. Immutable source for constructor `0x140917ff0`, optimizer vtable/member defaults, destructor and Type3 SUB teardown, proving initialization and lifetime for a candidate owner distinct from `ownedDoeStudy`.
2. Full bounded `.OPT` parser effects for supported numeric and row text profiles, including its malformed/conversion and late-read failure prefix; a separate candidate result must preserve the native parser's false bool.
3. Source-backed Type3 `SfDoeStudy` XML/name/path association so the candidate builds the exact `.OPT` path and attaches the resulting owner to the same SubProject.
4. Getter-to-ShapeOptim field/list mapping plus native Save vt+0x80 and vt+0xe8 effects, including the ordered GEO write contract, before relaxing either public Type3 guard.

If the next work must remain bounded without new native evidence, the only safe change is additional tests of the private file parser's admitted profile and refusal boundary. Such tests do not establish actual-owner Open/Save parity.

## Qualification

This crosswalk uses retained original decompiles/specs and current candidate source only. No source, test, build, GUI, original process, or mutable REA action was performed. No reconstructed DEF_PRE_64 artifact is discussed.
