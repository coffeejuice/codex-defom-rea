# CP136: Cogging multi-reheat and coupled-manipulator frontier

## Result

The smallest source-closed Cogging increment is the **positive `MAN_COUPLE.DAT` presence decision and its two movement-reference assignments**, not a broader multi-reheat serializer. The retained native code uses only file existence; it does not parse the file. When the spring manipulator path sees the file, it sets `couple=true`, then writes object 5 movement reference `(4, 1.0)` and object 7 `(6, 1.0)` before routing those objects. Current `cogPlaceManipulators` instead rejects when its reconstructed project-relative probe finds the file, and its local `couple` remains false. This is a bounded branch independent of `viewer.cpp`.

## Evidence and limits

- `source/docs/re/moproj2mst/mdt/L1/spec_L1B.md` §5, native pseudocode at lines 349 and later object-5/object-7 blocks, records `QFile::exists("MAN_COUPLE.DAT")`, relative to process current directory, and the exact conditional assignments to `+0x22c0/+0x22c8`.
- `source/docs/re/moproj2mst/mdt/L1/spec_L1A.md` opening call-order note establishes that the Cogging operation loader sets the process current directory to the project folder immediately before its operation load slots. `analysis/resume76-open-save-dialog-filter-path.md` separately documents the Open routine's project-directory switch.
- Current implementation is `source/apps/moproj2mst/src/engine/cogging_save.cpp:568-585,599-602,635-638`. It tests `projectDir(p)/MAN_COUPLE.DAT` and throws `M2M_E_UNSUPPORTED`; `bool couple` is otherwise never made true.
- The file's contents/format are not needed for this specific branch: the native test is `QFile::exists`. This does not qualify any additional state loaded from elsewhere, nor the downstream Router geometry result. The GUI's Save-time current directory must still be the project's directory for a project-path-based port check to be action-equivalent; operation-load evidence alone does not prove it remains unchanged through all callbacks until Save.

## Smallest candidate test/action

A focused private `cogPlaceManipulators` case can arrange a spring-enabled Cogging operation and a disposable project containing an empty `MAN_COUPLE.DAT`, with the test thread's current directory explicitly set to that project. Verify the native-derived decision on both existing objects: object 5 gets movement reference id `4`, value `1.0`; object 7 gets id `6`, value `1.0`. Keep a paired absent-file case showing those conditional assignments are not made. Exercise Router through existing deterministic doubles or isolate only the presence-to-couple mapping if geometry cannot be made stable. Do not infer the file's semantics from its name.

Before replacing the public refusal with the project-relative presence check, close the Save-time directory identity at the native Save caller or keep the public refusal and implement only a private helper with an explicit directory argument. The native behavior itself is statically closed once the lookup directory is supplied.

## Multi-reheat status

No further source-closed multi-reheat output/failure decision is available. `analysis/resume53-cogging-multireheat.md` and `analysis/resume64-cogging-memory-bound.md` establish a one-past logical read for two reheats but not whether it faults or what adjacent bytes contain. The current guarded refusal remains justified. The empty/one-reheat path and candidate public guard are already covered separately; a new multi-reheat behavior change would require an original runtime outcome, which still could not make undefined adjacent-memory bytes a general output contract.

Read-only source/evidence inspection only. No source, build, GUI, or mutable REA state was changed.
