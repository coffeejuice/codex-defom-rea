# CP216 operation project directory collector

## Scope

This change adds an internal, pure collector for the five project-level
directory decisions associated with the qualifying `setOperationId` branch.
It is not connected to `setOperationId`, Open, `FileEffects::mkdirs`, or any
filesystem writer. It does not establish the cause of the CP213
`Lubricants` observation.

## Evidence and path derivation

The retained `analysis/wave3-earlier-save-14008c630.asm` shows five sequential
helper results passed to `0x140081afb0`. The retained source qualification in
`source/docs/re/moproj2mst/s1-01/spec.md` identifies the corresponding
subdirectories, in order, as `Operations`, `Objects`, `Materials`,
`Equipments`, and `Lubricants`. The operation spec records the `id >= 1`
branch in `0x140050820` (`spec.md:315-318`).

The helper accepts the already-resolved project directory. Callers should
derive it through `projectDir(p)`, which is `cleanPath(absolutePath(p.path))`
in `src/model/project.cpp`. Each child is built with the same
`projectDir + separator + child` then `qt::cleanPath` construction used by
`src/moproj/read.cpp:listDir`. The collector appends exactly five paths for
IDs at least one; IDs below one append none. Repeated calls retain order and
multiplicity in the caller-owned vector.

## Validation

`tests/model/test_operation_project_directories.cpp` uses Testkit's
`TEST_CASE`, `REQUIRE`, and `CHECK_EQ` entry points. It checks ordering and
normalization, no-op behavior for ID zero, repeat-call multiplicity, and that
collection leaves the scratch directory entries unchanged. The test uses
`M2M_TEST_SCRATCH` for its filesystem fixture. No C `assert` is used.

The helper is header-only under `src/model`, and its quoted include resolves
relative to the test source. Full integration and grouped test execution are
left to the root coordinator as requested. A focused Release build was
attempted with `cmake --build build/baseline --target
m2m_model_operation_project_directories --config Release`; it stopped in
unrelated dependency compilation before reaching this target because the
host MSVC invocation could not resolve standard headers such as `cstddef` and
`array`. CMake also reported the build tree's test-source glob was stale.
