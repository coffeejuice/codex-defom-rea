# CP78 — Static review of the Session Open-read receipt

## Disposition

The bounded integration matches the native accepted-Open ordering at the candidate adapter level: it captures the same existing Session-owned `ModelManager` before creating/registering the Mo child, and keeps native factory completion, concrete main-window ownership, and readiness explicitly unknown. It does not alter public project selection or relax a later Open/Close guard.

Two limits should remain explicit. First, the public full-Save path calls `prepareProjectModel` before `openProject` applies its accepted-suffix check, so a candidate receipt can be created on an input for which native `0x14001ca30` would return before `c9b7ca`; do not treat this receipt as evidence that native Open reached the field read. Second, an escaped receipt can outlive the manager while exposing its historical address through a raw-pointer accessor; keep that value opaque or lifetime-safe.

## Original Open read and child ordering

Retained `evidence/gui_mo/s1/s1-05/sub0_s101.c`, `FUN_14001ca30`, shows that the native path first checks the accepted suffix set. Only the accepted branch derives the `.DB` path and invokes `0x140c9b7c0` at the Open call site (the retained CP78 qualification identifies `0x14001d078`). That leaf reads the existing qword at main-window `+0x50` (`0x140c9b7ca`) and returns it. The caller then checks for a model at the requested path and calls `0x140c79390` to create/register a child only when needed. The field read does not itself construct the manager, perform the child factory, or establish its concrete runtime class.

## Candidate source behavior

In `src/core/convert.cpp`, the full-Save path creates a `ConversionSession` and calls `session.prepareProjectModel(db)` before `initProject` and before `openProject`. In `src/engine/conversion_session.cpp`, `prepareProjectModel` first checks its existing fresh/empty-manager precondition, creates `SessionModelManagerReadReceipt` from that Session's canonical `modelManager_`, and only then constructs the candidate `DbHandle`, assigns its path, appends it and registers the two signal projections. This places the receipt before the candidate child allocation and registration. Later Project binding and current-model selection remain separate calls.

The receipt verifies shared-owner identity with `modelManagerOwner()` plus `ownerSession()==&session`, keeps only a weak manager owner, snapshots candidate counts, and labels its origin `CandidateExistingOwnerRead`. Its source-site string corresponds to the native call site, but all native completion/readiness constants remain false. It invokes no CP77 constructor/store observation. The existing candidate manager is not proven to be the original main-window `+0x50` object.

`ConversionSession::~ConversionSession` revokes the receipt before Project/session teardown. `requireManagerFor` tests revocation before using the stored Session identity, then locks and compares the actual live manager. It cannot authorize a replacement Session. The receipt adds no project selection, current model, event delivery, Project teardown, or Qt lifecycle effect. The focused tests cover same-owner identity, before/after counts, later Project/selection commits, retry/refusal, foreign Session, revocation, and unrelated Session state. No executable test result was produced in this review.

## Boundary 1: rejected suffix precedes native read

`openProject` (`src/moproj2mst/src/moproj/read.cpp:29-40`) resets `openedOk`, accepts `.moproj`, `.DB`, or `.KEY`, and returns false for other suffixes. Native `FUN_14001ca30` likewise checks suffixes before the `c9b7c0` manager read. Current `run` creates the receipt and candidate child before `openProject` performs its suffix gate.

This is an existing adapter ordering limitation because `prepareProjectModel` already precedes `openProject`; the receipt does not change the public error result for a rejected suffix in the normal path, and the local Session is destroyed/revoked. It does mean the receipt exists transiently on rejected input and must not be consumed as a native Open-read witness. If a future caller exposes or uses the receipt to authorize later native behavior, add an accepted-suffix admission condition or move the adapter read edge to after suffix acceptance while retaining its pre-child position.

## Boundary 2: historical raw identity accessor

`SessionModelManagerReadReceipt` keeps `std::weak_ptr<ModelManager> manager_` but also exposes `const ModelManager* observedManagerIdentity()`. This receipt is intentionally shareable beyond Session scope; after the Session releases its `modelManager_`, that returned pointer value may refer to an ended object lifetime. `requireManagerFor` is protected because it checks `admitted_` first and requires a live weak lock, but the public historical accessor bypasses that validation. The test that inspects it after Session destruction also retains a strong `ModelManager`, so it does not exercise the expired-manager case.

This does not affect the Open operation itself, but it weakens the stated safe escaped-diagnostic contract. Prefer an opaque non-dereferenceable token (for example, a stored integer identity) or make identity inspection return a live weak-lock result. At minimum, document that the raw pointer may only be compared while a separate strong manager lease is held and must never be dereferenced; a targeted test should keep only the receipt across Session destruction and verify reauthorization refuses.

## Review limit

Read-only retained-source and candidate-source review. No source edits, builds/tests, GUI control, original execution, or mutable REA operation occurred.
