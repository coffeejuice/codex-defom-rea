# CP239: Operation cache-presence fences in staged cleanup

Migrate the four later cache-reappearance fences in `src/model/operation_info_cleanup.cpp` to the existing `Operation::hasEngineModel() const noexcept` query. This is the smallest identified new operation-owner consumer migration with substantive existing refusal/state tests. It requires no common model edit, new ObjectKey, native continuation or fresh original evidence.

All application paths below are beneath `source/apps/moproj2mst/`. Only this report is written. No source/test edit, build, shared recovery-state update, original process, GUI control or mutable REA operation was performed, and no tests ran.

## Exact edit and exclusive ownership

In `src/model/operation_info_cleanup.cpp`, replace exactly these four expressions, at their existing positions:

```cpp
if (operation.engineModel)
    unavailable("Engine cache appeared after the source null skip");
```

with:

```cpp
if (operation.hasEngineModel())
    unavailable("Engine cache appeared after the source null skip");
```

The four owning functions/current locators are:

| Function | Current line | Ordering retained |
|---|---:|---|
| `attemptOperationInfoObjectListCleanup` | 136 | Live OPI and legal-stage checks, Props absence, cache absence, then actual object-list count. |
| `attemptOperationInfoPositionListCleanup` | 190 | Live OPI/legal stage, Props/cache absence, object emptiness, then position count. |
| `attemptOperationInfoRecoveryWrapperCleanup` | 241 | Live OPI/legal stage, Props/cache absence, empty object/position lists, then recovery-wrapper observation. |
| `attemptOperationInfoRecoveryFlagsCleanup` | 291 | Live OPI/legal stage, Props/cache absence, empty lists and null recovery wrapper, then recovery-flags observation. |

One production owner needs only `src/model/operation_info_cleanup.cpp`. Existing `tests/mdt/test_operation_info_cleanup.cpp` is the focused test seam; no new test source or registration is necessary for this expression relocation. Root owns integration, registration confirmation and grouped validation. There is no overlap with CP238's `src/engine/loader_l3.cpp` / dimension test or with the editor NSTART value-boundary dependency. `include/m2m/model.hpp`, `engine.hpp` and `src/engine/gui_model.cpp` need no edit.

Leave `attemptOperationInfoEngineCacheCleanup`'s earlier `const auto& cache = operation.engineModel` and its initial weak observation/address token alone. That state machine genuinely inspects/records more than presence. Its blocked retry still requires null/address/control-block checks in order; CP225 already migrated only its owner-equivalence portion to `sameEngineModelOwner`. Do not replace that identity predicate with this presence query or expand this four-line leaf into an unreviewed cache snapshot API.

## Why this is the same fence

`include/m2m/model.hpp` currently defines:

```cpp
bool hasEngineModel() const noexcept {
    return static_cast<bool>(engineModel);
}
```

The old `if` performs that same contextual shared-pointer boolean conversion on the actual Operation field. The query does not call lazy `engine::opModel`, copy/lock a shared or weak owner, change `use_count`, allocate a model, inspect native step data or perform geometry admission. It returns only a bool. Keep it in each original `if`, after the preceding checks; do not cache one earlier result across staged calls.

Presence here means non-null stored pointer, including shared-pointer aliasing semantics. It is not “a control block exists,” “use_count is positive,” “the class can produce a model,” or “a model is the current owner.” An aliasing shared pointer with a null stored pointer remains false, exactly as before. Foreign/expired OPI identity still refuses before this field is observed, and a late cache still refuses before reading the next list/wrapper or changing progress.

There is a similarly named **free function** `m2m::hasEngineModel(const Operation&)` in `src/ops/express.cpp` (around line 164). It checks `opBase(op)` and `op.project`, predicting ordinary class model availability; it does not inspect the actual cache. Do not substitute that function. Use the explicit member expression `operation.hasEngineModel()` in all four sites. The already-migrated Save gate in `src/datafiles/mdt_write.cpp` demonstrates this member query's intended presence semantics.

## Meaningful focused verification

Use the existing `m2m_mdt_operation_info_cleanup` target / expected CTest `m2m.mdt.operation_info_cleanup`; confirm the configured names rather than copying older reports' pre-renaming command examples. Existing Testkit cases exercise each selected fence through real staged entry points:

| Gate | Cache-appearance rejection case | Successful absent-cache / state-preservation case |
|---|---|---|
| Objects | `opi_objects_retry_rejects_objects_or_prior_payload_appearance` | `opi_empty_object_list_advances_only_to_position_cleanup` |
| Positions | `opi_positions_retry_rejects_new_positions_and_earlier_payloads` | `opi_empty_positions_preserve_storage_recovery_and_live_current` |
| Recovery wrapper | `opi_recovery_null_skip_rejects_reappearing_wrapper_or_predecessor_owners` | `opi_null_recovery_skip_preserves_flags_storage_and_current_at_next_condition` |
| Recovery flags | `opi_flags_null_skip_rejects_flags_or_predecessor_reappearance` | `opi_null_flags_skip_preserves_storage_current_and_unknown_item_cache` |

These cases install the cache only after the required earlier null/empty stages, assert the existing “Engine cache appeared” refusal, and preserve the staged cursor. The positive cases check the reached stop and unchanged storage/current/owner state. Foreign/expired OPI and CP225 same-address/different-control-block cases in the same executable guard against accidentally broadening the identity portion while making this local change.

No accessor-mirroring test is needed for four equivalent bool expressions. If implementation reveals missing state assertions at one reappearance branch, extend that existing case only to assert the previously established progress stage and retained cache identity after refusal; do not add a new production hook or an original/destructor launch. Preserve Testkit `CHECK`, `CHECK_EQ`, `REQUIRE` and its existing main. No files are probed, no scratch fixture is needed, and no new quoted include is introduced.

Root should run this focused target at the normal grouped checkpoint and then the complete source-stable launcher/freeze. This report claims neither a newly passing test nor closure from a prior result.

## Evidence and limits

Reviewed current Operation query, cleanup source/header and focused tests; CP222's property/cache alias ledger; CP224's presence-query recommendation; CP225's owner-comparison implementation and object-mutation audit; CP233's implemented HeatTreat predicate migration; CP237's implemented FormingExpress flag command; and CP238 editor/loader next-leaf notes. The four cache-presence fences remain direct field reads in the inspected source, so this is an unmigrated consumer family rather than reassignment of those completed leaves.

Retained `analysis/resume123-opi-object-list-prefix.md`, `resume124-opi-position-list-prefix.md`, `resume126-opi-recovery-null-prefix.md` and `resume127-opi-flags-null-prefix.md` qualify the original static branches and the conditional adapter prefixes. They distinguish empty object/position lists from nonnull allocated-empty recovery wrappers, preserve removal-before-delete ordering and keep nonnull effects blocked. Those records are static original-image/decompile qualifications and fixture results, not a fresh accepted-Close occurrence in this audit. The migration preserves the current adapter reappearance fences; it does not attribute an `Operation::hasEngineModel()` member API or shared-pointer representation to the original.

Do not confuse the engine cache represented by `engineModel` with native OPI+0x160 item-model knowledge. The canonical item-model observation remains Unknown under CP128/129 qualifications; null engine-cache checks do not authorize item-model cleanup, QObject expiry or public Close. `OperationInfoCleanupOutcome` values remain diagnostic continuation outcomes, not native success returns.

This leaf does not acquire a native graph pointer, touch model-owned object storage or issue an identity. CP225's live ObjectKey blockers therefore are not dependencies, and this report does not repeat issuance work under another name. Public engine-cache assignment/reset and the earlier raw cache observation remain migration surfaces; the four new query consumers do not establish cache privacy, whole-program ownership closure or R300 cache1 safety. All existing refusals and original-witness gates stay intact.
