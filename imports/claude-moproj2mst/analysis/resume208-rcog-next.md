# CP208 RCOG: queued Open and Cogging lookup continuation

## Disposition

No source-ready public Cogging increment is justified. CP156 established a concrete static route from Save's `processEvents(flags=0)` to File > Open and then `QDir::setCurrent(projectDir(P))`; later CP164/171/203 receipts preserve the key qualification: this is capability, not evidence that Open was queued or delivered during a specific Save. CP201's frozen candidate and its passive trace helper do not supply the missing original event/CWD/owner observation; the helper was explicitly not admitted for an original run because its watchdog termination receipt was defective.

The candidate `SaveEntryEventPrefix` remains an explicit supplied-schedule adapter. Its tests demonstrate only ordered one-shot adapter behavior. Public `cogging_save.cpp` currently probes `projectDir(p)/MAN_COUPLE.DAT`; changing it to process CWD or another Project's folder, or changing post-pump operation ownership, would choose among unobserved outcomes. Do not alter source or tests on this evidence.

## Smallest next original occurrence

One fresh controlled original Save occurrence must retain a common process/run identifier, executable identity, and hashes for the disposable projects. Queue File > Open for a second disposable project whose directory alone contains `MAN_COUPLE.DAT`, then perform Save on the first project's active Cogging operation. In that same trace capture:

1. Immediately before/after `processEvents(flags=0)` at `0x140098129`: process CWD, Project identity and active-operation identity.
2. Every `QDir::setCurrent` during the interval (caller and target), and direct confirmation whether the queued File > Open handler ran and its Open result.
3. The continuing Save's Project and operation identities at first post-pump resolution and at `0x140052ec0`.
4. At `0x140762f30`, process CWD and the exact result of relative `QFile::exists("MAN_COUPLE.DAT")`.
5. Save completion/failure and resulting file effects, with original Save and newly opened Project labeled separately.

If the occurrence shows a transition, encode only that observed schedule/CWD/owner transition at the existing seams and add assertions for both lookup directory/result and selected operation. If no Open is delivered, retain that exact observed outcome; do not generalize beyond this run. A candidate-injected event list or private existence bit cannot replace this witness.

## Qualification

This is a source/evidence audit only. No GUI session, original process, build, or mutable REA state was used. No source, tests, or shared plan/state were changed. The independent work queue remains blocked only on the specified original same-run occurrence; it is not blocked on a candidate implementation task.
