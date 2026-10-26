# CP98: retained SPRING item and Save evidence

## Decision

The retained source closes the outer list and Save serialization order, but not the item/selection translation. No narrow public positive-count increment is currently source-closed. Keep the candidate's populated-spring guard until the item constructor and current-item callback are recovered; a reader-only count-one test would not establish public Extra3D Open→Save behavior.

## What the retained Open caller establishes

In analysis/wave2-save-diffs-300-support-140ec8110.c around the FUN_1410dab40 call:

- It clears the spring QListWidget before reading.
- Only when FUN_1410dab40 returns true does it loop over records.
- For each record it allocates a 0x78 object, copies exactly 0x40 bytes from the record array, then passes the object, list widget, copied record and a virtual-call result to FUN_140eb2470.
- After the insertion loop it derives the spring checkbox from QListWidget count, resets a stored index to -1, selects row zero if the list is nonempty, and calls FUN_140ecdee0 on that current item.

This confirms the record reader output is only an intermediate; positive Open constructs native list items and invokes selection behavior.

## What Save is source-qualified to do

The retained analysis/wave2-save-diffs-300-support-140ec9040.c shows:

- If the spring checkbox is off or the item count is less than one, Save removes both spring files.
- Otherwise it obtains the current list item and calls FUN_140ecdee0 before serialization.
- It allocates a count * 0x40 record buffer. For each item, it copies 0x40 bytes from item offset +0x30 into that buffer, then calls FUN_1410db030.
- The retained writer analysis/wave2-save-diffs-300-support-1410db030.c serializes the count and each record's integer/double fields using QString number formatting. It scans per-record flags; with no nonzero flag it removes SPRING2.INI, and with a nonzero flag it writes the companion data. The literal map analysis/wave2-save-diffs-300-extra3d-keywords.json identifies the spring filenames.

Thus the Save file-writing tail and its file-choice branch are known. The missing behavior lies in how list item creation and selection/edit controls represent those 0x40 bytes.

## Remaining blocker and smallest next capture

The reviewed retained captures contain the call sites but no body-level decompile/disassembly for FUN_140eb2470 or FUN_140ecdee0. The slot map analysis/wave2-save-diffs-300-slot-map.json provides only dispatch mapping, not the state transformation. The exact unresolved contract is:

1. Which record fields become list-item text/data and what native record bytes are stored at item offset +0x30.
2. Which current-item changes FUN_140ecdee0 makes to line edits, checkbox state, selected index, and/or the item payload.
3. Whether calling it immediately after Open's setCurrentRow(0) has the same behavior as calling it on Save's current item.

The smallest useful evidence request is bounded original-PE body capture/decompile for those two addresses, including direct callees and constants/string references. No broad application capture is necessary to settle the static mapping. After that, a one-record static fixture can be designed; an original-process comparison is still needed before claiming runtime Open→Save equivalence.

## Candidate boundary

Candidate fillExtra3D in source/apps/moproj2mst/src/engine/simcontrol300_io.cpp returns no page for a successful nonempty readSprings result. Candidate readSprings in source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp keeps positive counts unsupported. These boundaries remain correct with the current evidence. CP94's exact nonpositive-count case is separate and unchanged.

A reader-only count-one test could check the seven scalar fields and missing-SPRING2.INI zero flag, but it would not exercise the item or Save contracts above and should not be described as a public compatibility increment.

## Evidence boundary

Reviewed decompiles: wave2-save-diffs-300-support-140ec8110.c, wave2-save-diffs-300-support-140ec9040.c, wave2-save-diffs-300-support-1410dab40.c, wave2-save-diffs-300-support-1410da800.c, wave2-save-diffs-300-support-1410da8f0.c, and wave2-save-diffs-300-support-1410db030.c. Original executable SHA-256: fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc. These are static retained artifacts; no original process was run.

No source edits, builds, tests, GUI actions, or mutable REA operations occurred.

