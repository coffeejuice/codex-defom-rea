# CP127 Type3 optimizer and `.OPT` body frontier

## Finding

The retained immutable exports in `analysis/` do not contain an instruction body beginning at either target:

- Optimizer constructor: `0x140917ff0` (RVA `0x917ff0`). The first required instruction is the entry instruction at that address; no bounded `.asm` export starts there or covers its body. The retained `FUN_1400b5470` decompile shows only the allocation, call, and publication to `SUB+0x28`.
- `.OPT` parser: `0x140908630` (RVA `0x908630`). The first required instruction is likewise unavailable. Retained `FUN_14008f530` decompiles show only the call with `(optPath, optimizer)` and local result storage.

Thus this follow-up adds no constructor initialization, file parsing, list construction, error, cleanup, or destruction facts. The first missing instruction ranges are the respective entry instructions (`0x140917ff0` and `0x140908630`) through each function's return; there is no partial entry window from which to infer a valid bounded body.

## What the existing exports do contain

`analysis/` contains extensive unrelated/native assembly windows and references in reports, but targeted lookup for these function starts returns no `.asm` or capture-manifest export at either target. Exact references appear in `analysis/resume125-type3-optimizer-owner.md` and earlier summary reports; those are not machine-code bodies. The retained decompiles remain:

- `evidence/gui_mo/L5d/b5470.c`: `FUN_1400b5470` allocates `0x50`, calls the missing constructor, then stores the result in SUB `+0x28` for type 3.
- `evidence/gui_mo/s1/mdt/DOE/d5.c` and `evidence/gui_mo/s1/s1-05/sub0_s101.c`: Type3 `SfDoeStudy` path obtains the `+0x28` pointer and eventually calls `FUN_140908630(optPath, optimizer)` after manager XML/InputTable processing.
- `evidence/gui_mo/s1/mdt/L7/d0.c` and `d1.c`: later `FUN_14013d740` reads the optimizer vtable and produces ordered ShapeOptim `listF0_` names. This is a consumer-side contract, not a substitute for the parser body.

The `wave2-doe-output-data-leaf-140917130.asm` file is a different address (`0x140917130`), not evidence for `0x140917ff0`; do not treat nearby function names/addresses as coverage. There is no retained code window in `analysis/` establishing either function's entry bytes.

## Frontier

The next needed immutable capture is a bounded code window at `0x140917ff0` through its return, plus the relevant SUB teardown caller, and a bounded code window at `0x140908630` through its return with direct helpers that mutate the optimizer. Until those entry ranges exist, constructor defaults/ownership and `.OPT` parse order/effects remain wholly unclosed. No Ghidra import was retried and no analysis database was modified for this checkpoint.

Read-only search of already retained files only; no source, build, GUI, binary-analysis session, or mutable REA database change.
