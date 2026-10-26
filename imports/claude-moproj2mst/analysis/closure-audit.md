# Converter closure audit

The selected baseline is not closed over the requested Open→Save→Close lifecycle. The first small correction is the parsed-diagram connector check (C01); it affects chain selection and therefore MST and saved Connectors without requiring a broad redesign.

This report uses retained static sources only. No original process, REA session, build or converter was run. Original addresses identify hypotheses tied to qualified decompilation; they are not execution observations. The JSON preserves source hashes, every inherited non-ported row, packet disagreements, discovery sites and divergent snapshot file hashes.

The main ledger has 1765 rows: 116 library, 54 out-of-scope, 1566 ported, 29 spec. These statuses are inherited author assertions, not audited end-to-end completion. The old reach.py seeds Open/read/walk, main MST and operation slot +0x280. Save and Close entry roots are absent; closure.py scans E8/E9 bytes without instruction decoding and omits indirect targets.

## First correction: connector validation

The current parser connects any valid target index. The retained L5S change at 1671c942a297f590ee63acb93a9fe87581543929 adds class-flag connector types and compatibility checks. Original 0x140075050 calls model slot +0x88 before clearing either connector ([75050.c](../evidence/gui_mo/L5d/75050.c)); 0x140074f50 calls input connector slot +0x70 ([74f50.c](../evidence/gui_mo/L5d/74f50.c)); the compatibility body is [conn2.c](../evidence/gui_mo/L5d/conn2.c). Resolve its literal addresses against the preserved original before treating string labels as established.

The L5S helper is partial: after type acceptance, 0x140074f50 accepts an unlinked input, but an already linked input requires an already linked output and matching endpoint-index tests. The parse loop in sub0.c lines 925–948 attempts each fresh item output once after all items are constructed (diag0.c lines 424–427). Earlier edges can occupy that item's input, but cannot occupy its output. Thus this parse slice needs the type predicate plus rejection of an already occupied target input; the general reconnect branch is outside the correction. Constructor flag branching is retained in 82760.c from line 264, including the asymmetric port path. Default dialog actions are irrelevant to this parser check. Retain current cellless placement, item refresh and database-dependent view height; replacing the older snapshot file would remove later recovery.

Use the real parser in p03/test_subproject.cpp for equal, incompatible, bridging and Convert2Dto3D asymmetric port types; exercise duplicate targets and link replacement order. Keep p03/test_order.cpp as chain-order regression. Freeze the candidate and obtain original-first mixed-dimension and duplicate-link fixtures before claiming parity. A successful correction closes this slice only.

## Reachable lifecycle and shared engine

convert.cpp constructs a Project/registry, parses and activates subproject 0, loads the active operation, saves current state, optionally saves the project, visits property-save methods, generates MST and finally applies deferred writes. The original static Save source instead performs direct stage writes and continues after some failures; see [save-semantics.md](save-semantics.md). Open-time effects are also deferred, so a later refusal can erase effects the original already made. Recover writer-specific order and status semantics before changing the generic write helper.

RAII restores the Project TLS pointer and current-directory scope; GuiModel owns its engine step and DbHandle owns a reader whose destructor resets allocated state. Those source observations do not establish original Close behavior or engine-global reset. The engine mutex serializes calls but cannot reset persistent version state, GUI variant flags, shared Fortran units/caches or exceptional TLS state. The original Close root remains unknown. Repeated-call tests are a way to prove isolation of each single-project call; accumulated cross-project original state is outside the acceptance boundary.

CMake links defrt, defcmd, def_pre_core and transitive model/key/database libraries while setting DEF_PORT_SKIP_TESTS for those dependencies. The GUI sim-control initializer contains explicit differences from its DEF_PRE twin. Shared function/twin closure therefore requires its own address/layout/initialization map and tests. Lexical shared-source sites in JSON are discovery candidates only.

## Prioritized backlog

| ID | Priority | Work | Qualification |
|---|---:|---|---|
| C01 | 1 | Restore original connector validation before linking parsed diagram items | confirmed port omission; preserved fix is partial |
| C02 | 1 | Recover Save file ordering and each writer failure contract | confirmed fidelity difference; correction requires writer-specific recovery |
| C03 | 1 | Finish geometry bounds and scene state used by active-operation Save | explicit refusals on relevant dependencies |
| C04 | 1 | Trace close/destructor and shared process-global state for isolated repeated conversions | partially reconstructed ownership; lifecycle closure unproved |
| C05 | 1 | Rebase historical tests onto fresh single-project oracle qualification | historical validation cannot establish current scope |
| C06 | 1 | Reconcile main and packet ledger into actual Open→Save→Close graph | inherited inventory is not complete reachability |
| C07 | 2 | Restore missing operation property-save dispatch and MST-time model updates | static-source omission candidates; call-chain verification required |
| C08 | 2 | Account for DOE/optimisation branches in full headless scope | explicitly unsupported; no scope waiver accepted |
| C09 | 2 | Recover remaining class-specific Save/property files and defensive refusals | explicit refusal families; heterogeneous contracts |
| C10 | 2 | Recover Cogging multiple-reheat and failed-open behavior | explicit refusal plus intentional error-policy deviation |
| C11 | 2 | Resolve default-dialog branches and strict-warning behavior | adapter/refusal policy distinct from recovered original |
| C12 | 2 | Preserve alternative working files without overwriting newer baseline | same-head divergent snapshots; not automatically integrated |
| C14 | 2 | Prove reused DEF_PRE dependencies match GUI twins | shared engine closure not validated by converter-only tests |
| C13 | 3 | Account for silent XML/property/GUI omissions and inherited waivers | bounded discovery candidates; not all defects |

Each JSON backlog item includes original addresses, source/evidence links, dependencies, explicit unknowns and focused validation. Priority 1 items block broad fidelity claims; priority 2 items complete function families and contracts; priority 3 is bounded omission triage. No priority implies permission to redesign original behavior.

## Preserved alternatives and validation limits

The preserved charming-goldwasser-426a42 and funny-bun-e8c703 working copies both point to 9f1283895be488a1e31ebd47637c06b06ad80f6d but differ in 33 files (exact paths and SHA-256 pairs in JSON). Their snapshots and the L5S connector change remain preserved; neither is an automatic overlay.

The current baseline checked 11 historical MST-only cases: nine raw matches and two waiver-adjusted passes. It did not validate full Save, fresh original sessions, Close, nonzero successful ABI calls through every binding, or all shared engine tests. Historical 16/49 Save matches and 0/49 fresh-corpus resave matches belong to earlier binaries. Qualify reference session history, preserve candidate freezes, and record every refusal, mismatch, waiver and unavailable expected result.

Silent-omission candidates include BooleanProperties save dispatch, Convert2Dto3D/CopyMirror MST-time property-to-engine calls, Convert status files, recovery-list filling, XML grammar, UI-derived state and the inherited group Expanded policy. These require original call/branch evidence; comments or missing strings alone do not establish a defect. Explicit refusals include geometry-list boxes, scene subobjects, perspective projection, DOE variables/constraints/studies, HeatTreat records and Cogging reheats. Their exact site lists are retained, but a site count is not a scenario denominator.

No DMO build or deployment belongs to this converter audit. No source or preserved snapshot was edited.
