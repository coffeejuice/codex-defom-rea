# Checkpoint 66 histogram indirect-xref attempt

## REA query and outcome

I opened the retained immutable original `evidence/original-binaries/DEF_GUI_MO_64.exe`, verified by SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, using REA's automatic native provider selection. The read-only query `xrefs(address=0x140c15910)` selected Ghidra 12.1.4 but timed out during target import/analysis at the 330,000 ms startup deadline (`provider_timeout`). It returned no xref data, so this is an unavailable query, not evidence of zero xrefs.

I closed the active REA target afterward with `close_binary`; the tool returned `result: null` and no analysis snapshot was saved. No source, build, GUI, or target mutation occurred. The compact lifecycle/query receipt is `analysis/resume66-histogram-xref-capture.txt`.

## Remaining edge

The exact open edge is still a caller of manager wrapper `0x140c15910` and the first use of its returned RAX. If resolved, that use must be followed only far enough to classify scene insertion, manager+`0x10` guard registration, another owner store, or pass-through. The retained PE-byte searches in `resume60-graph-owner-xref.md` and `resume64-histogram-caller-raw.txt` found no direct/common incoming reference; this REA attempt could not extend that result to analyzer-resolved indirect or module references.

The graph ownership decision therefore remains unresolved. No evidence here validates or refutes `PostGraphSceneState::items_` as unique ownership. A completed static xref query using an already-imported root-owned analysis, or a lighter supported analyzer, is required to close the caller edge; the timed-out import itself does not justify any ownership inference.
