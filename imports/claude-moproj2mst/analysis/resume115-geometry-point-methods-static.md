# CP115 native Geometry PointPair receiver methods

## Result

The retained original PE resolves the receiver-side shape and order for both point calls. `0x140818000` is the Upper method and `0x140818080` is the Lower method, as proven by the reader's direct calls at `resume50-doe-variable-body-pe-annotated.asm:3347-3350` and `:3554-3557`. Each method accepts a signed row index in EDX and two QPointF pointers in R8/R9. It returns without changing anything when the index is negative or is at/above the outer `Var+0x28` row count. For a valid index, it copies both 16-byte QPointF values into one contiguous 32-byte pair temporary, selects the GeoFile at that outer index, then appends the pair to that row's side-specific list: Upper at GeoFile+0x28, Lower at GeoFile+0x30.

The methods append one paired record per accepted `PointPair` callback; they do not replace list element zero. The reader passes EDX=0 for both methods, so only the first GeoFile row receives its Upper/Lower pair lists in this reader path. The methods themselves support any valid outer row index. An invalid row index is a silent no-op.

## Static evidence binding

The target is the retained `analysis/resume48-distortion-vt198-rea-session/DEF_GUI_MO_64.exe`, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, independently recomputed before extraction. The PE image base is `0x140000000`; both method byte ranges are in `.text` and map to file offsets `0x817400` (`0x140818000`) and `0x817480` (`0x140818080`). Bounded raw bytes are retained in `analysis/resume115-geometry-point-methods-bytes.txt`.

Both methods have the same control/data path except for the final side-list offset:

1. At `+0x19..+0x20`, `cmp dword ptr [rsp+0x58],-1` / `jle` returns for signed index below zero. The argument was saved from EDX at entry (`+0x0a..+0x0e`).
2. At `+0x21..+0x2c`, the method takes `this+0x28` and calls `0x140004ae0`; `closure-audit.json` classifies that target as `QList size`. It compares the returned count with the same saved index and branches to the epilogue when `count <= index` (`+0x2d..+0x32`). Thus no row selection or pair construction occurs for invalid indices.
3. The valid path loads the saved R9/R8 point pointers, passes them with a stack destination to `0x1401f45f0` (`Upper +0x42`, Lower +0xc2). The helper performs four 16-byte `rep movsb` copies and produces a 32-byte aggregate: two QPointF-sized members, in input order. Reader construction at `resume50-doe-variable-body-pe-annotated.asm:3330-3345` / `:3537-3552` independently binds each input to the imported `QPointF(double,double)` constructor and copies each constructed 16-byte point into the saved arguments.
4. At `Upper +0x57` / `Lower +0xd7`, the method calls the row selector `0x1402eee10` with the outer `Var+0x28` list and saved index. It obtains the selected row pointer, then passes the row's correspondence-list member plus the pair temporary to `0x1400ad3b0` (Upper member offset `+0x28` at `+0x60..+0x76`; Lower member offset `+0x30` at `+0xe0..+0xf6`). The per-side offsets are distinct; this is not one shared paired-list field.
5. The bounded `0x1400ad3b0` body has the QList append pattern: it checks implicit-sharing state, takes a detach/grow path when shared, or obtains the existing list end when unshared, then constructs/copies the supplied 32-byte value at the append position. There is no index-zero argument to this inner helper. The same append helper is called by both receiver methods. This establishes append rather than replacement.

The original reader fixes the outer target row: `xor edx,edx` immediately precedes each call (`resume50-doe-variable-body-pe-annotated.asm:3347-3350` and `:3554-3557`). The Upper method completes and its temporary QString/QPointF cleanup runs before the reader advances to later siblings; the Lower method is called at its later branch. There is no rollback of an already appended Upper pair if a subsequent Lower pair operation fails.

## Failure and uncertainty boundary

For invalid outer row indices, the receiver methods have an explicit no-op path. On a valid row, both methods call a void append helper and have no post-call result/status branch. The captured methods do not catch exceptions or translate allocation failures. The PE bytes establish the COW/detach-or-append structure and that the aggregate copy itself is fixed-size; they do not establish allocator failure behavior, the concrete Qt allocator's exception policy, or whether a thrown failure is recoverable at the outer reader. Preserve that as unknown; do not invent a rollback guarantee.

The caller creates PointPair components by splitting on `", "` with KeepEmptyParts, then requests token indices 0-3 and calls `QString::toDouble` with a null `ok` output before constructing the QPointFs (`resume115-doe-correspondence-owner-plan.md:9`). Those conversion/indexing details precede these methods. This receiver audit does not close locale-dependent numeric parsing, malformed token behavior, or the actual Geometry import caller/admission.

## Implementation consequence

If a later isolated receiver-only increment is authorized, model two independent `QList<QPair<QPointF,QPointF>>`-equivalent sequences on `GeoFile`: Upper pairs and Lower pairs. Each pair holds two doubles per QPointF. Append to the selected row's side list; do not write an element at index zero. For this reader's observed calls the selected row index is zero. Keep the original public Geometry refusal and do not claim actual XML-to-row integration from this helper evidence alone.
