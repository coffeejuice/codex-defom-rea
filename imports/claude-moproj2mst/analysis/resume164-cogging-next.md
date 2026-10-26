# CP164 Cogging Save CWD/path mutation and failure-order continuation

## Finding

There is no new source-ready public CWD contract on the retained evidence. The smallest justified cut remains explicit-directory path projection/testing only; a public Save path hookup based on `projectDir(p)` would assume away the event-pump mutation window. The native Cogging MDT writer itself has a source-ready failure-order contract: it opens the exact destination before serialization; a failed open returns the handled-class result, leaves writer globals untouched, and allows subsequent property writers and MST generation to proceed. Candidate source already models this native writer contract. Do not expand it into transactional rollback or change the outer Save sequence.

The native CWD question is narrower than “does Save call chdir?” The synchronous Save prefix has no direct current-directory setter between its event pump and the relative `QFile::exists("MAN_COUPLE.DAT")` lookup in Cogging geometry processing. But `QCoreApplication::processEvents()` can dispatch File > Open, whose OpenProject route sets CWD to that project's directory. Static evidence proves the route is possible, not that it occurs during a retained Save. Project identity/operation selection after reentrant Open is also unobserved.

## Smallest CP164-ready cut

1. Keep the converter’s explicit path helper and native Cogging writer semantics unchanged. Candidate unit coverage may assert the explicit lookup-directory argument and the native writer’s failed-open continuation/order, but must label these candidate-only checks; they are not original runtime proof.
2. Do not wire ordinary Save to an inferred project directory or claim CWD stability. The missing original witness is one fresh same-run trace spanning immediately before and after Save’s `processEvents`, through `0x140052ec0`, and to `0x140762f30`’s relative existence call. Record CWD, project/active-operation identities, each `QDir::setCurrent` caller/target, and the exact `QFile::exists` result.
3. To establish whether the concrete Open route changes the lookup outcome, queue a File > Open of a second disposable project with `MAN_COUPLE.DAT` only there, and record whether it dispatches inside the pump. Preserve original failure/order outputs and resulting file hashes; do not infer from candidate execution.

Until that witness exists, the CP164 conclusion is “missing witness,” not a new public implementation cut. Failure ordering is established for ordinary file-open failure and normal return; the invalid two-reheat path’s access violation, buffering/flush outcome, and partial file bytes remain unspecified.

## Evidence and qualifications

- `analysis/resume156-cogging-cwd-event-next.md` SHA-256 `BEDE3E2E5D1DDC5B8366AE06496C1CAC7A44534873215E3330C3B97F2AF5079D`: event-pump to OpenProject CWD-setter route and precise missing same-run witness.
- `analysis/resume151-cogging-save-cwd-next.md` SHA-256 `FD021AE643A73C952384684472C48504233DB62880E53726400EF506AE6F1B14`: direct-setter inventory and Save/callback order qualification.
- `analysis/wave3-cogging-native-io-contract.json` SHA-256 `DBF5E6AA0FD00CA155BA174ED28E16A887C4966D3822F22B2674D759C7418B85`; companion `analysis/wave3-cogging-native-io-contract.md` SHA-256 `F07CA40CDA337927355A2562DA42487A5B1FCA18D02D186B2A9943F3828635FE`: bounded native path/open/CRT behavior and limitations.
- `analysis/resume62-cogging-fault-contract.md` (hash not captured in this pass): instruction-qualified two-reheat first invalid read at `0x14076cb80`, after the Reheat 2 banner call; it does not establish whether the address is readable at runtime or what bytes become visible.
- Original PE identity from retained native receipts: `DEF_GUI_MO_64.exe`, v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

This was a read-only evidence audit apart from this report. No source, tests, shared plan/state, build, GUI/app control, or live REA/native database was changed or used.
