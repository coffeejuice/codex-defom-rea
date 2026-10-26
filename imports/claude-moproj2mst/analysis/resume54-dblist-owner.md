# Checkpoint 54: original DBLIST handler ownership and gate

## Result

The retained static artifacts establish the DBLIST dispatch site, but they do not contain the body of `FUN_140655df0`; therefore the native owner, fields written, validation, side effects, and return/failure behavior remain unresolved. The global gate is likewise unresolved beyond the call site. No original runtime or GUI evidence was collected in this task.

The call site is in `FUN_140650e60` from `analysis/wave2-doe-session-valid-existing-input-decompile.json`, whose static receipt binds it to original `DEF_GUI_MO_64.exe` SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. It compares the direct SessionRoot child tag to DBLIST, evaluates `FUN_1402cbec0(0x143d766b8)`, and on a true result invokes `FUN_140655df0(param_1, node)`. The same loop gives ReportList a separate handler (`FUN_1406554a0(param_1, node, param_3)`) behind the visible same condition. `param_1` is the part-level inner model, but this call alone does not identify the native storage field or prove a `SessionPart`-equivalent owner.

The existing decompile's `functions` list contains only requested addresses `140650e60`, `140658690`, `14081b2b0`, and `14081aec0`; neither DBLIST helper nor gate is among them. A case-insensitive search of retained `evidence` and `analysis` finds the DBLIST helper's address only in the prior seam write-up/call-site evidence, not a helper body. This establishes absence from the retained exports, not absence from the imported Ghidra database.

## Smallest root-serialized query

Queue one bounded export on the already verified **private copied Ghidra project**, with no shared REA session and no target execution:

```powershell
python tools/doe_ghidra/run_more.py --label dblist-owner 140655df0 1402cbec0
```

`run_more.py` requires a fresh label/output, reads the verified project provenance receipt, and invokes `DoeExport.java` with `-readOnly -noanalysis`. It will export the DBLIST handler and the called global predicate, plus the handler's references/known callees. Serialize this run at the root against any other work using the same private project/runtime; do not attach a live/shared REA session or overwrite an existing label's outputs. If the helper decompiles but its ownership stores remain opaque, the smallest follow-up is to export only its resolved direct callees and the caller/receiver constructor/accessor that establishes the destination object. Do not broaden to a batch until that result identifies a concrete dependency.

The `1402cbec0` query is important because the visible condition controls whether the original dispatches the helper. Until its body and caller context are read, do not equate it to headless session presence or remove the candidate's public active-Type2 guard. The parser seam remains detached and non-dispatched; B127 was not activated or rerun here.

## Scope

Read-only file searches and inspection of retained JSON decompile/receipt artifacts only. No source, test, build, GUI, mutable REA database, or original application was changed or run.
