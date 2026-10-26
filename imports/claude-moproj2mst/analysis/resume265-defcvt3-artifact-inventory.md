# DEF_CVT3 artifact inventory

## Finding

The installed v11.0.2 tree contains `DEF_CVT3.com`, not a `DEF_CVT3.exe` or DLL. The file is present at:

`C:\Program Files\SFTC\DEFORM\v11.0.2\3D\DEF_CVT3.com`

It is 36,864 bytes and has SHA-256 `8DD8B87C8B24CEC933E98FEE31946103D95CB0C09A6CB28119131AACC04652FE`. Independent header verification after discovery found `MZ`, `e_lfanew=0xe8`, and `PE\0\0` at that offset: it is an x86 PE32 console executable despite its `.com` suffix. It has five sections and a COFF timestamp of 2007-07-05 14:40:26 UTC. Its Windows version-resource fields are blank, so no product version is established.

The same directory also contains `DEF_CVT.com` (229,888 bytes; SHA-256 begins `0E8A6523A2918AEEECAAE00660DD61B52E54A920E0…`) and `DEF_OPT_CVT_SDB.exe` (7,551,488 bytes; SHA-256 begins `05C3B19594491C0BDC346061762E027421C2B61DB2…`). These are distinct names and cannot be assumed to be interchangeable with DEF_CVT3. `DEF_STL.exe` and geometry import libraries such as `TKIGES.dll`/`TKSTL.dll` are also present, but dependency relationships were not verified by executing or disassembling the target.

## Checkout evidence

Before the copy below, the import tree had no matching `DEF_CVT3` binary or DLL. Its converter fixture `source/tests/commands/FILE_INPUT/fi_stl_def_cvt3.json` says the intended route is `DEF_CVT3 KEY STL` and attributes a handoff to `DEF_STL.EXE` to strings in `DEF_CVT3.com`; it explicitly labels prompt answers as guesses and does not establish a complete dependency closure. It references `C:/Program Files/SFTC/DEFORM/v11.0.2/3D/LABS/TOP_DIE.STL` as input.

The separately referenced `analysis/resume256-format-next-leaf.md` is present in this converter subtree. The initial inventory ran from the parent checkout and incorrectly reported it absent; this correction reconciles its explicit external-converter blocker.

## Copy-scope conclusion

The user's standing import instruction authorized copying related original evidence into the converter project. The root agent copied the installed `DEF_CVT3.com` and possible handoff target `DEF_STL.exe` into `evidence/original-binaries/` without changing the installed files. Independent SHA-256 and byte-for-byte source/destination comparisons passed: `DEF_CVT3.com` is `8dd8b87c8b24cec933e98fee31946103d95cb0c09a6cb28119131aacc04652fe`; `DEF_STL.exe` is 233,472 bytes and `7a4d8fe41b94709ac4b0094ce9a6ccb85cf0ae2c10fdc6785085c0b961e27a9a`. `DEF_STL.exe` is also x86 PE32 console (four sections, COFF timestamp 2014-09-23 15:02:39 UTC). These copies establish static analysis inputs, not a proven runtime dependency or format behavior. Neither was executed.

## Search boundary

Searched the named v11.0.2 install tree and `imports/claude-moproj2mst` for DEF_CVT3/CVT3 names. No whole-drive recursive scan was performed.
