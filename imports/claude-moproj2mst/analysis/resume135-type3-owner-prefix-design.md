# CP135 Type3 owner prefix design boundary

## Decision

No new source-owned Type3 SUB optimizer owner can be added without exceeding the evidence boundary. The exact source-closed constructor *value prefix* already exists as `Type3OptimizerScalarState` in `src/ops/type3_optimizer_scalar_state.hpp/.cpp`: it initializes the recovered scalar defaults and now has the recovered line tokenizer/dispatcher helpers. It is intentionally detached from `SubProject`.

An owner-field integration is not yet justified. Native code does establish a `0x50` allocation at `0x1400b5470`, calls constructor `0x140917ff0`, and publishes its result at `SUB+0x28` for type 3. But an equivalent owner cannot be represented by just this scalar state: native construction also default-constructs/clears QString `+0x28` and initializes two nontrivial shared-data members `+0x18/+0x20` through copy/ref helpers. CP128 specifically leaves their contained value and cleanup semantics open. Replacing those members with the existing `std::vector` projections would invent storage/refcount semantics.

## Why not add a `SubProject` field now

`SubProject` is declared in `include/m2m/model.hpp`; `makeSubProject` in `src/model/subproject.cpp` produces type-specific candidate state, and `Project::open` clears/replaces `p.subs` at `src/model/project.cpp:65`. Adding a `unique_ptr<Type3OptimizerScalarState>` or `optional` there would define candidate construction, destruction-on-sub-clear and replacement behavior for an object that is not yet the native `SUB+0x28` owner. The current `SubProject::~SubProject` in `src/model/subproject_lifetime.cpp` only handles its flow receiver witness. Native containment teardown order for `+0x28`, including failed/replaced project state, has not been recovered. More importantly, an incomplete scalar-only member would appear to satisfy the loader's nonnull `+0x28` check while being insufficient for `.OPT` parser writes and later ShapeOptim getters.

The native construction-site null case also is not equivalent to a simple value object: `operator new(0x50)` returning null stores null, while constructor-exception cleanup/partially initialized member unwind remains unqualified. A C++ RAII wrapper would choose a concrete unwind policy rather than reproduce that unresolved native path.

## Source-ready limit and next evidence

The only supported cut today is continued use of `Type3OptimizerScalarState` as a pure detached projection; no additional files or tests are required for its constructor defaults. It must not be named or wired as the SUB owner. The next owner integration needs, at minimum, a retained containing-SUB destructor/replacement/unwind path that disposes `SUB+0x28` and establishes its relation to `+0x20`, plus sufficient shared-member construction/cleanup evidence to select candidate fields with honest semantics. Only then can an exact file set/test be proposed for a SUB-scoped private owner. The later parser still needs the QFile/QTextStream and row-conversion boundary documented in CP134.

Public Type3/ShapeOptim guards remain. DOE `Study` at `SUB+0x20` is not the optimizer owner.

## Evidence basis

Read-only review of `analysis/resume135-type3-owner-next.md`, `analysis/resume128-type3-constructor-parser.md`, `analysis/resume134-type3-stream-owner.md`, `evidence/gui_mo/L5d/b5470.c`, current `SubProject` declaration/factory/destructor and Project open-clear path. Original PE identity is recorded in the cited reports as `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. No source/build/test/GUI/original-process/mutable-REA action was performed.
