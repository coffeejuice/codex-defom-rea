# CP62 isolated DBLIST manager lookup prefix freeze

The new helper implements the qualified existing-file **lookup prefix** against the actual Session manager. On a miss it preserves diagnostics and stops before native child construction. No absent-child registration, filesystem observation, warning/default choice, gate/mode admission or public DBLIST/B127 behavior is enabled.

Only three exclusive new source/test files and this report were written for this implementation. Existing ModelManager, StudyIo, Session, model, public reader and CMake files were not edited. Source is frozen for root's grouped integration/build.

## Source contract and owner

New API: isolatedDbListStoredPathValue(DbHandle), bindDbListManagerPrefix(ConversionSession), and DbListManagerPrefix::lookupOrStopBeforeConstruction(session,resolvedExistingPath). Binding requires a live actual Session Project, its already-existing StudyIo, its actual mainwindow+50 ModelManager shared owner, the same manager alias exposed by StudyIo, and that manager's ownerSession. The lease holds weak I/O/manager references and a comparison-only Project identity. Revalidation rejects foreign/released Project, Session and I/O identity/control block changes before lookup diagnostics. StudyIo exposes only a raw modelManager alias; this check verifies that pointer while the actual manager shared control block comes from Session.

The caller must independently qualify DBLIST's param6==0, global gate, path resolution and QFileInfo::exists()==true before this selected branch has production meaning. This isolated helper does not certify those preconditions or call any filesystem API. Tests explicitly supply prepared adapter state, not original existing-file observations.

The stored-path getter is qualified by bounded immutable-original DUMPBIN of140cdd190 through its ret at140cdd274. For pathSet false, it constructs a null QString. For pathSet true it copies child+28; r8b==true jumps atcdd205 to the return-copy branch atcdd245, skipping the basename/conversion helper819090. Consequently the selected manager lookup compares raw stored values without adding canonicalization or case folding. Null and nonnull empty values remain distinct in the returned value; QString equality compares their content.

The native getter capture command was:

```text
dumpbin.exe /disasm:bytes /range:0x140cdd190,0x140cdd275 evidence/original-binaries/DEF_GUI_MO_64.exe
```

Immutable GUI SHA-256: fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc. Exact selected getter bytes SHA-256: cfa1881ffc6a6d5c7c0e7ecff362265172abf21c0a6761d73b3374ac1dbe5c1c. Its PE unwind region is140cdd190..140cdd276 exclusive. No mutable database or original process was used.

Retained c78c10 decompile plus resume62 raw capture establish a full child-list traversal that retains its most recent matching child and continues the outer traversal. The helper therefore returns the last exact match, records visited identities in order and leaves current selection, child paths/IDs/load state, list membership and signal connections unchanged. Attempts are adapter diagnostics, not a native allocation or lifetime claim. Returned child and diagnostic pointers are borrowed and must not be dereferenced after manager ownership changes without appropriate requalification.

## Exact absent-child stop

A miss records stoppedBeforeNativeConstruction then throws M2M_E_UNSUPPORTED:

```text
DBLIST absent child requires native manager virtual+68 construction and path-setting effects
```

This is before c79390's virtual+68 call, path setter, ID assignment, list append and two QObject connections. Earlier completed lookup attempts remain recorded; no later requested path executes after refusal. No partially constructed child or repeated factory attempt is invented.

The root task explicitly allowed a proven-fields fallback when constructor identity remained opaque. That fallback applies here: existing prepareProjectModel is for an empty manager and binds Project.dataDb, effects absent from this DBLIST helper. Reusing it would be wrong. Native derived+68 selects045490->043fe0; full child constructor state is not supplied here. Existing dbSetPath also explicitly omits the object-color table route after path handling, so its success is not proof that native cdcdc0 completed. appendCreatedModel/registerModelSignals are already post-construction seams and cannot justify crossing those earlier dependencies.

## Focused tests and static checks

Six new Testkit cases cover raw path/null/empty values; actual manager identity with ordered duplicate last-match and unchanged current/IDs/connections; no case/path normalization and pathSet gating; prior attempt retention and no creation on a later miss; missing/foreign/replaced/forged-control-block/released owner rejection; and weak lease lifetime. Prepared child fixtures are explicitly labeled, and the test never calls the helper from public B127 parsing.

All 16 quoted includes resolve under the current file/include roots. There are six unique TEST_CASE names, a Testkit main, supported CHECK/REQUIRE macros, no C assert, final newlines and no trailing whitespace. Testkit macro definitions were checked in this lane's preceding work. These are static checks only; no compiler, build, CTest or executable tests ran.

Suggested root target: m2m_doe_dblist_manager_prefix; CTest name: m2m.doe.dblist_manager_prefix. Add the new cpp to core like the other internal DOE prefix helpers and link the test to that core. Root must compile and run it before claiming test validation.

Next dependency: qualified native Mo child-constructor state and complete cdcdc0 path/object-color effects on the same actual manager owner before enabling absent-child registration. Actual input gate/mode/context/existence and warning/default behavior remain separate production requirements. Public B127/DBLIST/Report guards remain unchanged.

Freeze UTC: 2026-10-24T21:38:29.146049+00:00.

| New source/test | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/dblist_manager_prefix.hpp` | `04253227dfa7289abdff8d08ce0b6a4fbda499baa2b28895853609c94e3a3614` |
| `source/apps/moproj2mst/src/doe/dblist_manager_prefix.cpp` | `6c4d3084db66cf4c3b6e954b31a4e3569058f2e04c9d92bdb174f7b95cc4bcf9` |
| `source/apps/moproj2mst/tests/doe/test_dblist_manager_prefix.cpp` | `46e58b9ab6ce8f8280c7de693bf68c52f8624ac3f591c5bef70cf4b0bd405514` |

| Controlling evidence/current interface | SHA-256 |
| --- | --- |
| `analysis/resume62-dblist-owner.md` | `42e9061ced3edb0b81e136cd40db59761e577a2dd9e0216eb3540be03c7742eb` |
| `analysis/resume62-dblist-owner-receipt.json` | `d7c045a4bc841707c5b19157bced0dad46a15827336f245b817777c032af22ec` |
| `analysis/wave2-doe-output-default-task-model-manager-id-decompile.json` | `8bb06574eea454e8d63c6b188efe57ed2dacd06d600dce38b89e8320c59a17a4` |
| `source/apps/moproj2mst/include/m2m/engine/model_manager.hpp` | `7c55be276bc833f1681d81ebccbc87612d9e3f61e7d486c9388e43dda7202e59` |
| `source/apps/moproj2mst/src/engine/model_manager.cpp` | `69f426a43ffead038991178fe7106c3fd3f4b46b93fc4968b48d0dbd2f94972d` |
| `source/apps/moproj2mst/src/engine/db_load.cpp` | `f87d28001efdba3fc3fd286077a4ef83c7cb106d8c8495daa6eacbc9d5b35b75` |
| `source/apps/moproj2mst/src/doe/session_records.cpp` | `8e8410f7fb4596bc62e752b02969208e51c9cfcfab402ba72727885bb3f50794` |
