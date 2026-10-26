# CP142 independent review: DOE Output Control/Regions mkpath prefix

## Review result

The private helper binds the expected Project → active Type2 SubProject → owned Study chain before filesystem I/O and computes the path from that Study's name. Its immediate `nativeMkpathAbsolute` call and retained `{path, succeeded}` attempt match the planned first effect. The failed result is represented at the warning-modal boundary; the test correctly does not simulate modal dismissal or later Save continuation. No public DoeOutput Save admission is changed.

## Owner and path binding

`attemptOutputRegionsMkdirPrefix` checks `sub.project`, `activeSubProject(project)`, `sub.type == 2`, `sub.ownedDoeStudy.get()`, `sub.doe`, `study.sub`, and nonempty `study.name`. `doeControlDir(sub, &study.name)` uses the supplied name because `sub.doe` is nonnull; `cleanPath` then adds the `Regions` child. The fixture creates a real Project with `initProject`, selects its active sub, sets Type2, constructs the Study through `makeStudy` (which sets `study.sub` and the name), and attaches it through both Study owner fields. The success assertion compares against `doeControlDir` computed from that same Study and verifies the physical directory and unchanged Study name. The Project has no viewer-model setup, so the helper/test do not depend on the later viewer check.

The helper also rejects a non-absolute derived path before calling the candidate adapter and refuses non-immediate `FileEffects` before making any attempt. The current test covers foreign-sub ownership, Type3 rejection, and non-immediate-profile rejection while preserving a seeded planned mkdir. It does not yet exercise a relative-project-path rejection, empty/mismatched Study alias, or a Study whose backlink/owned pointer is independently corrupted; those are test coverage omissions, not evidence that the corresponding code checks are absent.

## Attempt and failure order

With `nativeDirectories=true`, the helper calls `nativeMkpathAbsolute(path)` once and only then appends exactly one `directoryAttempts` entry. The success test requires the exact normalized target, one successful attempt, physical directory existence, and no queued mkdir, writes, or deletes. The failure fixture creates a regular file at the exact `Regions` target; it verifies the failed attempt remains recorded, the blocker remains a file, and the already-created parent remains a directory. The outcome is `WarningModalRequired`, so the helper does not report Save continuation or native public status.

One limit is that attempt recording follows the physical call. If allocating `directoryAttempts` storage itself throws, the directory effect may have happened without a recorded result. This is an ordinary candidate instrumentation/allocation boundary and is not addressed by this private prefix; it must not be presented as native failure parity.

## Scratch isolation and cleanup

`fresh()` roots each unique project under `m2m_test::scratchRoot()` and passes the result through `scratchOutput`, which resolves existing ancestors and rejects paths outside the configured scratch root. The filename combines test name, process ID, tick count, and a process-local serial, then requires the leaf not to exist. The blocker and parent directories are created only beneath that unique leaf. No direct temp path or external fixture is used.

The test does **not** remove these leaves after completion. This is safe for isolation and repeated runs because every invocation uses a fresh checked leaf, but it leaves accumulated scratch directories. If cleanup is desired, a guard should own and remove only this leaf, and should be declared before the `Rig` so it runs after Project/Study destruction. Do not clean the shared scratch root.

## Scope conclusion

The implementation is correctly limited to the first directory attempt and its failure-modal boundary. Its success/failure tests are consistent with the plan, and the public `SfOprDoeOutput` guard remains untouched. Recommended follow-up is optional: add the omitted path/alias rejection assertions and leaf-scoped cleanup if repeated test runs are leaving material scratch residue. No source/build/test/GUI/mutable REA change was made for this review.
