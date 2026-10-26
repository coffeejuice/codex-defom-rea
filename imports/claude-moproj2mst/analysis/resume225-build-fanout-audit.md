# CP224 build fan-out audit

## Evidence and limits

The retained full-build log is `build/logs/cp224-two-owner-fixed-full-build-05.log`; its build receipt is `analysis/cp224-two-owner-fixed-full-build.json`. The full build command took 150.437 s after a 2.406 s configure. The log contains 696 numbered build edges (Ninja counter reaches `[696/696]`); directly counted output lines include 342 `Building CXX object` and 353 `Linking` records. These counts describe printed actions, not unique translation units: Ninja restarts/rechecks and the shared DLL build can make counters appear as both 702 and 696. The receipt has no retained `ninja -t graph`, `-t query`, or `-d explain` output. Therefore exact attribution of those 696 edges to `engine.hpp` versus `model.hpp` is unavailable from existing evidence.

CP224's configured source-tree identity is 3,561 files with SHA-256 `7389248268ad2c65d9c871c2aebb62afcdfd1789c1f0325ef4e7d7dcadc84fe8`. It cannot be compared as a single-change baseline against CP223 or CP222 to isolate two header edits: source-tree digests cover the entire tree and no before/after per-file edit manifest is embedded in this receipt. `rg` of current include directives shows both headers are widely used by production and test code, consistent with broad dependency fan-out, but textual inclusion counts do not equal scheduled compile edges.

## Concrete experiment for a later matched build

Use one unchanged candidate build tree and a fixed baseline source snapshot. First record Ninja `-t query` output for `engine.hpp` and `model.hpp` and `-d explain` on the naturally required build after touching each header independently in disposable matched worktrees; record compile and link edges separately. Then try a narrow split of the mutable engine declaration surface into an internal header used only by the affected engine clients, keeping `engine.hpp` as a compatibility umbrella. Re-run the same touched-header experiment and compare edge counts and wall times. If model.hpp proves to dominate, evaluate one independent model seam only after the engine experiment. Preserve API compatibility, source/binary bindings, fixture hashes, the full suite, and historical evidence unchanged. This report makes no claim that either header alone caused CP224's work.

## Result

The measured 150.437-second full build and 696 logged Ninja edges establish a costly broad incremental build. The saved artifacts do not support a precise per-header causal count; a saved dep graph or Ninja explanation is necessary before claiming one.
