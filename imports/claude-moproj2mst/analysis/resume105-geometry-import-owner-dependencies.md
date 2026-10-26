# CP105 — Geometry import owner and payload boundary

## Bounded result

After `GeoObject` is converted and stored at `0x1409116b3`, the static reader continues with scalar/QString extraction, not an identified file open. `MorphMethod` is read at `0x1409116f1`; subsequent fields include `UpperGeoFile` (`0x1409117a3`), `LowerGeoFile` (`0x1409118e7`), and `NominalGeoFile` (`0x140911a2b`). For the two visible upper/lower fields, the code combines the attribute with the earlier QFileInfo path local via the internal helper at `0x1400048f0` (`0x140911862`, `0x14091189f`, `0x1409119a6`, `0x140911aea`). These are path-string construction calls; no QFile/QFileDevice/QIODevice constructor or open call is present in this bounded extraction.

At `0x140911f99`, the reader calls internal `0x1408175c0` with `this` from `[rsp+0xa8]`, GeoOperation from `[rsp+0x210]`, GeoObject from `[rsp+0x21c]`, and a QString pointer in `r9`; two further QString-like arguments are placed in the outgoing stack slots at `[rsp+0x20]` and `[rsp+0x28]`. This is the earliest visible call after the attributes that plausibly binds the extracted geometry data to an owner object. The callee is not included in the retained `resume50` bounded reader capture, so its identity/field effects cannot be asserted. Follow-up calls on the same receiver at `0x140817840`, `0x140817f40`, `0x140817fa0`, and `0x140817890` pass an index and flags/value, consistent with further receiver mutation, but do not prove allocation, ownership, or file loading.

After these calls, `0x14091201c` begins iterating the Geometry element's child nodes. That is the next place to seek payload decoding or file access. The retained evidence inspected here does not include an identified QFile/QIODevice open/read call nor a recovered concrete owner type/destructor relation.

## What this establishes for the candidate

The native path is richer than scalar metadata: it computes the QFileInfo-derived directory, combines it with geometry filename attributes, and submits geometry fields to receiver `[rsp+0xa8]` through `0x1408175c0`, then supplies indexed metadata and child traversal. But the current candidate lacks the native input-path object and does not have a source-qualified concrete geometry owner to receive these values. It is not safe to advance the import boundary based only on the attribute names or the setter-shaped callsite.

## Exact next dependency

Recover `0x1408175c0` and the adjacent receiver methods (`0x140817840`, `0x140817f40`, `0x140817fa0`, `0x140817890`) from immutable original PE, including receiver construction/type and resulting storage ownership. Then inspect the child loop from `0x14091201c` through its first file-related call to establish payload consumer semantics. Until both edges are closed, Geometry import remains unsupported at the pre-QFileInfo filename/path boundary.

## Evidence limits

- Source: `analysis/resume50-doe-variable-body-pe-annotated.asm`, addresses `0x1409116b3..0x14091201c`.
- Reader and caller provenance: `analysis/resume104-doe-geometry-entry-plan.md` and `analysis/resume104-geometry-child-after-fileinfo.md`.
- No runtime execution or mutable REA query was performed. The setters' semantics, receiver concrete type, allocation, ownership, and actual payload open/read are unresolved.
