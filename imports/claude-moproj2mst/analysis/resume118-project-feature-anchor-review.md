# CP118 Project feature-context anchor review

## Result

No blocker found in the frozen implementation. The Project-owned anchor is passive, tied to the existing Project storage epoch, and records only the logical constructor-attempt boundary and an adapter timing observation. The attempt is entered at `initProject` after the actual Session Mo has been prepared but before `f10` and the Project backlink are written. It stops before the unqualified proxy allocation. Existing unprepared adapter initialization still proceeds, and the anchor advertises no native constructor completion, heap effects, feature values, registration, or public Close readiness.

## Timing against native and candidate order

The retained cleanup-order report places native `0x140089917` feature-context construction before the later Project `f10/path/unit/SUB0` writes and distinguishes that constructor from later Open registration (`0x1400aa960`). Candidate `initProject` writes `tokMode=0`, invokes `beginFeatureContextConstructorAttempt()`, then writes `f10=5`, constructs path/DB/title and remaining fields, and creates SUB0. This preserves the report's relevant order: the candidate source hook precedes `f10`, path, unit and SUB0. The earlier `tokMode` reset is a retained `initProject` predecessor and does not move the hook after the later fields.

The actual Save call at `core/convert.cpp:206-208` prepares the actual Mo before `initProject` and commits `moProject=&p` after it. The hook therefore observes the intended owner at the intended phase. It captures the adapter's current `f10` only as timing evidence, explicitly not as native feature state. The pending source site and next allocation site are reported separately, so no successful native allocation is implied.

## Identity and lifetime

`Project` creates one anchor from the same `slotBorrow_` storage epoch and actual `slotsAnchor_`; the anchor retains weak references to the epoch, slots, and prepared model, plus identity-only raw pointers. The prepared-model gate requires Mo kind, positive model ID, path set, actual Project/slot identity, same shared ownership control block, and null backlink at first entry. A later invocation permits the same Project backlink but detects changed pointer/control-block/slot binding and makes that mismatch sticky rather than rebinding the consumed attempt.

`Project::~Project` marks the anchor's Project scope inactive before ordinary members are destroyed. The anchor does not keep Project, model, slots, or the storage epoch alive. The tests cover distinct live Project epochs, same-address reconstruction, storage release, same-pointer/different-control-block rejection, and the pending stage remaining unchanged through the ordinary adapter backlink commit. A held diagnostic anchor exposes stale identities for comparison only; it has no continuation or mutation API.

## Effect and public-boundary checks

The new storage code creates only a C++ metadata shell. The attempt hook contains no native heap call, provider lookup/deletion, feature-byte default, global access, map/container mutation, or registration. On the first qualified attempt it records the pending state and returns at `0x1412f107b`, before the 8-byte proxy allocation. It does not claim the later 48-byte sentinel allocation or cleanup behavior.

The source changes do not edit `ConversionSession`, Save dispatch, `releaseProjectAfterCloseEffects`, `ModelManager::clear`, or the existing Close coordinator/guards. `completeForPublicClose` and all native-completion flags are compile-time false. Legacy unprepared `initProject` calls retain their previous field/SUB0 behavior; the prepared path also continues after the pending report. Thus the candidate neither enables public Close nor introduces an Open rejection at this heap boundary.

## Scope and limits

The receipt and source were reviewed read-only. No build, tests, GUI, original execution, mutable REA operation, or source edit was performed. This review validates the candidate's static ordering and owner checks only. It does not qualify native allocation results, constructor completion, provider/global state, Qt registration, or accepted Close.

Frozen implementation receipt: `analysis/resume118-project-feature-anchor-implementation.md` (SHA-256 `5ec4a8ee61b697931179295f72990ebe110931442a67f411adfa08b539aa4b7e`).
