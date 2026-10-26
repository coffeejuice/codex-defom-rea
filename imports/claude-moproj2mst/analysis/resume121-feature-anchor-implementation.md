# CP121 private feature-anchor boundary: source release

The complete Project feature-context definitions now live in the private model header rather than model.hpp. Project retains the same nested type identities, members and ownership; this patch changes definition visibility and the getter's implementation location. Proxy allocation, native registration and Close remain guarded as before.

Root edited model.hpp. This lane changed only these four reserved source files:

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| NEW src/model/project_feature_context_anchor.hpp | `46be402688973a6412f17de8e340c53fead5552370b824efe60239ea3a7bc540` |
| src/model/project_owned_storage.cpp | `4c49fa2daa380612e223c41a19410a7fc30bd74194be5bd65f000b5e2986e32f` |
| tests/model/test_project_feature_context_anchor.cpp | `c73dde3a67c4d929575a89a357eefefe5d7bf8b3eb0e40dea9bbc80c9690c3d1` |
| NEW tests/model/test_project_header_opaque.cpp | `8094d713bb693b1ba904df6401a415acda579be124b4323e1ad66a97a67bea3b` |

Read-time root-owned model.hpp hash is `6ee1d43bf8112ba37d224e3f626ac88d99ca6a89f4835169d8d13242a13c65b6`. The frozen design is analysis/resume121-feature-anchor-boundary-design.md, SHA `a7408fe559527a372179af2331ad72a5abfb1443dca077f785be82b48f1f45f8`.

The new header defines all four original nested types outside Project using their qualified names, with explicit int enum bases matching root's opaque declarations. Their brace-delimited definition bodies match the CP120 source archive byte-for-byte after newline normalization; member indentation was retained for this exact extraction check. The private header uses literal `namespace m2m`, so p11's existing `#define m2m m2m_xc` textual implementation include preserves its renamed namespace. Neither p11 nor any shared header/CMake file was edited by this lane.

The storage implementation gained only its local private include and the identical `return featureContext_;` getter body. Its existing Project construction, invalidation/destruction, epoch ownership, prepared-model checks and pending-attempt/report behavior remain unchanged. Existing eight feature-context cases gained only the private complete-type include. A model.hpp-only caller that inspects complete types or enum values now needs that private include; universal public C++ source compatibility is not claimed. The C ABI is separate and unchanged.

The new opaque test uses sizeof-SFINAE to require Anchor and AttemptReport to remain incomplete through the public model header, checks both enum bases are int, checks the getter's pointer type/noexcept and Project's destruction/noncopy constraints, then retains an opaque anchor across Project scope and resets it. This verifies the out-of-line getter/destruction linkage and opaque shared ownership without inspecting private state. Existing tests retain the stronger actual epoch/timing assertions.

Static preflight receipt: analysis/resume121-feature-anchor-static.json, SHA `3f2efe8d39f7cb5fa57f07c7a8dafac21c3ae34f2ed3aa690de1dbf228037128`. Comparison input build/wave3-checkpoint120-verified/source.zip SHA is `454d4fdf3fdb996937a538c1b1c7e25928d35a9fd0af32b20caba47bca78c14d`. Checks established exact extracted bodies; unchanged storage cpp after removing the two intended additions; unchanged existing test after removing its include; unchanged Project tail after accounting for the getter declaration; resolvable quoted includes; supported Testkit macros/entrypoints; absence of C assert; balanced delimiter counts; and preserved p11 macro/include ordering. These are static checks, not compiler, ABI or runtime results.

Root validation targets are m2m_model_project_feature_context_anchor (eight cases), m2m_model_project_header_opaque (one case), m2m_p11_xcheck, representative standalone packet targets, and the normal source-stable full suite/archive. Paired compiler-bound sizeof/alignof observations for Project/Anchor/Report remain root's layout gate; source comparison alone is not a compiler layout measurement. The prospective private-header dependency set remains ten explicit storage cpp objects, one p11 textual consumer and one anchor test: 12 actions in the retained graph, with required relinks still applying. No elapsed-time savings have been measured.

All four source files are frozen and released to root; this lane holds further edits through grouped validation and archive. No build, CTest, original process, GUI, mutable REA or native heap/provider/Close activity occurred.
