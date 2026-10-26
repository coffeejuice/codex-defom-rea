# CP273 Boolean timing: next executable leaf

## Decision

Do not repeat the zero-slot `postLoad` investigation. It is a settled, implemented branch: native `11df0cb` checks the OPI slot-record count, calls `addObject(OPI)` at `11df0e9` on zero, and continues at `11df0ee`; candidate `l5_slots.cpp` and `test_boolean_postload_creation.cpp` already model this. It does not establish constructor notifier timing or an Apply-origin add during first Open.

The next bounded action is a **read-only retained-evidence index audit against the exact RBOOL witness schema**: run the existing offline Boolean Open witness checker over the retained Boolean capture directories and retain only its compact per-input coverage result, then compare any admitted records to CP164's two separate cuts (same-E constructor chain, and complete-Open nested Apply/add chain). This is executable now, does not require source/build/GUI/original-process/mutable-REA access, and can establish whether any already-retained record supplies a missing join. Do not run the checker against Forming captures as if they were Boolean evidence, and do not promote a negative coverage result into evidence that the event never occurs. If the audit remains negative, the next useful evidence is a fresh original witness only after supported GUI-control release plus verifiable receipt/status; the present recovery instructions prohibit acquiring another GUI session before that condition.

## Required matching fields

For constructor timing, require one Boolean Object-factory E allocation generation, `11e2480(E, OPI)`, same-E QWidget return `377990`, setup/guard normal returns through `130830`, E+38 resolution, E+28 bind return, and notifier call `130ad1` identified by caller continuation `130ad6`, including notifier identity/suppression state. For first-Open Apply timing, require the complete Open boundary (`271b0..28514` or the qualified CLI startup branch through `1e4e`) containing current-widget UpdateData -> Apply `11e2870` -> E+38/OPI resolution -> normal add return `11e295a` or `11e2bcb`. Label `11df0ee` separately as postLoad. Keep process, thread, owner, invocation, and allocation lifetimes consistent; a partial prefix is not a witness.

## Evidence basis and limits

- `imports/claude-moproj2mst/analysis/resume164-boolean-next.md` defines the two distinct receipts and records the retained checker as a coverage miss, not proof of absence.
- `imports/claude-moproj2mst/analysis/resume208-rbool-next.md` confirms CP164's zero-slot path is separate from constructor notification and first-Open Apply; it rejects synthesizing timing from later fill or postLoad.
- `imports/claude-moproj2mst/analysis/resume266-boolean-timing-next-leaf.md` reports the same distinction and that no source-promotable timing leaf was found without a runtime occurrence.
- `imports/claude-moproj2mst/analysis/resume214-boolean-timing.md` identifies the retained original target as DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; its static disassembly does not establish a runtime occurrence.
- `analysis/recovery-plan.json` keeps full application recovery active. `python tools/recovery_status.py` reports current GUI validation blocked on supported release lifecycle; this does not block the offline evidence audit.

This note is converter-only. No source, test, plan/state, build, GUI, original executable, or mutable REA data was changed or accessed. No reconstructed DEF_PRE_64 instance is discussed.
