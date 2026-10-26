# CP121: independent private-anchor boundary/build review

The proposed extraction has no structural layout/C-ABI blocker, provided it retains the exact nested type identities, nonstatic data-member order/defaults and existing shared_ptr ownership. A controlled projected dependency dry run gives12 compile actions for a later private-header edit, versus448 for model.hpp. This is a compile-set projection, not a measured build speedup or compiled refactor acceptance.

Reviewed proposal: `analysis/resume121-feature-anchor-boundary-design.md`, observed SHA-256 `a7408fe559527a372179af2331ad72a5abfb1443dca077f785be82b48f1f45f8`. Its mechanical extraction/write-set and explicit C++ source-include migration are the review contract. The final proposal incorporates the independent dependency-attribution correction: p11 is a textual cpp consumer rather than an eleventh ordinary project_owned_storage.cpp object, and its renamed-namespace compatibility/focused gate are explicit.

## Independent dependency projection

The experiment reuses CP120's retained1,155-edge compile graph and verified CP118 archive, in a fresh disposable leaf:

```text
C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\build\cp121-anchor-boundary-3237f8ca16c5
```

No active source/test or candidate build path was written. The archive was extracted only into this leaf. `probe/build.ninja` copies the prior dependency-only graph and adds the proposed header as a dependency of all complete-type consumers, including textual cpp-include closure. The proposed header in this scratch is only a dependency token, not an implementation. Output placeholders and uniform timestamps produce a clean baseline. Every command is `ninja -d explain -n`; no compiler/linker/build action executes. As in CP120, generator rules avoid relocation command-fingerprint noise; actual production command hashes, external SDK dependencies and link actions are outside this probe.

| Changed copied input | Scheduled C++ actions | Dry-run command duration |
| --- | ---: | ---: |
| None: baseline | 0 |0.030431s |
| Proposed private complete-type header |12 |0.029670s |
| Existing public model.hpp |448 |0.043123s |
| Existing project_owned_storage.cpp |11 |0.029808s |

The durations describe the stat-only probe. The436-edge difference between private/public-header runs is conditional on the proposed include boundary; it does not remove downstream relinks or establish wall-time savings. The one-time extraction edits model.hpp and therefore incurs its broad invalidation once.

The projected12-edge private-header set is:

| Source/consumer | Compile targets |
| --- | --- |
| Ordinary project_owned_storage.cpp translation units:10 | `m2m_core`, `m2m_p08_emit_a`, `m2m_p09_emit_b`, `m2m_p09_keywords`, `m2m_p10_base_order`, `m2m_p12_p12_writers`, `m2m_p13_p13_mdwz`, `m2m_p13_p13_writers`, `m2m_p15_express`, `m2m_p15_tree` |
| Textual source inclusion:1 | `m2m_p11_xcheck` |
| Anchor observer test:1 | `m2m_model_project_feature_context_anchor` |

`tests/p11/test_xcheck.cpp:22..29` defines `m2m` as `m2m_xc`, then directly includes project_owned_storage.cpp alongside other sources. The copied dependency log independently records that cpp as a dependency of p11's test object. It therefore also inherits the new header dependency. A scan only for direct new-header include statements would miss it. The private header's ordinary `namespace m2m` tokens must remain compatible with that existing macro; do not bypass it through a differently spelled namespace or introduce a separate implementation TU/link requirement. No p11 source edit is implied. Its focused build/run remains required coverage.

The proposed opaque-header test has no edge in this archived graph yet. If it includes model.hpp but no private definition as designed, later private-header changes should not dirty it. Future actual complete-type consumers can legitimately widen the12-edge set. Root must check actual compiler-discovered dependencies after extraction; this injected dependency graph does not prove includes compile.

## Layout, identity and compatibility review

Moving nested enum/class/report definitions does not itself add Project object storage. The proposal retains the exact `std::shared_ptr<FeatureContextAnchor>` member, field order/access, all other members and deleted Project copy/move operations. It retains the same nested qualified types rather than replacements/aliases. Both scoped enums have underlying int today; explicit `: int` forward declarations/definitions preserve representation. The relocated Anchor and report must retain their own exact fields/defaults/order and false native-admission traits.

Returning a shared_ptr to an incomplete nested class remains a valid boundary when construction/destruction needing the complete class stays in the existing storage implementation. Moving the getter's identical shared-pointer-conversion body out of model.hpp adds no allocation, control block, epoch or callback. Keeping required definitions in project_owned_storage.cpp preserves the standalone and p11 textual-source linkage model.

The proposal correctly documents an intentional C++ include migration. A model.hpp-only consumer that calls anchor observers, names enum enumerators, uses the returned report value or inspects class size/traits must include the private complete-type header. This is not universal public C++ source compatibility. Merely owning/resetting its shared_ptr or declaring signatures remains the intended opaque boundary. The separate six-export C ABI/header/version must remain unchanged; those exports do not expose these C++ types.

These are structural source arguments, not compiler-measured sizeof/alignof or binary ABI observations. Root should bind before/after layout evidence under the same compiler/options where required. Do not use offsetof on non-standard-layout Project or equate its adapter layout with original native offsets. Retain actual destructor epoch invalidation order, weak storage/control-block identity, pre-f10 constructor-attempt timing, sticky rebinding refusal and all eight existing anchor cases.

## Acceptance requirements and result

No design blocker is identified. The frozen proposal corrects p11 attribution/coverage; preserve that namespace convention during extraction. Do not add another pimpl owner, new allocation, lifecycle/provider/heap behavior, public admission or altered enum values to this mechanical boundary patch.

The optional opaque-header compile gate is useful: Anchor/Report remain incomplete, enum representation is int, Project construction/destruction and incomplete shared_ptr ownership link successfully without private definitions. Root should group both model targets, ordinary standalone sources and p11 before the full source-stable suite/archive. Required static-library/test relinks, complete inventory, fixture hashes and source/executable bindings remain unchanged. Full test coverage cannot be inferred from these12 dry-run actions.

Retained artifacts under the experiment root:

| Artifact | SHA-256 |
| --- | --- |
| `summary.json` | `af9954eb1ce9a630be21c1f50e2923dd50907057e50335df85fdd6bd0c6b8371` |
| `probe/build.ninja` | `072e2bf4f41c32e83f3c6068935ab88b7421ccd1aa0972e801ef9b0c99efad75` |
| `private-header-explain.txt` | `229f3a148074da96c771c4d5704b12841ecf8a27f640d79e40d90e852b63f18f` |

Raw per-run stdout/explanation files and the copied frozen inputs remain in that leaf. Only this report and owned disposable dependency-probe artifacts were written. No production source/test/CMake/candidate build edit, compilation, test execution, GUI, original launch or mutable REA occurred. Time/token savings were not measured; token usage was unavailable.
