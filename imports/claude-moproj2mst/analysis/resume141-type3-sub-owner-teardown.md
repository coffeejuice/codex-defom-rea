# CP141 Type3 SUB optimizer ownership and teardown frontier

## Conclusion

Retained evidence proves the native Type3 optimizer is separately allocated and published in `SUB+0x28`, alongside the distinct DOE manager at `SUB+0x20`. It does not close the containing SUB's destructor/disposal path or the open/Close/replacement unwind order for either pointer. No source-backed SUB-owned optimizer integration is safe yet. The existing detached `Type3OptimizerScalarState` remains only a value projection; no public Type3 owner is available.

## Construction and caller relation

`evidence/gui_mo/L5d/b5470.c`, `FUN_1400b5470(SUB,P,type,id)`, shows this order:

- SUB common members and flow model are initialized first.
- For type 2 or type 3, allocation of `0xb8` bytes and call `FUN_1400d1180(manager,SUB)` produce the separate pointer stored at `SUB+0x20`.
- For type 3 only, allocation of `0x50` bytes and call `FUN_140917ff0(optimizer)` produce the separate pointer stored at `SUB+0x28`. A null result from allocation is stored as null. Other types store null in `+0x28`.
- The rest of SUB initialization follows these two owner publications.

The receiver methods `0x1400b5a90` and `0x1400b5ab0` are simple getters for `+0x20` and `+0x28`, respectively, as recorded in the retained sub-project spec/decompiles. In the Type3 `SfDoeStudy` load arm (`evidence/gui_mo/s1/mdt/DOE/d5.c` / `s1-05/sub0_s101.c`), both pointers are fetched and the arm requires them before it constructs the `.OPT` path; the `+0x20` manager's XML/InputTable work precedes `0x140908630(optPath, +0x28 optimizer)`. The `.OPT` parser takes a borrowed optimizer pointer, mutates it after successful QFile open, and does not destroy it. Thus `+0x20` is not the optimizer, and neither parsing nor the parser's stack QFile/QTextStream creates or owns the `+0x28` object.

`0x140917ff0`'s own constructor/destructor calls are separately described in `analysis/resume128-type3-constructor-parser.md`: base/derived initialization, QString `+0x28`, shared-data members `+0x18/+0x20`, scalar stores, then optimizer destructor `0x140918060` before base destructor `0x14091e040`. That closes the optimizer-internal teardown if the destructor is invoked; it does not establish who invokes it.

## What teardown/replacement is missing

The retained SUB constructor listing has no destructor or delete path. The type table in `source/docs/re/moproj2mst/s1-01/spec.md` establishes the pointer offsets, but its section only documents the constructor and load-time use. Searches of retained SUB decompiles, L0/L5d evidence and the closure index found no SUB destructor body or disposal sequence mentioning `+0x28` or `+0x20`. In particular, no retained function demonstrates:

1. whether destruction frees `+0x28` before or after `+0x20`, or whether either pointer is conditionally released by type;
2. the exact containing project/sub-list cleanup or replacement path that invokes that destructor on Open/Close;
3. constructor exception/unwind behavior if `0x140917ff0` throws after `+0x20` was successfully constructed, and how partial SUB members / allocation are reclaimed;
4. whether allocation-null and constructor-failure cases have different downstream cleanup/diagnostics.

The GUI creation path `0x1400a6a00` uses a different constructor `0x1400b5680` sharing the main model; that path is not evidence for file-open's `0x1400b5470` optimizer owner or its teardown. Candidate `Project::open` calls `p.subs.clear()` and `makeSubProject`; current `SubProject::~SubProject` records only the DOE flow receiver witness. Attaching a smart pointer at this stage would make host destruction/replacement deterministic but would choose native SUB lifetime and unwind behavior that has not been qualified.

## Smallest source-closed integration

There is no source-ready SUB owner field/test set until the containing SUB disposal and unwind contract is captured. Adding only a scalar prefix would not represent the native `+0x18/+0x20` shared-data members or `+0x28` QString and could make a nonnull candidate appear to satisfy the native loader's owner gate. A detached scalar value remains valid as-is and is not an owner.

Required next immutable evidence is the native containing-SUB destructor/disposer and its caller(s) in the project sub-list replacement/Close path, including the exact destruction order for `+0x28` and `+0x20`; and the EH/unwind record for `FUN_1400b5470` covering each nested allocation/constructor call. Once these exist, a private SUB-scoped owner prefix can be proposed without claiming full optimizer parser/getter parity. Public Type3/ShapeOptim remains guarded independently.

## Qualification

Read-only synthesis of `analysis/resume135-type3-owner-next.md`, `analysis/resume135-type3-owner-prefix-design.md`, `analysis/resume128-type3-constructor-parser.md`, original-derived constructor/caller listings and retained source SUB lifetime. The original GUI PE SHA-256 in the controlling reports is `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. No new binary import, database mutation, source/build/test, GUI or original execution was performed. No containing-SUB destructor order is claimed.
