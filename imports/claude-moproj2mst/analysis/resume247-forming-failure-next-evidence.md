# CP247 Forming failure next evidence (revised probe boundary)

## Bounded findings

**Initial KEY writer status.** The retained original L3 decompile `evidence/gui_mo/s1/mdt/L3/d0.c` (function `0x140c06130`, lines 84–161; SHA-256 `6D3703619843FB9AE1760F91E6633FB982C35CDC39D9DD2D89BEE7435477FBFF`) calls `0x140358190(step, object, tempPath, 1)` and discards its return, then calls manager add. `source/docs/re/moproj2mst/mdt/L1/spec_L1A.md` §3.5 qualifies the GUI wrapper: it normalizes/appends `.KEY`, skips the overwrite prompt when `force=1`, invokes `0x140896840`, and returns `true`. The engine writer body returns status 3 when its `fopen` fails, but that status does not escape the GUI wrapper. This body status is statically established; it is not observable through the original caller's wrapper return, and this report does not claim a runtime measurement of it.

**Initial source lifetime/name.** The first source is the fixed `<QDir::tempPath()>/Equipment (N).KEY`, where N is equipment-row count + 1; it is not a `QTemporaryFile`. d0 constructs a `QFile`, checks existence and attempts removal if present, calls the wrapper, calls manager add, then attempts removal of the same path. Return values from these filesystem calls are discarded. L3 spec §3 and L1 spec §3.5 corroborate the path and force flag.

The retained QTemporaryFile evidence concerns the **Qt copy fallback** destination, not this initial equipment source. `analysis/wave3-equipment-temp-rename-contract.json` binds its constructor/destructor auto-remove and rename/stream fallback behavior to private-Qt decompile receipts, static-only. Keep those generated fallback temporaries distinct from the fixed `Equipment (N).KEY` source.

**After add returns -1.** In d0, manager add's integer result is stored with no positive-ID branch. The code then attempts source removal, copies the equipment name into movement bytes, calls manager `vt+0x208` with the returned ID including -1, stores that ID in slot and operation, reacquires the step, and—if non-null—calls `vt+0x208` again and attempts the second `0x140358190` write. The second wrapper return is also ignored. This establishes control-flow continuation and order, not the concrete `vt+0x208(-1)` path or its filesystem result.

## Smallest remaining evidence step

A fresh owned-original failure occurrence may be useful only if a safe, isolated trigger can make creation/open of the fixed first source fail while keeping loader preconditions valid. Capture only observable runtime facts: whether the fixed source exists after the attempt; whether manager add is reached and returns -1; the two `vt+0x208` results; whether the two ID stores occur; whether the second writer wrapper is called; and final temp/path existence. Do **not** ask the GUI-level probe to report the inner writer's status 3; the wrapper returns true and hides it. No supported instrumentation method for observing that internal status is established in these retained records, so do not instrument or claim it measured. If no isolated failure trigger exists, the smallest next step is to retain the failure filesystem outcome as unresolved; static control flow is already established.

Any fresh occurrence remains subject to the established owned-process and GUI-control release gate; none was run here. The existing manager/copy and QTemporaryFile fallback audits are consumed rather than repeated.

## Provenance

Original GUI identity retained in CP246: DEFORM v11.0.2, `evidence/gui_mo/DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`; d2.c SHA-256 `CF3F99AF220CB92EC9CE8A19328640113333DF4C0194FAAC2B20B475BA2C6F57`. L1/L3/Packet-S documentation and decomp are static qualifications, not fresh runtime evidence. No manager/copy audit was repeated; no source, test, build, shared state, original, GUI, or mutable REA artifact was changed or run.
