# CP218 next source increment: stage Open directory decisions

## Decision

One narrow source increment is ready without CP217's original runner, GUI
control, or a same-occurrence original runtime witness: connect the existing
pure five-path collector to the candidate's actual `parseOperation` Open path
and retain its ordered decisions in per-conversion `Ctx` state. Keep this as
internal staged metadata. Do not append it to `FileEffects::mkdirs`, call a
filesystem API, or expose a public flag/API that applies it.

This advances the Open lifecycle representation while leaving the public
directory-write gate closed. It is a prerequisite for later writer integration,
not evidence that the original produced a particular directory in CP213/214.

## Exact bounded change

The helper is already implemented and reviewed in
`source/apps/moproj2mst/src/model/operation_project_directories.hpp`; do not
recreate it. In `parseOperation` (`src/model/operation_rescue.cpp`), after the
`Type` branch applies the original zero-ID ordinal fallback and
`createOperation` returns, pass the effective parsed ID and
`projectDir(p)` to that helper, storing the five ordered cleaned paths in a
dedicated Open-decision vector on the per-conversion `Ctx`. This location is
limited to operations actually constructed by project parsing; do not place
the collection in `setOperationId` or `initOperation`, which also serve other
construction/copy routes and lack this Open parse context.

Tests should exercise the parser/Open route, not only the already-covered
helper: a positive B98 operation contributes the five paths in the retained
order; zero is first replaced by its document ordinal and therefore qualifies;
negative IDs contribute none; multiple operations preserve order and
multiplicity. Assert the project tree is byte/entry unchanged and that the
decision vector is not consumed by `applyEffects` or any writer. Existing
CP217 candidate preflight already establishes that the paired `[-1, -2]`
project is accepted by candidate Open with an unchanged tree, but it does not
test or qualify original behavior.

## Evidence and boundary

The exact branch and order are source-qualified: the original ParseOperation
uses ordinal fallback only for `id == 0`; `0x140050820` enters directory setup
for `id >= 1`; `0x140051190` reaches `0x14008c630`; and the retained static
chain records the ordered `Operations`, `Objects`, `Materials`, `Equipments`,
`Lubricants` path requests. Candidate `parseOperation` already has the
effective ID, `Project`, and `Ctx` at the point where it calls
`createOperation`. `projectDir(p)` supplies the cleaned absolute project root.
CP216's helper tests already cover path normalization, ordering, repeat
multiplicity, nonqualifying ID no-op, and no filesystem mutation.

Reconcile older audits with later work:

- CP216's collector is complete and tested, but disconnected from Open; this
  proposal is lifecycle integration of that existing helper, not another
  collector implementation.
- CP214/215 qualify original outer Open/current-project/activation and DOE
  reader-I/O boundaries, respectively; neither observes the directory helper
  occurrence. CP215's filtered observer draft supplies no runtime witness.
- CP217's candidate negative-ID preflight passed and left its tree unchanged.
  It says nothing about original acceptance or the directory creator. The
  uninstrumented runner remains HOLD, so do not cite its proposed experiment
  as executed evidence.
- CP218 found no supported correction in the A55 Save difference, B98's
  root-derived KFREAD path difference, or B120/B121/B122 action-created DOE
  differences. The R300 access audit still blocks cache1 publication. Those
  results do not supersede or broaden this Open-only staging increment.

The next gate for **applying** these decisions as directory writes remains a
supported same-occurrence original witness that binds the parsed effective ID,
the `0x140050820 -> 0x140051190 -> 0x14008c630` path, ordered helper arguments,
the `0x140081afb0` attempts/results and relevant failure/order behavior to a
comparable Open. CP217's proposed filesystem comparison could only be
corroborative: even a clean control/variant result would not directly bind the
creator call. Save and accepted Close remain separate open gates.

## Scope

Source/evidence recommendation only. No source, tests, build, fixture, original
process, GUI-control session, or shared recovery state was changed.
