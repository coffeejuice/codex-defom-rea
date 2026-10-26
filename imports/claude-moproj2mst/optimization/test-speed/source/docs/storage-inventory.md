# Folders the DEFORM recovery created on Sim2

Taken 2026-10-22 for [storage-plan.md](storage-plan.md). Sizes in GB (2^30 bytes); "build GB" is the part inside `build*` folders.
Class: **keep** = evidence or sources to archive; **scratch** = regenerable, removable after approval; **keep until pushed** = a worktree whose branch or detached commit exists only on Sim2 (check before removal).

Total: 355 folders, 203.5 GB. Also 66 loose build logs and `.cmd` files in the root of `Projects` (scratch, under 10 MB).

| Group | Folders | GB |
|---|---:|---:|
| C:\Users\alext\Projects | 138 | 19.8 |
| Claude session worktrees (claude-deform-rea\.claude\worktrees) | 33 | 0.9 |
| C:\Users\alext\Projects\deform-rea-work | 163 | 44.9 |
| Home folder, tools and caches | 11 | 131.9 |
| %TEMP% leftovers (grouped by name pattern) | 10 | 6.1 |

## C:\Users\alext\Projects

| Folder | GB | build GB | Purpose | Class |
|---|---:|---:|---|---|
| `DEFORM_corpus` | 5.87 | 0.00 | test corpus (hash-checked copies) | keep |
| `claude-deform-rea-dbread` | 0.67 | 0.66 | git worktree, branch claude/p-dbread (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-kwmerge` | 0.54 | 0.53 | git worktree, branch claude/kw-merge (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p5gate` | 0.40 | 0.39 | git worktree, branch claude/p5-gate (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p401` | 0.37 | 0.36 | git worktree, branch claude/dbkey-triage (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-intall` | 0.32 | 0.30 | git worktree, branch claude/int-all (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p673` | 0.32 | 0.31 | git worktree, branch claude/p67-3 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p910int` | 0.32 | 0.30 | git worktree, branch claude/p910-int (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p10-02` | 0.29 | 0.28 | git worktree, branch claude/p10-02 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p8base` | 0.29 | 0.27 | git worktree, branch claude/p8-base (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-a050` | 0.28 | 0.27 | git worktree, branch claude/p5-14001a050 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-c1xint` | 0.28 | 0.26 | git worktree, branch scratch/c1x-on-p8int (was local only), backed up as backup/<branch> 2026-10-22 | keep |
| `claude-deform-rea-p802int` | 0.28 | 0.27 | git worktree, branch claude/p8-int (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-msgunit` | 0.27 | 0.25 | git worktree, branch claude/p5-msgunit (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p67` | 0.24 | 0.23 | git worktree, branch claude/project-thread-6cvah9 (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-p67gate` | 0.21 | 0.20 | git worktree, branch claude/p67-gate (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-p675fix` | 0.20 | 0.19 | git worktree, branch claude/p67-5-fix-p675fix (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p808` | 0.20 | 0.19 | git worktree, branch claude/p8-08 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-c05` | 0.18 | 0.17 | git worktree, branch claude/c05 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-c07` | 0.18 | 0.17 | git worktree, branch claude/c07 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-censussmall` | 0.18 | 0.17 | git worktree, branch claude/census-small (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p801` | 0.18 | 0.17 | git worktree, branch claude/p8-01 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p806` | 0.18 | 0.17 | git worktree, branch claude/p8-06 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p807` | 0.18 | 0.17 | git worktree, branch claude/p8-07 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p9-01` | 0.18 | 0.17 | git worktree, branch claude/p9-01 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p9-03` | 0.18 | 0.17 | git worktree, branch claude/p9-03 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-c11` | 0.17 | 0.15 | git worktree, branch claude/p67-1-2d-c11 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-c12` | 0.17 | 0.15 | git worktree, branch claude/p67-1-2d-c12 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-c13` | 0.17 | 0.15 | git worktree, branch claude/p67-1-2d-c13 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-c1x` | 0.17 | 0.16 | git worktree, branch claude/p67-1-2d-c1x (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-g2sv` | 0.17 | 0.16 | git worktree, detached HEAD, commit already on main | scratch |
| `claude-deform-rea-p671-2d` | 0.17 | 0.15 | git worktree, branch claude/p67-1-2d (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p804` | 0.17 | 0.16 | git worktree, branch claude/p8-04 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p805` | 0.17 | 0.16 | git worktree, branch claude/p8-05 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p809` | 0.17 | 0.16 | git worktree, branch claude/p8-09 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p671-c08` | 0.16 | 0.15 | git worktree, branch p67-1-2d-c08 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p671-c10` | 0.16 | 0.15 | git worktree, branch p67-1-2d-c10 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p675` | 0.16 | 0.16 | git worktree, branch claude/p67-5 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p803` | 0.16 | 0.15 | git worktree, branch claude/p8-03 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p677` | 0.15 | 0.14 | git worktree, branch claude/p67-7 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p677-int` | 0.15 | 0.14 | git worktree, branch claude/p67-7-int (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-p2rest` | 0.13 | 0.12 | git worktree, branch claude/p2-rest (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p501` | 0.13 | 0.12 | git worktree, branch claude/p5-01-follow (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p673i` | 0.13 | 0.12 | git worktree, branch claude/p67-3-int (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p9-03b` | 0.13 | 0.12 | git worktree, branch p9-03b (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p677-search` | 0.12 | 0.11 | git worktree, branch claude/p67-7-search (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-pdie` | 0.12 | 0.11 | git worktree, branch claude/dbread-fix-pdie (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p677-cube` | 0.11 | 0.10 | git worktree, branch claude/p67-7-cube (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p677-decide` | 0.11 | 0.10 | git worktree, branch claude/p67-7-decide (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p677-srcnt` | 0.11 | 0.10 | git worktree, branch claude/p67-7-srcnt (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p677-surface` | 0.11 | 0.10 | git worktree, branch claude/p67-7-surface (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p677a` | 0.11 | 0.10 | git worktree, branch claude/p67-7-adapters (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p672` | 0.10 | 0.09 | git worktree, branch claude/p67-2 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-kwm-audit` | 0.09 | 0.08 | git worktree, branch claude/kwm-audit (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-kwm-curve` | 0.09 | 0.08 | git worktree, branch claude/kwm-curve (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-kwm-movctl` | 0.09 | 0.08 | git worktree, branch claude/kwm-movctl (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-kwm-win3` | 0.09 | 0.08 | git worktree, branch claude/kwm-win3 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p1001` | 0.09 | 0.09 | git worktree, branch claude/p10-01 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p407` | 0.09 | 0.08 | git worktree, branch claude/p4-07 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p501int` | 0.09 | 0.09 | git worktree, branch p501-int (was local only), backed up as backup/<branch> 2026-10-22 | keep |
| `claude-deform-rea-p5sk` | 0.09 | 0.09 | git worktree, branch claude/p5-shortkey (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p671` | 0.09 | 0.08 | git worktree, branch claude/p67-1 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p672-d2` | 0.09 | 0.08 | git worktree, branch p672-d2 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p673s` | 0.09 | 0.08 | git worktree, branch claude/p67-3-srcnt4 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p674` | 0.09 | 0.08 | git worktree, branch claude/p67-4 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p676` | 0.09 | 0.08 | git worktree, branch claude/p67-6 (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-p677-pipeline` | 0.09 | 0.08 | git worktree, branch claude/p67-7-pipeline (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p802` | 0.09 | 0.08 | git worktree, branch claude/p8-02 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p902` | 0.09 | 0.08 | git worktree, branch claude/p9-02 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p904` | 0.09 | 0.08 | git worktree, branch claude/p9-04 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p905` | 0.09 | 0.09 | git worktree, branch claude/p9-05 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-kwg3` | 0.08 | 0.08 | git worktree, branch claude/project-thread-3dav7e (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p672-cube` | 0.08 | 0.08 | git worktree, branch p672-cube (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p672-path` | 0.08 | 0.08 | git worktree, branch p672-path (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-g2geo` | 0.07 | 0.07 | git worktree, branch claude/kw-g2geo (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p1gate` | 0.06 | 0.05 | git worktree, branch claude/p1-gate (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-g2bc` | 0.05 | 0.04 | git worktree, branch claude/kw-g2bc (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-kwg2mg` | 0.05 | 0.04 | git worktree, branch claude/p4-plan (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-p403` | 0.05 | 0.05 | git worktree, branch claude/p4-03 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p673o` | 0.05 | 0.04 | git worktree, branch claude/p67-3-stdout (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-cubecheck` | 0.04 | 0.03 | git worktree, detached HEAD, commit already on main | scratch |
| `claude-deform-rea-kwg2obj` | 0.04 | 0.04 | git worktree, branch claude/project-thread-4e1o32 (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-kwg4b` | 0.04 | 0.03 | git worktree, branch claude/project-thread-hca1wm (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-kcprintf` | 0.03 | 0.02 | git worktree, branch claude/key-common-printf (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-kwg1a` | 0.03 | 0.03 | git worktree, branch claude/project-thread-3qftn2 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-kwg1b` | 0.03 | 0.03 | git worktree, branch claude/project-thread-2zjao7 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-kwg4a` | 0.03 | 0.02 | git worktree, branch claude/project-thread-f8t4ri (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-mcli` | 0.03 | 0.03 | git worktree, branch claude/project-thread-69qrjt (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p104` | 0.03 | 0.02 | git worktree, branch claude/project-thread-58tdqr (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p107` | 0.03 | 0.02 | git worktree, branch claude/project-thread-m7jc8f (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p108` | 0.03 | 0.02 | git worktree, branch claude/project-thread-x6l2gx (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p112` | 0.03 | 0.02 | git worktree, branch claude/p1-12-dbcore (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p112v` | 0.03 | 0.03 | git worktree, branch p112-verify-local (was local only), backed up as backup/<branch> 2026-10-22; 2 uncommitted files (datarm_dir.cpp, db_info.cpp) | keep until reviewed |
| `claude-deform-rea-p403o` | 0.03 | 0.02 | git worktree, branch p403-oracle-local (was local only), backed up as backup/<branch> 2026-10-22 | keep |
| `claude-deform-rea-p4repro` | 0.03 | 0.03 | git worktree, detached HEAD, commit already on main; 3 uncommitted files (objcheck test tweaks) | keep until reviewed |
| `scratch_3dav7e` | 0.03 | 0.02 | packet scratch / e2e run output | scratch |
| `claude-deform-rea` | 0.02 | 0.01 | main checkout of the port repo (size without .claude\worktrees) | keep |
| `claude-deform-rea-comparator` | 0.02 | 0.02 | git worktree, branch claude/comparator (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-gate` | 0.02 | 0.02 | git worktree, branch claude/project-thread-fh7qbl (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-keycommon` | 0.02 | 0.02 | git worktree, branch claude/project-thread-4ygsdl (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p101` | 0.02 | 0.02 | git worktree, branch claude/project-thread-3jrqes (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p102` | 0.02 | 0.02 | git worktree, branch claude/project-thread-kvhbb6 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p103` | 0.02 | 0.02 | git worktree, branch claude/project-thread-8pgms3 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p105` | 0.02 | 0.02 | git worktree, branch claude/p1-05-sdctl (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p106` | 0.02 | 0.02 | git worktree, branch claude/project-thread-h1h0q5 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p109` | 0.02 | 0.02 | git worktree, branch claude/project-thread-bfa515 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p110` | 0.02 | 0.02 | git worktree, branch claude/project-thread-069xoh (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p110r` | 0.02 | 0.02 | git worktree, branch claude/p1-10-rebased (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p111` | 0.02 | 0.02 | git worktree, branch claude/project-thread-wr4cir (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p113` | 0.02 | 0.02 | git worktree, branch claude/project-thread-fzevnx (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p114` | 0.02 | 0.02 | git worktree, branch claude/project-thread-8ke72q (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p1found` | 0.02 | 0.02 | git worktree, branch claude/project-thread-18pl1g (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-c04c06` | 0.01 | 0.00 | git worktree, branch claude/c04-c06 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-census` | 0.01 | 0.00 | git worktree, branch claude/census (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-dbmap` | 0.01 | 0.01 | git worktree, branch claude/project-thread-bz5k5e (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-p1003` | 0.01 | 0.00 | git worktree, branch claude/p10-03 (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-p675f` | 0.01 | 0.00 | git worktree, branch claude/p67-5-fix (merged into main) | scratch (sources in git); build output inside |
| `p802-scratch` | 0.01 | 0.00 | packet scratch / e2e run output | scratch |
| `p8review_e2e_runs` | 0.01 | 0.00 | packet scratch / e2e run output | scratch |
| `c1x_e2e_runs` | 0.00 | 0.00 | packet scratch / e2e run output | scratch |
| `claude-deform-rea-cjcz5r` | 0.00 | 0.00 | git worktree, branch claude/project-thread-cjcz5r (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-corpus` | 0.00 | 0.00 | git worktree, branch claude/corpus-0.3 (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-grp2` | 0.00 | 0.00 | git worktree, branch claude/project-thread-j4c7t9 (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-kw13` | 0.00 | 0.00 | git worktree, branch claude/project-thread-qj8op8 (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-kw457` | 0.00 | 0.00 | git worktree, branch claude/project-thread-ujpfry (pushed, not merged) | scratch (sources in git); build output inside |
| `claude-deform-rea-kwreg` | 0.00 | 0.00 | git worktree, branch claude/project-thread-kd6glp (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-ledger` | 0.00 | 0.00 | leftover folder, not a registered worktree | scratch |
| `claude-deform-rea-oracle` | 0.00 | 0.00 | git worktree, branch claude/oracle-runner (merged into main) | scratch (sources in git); build output inside |
| `claude-deform-rea-p89` | 0.00 | 0.00 | git worktree, branch claude/project-thread-oeb34z (merged into main) | scratch (sources in git); build output inside |
| `gate-scratch` | 0.00 | 0.00 | packet scratch / e2e run output | scratch |
| `kwm-audit-save` | 0.00 | 0.00 | packet scratch / e2e run output | scratch |
| `p674-scratch` | 0.00 | 0.00 | packet scratch / e2e run output | scratch |
| `p675fix-scratch` | 0.00 | 0.00 | packet scratch / e2e run output | scratch |
| `p677-srcnt-tools` | 0.00 | 0.00 | packet scratch / e2e run output | scratch |
| `p801-scratch` | 0.00 | 0.00 | packet scratch / e2e run output | scratch |
| `p803-scratch` | 0.00 | 0.00 | packet scratch / e2e run output | scratch |
| `p804-scratch` | 0.00 | 0.00 | packet scratch / e2e run output | scratch |
| `p809-scratch` | 0.00 | 0.00 | packet scratch / e2e run output | scratch |

## Claude session worktrees (claude-deform-rea\.claude\worktrees)

| Folder | GB | build GB | Purpose | Class |
|---|---:|---:|---|---|
| `vibrant-kowalevski-75b9e0` | 0.32 | 0.30 | Claude session worktree, branch claude/p2-183d00 merged | scratch |
| `angry-kilby-bee9b8` | 0.09 | 0.08 | Claude session worktree, branch p405-oracle (pushed) | scratch |
| `pensive-bardeen-cc7b7e` | 0.09 | 0.08 | Claude session worktree, branch claude/p4-06 merged | scratch |
| `zealous-tharp-625a23` | 0.09 | 0.08 | Claude session worktree, branch claude/p4-04 merged | scratch |
| `pensive-pasteur-7fcc3e` | 0.06 | 0.05 | Claude session worktree, detached HEAD, commit already on main | scratch |
| `elastic-williams-1b3b9d` | 0.05 | 0.05 | Claude session worktree, branch claude/p4-02b merged | scratch |
| `festive-tharp-204c95` | 0.05 | 0.05 | Claude session worktree, branch claude/p4-02a merged | scratch |
| `friendly-turing-bdc87c` | 0.02 | 0.00 | Claude session worktree, branch claude/friendly-turing-bdc87c merged | scratch |
| `youthful-perlman-f9f4ae` | 0.02 | 0.00 | Claude session worktree, branch claude/project-thread-mq7y7u merged | scratch |
| `competent-raman-04f283` | 0.01 | 0.00 | Claude session worktree, branch claude/competent-raman-04f283 merged | scratch |
| `determined-wing-88bfd7` | 0.01 | 0.00 | Claude session worktree, branch claude/determined-wing-88bfd7 merged | scratch |
| `elastic-cori-259957` | 0.01 | 0.00 | Claude session worktree, branch claude/elastic-cori-259957 merged | scratch |
| `elastic-meitner-a81ed9` | 0.01 | 0.00 | Claude session worktree, branch claude/elastic-meitner-a81ed9 merged | scratch |
| `elated-tharp-d77b63` | 0.01 | 0.00 | Claude session worktree, branch claude/elated-tharp-d77b63 merged | scratch |
| `funny-goldberg-785b77` | 0.01 | 0.00 | Claude session worktree, branch claude/funny-goldberg-785b77 merged | scratch |
| `hungry-wiles-11d339` | 0.01 | 0.00 | Claude session worktree, branch claude/project-thread-yhfsxj merged | scratch |
| `intelligent-napier-13845a` | 0.01 | 0.00 | Claude session worktree, branch claude/plan-solver-harness pushed | scratch |
| `magical-euclid-62300d` | 0.01 | 0.00 | Claude session worktree, branch claude/magical-euclid-62300d merged | scratch |
| `magical-khorana-dcdea4` | 0.01 | 0.00 | Claude session worktree, branch claude/gui-run-research merged | scratch |
| `quizzical-rubin-cde642` | 0.01 | 0.00 | Claude session worktree, branch claude/plan-p5-p10 merged | scratch |
| `reverent-saha-cd33d7` | 0.01 | 0.00 | Claude session worktree, branch claude/gui-mo-research pushed | scratch |
| `trusting-cartwright-a46606` | 0.01 | 0.00 | Claude session worktree, branch claude/trusting-cartwright-a46606 merged | scratch |
| `zen-khayyam-73660a` | 0.01 | 0.00 | Claude session worktree, branch claude/project-thread-dh47x7 pushed | scratch |
| `awesome-dirac-04e468` | 0.00 | 0.00 | Claude session worktree, branch claude/awesome-dirac-04e468 merged | scratch |
| `elastic-liskov-14bb4f` | 0.00 | 0.00 | Claude session worktree, branch claude/elastic-liskov-14bb4f merged | scratch |
| `epic-heyrovsky-727397` | 0.00 | 0.00 | Claude session worktree, branch claude/epic-heyrovsky-727397 merged | scratch |
| `kind-goodall-f8fe0c` | 0.00 | 0.00 | Claude session worktree, branch claude/kind-goodall-f8fe0c merged | scratch |
| `musing-fermi-d31c14` | 0.00 | 0.00 | Claude session worktree, branch claude/musing-fermi-d31c14 merged | scratch |
| `musing-poincare-7325e3` | 0.00 | 0.00 | Claude session worktree, branch claude/musing-poincare-7325e3 merged | scratch |
| `reverent-wing-c0e777` | 0.00 | 0.00 | Claude session worktree, branch claude/reverent-wing-c0e777 merged | scratch |
| `sleepy-lumiere-55d9e9` | 0.00 | 0.00 | Claude session worktree, branch claude/sleepy-lumiere-55d9e9 merged | scratch |
| `sleepy-mendel-ea9662` | 0.00 | 0.00 | Claude session worktree, branch claude/sleepy-mendel-ea9662 merged | scratch |
| `sleepy-spence-b82216` | 0.00 | 0.00 | Claude session worktree, branch claude/sleepy-spence-b82216 merged | scratch |

## C:\Users\alext\Projects\deform-rea-work

| Folder | GB | build GB | Purpose | Class |
|---|---:|---:|---|---|
| `out` | 11.10 | 0.02 | decompiles, listings, tables; out\p89\capcache = 11 GB DEFORM capture cache | keep (evidence) |
| `p112_dbcopies` | 4.67 | 0.00 | scratch DB copies / test builds / GUI run dir | scratch |
| `gui_mo` | 0.49 | 0.00 | GUI MO research: Ghidra project, xref data (holds 2 DEFORM exes) | keep (evidence, live) |
| `ghidra` | 0.21 | 0.00 | Ghidra master project: DEF_PRE_64 + DEF_DBM | keep (evidence, live) |
| `ghidra_a` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_c` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_c04c06` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_c05` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_c07` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_c11` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_c13` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_census` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_censussmall` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_cjcz` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_d` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_dbmap` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_dbmap1` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_dbmap2` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_dbmap3` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_dbmap4` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_dbmap5` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_dbmap6` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2a` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2bc_ec` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2bc_local` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2bc_nodal` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2bc_win` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2c` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2copy` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2d` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2e` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2f` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2g` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2h` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2i` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2j` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2k` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_g2obj` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_kc` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_kw13` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_kw457` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_kw457a` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_kw457b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_kw457c` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_kw457d` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_kwg1b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_kwg3` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_kwg4b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_kwreg` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_ledger` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_mcli` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p1001` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p1002` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p101` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p103` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p105` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p106` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p107` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p108` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p109` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p110` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p111` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p112` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p113` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p114` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p1f` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p1g` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p2c` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p2rb` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p402a` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p402b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p403` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p404` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p405` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p405b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p405m` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p406` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p407` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p4g3d` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p501` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p510` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p5sk` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p67` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p671` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p6712d` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p671c` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p671c08` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p671c10` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p671m` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p672` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p672a` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p672b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p672c` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p673` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p673a` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p673b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p673c` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p673d` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p673e` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p674` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p675` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p675a` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p675b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p675c` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p675fix` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p676` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p676b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p677` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p677a` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p677b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p677c` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p677d` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p677e` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p677f` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p801` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p802` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p803` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p804` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p805` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p806` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p807` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p808` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p809` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p89` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p89f1` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p89f2` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p89f3` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p89f4` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p901` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p902` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p903` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p903b` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p904` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `ghidra_p905` | 0.21 | 0.00 | Ghidra worker copy of the master (DEF_PRE_64 + DEF_DBM) | export names, then scratch |
| `scratch_build` | 0.17 | 0.00 | scratch DB copies / test builds / GUI run dir | scratch |
| `simrelink` | 0.13 | 0.00 | solver relink spike (recipes + SFTC lib + relinked exe) | keep recipes only |
| `ghidra_gui` | 0.12 | 0.00 | Ghidra project: DEF_GUI23 | keep (evidence, live) |
| `simrelink2d` | 0.06 | 0.00 | solver relink spike (recipes + SFTC lib + relinked exe) | keep recipes only |
| `scripts_p673` | 0.01 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `guirun` | 0.00 | 0.00 | scratch DB copies / test builds / GUI run dir | scratch |
| `scripts` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_c07` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_mcli` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p101` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p105` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p106` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p108` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p111` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p404` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p406` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p407` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p67` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p677` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p677_cube` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p677_decide` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p677_pipeline` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p677_search` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p677_surface` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p803` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |
| `scripts_p89` | 0.00 | 0.00 | headless Ghidra scripts | keep (evidence) |

## Home folder, tools and caches

| Folder | GB | build GB | Purpose | Class |
|---|---:|---:|---|---|
| `%USERPROFILE%\.deform-oracle` | 70.09 | 0.00 | oracle cache: entries = DEFORM outputs for 1,907 cases; tmp/stage = staging | keep entries; tmp+stage scratch |
| `%USERPROFILE%\.deform-port-runs` | 56.62 | 0.00 | port run dirs (failing runs kept) + port output cache | scratch |
| `%TEMP%\claude` | 2.51 | 0.00 | Claude session scratchpads and task outputs | scratch |
| `%USERPROFILE%\.kwg1a-corpus` | 1.19 | 0.00 | kw-g1a packet copy of corpus files | scratch |
| `C:\tools\ghidra_12.1.4_PUBLIC` | 0.84 | 0.00 | Ghidra install | tool |
| `C:\tools\jdk-21` | 0.32 | 0.00 | JDK for Ghidra | tool |
| `%USERPROFILE%\AppData\Roaming\npm\node_modules\rea-agents` | 0.25 | 0.13 | REA install | tool |
| `%USERPROFILE%\AppData\Local\ghidra` | 0.06 | 0.00 | Ghidra caches | scratch |
| `%USERPROFILE%\AppData\Roaming\ghidra` | 0.01 | 0.00 | Ghidra user settings | keep (small) |
| `%USERPROFILE%\p405_oracle` | 0.00 | 0.00 | p4-05 packet oracle outputs | scratch |
| `%USERPROFILE%\.claude\skills\reverse-engineer-anything` | 0.00 | 0.00 | REA skill for Claude Code | tool |

## %TEMP% leftovers (grouped by name pattern)

| Folder | GB | build GB | Purpose | Class |
|---|---:|---:|---|---|
| `%TEMP%\dbread_w3_*  (66 dirs)` | 2.18 | 0.00 | DBREAD writer test dirs | scratch |
| `%TEMP%\gendb_e2e_*  (146 dirs)` | 1.56 | 0.00 | GENDB e2e run dirs | scratch |
| `%TEMP%\p112_db_*  (1 dirs)` | 1.41 | 0.00 | p1-12 DB test copies | scratch |
| `%TEMP%\p67e2e  (1 dirs)` | 0.33 | 0.00 | P6-7 e2e run dir | scratch |
| `%TEMP%\rea-ghidra-*  (2 dirs)` | 0.30 | 0.00 | REA analysis databases (ephemeral) | scratch |
| `%TEMP%\dbread_rt_*  (39 dirs)` | 0.27 | 0.00 | DBREAD round-trip test dirs | scratch |
| `%TEMP%\*_cmdoracle_*  (947 dirs)` | 0.02 | 0.00 | command oracle run dirs | scratch |
| `%TEMP%\*_p802_*  (10 dirs)` | 0.00 | 0.00 | p8-02 oracle run dirs | scratch |
| `%TEMP%\rea-readme  (1 dirs)` | 0.00 | 0.00 | REA readme cache | scratch |
| `%TEMP%\ghidra  (1 dirs)` | 0.00 | 0.00 | Ghidra temp | scratch |
