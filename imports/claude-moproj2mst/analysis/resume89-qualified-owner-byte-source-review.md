# CP89 qualified owner synchronization-byte source review

## Result

The selected `u1b58 == 0` check is replaced by one bool-result `GuiModelAccess::withStep` callback at the same decision point (`viewer.cpp:546-570`). The callback reads only `nativeStep->u1b58`, returns the comparison, and does not retain or return a pointer. The immediately preceding checks establish canonical node-variable registration and coordinates, then qualify any element owner against the same live model step, and finally require `nodes->u130 == model->step.get()` with nonnull model and step. Therefore the callback's no-null-fallback dereference is supported by the already-established N-owner identity.

The guard order remains: node coordinate/canonical registry checks; actual E-owner check (if present); actual N-owner check; guarded byte read; then dimension and signed step-version decisions. The access layer's pending-state gate occurs immediately before the byte borrow. No native pointer escapes, and the only production change is this condition; other raw `model->step` reads in the function remain explicitly unconverted.

## COW, timer and barrier timing

The callback ends before `modelDim`, the high-version signed comparison, coordinate COW/getter calls, index-table selection, timer checkpoints, DB receipt checks, or pending-frame installation. For a nonzero byte, behavior still skips the high-version zero-byte prefix and falls through to the existing actual coordinate/connectivity getters. For zero at a qualifying high version, the original COW dispatch still occurs before tables/cache/DB checks, and a pending frame is installed only at the existing later DB continuation boundary. No read or guard was moved across those effects.

One existing classification detail remains: `modelDim` and subsequent qualified reads use the public raw `model->step` after the short borrow. The callback is synchronous and there is no supported callback or reentrant operation between the byte result and those reads in this path. This migration does not claim safety under concurrent external replacement or revoke legacy pre-held pointers.

## Regression coverage

`geometry_actual_node_nonzero_flag_bypasses_version_rejection` now crosses byte values `1`, `0x80`, and `0xff` with versions `6000` and `INT32_MAX`. It checks two expected getter dispatches, the actual step-derived dimensions, unchanged COW body identity/use count, scene result, coordinate/connectivity identities, and exact input byte/version after refresh (`test_viewer_box.cpp:1308-1327`). Existing neighboring cases retain zero-byte old-version guards, high-version COW/cache ordering, and parked retry behavior.

## ABI and scope

The change adds no field, header/API, native-layout, or export modification. Existing test compile-time assertions remain in place. The source report records that the selected condition is the sole replacement and the helper/header/public C ABI files are unchanged. Current hashes match the CP89 receipt: `viewer.cpp` `b715bb8fe08b7e6b9d91e2f027fa50a652848b1ce98a26f84a02b682593e0fd0`; `test_viewer_box.cpp` `58ddea703f0695e371d2c379ae18e9f070b0403732d9842a3377390b60510b98`.

No blocker found. This was read-only; no source/test edits, builds, or tests were made.
