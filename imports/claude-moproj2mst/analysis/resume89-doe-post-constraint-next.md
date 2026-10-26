# CP89: DOE continuation after no-row Constraints

## Finding

There is no source-ready new child reader immediately after CP88. On the
CP88 active-owner fixture, the direct sibling after the no-row
`<Constraints>` element is `<Operations/>`. Native Constraints handling
clears its count before direct-row dispatch; with no exact direct
`Constraint` child it returns true. The outer reader then advances to the next
XML sibling. The Operations reader at `0x140912a20` traverses its children,
applies no payload mapping, and returns its initially true status. That
no-payload branch is already implemented and covered by CP83. This fixture
ordering is candidate test input, not a claim that the original retained
B121 document contains a Constraints sibling.

Native child order is the order in the input DOM, not a fixed order imposed by
the reader. The retained B121 input shows common headers, `DoeVariables`,
then `Operations`, and contains no `Constraints`. Thus there is no universal
“tag after Constraints”; the next sibling is input-dependent. The supported
no-row rule is: clear on the encountered Constraints element, ignore
nonmatching direct children, return true, and continue with that element's
next sibling. The outer variable-number finalizer runs once only after all
top-level child readers finish normally.

## Lazy and Save continuation

For the CP88 actual-owner fixture's `DoeVariables`, region names,
no-row Constraints, then Operations order, normal child-reader completion
continues the existing lazy Study load. The fixture observes ordered LHS,
FF, USR sample handling, followed by the current `DEF_DOE.PAR` read, on the
same Study owner. This is source-backed candidate continuation; it does not
establish original GUI/runtime execution or constraint-record behavior.

Same-owner public Save serializes the admitted empty/no-row model into the
immediate InputTable XML output and reaches the refresh boundary. It then
stops at `DoeTableOwnerState::requireRefreshDelivery` because actual
main-window `+0x458` / timer/UI receiver delivery is unavailable. The old
`Sub_Project` DOM is retained and the final project write is not reached.
Public Save completion therefore remains blocked at this refresh owner,
independently of Constraints dispatch.

## Next action and file ownership

No new source/test files are justified for Operations or the no-row
Constraints route; both behaviors already have native and candidate coverage.
The next useful public Save step requires a source-qualified actual refresh
sender/receiver/timer lifetime and return contract. Until that owner is
available, no new public output branch is reachable without inventing a
GUI receiver. Recognized direct `Constraint` rows still need real record
storage/manager `vt+8` append ownership and remain guarded. No exclusive
source/test write set is recommended from this CP89 read-only pass.

## Evidence

Original GUI SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

* `analysis/resume88-doe-constraint-dispatch.md`, SHA-256
  `d03929659377c119b0d9dc9d0a3c6e2271f05546b67c0f35245551d3aaeaa554` —
  exact Constraint tag/clear/no-row behavior and CP88 fixture continuation.
* `analysis/resume83-doe-operations-source-review.md`, SHA-256
  `077873c8b590c4a0a32f34c955d1cb11f3e2d4cde4c605e9e91a30a3572e0273` —
  native Operations no-payload result and lazy/Save continuation.
* `analysis/wave2-doe-lazy-input-empty-readers-decompile.json`, SHA-256
  `8bbbb410969e3f95a33aa37eb123518c372f55d869e0ab21de9bbc39aab9c2bb` —
  original `0x140912d10`, `0x140912a20`, and outer reader evidence.
* `analysis/resume49-doe-variable-parse.md`, SHA-256
  `abb8440dbeae34b01156508cf85785241c6e4917e717cdb336a53629c570b614` —
  retained B121 XML child ordering; no Constraints element in that ordering.

This was a read-only source/receipt review. No source, build, test, GUI,
original application, or mutable REA database was changed or run.
