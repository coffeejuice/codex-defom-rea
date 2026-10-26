# CP121 Project feature-anchor extraction review

## Result

No actionable extraction defect was found in the frozen source inspected. The current declarations and definitions follow the boundary plan in `analysis/resume121-feature-anchor-boundary-design.md`; the private type identities remain nested under `m2m::Project`, the Project-owned `shared_ptr` member stays in place, and the textual p11 compile path remains compatible with its `m2m` namespace rename.

## Source checks

- `include/m2m/model.hpp` forward-declares the two scoped enums with `: int`, the anchor class, and the report struct. The existing `std::shared_ptr<FeatureContextAnchor> featureContext_` remains in the original private-member sequence. The getter and report method signatures retain their nested type names; no Project member, base, public native lifecycle hook, or extra owner was added by this extraction.
- `src/model/project_feature_context_anchor.hpp` defines those same nested types under `namespace m2m`. Enum declaration order is `PassiveShell`, `ProxyAllocationPending`; stop order is `PreparedMoUnavailable`, `ConstructorPhaseNotEntered`, `HeapProfileUnavailable`, `OwnerBindingChanged`, `ProjectStorageUnavailable`. The anchor remains `final`, non-default/noncopyable, and retains the field order `project_`, weak storage, weak slots, weak model, model identity, stage, optional f10 snapshot, backlink flag, scope-active flag, binding-changed flag. Its five native-admission traits remain `false`. The report retains fields in order: stage, stop, source occurrence, next source site.
- `project_owned_storage.cpp` includes the private header before entering `m2m::engine`, and uses the complete type in the existing `namespace m2m` Project implementation. The constructor still creates one anchor against the existing storage epoch; the destructor still invalidates its scope before member teardown. No constructor/registration/heap behavior or public Close admission was added.
- `test_project_feature_context_anchor.cpp` includes the private header for observer/enum/report use. `test_project_header_opaque.cpp` includes only `model.hpp` for Project, statically checks Anchor and Report remain incomplete and enum underlying types are `int`, and exercises opaque `shared_ptr` ownership across Project lifetime.
- `tests/p11/test_xcheck.cpp` defines `m2m` as `m2m_xc` before textually including `project_owned_storage.cpp`. Since the implementation includes the private header before entering its namespaces, the header's ordinary `namespace m2m` tokens undergo the same macro substitution. No new ordinary source dependency or namespace escape was found.

The extraction source matches the required mechanical properties in the boundary design. I did not compare compiler-produced `sizeof`/`alignof` or binary symbols; those remain build gates. The intentional C++ include migration for consumers needing complete anchor/report definitions is accurately described in the design. The six-export C ABI is unaffected by these nested C++ definitions, but final ABI verification remains a separate build receipt.

## Scope

Static source/design review only. No source edit, build, test execution, GUI control, original-process execution, or mutable REA operation was performed. No actionable finding is being reported.
