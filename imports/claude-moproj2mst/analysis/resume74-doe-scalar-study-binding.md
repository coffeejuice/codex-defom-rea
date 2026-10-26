# Checkpoint 74: scalar projection and the actual lazy Study owner

The B121 direct-read result can be bound to the Study already owned by an actual Type2 SUB for an explicitly isolated model test. It cannot replace the nested-variable reader in lazy activation as a recovered native parse: that would skip the native variable/keyword construction and `SetOprHasDoe` notification that precede final nominal text, selector state, reindexing and root Operations processing. Public Type2 Save must retain its nonempty-variable reconciliation guard even after an isolated owner binding.

This lane wrote only this report. It did not modify source, run builds/tests, control an application or use mutable REA.

## Existing owner and entry path

Candidate paths below are relative to `source/apps/moproj2mst/`.

1. `src/model/subproject.cpp:289–310`, makeSubProject(Project&,type,id), constructs a real SUB and its flow-constructor witness, sets `sub.project`, type and id. Type2 calls makeStudy, stores it in `sub.ownedDoeStudy`, then sets `sub.doe` to that same object.
2. `src/doe/doe_study.cpp:84–95`, makeStudy, sets `Study.sub`, constructs the initial `DoeStudy<id>` name and attempts its study directories through the existing Project StudyIo. Directory failure is ignored as qualified. The later reader must reuse this object, not construct a replacement Study whose directory effects would be repeated.
3. `subproject.cpp:344–354`, activateSubProject, makes the target active and parses its retained DOM when not loaded. `parseSubProject:366–389` sees the actual SfDoeStudy child of the active SUB, requires owned Study/raw alias and Project StudyIo, then passes that Study and I/O into readLazyStudy. Child order matters: earlier DiagramItems may already be constructed, and later children/connector/loaded/view effects remain unreached after an exception.
4. `src/doe/activation.cpp:31–47`, readLazyStudy, commits the SfDoeStudy name first, derives the Control path from that name, reads DoeInputTable.XML through the actual StudyIo, and calls readInputTable on success. Failed input open records the lastError and still continues through later lazy sample operations. An exception from a nonempty variable reader does not continue.
5. `doe_study.cpp:375` onward, readInputTable, parses DOM, applies common header children in document order, then currently refuses the first nonempty DoeVariables at its native `0x14090ee20` boundary. If reading returns normally, readLazyStudy next calls the three load-like sample functions (which actually write headers at their phases), then reads current DEF_DOE.PAR. Do not move those sample effects before the nested-variable boundary.

This is the exact lazy owner seam. It is distinct from selecting DOE output tasks/session payloads. Current convert explicitly activates SUB0 (`src/core/convert.cpp:235`); an internal fixture selecting an owned Type2 SUB1 does not establish a newly supported public selection path.

## Why a fully parsed direct result is not native lifecycle completion

CP72/73's `readB121TlocModelPrefix(root,Study&)` verifies the complete fixed schema and both maps before committing one ordinary Var to `Study.scalarPrefixVariableOwners` and its alias to `Study.vars`. It stores final nominal text/parameter and root operation bindings. Its role is a candidate model projection, with no native constructor/notifier effects.

Retained original `0x14090ee20` evidence in `resume50-doe-variable-body-pe.md` instead establishes:

- Create/append variable via actual manager virtual+0xf8 (`0x140919de0`), append before assigning name, set selected index and invalidate all sample modes. Then assign varId/dimension and process children.
- For the scalar keyword, construct/append the native keyword record, merge its operation IDs, then call manager virtual+0x118 (`0x1400d25a0`) for `slotSetOprHasDoe(operationId,true)` in native order. Int operation ID precedes bool true.
- The notifier occurs before the later parameter selector/type/text setter effects and variable reindex/deferred links. A nominal text collected from XML is not yet the keyword's committed nominal string at that boundary. The root Operations reader is a later sibling of DoeVariables.

`src/doe/variable_prefix.cpp:73–160` already models this bounded early scalar prefix and deliberately throws before unresolved delivery. Its typed keyword has constructor text/parameter values and a separate PendingScalarVariableSignal with collected nominal text. The direct-read projection contains more final data than this reached native state. Copying it into that prefix after the throw would advance past unexecuted effects.

The actual Session/StudyIo notification binding exists as `SetOprHasDoeSessionBinding`, but its production constructor-history projection does not close receiver backend/liveness state. Fresh unsuppressed dispatch resets flag90 and then refuses at unavailable ordered population; a declared diagnostic list or default false/empty backend is not native evidence. Even if a qualified suppression/missing-item branch later returns, subsequent selector/text/reindex/root-map effects must be implemented in order before claiming a completed read.

## Implementation-ready isolated owner transition

The smallest independent owner test/helper can explicitly bind the existing **diagnostic model projection**, without modifying lazy activation or its parser. A suitable new isolated interface is:

```cpp
OwnedB121ScalarProjection bindB121ScalarStudyProjection(
    engine::ConversionSession& session, Project& project,
    SubProject& sub, const xml::Node& inputTable);
```

Its state transition is `FreshOwnedStudy -> IsolatedScalarProjectionBound`; it must not use a name such as NativeReadCompleted or set `sub.loaded`. The result can hold weak Study/I/O identities and identify that native variable lifecycle is incomplete. A completed direct projection is not an admission token accepted by public Save.

Before any model mutation, validate actual Session liveProject, owned SUB membership, active selected SUB for this lazy-owner profile, type2, sub.project, nonnull owned Study with pointer/control-block agreement, sub.doe and Study.sub backlinks, exact existing Project StudyIo, noncreating actual Session notifier binding, and same ModelManager owner (`manager.ownerSession()==&session`). Reject fresh-model violations (`Study.vars` or scalar projection owners already nonempty) before calling the direct reader. Never create or rebind a StudyIo/notifier/Study to satisfy validation. Source-bound manager name/header changes from a prior real lazy prefix must remain; the isolated helper should not invoke makeStudy or readLazyStudy again.

Use readB121TlocModelPrefix against this exact actual Study, with its existing strict header/schema guards. This stores ordinary owned Var projection data and raw aliases on that Study. It should perform no filesystem effects, no sample loading/writing, no refresh, no SetOprHasDoe dispatch, no public loaded-state commit and no old DOM replacement. This is permissible as an explicitly isolated candidate model state, not as a production native read branch. Root can implement it in new helper/header/test files if such an API improves owner coverage; no shared activation edit is needed for that test.

The result should be invalidated/refused when rechecked after Project/SUB/Study/I/O replacement or release. Retaining a strong Study lease must not make a dead Project/backlink dispatchable. Find original SUB membership before dereferencing any saved SUB pointer. A weak identity result is sufficient; it does not own the actual Session.

### Focused fixture

Construct a real ConversionSession Project, existing bound recording StudyIo/Session owners and actual Type2 SUB through makeSubProject; preserve its constructor directory receipts, then select that SUB explicitly for this internal test. Retain the exact owned Study pointer and initial sample flags/tables/name. Read the 707-byte B121 XML as test input into a DOM, and call the isolated binding helper.

Assert same Study pointer/backlinks and same StudyIo/notifier/ModelManager; exactly one owned plain Var and matching alias; scalar/map fields; unchanged sample tables/mode/invalid flags (this projection intentionally does not execute native invalidation); no additional directory/read/write/notification effects; no sub.loaded/public DOM replacement change. Then invoke only the already-isolated writer and compare its planned XML bytes to the fixed B121 target. That comparison is model projection coverage, not native lazy parsing or FileEffects execution.

Negative cases: foreign Session/SUB, alias mismatch, missing/replaced StudyIo, existing variables, stale result after release, and wrong B121 schema. Preserve public readInputTable's nested-variable refusal and public saveInputTable's variable-reconciliation refusal. A real readLazyStudy call using the same B121 bytes must still stop at the native variable boundary with no later sample attempts; it must not be silently routed through this projection helper.

## Smallest real lazy-reader source increment and ownership obstacle

If the goal is to advance the **production** lazy path rather than strengthen projection tests, the next implementable boundary is the existing native scalar prefix through its pending SetOprHasDoe call, retaining partial mutations after Error. It must be attached at the DoeVariables branch encountered by readInputTable, while the actual owned Study and I/O remain the ones passed from parseSubProject. A staged result must explicitly mean NativeVariablePrefixInterrupted, not successful parse; readLazyStudy must not reach sample operations after that guard.

That step needs a concrete lifetime interface first:

- Current B121ScalarVariablePrefix holds a shared_ptr<Study> and owns a derived ScalarVariablePrefixRecord. A stack-local prefix destructor removes its alias during unwinding, so inserting it temporarily into readLazyStudy would not preserve the interrupted state for later inspection.
- Storing that prefix unchanged inside Study creates an ownership cycle because it strongly owns the Study. A future retained prefix must use weak/nonowning checked Study identity or be held by an explicit Session-owned arena with verified cleanup, not a cyclic lease.
- `Var` has **no virtual destructor** (`include/m2m/mst/doe_model.hpp:47`). The concrete derived prefix record cannot be adopted into existing `vector<unique_ptr<Var>> scalarPrefixVariableOwners`; deleting through that base would be invalid. The current direct model helper stores plain Var and is safe on that point. Use a correctly typed concrete arena/holder with out-of-line destruction or refactor the prefix representation under explicit ownership assignment; do not make an unrelated destructor/API change implicitly.

Root should assign the exact disjoint Study/prefix/activation interfaces before implementation: likely `doe_study.hpp` for a typed retained owner, `variable_prefix.hpp/.cpp` for checked noncyclic lifetime/continuation, `doe_study.cpp` for the nested branch and `activation.cpp` only if the caller needs a result parameter. `subproject.cpp` is not required merely to select the existing Study; it already supplies the correct owner. Shared Session/notifier files require separate ownership if actual dispatch is attempted. This report grants no source edit.

## Explicit refusal boundaries before public Type2 Save

1. **Current real lazy read:** first nonempty DoeVariables remains refused by readInputTable. The isolated final projection must not bypass it.
2. **Prospective native-prefix integration:** after committed variable/keyword/operation-prefix mutations, refuse at actual SetOprHasDoe delivery/receiver backend until source-qualified continuation exists; do not commit collected nominal text or root Operations early.
3. **A separately isolated model bound to Study:** public Save still refuses at nonempty variable dimension reconciliation (`0x1400d2b60`, Type2SaveNextBoundary::VariableDimensionReconciliation). Direct writeB121TlocModelPrefix does not qualify that caller. Conditional temp.KWD/dimension handling is a separate dependency.
4. **Even after those close:** immediate nonempty XML serialization/device and actual DoeTableRefresh timer/UI/population continuation remain; four PAR calls and later DOE/MST outputs do not become public success by inference. Type3/OPT retains its independent guard.

The earliest missing production interface is retained concrete scalar-prefix ownership through interruption; the first behavioral continuation remains SetOprHasDoe delivery before final keyword/model data. The exact lazy Study object itself is already available. No extra manager or source-independent Study snapshot is needed.

## Inspection receipts

| Inspected source | SHA-256 |
| --- | --- |
| src/doe/activation.cpp | BFDB46BC90CC2940536FC39450170BE6BF590F1C08D79C1B9466E4DEEF1611D7 |
| src/model/subproject.cpp | 187AC92F56CC1CE9A4E1C08D3037C90FE98E1E17A8885340A78CA5901863977C |
| src/doe/variable_prefix.cpp | 648F27D48011D18A467806DA90CEAAB045811FF4998DB8FE80E37926B47E759E |
| include/m2m/mst/doe_model.hpp | 1706AE4DB922B1E6CDA879F62494F21F996132C2470530ABB90996C6D3A40429 |

Paths are beneath source/apps/moproj2mst. Native mutation-order claims use the retained CP50/51 PE qualification and CP58/59 constructor history reports; scalar projection/writer scope uses CP72/73 reports. Historical B121 XML remains 707 bytes with SHA-256 `18ec0c482730c82ade12e91771ca67e53e4604573032624b99f3f4829a86b069`. No new original capture, runtime parity or public success claim is made.
