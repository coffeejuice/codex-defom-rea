# Resume 47: inherited SimCtrl.PPV omission audit

## Finding

The next reachable in-scope inherited omission with a concrete first consumer is recovery-list `SimCtrl.PPV` handling. Current code refuses Open when the active operation's `SimCtrl.PPV` exists, and refuses Save when its recovery-B list is nonempty. This is a confirmed unsupported family on the selected lifecycle, not merely a static candidate.

## Source-bound reachability and first consumers

### Open

`engine::openActiveOperation` dispatches the active flow item's class loader in the normal selected-subproject path (`source/apps/moproj2mst/src/engine/loader.cpp`, around 1052–1079). The base loader calls `loadSimCtrl`; its body checks for `SimCtrl.PPV` after loading `SimCtrl.KEY` (`loader.cpp:225–245`). When present it throws `M2M_E_UNSUPPORTED` with native reader address `0x140d4b400` and unrecovered separator `0x1446b4b20`. Original retained caller evidence in `evidence/gui_mo/s1/mdt/L0/A/b0.c` and `b0a.c` shows calls to `0x140d4b400` in the SimCtrl import path. A project with this file reaches the unsupported branch during Open.

### Save

`saveCurrent` reaches `saveSimCtrl` for the active operation (`source/apps/moproj2mst/src/engine/save_current.cpp`, around 276–298). The writer emits `SimCtrl.KEY`; when `op.recoveryB` is nonempty it then refuses with `M2M_E_UNSUPPORTED`, identifying native writer `0x140d4afe0`. Recovery list A is owned by the active subproject data root (`model.hpp:302,365`); recovery list B belongs to the operation. Existing model-level SimCtrl serialization handles corresponding `NRECVL`/`ERECVL` KEY records (`src/mst/emit_b_lines.cpp`), but does not establish the PPV companion-file contract.

The first downstream external consumer is the native SimCtrl reader/writer, not MST text: PPV import populates recovery records and the Save writer persists recovery data. Exact two-list grammar and read/write ordering remain incomplete.

## Classification

- Reachability: established for selected-operation Open/Save, conditional on a PPV file or nonempty recovery list.
- Current behavior: explicit unsupported failure on both Open and Save.
- Native qualification: reader `0x140d4b400`, separator `0x1446b4b20`, writer `0x140d4afe0`; retained caller evidence and address annotations, not a new runtime capture.
- C13 relationship: inherited recovery-list omission now proven as a concrete refusal; track as a confirmed in-scope gap, not only a silent candidate.

## Narrow implementation and test, once qualified

Implement the paired PPV reader/writer behind existing SimCtrl load/save hooks, preserving native side-file order. First recover separator, list ownership, row grammar, defaults, malformed-row behavior, and failure ordering from `0x140d4b400`/`0x140d4afe0` and native model fields. Preserve direct-write and partial-failure behavior. Do not remove the refusal while leaving lists empty.

Focused regression: one disposable project with both recovery lists populated and `SimCtrl.PPV`; compare fresh original and converter Open → Save for parsed values/ownership, SimCtrl.KEY bytes, SimCtrl.PPV bytes, and malformed-row/write-failure order. Add an absent-file/empty-list case. Existing `NRECVL`/`ERECVL` KEY tests do not establish PPV coverage.

## Smallest remaining evidence query

The exact `0x140d4b400` and `0x140d4afe0` bodies are not present as individual local decompilation files in the searched subset. Before implementation, request a bounded static extraction of those two functions and immediate callers only:

1. Reader: file-open branch, row split, list A/B assignment, return/failure paths, separator and conversion calls.
2. Writer: record traversal, exact formatting, open/write/close handling, return behavior.
3. Both: caller addresses and branch selecting each list.

This is sufficient to define the implementation contract without a broad dossier or new original process. Then validate one original-first fixture and expand coverage only when the recovered branches require it.

## Qualifications

This conclusion applies to normal selected-operation Open/Save paths and conditional PPV/list presence; not every project uses recovery tracking. Checkpoint45 has no fresh PPV parity result. No source changes, build, GUI activity, or REA database mutation were performed.
