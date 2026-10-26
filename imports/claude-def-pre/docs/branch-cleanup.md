# Branch cleanup: keep and delete lists

Prepared 2026-10-22. Already done (nothing deleted): full bundle of all refs on the NAS (`repos\claude-deform-rea_2026-10-22_all-refs.bundle`, verified, SHA-256 beside it); every branch head below has an `archive/<branch>` tag on GitHub (179 tags); gate tags `gate/p0`, `gate/p1`, `gate/p2-3`, `gate/p4`, `gate/p5`, `gate/p6-7` and `v1.0-def-pre` (= main `d7819e3`).

## Keep as branches (16)

| Branch | Patches not on main | Why |
|---|---:|---|
| `claude/census` | 2 | holds work not on main (docs or tests); merge or archive later |
| `claude/dbkey-triage` | 2 | holds work not on main (docs or tests); merge or archive later |
| `claude/gui-mo-research` | 1 | holds work not on main (docs or tests); merge or archive later |
| `claude/p10-03` | 1 | holds work not on main (docs or tests); merge or archive later |
| `claude/p4-05-oracle` | 1 | holds work not on main (docs or tests); merge or archive later |
| `claude/p67-6` | 2 | holds work not on main (docs or tests); merge or archive later |
| `claude/p67-7-int` | 1 | holds work not on main (docs or tests); merge or archive later |
| `claude/plan-solver-harness` | 10 | solver harness plan |
| `claude/project-thread-6cvah9` | 2 | holds work not on main (docs or tests); merge or archive later |
| `claude/project-thread-bz5k5e` | 11 | holds work not on main (docs or tests); merge or archive later |
| `claude/project-thread-cjcz5r` | 4 | holds work not on main (docs or tests); merge or archive later |
| `claude/project-thread-dh47x7` | 3 | GUI MO plan |
| `claude/project-thread-j4c7t9` | 13 | holds work not on main (docs or tests); merge or archive later |
| `claude/project-thread-mq7y7u` | 3 | this storage plan |
| `claude/project-thread-qj8op8` | 3 | holds work not on main (docs or tests); merge or archive later |
| `main` | 0 | main |

## Delete (189 branch names: 174 local, 133 on GitHub)

Each is recoverable from its `archive/` tag. "Patches not on main" > 0 only for local scratch and backup branches (throwaway refolds, debug prints, verification hacks), reviewed in the thread.

| Branch | Local | GitHub | Patches not on main | Tag |
|---|:-:|:-:|---:|---|
| `backup/census-merge` |  | x | 3 | `archive/census-merge` |
| `backup/p112-verify-local` |  | x | 0 | `archive/p112-verify-local` |
| `backup/p402a-verify-local` |  | x | 1 | `archive/p402a-verify-local` |
| `backup/p403-oracle-local` |  | x | 0 | `archive/p403-oracle-local` |
| `backup/p501-int` |  | x | 1 | `archive/p501-int` |
| `backup/p8-02-int-local` |  | x | 3 | `archive/p8-02-int-local` |
| `backup/p804-int` |  | x | 1 | `archive/p804-int` |
| `backup/p804-int3` |  | x | 0 | `archive/p804-int3` |
| `backup/p809-runs` |  | x | 1 | `archive/p809-runs` |
| `backup/p901-with-p904` |  | x | 0 | `archive/p901-with-p904` |
| `backup/scratch/c1x-on-p8int` |  | x | 1 | `archive/scratch/c1x-on-p8int` |
| `backup/wip-p112v` | x | x | 1 | `archive/wip-p112v` |
| `backup/wip-p4repro` | x | x | 1 | `archive/wip-p4repro` |
| `census-merge` | x |  | 3 | `archive/census-merge` |
| `claude/angry-kilby-bee9b8` | x |  | 0 | `archive/claude/angry-kilby-bee9b8` |
| `claude/awesome-dirac-04e468` | x |  | 0 | `archive/claude/awesome-dirac-04e468` |
| `claude/c04-c06` | x | x | 0 | `archive/claude/c04-c06` |
| `claude/c05` | x | x | 0 | `archive/claude/c05` |
| `claude/c07` | x | x | 0 | `archive/claude/c07` |
| `claude/census-small` | x | x | 0 | `archive/claude/census-small` |
| `claude/census-small-p8` | x | x | 0 | `archive/claude/census-small-p8` |
| `claude/comparator` | x | x | 0 | `archive/claude/comparator` |
| `claude/competent-raman-04f283` | x |  | 0 | `archive/claude/competent-raman-04f283` |
| `claude/corpus-0.3` | x | x | 0 | `archive/claude/corpus-0.3` |
| `claude/dbread-fix-model` | x |  | 0 | `archive/claude/dbread-fix-model` |
| `claude/dbread-fix-pdie` | x |  | 0 | `archive/claude/dbread-fix-pdie` |
| `claude/dbread-rt` | x | x | 0 | `archive/claude/dbread-rt` |
| `claude/determined-wing-88bfd7` | x |  | 0 | `archive/claude/determined-wing-88bfd7` |
| `claude/elastic-cori-259957` | x |  | 0 | `archive/claude/elastic-cori-259957` |
| `claude/elastic-liskov-14bb4f` | x |  | 0 | `archive/claude/elastic-liskov-14bb4f` |
| `claude/elastic-meitner-a81ed9` | x |  | 0 | `archive/claude/elastic-meitner-a81ed9` |
| `claude/elastic-williams-1b3b9d` | x |  | 0 | `archive/claude/elastic-williams-1b3b9d` |
| `claude/elated-tharp-d77b63` | x |  | 0 | `archive/claude/elated-tharp-d77b63` |
| `claude/epic-heyrovsky-727397` | x |  | 0 | `archive/claude/epic-heyrovsky-727397` |
| `claude/festive-tharp-204c95` | x |  | 0 | `archive/claude/festive-tharp-204c95` |
| `claude/friendly-turing-bdc87c` | x |  | 0 | `archive/claude/friendly-turing-bdc87c` |
| `claude/funny-goldberg-785b77` | x |  | 0 | `archive/claude/funny-goldberg-785b77` |
| `claude/gui-run-research` | x | x | 0 | `archive/claude/gui-run-research` |
| `claude/hungry-wiles-11d339` | x |  | 0 | `archive/claude/hungry-wiles-11d339` |
| `claude/int-all` | x | x | 0 | `archive/claude/int-all` |
| `claude/intelligent-napier-13845a` | x |  | 0 | `archive/claude/intelligent-napier-13845a` |
| `claude/key-common-printf` | x | x | 0 | `archive/claude/key-common-printf` |
| `claude/kind-goodall-f8fe0c` | x |  | 0 | `archive/claude/kind-goodall-f8fe0c` |
| `claude/kw-g2bc` | x | x | 0 | `archive/claude/kw-g2bc` |
| `claude/kw-g2geo` | x | x | 0 | `archive/claude/kw-g2geo` |
| `claude/kw-g2mg` | x | x | 0 | `archive/claude/kw-g2mg` |
| `claude/kw-merge` | x | x | 0 | `archive/claude/kw-merge` |
| `claude/kwm-audit` | x | x | 0 | `archive/claude/kwm-audit` |
| `claude/kwm-curve` | x | x | 0 | `archive/claude/kwm-curve` |
| `claude/kwm-movctl` | x | x | 0 | `archive/claude/kwm-movctl` |
| `claude/kwm-win3` | x | x | 0 | `archive/claude/kwm-win3` |
| `claude/magical-euclid-62300d` | x |  | 0 | `archive/claude/magical-euclid-62300d` |
| `claude/magical-khorana-dcdea4` | x |  | 0 | `archive/claude/magical-khorana-dcdea4` |
| `claude/musing-fermi-d31c14` | x |  | 0 | `archive/claude/musing-fermi-d31c14` |
| `claude/musing-poincare-7325e3` | x |  | 0 | `archive/claude/musing-poincare-7325e3` |
| `claude/oracle-runner` | x | x | 0 | `archive/claude/oracle-runner` |
| `claude/p-dbread` | x | x | 0 | `archive/claude/p-dbread` |
| `claude/p1-05-sdctl` | x | x | 0 | `archive/claude/p1-05-sdctl` |
| `claude/p1-10-rebased` | x | x | 0 | `archive/claude/p1-10-rebased` |
| `claude/p1-12-dbcore` | x | x | 0 | `archive/claude/p1-12-dbcore` |
| `claude/p1-gate` | x | x | 0 | `archive/claude/p1-gate` |
| `claude/p10-01` | x | x | 0 | `archive/claude/p10-01` |
| `claude/p10-02` | x | x | 0 | `archive/claude/p10-02` |
| `claude/p2-183d00` | x | x | 0 | `archive/claude/p2-183d00` |
| `claude/p2-rest` | x | x | 0 | `archive/claude/p2-rest` |
| `claude/p4-01-driver` | x | x | 0 | `archive/claude/p4-01-driver` |
| `claude/p4-02a` | x | x | 0 | `archive/claude/p4-02a` |
| `claude/p4-02b` | x | x | 0 | `archive/claude/p4-02b` |
| `claude/p4-03` | x | x | 0 | `archive/claude/p4-03` |
| `claude/p4-04` | x | x | 0 | `archive/claude/p4-04` |
| `claude/p4-05` | x | x | 0 | `archive/claude/p4-05` |
| `claude/p4-06` | x | x | 0 | `archive/claude/p4-06` |
| `claude/p4-07` | x | x | 0 | `archive/claude/p4-07` |
| `claude/p4-gate` | x | x | 0 | `archive/claude/p4-gate` |
| `claude/p4-plan` | x | x | 2 | `archive/claude/p4-plan` |
| `claude/p5-01` | x | x | 0 | `archive/claude/p5-01` |
| `claude/p5-01-follow` | x | x | 0 | `archive/claude/p5-01-follow` |
| `claude/p5-14001a050` | x | x | 0 | `archive/claude/p5-14001a050` |
| `claude/p5-gate` | x | x | 0 | `archive/claude/p5-gate` |
| `claude/p5-msgunit` | x | x | 0 | `archive/claude/p5-msgunit` |
| `claude/p5-shortkey` | x | x | 0 | `archive/claude/p5-shortkey` |
| `claude/p67-1` | x | x | 0 | `archive/claude/p67-1` |
| `claude/p67-1-2d` | x | x | 0 | `archive/claude/p67-1-2d` |
| `claude/p67-1-2d-c08` |  | x | 0 | `archive/claude/p67-1-2d-c08` |
| `claude/p67-1-2d-c10` |  | x | 0 | `archive/claude/p67-1-2d-c10` |
| `claude/p67-1-2d-c11` | x | x | 0 | `archive/claude/p67-1-2d-c11` |
| `claude/p67-1-2d-c12` | x | x | 0 | `archive/claude/p67-1-2d-c12` |
| `claude/p67-1-2d-c13` | x | x | 0 | `archive/claude/p67-1-2d-c13` |
| `claude/p67-1-2d-c1x` | x | x | 0 | `archive/claude/p67-1-2d-c1x` |
| `claude/p67-2` | x | x | 0 | `archive/claude/p67-2` |
| `claude/p67-3` | x | x | 0 | `archive/claude/p67-3` |
| `claude/p67-3-int` | x | x | 0 | `archive/claude/p67-3-int` |
| `claude/p67-3-srcnt4` | x | x | 0 | `archive/claude/p67-3-srcnt4` |
| `claude/p67-3-stdout` | x | x | 0 | `archive/claude/p67-3-stdout` |
| `claude/p67-4` | x | x | 0 | `archive/claude/p67-4` |
| `claude/p67-5` | x | x | 0 | `archive/claude/p67-5` |
| `claude/p67-5-fix` | x | x | 0 | `archive/claude/p67-5-fix` |
| `claude/p67-5-fix-p675fix` | x |  | 0 | `archive/claude/p67-5-fix-p675fix` |
| `claude/p67-7` | x | x | 0 | `archive/claude/p67-7` |
| `claude/p67-7-adapters` | x | x | 0 | `archive/claude/p67-7-adapters` |
| `claude/p67-7-cube` | x | x | 0 | `archive/claude/p67-7-cube` |
| `claude/p67-7-decide` | x | x | 0 | `archive/claude/p67-7-decide` |
| `claude/p67-7-pipeline` | x | x | 0 | `archive/claude/p67-7-pipeline` |
| `claude/p67-7-search` | x | x | 0 | `archive/claude/p67-7-search` |
| `claude/p67-7-srcnt` | x | x | 0 | `archive/claude/p67-7-srcnt` |
| `claude/p67-7-surface` | x | x | 0 | `archive/claude/p67-7-surface` |
| `claude/p67-gate` | x | x | 0 | `archive/claude/p67-gate` |
| `claude/p8-01` | x | x | 0 | `archive/claude/p8-01` |
| `claude/p8-02` | x | x | 0 | `archive/claude/p8-02` |
| `claude/p8-03` | x | x | 0 | `archive/claude/p8-03` |
| `claude/p8-04` | x | x | 0 | `archive/claude/p8-04` |
| `claude/p8-05` | x | x | 0 | `archive/claude/p8-05` |
| `claude/p8-06` | x | x | 0 | `archive/claude/p8-06` |
| `claude/p8-07` | x | x | 0 | `archive/claude/p8-07` |
| `claude/p8-08` | x | x | 0 | `archive/claude/p8-08` |
| `claude/p8-09` | x | x | 0 | `archive/claude/p8-09` |
| `claude/p8-base` | x | x | 0 | `archive/claude/p8-base` |
| `claude/p8-int` | x | x | 0 | `archive/claude/p8-int` |
| `claude/p9-01` | x | x | 0 | `archive/claude/p9-01` |
| `claude/p9-02` | x | x | 0 | `archive/claude/p9-02` |
| `claude/p9-03` | x | x | 0 | `archive/claude/p9-03` |
| `claude/p9-04` | x | x | 0 | `archive/claude/p9-04` |
| `claude/p9-05` | x | x | 0 | `archive/claude/p9-05` |
| `claude/p9-ac8a0` | x | x | 0 | `archive/claude/p9-ac8a0` |
| `claude/p910-int` | x | x | 0 | `archive/claude/p910-int` |
| `claude/pensive-bardeen-cc7b7e` | x |  | 0 | `archive/claude/pensive-bardeen-cc7b7e` |
| `claude/pensive-pasteur-7fcc3e` | x |  | 0 | `archive/claude/pensive-pasteur-7fcc3e` |
| `claude/plan-p5-p10` | x | x | 0 | `archive/claude/plan-p5-p10` |
| `claude/project-thread-069xoh` | x | x | 0 | `archive/claude/project-thread-069xoh` |
| `claude/project-thread-17q2q7` | x |  | 0 | `archive/claude/project-thread-17q2q7` |
| `claude/project-thread-18pl1g` | x | x | 0 | `archive/claude/project-thread-18pl1g` |
| `claude/project-thread-2zjao7` | x | x | 0 | `archive/claude/project-thread-2zjao7` |
| `claude/project-thread-3dav7e` | x | x | 0 | `archive/claude/project-thread-3dav7e` |
| `claude/project-thread-3jrqes` | x | x | 0 | `archive/claude/project-thread-3jrqes` |
| `claude/project-thread-3qftn2` | x | x | 0 | `archive/claude/project-thread-3qftn2` |
| `claude/project-thread-4e1o32` | x | x | 0 | `archive/claude/project-thread-4e1o32` |
| `claude/project-thread-4ygsdl` | x | x | 0 | `archive/claude/project-thread-4ygsdl` |
| `claude/project-thread-58tdqr` | x | x | 0 | `archive/claude/project-thread-58tdqr` |
| `claude/project-thread-5xiymd` | x | x | 0 | `archive/claude/project-thread-5xiymd` |
| `claude/project-thread-69qrjt` | x | x | 0 | `archive/claude/project-thread-69qrjt` |
| `claude/project-thread-8ke72q` | x | x | 0 | `archive/claude/project-thread-8ke72q` |
| `claude/project-thread-8pgms3` | x | x | 0 | `archive/claude/project-thread-8pgms3` |
| `claude/project-thread-acezpe` | x | x | 0 | `archive/claude/project-thread-acezpe` |
| `claude/project-thread-bfa515` | x | x | 0 | `archive/claude/project-thread-bfa515` |
| `claude/project-thread-f8t4ri` | x | x | 0 | `archive/claude/project-thread-f8t4ri` |
| `claude/project-thread-fh7qbl` | x | x | 0 | `archive/claude/project-thread-fh7qbl` |
| `claude/project-thread-fzevnx` | x | x | 0 | `archive/claude/project-thread-fzevnx` |
| `claude/project-thread-h1h0q5` | x | x | 0 | `archive/claude/project-thread-h1h0q5` |
| `claude/project-thread-hca1wm` | x | x | 0 | `archive/claude/project-thread-hca1wm` |
| `claude/project-thread-kd6glp` | x | x | 0 | `archive/claude/project-thread-kd6glp` |
| `claude/project-thread-kvhbb6` | x | x | 0 | `archive/claude/project-thread-kvhbb6` |
| `claude/project-thread-m7jc8f` | x | x | 0 | `archive/claude/project-thread-m7jc8f` |
| `claude/project-thread-oeb34z` | x | x | 0 | `archive/claude/project-thread-oeb34z` |
| `claude/project-thread-p263k7` |  | x | 0 | `archive/claude/project-thread-p263k7` |
| `claude/project-thread-ujpfry` | x | x | 10 | `archive/claude/project-thread-ujpfry` |
| `claude/project-thread-vb3uwx` | x | x | 0 | `archive/claude/project-thread-vb3uwx` |
| `claude/project-thread-wr4cir` | x | x | 0 | `archive/claude/project-thread-wr4cir` |
| `claude/project-thread-x6l2gx` | x | x | 0 | `archive/claude/project-thread-x6l2gx` |
| `claude/project-thread-xp4r5r` |  | x | 0 | `archive/claude/project-thread-xp4r5r` |
| `claude/project-thread-yhfsxj` | x | x | 0 | `archive/claude/project-thread-yhfsxj` |
| `claude/quizzical-rubin-cde642` | x |  | 0 | `archive/claude/quizzical-rubin-cde642` |
| `claude/reverent-saha-cd33d7` | x |  | 0 | `archive/claude/reverent-saha-cd33d7` |
| `claude/reverent-wing-c0e777` | x |  | 0 | `archive/claude/reverent-wing-c0e777` |
| `claude/sleepy-lumiere-55d9e9` | x |  | 0 | `archive/claude/sleepy-lumiere-55d9e9` |
| `claude/sleepy-mendel-ea9662` | x |  | 0 | `archive/claude/sleepy-mendel-ea9662` |
| `claude/sleepy-spence-b82216` | x |  | 0 | `archive/claude/sleepy-spence-b82216` |
| `claude/trusting-cartwright-a46606` | x |  | 0 | `archive/claude/trusting-cartwright-a46606` |
| `claude/vibrant-kowalevski-75b9e0` | x |  | 0 | `archive/claude/vibrant-kowalevski-75b9e0` |
| `claude/youthful-perlman-f9f4ae` | x |  | 0 | `archive/claude/youthful-perlman-f9f4ae` |
| `claude/zealous-tharp-625a23` | x |  | 0 | `archive/claude/zealous-tharp-625a23` |
| `claude/zen-khayyam-73660a` | x |  | 0 | `archive/claude/zen-khayyam-73660a` |
| `p112-verify-local` | x |  | 0 | `archive/p112-verify-local` |
| `p402a-verify-local` | x |  | 1 | `archive/p402a-verify-local` |
| `p403-oracle-local` | x |  | 0 | `archive/p403-oracle-local` |
| `p405-oracle` | x |  | 1 | `archive/p405-oracle` |
| `p501-int` | x |  | 1 | `archive/p501-int` |
| `p67-1-2d-c08` | x |  | 0 | `archive/p67-1-2d-c08` |
| `p67-1-2d-c10` | x |  | 0 | `archive/p67-1-2d-c10` |
| `p672-cube` | x |  | 0 | `archive/p672-cube` |
| `p672-d2` | x |  | 0 | `archive/p672-d2` |
| `p672-path` | x |  | 0 | `archive/p672-path` |
| `p8-02-int-local` | x |  | 3 | `archive/p8-02-int-local` |
| `p804-int` | x |  | 1 | `archive/p804-int` |
| `p804-int3` | x |  | 0 | `archive/p804-int3` |
| `p809-runs` | x |  | 1 | `archive/p809-runs` |
| `p9-03b` | x |  | 0 | `archive/p9-03b` |
| `p901-with-p904` | x |  | 0 | `archive/p901-with-p904` |
| `scratch/c1x-on-p8int` | x |  | 1 | `archive/scratch/c1x-on-p8int` |
| `wave2-base` | x |  | 0 | `archive/wave2-base` |

## Worktrees to remove (141)

`git worktree remove` plus the folder. Skipped at run time if a Claude session is still working in it or it has uncommitted changes other than line endings.

| Worktree | Branch | Uncommitted tracked files |
|---|---|---:|
| `claude-deform-rea/.claude/worktrees/angry-kilby-bee9b8` | `p405-oracle` | 0 |
| `claude-deform-rea/.claude/worktrees/awesome-dirac-04e468` | `claude/awesome-dirac-04e468` | 0 |
| `claude-deform-rea/.claude/worktrees/competent-raman-04f283` | `claude/competent-raman-04f283` | 0 |
| `claude-deform-rea/.claude/worktrees/determined-wing-88bfd7` | `claude/determined-wing-88bfd7` | 0 |
| `claude-deform-rea/.claude/worktrees/elastic-cori-259957` | `claude/elastic-cori-259957` | 0 |
| `claude-deform-rea/.claude/worktrees/elastic-liskov-14bb4f` | `claude/elastic-liskov-14bb4f` | 0 |
| `claude-deform-rea/.claude/worktrees/elastic-meitner-a81ed9` | `claude/elastic-meitner-a81ed9` | 0 |
| `claude-deform-rea/.claude/worktrees/elastic-williams-1b3b9d` | `claude/p4-02b` | 0 |
| `claude-deform-rea/.claude/worktrees/elated-tharp-d77b63` | `claude/elated-tharp-d77b63` | 0 |
| `claude-deform-rea/.claude/worktrees/epic-heyrovsky-727397` | `claude/epic-heyrovsky-727397` | 0 |
| `claude-deform-rea/.claude/worktrees/festive-tharp-204c95` | `claude/p4-02a` | 0 |
| `claude-deform-rea/.claude/worktrees/friendly-turing-bdc87c` | `claude/friendly-turing-bdc87c` | 0 |
| `claude-deform-rea/.claude/worktrees/funny-goldberg-785b77` | `claude/funny-goldberg-785b77` | 0 |
| `claude-deform-rea/.claude/worktrees/hungry-wiles-11d339` | `claude/project-thread-yhfsxj` | 0 |
| `claude-deform-rea/.claude/worktrees/kind-goodall-f8fe0c` | `claude/kind-goodall-f8fe0c` | 0 |
| `claude-deform-rea/.claude/worktrees/magical-euclid-62300d` | `claude/magical-euclid-62300d` | 0 |
| `claude-deform-rea/.claude/worktrees/magical-khorana-dcdea4` | `claude/gui-run-research` | 0 |
| `claude-deform-rea/.claude/worktrees/musing-fermi-d31c14` | `claude/musing-fermi-d31c14` | 0 |
| `claude-deform-rea/.claude/worktrees/musing-poincare-7325e3` | `claude/musing-poincare-7325e3` | 0 |
| `claude-deform-rea/.claude/worktrees/pensive-bardeen-cc7b7e` | `claude/p4-06` | 0 |
| `claude-deform-rea/.claude/worktrees/pensive-pasteur-7fcc3e` | `(detached)` | 0 |
| `claude-deform-rea/.claude/worktrees/quizzical-rubin-cde642` | `claude/plan-p5-p10` | 17 |
| `claude-deform-rea/.claude/worktrees/reverent-wing-c0e777` | `claude/reverent-wing-c0e777` | 0 |
| `claude-deform-rea/.claude/worktrees/sleepy-lumiere-55d9e9` | `claude/sleepy-lumiere-55d9e9` | 0 |
| `claude-deform-rea/.claude/worktrees/sleepy-mendel-ea9662` | `claude/sleepy-mendel-ea9662` | 0 |
| `claude-deform-rea/.claude/worktrees/sleepy-spence-b82216` | `claude/sleepy-spence-b82216` | 0 |
| `claude-deform-rea/.claude/worktrees/trusting-cartwright-a46606` | `claude/trusting-cartwright-a46606` | 0 |
| `claude-deform-rea/.claude/worktrees/vibrant-kowalevski-75b9e0` | `claude/p2-183d00` | 0 |
| `claude-deform-rea/.claude/worktrees/zealous-tharp-625a23` | `claude/p4-04` | 0 |
| `claude-deform-rea-a050` | `claude/p5-14001a050` | 0 |
| `claude-deform-rea-c04c06` | `claude/c04-c06` | 0 |
| `claude-deform-rea-c05` | `claude/c05` | 0 |
| `claude-deform-rea-c07` | `claude/c07` | 0 |
| `claude-deform-rea-c11` | `claude/p67-1-2d-c11` | 0 |
| `claude-deform-rea-c12` | `claude/p67-1-2d-c12` | 0 |
| `claude-deform-rea-c13` | `claude/p67-1-2d-c13` | 0 |
| `claude-deform-rea-c1x` | `claude/p67-1-2d-c1x` | 0 |
| `claude-deform-rea-c1xint` | `scratch/c1x-on-p8int` | 0 |
| `claude-deform-rea-censussmall` | `claude/census-small` | 0 |
| `claude-deform-rea-comparator` | `claude/comparator` | 0 |
| `claude-deform-rea-corpus` | `claude/corpus-0.3` | 0 |
| `claude-deform-rea-cubecheck` | `(detached)` | 0 |
| `claude-deform-rea-dbread` | `claude/p-dbread` | 0 |
| `claude-deform-rea-g2bc` | `claude/kw-g2bc` | 0 |
| `claude-deform-rea-g2geo` | `claude/kw-g2geo` | 0 |
| `claude-deform-rea-g2sv` | `(detached)` | 0 |
| `claude-deform-rea-gate` | `claude/project-thread-fh7qbl` | 0 |
| `claude-deform-rea-intall` | `claude/int-all` | 0 |
| `claude-deform-rea-kcprintf` | `claude/key-common-printf` | 0 |
| `claude-deform-rea-keycommon` | `claude/project-thread-4ygsdl` | 0 |
| `claude-deform-rea-kw457` | `claude/project-thread-ujpfry` | 0 |
| `claude-deform-rea-kwg1a` | `claude/project-thread-3qftn2` | 0 |
| `claude-deform-rea-kwg1b` | `claude/project-thread-2zjao7` | 0 |
| `claude-deform-rea-kwg2mg` | `claude/p4-plan` | 0 |
| `claude-deform-rea-kwg2obj` | `claude/project-thread-4e1o32` | 0 |
| `claude-deform-rea-kwg3` | `claude/project-thread-3dav7e` | 0 |
| `claude-deform-rea-kwg4a` | `claude/project-thread-f8t4ri` | 0 |
| `claude-deform-rea-kwg4b` | `claude/project-thread-hca1wm` | 0 |
| `claude-deform-rea-kwm-audit` | `claude/kwm-audit` | 0 |
| `claude-deform-rea-kwm-curve` | `claude/kwm-curve` | 0 |
| `claude-deform-rea-kwm-movctl` | `claude/kwm-movctl` | 0 |
| `claude-deform-rea-kwm-win3` | `claude/kwm-win3` | 0 |
| `claude-deform-rea-kwmerge` | `claude/kw-merge` | 0 |
| `claude-deform-rea-kwreg` | `claude/project-thread-kd6glp` | 0 |
| `claude-deform-rea-mcli` | `claude/project-thread-69qrjt` | 0 |
| `claude-deform-rea-msgunit` | `claude/p5-msgunit` | 0 |
| `claude-deform-rea-oracle` | `claude/oracle-runner` | 0 |
| `claude-deform-rea-p10-02` | `claude/p10-02` | 0 |
| `claude-deform-rea-p1001` | `claude/p10-01` | 0 |
| `claude-deform-rea-p101` | `claude/project-thread-3jrqes` | 0 |
| `claude-deform-rea-p102` | `claude/project-thread-kvhbb6` | 0 |
| `claude-deform-rea-p103` | `claude/project-thread-8pgms3` | 0 |
| `claude-deform-rea-p104` | `claude/project-thread-58tdqr` | 0 |
| `claude-deform-rea-p105` | `claude/p1-05-sdctl` | 0 |
| `claude-deform-rea-p106` | `claude/project-thread-h1h0q5` | 0 |
| `claude-deform-rea-p107` | `claude/project-thread-m7jc8f` | 0 |
| `claude-deform-rea-p108` | `claude/project-thread-x6l2gx` | 0 |
| `claude-deform-rea-p109` | `claude/project-thread-bfa515` | 0 |
| `claude-deform-rea-p110` | `claude/project-thread-069xoh` | 0 |
| `claude-deform-rea-p110r` | `claude/p1-10-rebased` | 0 |
| `claude-deform-rea-p111` | `claude/project-thread-wr4cir` | 0 |
| `claude-deform-rea-p112` | `claude/p1-12-dbcore` | 0 |
| `claude-deform-rea-p112v` | `backup/wip-p112v` | 0 |
| `claude-deform-rea-p113` | `claude/project-thread-fzevnx` | 0 |
| `claude-deform-rea-p114` | `claude/project-thread-8ke72q` | 0 |
| `claude-deform-rea-p1found` | `claude/project-thread-18pl1g` | 0 |
| `claude-deform-rea-p1gate` | `claude/p1-gate` | 0 |
| `claude-deform-rea-p2rest` | `claude/p2-rest` | 0 |
| `claude-deform-rea-p403` | `claude/p4-03` | 0 |
| `claude-deform-rea-p403o` | `p403-oracle-local` | 0 |
| `claude-deform-rea-p407` | `claude/p4-07` | 0 |
| `claude-deform-rea-p4repro` | `backup/wip-p4repro` | 0 |
| `claude-deform-rea-p501` | `claude/p5-01-follow` | 0 |
| `claude-deform-rea-p501int` | `p501-int` | 0 |
| `claude-deform-rea-p5gate` | `claude/p5-gate` | 0 |
| `claude-deform-rea-p5sk` | `claude/p5-shortkey` | 0 |
| `claude-deform-rea-p671` | `claude/p67-1` | 0 |
| `claude-deform-rea-p671-2d` | `claude/p67-1-2d` | 0 |
| `claude-deform-rea-p671-c08` | `p67-1-2d-c08` | 0 |
| `claude-deform-rea-p671-c10` | `p67-1-2d-c10` | 0 |
| `claude-deform-rea-p672` | `claude/p67-2` | 0 |
| `claude-deform-rea-p672-cube` | `p672-cube` | 0 |
| `claude-deform-rea-p672-d2` | `p672-d2` | 0 |
| `claude-deform-rea-p672-path` | `p672-path` | 0 |
| `claude-deform-rea-p673` | `claude/p67-3` | 0 |
| `claude-deform-rea-p673i` | `claude/p67-3-int` | 0 |
| `claude-deform-rea-p673o` | `claude/p67-3-stdout` | 0 |
| `claude-deform-rea-p673s` | `claude/p67-3-srcnt4` | 0 |
| `claude-deform-rea-p674` | `claude/p67-4` | 0 |
| `claude-deform-rea-p675` | `claude/p67-5` | 0 |
| `claude-deform-rea-p675f` | `claude/p67-5-fix` | 0 |
| `claude-deform-rea-p675fix` | `claude/p67-5-fix-p675fix` | 0 |
| `claude-deform-rea-p677` | `claude/p67-7` | 0 |
| `claude-deform-rea-p677-cube` | `claude/p67-7-cube` | 0 |
| `claude-deform-rea-p677-decide` | `claude/p67-7-decide` | 0 |
| `claude-deform-rea-p677-pipeline` | `claude/p67-7-pipeline` | 0 |
| `claude-deform-rea-p677-search` | `claude/p67-7-search` | 0 |
| `claude-deform-rea-p677-srcnt` | `claude/p67-7-srcnt` | 0 |
| `claude-deform-rea-p677-surface` | `claude/p67-7-surface` | 0 |
| `claude-deform-rea-p677a` | `claude/p67-7-adapters` | 0 |
| `claude-deform-rea-p67gate` | `claude/p67-gate` | 0 |
| `claude-deform-rea-p801` | `claude/p8-01` | 0 |
| `claude-deform-rea-p802` | `claude/p8-02` | 0 |
| `claude-deform-rea-p802int` | `claude/p8-int` | 0 |
| `claude-deform-rea-p803` | `claude/p8-03` | 0 |
| `claude-deform-rea-p804` | `claude/p8-04` | 0 |
| `claude-deform-rea-p805` | `claude/p8-05` | 0 |
| `claude-deform-rea-p806` | `claude/p8-06` | 0 |
| `claude-deform-rea-p807` | `claude/p8-07` | 0 |
| `claude-deform-rea-p808` | `claude/p8-08` | 0 |
| `claude-deform-rea-p809` | `claude/p8-09` | 0 |
| `claude-deform-rea-p89` | `claude/project-thread-oeb34z` | 0 |
| `claude-deform-rea-p8base` | `claude/p8-base` | 0 |
| `claude-deform-rea-p9-01` | `claude/p9-01` | 0 |
| `claude-deform-rea-p9-03` | `claude/p9-03` | 0 |
| `claude-deform-rea-p9-03b` | `p9-03b` | 0 |
| `claude-deform-rea-p902` | `claude/p9-02` | 0 |
| `claude-deform-rea-p904` | `claude/p9-04` | 0 |
| `claude-deform-rea-p905` | `claude/p9-05` | 0 |
| `claude-deform-rea-p910int` | `claude/p910-int` | 0 |
| `claude-deform-rea-pdie` | `claude/dbread-fix-pdie` | 0 |

Two special cases: the main checkout `claude-deform-rea` is on `claude/project-thread-vb3uwx`, which is in the delete list, so it is switched to `main` first (its untracked files stay); `quizzical-rubin-cde642` differs only in line endings, so it is removed with `--force`.

## Worktrees kept (15)

* `claude-deform-rea` (`claude/project-thread-vb3uwx`)
* `claude-deform-rea/.claude/worktrees/intelligent-napier-13845a` (`claude/plan-solver-harness`)
* `claude-deform-rea/.claude/worktrees/reverent-saha-cd33d7` (`claude/gui-mo-research`)
* `claude-deform-rea/.claude/worktrees/youthful-perlman-f9f4ae` (`claude/project-thread-mq7y7u`)
* `claude-deform-rea/.claude/worktrees/zen-khayyam-73660a` (`claude/project-thread-dh47x7`)
* `claude-deform-rea-census` (`claude/census`)
* `claude-deform-rea-cjcz5r` (`claude/project-thread-cjcz5r`)
* `claude-deform-rea-dbmap` (`claude/project-thread-bz5k5e`)
* `claude-deform-rea-grp2` (`claude/project-thread-j4c7t9`)
* `claude-deform-rea-kw13` (`claude/project-thread-qj8op8`)
* `claude-deform-rea-p1003` (`claude/p10-03`)
* `claude-deform-rea-p401` (`claude/dbkey-triage`)
* `claude-deform-rea-p67` (`claude/project-thread-6cvah9`)
* `claude-deform-rea-p676` (`claude/p67-6`)
* `claude-deform-rea-p677-int` (`claude/p67-7-int`)
