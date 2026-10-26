# CP101: active DOE refresh receiver and Save return witness

## Finding

The retained native evidence closes the static receiver chain but does not close the public Save branch. It identifies a main-window-cached `SfMoDoeTable` (`+0x458`), constructor-time registration for `signalDoeTableRefresh()` → `slotUpdateUI()`, and the slot's timer-stop then refresh call order. It does not establish a live receiver, dispatch outcome, or normal callback return for any particular Save occurrence. Nor does it bind the refresh's current-Project manager to the manager cached by that Save arm. The public candidate must remain guarded.

## Static chain and runtime gap

Against original GUI PE SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, retained bounded captures establish:

- Lazy getter `0x1400196d0` checks main-window `+0x458`, constructs on a miss, and publishes only after constructor return. The dock uses `+0x460`.
- Table constructor `0x1400eb6f0` creates child timer at table `+0xb0`, completes setup and initial UI refresh, then registers this receiver at `0x1400ebc5c` for key `2signalDoeTableRefresh()` and slot `1slotUpdateUI()`.
- Static meta-dispatch reaches `slotUpdateUI()` at `0x1400f4e10`. It stops timer `+0xb0` at `0x1400f4e25`, then calls the table virtual refresh at `+0x1b8` (`0x1400f47e0` in the primary vtable). Refresh uses then-current Project/active-SUB context and has a normal tail that clears its in-progress field.
- Save's Type2 arm caches one manager from the selected SUB's `+0x20`; after the refresh dispatch returns, it uses that cached manager for current PAR and modes 0, 1, 2. The table refresh resolves its own current context independently.

These are static code/registration facts. They do not show that, at a particular Save, main-window `+0x458` contains a fully constructed live table, the global dispatcher is not suppressed, a matching registration is present and live, the direct slot enters and returns normally, timer/refresh calls complete, or refreshed `table+0x50` equals the cached Save manager. Static registration alone cannot certify any of those runtime values or events.

## Smallest candidate source seam

`source/apps/moproj2mst/src/moproj/write.cpp:146-160` is the first candidate Save continuation after the XML writer's normal-return observation. It journals the refresh boundary at line 155, obtains the Session-owned table state at 156, and calls `requireRefreshDelivery` at 159. `source/apps/moproj2mst/src/doe/table_owner_state.cpp:232-236` verifies Session/field projection and then refuses because timer/UI receiver delivery is unsupported. There is no later public current-PAR/sample continuation reachable today.

The smallest safe next seam is an occurrence-scoped refresh receipt at this boundary, backed by a real source-owned receiver/dispatch abstraction. The receipt must bind the selected SUB and strong Study/I/O/manager owner before XML, identify the actual `+0x458` receiver generation and live registration, distinguish suppression/no target from direct delivery, and record timer-stop plus slot normal return. It must also verify the receiver's post-refresh manager is the cached Save manager before continuing with that manager. Preserve ordinary XML false-result behavior: native still dispatches refresh after an ordinary false writer result. Without this source-owned receipt, a fabricated callback-return flag or detached helper would not establish public reachability.

## Evidence and limits

Reviewed retained receipts:

- `analysis/resume65-doe-refresh-owner.md` (SHA-256 `34d23e77aee1693b151479f089fb2baf7c1a1484004cb5f773036ad74bd53855`), with getter/slot bounded captures `analysis/resume65-doe-refresh-getter.asm` and `analysis/resume65-doe-refresh-slot.asm`.
- `analysis/resume66-doe-refresh-ui.md` (SHA-256 `0975503f6ce1b52a47e072ae052725b236328127cd5f6ce01ed2a4ec42ad17b3`).
- `analysis/resume67-doe-table-owner-lifecycle.md` (SHA-256 `cfb8ee57da5cb32e9081fa3f477179f3f8abf486987f2d6ae6f3cd5ad6bb4c76`) and `analysis/resume68-doe-table-owner-state.md` (SHA-256 `5cb50b1f6d30e3d92e5c60a51729044c9a90a94acba3a07c6e45a5c753951d9d`).
- `analysis/resume90-doe-save-refresh-binding.md` (SHA-256 `2d5e78758f7ed5928423b462d3f44c41713ee1e2dcaa2b6e10c22c0426b36f42`) and `analysis/resume98-doe-refresh-return-blocker.md` (SHA-256 `dfc34153ff9488ccbe7ac254c28ccdd6a8ed62eff1a595390dc65f56166f864c`).

Current source hashes reviewed: `write.cpp` `8af4be80324c39e1f8f452d7723bebd200e4b7372180d8e32c1e6ee32c2399a2`; `table_owner_state.cpp` `d57b8a6ba6f6dac0d7d9efb515b73a6dafbc10ba4213e5681c37744faf5fcfcf`. This was retained static/source review only. No REA session/database, GUI, original runtime, source edit, or build was used.
