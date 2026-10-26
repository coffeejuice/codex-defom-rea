# CP81 build invalidation review and incremental compile design

## Purpose

Explain the 64.640-second CP79 full-build phase after a successful focused preflight using the frozen CP79 build receipt, Ninja output, and that source snapshot's CMake target graph. Propose a controlled incremental compile experiment without running a build or modifying the candidate.

## What the receipts show

The relevant receipt is `analysis/resume79-integrated-grouped3-build.json`: source hash `22828eda881712488bad653fdcf0a2e05b2cebdfe32934974865b13922f1e14c`, 3,347 files, source stable before/after, and 256 artifact hashes in the completed receipt. Its phases were configure 1.954 seconds, focused preflight 17.532 seconds, then full build 64.640 seconds (total 89.848 seconds). The preceding CP79 grouped2 receipt completed a full build in 60.750 seconds but was bound to a different source hash (`730a509d…`, same file count). Therefore the CP79 runs are not a same-source incremental timing comparison.

The focused-preflight log has 67 C++ object builds and 7 links, including `m2m_core.lib` and six focused test executables. It is successful, but it only prepares the requested target subset. The following full-build log contains another 100 C++ object builds and 213 link actions (314 numbered tasks total). The log begins by linking `moproj2mst.dll`, then compiles remaining test objects and links the broad set of converter/test executables. CMake's frozen CP79 `CMakeLists.txt` defines `m2m_core` as a static library, builds each discovered `test_*.cpp` as its own executable, and links the normal test executables to `m2m_core` plus `testkit` (lines 90–96 and 116–164). The graph therefore has substantial work after a narrow successful preflight: the unselected test object files still need compiling and the full target set still needs linking against the built core library.

The 64.640 seconds is consequently accounted for by real full-target work, not a long configure: 100 remaining object compilations and 213 links under `--parallel 4`. The receipts/logs do not give per-action durations, so they cannot show how much time was compilation versus linking or which individual action dominated. Source fingerprints changed between the CP79 grouped2 and grouped3 attempts, further preventing attribution to configure reuse, a particular source edit, or a clean incremental-build baseline. No cache-statistics receipt is available to quantify compiler cache hits.

## Controlled incremental compile experiment

On a future frozen checkpoint, use a disposable copy of its source archive and a separate disposable build tree; never reuse the live `build/candidate`. First configure/build the full target once and record the source content fingerprint, CMake cache hash, complete 256-style artifact path/size/SHA map, and `ninja -t query` output for one representative test target. Verify the source fingerprint matches the archive and every produced artifact is hash-bound.

Then run a no-change full build and require zero compile/link actions, unchanged source fingerprint/cache hash, and an identical complete artifact map. In a second copy of that already complete scratch build, change only the modification time of one ordinary test `.cpp` without changing its bytes. The source fingerprint (which hashes relative paths and file bytes, not mtimes) must remain identical. Build its test target followed by the full target. Preserve the raw Ninja log/action count and complete pre/post artifact map; require that the action fanout matches the graph, the target executable's resulting hash is captured, and all other artifact hashes are checked. This yields a causal baseline for an actual one-file incremental compile while validating the source and artifact binding contract.

This experiment is deliberately narrower than the 64.640-second full target sweep. It measures whether the fully built snapshot is incrementally clean and how one leaf invalidation propagates. It will not claim that such an incremental edit reproduces the CP79 broad-build cost or identify the per-action share of that cost. No candidate build or source mutation was performed for this review.
