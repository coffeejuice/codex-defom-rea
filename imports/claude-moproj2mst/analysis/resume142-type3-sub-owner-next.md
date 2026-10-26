# CP142 Type3 SUB owner and teardown next frontier

## Result

The retained native evidence still does not close a containing-SUB destructor, an Open/Close replacement path that disposes an existing SUB, or constructor-failure unwind for `FUN_1400b5470`. It identifies the constructor, publication slots and loader uses, but no native owner teardown order for either `SUB+0x20` (DOE manager) or `SUB+0x28` (Type3 optimizer). There is no new source-closed owner integration.

## Positive native evidence

- `evidence/gui_mo/L5d/b5470.c` and its duplicate `evidence/gui_mo/s1/mdt/L7/d3.c` show `FUN_1400b5470(SUB, Project, type, id)`. For types 2/3 it allocates `0xb8`, calls `FUN_1400d1180` and stores the result at `SUB+0x20`. For type 3 it next allocates `0x50`, calls `FUN_140917ff0`, and stores the result at `SUB+0x28`. The two slots are separate. The listing contains no cleanup call or failure branch for an exception from either constructor; allocator-null stores null. That is not proof exceptions cannot occur or that partial construction leaks.
- `evidence/gui_mo/s1/s1-01/dc/open0.c`, `FUN_14008b410`, is the normal project-file subproject producer. After reading type/id arrays, it constructs each additional SUB with `FUN_1400b5470`, then appends the resulting pointer to `Project+0x58` through `FUN_14172d7e0`. It later iterates the stored entries and reads/parses their subproject state. In the shown producer path, no pre-clear/delete of the previous list or element destructor appears.
- `evidence/gui_mo/s1/s1-01/dc/open4.c` constructs a type-0 default/main SUB and appends it to `Project+0x58`; this does not exercise either native owner slot. `evidence/gui_mo/s1/s1-01/dc/sub0.c`'s GUI subproject creation uses type 4 through the same constructor, also with neither owner. These are not teardown evidence for a type-3 SUB.
- DOE-specific evidence names the `+0x20` manager destructor chain (`0x140919cb0 -> 0x14091db70 -> 0x1408169d0`, with geometry cleanup `0x140817d10`) and the Type3 optimizer destructor (`0x140918060`, then base `0x14091e040`) if their respective destructor entries are invoked. Neither retained call chain shows a containing-SUB caller or orders these two destructor calls relative to one another.

## Project replacement evidence and its limit

The retained native loader producer establishes append, not replacement. I found no retained Open/Close decompile invoking a SUB deleting destructor or deleting/clearing old `Project+0x58` entries. The list accessors (`0x1400a7360`, `0x1400a74f0`) establish count and indexed pointer retrieval only. `source/apps/moproj2mst/src/model/project.cpp::initProject` calls `p.subs.clear()` and `makeSubProject`; candidate `std::unique_ptr` destruction there is a property of this reconstruction, not evidence of the native replacement sequence. Likewise, the candidate `SubProject::~SubProject` currently records only its flow receiver witness and does not prove the original owner order.

## Precise missing witnesses

1. The native SUB deleting-destructor/disposer body (including how it selects and invokes the destructor for `+0x20` and `+0x28`) is missing from retained evidence.
2. The native project `Open`/`Close` or replacement caller that releases the old sub-list and invokes that disposer is missing; current native open evidence only appends newly constructed entries in `0x14008b410`.
3. The unwind metadata / cleanup landing path for `FUN_1400b5470` covering the `0x1400d1180` and `0x140917ff0` constructor calls is missing. Without it, cleanup after a throw during later owner construction cannot be ordered or matched.

The next useful capture is one bounded immutable-PE recovery of the actual SUB deleting-destructor entry and its project-list release caller, plus the relevant `FUN_1400b5470` EH/unwind actions. Until then, do not choose a `+0x20`/`+0x28` destruction order or claim Close/replacement parity. Keep `Type3OptimizerScalarState` detached and the public Type3/ShapeOptim guard intact.

## Qualification

Read-only review of the retained `b5470.c`, `d3.c`, `open0.c`, `open4.c`, `sub0.c`, `resume128-type3-constructor-parser.md`, `resume135-type3-owner-next.md`, `resume135-type3-owner-prefix-design.md`, `resume141-type3-sub-owner-teardown.md`, and candidate Project/SUB lifecycle. Original PE SHA-256 remains `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. No GUI, mutable REA database, source/build, or original-process action was performed.
