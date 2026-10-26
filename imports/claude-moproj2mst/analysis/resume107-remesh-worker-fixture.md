# CP107: enabled-remesh worker fixture qualification

No retained historical enabled-remesh fixture qualifies. A read-only inventory of all 31,017 regular files under `evidence/` found zero case-insensitive exact filenames `LOCAL_REMESH.DAT` or `LOC_DEN_CTRL.DAT`, and zero filenames containing either stem. Consequently this checkpoint cannot identify a retained project whose original Open loaded enabled remesh. This is the precise historical-occurrence blocker, rather than a parser or writer implementation blocker.

The smallest candidate worker fixture is a **constructed overlay** on the supported retained single-project `evidence/gui_mo/oracle_b/133_B_DBSTEP_BOOL3_before`. It is suitable for testing candidate source order and the private opted-in terminal protocol; it is not a retained enabled-remesh oracle or evidence of fresh original termination.

## Constructed input and provenance

Copy that entire 64-file before directory into a uniquely owned short leaf, for example `C:/PROBLEM/r107_<decimal-pid>_<16hex-token>`, preserving relative names. Require every actual absolute path to have fewer than 100 characters before creating files. Even a ten-digit pid plus sixteen-digit token yields an 87-character longest existing relative path (43 characters). Never reuse a preexisting leaf or fall back to an unrelated location. Keep diagnostic/protocol receipts in unique checkout scratch; delete only the verified owned project leaf after handles and child have been retired.

The input XML is `COG_P1_B1.moproj` (25,644 bytes, SHA-256 `8e5e896cce67164430324cddc25324043449428b789d201bb3bef9c49cc89f79`), with exactly one retained `<Sub_Project>` (line479), SI units, and DB `COG_P1_B1.DB` (9,216,000 bytes, SHA-256 `707326e90e4f8fb0fee2aa1fa9f174e8d6e40b213fe2a7c0a828be00829671df`). The source before manifest has 64 entries. Canonical manifest SHA-256 is `0ff2038095f238744a0d61a1cbf2b3da01f061ba4080f3a0dff08aa248e0c5e6`: sort relative slash paths using case-sensitive lexical string order, then concatenate each file's lowercase SHA-256, one space, decimal byte size, one space, relative slash path, LF, encoded UTF-8 without BOM. This is an inventory fingerprint, not a byte mask.

Add exactly these binary bytes before conversion:

| Relative path | Bytes | Size | SHA-256 |
| --- | --- | ---: | --- |
| `LOCAL_REMESH.DAT` | `DONE\n` | 5 | `8221ac66be71558c921fb44cfb66f7997699aea754d917763882d6d9eddc836e` |
| `LOC_DEN_CTRL.DAT` | `FILTER_DISTANCE 7\nDONE\n` | 23 | `bb28f25ddbd98300249e3a867f78ebc5714d16640996a6059ef4441e6bd81d50` |

These are a valid unmarked primary sentinel and valid fallback, with no numeric ambiguity, unsupported bytes, missing terminator, or unassigned integer token. `readRemeshImpl` (`simcontrol300_remesh.cpp:239`) opens the primary, scans `DONE`, rewinds it, closes it because there is no `KEYWORD_DRIVEN`, and reads the fallback. `loadRemeshAtProjectOpen:320` copies completed `loaded=true` into the same Session remesh owner's `enabled` field; `filterDistance` becomes7. The actual Open calls `ConversionSession::loadProjectRemesh` through `moproj/read.cpp:59`; it is not an unattached helper seed. Existing `remesh_native_denied_primary_falls_back_and_unmarked_primary_closes_before_fallback` and `session_actual_open_loads_one_remesh_owner_that_survives_project_storage_release` cover the reader/fallback and actual Session-owner halves. They do not themselves establish this new combined worker run.

## Earliest lock and lifetime

The lock can be acquired **after overlay creation and before the entire conversion/Open**, with no new Open/Save callback needed:

`CreateFileW(primary, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr)`.

Require a valid handle and retain it through terminal observation. `NativeReadFile` (`simcontrol300_remesh.cpp:73`) requests `GENERIC_READ`, sharing `FILE_SHARE_READ | FILE_SHARE_WRITE`; these accesses are mutually compatible with the held read-only handle. Both primary scan and rewind therefore remain available. The fallback is a different file and needs no lock. At Save, `CoggingFile` (`datafiles/cogging_file.cpp:18`) requests `GENERIC_WRITE`, share read|write, `CREATE_ALWAYS`. The existing handle does not share write, so the writer is denied before truncation. This is a deterministic sharing-denial fixture, not a general claim that arbitrary failed fopen preserves contents. CP106's focused terminal-contract test uses this exact read-sharing denial for the writer boundary.

Prefer the supervisor owning the noninheritable lock (`lpSecurityAttributes=nullptr`) before launching the opted-in child; the child need not inherit a raw handle. It remains effective across processes and can be released by the supervisor after the child's terminal/deadline receipt. Do not release it at an Open completion callback. Record lock-acquired, full path, creation disposition/access/share flags, child identity, and release receipt. No lock was acquired or process launched in this investigation.

## Before/after manifest expectations

Capture the full 66-file overlay input manifest before launch, and every post-worker regular file plus missing/error markers afterward. Preserve full raw before/after bytes for any changed or unexpectedly missing file; retain expected and available actual bytes before assertions. Do not silently normalize the rewritten project XML, paths, whitespace, numbers, or error details.

The primary and fallback hashes above must remain unchanged at failed remesh open. The preceding retained raw Save outputs have the following comparison targets in `evidence/gui_mo/oracle_b/133_B_DBSTEP_BOOL3` (no `_after` suffix):

| Relative path | Before | Expected earlier Save output size / SHA-256 |
| --- | --- | --- |
| `COG_P1_B1.MST` | 7247 / `07b0a85d88c31326ecd1f069d765fdc14e8f5efdc046195ed301eb014e20eb1c` | 7196 / `9bad363c4454ed9339129c6845ee82ebc044565e9165bcdb846f4fc35ed75ef1` |
| `Objects/Object00002.KEY` | absent | 2411 / `7adeee1681d831de2c49c972735ee9d76d102fe13d2b5a0917bafe33402f94e1` |
| `Operations/Task00002/Boolean.BWZ` | absent | 182 / `0d696c7c8b4ed38bd4b0a98dc2e24f5bf1bd68059a902c1ccc3387c99eb00ca8` |
| `Operations/Task00002/ObjProperties00004.KEY` | absent | 169 / `2dc78ce7840dc99712a35dd66e6ee132324960dd97c15902276fcf6fc6fa0f4a` |
| `Operations/Task00002/SimCtrl.KEY` | 2143 / `005dc77af3190c7590da6e6d7de15d78bad59dc2cc43540b5f4883ece7485c42` | 2111 / `6f0dc482e89e19de938530257297c098fc239abfaacb4ba1344f1d530897fe15` |

The five raw comparisons are retained historical Boolean Save evidence, qualified in CP105; remesh is a constructed extra input. Remesh state is consumed after these writers in the candidate conversion path, so these are source-ordered expected prefix effects, still requiring the new worker observation. Project XML is also attempted before remesh; its path-dependent raw rewrite must be retained as a separate observed artifact, without pretending the historical fixture supplies an exact XML after oracle. Do not restrict the full post manifest to these five files or assume all other files are unchanged without recording them.

## SaveCallState and terminal receipt

With strict WRITE_SAVE, otherwise successful project/MST writers, the single-subproject normal Save reaches failed remesh open only after the following snapshot:

```text
answer = DialogAnswer::No
opened = true
checkedSubprojects = 1
validationComplete = true
lastOperationModalPending = false
pendingDefault = DialogAnswer::NoButton
projectWriteAttempted = true
projectWriteReturned = true
projectWriteSucceeded = true
mstWriteAttempted = true
mstWriteReturned = true
mstWriteSucceeded = true
```

`saveCurrent` resets the state and sets validation fields (`engine/save_project.cpp:44`); native project output commits its fields at `core/convert.cpp:346`, and MST output commits its fields at410..427. The remesh call is430..441, before adapter false-status reporting. `RemeshTerminalForwarder` copies this exact live value at leaf dispatch and reuses that copy for failClosed; no Session pointer or service escapes into a public result. A mismatch in any prefix writer status is a different case and must not be relabeled as this expected all-success prefix.

Only an explicitly installed private `FreshDefault` worker service admits the terminal request: site `FirstNullFilePrintf`, source `0x1400abcf0`, proof `NativeFreshDefaultStartup`, format attempt ordinal1, modeled errno22, expected process status `0xC0000417`. Require the actual terminal-publication receipt plus owned-child status, not exit status alone. Unknown/custom/null services preserve the unsupported guard. Returned dispatch, failed publication, or parked/deadline termination is harness failure, never source-terminal success. Retain `candidate-fixed-endpoint; native-default-handler-prelude-unimplemented` in any eventual terminal acceptance claim. The candidate endpoint is not a fresh execution of the original handler prelude.

No normal Save return, native Close, successful Close cleanup, or destructor unwind follows an admitted successful terminal endpoint. Parent/supervisor cleanup of this owned scratch is separate harness work. A normal successful conversion of the overlay with no lock is a useful control but is not this terminal case.

## Frozen evidence and scope

Source hashes inspected:

| File | SHA-256 |
| --- | --- |
| `src/engine/simcontrol300_remesh.cpp` | `a865bf7aead8ce3b74f7d670340125538cf7903381b9335d851f55dd2bf4c228` |
| `src/datafiles/cogging_file.cpp` | `7017a6d45635dde013e69bf9424ddb5f4df266012ad749f5575bea08982152b0` |
| `src/core/convert.cpp` | `71fe67b9c6be7bde83001e663aa7c50f01c4a6421f357c0db893a5df0a0bd632` |
| `src/core/remesh_terminal_call_service.hpp` | `0f3f3cdd1ed46ab92521a3d15273983fa3d36b8439dbb43d432afdda7d580e81` |
| `src/engine/save_project.cpp` | `90a0ddbf6b4c92b4f4fe8bc106aa2e9248f31c84d2a0981d0ee10eda0baf2337` |
| `src/engine/conversion_session.cpp` | `f1ff5d1c3934b1daa769f58c233b397a48c6eacb7cfdf790dba36edfea820604` |
| `tests/mdt/test_conversion_session.cpp` | `fc62ac9656329f8190a1f6fb601f550b0dd2b31fe688db19e0e7547b11677e18` |
| `tests/s/test_session_dbstep_boolean_raw.cpp` | `5778b1d79175fb2eae29d2d4d043d56a7881d0241206f7cd3a4cfdcbe34873c1` |

Paths in this table are under `source/apps/moproj2mst`. Only this unique report was written. No source, fixture, CMake, shared report, REA database, GUI, original executable, build, test process, terminal worker, or C:/PROBLEM path was modified/executed. The inventory and hashes are static observations; the combined overlay worker acceptance is a next test task requiring explicit source ownership and root validation. Historical enabled-remesh acceptance remains blocked on a retained same-project input occurrence.
