# Resume 50 non-GUI acceptance comparison

## Current gate state

The continuation checker still reports `CONTINUE`, 15 unfinished items, and all eight objective completion gates pending. The verified integrated candidate is checkpoint48 (`analysis/wave3-checkpoint48-validation.json` / `wave3-checkpoint48-integration.json`): 151 tests registered, 144 plain passes, six gap-qualified passes, one diagnostic-difference pass, zero failed assertions, and `objective_complete: false`. Its acceptance limits remain explicit: no original executable was launched, the corpus ABI assertion permits `M2M_OK` or `M2M_E_UNSUPPORTED`, and the folder comparator remains diagnostic.

Checkpoint49 source work has separate successful focused **build** receipts for Close model slots, DOE task factory, and Distortion contacts. Those receipts bind different source hashes and report successful compilation, not test execution or one integrated checkpoint49 source freeze. For example, `resume49-close-focused-build.json` builds target `m2m_mdt_mo_model_slots` from hash `6dc12e17...`; `resume49-doe-focused-build.json` and `resume49-distortion-focused-build.json` bind other hashes. Do not combine those build receipts into a single tested candidate claim.

## One executable comparison that needs no GUI control

Run the already-built focused test target for **Mo Project owned-slot teardown order**, then compare its ordered destruction/guard assertions to the retained original static Close chain:

```powershell
ctest --test-dir build/candidate -R '^m2m\.mdt\.mo_model_slots$' --output-on-failure
```

The target is already built according to `analysis/resume49-close-focused-build.json`, so this is the smallest next executable step and does not acquire GUI control or touch an original process. The expected contract is in `analysis/resume49-close-slot-implementation.md` and the retained manager-tail evidence it cites: Project cleanup/delete and backlink null at `0x140440c0 -> 0x14044bf0`, then virtual slot deletion/null stores at `0x14045000` for the five represented stores, followed by base cleanup `0x140cda520`. The test should confirm the port's actual owner capsule preserves its specified five-slot release sequence, prevents aliases from outliving storage, and rejects repeated/out-of-order release. Record the test log/hash against the exact source hash before using it as candidate evidence.

This comparison is deliberately narrow. Passing it would verify the new storage helper's modeled ownership and order against retained static source evidence. It would **not** prove native `QPointer` invalidation, QObject/Qt destructor payloads, null-rebind callbacks, real Project+60 cleanup, base callbacks, public Close execution, or whole lifecycle parity; the worker receipt explicitly leaves those guarded. It is therefore useful architecture evidence, not a completion-gate pass.

## Checkpoint49 integration required after focused result

After this focused test, the coordinator still needs to integrate the new storage source into the main/session path and standalone test targets, retain the actual registry owner for the required lifetime, and run affected regressions. The Close receipt says `conversion_session`, `model_manager`, and build integration remain root-owned. Preserve public Close guards until earlier phases and the actual child destructor sequence are bound. Do not use a passing ownership unit test to enable lifecycle effects it does not observe.

## Other current work that must not be folded into this comparison

- DOE TaskInfo default-font helper is isolated and intentionally not wired into the native factory; its report says runtime tests are pending and the Report QObject TLS/refcount/thread inputs remain guarded.
- Distortion valid-data contact-pair target has a separate focused build receipt; its tests and exact candidate binding need their own validation.
- Editor style15/17 gradient stop ownership remains a dependency; an empty-gradient helper must preserve vector ownership and allocation/refcounts, not collapse to an opacity boolean.
- The historical A55 and DOE folder differences and B98 path difference remain separately qualified; none is addressed by Project slot teardown.
- No fresh independent original Open→Save→Close result exists; the earlier prepared Billet run crashed after Open.

## Evidence inspected

- `analysis/takeover-state.json`, `analysis/wave3-checkpoint48-validation.json`, `analysis/wave3-checkpoint48-integration.json`.
- `analysis/resume49-close-focused-build.json`, `resume49-close-slot-implementation.md`, `resume49-doe-focused-build.json`, `resume49-doe-default-font-helper.json`, and `resume49-distortion-focused-build.json`.
- `analysis/resume49-graph-ownership.md` and the original evidence references recorded there.
- Read-only `python admin/check_continuation.py`: 15 remaining items; all eight gates pending.

No source, build, GUI, original process, fixture, or mutable REA database was changed or run in this audit.
