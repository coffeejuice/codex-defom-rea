# CP124 ShapeOptim Type3 and GEO-list frontier

## Finding

The next ShapeOptim prerequisite after CP123 is the Type3 optimization-study owner at `SUB+0x28`, not another Type2 DOE name field. The original Type3 sub-project has two distinct objects: the common DOE manager at `SUB+0x20` and a `0x50` optimization-data object at `SUB+0x28`. The current adapter models only the Type2 `SUB+0x20` manager/name chain and deliberately refuses Type3 before it can reach the later load.

There is no source-closed Type3 integration increment yet. The missing `SUB+0x28` owner must be created and populated from Type3 input before either native ShapeOptim `loadClassData` or the `listF0_` producer can run. Reusing the Type2 `doe::Study` value or B121/B122 variable records as the optimization object would conflate separate native owners.

## Native producer and ordered consumers

Retained native decompile `evidence/gui_mo/s1/mdt/L7/d0.c` shows `ShapeOptim` `loadClassData` at `0x14013d740` obtaining `SUB+0x28` through `0x1400b5ab0`. When nonnull, it reads optimizer values into ShapeOptim class fields in this order: vtable `+0x138` to OB `+0xcc`; `0x14091e0b0` to OB `+0xd8`; `0x14091e0f0` to OB `+0xd4`; vtable `+0x128` to OB `+0xd0` and its Boolean `+0xe8`; vtable `+0x118` to OB `+0xc0` and derived `+0xc4`; then vtable `+0x28`/`+0x30` item count/access for stop-criterion records. Item tags `0x3f2`, `0x3f4`, `0x438`, and `0x439` set separate flags/fields; other tags take the fallback object/field path. It then calls vtable `+0xc0` for the geometry-name count and `+0xd8` for each ordered QString, appending each to `OB+0xf0` (`listF0_`) through `FUN_140004b90`. Finally vtable `+0x148` and `+0x158` feed OB `+0xe0` and `+0xe9`. This excerpt establishes the list's actual producer and an ordered set of earlier owner reads; it does not qualify the unknown optimizer object's constructor values, parser, or destruction semantics.

The retained SUB constructor `evidence/gui_mo/L5d/b5470.c` (`0x1400b5470`) allocates the shared `0xb8` manager through `0x1400d1180` for both Type2 and Type3, and separately allocates the `0x50` object through `0x140917ff0` only for Type3. The SUB layout table in `source/docs/re/moproj2mst/s1-01/spec.md` §8.1 confirms `+0x20` and `+0x28` are distinct pointers. The common `0x1400d1180` manager constructor binds the SUB, generates the `DoeStudy<id>` name at `+0x98`, and performs manager initialization. Therefore CP123's identity-checked Type2 name read cannot stand in for the absent Type3 optimization data.

Native `loadData` `0x14013cbe0` calls `studyFolder` before reading `OB+0xf0`. It then iterates names in list order, creates a kind-3 ROI object with that name, forms `<Opt folder><name>.GEO`, and enters the geometry loader at `0x1408f3840`. Current source has only the later list consumer in `src/engine/shapeoptim_load.cpp`; the only source references to `listF0_` are its declaration, `init()` clear, and this guard/use. The producer belongs to the missing Type3 `loadClassData` owner, not a hidden current list builder.

## Current source boundary

- `src/model/subproject.cpp::makeSubProject` creates `ownedDoeStudy`/`doe` only for type 2; its Type3 `SfDoeStudy` parser arm refuses before loading the native `.OPT` state.
- `src/ops/report.cpp::ShapeOptimClass::loadClassData` refuses `sp->type == 3` before any optimizer field or list import.
- The `loadOperation` sequence invokes `loadClassData` before `loadData`, so the Type3 path cannot reach CP122's immediate folder effect or CP123's Type2 name branch through normal Open.
- The isolated CP122 path does not reproduce the original failed-mkpath continuation: original `0x14081afb0` displays a one-OK modal then returns true, whereas the candidate records failure and stops at its explicit modal boundary. Runtime modal completion is still unqualified; keep that boundary visible.
- Retained Oracle B project-file search found no `Type="3"` sub-project record to supply a Type3 parser fixture. Existing Type3 tables/decompiles are structural evidence, not an active Type3 file or runtime capture.

## Smallest next step and required evidence

Before source integration, close one actual Type3 fixture through the immutable original and identify both owner lifetimes and the `.OPT` load route. The minimal passive capture should bind the same SUB identity to `SUB+0x20` and `SUB+0x28`, record their constructor/vtable identities and ownership/destruction order, preserve the manager `+0x98` name value, and capture the Type3 `<SfDoeStudy>` input plus the exact `.OPT` path/open/read result. For `SUB+0x28`, record the values and ordered result of the vtable calls observed in `0x14013d740` (`+0x118`, `+0x128`, `+0x138`, `+0x148`, `+0x158`, `+0x28`/`+0x30`, `+0xc0`/`+0xd8`) and the QString null/empty/value identity of each geometry name. Then bind those values to the generated `listF0_` order before `0x14013cbe0`.

Only after that owner is bound should the later GEO frontier be opened. Capture each row's `name -> <Opt>/<name>.GEO` path, object kind/name publication point, loader status, and any partial geometry effects on failure. For the remaining `loadOperation` tail, bind flow-selected operation and ROI-display/model identity through `setStepOperation`, `stepModels`, and `postLoadViews`; for Save, separately bind the Type3 optimizer and ROI objects at `vt+0x80`/`vt+0xe8`, including each `.GEO` write/open result. Static order alone does not establish these runtime owners or public Save eligibility.

Until then, keep Type3 activation guarded at `loadClassData`, keep populated GEO import guarded in the later reader, and keep ShapeOptim public Save gated. A bounded private Type3 owner/parser is the next candidate unit, but the current repository lacks the owner/parser fixture required to implement it without inventing state.

## Evidence basis and limits

Read-only inspection of the retained L7, SUB-constructor, DOE-constructor decompiles; `source/docs/re/moproj2mst/s1-01/spec.md` §8.1; current `model/subproject.cpp`, `ops/report.cpp`, and `engine/shapeoptim_load.cpp`; and CP122/CP123 receipts. No source, test, build, GUI, original execution, or mutable REA change was made. `git diff --check` passed for the new report.
