# Checkpoint 63: DBLIST missing-path warning contract

## Closed conditional path

The warning trigger is now statically bounded by the corrected
`FUN_1406554a0` evidence in `analysis/resume62-dblist-owner.md`:

1. The retained caller first checks its sixth-argument byte. Nonzero bypasses
   DBLIST handling. Zero permits the separate global gate, which checks
   `*(int32_t*)(0x143d766b8 + 0x1c) == 0`; only then does the corrected
   `DBLIST` child dispatch to `FUN_1406554a0(param_1, node, param_3)`.
2. The handler uses the copied QString `param_3` to construct QFileInfo and
   take its absolute directory. It iterates direct DBLIST child nodes and
   accepts exact `DBNAME` tags. For each, it takes the node text, combines it
   with that directory through QDir `absoluteFilePath`, cleans the result,
   and checks QFileInfo `exists`.
3. A missing resolved path sets the local success byte false and appends the
   original DBNAME text to a local missing-name list. The loop continues.
   Existing paths, including ones after a missing entry, still pass through
   the main-window ModelManager lookup/ensure path before the warning branch.
   The handler does not batch-check all DBNAMEs before making those manager
   calls.
4. After the loop, success true skips the warning. When false, the handler
   reads the separate manager/state DWORD at `0x143d766b8 + 0x0c` through
   `FUN_1402cb7a0`. Value `5` skips only the warning. Any other value enters
   title/body construction and calls the QMessageBox import. Mode `5` does not
   suppress earlier existing-path manager registrations, clear the failure
   byte, or prevent the helper returning false.

The first `+0x1c` gate and the later `+0x0c == 5` warning-suppression test are
different fields and different decisions. The caller's sixth-argument byte is
a third condition; its domain meaning remains unknown. Do not replace any of
these with a candidate “has session” or `SessionNotificationState::suppressed`
check.

## Warning contents and parent

The dialog call at `0x1406558cc` is the imported
`QtGui4_64.dll!QMessageBox::warning(QWidget*, const QString&, const QString&,
StandardButtons, StandardButton)`. Its title argument is the exact QString
literal `Error in reading`. Its message begins with the exact prefix
`The following Database files referred to in the Session file could not be
found:\n\n`, followed by the local list populated from the missing `DBNAME`
texts in DOM iteration order. This closes the logical message input; it does
not establish a resolved-path list. The handler's 0x400-byte scratch buffer
and formatting helpers contribute to the final string construction; their
capacity/truncation and separator details are not asserted here.

The QWidget argument source is also bounded: the handler calls helper
`0x140657870`, which ignores its incoming value and calls
`0x1402c8090(0x143d766b8)`; that leaf returns the qword at global offset `+0`.
The value is then passed as `RCX` to `QMessageBox::warning`. The Qt import
signature qualifies it as the parent-widget argument; the recovered manager
contract identifies the global `+0` owner as the main-window pointer. This
does not independently qualify a runtime QObject/QWidget instance, its
lifetime, or whether it is non-null on every route.

The warning's return value is not used to choose the handler result or undo
the earlier manager path. The same false success byte is returned after the
dialog path. If the mode value is `5`, the dialog is skipped and the same
false byte is still returned. `FUN_140650e60` then advances to later sibling
elements; a later ReportList child is a separate handler (`FUN_140655df0`)
and is not folded into DBLIST's warning semantics.

## Exact string-byte references

Original PE: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

| Value | VA / raw file offset | Size including NUL | SHA-256 |
|---|---|---:|---|
| Title `Error in reading` | `0x1424e3f38` / `0x24e2738` | 17 | `4bcc645adfac273583213a153e3526c1e0c58a1b6788c250a0eae048b06bc602` |
| Message prefix | `0x1424e3ee0` / `0x24e26e0` | 83 | `fc3188ef54fc8c010cec117d559432d296b501a953b0a445e7b55a459393673c` |

The warning import slot is `0x141e0bf40`, in the QtGui IAT whose base is
`0x141e059f0`, zero-based entry 3,242. The exact decorated name and raw
prefix bytes remain recorded in `analysis/resume61-dblist-first-effect.md`.
The bounded handler, main-window getter, warning-mode leaf, and helper chain
are already retained in `resume60-dblist-reportlist-6554a0.asm` and
`resume62-dblist-owner-leaves.asm`; no duplicate raw capture was needed for
this synthesis.

## Limits and implementation consequence

This closes a source-backed **static branch contract**, not an original runtime
observation. It does not prove the gates' runtime values, B127's resolved
relative paths, which DBNAMEs exist in an original run, whether the main window
is initialized, or whether the dialog was displayed. Existing-path child
creation also has a separate constructor-completion dependency; this report
does not claim that full manager path is ready for public integration. The
unconditional candidate DBLIST refusal must not be removed based only on this
static trace.

A future isolated pure formatter could accept the already-determined ordered
missing-name list and return the fixed title/prefix plus names, but this note
does not define buffer-size behavior or modal UI dispatch. Production wiring
still requires the original outer caller's real sixth argument and global
state owner, plus candidate path-existence and manager-constructor contracts.
No UI call should be added to the headless conversion path from this evidence
alone.

Read-only static synthesis of the corrected CP62 captures. No source, build,
test, GUI, original runtime, fixture, or mutable REA database was changed or
executed.
