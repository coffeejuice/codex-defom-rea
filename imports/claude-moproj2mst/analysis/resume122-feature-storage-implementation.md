# CP122 passive typed feature epoch: source release

Project now constructs its existing storage marker through a private typed bundle factory. The marker's shared control block owns that bundle through a typed deleter; weak epochs retain their existing Project lifetime boundary. This is passive adapter storage only. No source CRT heap, proxy/map object, constructor stage, feature value, native cleanup or public admission was added.

The exact reserved source set is frozen:

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| NEW src/model/project_feature_storage.hpp | `4890457bd665909fca8f6059bd09093b8b5bfab63aff12f3b4f9d2bb72f53ec0` |
| src/model/project_owned_storage.cpp | `af2e1da0fe4bdc582e29731b2f0e6ce7d7f0a882ea2ade477f164807da1199e8` |
| src/model/project_feature_context_anchor.hpp | `44883c50ec41c56d7a0f596f46647d6a931de5c7fd7862faba2a1641a30fe48a` |
| NEW tests/model/test_project_feature_storage_epoch.cpp | `03b798bfa44a3e61c412af8bec75e66cc4352d39de78fbe16c07db051e3381a0` |

Root explicitly approved the conditional private-anchor bridge. That header gains only a forward declaration and friend for detail::FeatureContextAccess; its fields, defaults, observer bodies and enum/report definitions remain unchanged. Public model.hpp and the empty final ProjectStorageLifetime marker header are byte-identical to CP121: hashes `6ee1d43bf8112ba37d224e3f626ac88d99ca6a89f4835169d8d13242a13c65b6` and `0486d2a65638576a3544d3cf7732a64e84d5f808bdd88202daf393deb463264c` respectively.

The private header exposes makeFeatureEpoch, opaque typed bundle lookup and the actual-anchor hasTypedEpoch diagnostic. Bundle and deleter definitions live only in the existing storage cpp. Typed lookup uses std::get_deleter and verifies that the supplied marker address is the bundle's actual marker; foreign owners, null tokens and wrong-marker aliases refuse. It uses no guessed layout offsets or raw-Project registry. The bridge checks actual source-scope liveness and borrows the anchor's real weak epoch only for synchronous lookup; it returns a Boolean and keeps no owner.

The factory allocates the host bundle, then constructs shared_ptr<const Marker> with a trivial bundle-owning deleter. A shared-control-block construction exception invokes that sole deleter; no parallel unique owner can double-delete. Last strong ownership deletes the host bundle once even if weak control-block observers survive. This changes host allocation shape from one make_shared allocation to separate bundle/control-block allocations; those are adapter allocation failures, not claimed native operator-new/CRT behavior. Neither deletion nor Project/Session release emits native effects.

Project's only behavioral source edit is its epoch initializer. Existing member order, source-scope invalidation at Project destructor entry, later epoch expiry, Mo-slot weak borrowing and Session backlink clearing remain intact. All definitions remain in the existing cpp's compilation closure, including the renamed p11 textual include. No CMake or p11 source edit was required; literal namespace tokens preserve macro expansion.

The new Testkit executable has seven cases:

* last-strong deletion once while a weak control block survives;
* foreign owners, distinct bundles and wrong/null marker aliases;
* first host allocation failure without a produced owner;
* shared-control-block host allocation failure with exactly one bundle deletion;
* an actual Project shell using the typed epoch while native construction stays passive;
* an actual prepared/registered Session Mo retaining slots after Project/epoch release;
* placement reconstruction at the same Project address producing a different epoch and refusing the escaped old anchor.

The failure cases replace operator new/delete only inside this new executable. A small RAII-armed probe restricts injection to the factory call, resets before Testkit assertions, and counts deletion of its first host allocation. This tests host ownership exception safety; it is not a native allocation-failure observation. The Session test uses a missing-file probe below configured M2M_TEST_SCRATCH and performs no original execution.

Static receipt analysis/resume122-feature-storage-static.json has SHA `aae6d4c75f3de31075e4eb71cac4fcadb73712b9d5ef926ea631852e0b306356`. Comparison binds CP121 source.zip SHA `26066609d465bc642de364cb07e21b5f8082ef9144e01418f40fc03ce5d90e97` and verifies the limited cpp delta, private friend-only delta, unchanged public/marker headers, resolved quoted includes, supported Testkit macros/entrypoint, absence of C assert, balanced delimiter counts, scratch containment, namespace convention and absence of Windows heap APIs. No compiler or test was executed by this lane.

Root's first focused compile rejected the test's call to private ConversionSession::releaseProjectStorage. The repaired Session case uses its public destructor via a bounded scope and retains the actual registered ModelManager, Mo child, slots and anchor outside that scope. It then checks backlink clearing, epoch/source-scope expiry, typed-lookup refusal and unchanged retained child/slot ownership. No production API was widened and no native Close precondition/result is claimed. Static preflight was repeated on the repaired seven-case test; root owns the rebuild and actual results. All production hashes above remain unchanged.

Root's focused target is m2m_model_project_feature_storage_epoch; CTest name m2m.model.project_feature_storage_epoch. Existing anchor and opaque-header targets remain relevant, together with m2m_p11_xcheck, representative standalone packet links and required full acceptance. Runtime outcomes, compiler/RTTI compatibility and allocation probe behavior await those actual runs; static preflight does not certify them.

The earlier design remains analysis/resume122-feature-proxy-next.md SHA `17d0b0f43773c84bd73b418d5cec3195af704b0f985d87cc10695066bc9b2995`. Its actual runtime heap producer, first-occurrence proxy execution and unresolved-block retirement dependencies are still queued. Only passive storage was implemented. All four source files are released and held unchanged through root validation/archive. No build, CTest, GUI, original process, mutable REA, native allocation/cleanup or Close activity occurred.
