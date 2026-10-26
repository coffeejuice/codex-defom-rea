# CP112 operation coverage next step

## Decision

No newly inspected operation/property branch is ready for safe admission from the retained evidence in this lane. The highest-value next source action is a bounded original-image field-xref trace for the guarded connector/DBLIST `+0x1c` state, not another candidate-only behavior test.

## Why this is the best next target

The current queue identifies RCONNECT's concrete gap as “DBLIST +0x1c writer/caller/reset unbound; public admission guarded” (`analysis/takeover-state.json`, RCONNECT). Existing DBLIST evidence is useful but does not close that contract:

- `analysis/resume60-dblist-slot-next.md` ties the retained caller `FUN_140650e60` to the DBLIST branch at `0x1424e3cb0`, calling `FUN_1406554a0`. The bounded handler trace reaches collection iteration and a modal warning, but does not identify the DBLIST destination field or persistence effect.
- `analysis/resume61-dblist-first-effect.md` resolves the indirect destination to `QMessageBox::warning`; this confirms the first visible effect, not a state write.
- `analysis/resume62-dblist-manager-implementation.md` recovers a private manager-prefix helper, while explicitly retaining public B127/DBLIST/Report guards because the actual child-constructor and subsequent state effects are not qualified.

The needed next artifact is a source-linked xref chain for the `+0x1c` field from object construction/reset through the exact DBLIST handler/writer and its caller. It should identify the owner object, successful and missing-item branches, and the first externally visible effect. Until then, a test of the private prefix helper cannot justify public connector admission, and there is no safe helper-only production change to make.

## Other refused branches considered

- ROPS still lists Report document ownership, Save viewer effects, and accepted Close as unqualified. This is broader lifecycle state and is not smaller than the DBLIST field dependency.
- RCOG records a multiple-reheat writer path beyond the recovered one-reheat array contract. The source itself marks `nReheats >= 2` as writing past the array, so removing that guard would model undefined behavior without a closed native outcome.
- The RFORMAT SPRING positive-count ownership gap is unchanged and intentionally not revisited here.

## Recommended bounded action

Using the retained original image SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, perform a read-only static xref search for reads/writes of the candidate `+0x1c` slot and capture only the constructor/reset, writer, and their direct callers needed to connect them. Bind every capture to image address and SHA. If those references do not resolve a unique field owner and writer branch, retain the current public guard and report the first unresolved reference. This lane made no source, test, build, GUI, or mutable REA changes.
