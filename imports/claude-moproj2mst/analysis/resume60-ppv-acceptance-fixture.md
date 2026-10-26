# Checkpoint 60 PPV acceptance fixture search

## Result

No retained case was found that satisfies the requested comparison: a local
`SimCtrl.PPV` input plus the corresponding original Save output. The recursive
read-only searches found no `.PPV`/`SimCtrl.PPV` file under either the checked-in
`evidence/gui_mo` tree (including `oracle_b`) or the retained prior
`deform-rea-work/gui_mo/oracle_b` path. Searches of the analysis receipts and
current tests found writer/reader contracts and synthetic owner fixtures, but
no original PPV file bytes or original PPV Save output receipt.

The 93/5 historical full-save cases do not substitute for this fixture. Their
retained reports identify A55, three post-interaction DOE comparisons, and B98;
none records a SimCtrl.PPV path. The recent root-only test loads a read-only
Oracle-B project and injects a root recovery point in memory. It has no local
PPV point or physical PPV input/output, so it characterizes only the
local-empty writer gate.

## Closest source-qualified fixture shape

The retained source contract does define what a future input should contain:

- one selected operation with an existing `SimCtrl.KEY`, so Open reaches the
  PPV branch rather than the KEY-missing inheritance return;
- a local point record in operation OPI `+0x20` (`recoveryB`);
- for the predecessor/shared branch, the active data-root list at `+0x19a8`
  and an operation flag list at `+0x28` covering the shared indexes;
- an operation task-folder `SimCtrl.PPV` file whose rows exercise the selected
  local/shared routing; and
- a raw original Save output tree captured after the same input and action
  boundary, including `SimCtrl.KEY`, `SimCtrl.PPV`, and the project/container
  files whose state may be affected.

The relevant static bases are `analysis/resume50-ppv-reader-model.md`,
`analysis/resume54-ppv-owner.md`, `analysis/resume57-ppv-count-model.md`, and
`analysis/resume59-ppv-save-next.md`. These qualify list roles, parser/writer
ordering, and serialization fields; they do not supply a project instance or
runtime result.

## Exact missing evidence

To promote an imported project into the requested historical acceptance fixture,
the retained evidence must include all of the following, bound together:

1. The pristine input project tree and hashes, including the selected operation's
   `SimCtrl.KEY` and local `SimCtrl.PPV`, plus the exact task-folder path.
2. A retained original Save output tree/hashes from that same input state,
   including raw `SimCtrl.PPV` bytes and `SimCtrl.KEY` bytes. An unrelated
   post-GUI project tree or a payload calculated from the static writer is not
   an original Save receipt.
3. The selected subproject/operation identity, predecessor condition, and the
   populated local/shared/flags values or a source/runtime receipt tying the
   on-disk input to those owners.
4. Original executable and Qt module identities, plus the actual Open→Save
   outcome and output path. Static decompilation alone does not establish that
   this fixture was opened and saved successfully by that original process.

No checked-in files or prior receipts provide this binding. A synthetic local
point added to a disposable imported model can validate the candidate adapter
and source-derived byte format, but without the original run/output it remains
candidate/unit coverage, not public-lifecycle acceptance.

## Gate and next action

The existing GUI release assessment still reports `release_unverified` and no
supported release operation plus verifiable status. This task used no GUI
control. Consequently a new original Save capture cannot be used to fill the
missing evidence until the standing app-control release gate is independently
resolved.

The next safe action is to preserve the current limitation and continue
candidate-only PPV adapter tests/build validation. If an immutable archived
original PPV project/output pair is later located, first hash and qualify the
input/output tree, then compare a fresh candidate conversion in a disposable
copy against those retained bytes. Otherwise the exact missing deliverable is a
source-bound original Open→Save capture for a project with a local PPV file and
both local/shared ownership state recorded. Do not label a generated synthetic
PPV row as an original fixture.

## Scope

Read-only inventory only. No source/CMake edits, builds, GUI interaction, or
mutable REA work occurred.
