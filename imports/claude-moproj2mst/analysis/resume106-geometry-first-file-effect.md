# CP106 — Geometry child-loop file-effect boundary

## Result

The retained Geometry child loop does not show a file open/read. Starting at `0x14091201c`, it walks child nodes and dispatches by tag. For `UpperCorrespondence`, it looks for `PointPair` children (`0x140912142`), reads each element's text (`0x140912184`), splits the text on `", "` (`0x1409121cc`), converts four fields to doubles (`0x1409121f5`, `0x14091221b`, `0x140912241`, `0x140912267`), constructs two QPointF values (`0x140912290`, `0x1409122b0`), then calls `0x140818000` on the same receiver `[rsp+0xa8]` (`0x1409122f6..0x1409122fe`). The `LowerCorrespondence` branch repeats this shape and calls `0x140818080` (`0x1409124e1..0x140912264c`). Both are in-memory point-pair updates; the call targets' bodies are not part of this retained reader capture.

The loop's exits advance DOM siblings (`0x140912332..0x140912364` and `0x14091267e..0x1409126ac`). The overall reader later destroys local QFileInfo/QString/QDom wrappers (`0x140912759..0x1409127ac`) and returns (`0x140912a0e..0x140912a10`). Those are local cleanup, not destruction of the receiver or its `this+0x28` records.

The only QFile-related calls identified in this Geometry body are the QFileInfo constructor/path and QFileInfo destructor. The bounded reader region `0x140911532..0x140912a10` has no identified QFile/QIODevice open/read call. This is a negative result for that function body/capture only; it does not rule out that the stored filenames are consumed later by another routine.

## Ownership boundary

This closes a native child-data mutation edge: geometry filename metadata is stored by `0x1408175c0`, then correspondence point pairs are sent to the same receiver through `0x140818000` and `0x140818080`. It still does not identify the receiver's construction site/concrete class, its final destruction owner, or any later consumer that opens the stored geometry files. Do not implement actual payload import or assume the receiver lifetime from these calls alone.

## Exact next dependency

Search immutable PE for direct and indirect callers of the recovered receiver construction/site and `0x140818000`/`0x140818080`, then inspect the first later consumer of the stored filename fields. The receiver destructor/container teardown must be recovered separately to close lifetime. If no bounded xref binds these calls to a file consumer, retain refusal at the current pre-QFileInfo candidate boundary.

## Evidence

- Binary `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
- `analysis/resume50-doe-variable-body-pe-annotated.asm`: original reader instructions `0x14091201c..0x140912a10`.
- `analysis/resume105-geometry-receiver-methods.md`: recovered `0x1408175c0` insertion/update routine and unresolved receiver class/lifetime.
- Static disassembly only; no runtime execution, mutable REA, or file-payload inspection.
