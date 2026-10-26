# Status: keyword model <-> .DB mapping (thread bz5k5e)

Branch `claude/project-thread-bz5k5e` (from `claude/project-thread-xp4r5r`), worktree
`claude-deform-rea-dbmap`, Ghidra copies `deform-rea-work/ghidra_dbmap*`.
Decompiles in `deform-rea-work/out/dbmap/` (`loader_ann.c`, `gendb_ann.c`, `kfw_ann.c` are annotated with region names
by `docs/re/dbmap/annotate.py`).

- [x] region-index globals -> region names (`docs/re/dbmap/region_globals.tsv`)
- [x] per-class mapping sections (docs/re/dbmap/*.md)
- [x] overview `docs/re/db-model-mapping.md`
- [x] spot checks on COG step 25 (overview §7); 2D layouts still unchecked on data
