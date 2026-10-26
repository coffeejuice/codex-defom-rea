# CP179 connector implementation: private +0x08 zero

Implemented only QtCore RVA `[0x13120b,0x13120f)`, end exclusive. `SparseQObjectPrivate::private08` is an initially disengaged optional uint64 observation; the accepted CP178 predecessor records zero in the same retained private allocation and advances to `BeforePrivateZero10`. The adjacent private +0x10 store remains unmodeled, with no field materialized for it. This is distinct from the later public task +0x08 pointer backlink.

## Independent static evidence

Read the bounded retained initializer and public-constructor disassembly and independently parsed the shipped PE section table. `.text` RVA 0x1000 maps to raw offset 0x400. The selected cut at raw 0x13060b is `48 89 7b 08`, SHA-256 `0ff59178935f4e42602274d216887da3888a7055509d5c9caff6c34bf579ed7e` (four bytes). The next instruction is `48 89 7b 10` at RVA 0x13120f. RVA 0x131178 retains incoming private RCX in RBX; RVA 0x1311b5 clears EDI, which zero-extends RDI. Inspection through RVA 0x13120b found no intervening RBX/RDI write or call on the qualified equal branch. CMP EDX,0x40804 at 0x1311f1 and JE at 0x1311f7 skip the excluded mismatch call. The public caller supplies EDX at 0x13140f and calls at 0x131417. This proves the selected static effect, not live occurrence or all caller paths.

## Admission and preservation

The new stage rechecks the retained Report ordinal, allocation identity and 0xc0/0x88 sizes; written task/private address points; exact live root ownership; all four owner-and-pointee matched QList/QString/QVector bindings and committed markers; all six preceding private zeros; and the module-qualified public caller version argument/load/call. It requires private08 to be unknown. Every check precedes the only new sparse write. Missing/wrong provenance and already observed private08 refuse. Earlier mismatch refusal remains unchanged. Terminal same/different key reentry refuses at RVA 0x13120f without replay.

No root reference deltas or observations, maps, allocation counts, bindings, markers, identities, completion flags or backlinks are changed by CP179. No absolute shared-null count is inferred.

## Tests prepared and static checks

Migrated ordinary first-call and continuation stop assertions to BeforePrivateZero10 / RVA 0x13120f. Earlier supplied sparse fixtures reset private08 when reconstructing their predecessors. Added five CP179 focused cases: fresh unknown observation; sparse CP178 predecessor with independent zero deltas and no absolute counts; a ConversionSession pairing with unknown-to-zero transition, arbitrary symbolic history, optional observations, full predecessor preservation and repeated same/different-key refusals; a malformed-frame refusal matrix including already observed zero/nonzero private08 and complete before/after snapshots; and missing/foreign/aliasing root-owner refusals.

Checked unique test names and the bounded source stop/admission shape statically. No build, test execution, candidate freeze, original comparison, deployment, GUI, app-control or live/mutable REA operation was performed. The parent owns execution and acceptance gates. No shared plan/state file was changed. Source ownership of the three assigned files is released with this receipt.

## SHA-256 receipts

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/include/m2m/doe_task_runtime.hpp` | `0dda6382a161822eaedd510c08e91b2a7231bc3a31a83c6bfeee19489065128e` |
| `source/apps/moproj2mst/src/doe/task_factory.cpp` | `ea7b2db9ac979e51b6f196360fc98d5ec406d34c52b439ed3a886c44fa0534b0` |
| `source/apps/moproj2mst/tests/mdt/test_doe_task_factory.cpp` | `8ea82fda1aa489da24361d172e6b230fa362d35531d48ca0aeea8871b06ef6de` |
| `analysis/resume179-connector-next.md` | `10e55a3a08500371bb824c095928ff58db3a2f5917f30f14ba7a0c887efa6036` |
| `analysis/resume163-connector-qobject-67131160.asm` | `006a86bf080b956ccfa0885cb2a20313a0c69a198648dd770ea93caf82527aad` |
| `analysis/resume163-connector-qobject-671313d0.asm` | `6c8b6eb8379080b7790c812f50d7f523891366e518c94e5a38652a39f798dee9` |
| `evidence/original-binaries/QtCore4_64.dll` | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
