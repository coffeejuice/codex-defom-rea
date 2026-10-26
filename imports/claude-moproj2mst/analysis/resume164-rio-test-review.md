# CP164 candidate conjunction test review

## Verdict: accept with one evidence-bound wording correction

The added test is appropriately candidate-only. It does not claim an original Save return, native termination, `errno`, original process status, or that a candidate terminal worker ran. Its `M2M_E_UNSUPPORTED` assertion and message are the adapter's ordinary remesh-boundary result.

The three intended failure conditions are exercised in an isolated copied fixture:

- Project output writing returns false after the delegating device closes and its single scripted `error()` query returns 7. The observed callback sequence is `project open` (the output factory), `aboutToClose`, `close returned`, `error query`; the call flags assert project attempted/returned/false.
- MST writing is attempted and returns false after a read-only, `FILE_SHARE_READ` handle is created in `aboutToClose`. The MST sentinel is then asserted byte-for-byte unchanged.
- Enabled remesh writing is blocked by a read-compatible handle held throughout conversion. A preflight `CreateFileW(GENERIC_WRITE)` verifies `ERROR_SHARING_VIOLATION` before conversion, and both primary and fallback contents are checked unchanged afterward. The fallback seed causes the copied fixture's remesh state to enable on input; with remesh disabled, this path would not produce the asserted unsupported remesh result.

The Boolean BWZ and INI side effects are checked, and the MST/project failure flags show that those earlier failures were reached before the final remesh result. The locks and file mutations are confined to `Rig`'s per-process/per-tick scratch copy. The test leaves the existing disabled-remesh test intact.

## Required precision about event coverage

The `events` vector does **not** observe the conversion's input/project `Open` or `Save-current`. Its `project open` event is emitted by `OutputFactory::open` when the later project-file writer opens its output. The ordering it establishes is therefore output-writer open → `aboutToClose` → close returned → post-close error query. The surrounding test comments and save-call flags support that the conversion reached its save path, but the vector itself is not evidence of the earlier Open/Save-current steps.

Keep the review/resume description limited to the observed output-writer sequence (or add an explicit observation hook if direct Open/Save-current ordering is a required assertion). This is a claim-scope correction; it does not invalidate the conjunction result or justify any original-behavior claim.

No source, test, build, GUI/app-control, original-process, live REA, or mutable evidence operation was performed. This review file is the only write.
