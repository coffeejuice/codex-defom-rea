# CP112 — bounded DBLIST setter xref attempt

## Outcome

The requested xref could not run because Ghidra did not finish importing and analyzing the original executable within its configured 330,000 ms startup deadline. The request returned `provider_timeout` for operation `xrefs`; no xref list, setter caller, receiver dataflow, or lifecycle evidence was produced. I did not retry the unchanged query or run a broader procedure analysis.

The read-only session was closed afterward. A follow-up `binary_session` confirmed `open=false` and `cleanup_pending=false`, releasing this REA slot.

## Attempt identity and exact scope

* Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`
* SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
* Provider: Ghidra 12.1.4, x86-64 PE, analysis-profile digest `bb0af08834149e89edadd849fc76d93580191b315769be5638e63d0f4ebf4051`
* Query: incoming references to exact setter address `0x1402cbf30`
* REA run id: `7a7be371-5ea3-41c3-891f-a592a982b1c0`
* Timeout: `provider_timeout`, `timeout_ms=330000`; diagnostic says Ghidra did not finish importing/analyzing within startup deadline.
* Original file size in admission receipt: 67,611,136 bytes. The temporary imported snapshot matched the target SHA.

The returned diagnostics identify startup, import/analyze timeout, and incomplete output. They do not say whether Ghidra completed any usable partial analysis, and there is no evidence ID for a completed xref result. Therefore the attempt neither confirms nor contradicts earlier bounded DUMPBIN findings.

## What remains unknown

The exact unresolved fact is still a setter-call occurrence where `rcx` is proven to equal application-state singleton `0x143d766b8` (or a proven alias), followed by same-instance write/reset order sufficient to establish `+0x1c` at DoeOutput admission. Existing receipts show the helper writes one through its argument and the handler reads the singleton's field, but do not join those two receivers. CP106's CRT initialization receipt binds the AppState constructor occurrence before `main`; it does not establish a `+0x1c` constructor write or setter call.

The next useful static attempt requires a changed, qualified execution setup—such as a provider profile that skips expensive analyzers, if that profile is approved and its coverage is documented—or an already analyzed retained database. Do not present this timeout as a negative xref result. Until a receiver/lifecycle witness is obtained, retain the DBLIST public guard and unavailable candidate admission state.

## Limits and changes

The operation was a read-only static query against the immutable shipped executable. It did not execute the original app, access the GUI or mutate a persistent analysis database. No source, build, project data, or mutable REA database was changed. Session release was verified as described above.
