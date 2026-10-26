# CP75: isolated canonical +3c8 cell implementation freeze

The isolated CP65 lazy getter and CP68 typed window projection now share one canonical `+3c8` cell through `WidgetAreaLazyParent::bindWindowOwner`. No Session, public Open/Save, constructor execution, Qt delivery, Boolean Apply or PostPending path was enabled. A newly created window cell is unavailable; historical constructor return does not seed null.

The five files below are frozen for root's coordinated validation. This lane did no compilation, build, CTest/full suite, GUI, original execution or mutable REA. Static preflight is recorded in `resume75-boolean-canonical-cell-preflight.json`.

## Implemented boundary

- One cell holds unknown/null/nonnull knowledge, pointer identity, weak allocation generation, optional weak typed target, revision, commit/store-occurrence accounting and active getter identity. Window typed snapshots and lazy raw snapshots view this same storage; no snapshot-copy synchronization is used.
- Repeated binding to the same window reuses its live lazy wrapper. Canonical control-block checks reject forged window/parent/allocation aliases even when raw addresses collide. Unknown native-alive state remains unknown; a known-dead or revoked parent cannot continue this helper.
- An initial typed observation binds the same raw allocation. After a raw commit, the same API may bind the corresponding typed record without replacing the field value. A different raw allocation or typed-record lifetime is refused. Known nonnull remains nonnull if allocation/typed aliases expire; the getter guards unavailable return bindings instead of taking the null branch.
- The existing staged allocation/constructor-return/no-commit-on-interruption order remains. A window-bound ParentCommitPending additionally requires an explicit184cb observation carrying parent allocation generation, this getter's invocation identity, expected cell revision and a positive unconsumed store occurrence. Missing input retains the pending guard; invalid input refuses before a field commit. Source inputs are declared observations, not a native readiness detector.
- The cell commits the raw result only after those guards; typed target/impl/native liveness are not invented. Null allocation can commit null with no constructor and a later getter can retry. Legacy standalone `createQualified` calls retain their existing isolated declared-stage commit API; explicit184cb admission is required for the new shared-window binding. Neither API is wired to Session/native execution.
- Getter destruction clears only its diagnostic active-frame marker through a weak cell lease, including when its lazy wrapper was abandoned. It neither clears the native field nor runs native cleanup. Session adapter release, Project release and native deletion are not implemented here.

`observeInitialArea3c8` still cannot change known null into a nonnull observation. The only newly shared null-to-result write is the lazy getter's selected source commit. Other CP68 initial fields and replacement helpers retain their existing contracts. Native initializer184fa, mutable later observation/read bridge18477, cleanup18575 and actual Session constructor cursor remain separate dependencies; no general field setter or historical-receipt execution was added.

## Verification prepared for root

Focused targets from the existing CMake globs:

- `m2m_mdt_widget_area_canonical_cell` — new10 Testkit cases: unknown/shared binding, missing-store guard and shared commit/typed binding, typed cached branch, wrong owner/revision/zero occurrence, null allocator retry/replay and invocation rejection, interrupted/nested construction, expired raw/typed state, colliding ownership, revocation/dead parent, and abandoned-wrapper cleanup.
- `m2m_mdt_widget_area_lazy` — existing CP65 compatibility/ordering cases.
- `m2m_mdt_widget_area_owner_state` — existing CP68 typed lifetime/reference/replacement cases.

Static preflight confirms Testkit `run_all` entry, supported CHECK macro, no C assert, quoted includes resolving under the actual app include directory, no trailing whitespace, and CMake `CONFIGURE_DEPENDS` source/test globs with `m2m_core`/Testkit linkage. `git diff --check` returned success, though checkout tracking is not used as proof of the imported files' content; the explicit file checks/hashes are authoritative for this receipt. These checks are not a compile or passing-test claim. New lazy implementation now references the owner-state cell implementation; both are already included in the common `src/*.cpp` core glob.

## Frozen files

All paths are relative to `source/apps/moproj2mst/`.

| File | SHA-256 |
| --- | --- |
| `include/m2m/engine/widget_area_lazy.hpp` | `1b43795c6c63551b3e7b59ec5362badd04b750d27edde0c9cd13fb6a2818e83a` |
| `src/engine/widget_area_lazy.cpp` | `7eb64c089ca9bdc03074c6709498d2239a9003cfa0f19a5100b5d1e2a6423dfa` |
| `include/m2m/engine/widget_area_owner_state.hpp` | `e789b5f2a4a9ecb501ee175ea8fecfa0ba1260f3e3b59decdecfe3a17485c661` |
| `src/engine/widget_area_owner_state.cpp` | `add5debe7a41ef7f4e4c23aed84b42033a60d37e5dcb7449878b58e735eb0c33` |
| `tests/mdt/test_widget_area_canonical_cell.cpp` | `6a7072b3b0f4b2699b00c2438eddf209ab17d895ad20b72777ecf85c802a6727` |

## Evidence and remaining admission

Implementation follows `resume75-boolean-canonical-cell.md` and native source qualifications in `resume75-boolean-constructor-owner.md`. Retained original probe004 establishes a historical outer main-window constructor return before CLI Open, not a direct184fa/18477 read, current area pointer or first Boolean editor Apply. Its selected editor is eb26f0, not Boolean11e2480.

The exact next native state witness remains either the actual selected main-window owner/caller at184fa and post-store18505, or the smaller current lazy-field read at18477. A reached candidate constructor continuation could instead supply the corresponding source occurrence. None is supplied by a qualification string or by possession of a helper token. Actual area constructor/impl/registration/placeholder callbacks, native pointer-to-typed-owner mapping and first-Open timing remain guarded. Root owns all build/integration validation; this owner remains on source hold.
