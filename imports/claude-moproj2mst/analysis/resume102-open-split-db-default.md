# CP102 Open split-database default branch

## Result

CP98's Save-class inventory remains valid, but its broad dialog conclusion needs this qualification: a concrete, source-closed **Open-time default-answer branch** is already represented and can be tested without a fresh GUI witness. It is the project DB “2G Split DBs” question at native `DBH::open` `0x140cdc480`. This does not resolve Close's nested modal schedule or unblock DoeOutput/Report Save.

## Qualified branch

The native input is a successful primary DB reopen followed by `has_previous_part(reader)` at `0x140814e80`. Its true condition means a previous-part chain is present (the DB spec defines the on-disk detection as a first-step magnitude of at least two plus an existing numbered `<base><n>.DB` part). On this branch, `0x140cdc480` asks whether to load all parts as an integrated DB, with Yes marked `Default` and No marked `Escape`. The fresh shipped dialog therefore selects Yes under the retained default-button qualification. The Yes branch calls `setup_part_chain` (`0x1408150f0`); on return the loader proceeds through the header-cache build when requested, sets `loaded`, and returns success. The false predicate retains Yes as the initialized answer and also calls `setup_part_chain`, which establishes a one-part chain.

Candidate `source/apps/moproj2mst/src/engine/db_load.cpp:176-202` has this profile: it calls the retained `has_previous_part`, records the default-Yes warning, calls `setup_part_chain` in either case, then builds the cache and marks the handle loaded. Public Open reaches `openProjectDb` (`db_load.cpp` later in the file) before opening/activating the project operation. This is a concrete Open dispatch point with a closed predicate, default, and primary receiver mutations.

## Smallest useful test seam

Add or extend an isolated DB loader test around the actual `dbOpen`/project-Open seam using a disposable two-part DB fixture:

1. Make the primary DB open succeed and make `has_previous_part` observe one valid preceding part.
2. Assert that the adapter's fresh-default profile selects Yes, the actual reader's part chain is populated, header-cache loading follows chain setup, and `DbHandle::loaded` becomes true.
3. Retain a one-part control case to assert the same setup call yields the primary-only chain.

The focused source target should cover `source/apps/moproj2mst/src/engine/db_load.cpp` and its existing DB loader test target; do not change `core/convert.cpp`, public dialog APIs, or the Close answer helper. The retained DB packet explicitly says the current corpus has no separate split-chain fixture, so a test needs a scratch-created multipart database or must remain a documented fixture gap.

## Exact qualification limit

This closes the **default answer and synchronous DB receiver branch**, not the modal event schedule. The original `QMessageBox::question` uses `QDialog::exec`; static PE evidence does not establish which already-registered timer or other event callbacks run before the default Yes is returned, nor their receiver identities. Thus the branch is implementable/testable as a source-qualified decision plus `setup_part_chain` effect, but claiming full original modal-loop equivalence still requires a retained original trace of the modal interval and delivered event IDs. Do not use the Open default to fill CP60's two distinct Save-entry-poll and Close-warning schedule inputs.

## Evidence

- `source/docs/re/moproj2mst/mdt/DB/spec.md` §1.4 and §8: native call sequence, prompt/buttons, previous-part predicate, chain behavior and dialog matrix.
- `source/docs/re/moproj2mst/mdt/DB/packets.md` DB-1: `0x140cdc480` path and explicit note that a split-chain fixture was not present in the earlier test packet.
- `source/apps/moproj2mst/src/engine/db_load.cpp:176-202`: candidate predicate/default/chain/cache/loaded order.
- `analysis/wave2-db-inheritance-contract.md` §1 and `analysis/wave2-db-inheritance-contract.json`: bounded native DB helper qualification; its inherited-object helper is not the question caller and correctly has no question.
- `analysis/resume60-close-schedule-next.md`: distinct Save-entry poll and nested Close warning modal intervals, with delivered timer partition unresolved.
- `analysis/resume98-operation-dialog-reconcile.md`: CP98 baseline for remaining operation-class guards.

Read-only static/source reconciliation. No GUI or original execution, source edit, build, test run, or mutable REA action was performed.
