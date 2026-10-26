# Checkpoint 70: detailed outcome static review

No correctness, lifetime, or status blocker was found in the inspected implementation of `convert_details.hpp` and `convert.cpp` against `resume70-doe-detailed-outcome-contract.md`. The review is static and does not substitute for root's grouped build and focused runtime checks.

## Verified paths

Paths are beneath `source/apps/moproj2mst/`.

- `include/m2m/convert_details.hpp:14–18` contains only a shared const journal, I/O mode, value-only SaveCallState and returned integer code. It exposes no Project/Session/device pointer or executable FileEffects. The included SaveCallState definition has no owning runtime references.
- `src/core/convert.cpp:159–164` resets details after obtaining the engine mutex and keeps saved plus Save-entry validity outside the try. An old journal is not retained as the outcome of a new entered call. C++ failures before reset are outside the returned-call guarantee.
- `convert.cpp:198–203` attaches the exact actual StudyIo journal and mode immediately after factory assignment, before the later owner bindings at lines 204 onward. A subsequent owner/Open/Save guard cannot destroy the caller's journal alias.
- `convert.cpp:329–347` marks Save entry immediately before each real engine Save call, including the narrower profile. Native project Attempted is set before the call and Returned only after normal return (335–338). The native call passes `&saved.project`, preserving the same optional direct-writer observation alias.
- `convert.cpp:447–451` copies only SaveCallState after catches, only when entered, and sets returnedCode to the actual returned Result code. It does not interpret default-true writer flags as completed writers. The caller must gate these values by Attempted/Returned.
- `convert.cpp:470–478` executes the same run once, with writeFile true and the detail observer. Existing overloads pass a null observer, and legacy SideBytes clearing/collection remains unchanged. No recording detailed-conversion overload is added by this slice; lower-level recording Save tests remain distinct from this native public entry point.

Moving saved outside the try leaves its callbacks and current pointer internally alive briefly after Project destruction, but they are not invoked or exposed during catch/finalization; ordinary destruction of their value containers does not dereference the captures. There is no new lifetime defect from that holder in the inspected flow. The canonical journal contains its own value/store data, while StudyIo's warning callback remains outside the journal.

## Nonblocking public contract clarification

At `include/m2m/convert_details.hpp:12–24`, the current comment explains ownership but omits the validity rules required to interpret the public fields. Minimal fix: add a brief API comment stating all of the following; no runtime change is required:

1. Details reset for each entered call; read/reuse them only after this synchronous call returns, with no concurrent access. A shared const alias is not a thread-safe live snapshot.
2. Null journal means no actual StudyIo binding; empty bound journal means no XML attempt. Recording bytes describe the simulated store; native stream acceptance does not establish durable bytes.
3. `saveCall` is present only after actual Save entry. Writer Succeeded defaults have meaning only when the corresponding Returned flag is true; Attempted without Returned is an interrupted call, not ordinary false. `returnedCode` corresponds to the returned Result, rather than XML or project-writer success.
4. RefreshBoundaryReached observes the reached boundary only. It does not mean refresh delivery or completed Save. This API performs native conversion; it is not the legacy convertToBytes adapter.

This is API documentation debt, not a discovered implementation failure. Root can add these comments before freeze without expanding the source slice.

## Inspection receipt and limits

| Inspected file | SHA-256 |
| --- | --- |
| include/m2m/convert_details.hpp | AA40AA82A97066C7F67892D4750638A9DFBBFFDA21D3B69344B734BE808B3C8D |
| src/core/convert.cpp | 5A167FCDFD8326E80377D019BD7A53B87B7F378A031E5863F804C2D0754DEC33 |

Shared root edits are in flight; these hashes identify this review snapshot, not a frozen build. Supporting reads were limited to the existing SaveCallState, StudyXmlJournal/StudyIo ownership, Save entry implementation, and previously inspected native observer. No source/test edits, build, CTest, GUI, mutable REA, original application action, or new native evidence capture occurred. This report is the only write in this review lane.
