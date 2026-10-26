# CP123: measured dependency set after the feature-anchor boundary

The post-extraction compiler dependency records confirm the intended boundary. A private feature-anchor header touch schedules13 C++ compile actions; model.hpp schedules451. The earlier12-edge projection gains exactly one legitimate CP122 consumer, the new storage-epoch test. The opaque model-header test remains outside the private-header set. These are controlled dry-run action counts, not measured rebuild-time savings.

No active source, test, candidate build file or timestamp was modified. No compiler/linker, full suite, GUI or original executable ran.

## Frozen inputs and method

The independently hash-checked CP122 `build/wave3-checkpoint122-verified/source.zip` is SHA-256 `0e61b30e5a06b116a85aac036233831cf1c4b54478c2d5ec23b38e1784646e4e`. Its freeze receipt binds source tree `ead1572502ec4844c977bb33ecfcfc5aa3de1ace5d8a0824f15980d6ededb913`,3462 files. The receipt's tree binding is retained; this lane independently checks archive bytes rather than rerunning the acceptance freeze.

The experiment lives only at:

```text
C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\build\cp123-anchor-invalidation-80fc8dc8a9f8
```

`graph/` contains copied candidate `build.ninja`, `CMakeFiles/rules.ninja`, `.ninja_deps` and `.ninja_log`. Each original/copy hash matched during capture; all original hashes were still unchanged at the experiment's final check. `ninja -t deps` ran on this copied graph, retaining raw `deps.txt`. Unlike CP121's projected graph, the private-header dependencies here come from actual compiler-recorded dependency entries; no proposed include edge was injected.

All1158 C++ compile edges mapped to their archived explicit source and recorded project dependencies; zero edges were omitted and zero lacked dependency records. A disposable dependency-only graph preserves those input relations while removing relocation command-hash, old artifact content and external SDK timestamp effects. Its rule is `command = cmd /c exit 99`, `generator = 1`; every invocation uses `-n`, so no rule executes. The new graph contains empty output placeholders, not compiled artifacts.

Archive input copies receive a uniform old timestamp and output placeholders a newer timestamp. Each run resets those timestamps, then advances only the selected copied header. A clean baseline prevents extraction timestamps from causing false invalidation. The exact commands are `C:/Tools/Ninja/ninja.exe -d explain -n`, working in this experiment's `probe/`; stdout and explanation streams are retained separately. This measures dependency fan-out through actual recorded inputs, not a complete production Ninja command/link replay.

## Results

| Advanced copied input | Scheduled C++ actions | Dry-run command duration |
| --- | ---: | ---: |
| None: clean baseline |0 |0.029121s |
| `src/model/project_feature_context_anchor.hpp` |13 |0.025572s |
| `include/m2m/model.hpp` |451 |0.040206s |

The13-edge private set comprises:

| Consumer | Targets |
| --- | --- |
| Ten explicit project_owned_storage.cpp objects | `m2m_core`, `m2m_p08_emit_a`, `m2m_p09_emit_b`, `m2m_p09_keywords`, `m2m_p10_base_order`, `m2m_p12_p12_writers`, `m2m_p13_p13_mdwz`, `m2m_p13_p13_writers`, `m2m_p15_express`, `m2m_p15_tree` |
| Textual source inclusion | `m2m_p11_xcheck` |
| Existing complete-type observer test | `m2m_model_project_feature_context_anchor` |
| Added CP122 complete-type storage test | `m2m_model_project_feature_storage_epoch` |

The storage-epoch test explicitly includes the private header. A set comparison with CP121's projected12 targets finds exactly this one added edge and no removed edges. p11 remains correctly included through its textual inclusion of project_owned_storage.cpp; the post-build dependency record confirms that transitive header exposure.

`m2m_model_project_header_opaque` is in the451-edge public-header set and absent from the13-edge private set. Its source includes model.hpp without private definitions and retains its incomplete Anchor/Report assertions. This is evidence that the intended opacity survives compiler dependency discovery. There is no broad model-header dependency leak in the private touch's measured set.

The extracted CP122 private header hashes to `44883c50ec41c56d7a0f596f46647d6a931de5c7fd7862faba2a1641a30fe48a`; model.hpp hashes to `6ee1d43bf8112ba37d224e3f626ac88d99ca6a89f4835169d8d13242a13c65b6`. The private header legitimately differs from the initial CP121 extraction as later feature-storage work evolved. The actual13-edge result therefore measures the implemented boundary after that work, not just the original proposal.

## Interpretation and validation limits

The private/public-header runs differ by438 scheduled compile actions in this graph. That establishes lower compilation exposure for private-only changes. The millisecond durations above measure stat traversal and dry-run output generation; they are not real compile or launcher times. No elapsed-time speedup or token saving is claimed.

Changing a core object still requires its library and applicable downstream links. This probe omits links, CMake regeneration and full acceptance; it cannot justify skipping any of them. Public shell/layout/signature changes still dirty model.hpp users. New legitimate private-type consumers can widen the private set, as CP122's storage test did.

Retain this measured boundary for private feature evolution. At the next real private-only source-frozen checkpoint, record actual compile/link actions and ordinary grouped build elapsed time with the same compiler/cache/parallelism and host-load context before attributing wall-time benefit. Keep complete focused/full tests, source/binary binding, fixture manifests, C ABI checks and final archive unchanged. No production change is required by this report.

## Retained raw evidence

Ninja binary SHA-256 is `e52a7ad9538d9618c67a0bd777964e2eec8a30f68b810a2f6adce1f2daf847b8`. `summary.json` includes the original copied graph hashes, full mapping, action sets, archive/header identities and unchanged-candidate check. Artifacts under the experiment root:

| Artifact | SHA-256 |
| --- | --- |
| `summary.json` | `03f4903fdc81fa98e1a20f51343111921d3935d122a2a30aaf9498f995468bf2` |
| `probe/build.ninja` | `4ca2692d270e8ad0baad098ea4e7da013cb6c2661b9af23c2d359eefcb2866ec` |
| `deps.txt` | `a45058b8073b82275b6424c9c038d2527d3575e8399f155a7498b3e73ddab6ea` |
| `private-header-explain.txt` | `0cabff00c9d4802c7a6d22ed44ab7fa212919eecf7205751bc2a49eb632a72ee` |
| `model-header-explain.txt` | `a2e11b1ba33e25a8f6fa4d8aa6eeef1d99f59f24f53eaa0dac375081bb529189` |

Per-run raw stdout/explanation files, copied original Ninja inputs and extracted frozen source remain in that owned leaf. Only this report and disposable measurement artifacts were written. No active candidate writes or test execution occurred. Token usage was unavailable.
