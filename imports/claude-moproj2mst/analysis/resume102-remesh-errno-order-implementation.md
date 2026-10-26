# CP102 finite-real errno preservation source freeze

Successful ordinary real conversion now restores its incoming errno instead of
publishing the host range-detection clear. A newly raised current-call ERANGE
still reaches the existing guard before restoration. This fixes ordering across
an earlier integer overflow and later finite fields without admitting any new
numeric value/profile or changing model outputs/adapter statuses.

Evidence was frozen before source work:
`analysis/resume102-remesh-errno-order-contract.md`, SHA-256
`e00d891bf18282ebbc13b97b1495bef26d1d3caf10bd3e81f7270710046c7852`.
It pins native wrapper entry/return/error stores and the scoped actual reader
counterexample. CP101 archive release and exact pair approval preceded edits.

## Exact change and five cases

Only ordinary `originalReal()` fallback changed: save prior errno, clear for
host strtod range detection, retain the same finite/current-ERANGE check, then
restore prior errno only on accepted return. The canonical range and prefix
branches return earlier and are untouched. No parsing/profile, field/cursor,
close, Save, public constructor, API/header or CMake change was made.

New `test_simcontrol300_remesh_errno_order.cpp` exercises:

1. Entry0/EDOM/ERANGE persists through multiple finite scalar and method records;
   prior ERANGE must not be mistaken for a current conversion error.
2. Integer overflow sets ERANGE, then filter7.5 and scaling2.25 apply without
   erasing it; loaded/enabled and method5 are retained.
3. The already-admitted no-digits0 path preserves prior errno, including its
   existing scaling lower fallback1.
4. A new unsupported fractional overflow/underflow overwrites prior EDOM with
   ERANGE and remains refused before the filter store, preserving its method
   prefix and no enabled assignment. Restoration cannot mask that failure.
5. Actual Session `loadProjectRemesh()` uses its SAME remesh owner; a negative
   integer overflow followed by finite fields preserves ERANGE at memory-file
   close and immediate dispatcher return. This is not a claim about errno after
   unrelated project/GUI/outer API operations, and no final M2M mapping is inferred.

Memory close records errno before its own effects. No disk/original/user files
are used by this focused target. No existing fixture required amendment.

## Preflight and release

Diff against CP101's source archive is the save/restore and explanatory comments
around the unchanged host call/guard. Includes resolve; Testkit assertions are
supported; delimiters balance; one Testkit main is present; no C assert or
trailing whitespace occurs. Existing remesh tests contain no success-clears-
errno expectation that needs changing. No new include/registration is needed.

New target `m2m_mdt_simcontrol300_remesh_errno_order`, CTest
`m2m.mdt.simcontrol300_remesh_errno_order`, has5 cases. No worker build,
executable test, CTest, GUI/original execution or mutable REA occurred.

Frozen files relative to `source/apps/moproj2mst/`:

| File | SHA-256 |
|---|---|
| `src/engine/simcontrol300_remesh.cpp` | `39249f84faa6bef97e53504b7fb22f632a539d42cc7b5270a7bd3c553321814f` |
| `tests/mdt/test_simcontrol300_remesh_errno_order.cpp` | `0188313ffdf40a6d5e8a7f3ab3ffa8559a3c8b5961f735d0e299ecfb48e92acc` |

Both are released/frozen for independent source review and root integration.
Root owns build/archive; the reviewer has the exact hashes/contract. Source
review and execution are pending at receipt write time. Scope remains the
normal existing C-runtime value/errno adapter; failure/TLS/FP-environment and
selected editor constructor qualifications are unchanged.
