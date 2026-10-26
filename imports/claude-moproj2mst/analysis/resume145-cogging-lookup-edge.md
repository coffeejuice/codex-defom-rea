# CP145: Cogging lookup-to-owner integration edge

## Finding

The object owner and callback path are source-closed: native File > Save resolves the active diagram operation, invokes its class save slots, and Cogging `vt+0xd8` (`0x14075c260`) obtains `SfOprCoggingProperties` then its `CogModel` and calls `0x140762ec0`; that calls `0x140762f30` for manipulators. Candidate mirrors the owner path: `saveCurrentOperation` routes the active operation, `coggingAfterSave` obtains `CoggingProps` from that operation and `props->model`, then `coggingGeometryStep` calls `cogPlaceManipulators`. Thus CP143's explicit-bool reference helper has a precise owner and two call positions; no separate model-owner lookup is missing.

## Lookup edge remains unqualified

Native `0x140762f30` samples `QFile::exists("MAN_COUPLE.DAT")` into a stack-local boolean only in the spring branch, after preparing object 4. It later applies the two conditional references after object 5 and object 7 transformations, before each object's Router call. The lookup does not come from `CogModel`, project XML, or MDT data.

Open and operation-load evidence places the process current directory at the project folder. However Save entry `0x1400980d0` services `QCoreApplication::processEvents()` before active-operation resolution and `0x140052ec0`; neither retained static Save code nor retained oracle logs show the CWD/file-existence result at the later Cogging call. `analysis/resume141-cogging-save-cwd.md` records the missing native witness. Oracle B `42_vdie` is one-reheat and contains no MAN_COUPLE case (`analysis/resume51-cogging-42-vdie-candidate.md` compares candidate output to that retained oracle); it cannot qualify this lookup.

## Smallest integration if the lookup result is supplied

At the existing candidate owner path, pass a qualified `bool coupled` from `coggingAfterSave` through `coggingGeometryStep` to `cogPlaceManipulators`. In the spring branch, retain the sampled value locally and call the CP143 helpers at the existing effect points only: object 5 after copy/inter-object/rotate and before Router; object 7 after copy/inter-object/translate/direction/rotate and before Router. Keep object 6 unchanged. This wires the helper to the actual `CogModel`/engine object owner without changing operation selection or touching `viewer.cpp`.

The boolean's source cannot yet be `projectDir(p)/MAN_COUPLE.DAT` in a public behavior claim. The smallest blocker-closing original capture is the CP141 witness: at the `QFile::exists` call in a fresh Save, record `QDir::currentPath()` and the relative existence result, with a project-directory file versus launch-directory-only control and the Save-entry process-events interval. Until then, keep the lookup result explicit/injected for private tests and retain the current public guard; do not wire the CP143 helpers into normal Save.

## Evidence

- Native owner/call order and lookup site: `source/docs/re/moproj2mst/mdt/L1/spec_L1B.md` §§0, 5; retained decompile `evidence/gui_mo/s1/mdt/L1/B/b21.c` and `f30.asm`.
- Candidate owner and callback: `source/apps/moproj2mst/src/engine/save_current.cpp` (`saveCurrentOperation`) and `source/apps/moproj2mst/src/engine/cogging_save.cpp` (`coggingAfterSave` → `coggingGeometryStep` → `cogPlaceManipulators`).
- Save event pump/order: immutable original decompile `evidence/gui_mo/s1/mdt/save_d0.c` lines around 31–47; bounded PE order receipt `analysis/wave3-boolean-save-io-contract.json`.
- CP143 helper scope/semantics: `analysis/resume143-cogging-coupled-refs.md`.

Read-only retained-source/evidence inspection only. No source, build, GUI, or mutable REA state was changed.
