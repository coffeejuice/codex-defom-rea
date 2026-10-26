# CP62 DBLIST owner, mode guard and persistent effects

DBLIST `1406554a0` does not store a DBNAME list on the part model after displaying a warning. Its existing-path branch reaches the actual main-window ModelManager and ensures a child model exists **during the DBNAME loop, before the warning**. The visible tail after the warning only destroys locals/owned arguments and returns the accumulated success flag. Earlier successful child registrations survive a later missing path; no rollback appears here.

This reconciles resume61-dblist-first-effect.md: its QMessageBox import/title/prefix identification remains correct, but it did not close the earlier manager call. The first persistent-effect owner is now qualified statically. This does not establish B127 runtime admission, path existence, native constructor completion or public integration.

## Evidence and scope

All new captures read the retained immutable evidence/original-binaries/DEF_GUI_MO_64.exe, SHA-256 fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc. DUMPBIN version 14.44.35228.0 is printed in the receipts. Only this report and unique resume62-dblist raw capture/receipt files were written. No source, build, executable tests, GUI, original process, target call or mutable analysis database was used.

analysis/resume62-dblist-owner-receipt.json retains exact capture commands, unwind-region metadata, selected PE import names/thunk bytes and the DBNAME literal bytes. A PE unwind region bounds a selected capture, not a claim that every logical callee is closed. Small leaf routines without unwind records were captured through their visible ret, with alignment padding retained.

## DBLIST admission and param6

Correct tag mapping remains DBLIST `1424e3cb0` ->1406554a0(part,node,copiedQString), and ReportList `1424e3cb8` ->140655df0(part,node). At651eaa, the caller reads its sixth parameter byte from[rsp+668]; nonzero jumps to651f61 without calling the DBLIST helper. Zero reaches the global gate1402cbec0(global143d766b8), which requires dword[global+1c]==0. Only then does651f55 call6554a0. ReportList is separately gated and does not have that extra param6 test in the retained branch.

The prologue saves two registers and subtracts628h, giving a638h entry-to-current stack displacement. The sixth Windows-x64 argument at entry+30h therefore maps to current+668h. The caller also copies its third QString argument from current+650h (entry home+18h) before passing it in R8. This is actual argument provenance, not a guessed Session field. Current source must retain the mode/gate distinction; SessionNotificationState.suppressed is not evidence for either field.

The handler constructs QFileInfo from that copied QString at6554f5, then obtains absoluteDir at655506. It processes firstChild/nextSibling DOM nodes, converts each toElement and matches exact DBNAME through QString equality. The selected import table and literal bytes close these identities. Non-elements/nonmatching tags skip the file branch; this is not a general arbitrary-child value store.

For a DBNAME, it obtains text, calls QDir::absoluteFilePath against that directory, then QDir::cleanPath, and builds QFileInfo on the result. QFileInfo::exists at655682 decides the two branches. This closes the original call sequence, not host filesystem equivalence, original working-directory state, symlink/case behavior, or the copied QString's semantic identity at all outer callers. In particular, no file open/read is implied by exists.

## Actual persistent owner and first state-bearing branch

For an existing resolved path, the handler calls:

| Site | Original effect/owner chain |
| --- | --- |
|655697 ->657870 | Ignores saved part receiver; calls2c8090(global143d766b8) |
|2c8090 | Returns qword[global+0], the main-window owner |
|65569f ->c9b7c0 | Returns qword[mainwindow+50], the ModelManager |
|6556ac ->c78920 ->c78e80 | Passes the resolved path to the manager ensure/lookup path |
|c78e98 ->c78c10 | Looks through manager+18 child list and compares each child's cdd190(...,true) string with the requested path |
|c78eb4 ->c79390 | Only when lookup returns null: create/register a child |

The manager is not the incoming part model, SessionRecord, TaskInfo, current SUB or flow receiver registry. Part-level dispatch does not imply part-level storage. Existing manager contract wave2-doe-default-task-manager-contract.json and its retained register/id decompiles corroborate the new raw captures.

On the absent-child branch, c79390 calls the manager's virtual+68 child constructor atc793b9, passes the path to child settercdcdc0(...,false) atc793ce, obtains first unused positive ID via c79470, stores it throughcdd740, appends the same child to manager+18 atc793fd, then makes two QObject connections with raw type3 to manager+10's owner. Current selection is not changed in this routine. The actual derived Mo factory and native child constructor have their own prior qualification/guards; a diagnostic DbHandle allocation is not automatically their complete native execution.

Thus the first new heap state can arise inside virtual+68; the first explicit ordered-manager membership mutation is the append after constructor/path/ID return. A path already found by lookup returns its child without this create/append branch. The lookup's local iterator transitions are not a new persistent DBLIST destination field. Its exact child-path getter/iterator behavior must be preserved if implemented; no new canonicalization, first-match simplification or safe null behavior is introduced by this audit.

## Warning route and what follows it

The local success byte starts true at6554c3. A missing resolved path sets it false at6556b3 and appends the DBNAME text to a local missing list. The loop continues. Successful paths before or after missing entries still execute the manager route. There is no batch prevalidation before those calls.

After the loop, a true success byte skips warning. Otherwise655751 calls2cb7a0(global143d766b8), now closed by its leaf body to reading dword[global+0c]. Value5 skips warning; any other value enters the missing-list title/message construction and the QMessageBox::warning import at6558cc. This mode check occurs after all DBNAME work, so it suppresses the warning only; it does not skip earlier existing-path manager calls or restore success to true.

The warning's StandardButton result is not used to choose persistence or success in this routine. From6558d2 through65596f the routine cleans QString/list/node and copied-string arguments, copies the same success byte to its return and returns at65597e. No new persistent manager/part/Task mutation is visible after the warning. As usual, destructors and a modal dialog may have their own indirect/runtime effects; static local-tail closure is not a whole-program side-effect-free claim.

At caller651f5a the returned byte replaces a local result;651f61 joins the sibling-advance path at652027. This is assignment, not an AND accumulator: a later handler can overwrite that same return byte. That path advances the DOM sibling iterator and loops. Later ReportList/Session siblings therefore remain distinct possible effects; the DBLIST result does not throw or terminate child traversal here. End-of-loop caller cleanup returns the then-current byte. No invented DBLIST storage commit occurs at that point.

## Candidate owner and bounded next dependency

Current ConversionSession already owns modelManagerOwner(). convert.cpp binds that same shared owner into StudyIo for an actual session. ModelManager explicitly models mainwindow+50, owns its child list and keeps current selection separately. Reuse this owner; do not create another manager from SessionPart or treat currentModel as the whole DBLIST population. The standalone candidate active-regression Project/StudyIo setup does not by itself prove this actual Session owner chain.

The current prepareProjectModel API is for a fresh empty manager and unbound Project. It cannot be reused as a DBLIST add-another-model operation: it would reject or rebind ownership, and its publication of Project.dataDb is not an effect shown in6554a0. ModelManager's existing appendCreatedModel/registerModelSignals seams represent only the post-construction prefix, not the complete c78920/c78e80 lookup/create path.

The next bounded evidence action is to close the actual cdd190(...,true) path getter and the derived manager virtual+68 construction entry for this same owner, retaining existing child-path semantics and partial-constructor failure order. Then root can assign an isolated actual-manager lookup/ensure prefix with explicit child-constructor dependencies. Before production DBLIST binding, separately supply the real caller param6, the global+1c gate, copied QString context, file-existence observation and warning/default-route contract. Nothing here proves those values for B127 or authorizes its public guard removal.

## Receipt

UTC: 2026-10-24T21:34:32.829519+00:00. Static evidence only; no executable validation.

| Evidence/source | SHA-256 |
| --- | --- |
| `analysis/resume61-dblist-first-effect.md` | `592958b44af829135fc08ede9e4959b2815a1d10bce5ac51eafaec3b66ce2089` |
| `analysis/resume60-dblist-reportlist-6554a0.asm` | `d02bd67b3b8e1864df5b03cb87db6c9068418fa7c50d5456dee35701f09f0da1` |
| `analysis/resume62-dblist-owner-leaves.asm` | `86dc35ca471d9e6fc541d05b48c2ce3f2206879a739bf8eb0f7ed5c5beb0a2ac` |
| `analysis/resume62-dblist-manager-path.asm` | `df8c12a7ee45f0eeccf30cdf63c863080e89259a23da1503d9870653cffe91a0` |
| `analysis/resume62-dblist-register-and-caller.asm` | `438bbe2b9ff00ab35f7ad8554e762044fdfa589182d61143157fb02f8ab39af1` |
| `analysis/resume62-dblist-mode-tail.asm` | `806a7c67f8ddb519d07c2ecc53875c8ef7ef418e110b1e6b9f9aa700be381e0d` |
| `analysis/resume62-dblist-owner-receipt.json` | `d7c045a4bc841707c5b19157bced0dad46a15827336f245b817777c032af22ec` |
| `analysis/wave2-doe-default-task-manager-contract.json` | `c999f44709bf823e29f7288625bfdc2c49f6d428ec230e7584dea16ae89bf4de` |
| `analysis/wave2-doe-output-default-task-model-manager-register-decompile.json` | `61078de53c378d892520b2e838f7e8f3fc32f7de56ebca36607b8756715e3fc2` |
| `analysis/wave2-doe-output-default-task-model-manager-id-decompile.json` | `8bb06574eea454e8d63c6b188efe57ed2dacd06d600dce38b89e8320c59a17a4` |
| `source/apps/moproj2mst/include/m2m/engine/model_manager.hpp` | `7c55be276bc833f1681d81ebccbc87612d9e3f61e7d486c9388e43dda7202e59` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `fb54fd5f79dbbe6660c0cc0284015bff8d33a8ae9495bf88de8b985b7f55a8fd` |
| `source/apps/moproj2mst/src/core/convert.cpp` | `ea57d098969ab37d498314662dd8cd5829f200e73c97b3ceab191beaa05c21d5` |
