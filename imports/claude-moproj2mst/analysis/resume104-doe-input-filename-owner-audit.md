# CP104 filename owner audit for DOE Geometry entry

## Finding

The native argument entering `0x14090ee20` is the exact `QString` filename passed to the outer InputTable reader and opened as its `QFile`. It is neither a `QFileInfo` nor a path recomputed from the project/study name. This gives the candidate a small, source-qualified input-identity seam; it does not give it native QFile/QFileInfo ownership or establish the imported GEO file path/effects.

## Native call and owner evidence

The immutable original GUI PE is SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. The bounded caller capture `analysis/resume104-doe-geometry-path-caller-14090e080.asm` is SHA-256 `429f7c5507b5833d0b9743df5355a7153322dfe4a35b60ce98c3453ad6f87ceb`; its receipt records the byte range through `0x14090ee10`.

At `0x14090e080`, `RCX` (the first argument) is saved at `[rsp+8]` and accessed as `[rsp+0x3a0]` after the `0x398` stack allocation. The caller passes that same value to the `QFile` constructor at `0x14090e0b9..0x14090e0c6`. In the `DoeVariables` arm, it reloads `[rsp+0x3a0]` at `0x14090e457`, copy-constructs a `QString` at `0x14090e467` (IAT `0x141e059d8`), and calls `0x14090ee20` at `0x14090e48d` with this copied filename as argument 1. This is static argument-flow evidence; it does not prove a runtime invocation.

The retained `0x14090ee20` body uses argument 1 as the filename source for a local `QFileInfo` at the Geometry arm (`0x140911548` constructor; `0x14091155f` path call). The local QFileInfo is created only after the exact direct `Geometry` tag branch. No evidence reviewed shows an already-created QFileInfo reference being passed into the helper. Earlier CP104 entry-plan notes identify this as the first geometry-specific side-effect/owner boundary.

## Candidate path seam and proposed context

`model/subproject.cpp::parseSubProject` calls `doe::readLazyStudy` with the owned Study and the `DoeStudies` directory. `doe/activation.cpp::readLazyStudy` computes the `Control/DoeInputTable.XML` `String` in local `input`, supplies it to `StudyIo::readFile(input, bytes)`, then calls `readInputTable(bytes, study)` and discards `input` at that boundary. `doe_study.hpp` currently exposes only the bytes-plus-Study reader; `loadInputTable` in `doe_study.cpp` likewise computes a path, reads bytes and drops the path before parsing.

The narrow internal value to propagate is an optional per-invocation source filename, e.g. `DoeInputTableSource { String fileName; }`, passed from `readLazyStudy` alongside the successful read into `readInputTable`, then only to a future direct-variable/Geometry child parser. Its value must be exactly the candidate's `input` String supplied to that `StudyIo::readFile` call. Keep direct bytes-only callers explicitly source-less; do not synthesize a path for them. A value-owned `String` context avoids dangling references and identifies the path text without claiming Qt data-block identity, refcounts, normalization, QFile, or QFileInfo state.

If one helper must accept both cases, make source presence explicit (optional source context) rather than an empty-string sentinel. Keep the current public `readInputTable(bytes, Study&)` behavior/API until a separate integration is approved; an internal overload or reader context can carry the identity without storing it on `Study` or in global state. No code change is part of this audit.

## Geometry import remains blocked

That context can let candidate Geometry parsing use the same XML-source filename argument as the native caller. It does not itself establish the resulting `QFileInfo::path()` string or native private/refcount/lease behavior, and it is not the geometry payload filename. The retained B122 `Geometry` row's three `.GEO` strings and payload bytes are fixture facts, not a general rule deriving them from the InputTable path. Native handling after the QFileInfo prefix still needs owner-bound evidence for `GeoOperation`, `GeoObject`, `MorphMethod`, file/correspondence attributes, the geometry record append, file existence/copy effects, and failure continuation.

Therefore the next implementable increment can propagate exact source filename text and reach an explicit guard immediately before constructing any candidate QFileInfo/geometry owner. It must not claim imported geometry, fabricate QFileInfo references, or use the B122 pure parser/binder as proof of the native import path. Geometry import should resume only after the exact path transformation/ownership and payload/effect branch are separately qualified.

## Scope

Read-only review of retained original static captures and current source. No source, test, build, GUI or mutable REA changes were made. The CP104 capture's static qualification and unresolved native QObject/Qt ownership caveats remain in force.
