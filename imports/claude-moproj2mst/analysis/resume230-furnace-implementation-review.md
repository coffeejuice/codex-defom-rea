# CP230 furnace implementation review

**Result: PASS.** No source-contract defect found in the reviewed CP230 change.

## Review findings

- `GuiModel::initializeFurnacePostLoadDefaults()` calls `simCtl()` exactly once as its first operation. Its first mutation is `sc->furnace_on = 1`, followed by the curve count query. It calls `setup_curves()` only for a zero count, passing the address of a local `const std::int32_t` set to `0x16`, then calls `set_defaults()` after setup returns.
- The return from `simCtl()` remains unchecked. A missing `step` therefore retains the legacy unchecked null dereference; the command adds neither a no-op nor a checked error path. The geometry barrier remains first because `simCtl()` applies it before returning.
- `furnacePostLoad` retains `GuiModel* m = opModel(p, op); if (!m) return;`, invokes the command, then evaluates `(void)furnaceProps(c.propsOf(nullptr));`. It does not fetch `simCtl()` itself or move the property cast ahead of initialization.
- The control pointer is a local within the owner command and is not returned, stored, or retained. The native furnace calls remain scoped to that owner method.
- No C ABI or cache1 implementation change appears in the bounded diff. The public C++ model declaration is an ordinary `GuiModel` method; the loader slot and callback signature remain unchanged.
- The existing `furnace_postload_step_scope` regression is unchanged. It checks the empty-curve branch (furnace flag, 22 curves from setup, first curve shape) and populated-curve preservation across repeated post-load calls.

## Validation evidence

- `analysis/cp230-furnace-focus-build.json` records successful configure and Release build of `m2m_mdt_loader_l2`, with `source_stable_during_build: true` and matching before/after source-tree SHA-256 `bd48a72ac3e59496ae09897e27d39ab99ff7f2169757619fca5e34672e35b9ab`.
- `build/candidate/Testing/Temporary/LastTest.log` records `m2m.mdt.loader_l2` passed; all eight cases passed, including `furnace_postload_step_scope` (0 of 8 failed).
- The implementation report also records `git diff --check` passing. This review did not run a build or tests; it inspected the recorded artifacts and current source.

## Scope

This is a bounded source and receipt review. It does not establish original-runtime behavior or full lifecycle acceptance. No source, tests, build state, GUI, or REA state was modified.
