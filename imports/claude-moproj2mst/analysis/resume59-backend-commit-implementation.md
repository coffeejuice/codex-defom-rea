# Isolated per-generation backend setter prefix

The new helper records the backend-pointer store only after an explicit prerequisite driver returns for the same actual Session/model generation. Its default registration/removal driver refuses. It does not publish a backend witness to the flow registry, qualify SetOprHasDoe population, or change public B121 parsing.

Only the three assigned new source/test files and this receipt were written:

- `source/apps/moproj2mst/include/m2m/doe/backend_commit_prefix.hpp`
- `source/apps/moproj2mst/src/doe/backend_commit_prefix.cpp`
- `source/apps/moproj2mst/tests/doe/test_backend_commit_prefix.cpp`

## API and actual owner checks

```cpp
BackendCommitPrefix(engine::ConversionSession& actualSession,
                    const std::shared_ptr<FlowReceiverWitness>& actualReceiver);
void BackendCommitPrefix::apply(engine::ConversionSession& actualSession,
    BackendSetterOccurrence, BackendSetterPrerequisiteDriver&);
std::optional<const void*> BackendCommitPrefix::current(
    engine::ConversionSession& actualSession) const noexcept;
void BackendCommitPrefix::invalidateCurrentEvidence(std::string reason);
```

Construction and every apply require the same live Session Project, its actual registry/notifier, an exact registry member including shared control-block identity, a live witness generation, actual Project-owned SUB membership, the same embedded FlowChart, and the factory's `&sub.flow` identity token. Saved Project identities are compared before dereference. The helper retains weak owner/generation leases; it cannot revive released Project storage. An escaped witness lease is insufficient once its SUB or registry lifetime ended.

`current` begins unavailable. It returns unavailable for a foreign/dead/released actual context. A committed null is distinct from unavailable. The record owns no backend object or lifetime; the caller must invalidate evidence across any unqualified write/publication/lifetime interval. Invalidation retains earlier attempt history and requires an explicit reason. This is an isolated projection, not a claim that a declared test driver established a real native backend.

## Preserved native order and guards

Controlling evidence is `analysis/resume59-doe-notify-next.md` (SHA-256 `746fff4d803ace31221b2ac58f4d4ad914031dc89451674201148997b4834e16`). It verifies secondary model vtable `0x14213ba88+8 ->0x140071b30`; derived registration/removal precedes leaf `0x14014c3f0`, whose `0x14014c404` store assigns interface `+8` / whole-model `+0x18`.

Each explicit occurrence carries a nonzero diagram-call identity and one fixed source site:

| Occurrence | Caller site | Required argument | Prerequisite |
| --- | --- | --- | --- |
| OldModelDetach | `0x140170540` | null | removal call `0x140071b8e ->0x1402cffd0` |
| FirstNewModelAttach | `0x140170581` | non-null actual backend identity | registration `0x140071b69 ->0x1402cfbe0` |
| SecondNewModelAttach | `0x1401705f4` | non-null actual backend identity | the same registration path, again |

Unknown sites, missing call identity, contradictory arguments and repeated attempts at the same call/site refuse before driver effects. Separate source occurrences, including repeated same-pointer attachments, remain separate attempts. The caller owns the surrounding old-detach/new-attach order and intervening view effects; the helper never synthesizes missing occurrences or claims their history is closed. The no-nested-apply rule is an explicit adapter scope guard, not a newly inferred native event-loop rule.

The driver receives the same actual notifier, model witness, Project, SUB, occurrence and fixed `2signalDiagModelCanAddItem()` / `1slotDiagModelCanAddItem()` strings, flag 0. This is a distinct notifier key from SetOprHasDoe. Missing driver qualification and default driver methods refuse at registration or removal with `M2M_E_UNSUPPORTED`; there is no empty-map fallback. A nonempty diagnostic qualification names the declared adapter profile only and does not establish native completion.

The helper records prerequisite entry/return and propagates its exception, retaining previous committed projection and the driver's partial effects. After return it revalidates the actual live generation before recording the leaf store. Project release or SUB death during the active prerequisite refuses at GenerationChanged without publishing the new value. Diagnostic history retains the completed prerequisite and refusal. No owned backend, QIcon, graphic-map or Operation Boolean is created or modified.

Actual production registration/removal and population remain unknown. Before production wiring, recover the complete `0x1402cffd0` removal contract and supply the actual same-notifier prerequisite binding plus full source-call history. Existing Session diagram timer/attachment commits do not supply those effects. This increment installs no production caller.

## Focused tests and validation limits

Eleven new cases cover:

- Default unknown registration/removal and unknown registry backend.
- Same actual notifier/model context, before-store observation, distinct first/second/repeated same-pointer occurrences and exact caller sites.
- Null detach after its removal prerequisite.
- Partial prerequisite effects and prior commit retained on refusal; no replay of a refused source occurrence.
- Foreign Session/witness/shared-control-block rejection.
- Actual Project release before apply, and release during an active prerequisite before the leaf store.
- SUB death and replacement retaining separate generations without transferring state.
- Explicit unknown-interval invalidation while retaining history.
- Invalid occurrence guards before prerequisite effects.
- Qualified diagnostic null commit still leaving SetOprHasDoe Population unavailable, with its original flag90 reset retained.

Tests use explicitly labeled diagnostic prerequisite drivers. They do not implement the original Qt notifier, native QObject construction/destruction, backend lifetime, whole setModel or Close parity. Release tests invoke the actual candidate Project storage-release boundary with no diagram binding; they do not claim normal Close was replayed.

Static review confirmed the current actual Session/registry/witness interfaces, non-lazy owner checks, shared-control-block membership before witness dereference, eleven test declarations/Testkit main, complete OpClass include, and no trailing whitespace. No build or executable tests were run in this lane. Parent should build `m2m_doe_backend_commit_prefix`, run `m2m.doe.backend_commit_prefix`, and retain flow receiver constructor/witness, notifier binding, and scalar-prefix regressions. Existing CONFIGURE_DEPENDS discovery requires no CMake change.

All existing public nonempty DoeVariables, scalar post-commit delivery, Population, BackendWitness, and icon/backend guards remain unchanged. `FlowReceiverWitness::backendWitness()` still returns unavailable. No shared interfaces, public Save/B121, CMake, original evidence, application, GUI, mutable REA state, build or deployment was changed. Token measurements are unavailable.

## Frozen source hashes

| File | SHA-256 |
| --- | --- |
| `include/m2m/doe/backend_commit_prefix.hpp` | `bff0cf38fa492634904e7890f57c9e7f370f8d14b904be032fa018f7bc29f990` |
| `src/doe/backend_commit_prefix.cpp` | `cb3695f1d430ef37b4d77a89f1d276d77e8ee763acda32531d50644e9f813dc7` |
| `tests/doe/test_backend_commit_prefix.cpp` | `560c35c60473151d9858baa5f241953befabc4646e87520b9b89d3c735c9b256` |
