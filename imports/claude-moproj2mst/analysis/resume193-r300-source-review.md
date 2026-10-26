# CP193 R300 furnacePostLoad independent source review

## Result: PASS

No source-review blockers found. Reviewed `resume193-r300-next.md`, `resume193-r300-plan-review.md`, and `resume193-r300-furnace-post-implementation.md` against the current source/test pair and a direct text diff against `build/wave3-checkpoint192-verified/source.zip`. The production diff is confined to the furnacePostLoad alias scope; the test diff adds only `furnace_postload_step_scope`. CP192 and neighboring owners are unchanged in this comparison.

At loader_l2.cpp:322-336, the original model lookup and null-model return are unchanged. The callback is captureless, immediately invoked once with the original `m->simCtl()` expression, returns void by deduction, and is neither stored nor returned. Its pointer parameter remains local to the invocation. The exact order is preserved: furnace_on = 1; curve_count() == 0; only in that branch local int32 n = 0x16, setup_curves(&n), then set_defaults(). The trailing unused `furnaceProps(c.propsOf(nullptr))` cast remains unchanged, outside and after the callback.

GuiModel::simCtl at gui_model.cpp:179-183 still calls requireGeometrySyncAvailable before checking step. Thus geometry refusal precedes all callback effects and the trailing cast. A null step still yields a null control that this owner dereferences without a check. No admission helper, direct member shortcut, null guard, detached execution, or exception adapter was introduced. These are current reconstructed-source facts; no native null-policy experiment was performed.

The test at test_loader_l2.cpp:219-298 uses corpus08 operation 1 with the established HTFurnace class substitution, binds the property owner, loads once, obtains the existing public slot, and sets cls->current = op. Empty-branch setup clears curves before obtaining any first-curve alias, requires zero count, seeds furnace_on = 0 and NSTEP = 37, then invokes the slot directly. It independently asserts flag 1, exact count 22, and first-curve shape ndim 1, n[0] 2, ncomp 6. The count follows the reconstructed member implementation's 21 fixed entries plus one extra; the shape follows the retained table, without invoking production defaults to compute expectations.

For the populated branch, the first pointer is acquired only after setup. Distinct value = 77.123456789 and u54 = 91 sentinels survive two direct calls, with exact count, first-curve identity, and flag assertions each time. Each branch checks cached model, step, control, property/data/furnace identities, property owner, NSTEP, copied furnace count/name and data name. All six FileEffects vector sizes are snapshotted after load and checked after each call. Together with the slot's unused FileEffects parameter and unchanged body, this supports absence of new file effects in this source cut; vector-size assertions alone are not a general filesystem audit. Existing includes provide the complete DYNARY type.

The L2 spec introduction, section 2, corpus caveat, and ledger row 13 were inspected. Their native basis is retained static-analysis annotation for 0x140c65830: flag, zero-count setup request 0x16, defaults, unused cast, and unchecked SC. This review does not claim fresh assembly inspection, original HTFurnace corpus coverage, runtime equivalence, arbitrary reentry protection, owner pinning, closure of other aliases, or completion of the Stopping barrier.

Reviewed SHA-256 values match the implementation receipt:

- loader_l2.cpp: b82fdbc181ad55d25d74a5ded03d8876619676b86fda3428eb509e522ea31b62
- test_loader_l2.cpp: 34e4d9b1568fb5211067259a982ba66ea20871bbb402979e474e7d84166f39b2

Static review only. Focused and full runtime checks remain with integration. This pass wrote only this report; it made no source/test changes and ran no build, test, GUI/app-control, REA, database, executable, or deployment operation.
