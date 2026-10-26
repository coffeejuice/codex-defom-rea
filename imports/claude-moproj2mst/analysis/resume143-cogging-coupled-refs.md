# CP143: Explicit Cogging coupled-reference helper

## Change

Added a private helper pair in `source/apps/moproj2mst/src/engine/cogging_coupled_refs.hpp/.cpp`:

- `applyCoupledObject5Reference(object, coupled)` writes `int32 4` at `+0x22c0` and `double 1.0` at `+0x22c8` only when `object != nullptr && coupled`.
- `applyCoupledObject7Reference(object, coupled)` does the same with id `6`.
- False and null inputs are no-ops; values already present in the object remain untouched.

The functions accept only the caller-qualified existence boolean. They do not access files, project paths, or current directory. The native call-site order remains as CP142 specifies: object 5 helper after copy/inter-object/rotate and before its Router; object 7 helper after copy/inter-object/translate/direction/rotate and before its Router. This isolated helper is not yet wired into `cogging_save.cpp`, so it does not change public Save admission or claim end-to-end coupled behavior.

Added `source/apps/moproj2mst/tests/mdt/test_cogging_coupled_refs.cpp`, which seeds sentinel values, verifies false preserves them, verifies true writes the object-specific id and 1.0, and calls both helpers with null. `CMakeLists.txt` already discovers `src/*.cpp` and `tests/*/test_*.cpp` through `CONFIGURE_DEPENDS`; no CMake change was made.

## Evidence and validation

The offsets and constants are from `source/docs/re/moproj2mst/mdt/L1/spec_L1B.md` §5 (native assignments at the object-5 and object-7 points), also summarized in `analysis/resume142-cogging-private-next.md`. `LLOBJ_DATA` is a `0x3f28`-byte object, so both fields lie inside its recovered layout.

No build or test was run. No GUI or mutable REA state was used. Public Save guard and existing Cogging source were not changed.
