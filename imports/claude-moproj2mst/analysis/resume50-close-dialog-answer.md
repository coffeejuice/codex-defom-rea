# Normal Close question: static default and answer mapping

## Native guard path

`analysis/wave3-normal-close-guard-contract.json` binds the Close model guard `0x140044110` and dialog/button path to original binary SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` and QtGui SHA-256 `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`, using retained static PE/DUMPBIN evidence only.

- Normal Close first calls manager can-close while project and diagram remain attached. The model guard allows immediately if the relevant project pointer is null or application process type is neither 1 nor 5.
- Otherwise it asks a three-button question: Yes `0x4000`, No `0x10000`, Cancel `0x400000`, combined `0x414000`, and requests `NoButton` (0) as the explicit default. For the retained shipped Windows/Common, WindowsXP, or WindowsVista style path with unchanged initial focus, the helper's explicit-default logic selects only AcceptRole, which none of those three buttons has. The recovered focus chain puts Yes first; QDialog auto-default/focus behavior therefore yields Yes `0x4000` as the fresh default.
- The exact default derivation is static and qualified: application startup style selection was not separately recovered; runtime/custom style, environment override, external event filter, focus movement, and actual GUI behavior are outside the claim.

## Return mapping

For the current-operation question, Yes invokes nested Save `0x1400980d0` before detachment and replaces the Close answer with its returned Qt integer. Exactly Yes `0x4000` is remapped to Cancel `0x400000`; other values are not rejected merely because they are zero. The Close guard invokes its post-answer callback and RemoveGraphItems only if final answer is not Cancel; actual teardown/detach occurs later after guards return true.

Composition with the last-operation warning: the warning itself statically offers only Yes; nested Save returns that Yes early before current-operation or file writers. The Close guard maps this returned Yes to Cancel. Thus a warning reached inside normal Close cancels Close and bypasses the accepted-Close callback. This integer path is distinct from nested modal-loop event delivery, which may dispatch eligible events before the dialog returns.

## Candidate adapter guard

An adapter Close branch can model the source-selected answer mapping as:

1. Apply the source-proven process-type/project-pointer gate before asking.
2. Represent the normal three-button Close question's default as Yes only under the retained shipped-style/initial-focus qualification; otherwise require a supplied answer rather than assume Yes.
3. If Yes is chosen, run the nested Save contract and use its integer result. Remap exactly `DialogAnswer::Yes` to `DialogAnswer::Cancel`.
4. On Cancel, retain project attachment and skip accepted-Close display callback and removal dispatch.
5. Preserve modal event dispatch as its own explicit schedule input. A proven default answer does not establish an empty due-event set; do not let the answer guard silently bypass due modal events.

The direct answer mapping is source-closed enough for a unit branch test without GUI capture, subject to the documented style/focus profile. Full runtime acceptance remains pending. Nested Save may include the unresolved warning modal event schedule; do not label the composed Close path fully implemented until that boundary is represented or remains fail-closed.

## Evidence qualifications

This report relies on `wave3-normal-close-guard-contract.json`, `wave3-close-save-return-contract.json`, and their listed immutable static artifacts. The normal question's modal `QDialog::exec` dispatches the thread event loop; eligible timer readiness/order remain unresolved. No original runtime, GUI, build, or source change is claimed.

No source edits, builds, GUI sessions, or mutable REA operations were performed.
