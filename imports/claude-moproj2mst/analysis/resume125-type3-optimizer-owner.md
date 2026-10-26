# CP125 Type3 optimizer owner: immutable-PE closure

## Result

The retained PE-derived evidence establishes where the Type3 optimizer owner is allocated, where its pointer is stored, which `.OPT` parser is called during project load, and which later ShapeOptim slot produces the geometry-name list. It does **not** establish the optimizer object's initialized fields, destructor/ownership behavior, or `.OPT` parser's state effects. There is no source-closed implementation of `SUB+0x28` or `listF0_` from this evidence alone. Keep the Type3 ShapeOptim guard in place.

The distinction matters because `SUB+0x20` and `SUB+0x28` are separate objects. The former is the DOE manager used for the `SfDoeStudy` name and InputTable; the latter is a separate `0x50`-byte optimization object and is the receiver of `.OPT` parsing and the later ShapeOptim getter calls. The Type2 `doe::Study` owner cannot stand in for that object.

## Constructor and pointer ownership evidence

The retained decompile `evidence/gui_mo/L5d/b5470.c`, `FUN_1400b5470 @ 0x1400b5470`, is the SUB constructor. For type 2 or 3 it allocates `0xb8` bytes, calls `FUN_1400d1180`, and stores the result at `SUB+0x20`. For type 3 only, it separately allocates `0x50` bytes, calls `FUN_140917ff0`, and stores the returned pointer at `SUB+0x28`; on allocation failure it stores null. Non-type-3 SUBs get null at `+0x28`. The duplicate retained listing `evidence/gui_mo/s1/mdt/L7/d3.c` shows the same sequence. `source/docs/re/moproj2mst/s1-01/spec.md` §8.1 independently records the layout and distinguishes these two slots.

This closes the allocation site, requested size, type condition, and pointer publication field. It does not close what `FUN_140917ff0` initializes, whether it can fail after allocation, its internal member defaults, vtable identity, nested ownership, destructor, or unwind behavior. The available `d3.c` occurrence is the call site, not a body for `FUN_140917ff0`. The common `FUN_1400d1180` at `evidence/gui_mo/s1/mdt/DOE/d3.c` is specifically the `+0x20` manager constructor and is not evidence for the `+0x28` optimizer.

## `.OPT` load and list production chain

The original sub-project parser is retained in `evidence/gui_mo/s1/mdt/DOE/d5.c`, `FUN_14008f530` (also represented by `evidence/gui_mo/s1/s1-05/sub0_s101.c`). In the Type3 `SfDoeStudy` arm it retrieves both pointers: `FUN_1400b5ab0(SUB)` for `SUB+0x28`, and `FUN_1400b5a90(SUB)` for `SUB+0x20`. It proceeds only when both are nonnull. It then calls `FUN_1400ba350` to obtain the study path, constructs the `.OPT` pathname from the study path, `QFileInfo(baseName(project path))`, and the literal `.OPT`; parses the XML element into the `+0x20` manager with `FUN_1400d2aa0`; loads its InputTable with `FUN_1400d3970`; and calls `FUN_140908630(optPath, optimizer)` at `0x14008f530`'s Type3 arm. The parser return is stored in a local; the retained excerpt does not show an aborting test before local cleanup/continuation. This is call ordering only, not proof of `.OPT` content interpretation or resulting owner state.

The later list producer is `FUN_14013d740` in `evidence/gui_mo/s1/mdt/L7/d0.c` (duplicate form in `d1.c`). ShapeOptim `loadClassData` obtains `SUB+0x28` through `FUN_1400b5ab0`; when nonnull, it calls optimizer vtable slots to import fields into ShapeOptim and, after stop-criterion processing, obtains geometry-name count via `vt+0xc0` and each name via `vt+0xd8`, appending each in order to `OB+0xf0` (`listF0_`) through `FUN_140004b90`. It subsequently reads `vt+0x148` and `vt+0x158`. These slots define the minimum observable interface that a loaded optimizer must satisfy; they do not reveal what `.OPT` parsing populates or the semantics/defaults of each getter.

The list's later consumer is `FUN_14013cbe0` in `d0.c`: after the study-folder step it iterates the list, creates kind-3 ROI objects, forms `<Opt folder><name>.GEO`, and enters the geometry loader at `0x1408f3840`. That is downstream of the unresolved owner/parser and cannot be enabled by manufacturing `listF0_` values.

Current adapter boundary is consistent with this evidence: `src/model/subproject.cpp::makeSubProject` has no Type3 optimization owner/parser; `src/ops/report.cpp::ShapeOptimClass::loadClassData` retains a Type3 refusal; and `src/engine/shapeoptim_load.cpp` only has the later list consumer. A Type2 study name or the CP113/114/115 Geometry helpers do not supply Type3 optimizer state.

## Exact remaining static gap and next capture

The bounded immutable-PE attempt was to analyze `0x140917ff0` in the retained original binary. The binary opened as x64 PE, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. Ghidra 12.1.4 did not complete import/analysis within its 330-second startup deadline; the subsequent instruction request timed out, and the binary was closed. No new body, bytes, or destructor evidence was recovered. The retained files contain only the constructor call site for `0x140917ff0` and only the caller for `0x140908630`.

The smallest useful next immutable capture is bounded native code and data for:

1. `0x140917ff0` through its return, including direct calls, stores, and any referenced vtable/data; then the SUB Type3 teardown path that disposes `SUB+0x28`.
2. `0x140908630` through its return, including file-open/read branches and writes through the optimizer pointer, plus its direct helper callees that establish record/list construction or failure cleanup.
3. The relevant optimizer vtable and its target functions for `+0x28`, `+0x30`, `+0x118`, `+0x128`, `+0x138`, `+0xc0`, `+0xd8`, `+0x148`, and `+0x158`; record exact addresses and bounded bytes for each rather than inferring their contracts from the later caller alone.

If later active evidence becomes available, bind one actual type-3 SUB identity to the `+0x20` manager and `+0x28` optimizer pointers, `.OPT` path/open/read result, and the ordered getter results that become `listF0_`. Without those identities the static call graph cannot establish a particular active ShapeOptim operation's state or GEO effects.

## Limits

This is a retained-evidence/static-PE audit only. No mutable REA analysis, GUI/original execution, source edit, build, or test was performed. The retained evidence proves the pointer slot, parser call, and downstream list producer, but not constructor member values, optimizer ownership cleanup, `.OPT` parser semantics, or successful Type3 import. Public Type3 Open/Save and populated GEO import remain guarded.
