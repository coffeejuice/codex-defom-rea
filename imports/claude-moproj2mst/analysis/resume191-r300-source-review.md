# CP191 R300 storeSimNames independent source review

## Verdict

**PASS. No source-review blockers.** Current implementation follows `resume191-r300-next.md` and the plan review. The implementation report accurately describes the inspected source and fixture. This is static review; root still owns focused/full test execution and candidate freeze.

## Reconciliation and production boundary

Compared both current files with the retained CP190 `build/wave3-checkpoint190-verified/source.zip`. The only production difference is the immediately invoked lambda around the existing nullable SimControl acquisition, null return and four stores. The only test difference is the inserted `store_sim_names_step_scope` case. Existing source and fixtures outside those regions are unchanged (line comparison).

`save_current.cpp:219-243` retains active-subproject lookup and absent-subproject return, the operation-name snapshot, flow number, live local `simEntries` vector, first matching position with immediate break, no-match return, and unnamed-entry refusal before the one `opModel` acquisition. An unnamed first match is still refused; the loop does not skip it in search of a later named match.

The explicit `-> void` lambda is invoked immediately, is not stored, and receives exactly `m ? m->simCtl() : nullptr`. It retains `if (!sc) return`; because no code follows the callback, that local return preserves the previous function-tail return behavior. `GuiModel::simCtl` at `gui_model.cpp:179-183` still runs `requireGeometrySyncAvailable()` before checking the step and returning its embedded control or null. No direct step access, additional model lookup, or alternate admission helper was introduced.

The callback preserves exactly these ordered stores: `strncpy(oprnam, local8(name).c_str(), 0x40)`, `cursim[1] = no`, `strncpy(simnam, local8(first->name).c_str(), 0x50)`, `cursim[0] = first->sim`. Each locale conversion stays at its original store after SimControl admission. No preconversion or extra terminator was added. The callback neither returns nor saves `sc`, and no reference to its pointee escapes. The captured `first` still points into the live local vector for the synchronous invocation.

## Direct fixture and preflight

`test_save_current.cpp:505-612` opens configured corpus46, activates subproject 0, requires operation ID 2 and `SfOprHeatTransferExpress2D`, installs loader slots, sets the class current operation and loads the actual model. Project, class/model and step owners remain alive. The new case invokes `storeSimNames` directly and performs no Save or full-sync call.

Its independent expected-entry helper computes flow number and `simEntries`, selects the first matching position, requires existence and named admission, then copies the entry before the temporary vector dies. Before each positive call it binds expected strings/scalars and seeds both complete name buffers plus distinct scalar sentinels. It checks both bounded byte ranges including zero padding, the excluded indices 0x40/0x50, both CURSIM values, and cached-model/step identity. Those excluded indices are in bounds: the recovered layout declares OPRNAM[0x41] and SIMNAM[0x51]. Two short ASCII operation names force expected entry data to be recomputed. Bounded string construction remains safe if a store is missed.

Before detachment, matching named entry admission is required again. `DetachedStep` moves the unique step owner into retained storage, deletes copying, and restores through its noexcept destructor, including during exception unwinding. Only the direct helper runs while detached; checks cover the complete retained name buffers, both sentinel scalars, cached model and absent live step. Normal exit verifies restored step identity and repeats the positive check. An exception fails the Testkit case, rather than being swallowed. No optional no-match negative was added; the unchanged early return is covered by source inspection.

Existing includes resolve the internal helper, writer SimEntry/simEntries and flowNumber declarations. No include was added. The test file has its existing `main` calling `testkit::run_all`; CHECK, CHECK_EQ and REQUIRE are supported and remain active in Release. CMake's existing test glob/name mapping produces `m2m_s_save_current` and `m2m.s.save_current`, linked with m2m_core and testkit. Actual configured registration and execution remain integration checks.

## Evidence and limits

Packet S `source/docs/re/moproj2mst/s/spec.md` section 1 retains the annotated 0x140054540 selection and four-store order/widths. `simcount.cpp:42-59,264-279` supports MST-order traversal and the base operation-derived named entry. This is retained annotated source evidence, not newly inspected native assembly or fresh native execution. The unnamed-entry refusal remains reconstructed policy. This cut establishes lexical alias confinement; it does not pin ownership, prevent destructive reentry, install the parked Stopping barrier, close aggregate ownership/admission, or establish public GUI Save equivalence.

Reviewed file SHA-256 values match the implementation release:

- `save_current.cpp`: `62d4f2a827a5fc281692e0b5d8199a549a27594585018fed77ac3d399a46c455`
- `test_save_current.cpp`: `780eeec08f58c8a5afc92e512ae01944e5cb9c8e7c19da0b4b8fe071871349cb`

This review wrote only this report. No source, test state, build, GUI/app-control session, original process, REA database or deployment was changed or run.
