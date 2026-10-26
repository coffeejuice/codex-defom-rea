# Resume 51: 42_vdie public Cogging Save candidate run

## Result

The candidate-only public `WRITE_MDT` route completed successfully on a disposable copy of `42_vdie`. The CLI returned exit code `0`; it rewrote the copied active Cogging MDT and emitted the requested candidate MST. The copied project retained active diagram index `3`, whose fourth diagram item maps to operation 4 (`SfOprCogging`). Its input Task00004 MDT had `SIMLST 56`, `NoReheats 1`.

This is a candidate conversion compared with retained historical oracle-B output. No original application, GUI, or mutable REA session was run.

## Inputs and isolation

- Source project: `evidence/gui_mo/s1/corpus/42_vdie/vdie.moproj`, SHA-256 `014eba76c741dccc678762b2bf38d531f066931b15ccb4f64fd3af7110dc3ca4`.
- Source Task00004 input: `evidence/gui_mo/s1/corpus/42_vdie/Operations/Task00004/Cogging.MDT`, SHA-256 `b2b8e57be83ff27def086b0b95ce0ee53b68c2262d6b1c583ffd47a9507b786e`.
- Disposable project copy: `build/cogging-42-vdie-candidate/run/42_vdie/`.
- Before run: 50 files, 10,827,918 bytes; copied project and Cogging MDT hashes matched source hashes above. Full pre-run inventory is retained at `build/cogging-42-vdie-candidate/run/inventory-pre.json`.
- Candidate binary: `build/cogging-42-vdie-candidate/bin/moproj2mst.exe`, SHA-256 `5fb19776f3fb38deef1ffa648b771b1ef106e3b67403bef2e94f16114a042816`; `--version` reports `moproj2mst api 1`.
- Retained historical oracle-B output: `evidence/gui_mo/oracle_b/RS_42_vdie/Operations/Task00004/Cogging.MDT`, SHA-256 `714b238dd66674e0927d8b8dc8125aba3b66f21dee6871f5e3cb1592795cfd77`.

The binary was built in the unique candidate directory with the CMake `moproj2mst_cli` target, MSVC Release configuration, and the active Visual Studio developer environment. The first configure lacked Windows resource tools; retrying with static-library compiler probing and `/MANIFEST:NO`, then using `vcvars64.bat`, allowed the Release build to complete. No source files were edited for this run.

## Inventory and output comparison

The after-run inventory contains the same 50 project-copy files. Exactly one file changed: `Operations/Task00004/Cogging.MDT`, from 54,519 bytes / SHA-256 `b2b8e57be83ff27def086b0b95ce0ee53b68c2262d6b1c583ffd47a9507b786e` to 54,500 bytes / SHA-256 `b450b52a935827e640200062cf3ae84cf2a1fa198876cfc20b4e7bb873ebcf50`. No files were added or removed inside the project copy. The complete post-run inventory is `build/cogging-42-vdie-candidate/run/inventory-post.json`.

Against historical oracle-B output, both candidate and oracle have 2,844 lines. Their only differing line is line 3, the product banner:

- Oracle: `* DEFORM-2D/3D 11.0 (Service Pack 2)  `
- Candidate: `* DEFORM-2D/3D 11.0`

All remaining line bytes match exactly when line 3 is omitted. Thus the saved Cogging model content matches the retained oracle after accounting for the banner; whole-file hashes differ because the banner and its trailing spaces differ. Both files report `SIMLST 56` and `NoReheats 1`.

The requested MST was written outside the disposable project at `build/cogging-42-vdie-candidate/run/candidate.MST`, 101,489 bytes, SHA-256 `43017314c11a5259a3eae7e106a8f66e4f5dc6c00cdc7e93374f302352ac7e28`.

## Path qualification

The converter opened the project from the copied run path, while the retained oracle resides under `oracle_b/RS_42_vdie`. The Cogging MDT contains no path-valued metadata found by the bounded path-token scan. Since every line other than the product-banner line matches byte-for-byte, relocating the project did not introduce an observable path-dependent difference in this MDT. This is a comparison of the saved MDT only; it does not assert that the separate MST is path-independent.
