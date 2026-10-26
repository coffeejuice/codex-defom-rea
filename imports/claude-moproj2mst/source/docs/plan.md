# Plan: finishing the DEFORM .DB loader

State on 2026-10-20 (branch `claude/project-thread-5xiymd`): the container, step directory, step header,
region tables, dynamic arrays and about 45 per-node/per-element keywords are specified in
`docs/db-format-spec.md`, implemented in `deformdb/`, and match DEFORM's KEY export on both example
databases.

Rules that apply to every step: layouts come from static analysis of DEF_PRE_64.exe; the example
databases are used only to test what the code shows (KEY export run on copies); licence code is not
analysed.

## 1. Pin down the DMO interface (small, first)

List the exports and call patterns of `dll-text-pre.dll` and how DMO calls it, so every later step
produces data in the shape DMO needs. Output: the target API table in `docs/`, with each function
mapped to a `deformdb` call or to a gap.

Why first: it is cheap and tells us which keywords and access patterns DMO actually uses, so step 2
can be prioritised instead of being done blind.

## 2. Full keyword coverage (largest step)

Map all 304 keywords in `docs/re/keyword_registry.tsv` to their storage, grouped as the registry does:

| Group | Count | Where the data lives (from the code so far) |
|---|---|---|
| object | 137 | per-node/per-element regions (≈45 done), IIOBJ/LLOBJ records, LLBND/IIBDY boundary data, face BCs (ECC*) |
| simulation control | 70 | IISIM / LLSIM, step header |
| material | 42 | LLMPR (500 reals per material), LLTFM, IIFNC/LLFNC function tables |
| command | 33 | action keywords; mostly not stored in the DB (to be confirmed per keyword) |
| inter-object | 11 | contact / friction / heat-transfer data (region to be located) |
| phase transformation | 10 | LLTFM and related |

Method: follow each keyword's KEY writer and its loader (`FUN_1402a4a80` and siblings, the
`ReadSimCtrl`/`ReadObjInfo`/material readers), add the layout to the spec, expose it in `StepModel`,
and add it to `compare_key.py`. Every keyword gets a status in a coverage table: verified against the
KEY export, read from code but absent from the example files, or not stored in the DB.

## 3. Large-file performance

* Read only the regions a caller asks for (each region is its own compressed block).
* Lazy, cached directory and per-step region tables for 10 000+ step files.
* Cold-cache benchmark on the 5 GB file (cache flushed or first read after reboot) for full steps,
  single keywords and all steps in parallel, compared with DEF_PRE.

## 4. Drop-in replacement for dll-text-pre.dll

Implement the API from step 1 on top of `deformdb` (same export names and calling convention where
DMO links against them), then test inside DMO on both example databases. DMO itself is not modified.

## 5. Older databases (step version < 11000)

Implement the documented address-table remap (spec §6.2) and the older header layouts, so the typed
view works for 3D 2300–10999 and 2D 6000–10999. Needs at least one older .DB file to test; until one
is available this stays code-only.

## Order rationale

Step 1 is a short investigation that shapes steps 2 and 4. Step 2 is the stated top priority and the
largest piece. Step 3 can run alongside step 2 because it touches only the I/O layer. Step 5 is last
because there is no test file for it yet.
