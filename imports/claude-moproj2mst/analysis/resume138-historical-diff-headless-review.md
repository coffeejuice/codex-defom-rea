# CP138 historical-DIFF headless comparison review

## Decision

No new comparison was run. The only retained pair with a well-qualified candidate-only full-output harness that isolates a specific historical DIFF is B98, and the CP136 full comparison already establishes that every byte of the MST except the one project-root path occurrence matches the retained output. Repeating it with CP137 would not add a new behavior check: CP137 changed editor frequency handling, while the CP137 retained-validation result still records the same five historical DIFF rows.

The raw B98 mismatch cannot be removed by another disposable-root run. The original retained path is `C:/Users/alext/Projects/deform-rea-work/gui_mo/oracle_b/98_B_DS3F/Gear_blank.KEY`; the comparator's candidate project is required to remain under the approved short disposable `C:\PROBLEM` root, so candidate output necessarily has a different absolute project path. A raw same-root witness requires a paired original and candidate Save at the same project path. No such fresh original run is authorized/available under the current GUI-control release gate. The prior exact one-occurrence path qualification remains a diagnostic view; the raw B98 row remains DIFF.

## Other retained DIFFs

- **A:RS_55_Multiple_Operations_Example1:** the exact Task00002 SimCtrl difference and candidate/original byte values are known. `resume48-a55-cg-timing.md` identifies a conditional native CG-toggle cause, but CP137 still does not demonstrate the fresh constructor/setup/iteration-fill delivery. The current real candidate editor path stops before that fill. A further CLI comparison cannot establish whether the original callback ran, and a new original observation needs the GUI/process evidence blocked by the release gate. Do not treat an output mismatch or legacy isolated normalizer test as resolution.
- **B:120/121/122 DOE:** retained pairs have post-GUI-created DOE state/outputs absent from their corresponding before inputs. A CLI Save from the before projects would not be an equivalent control and cannot decide the missing side effects. Needed evidence is the qualified post-action project state or a fresh equivalent original action, neither available for an original-free comparison.
- **B:98:** CP137's retained validation continues to report one raw historical DIFF. CP136's full 29-path acceptance run has a durable input/candidate/output receipt and exact path-only qualification; rerunning the same harness for this unrelated CP137 change would repeat settled evidence.

## Freeze and evidence binding

The CP137 validation is source-stable, 328 registered (321 clean, six logged gaps, one logged difference), with historical 93 SAME / 5 DIFF and no objective-complete claim. Its artifact receipt is `build/wave3-checkpoint137-verified/receipt.json`; the CP137 source SHA-256 is `4327c2b399e739e69465845462a39c7fc441c1ba4c0456767b195a0061dd3278`, CLI SHA-256 `51b5e0a44e480f5f8fe25a0d0f15677fcd74206d17573d7a3e8918d353f77a64`.

The closest completed headless full-output comparison is `analysis/resume137-cp136-b98-full-output-comparison.md`, bound to CP136 and retaining raw and one-path-qualified MSTs separately. Its result is valid for that freeze only; CP137 has not been re-run through the B98 comparator here.

No source, current candidate, retained fixture, GUI, original application or mutable REA state was changed or run. The independent blocker is the missing supported GUI-control release/process observation for A55 and the absent equivalent post-action DOE inputs; the only fully comparable B98 raw difference is already isolated to a project-root string.
