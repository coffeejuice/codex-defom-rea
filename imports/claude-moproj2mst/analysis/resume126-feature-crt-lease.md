# CP126 canonical candidate runtime lease: source release

The private runtime can now distinguish its genuine retained shared owner from an unrelated control block containing the same raw address. This is candidate ownership identity only. A matching lease does not assert heap readiness, original-global observation, Project binding, native startup/cleanup or Close admission.

The three reserved existing source files are frozen:

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| src/model/feature_crt_runtime.hpp | `25b1cc1059fa225017bec07dd5289b9084f92c12fdd78f0bade562d3ce65071c` |
| src/model/feature_crt_runtime.cpp | `89989f0d68c6bf7657b8d86e587ebfe550ca95a457c2f385efd5279a01911be8` |
| tests/model/test_feature_crt_runtime.cpp | `1529ac9ef022c4e0c0d11b51e94f59832d789f4ea6d941b1054a2e1a5e564c61` |

The header declares `bool FeatureCrtRuntime::matchesRetainedLease(const std::shared_ptr<FeatureCrtRuntime>&) const noexcept`. Private Storage gains one weak canonical lease. The existing retaining factory initializes it once from its actual shared owner inside the function-local static initializer, before returning/publishing that owner. It is not reassigned on subsequent factory calls and creates no strong ownership cycle.

The matcher rejects null or pointer mismatch, locks that canonical weak lease, verifies its pointer is the same runtime, then requires both owner_before comparisons to be false. It invokes no factory, producer or Windows kernel function and leaves no retained temporary owner after return. The original candidate process/module retention policy is unchanged. No registry of raw Project addresses, default runtime creation or fallback heap was added.

The runtime Testkit executable now has six cases. The additional case prepares metadata before HeapCreate and checks genuine/copied ownership, empty leases, a null-pointer alias sharing the genuine control block, and a same-raw-pointer lease with a separate no-op control block. The helper explicitly confirms that the forged lease has a different owner and is rejected. It compares the complete observable producer state/counters/effects before and after checks. Existing actual-kernel coverage repeats those checks after successful production and still requires one HeapCreate only. Existing declared-model, source-role, private-constructor and process-retention checks remain intact. No second runtime or original process is fabricated.

Static preflight compared source with build/wave3-checkpoint125-verified/source.zip, SHA `b7327139fa179c184b51b248243e5fa281bf699cda346ed3f6f816fd3c942d4f`. It verified that constructor, producer, report and declared-result-projection function bodies remain identical; only private ownership storage/factory setup/matcher and tests changed. It checked exact pointer/control-block matching, no factory/kernel call in the matcher, weak assignment before factory return, resolved quoted includes, balanced delimiter counts, supported Testkit macros/entrypoint, six cases and absence of C assert. No compile or runtime result is claimed by this preflight.

Public model.hpp remains `6ee1d43bf8112ba37d224e3f626ac88d99ca6a89f4835169d8d13242a13c65b6`; Project storage cpp remains `af2e1da0fe4bdc582e29731b2f0e6ce7d7f0a882ea2ade477f164807da1199e8`. No shared model/Project/Session/C ABI/CMake file was edited. The existing target remains m2m_model_feature_crt_runtime / m2m.model.feature_crt_runtime; standalone/p11 storage compilation acquires no new dependency.

The design/evidence report is analysis/resume125-feature-crt-binding.md SHA `f66eb5db2c08ed54654ff32139f9d92a1b4193946fc7684c6178a996f196246f`. Its original module/constructor/global-slot/late CRT cleanup qualifications remain unchanged. The default Project guard stays closed, and original heap slot144700810 remains a static source role rather than a live observation. Authentic candidate lease identity alone never enables that binding or actual proxy allocation.

All three files are released to root and held through grouped build/tests/archive. Only this new analysis receipt accompanies them. No build, CTest, live heap, GUI, original process or mutable REA execution occurred in this lane. Root owns observed compiler/runtime acceptance.
