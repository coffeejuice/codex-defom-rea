# CP135 Type3 optimizer owner frontier

## Conclusion

The original SUB constructor establishes an actual producer and storage slot for the Type3 optimizer: on `type == 3`, it requests a `0x50`-byte allocation, calls `0x140917ff0` on success, and stores the returned pointer at `SUB+0x28`; allocation failure stores null. The loader later passes that pointer to the `.OPT` parser. This is enough to name the missing candidate owner and its native identity boundary, but not enough to implement a faithful private owner or safely attach the new parser helpers to it. No candidate Type3 owner is present in `makeSubProject`, and the Type3 guards remain necessary.

## Native producer and load order

`evidence/gui_mo/L5d/b5470.c`, `FUN_1400b5470`, supplies the construction sequence:

1. SUB base fields / members are initialized.
2. For type 2 or 3, it allocates a separate `0xb8`-byte DOE manager and stores it at `SUB+0x20`.
3. For type 3 only, it separately allocates `0x50` bytes and calls optimizer constructor `FUN_140917ff0`; the resulting pointer (or null if allocation returns null) is stored at `SUB+0x28`.
4. It then continues SUB initialization and returns the SUB pointer.

The `+0x20` DOE manager and `+0x28` optimizer are distinct allocations. The active parser call sequence in `evidence/gui_mo/s1/mdt/DOE/d5.c` / `evidence/gui_mo/s1/s1-05/sub0_s101.c` obtains both pointers and requires both nonnull before it builds the study `.OPT` path. It processes the `SfDoeStudy` XML manager and InputTable before calling `0x140908630(optPath, optimizer)`. Thus constructor availability precedes load-time parser mutation; parsing is not an owner factory.

`analysis/resume128-type3-constructor-parser.md` closes the optimizer constructor prefix: base constructor and derived vptr, QString at `+0x28`, initialized list-like shared members at `+0x18/+0x20`, and scalar defaults. Its teardown evidence closes the optimizer's own destructor sequence (`0x140918060` then base `0x14091e040`), but not how or when the containing SUB invokes it.

## Exact unresolved ownership and failure ordering

- The retained call-site listing establishes that `SUB+0x28` receives null on allocation failure, but does not establish a safe complete constructor-failure/unwind policy for a C++ candidate owner. The body `0x140917ff0` is present in CP128's bounded analysis, but nested shared members still have unknown element ownership/cleanup semantics.
- The optimizer parser is passed a borrowed nonnull pointer and never destroys it. Its `QFile` and `QTextStream` are locals; on open failure it closes/destroys those locals and returns false before setting optimizer `+0x48`; on successful open it sets `+0x48=true`, reads header/lines, then common cleanup closes QFile, destroys stream, QFile and path. It returns false even after mutation. This does not define SUB owner lifetime.
- The retained material reviewed here does not give a source-qualified containing-SUB destructor/deleter path proving when `SUB+0x28` is destroyed relative to `SUB+0x20`, failed project load, Close, or replacement. Do not infer it from the optimizer's own destructor or from the `+0x20` manager's destructor.
- Native parser callers use its return as an ignored/local value; false cannot be treated as “unloaded” or “no effects.” The call order establishes the parse invocation after XML/InputTable load, not success or population of the optimizer.

Current candidate source matches this boundary: `src/model/subproject.cpp::makeSubProject` still does not create an optimizer; `src/ops/report.cpp::ShapeOptimClass::loadClassData` explicitly refuses Type3, and `src/engine/shapeoptim_load.cpp` retains a Type3 guard. DOE `Study`/`StudyIo` and `SUB+0x20` are not substitutes for the optimizer at `SUB+0x28`.

## Smallest safe next owner cut

Before an actual `.OPT` loop, qualify one explicit, private Type3 owner object that corresponds to the constructor's known scalar defaults and distinct `+0x18/+0x20` row stores. The next useful source evidence is the containing SUB teardown/replacement/unwind path for `+0x28`, plus whether constructor failure can leave a partially initialized object. Keep the owner private and disconnected from public ShapeOptim until it can retain the exact same SUB-scoped lifetime and parser order. The current detached `Type3OptimizerScalarState` is only a value model; it is not sufficient proof of native `SUB+0x28` ownership.

## Qualification

Read-only synthesis of CP125/128/134 receipts, the retained `b5470.c` constructor/call-chain decompiles, and current candidate guard inspection. No source, build, test, GUI, original execution, or mutable REA action was performed. Addresses are from the original `DEF_GUI_MO_64.exe` evidence family identified in the cited CP128/134 reports. No native destructor order for the containing SUB is asserted.
