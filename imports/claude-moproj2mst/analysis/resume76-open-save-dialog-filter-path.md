# CP76: Open/Save filter and path order

## Finding

The retained PE evidence proves the **Open filter vocabulary** and the exact path-normalization branch after a file has been selected. The first retained filter string is `(*.moproj *.MOPROJ)` with label `Deform Project Files`; it is followed by database, keyword, and legacy-project filters. However, the available evidence does not include the `QFileDialog` construction/call site for the ordinary File > Open command, so it does not prove which filter is initially selected, the initial directory, or that the first filter is the actual default in a fresh session.

File > Save in the selected single-project lifecycle has no Save As dialog in the retained native call chain. It writes using the already-open project identity: `.moproj` uses the normalized project path, while `.MST` is derived by replacing the suffix on that path. Therefore there is no source-qualified default Save filter for this lifecycle; a Save As filter is outside the evidenced path.

## Open filter evidence and limits

`evidence/gui_mo/strings.txt` retains these adjacent literals:

1. `(*.moproj *.MOPROJ)` / `Deform Project Files`
2. `(*.db *.DB)` / `Database Files`
3. `(*.key *.KEY)` / `Keyword Files`
4. `(*.dprj *.DPRJ *.d2prj *.D2PRJ *.d3prj *.D3PRJ)` / `Previous F2/F3 Project Files`
5. `(*.d2mo *.D2MO *.d3mo *.D3MO)` / `Previous MO Project Files`
6. `(*.d2md *.D2MD *.dmd *.DMD)` / `Previous Machining Distortion Project Files`
7. `(*.mdt *.MDT)` / `Previous Cogging Project Files`

The retained string table establishes presence and order in the image, not the live dialog's selected row or user-visible order. The PE source inventory identifies `0x14001ca30` as the File > Open dispatcher, but that routine accepts the selected path as an argument; its retained decompile does not construct the ordinary file dialog. Other retained `getOpenFileName` call sites belong to a model import path and are not evidence for the project Open dialog. A fresh-dialog capture or the missing caller/decompile that builds this exact dialog is required to qualify the initial filter/default directory.

## Selected-path normalization order (static native contract)

For an ordinary selected path, the retained `0x14001ca30` routine first excludes legacy import suffixes (`dprj`, `d2prj`, `d3prj`, `d2mo`, `d3mo`, `d2md`, `dmd`, `mdt`) to the separate import path. For the in-scope project lifecycle it:

1. Computes the data-root identity from the selected path: if the selection is `.DB`, it is used as-is; otherwise `changeSuffix(selected, ".DB")` is used. `changeSuffix` uses `QFileInfo(path)`, `completeBaseName`, and `QDir::cleanPath`.
2. Finds or creates the data root/project object using that DB identity, then calls `0x14008a9b0(project, selectedPath)`.
3. Inside `0x14008a9b0`, accepts `.moproj` directly and stores `QDir::cleanPath(selectedPath)` as project path. A `.DB` or `.KEY` selection is converted to `.moproj` with `changeSuffix`, then cleaned. Other suffixes return false.
4. Sets Open state, runs the retained FOR003 offer, updates the data root from the cleaned DB path, and parses the `.moproj` only on the `.moproj` branch. It then sets the process current directory to `cleanPath(QFileInfo(projectPath).absolutePath())` and refreshes the window. The Open return can be true despite a parse failure; `project+0x55` records successful `.moproj` parsing.

Thus for the first-listed `*.moproj` class, the selected string is cleaned as the project path; a `.DB`-sibling key is derived for root lookup before parsing. The choice of default filter is not needed to test this already selected-path branch.

## Save path in this lifecycle

Native File > Save uses the current project rather than asking for a destination. The `.moproj` writer consumes the current `project+0x18` path; native Open has already cleaned it. The MST path helper (`0x1400a50a0`) uses the full project path when `full=true`, then `changeSuffix(..., ".MST")`, which cleans the result. For example, a project path `C:/x/Name.moproj` yields `C:/x/Name.MST` under the retained Qt path helper contract. The selected Save path does not run a separate name filter or Save As directory chooser.

## Smallest candidate-ready branch

Candidate work can be narrowly tested without dialog emulation: add cases at the existing Open path seam for explicitly selected `.moproj`, `.DB`, and `.KEY` paths, asserting suffix handling, cleaned `project+0x18`, derived `.DB` identity, parse branch, and project directory. `tests/p01/test_walk.cpp` exercises a corpus `.moproj` and the driver's DB identity setup, but it is not by itself proof of full `.DB`/`.KEY` selected-path coverage. Any extension should stay at the selected-path seam and not claim dialog defaults. A separate Save assertion can pin current-project `.moproj` plus sibling `.MST` output derivation.

Do not implement a candidate Open-dialog filter/default-directory choice from the string table alone. The smallest unresolved evidence item is the ordinary File > Open dialog caller that constructs `QFileDialog`/passes filters and selected filter state; a fresh-session screenshot plus selected-filter/default-directory readout would close runtime behavior. No Save As branch is in the selected File > Save lifecycle.

## Evidence

- `evidence/gui_mo/strings.txt` around `0x020fe350`–`0x020fe4d0`: Open filter strings and labels.
- `evidence/gui_mo/s1/s1-01/dc/sub0.c`, `FUN_14001ca30`: selected-path routing, DB identity, project creation, and calls into Open.
- `evidence/gui_mo/s1/s1-01/dc/open0.c`, `FUN_14008a9b0`: suffix validation, `cleanPath`/`changeSuffix`, parse branch, and project-directory switch.
- `source/docs/re/moproj2mst/s1-01/spec.md` §§1–1.2: Open order, helper semantics, and the qualification that the caller is File > Open but licence dialogs are not followed.
- `source/docs/re/moproj2mst/s1-02/spec.md` §§2.2–2.3: native Save MST destination derivation.
- `analysis/closure-audit.json`: identifies `0x14001ca30` as File > Open dispatcher and explicitly records incomplete broader default-dialog closure.

Static retained evidence only. No source, build, GUI, or mutable REA action was performed.
