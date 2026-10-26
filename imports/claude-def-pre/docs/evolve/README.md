# Evolve-mode records

One file per evolve-mode change (`AGENTS.md` section 2): an optimisation, a module swap, a new
API, or a fix of DEFORM's own behaviour. The faithful build stays the baseline; this file is
where every difference from it is written down and accepted. Name it `<topic>.md` (for example
`dxf-reader.md`) and copy the template below.

```markdown
# <topic>

* Status: proposed | in progress | accepted by Aleksandr on <date> | dropped
* Module(s): port/<module> (card: port/<module>/MODULE.md)
* Switch: CMake option `DEF_EVOLVE_<TOPIC>` (default OFF) or target `<name>`
* Branch / commits:

## Why

What the change improves and for whom (speed, memory, a new capability, a fixed DEFORM bug).

## What replaces what

The faithful code path, the new one, and the interface both sit behind. For a third-party
library: name, version, licence, URL, how it is built (pinned source in the superbuild).

## Oracle results

The tests run on both builds (tier 1, tier 2, tier 3, e2e, the card's oracle tests) and the
counts. Every difference from the oracle, one row each:

| Case | Difference | Cause | Accepted |
|---|---|---|---|

## Perf

Before/after from `.claude/skills/profile` (machine, commit, build type, input, runs, min/median
wall time, peak memory). Copy the final numbers into the card's "Perf baseline".
```
