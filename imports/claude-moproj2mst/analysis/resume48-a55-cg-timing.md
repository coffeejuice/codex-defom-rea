# Resume 48 A55 CG toggle timing

## Finding

Retained original static call-chain evidence places the CG toggle in the active Forming `SfPreSimControl300` editor's initial Open/constructor fill. It does not establish that this UI sequence completed for the historical A55 Save, nor that it is observed in a fresh original session. The reconstructed normal session path currently reaches the editor factory but stops at an earlier palette-input guard, before widget setup or iteration fill. Its isolated legacy projection directly calls the CG normalizer, but that is not the session-bound lifecycle path.

Therefore the answer for a fresh one-project Open → Save on today's reconstructed candidate is: **the CG slot is not yet demonstrated as reached**. The answer from the retained original static chain is conditional: **if the active Forming editor300 constructor and iteration fill proceed, and Task00002 has solver 2, the fill sets the CG radio checked, which synchronously reaches the auto-connected slot**. There is no fresh original runtime observation for this case.

## Original chain and preconditions

Retained `analysis/wave2-save-diffs-original-iteration-fill.asm` shows `0x140ec19f0` reading the active sim-control solver field (`SC+0x2e4`); when it equals 2, it obtains the CG radio at editor offset `+0xa90` and calls Qt `setChecked(true)` at `0x140ec21b6`. Original UI setup at `0x140f21d67` calls `connectSlotsByName`. The retained metadata identity maps `SfPreSimControl300` method 64, `on_m_rbtnDefSolCG_toggled(bool)`, through the meta dispatch to `0x140ed01b0`.

Thus static reachability requires all of these conditions: the selected operation is the active Forming operation; the editor300 exists and has completed setup/auto-connection; iteration fill is reached; and the loaded solver value is 2. With enabled=true, `0x140ed01b0` applies the source-qualified parameter rules documented in `analysis/wave2-save-diffs-diagnosis.md`. Setting the radio false is display-only, so the relevant transition is specifically to checked/true. This chain is static disassembly/metadata evidence, not a live trace.

The historical A55 input `RS_55_Multiple_Operations_Example1_before/.../Operations/Task00002/SimCtrl.KEY` has `SOLMTD 2 4` and four zero parameters. Its retained historical Save changes the second and fourth parameters to one. That is compatible with the conditional chain, but the pair carries old-session history and is not a fresh original observation. The original reader itself preserves the input fields. Do not infer that the historical output proves a fresh-session call or state.

## Current reconstruction path

The current normal Open path selects the active operation in `source/apps/moproj2mst/src/engine/loader_l3.cpp::formingSimControl300Open`. When a real session/model manager is bound, it begins the editor300 factory and immediately preserves guard `0x6504abb0` for unresolved `QWidgetPrivate` palette inputs. Its source comment explicitly says native allocation/setup and General/CG fill have not occurred. Consequently the real lifecycle path returns unsupported before `SfPreSimControl300` UI setup, auto-connect, iteration fill, or the CG toggle.

In contrast, the function's explicit legacy branch for an unbound manager calls `simcontrol300::normalizeCgAtOpen` directly. That branch is a compatibility projection for isolated fixture/MST tests; it does not establish the real session-owned UI timing. Current `simcontrol300::fillIteration3D` in `source/apps/moproj2mst/src/engine/simcontrol300.cpp` invokes the same normalization before constructing the iteration page. Internal page snapshot/lifecycle tests verify that bounded projection, but they do not prove native UI callback dispatch or the public session factory completing setup.

The retained original top-level constructor sequence names `0x140eb26f0 → 0x140eb5630 → 0x140eb80c0(page0) → 0x140ec19f0`. Current `simcontrol300_lifecycle.cpp` models page order including Iteration, while loader/editor factory guards mean this page model is not yet reached through the selected session-bound editor300 factory. The A55 difference itself names Task00002; it does not independently establish that it is selected active at the exact fill point in a fresh session. That active-selection precondition must be tied to the original Open path or observed in a fresh run.

## Next smallest action

Do not add another unconditional normalizer or relax the palette guard. Continue the existing editor300 dependency by resolving the minimal palette/style inputs needed to cross `0x6504abb0`, then recover only enough actual parentless editor construction/setup and auto-connect ordering to reach the constructor's initial page0 Iteration fill. Bind the real editor owner and current operation; test that solver2's checked transition applies the CG rule once, and solver values other than 2 do not. Preserve unresolved gradients/styles and other widget guards.

Before claiming the historical A55 bytes are the fresh behavior, obtain a bounded original-first observation of a fresh copy of A55 (or a held-out Forming fixture with solver2): capture selected active operation, initial SimCtrl parameters, constructor page order, whether true-toggle dispatch occurs, post-Open parameters, and subsequent Save bytes. If the original GUI still fails before that observation, retain the failure and use a different fixture only when it exercises the same qualified chain. After the implementation boundary is integrated, rerun the focused test and the full frozen-candidate regression, then compare exact A55 Task00002 bytes and verify the inactive Task00001 remains unchanged.

## Evidence inspected and limits

- `analysis/wave2-save-diffs-original-iteration-fill.asm`, `wave2-save-diffs-original-ui-fill-dispatch.asm`, `wave2-save-diffs-original-meta-dispatch.asm`, `wave2-save-diffs-original-slot-identity.json`, and `wave2-save-diffs-diagnosis.json`.
- `analysis/wave2-save-diffs-diagnosis.md` and `wave2-save-diffs-reference-inspection.json`.
- `analysis/resume47-editor300.json` (source frozen; focused build pending; no original capture).
- `source/apps/moproj2mst/src/engine/loader_l3.cpp`, `simcontrol300.cpp`, `simcontrol300_lifecycle.cpp`, and `editor_setup.cpp`.
- `source/apps/moproj2mst/tests/mdt/test_simcontrol300_snapshot.cpp` and `test_simcontrol300_lifecycle.cpp` for bounded internal page semantics.

No source, build, original application, or mutable REA database was changed or executed. No fresh original trace was performed.
