# CP107 — Geometry receiver lifetime and filename-consumer boundary

## Receiver provenance

The native Geometry reader does not construct the receiver used by `0x1408175c0` or the two correspondence methods. In the retained body, `[rsp+0xa8]` is set at `0x14090f0f5` from the return value of an indirect virtual call through the object at `[rsp+0x1030]`, vtable slot `+0x1a0` (`0x14090f0e0..0x14090f0f5`). A preceding virtual call on that same object uses slot `+0x178` to obtain a count, subtracts one, and passes that index to another slot `+0x1a0` call (`0x14090f0c5..0x14090f0f5`). This establishes that the receiver is supplied by an external runtime object/interface, not allocated by the Geometry reader.

The reader then mutates this returned receiver through `0x1408175c0` (which appends a separately allocated record to receiver `+0x28` on a key miss) and through `0x140818000` / `0x140818080` (correspondence point updates). The reader's exit cleanup at `0x140912759..0x1409127ac` destroys local QString/QFileInfo wrappers; normal return at `0x140912a0e` does not delete `[rsp+0xa8]`. No receiver destructor or `+0x28` list teardown is present in this reader body.

## Unresolved owner edge

The exact missing edge is the concrete dynamic type and lifetime of the object in `[rsp+0x1030]`, especially the implementation of its virtual slot `+0x1a0` at `0x14090f0ef`. Without the caller's concrete vtable/value (or a static caller that fixes it), the returned receiver cannot be tied to a constructor or destructor. The record initializer `0x1408178790` and list insertion seen in `0x1408175c0` establish child-record allocation/insertion but not ownership of the receiver or deletion of those records.

## Filename consumer

In this Geometry reader body, `UpperGeoFile`, `LowerGeoFile`, and `NominalGeoFile` are extracted and combined with the QFileInfo path local. The child loop processes Upper/Lower `PointPair` data as doubles and calls the receiver's point-update methods. No identified QFile/QIODevice open/read follows in the retained body. The static captures inspected do not establish a later consumer of the stored filenames; such a consumer may be in another method reached through the unresolved receiver type.

## Smallest next dependency

Recover a concrete caller of the readInputTable/Geometry reader that supplies `[rsp+0x1030]` and its vtable, then resolve slot `+0x1a0` and its matching receiver teardown. This is the minimum evidence needed to name the receiver constructor/destructor and trace the stored filenames to a later consumer. If the caller does not statically bind the vtable, the next dependency is a source-qualified runtime instance of that interface; do not infer ownership from `0x1408175c0` alone.

## Evidence qualification

- Original binary SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- `analysis/resume50-doe-variable-body-pe-annotated.asm`: receiver returned at `0x14090f0ef..0x14090f0f5`; receiver updates at `0x140911f99`, `0x1409122fe`, `0x14091264c`; local cleanup and return at `0x140912759..0x140912a10`.
- `analysis/resume105-geometry-receiver-methods.md`: bounded insertion method evidence.
- `analysis/resume106-geometry-first-file-effect.md`: child-loop point parsing and no identified file open/read in the captured reader.
- Static evidence only. No runtime execution or mutable REA query; no claim that the filenames have no later consumer outside this function.
