# CP219 negative-ID side-effect analysis

## Finding

The paired run has a clear filesystem distinction after the bounded five-second observation: the positive-ID control adds an empty root `Lubricants` directory, while the `[-1, -2]` variant adds three root-level `ObjPropertiesNNNNN.KEY` files. The variant files are real downstream side effects, not merely a parser receipt. They have the same byte lengths and SHA-256 values as the corresponding files already present under each `Operations/TaskNNNNN` directory. This is evidence that changing the IDs changes at least one later path decision or path interpretation. It does not establish that either case completed the full Open workflow, or identify the operation-directory creator as the cause of the `Lubricants` delta.

## Run and exact outputs

The raw pair is recorded under `analysis/resume217-plain-run-2026-10-26T14-38-49.491Z/`; the launch binding is in `analysis/cp219-launch-receipt.json`. The executed runner hash was `F8E133E17B69E6B3E4F05461A05C1F743CD7A672500881BF57141A428CE5CB77`, distinct from the reviewed source hash `84370534E343D03DEE21168A9DA3F77D0CD2BBF180FDDB3515F7C6E1B9C20467`. The receipt states the transient enabled copy is absent and the launch guard restored. Both cases used the same original executable SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, same source project and helper hashes, and independently cloned project roots.

`effect-comparison.json` reports no shared additions, no removals, and no changed pre-existing entries:

| Case | Added paths | Meaning in the observed tree |
| --- | --- | --- |
| `control` (IDs `1, 2`) | `Lubricants` | Empty root directory only |
| `negative_ids` (IDs `-1, -2`) | `ObjProperties00001.KEY`, `ObjProperties00002.KEY`, `ObjProperties00003.KEY` | Three 169-byte files at the project root |

The root-level negative-case files have SHA-256 values, respectively, `021caff357de2d72e5043483a36c35ef5848233258ba55530c0fde9499a0b713`, `26e9d62de1e0faeb32262d6ef2897a50530452f3246089bdcedb2c5cf6ceac2e`, and `4e19975afc35c73512b5a1906044799959d5cb7113a1e104039d5b026f2360e8`. Their same-named counterparts in both `Operations/Task00001` and `Operations/Task00002` have identical lengths and hashes. Directly hashing the retained `C:\PROBLEM\p217n` files confirms the manifest values. The payloads are the corresponding slot records (including `OTPRNG`/`OPSTOP` slot numbers); the root files are byte-identical copies of existing task files, not modified task files.

The captured process events show creation suspended, one resume, and the full 5,000 ms observation ending at `terminate_command`. Neither run has a verified natural exit: the runner then requested exit code `57857`, verified that forced cleanup, closed handles, and obtained a zero-row process-absence snapshot. `gui_control_acquired` is false. The case receipts have no flow/write/stream/parse errors and mark inventory/baseline integrity complete. These are bounded process and filesystem observations; forced cleanup and PID absence do not demonstrate a completed Open or a healthy GUI state.

## Source-qualified interpretation

The source records the original parser gate and creator chain separately from this experiment. `analysis/resume218-next-source-increment.md` and `source/docs/re/moproj2mst/s1-01/spec.md:315-318` qualify the operation-project directory gate as effective `id >= 1`, with ordered requests for `Operations`, `Objects`, `Materials`, `Equipments`, and `Lubricants`; the retained native chain is `0x140050820 -> 0x140051190 -> 0x14008c630`, with helper path attempts at `0x140081afb0`. `source/apps/moproj2mst/src/model/opinfo.cpp` documents `makeTaskDir` at `0x140051190`, the task-path helper, and that project-folder setup is associated with `0x14008c630`. The filesystem comparison observes only a resulting directory, not any of those calls, their arguments, attempt order, return values, or failure behavior.

The candidate parser source is useful for distinguishing another path mechanism. In `source/apps/moproj2mst/src/model/operation_rescue.cpp`, `parseOperationList` passes document ordinals, and `parseOperation` substitutes the ordinal only when the parsed ID is zero, creates the operation, then stages directory decisions. The staged collector excludes negative IDs. In the same parser, nested `<Objects>` and operation key/blob children continue through their own parsing route. `source/apps/moproj2mst/src/model/props_keys.cpp` documents `parseObjPropertiesKey` -> `storeBlob` (`0x140093ab0` -> `0x1400937b0`): the blob write path comes from `objPropertiesPath(op, slot)` and native mode attempts the parent mkpath before a truncating write. `source/apps/moproj2mst/src/model/opinfo.cpp` defines `objPropertiesPath` (`0x14005eb10`) through `numberedTaskFile`; `source/apps/moproj2mst/src/engine/loader.cpp` separately uses `slotFile(op, "ObjProperties", s)` for loading. Those qualified paths show that ObjProperties file writes/loads are distinct from the project-folder collector/creator.

The root-level location of the three negative-case files is consistent with a later operation property path resolving without its normal task-directory prefix under negative IDs, while the existing task-folder copies remain untouched. Because the run provides filesystem deltas only, it does not reveal whether this arose during XML blob parsing, a later save/load phase, path normalization, or another downstream step. It is therefore a downstream-divergence signal and a useful diagnostic clue, not a call-level attribution.

## What this does and does not establish

- The positive control's root `Lubricants` directory appeared during the observed interval, and the negative variant's did not.
- The negative variant produced three distinct root-level side files with bytes matching existing task-level ObjProperties records. This shows a side-effect difference beyond the `Lubricants` check.
- The paired outcomes are consistent with an ID-sensitive path, but are not causal proof of a positive-ID gate: there is one pair, other ID-dependent downstream behavior is visible, and the runner does not witness the native creator call.
- The absence of `Lubricants` in the variant does not prove that `0x14008c630` was skipped; it might have been reached and failed, or another stage could account for the observed tree. Conversely, its presence in control does not prove the exact creator path ran successfully.
- No result establishes full Open, Save, accepted Close, or GUI readiness. Both target processes were explicitly terminated after the fixed observation window.

## Next smallest decisive evidence

Capture one original, uninstrumented Open with a supported same-occurrence native observation that binds (1) the effective parsed operation ID, (2) entry/return of `0x14008c630` through `0x140050820 -> 0x140051190`, (3) the ordered project-folder helper arguments including `Lubricants`, and (4) the corresponding `0x140081afb0` attempts and results. A narrowly filtered debugger/trace at that chain is the minimum evidence that can decide whether the control's directory result came from this creator and whether the negative run would be expected to skip it. Preserve the ObjProperties path/write events in that capture as a separate downstream channel. If supported same-occurrence capture is still unavailable, the smallest useful action is to keep the pair classified as corroborative and obtain that capability; another filesystem-only pair cannot identify the creator call.
