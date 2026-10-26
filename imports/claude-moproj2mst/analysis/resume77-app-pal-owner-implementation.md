# CP77 — isolated `app_pal` owner state

Implemented the CP76 owner/lifecycle contract as a standalone model. The new `QApplicationPaletteOwnerState` distinguishes `Unknown`, `KnownNull`, and `KnownPresent` for the global `app_pal` wrapper. A read while unknown returns pending without calling the constructor provider. A read from a qualified null slot requests the native global-color argument `2`; on a valid returned `WidgetPaletteValue`, it records wrapper allocation, global-color construction, slot cache, and output copy exactly once. Repeated reads copy from the retained wrapper owner. A qualified present-slot observation binds a wrapper/value and reads copy without invoking initialization.

The wrapper retains the palette value's existing shared data owner and local group/resolve word. Conflicting or unqualified observations are rejected. The model is deliberately standalone: no `WidgetRuntimeState`, `ConversionSession`, SplitView, or production callsite was modified. No original `app_pal` state was inferred from probe002.

## Files

- `source/apps/moproj2mst/include/m2m/engine/application_palette_owner.hpp`
- `source/apps/moproj2mst/src/engine/application_palette_owner.cpp`
- `source/apps/moproj2mst/tests/mdt/test_application_palette_owner.cpp`

The converter CMake already recursively globs engine `.cpp` files and uses `CONFIGURE_DEPENDS` for `tests/*/test_*.cpp`, so this addition needs no CMake edit. The focused test has a `testkit::run_all()` entry point, includes resolve through the target's `include` directory, uses only supported `CHECK`/`REQUIRE` macros, and contains no C `assert`.

## Focused test coverage

`test_application_palette_owner.cpp` covers unknown-slot non-initialization, one-time lazy construction/cache with argument `2`, repeated-copy owner identity, a known-present copy with no initialization, and rejection of missing/conflicting qualifications. It seeds an explicit palette data alias and nonzero group/resolve word so the copy test detects accidental substitution or clearing. These are state-model tests; they do not claim native execution or exercise SplitView integration.

## Validation boundary

Static preflight only: reviewed the Testkit macros and include path, confirmed the CMake source/test glob rules, checked the new test file for a single `main`/`testkit::run_all()` entry point, and inspected the three-file change set. Build and CTest were intentionally not run per assignment; root owns them. No GUI or mutable REA interaction occurred.

## SHA-256

Record these hashes after freeze review:

- Header: `ac6badf7155d8c1c7b1cfd816719e3685d5b3e65dbb2731f51f41a346b858e58`
- Implementation: `58d6355b88150698b0af87128e8994f3595fa2c4d0a8db995aec1875d7851348`
- Focused test: `58b3c0d7e8831dd10023cdc8bdf057be5283c794469c306edb097108b1119e05`

