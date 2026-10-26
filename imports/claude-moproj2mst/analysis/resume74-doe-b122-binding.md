# CP74: isolated B122 geometry record binding

## Implementation

Added `bindB122GeometryVariablePrefix(Study&, const B122GeometryVariableRecord&)`. It accepts only the retained B122 record and a fresh Study with no Model variables or scalar/geometry prefix owners. It creates one Study-owned `Var`, initializes `name`, `number=varId`, and `index=-1`, maps the independent top-level operation binding into `Var::ops`, and copies the geometry metadata/path references into root-owned `Var::geometry`. It stores the owner in `Study::geometryPrefixVariableOwners` and then publishes the nonowning pointer in `Study::vars`.

Before creating/publishing the owner it validates the record's one-case values. A repeated binding or changed record throws `M2M_E_UNSUPPORTED` without changing the Study's logical values. Both destination vectors reserve capacity before either owner/pointer is appended, preventing a later capacity allocation failure from leaving one logical vector updated without the other.

The binder does not touch `Var::files`, geometry items, scalar fields, GEO files, `StudyIo`, `FileEffects`, directory state, public InputTable reading/writing, or conversion dispatch. `Geometry.GeoOperation` remains in `Var::geometry`; the separate top-level mapping is `Var::ops`. It does not implement or call `Study::geometryVars`.

## Focused test and static preflight

The new test reads the retained B122 InputTable fixture in binary/read-only mode, uses the CP73 pure parser, binds the result into a fresh Study, and checks owner/alias identity, exact geometry payload, empty file/item lists, separate operation list, `count`, `varAt`, `allVars` reindex, and `varsForOperation(1)` versus operation 2. A second test verifies duplicate and altered-record refusal without mutation.

Static preflight confirmed the test support include resolves, all owned files have final newlines and no trailing whitespace, and the helper/test do not call GEO I/O or the public reader/writer. No build, test, GUI, mutable REA, source shared with TLOC, CMake, or public dispatch was changed or run. Root owns the build/test decision.

## Owned files and SHA-256

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/geometry_variable_binding.hpp` | `ae999e952fdb1c2985b0b4b299bb309bcfb348304e386632100841291a708612` |
| `source/apps/moproj2mst/src/doe/geometry_variable_binding.cpp` | `3615f29ee6885323cf61f008e40137e526d20bab0a3d6df3e98145f06ddfb864` |
| `source/apps/moproj2mst/tests/doe/test_b122_geometry_variable_binding.cpp` | `d5fd711ace6ab0b73f9dc92c2d72e8b535fe0f7507991524c2ab95b4880aaa42` |

This is an isolated Study/Model prefix only. Public Type2 owner, nonempty `DoeVariables`/`Operations`, writer and geometry-query guards remain unchanged. No GEO ownership or generation, operation/object geometry selection, end-to-end B122 Open/Save, or native parity follows from the binding test.
