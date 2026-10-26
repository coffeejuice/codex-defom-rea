# Checkpoint 61: DBLIST warning-call destination and tag assignment

## Result

The indirect call at `0x1406558cc` through IAT slot `0x141e0bf40` resolves to
the imported Qt symbol

```text
QtGui4_64.dll!QMessageBox::warning(
  QWidget*, const QString&, const QString&,
  QMessageBox::StandardButtons, QMessageBox::StandardButton)
```

This resolves the visible call destination, not a DBLIST destination field.
The caller builds a warning title and accumulated text, then invokes a modal
warning API. Nothing in this bounded trace establishes that the DBNAME is
stored into a field of the part model. The indirect call is not a file writer
or database-open import.

## Static evidence and exact data

All offsets below are from the retained original
`evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`.
The DBLIST handler capture is
`analysis/resume60-dblist-reportlist-6554a0.asm`, whose SHA-256 is
`D02BD67B3B8E1864DF5B03CB87DB6C9068418FA7C50D5456DEE35701F09F0DA1`.
Despite its historical filename, corrected caller evidence maps this function
to the **DBLIST** branch (see the assignment section below).

The executable's PE import directory places `0x141e0bf40` in the
`QtGui4_64.dll` IAT. Its IAT base is `0x141e059f0`; the slot is entry 3,242
(zero-based). The exact decorated import name is:

```text
?warning@QMessageBox@@SA?AW4StandardButton@1@PEAVQWidget@@AEBVQString@@1V?$QFlags@W4StandardButton@QMessageBox@@@@W421@@Z
```

The call setup at `0x1406558a1–0x1406558cc` supplies a QWidget pointer in
`RCX`, a QString local in `RDX`, another QString local in `R8`, a button-flags
value in `R9`, and zero as the fifth argument at `[rsp+0x20]`. The title local
is constructed from the literal at VA `0x1424e3f38` (file offset
`0x24e2738`):

```text
Error in reading\0
```

The raw bytes are
`45 72 72 6f 72 20 69 6e 20 72 65 61 64 69 6e 67 00`, SHA-256
`4bcc645adfac273583213a153e3526c1e0c58a1b6788c250a0eae048b06bc602`.

The retained message prefix at VA `0x1424e3ee0` (file offset `0x24e26e0`) is:

```text
The following Database files referred to in the Session file could not be found:\n\n\0
```

Its raw 83 bytes are
`54 68 65 20 66 6f 6c 6c 6f 77 69 6e 67 20 44 61 74 61 62 61 73 65 20 66 69 6c 65 73 20 72 65 66 65 72 72 65 64 20 74 6f 20 69 6e 20 74 68 65 20 53 65 73 73 69 6f 6e 20 66 69 6c 65 20 63 6f 75 6c 64 20 6e 6f 74 20 62 65 20 66 6f 75 6e 64 3a 0a 0a 00`, SHA-256
`fc3188ef54fc8c010cec117d559432d296b501a953b0a445e7b55a459393673c`.

The handler constructs that prefix as a QString at `0x1406555872–0x1406555884`.
Before the warning call it also passes a local 0x400-byte buffer and other
QString/collection values through helper calls (`0x140004a60`,
`0x1400048f0`, and adjacent helpers). The import call's `R8` argument is the
constructed message local; its exact concatenation, list contents, and
separator/encoding transformation are not independently decoded here. The
button-flags value comes from the helper result in `RBX`; its symbolic
StandardButtons meaning is not resolved. The fifth argument is visibly zero.

The warning branch is conditional. The surrounding handler checks a local
boolean and the value returned by `FUN_1402cb7a0(0x143d766b8)` before entering
the list/warning path. The latter path appears only when the returned integer
is not 5. The meaning of these states and whether the retained DOE lifecycle
takes this path are not established by static evidence alone. Thus the title
and message prefix qualify the branch's UI text; they do not prove a warning
was displayed for the retained B127 or B120 project.

## Correct tag-to-handler assignment

Use the corrected dispatch mapping in
`analysis/resume60-dblist-slot-next.md` and its retained caller/literal
evidence, not the earlier reversed labels:

| Direct `SessionRoot` child | Literal VA | Original handler | What is covered here |
|---|---:|---:|---|
| `DBLIST` | `0x1424e3cb0` | `FUN_1406554a0(param_1, node, param_3)` | Bounded handler and conditional missing-database warning path |
| `ReportList` | `0x1424e3cb8` | `FUN_140655df0(param_1, node)` | Not this capture; its behavior remains separate/unresolved here |

`FUN_1406554a0`'s receiver is the caller's part-level object and its second
argument is the direct DBLIST DOM node. The title and missing-database prefix
fit a DBLIST/database-reference path. They do not change the corrected dispatch
assignment and do not justify assigning `FUN_1406554a0` to `ReportList` merely
because the capture filename says “reportlist.”

The retained B127 `.DS` has a direct DBLIST with one DBNAME text value
`../../../COG_P1_B1.DB`, followed by an empty ReportList. That corpus shape is
bound in `analysis/resume53-doe-dblist-seam.md`; it does not establish whether
the DBLIST handler's global gate is open, whether the referenced database
resolves under the original runtime's working-directory rules, or whether the
warning branch runs. The candidate's observed unconditional code-7 guard is
not original behavior evidence.

## Evidence boundary

Established statically: original PE identity; IAT slot and exact QMessageBox
import; warning-call argument placement; title/message-prefix bytes; corrected
DBLIST versus ReportList handler assignment; and a conditional path to the
warning call. Not established: original runtime execution, whether the dialog
appears for the retained fixture, the full warning body after string assembly,
button semantics, DBNAME resolution rules, persistent DBLIST ownership/storage,
or the ReportList handler's behavior. No helper-name inference is used for the
unknown buffer/list transformations. The historical 16-case native connector
probe and candidate parser tests are unrelated to this `.DS` dispatch.

Read-only static evidence only. No source, build, GUI, runtime, project fixture,
or mutable REA database was changed or executed.
