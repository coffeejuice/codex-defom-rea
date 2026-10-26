# CP121: move feature-anchor definitions behind the existing pointer

The smallest useful boundary keeps Project's existing shared-pointer field and object layout, but replaces the four nested feature-type definitions in model.hpp with declarations. A model-private header supplies their complete definitions to the two actual consumers. The implementation stays in project_owned_storage.cpp so standalone packet targets acquire no new external link dependency. This is a structural proposal only; no source/header/test/CMake was changed and the queued proxy implementation remains queued.

CP120 measured the dependency cause: model.hpp has 448 users among 452 observed CP118 compile actions, and its controlled copied-graph dry run reproduced all 452 from the nine changed inputs. It did not measure a speedup for this proposed refactor. The retained graph has ten explicit project_owned_storage.cpp object edges (core plus nine standalone targets), one p11 cross-check translation unit that textually includes that cpp, and one anchor test. Those are 12 projected compile edges for a later private-definition change. Required library/test relinks and full acceptance still apply; no wall-time saving is claimed.

## Exact proposed write set and source lines

Line references bind the current file hashes below; future source movement requires re-resolving anchors.

### 1. `include/m2m/model.hpp`

Replace **lines296..341**, ending before the existing `private:` at342, with:

```cpp
    // Complete feature-context types are model-private; this shell only owns the pointer.
    enum class FeatureContextStage : int;
    enum class FeatureContextStop : int;
    class FeatureContextAnchor;
    struct FeatureContextAttemptReport;
```

Both current scoped enums already use the default underlying type int; spelling it explicitly in the declarations and relocated definitions preserves that representation. Forward-declared scoped enums have a fixed representation; no enum object is added to Project. Report is only a return type here and may remain incomplete until a consumer uses its value. Anchor is only shared-pointer-owned and may remain incomplete here.

At **line367**, replace the inline getter body with its identical signature:

```cpp
    std::shared_ptr<const FeatureContextAnchor> featureContextAnchor() const noexcept;
```

Keep line368's report method declaration, all constructor/destructor/function signatures, `friend initProject`, and every nonstatic member **including the shared_ptr at350** unchanged and in the same access/order. Do not add a pimpl pointer, heap/state payload, destructor action or lifecycle method to Project. Keep the existing `<optional>` include at21 in this first minimal patch; removing a transitive include is unnecessary for the invalidation boundary and could introduce unrelated include-hygiene churn.

### 2. NEW `src/model/project_feature_context_anchor.hpp`

Use a local/private header, with no CMake interface/public include installation change:

```cpp
#pragma once
#include "m2m/model.hpp"
#include <optional>

namespace m2m {
enum class Project::FeatureContextStage : int { /* exact old296 enumerators */ };
enum class Project::FeatureContextStop : int { /* exact old297..300 enumerators */ };
class Project::FeatureContextAnchor final {
    // Exact old304..334 contents, including public destructor/deleted copy,
    // all observer bodies, false traits, friendship, fields and defaults.
};
struct Project::FeatureContextAttemptReport {
    // Exact old337..340 fields, in their original order.
};
} // namespace m2m
```

The comments above are extraction instructions, not replacement placeholder implementations. Preserve all enumerator names/order/values, field types/order/defaults, deleted operations, final, noexcept and native-admission false constants exactly. Define enum values before the class's initializer expressions. The type names remain **m2m::Project::FeatureContextAnchor**, Stage, Stop and AttemptReport; do not introduce equivalent namespace-level replacement types or aliases with different identity.

This header includes model.hpp; **model.hpp must never include it back**, directly or via another broad model dependency. No other shared header should include it merely for convenience. The one-time extraction may retain the observer inline bodies in this private header unchanged; later source-only observer implementation moves into the existing storage cpp can be evaluated independently. No new cpp is needed for this boundary.

Preserve the existing textual-include convention: tests/p11/test_xcheck.cpp:22 defines `m2m` as `m2m_xc`, then includes project_owned_storage.cpp at line27. The new header's literal `namespace m2m` expands under that macro, just as model.hpp and the implementation do today. Do not hide the namespace in a macro-resistant alias or use a differently named namespace. Keep `#pragma once` consistent with the existing headers and do not add the normal-namespace private header to p11 before its renamed include. The p11 source needs no edit, but its transitive private-header dependency and renamed symbols are required acceptance gates.

### 3. `src/model/project_owned_storage.cpp`

Add `#include "project_feature_context_anchor.hpp"` immediately after its current direct model.hpp include at**line9**. Its constructor, destructor, attempted binding, report and epoch methods already need the complete nested type and remain otherwise unchanged.

Add the relocated getter definition after the current accessors at**lines195..196**, before namespace close197:

```cpp
std::shared_ptr<const Project::FeatureContextAnchor>
Project::featureContextAnchor() const noexcept
{
    return featureContext_;
}
```

This is the identical shared-pointer conversion/body, relocated to the translation unit that already constructs/deletes the anchor. It adds no allocation or owner. Keeping it here matters: the retained graph compiles this file directly into nine standalone targets in addition to core, and the p11 cross-check includes it textually under the renamed namespace. Do not move its required definitions into a new lifecycle cpp and accidentally create unresolved symbols in those targets.

### 4. `tests/model/test_project_feature_context_anchor.cpp`

Add `#include "../../src/model/project_feature_context_anchor.hpp"` after the current model.hpp include at**line5**. Keep its eight identity/timing/epoch tests and existing static false/deleted-operation assertions unchanged. This is the only current direct non-implementation consumer of complete anchor/enum/report details; p11 consumes the implementation transitively. The relative private include resolves from tests/model without new include-directory/CMake declarations.

### 5. Focused boundary compile gate (recommended small test)

NEW `tests/model/test_project_header_opaque.cpp` should include **model.hpp only** for Project definitions, plus Testkit/memory/type_traits. A standard sizeof-SFINAE completeness probe must establish that Anchor and AttemptReport are incomplete, while the two scoped enum underlying types are int. This prevents an accidental convenience include from leaking the private definitions back into the448-user header. A small runtime case may construct/destroy a Project, obtain/retain/reset its `shared_ptr<const incomplete Anchor>` without including the private header; do not call anchor observers or the report method in this opaque-consumer test. Existing out-of-line Project destruction and relocated getter must satisfy linkage. No native feature/heap/Close activity is required or claimed.

This extra test is an architectural completeness/ownership compile gate, not a copy of native allocation behavior. It includes no private header, so a later private-only edit should not invalidate it. Existing configure-dependent test discovery supplies its target after root configure; no worker CMake edit is required. If root chooses the absolute four-file minimum, the broad full build still tests existing incomplete-type consumers, but lacks this explicit negative-completeness guard.

## ABI, source compatibility and lifetime gates

The proposal changes no Project nonstatic data member type, declaration order, access or base; nested type definitions, enum definitions and function bodies themselves consume no Project object storage. `shared_ptr<Anchor>` remains the exact field; no second allocation, storage epoch, ownership graph, cleanup action or callback is added. The relocated Anchor/Report retain exact member order and values, so their own representation must remain unchanged in this extraction. Qualified nested names and method signatures remain identical; C ABI headers/version/six exports stay unchanged.

Root should retain compiler-bound before/after sizeof/alignof observations for Project, Anchor and Report under the same architecture/options, plus enum-int representation and current copy/move constraints. Do not use portable offsetof assertions on the non-standard-layout Project or equate this C++ layout to the original PE Project+60 layout. One-time extraction changes definition placement, not native offset/source provenance. Keep the actual epoch/Project destructor scope invalidation before member teardown, prepared Mo/slot/control-block checks, f10 timing witness and sticky owner-mismatch behavior unchanged; existing eight tests are direct acceptance for those invariants.

There is an **intentional C++ include migration**: model.hpp-only code which currently calls anchor observers, uses enum enumerators, constructs/inspects the report, or asks sizeof/traits of the anchor must add the complete private header. The repository-wide source search finds two direct complete-type consumers plus the p11 textual implementation consumer. This is not universal public C++ source compatibility. Merely holding/resetting the shared pointer or declaring function signatures remains supported with the model header. If root requires unknown external C++ callers to retain all former transitive definition visibility, the four-definition extraction is not that contract; keeping those definitions public would retain broad invalidation on their edits. The six-export C ABI is a separate stable contract and does not expose these C++ types.

No new public native admission arises from a forward declaration or static-trait relocation. CP120's private epoch-bundle/resource-retirement/stage-contract questions are unchanged. Do not combine this patch with HeapCreate/HeapAlloc, registration, new enum states, native destructor/resource policy or a Project interface expansion. Keeping this mechanical change separate makes layout, epoch and graph acceptance reviewable.

## Verification and prospective dependency measurement

The refactor itself edits model.hpp and will invalidate its broad users once; root must perform normal focused builds (both model tests, representative standalone targets and explicitly m2m_p11_xcheck), required full build/tests and a source-stable archive. The p11 target establishes the macro-renamed textual-include and relocated-getter link boundary. Do not reuse old objects to conceal this cost. Expected future dependency exposure is based on the current copied graph, not an executed build after this proposal:

| Later change | Expected source consumers in retained graph |
| --- | --- |
|private complete-type header only|10 explicit storage-cpp object edges +1 p11 textual consumer +1 current anchor test =12 compile edges|
|storage cpp method implementation only|10 explicit object edges +1 p11 textual consumer =11 compile edges; required downstream links still apply|
|Project layout/signature/public shell change|model.hpp448-user exposure still applies; this boundary cannot erase it|

The proposed new opaque test includes model.hpp and deliberately no private definition, so private-only changes should not add it to the first row. Future actual constructor consumers may legitimately widen the private dependency set. At the first later private-only edit, capture ordinary Ninja explanations/dependency/action counts at a frozen boundary and verify the actual set; a paired measured build is required before claiming elapsed-time savings. Preserve all full-suite/fixture/source/executable/hash gates. No dependency hiding, stale-object reuse, weakened test inventory or skipped links is proposed.

## Immutable input receipt

Source hashes read: model.hpp `5653c60d568ee530be41aa35813d3f87cca418f5b9c94e9f7f34cf7ddefc99c6`; project_owned_storage.cpp `d8a98bcba68e52c8c1197a74df4a56a43874a91594be25f7aedb3dd7f24f69ac`; anchor test `cb84c0926c0a6ed7b3290bf5fd906793201324c37a96bd10ce37afdcfbe947ee`. CP120 build-invalidation report SHA-256 `600c766307beefc355f41ec98c310a92451c2f5cdc528cd442f0fa17ccb0c67b` binds the copied graph experiment at build/cp120-invalidation-577079392ba0. The only new graph analysis here counted its existing target-qualified source edges; no copied/active graph or timestamp was changed and no Ninja/build/compile command ran.

Only this exclusive design report was written. No shared source, proposed header/test, lifecycle API, build, GUI, original process or mutable REA work occurred. Implementation awaits root's integration window and explicit source ownership. Requested source reservation is model.hpp, NEW project_feature_context_anchor.hpp, project_owned_storage.cpp, the existing anchor test and NEW test_project_header_opaque.cpp; all paths are those specified above. No p11 or CMake source edit is requested.
