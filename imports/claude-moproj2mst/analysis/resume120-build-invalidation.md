# CP120: controlled dependency dry run for CP118 compilation

CP118's compile-volume spike is explained by its changed public `m2m/model.hpp`: 448 of the452 observed C++ compile actions depend on that header. A controlled Ninja dependency dry run reproduces exactly the full452-action compile set from the nine CP117→CP118 changed inputs, with no extra or missing objects. This supports shared-header invalidation as the dominant graph cause. It does not establish how much wall time came from each compile, machine load or filesystem activity.

No active source/candidate build file was modified, compiler/linker invoked, full suite run, GUI acquired or original target executed.

## Frozen input and copied graph

The CP117 archive SHA-256 is `2fd6b4099bf7696efb905deb600be8148e25cc9678712c8d0c957aac72895fa2`, source tree `0d73760a4b80029284c3582d2054efb6d009ebc4a9264f7d549101086895e734`. CP118 archive SHA is `5efaf3279436fb00bad9180325feabd60a94f81a2b7d7229919178aaa3e05628`, source tree `77c5fc4c27135b14fcbf7de7e64f19d14ee63f13e513f68d7cb29fdd557b5f1e`. The CP118 archive was hash-checked against its freeze receipt and extracted only into the disposable experiment root:

```text
C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\build\cp120-invalidation-577079392ba0
```

`graph/` contains copies of candidate `build.ninja`, `CMakeFiles/rules.ninja`, `.ninja_deps` and `.ninja_log`. Each copy was hash-checked before/after capture. Those original candidate files still matched the captured hashes after the experiment. `ninja -t deps` ran on the **copy**, producing retained `deps.txt`; it did not execute build commands. Its STALE labels reflect the copied graph's absent object files, not a diagnosis of original CP118 dirty state.

The source archives retain no historical pre-CP118 Ninja graph/dependency snapshot. Therefore the experiment deliberately reconstructs compile dependencies from the copied current graph and frozen CP118 source. It does not pretend to replay the original prebuild timestamps, command fingerprints or missing-output conditions.

## Controlled dry-run method

All1,155 current C++ compile edges were mapped to their explicit source inputs and retained project-header dependencies, normalized into the extracted archive; no C++ edge was omitted. `probe/build.ninja` is a stat-only dependency graph with empty owned output placeholders. Its generator rule has `command = cmd /c exit 99`, and every invocation uses `-n`, so no compile/link/build action can execute. CMake regeneration, links, command-hash differences from relocation, external SDK dependencies and original artifact contents are intentionally outside this isolated input-dependency experiment.

Frozen inputs received a uniform old timestamp and output placeholders a newer timestamp. Baseline `ninja -d explain -n` was clean. Each experimental run reset those timestamps, then advanced only selected copied inputs. The exact archived bytes remained unchanged. This control prevents copied archive timestamps or relocation from masquerading as a shared-header explanation.

| Inputs advanced in disposable copy | Scheduled C++ edges | Intersection with CP118 observed compiles | Predicted extras / observed misses |
| --- | ---: | ---: | ---: |
| None: clean baseline | 0 | 0 | 0 /452 |
| `include/m2m/model.hpp` only | 448 | 448 | 0 /4 |
| `include/m2m/engine/simcontrol300_io.hpp` only | 22 | 22 | 0 /430 |
| All nine changed archive inputs | 452 | 452 | 0 /0 |

Dry-run command durations were0.025861/0.039798/0.024756/0.039688 seconds respectively. These measure this stat-only probe, not a real build speedup. Explanation lines identify the advanced archived header as newer than each affected output. No original candidate timestamps were changed.

Archive byte comparison identifies eight changed existing files and one added test: both headers above; `simcontrol300_io.cpp`, `simcontrol300_native_io.cpp`, `project.cpp`, `project_owned_storage.cpp`; `test_simcontrol300_spring_reader.cpp`, `test_spring_page_io_roundtrip.cpp`; and new `test_project_feature_context_anchor.cpp`.

## Observed build reconciliation

| CP118 stage | Observed C++ compiles | Depend on changed model.hpp | Depend on changed IO header | No changed-input explanation |
| --- | ---: | ---: | ---: | ---: |
| Focused preflight | 157 | 154 | 9 | 0 |
| Full build | 295 | 294 | 13 | 0 |
| Combined | 452 | 448 | 22 | 0 |

These header columns overlap; they are not additive. The four observed compiles outside model.hpp's dependency set are core `simcontrol300_native_io.cpp` and tests `simcontrol300_native_io`, `simcontrol300_spring_reader`, `spring_page_io_roundtrip`. They have their changed source/IO-header explanation. Object identities include target paths, so standalone tests compiling a shared source are distinct necessary objects, not duplicate work on one object. This experiment found no preflight/full duplicate of the same target-qualified compile edge.

The retained CP118 grouped receipt measures configure2.172s, focused preflight32.031s, full-build command123.375s, grouped total163.710s. CP117 was2.219/1.859/22.828/32.989s. CP118 added130.721s grouped wall time while the graph expanded from two focused compiles/no full compiles to157/295. Full links rose285→294. Configure remained about two seconds. This evidence explains action volume; no controlled paired real builds or host utilization record proves an exact wall-time saving for a proposed change.

## One concrete mitigation preserving fidelity and coverage

Keep the broadly included Project shell stable and move `FeatureContextAnchor`'s full definition and changeable observer implementation behind its **existing shared_ptr member** into a dedicated model-private header/source. Retain nested type/function names, Project member order/layout, control-block/epoch identity, constructor timing, lifetime and ownership. Move changeable inline method bodies out of model.hpp. This needs no extra allocation, new owner or altered native admission.

Current source search finds only two non-header consumers of the anchor details: `src/model/project_owned_storage.cpp` and `tests/model/test_project_feature_context_anchor.cpp`. They can include the private complete-type header; unrelated model.hpp consumers need only its forward declarations/stable method signatures. Return-report/enumeration declarations require careful placement so existing declarations remain valid. Any proposed public C++ source-compatibility difference must be reviewed explicitly rather than assumed harmless; the six C exports must remain unchanged.

This one-time refactor itself invalidates model.hpp users and requires a normal full freeze. Its prospective benefit is that later private anchor-state/method changes need not touch the448-edge header. It does not make an actual Project layout/signature change free, and this experiment does not quantify future seconds saved. Do not hide header changes from Ninja, edit dependency files, reuse stale objects, drop standalone tests or suppress full links/tests to claim a speedup.

Retain grouped focused preflight, complete test inventory, fixture SHA manifests, source/executable bindings and full acceptance/archive. At the first subsequent private-only anchor change, capture ordinary Ninja explanations/action counts at a frozen boundary and verify the expected smaller dependency set; only paired measurements can establish the resulting time benefit.

## Retained experiment evidence

All artifacts below reside under the disposable root above:

| Artifact | SHA-256 |
| --- | --- |
| `snapshot.json` (copied graph/tool identities) | `34a279cae3edc475e16d58fe5c7c1f90abe4825a1e5654c5a1773fa4bac0fc7b` |
| `dry-run-summary.json` (all mapped/affected objects and comparisons) | `d37f4d3f476d8617625eaccbcb8f2f1d1a99c824ff5dfacfdf14e3569739b006` |
| `probe/build.ninja` | `cb9c3f9f85ff909cbd3271ba17b1f107eba214a7815c820de7cf18242f25ae23` |
| `baseline-stdout.txt` | `71aa391ec7d7e291b419a7f40fba91919e14ac2394ab4e8b66ed126e267d70f6` |
| `model-header-explain.txt` | `cb17c9712de9b8d66ea8b148b1bd37d2acabf9ade4028014f60bda65345f366b` |
| `all-changes-explain.txt` | `2276ae60f7450109733330685021ca984e6c938304a5d2ff9b98af2e758b7ac8` |

The source/header diffs, raw dependency listing, per-run stdout/explanation logs and snapshot remain available in that root. Only this report and owned disposable experiment artifacts were written. Token usage and token savings were not measured.
