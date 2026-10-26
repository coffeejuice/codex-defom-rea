# CP102 operation/property graph alias audit

## Result

I found no new source-qualified public operation/property dispatch increment beyond the already integrated Save property writers and CP96–97 reader/geometry branches. The nearest apparent alias candidate is a duplicate `BooleanData`/`BooleanProps` reference inside `Project.operations`, but retained source makes it a malformed adapter graph, not a recovered native input branch. A project-local rejection test is possible; adding that guard would harden an artificial graph rather than advance a reachable import/save contract, so I do not recommend presenting it as recovered behavior.

## Graph facts and dispatch

`Operation::props` is a `shared_ptr<Props>` and `Props::owner` is the represented OPI backlink. Candidate `setOprProps` assigns a property object to one operation and immediately writes `props.owner = &op`; `createOperation` calls the registered class factory to create properties for the new OPI before attaching them. The normal operation-rescue path likewise calls `makeProps` and then `setOprProps`. The step-4 Save loop walks the complete `Project.operations` vector and dispatches each row's property writer in row order. That is full candidate graph state and has no hidden session-owner lookup, but the normal producer path creates a per-operation properties object rather than sharing one.

Retained native Save evidence binds the operation property slot to the current operation's properties object, and the native properties owner backlink is a `QPointer` to its OPI. The Boolean helper evidence in `s1-04/spec.md` resolves the owner through `BooleanData+0x10` to `BooleanProps`, then the properties owner to the operation. These facts support checking identity consistency; they do not establish native behavior for two operations or two properties objects sharing one `BooleanData`. The candidate's native Boolean side-file helper already validates the supplied Project, operation membership, operation/property/data identity, and guarded OPI lifetime. It does not count a second project property sharing the same data.

## Why the alias is not a recovered public branch

`BooleanClass::makeProps` constructs a fresh `BooleanProps` and fresh `BooleanData`; the Boolean BWZ reader allocates data only when that one properties object has none. The ordinary operation construction and rescue routes attach that result to one OPI. The retained PE/spec evidence does not show a normal parser, Save, or dispatch route that aliases one `BooleanData` across separate properties owners. Directly assigning the same `shared_ptr` to two public C++ model rows is mechanically possible, but it is not evidence that a user project or public conversion route can produce it. Adding a project-wide uniqueness refusal would define new malformed-model handling, not close an omitted native dispatch arm.

The broader operation Save inventory is unchanged: CP88/CP98 already account for base Save, property `vt+0xb8` families, and the guarded DoeOutput/Report paths. Those latter paths still require their missing actual refresh/DBLIST or document/viewer owner state. CP101's static Boolean callback order proves only intra-emission dispatch order; registration/list snapshot and receiver identity at the first Open boundary remain runtime dependent. It does not expose another owner-free source increment.

## Narrowest safe next step

Do not change the public dispatcher or relax a guard from this audit. If root wants malformed-graph hardening, the smallest isolated candidate is the already proposed CP90 `nativeBooleanSideFileSequence` Project-local uniqueness check plus a two-operation shared-data negative test. Treat it as a separate defensive-contract decision, not as an original-compatible workflow increment. For a positive operation/property integration step, the exact missing witness remains a native path that reaches a not-yet-implemented property/class slot with its complete operation/property/owner state; no such slot is established by this bounded review.

This report is read-only. No REA database, GUI/original execution, source, build, or test was touched.
