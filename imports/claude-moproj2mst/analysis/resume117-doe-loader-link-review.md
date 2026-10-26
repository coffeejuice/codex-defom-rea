# CP117 DOE loader link review

## Finding

The stored-path-to-GEO-read chain from `analysis/resume116-doe-geometry-path-consumer.md` is internally consistent through the first CRT open and line reads. The unresolved edge remains invocation of main-window method `0x140032800` from the selected Open→Save workflow. The current source has a DOE Save emitter and a geometry-variable owner candidate, but it does not yet provide the native manager selection and operation/object mapping consumed by `0x1400d4560`. There is no source-closed loader branch to integrate into selected Save today.

## Consumer chain and first file effects

The retained native chain is:

1. `0x140032800` calls `0x1400d4560` at `0x140032afe`.
2. `0x1400d4560` requests its geometry-variable set through a manager virtual at `+0x1b0` (`0x1400d465c`), then obtains each Var's row-zero upper/lower stored path through `0x140817940` (`0x1400d4720`).
3. It converts upper QString with `toLocal8Bit` (`0x1400d48e2`), obtains `QByteArray::constData` (`0x1400d4908`), and calls format-2 loader `0x1408ee540` (`0x1400d4926`).
4. Format 2 calls parser `0x1408f7010`; before the parser's own open, its probe `0x1408f83a0` makes the first file attempt via `0x14146fcd8(filename, "rt")` at `0x1408f83f5`. A nonnull stream is read twice with `0x1414733ac(buffer, 2000, stream)` at `0x1408f8426` and `0x1408f843d`, then closed. A false probe prevents the parser's second open. A passing probe is followed by another `"rt"` open at `0x1408f709d` and subsequent parser reads.

These are concrete file effects on the stored upper path, rather than a QFileInfo check or path-only calculation. The lower path is attempted later only if the upper format-2 wrapper returns zero. The receipt does not establish that either file opened in a retained run, and the chain's presence does not establish that normal Save invokes it.

## Why the Save link is still open

The CP116 report identifies only bounded direct callers: `0x140032800` and a separate `0x140033cf0` path. Qt meta-dispatch cases `0x22..0x24` and `0x25..0x26` suggest how those methods may be called, but do not establish a live connection, receiver lifetime, or occurrence on normal Save / `DoeTableRefresh` return. The report also explicitly says the rel32 locator is not an exhaustive indirect-call or Qt-connection proof.

Current source confirms the missing owner inputs:

- `src/doe/doe_study.cpp::Study::geometryVars(int,int)` refuses whenever `vars` is nonempty because the native `0x14091ba60` operation/object mapping is unrecovered. It returns an empty result only for an empty study.
- `src/doe/geometry_variable_binding.cpp::bindB122GeometryVariablePrefix` can retain one fixture-qualified `Var` and its geometry path metadata, but populates only `ops` from the retained record and requires a fresh empty Study. It does not establish the native manager's selected geometry-variable set or bind the variable to a runtime operation/object pair.
- `src/mst/emit_b_doe.cpp::emitDoeGeometry` does run from operation Save emitters, but calls `geometryVars(opId, id)`. The current refusal prevents unsupported variables from being treated as correctly selected geometry variables. Its `emitDoeGeometryBlock` then projects filenames into MST and deliberately omits the `.COR` writer. Neither behavior is the native GEO loader chain.
- `src/doe/activation.cpp::readLazyStudy` retains the input-table path while parsing and then loads the sample files; that path is the XML source context. It does not dispatch the stored Geometry files through `0x1400d4560`'s equivalent, and its source context is not a manager/Var/object selection.

Consequently, routing from the existing Save emitter to a GEO reader would invent both call reachability and the missing `geometryVars(opId,id)` selection. Running the loader directly at XML Geometry entry would also reorder a downstream effect before later reader stages, unsupported branches, and Save selection.

## Smallest source-closed next step

No loader integration is justified by the present receipts. The smallest useful next evidence edge is the invocation into `0x140032800` (and its receiver/arguments) plus a closed caller or Qt connection path showing whether normal Open→Save or the actual `SfMoDoeTable::slotUpdateUI` return invokes it. If the selected Save graph reaches it, the next source contract must bind the same selected manager, Var, operation/object identity and local-byte path before porting the loader effects.

Until that edge is established, the only bounded source seam is to keep the current separated capabilities: preserve `GeometryPathValues` as a path-value projection; preserve `HeaderVariableRecord`/`GeometryVariable` as retained record state; keep `Study::geometryVars` refusing populated studies. This is not a loader feature, but it is the narrowest already-owning path that does not fabricate native selection or file effects.

## Evidence boundary

This review is read-only. It checked the CP116 report against current `activation.cpp`, `doe_study.cpp`, `geometry_variable_binding.cpp`, `mst/emit_b_doe.cpp`, and Save dispatch call sites. It did not inspect a new original runtime capture, mutate REA state, run the GUI, edit source, or build. The conclusion is limited to the retained static evidence and current source snapshot; it does not claim that an indirect native call is impossible.
