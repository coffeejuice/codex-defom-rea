# CP164: operation property dispatch boundary

## Result

The smallest source-ready cut is **not** to loosen the generic default in `propsToEngine`. `SfOprReport` already has a source-bound property callback, but public operation Save is stopped by the earlier Report owner/document lifecycle. The exact missing witness for advancing this branch is a fresh-original, Report-operation Open→Save trace that binds the document at `SubProject+0x30`, confirms the normal default answers to any dialogs on that path, and records the ordered Report property and Save-tail effects. Until that witness and owner state are represented, no dispatch edit is justified.

The generic dispatcher’s final `notPorted(op, "properties vt+0x60")` is a deliberate unsupported-class boundary, not evidence that unknown/default operation classes share a no-op property body. Preserve it. A direct helper call would not establish public caller reachability.

## Bounded evidence

- `source/docs/re/moproj2mst/s/spec.md`, section 1 and section 3: operation Save calls `0x1400543a0` (props vt+0x60) in the index-zero path, and the Report row binds `0x1412b1f50` to F80/F90/FB0. SHA-256: `0500677F9D792DA621CDFC123B7C8B9B5029FA91C2270AFCDFC7BF561814EE92`.
- `evidence/gui_mo/out_s/s9.c`, `FUN_1412b1f50`: retained decompile confirms the three shared helpers and successful return. This is static body evidence; it does not establish an Opened Report owner or a Save occurrence. SHA-256: `2678BBB3704D786DA55C4ACB5A86FC80BC9229189D39D4A7101DABB68E7FEB06`.
- `source/apps/moproj2mst/src/engine/save_current.cpp`: `propsToEngine` maps Report to the three-helper family, but `saveCurrentOperation` rejects Report at vt+0x68 before the generic dispatcher. SHA-256: `BC77D02830AC0C75EA5FA669F303B818178610291FDBDA837F1A1ABB08185129`.
- `source/apps/moproj2mst/src/ops/report.cpp`: Report load still refuses at the report-document accessor (`SP+0x30`) and its later viewer/UI steps. SHA-256: `E378D472550BBE9F6A109BAA330F6612277328AC425853CF5E17BA316252B43A`.
- `analysis/resume164-close-next.md`: current Close owner audit independently identifies the Report document producer/lifetime as the next missing dependency and distinguishes static conditional bytes from a fresh lifecycle occurrence. SHA-256: `13308E5BA842A4CFCC4399BBB60969AD4761C5EB0F06C9852D23A740F6B5352F`.

These references support a concrete call-path blocker, not a claim that Report is the only omitted behavior in the generic dispatcher. The retained evidence does not establish a generic “default dialog” answer or delivered nested-loop events for a Report Save; those must be observed on the precise original path rather than inferred from another dialog’s default.

## Next evidence boundary

Capture one copied, valid Report project in a fresh original process: record dialog prompts and default selections, whether Open publishes a Report document to the active subproject, the identity/payload lifetime of that document through Save, whether Save reaches vt+0x60 and vt+0x68, and the ordered side effects. Compare against candidate only after the Report document/viewer owner is modeled. If no valid Report project can reach that route, retain the refusal and report the exact missing fixture/owner instead of enabling the generic branch.

Read-only scope: source/evidence inspection only. No source or test changes, build, original application, GUI control, or live REA session was used.
