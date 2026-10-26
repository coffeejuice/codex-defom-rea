# DOE flow constructor binding: missing owner interface

The candidate has a stable construction identity, but the actual notifier owner
is unavailable through the constructor's current interface. No honest production
binding can be added within this task's assigned files. Preserve the Population
guard; do not synthesize registration after construction or fabricate a shared
owner from StudyIo's observed raw pointer.

This report is the only write. No test file was added: a test manually recording
witnesses after makeSubProject would exercise the existing isolated registry,
not prove a missing production constructor hook exists. No source/shared parser,
Project, Session, variable_notify, build, GUI or mutable REA operation changed.

## Actual construction point and stable identity

`src/model/subproject.cpp::makeSubProject(Project&, int type, int id)` corresponds
to normal native SUB constructor1400b5470. Its first statement allocates a
SubProject with make_unique. That object embeds its FlowChart value, so
`&s->flow` is available immediately after allocation and remains stable while
that SubProject lives. Returning/moving the unique_ptr and growing Project.subs
moves handles, not SubProject or its embedded FlowChart.

The candidate then assigns loaded/Project/type/ID fields and, for Type2, calls
makeStudy before returning. The source-qualified native constructor constructs
its own0x58 flow model through140070f10 before storing SUB Project/type/ID and
before Type2/3 manager1400d1180. Normal flow registration140071166 is inside that
model constructor. A staged candidate binding should therefore record the model
birth immediately after stable allocation, then explicitly bind its Project/SUB
context after the backlink is assigned, before Type2 study construction. This
does not establish native QObject/refcount constructor completion by itself.

Both current candidate call sites use this normal factory:

- initProject creates SUB0 through makeSubProject(p,0,0).
- moproj/read.cpp appends SUB1..N through makeSubProject after reading the
  document's subproject metadata.

**Correction/qualification of the prior audit's replacement caveat:** selected
normal read.cpp explicitly retains constructor SUB0 and appends later SUBs; it
does not replace SUB0 while reading that project. initProject itself clears old
SUB storage before constructing its new SUB0. Do not infer a skeleton receiver
death on the normal first read. Death still needs coupling to actual SUB clear,
destruction and Project storage release rather than a reconstructed current list.

The native copy-model registration140071a7f remains a distinct constructor source.
No copy constructor call is supplied by these two normal candidate factory call
sites. Keep the isolated Copy event separate until its actual caller is bound.

## Owner availability at each point

| Point | Stable candidate flow identity | Actual shared notifier/registry reachable there |
| --- | --- | --- |
| ConversionSession construction | No SUB flow yet | Session owns notificationState_ and exposes sessionNotificationStateOwner() |
| initProject -> makeSubProject SUB0 | Yes, after make_unique | No Session parameter; Project.doeIo has not been configured by convert.cpp |
| convert.cpp after initProject | SUB0 already exists | Creates StudyIo, then binds the session's existing notifier |
| read.cpp -> makeSubProject SUB1..N | Yes | StudyIo is available in normal conversion, but only observed raw notifier pointer is exposed to this constructor |
| SUB/Project destruction | Identity ends here | No callback/lease currently tells the isolated witness registry that the receiver died |

makeSubProject receives only Project/type/ID. Project has no session/notifier or
FlowReceiverWitnessRegistry owner/context field. Its unrelated operation-class
registry is not this global notifier registry. StudyIo exposes a binding setter,
a lazy sessionNotificationState() accessor, and a noncreating
observedSessionNotificationState() raw pointer. It does not expose the shared
notifier control block. Neither invoking the lazy accessor nor constructing a
shared_ptr from that raw pointer can recover the actual owner correctly.

The session creates Project storage before convert.cpp calls initProject, so a
root-owned binding can be installed in advance. The existing actual session owner
is sufficient to create the isolated registry there, but no current interface
carries it through Project to the model-construction point.

## Lifetime coupling required

Project.subs owns unique_ptr<SubProject>; each SUB owns its embedded FlowChart.
The isolated registry owns diagnostic witness nodes and may have escaped shared
leases. Deleting a SUB does not automatically destroy a registry-owned node or
mark it dead. Current SubProject has no witness/death lease or explicit destructor
hook. Merely storing &sub.flow in the registry would leave nativeReceiverAlive()
true after that candidate storage is freed.

The session's releaseProjectStorage resets its Project after releasing the
ProjectScope. Project/SUB storage can also be cleared at initProject while the
session notifier stays alive. Thus a single cleanup at Session destruction is
insufficient to establish individual receiver death. An actual SUB lifetime lease
must notify the same registry before its FlowChart storage disappears, including
construction failure/adapter cleanup paths within the chosen represented scope.
That adapter lease is not evidence of all native Qt unwind/Close behavior.

## Smallest missing interface and disjoint write set

Root must assign the shared integration files before implementation:

1. Session owns one FlowReceiverWitnessRegistry using its already-owned notifier,
   created before initProject and retained through Project/SUB teardown.
2. Project carries a noncreating weak observer of that exact registry, installed
   by the actual Session before SUB0 construction. No ownership cycle or second
   notifier is introduced; unavailable remains explicit for unbound callers.
3. makeSubProject records NormalSub construction at the stable allocation point,
   then calls bindModelContext only after the Project backlink is available.
4. Each SUB retains a lifetime lease that records death of that precise witness
   generation before model storage disappears. Registry identity/control block
   and Project context must be validated; address reuse creates a new generation.

The likely shared write set is model.hpp plus model/subproject.cpp for the
construction/lifetime observer, and conversion_session.hpp/.cpp for the actual
registry owner/pre-init Project binding. A separate small lifetime-adapter source
may carry the noexcept cleanup lease. The existing read.cpp and project.cpp calls
can continue through the common factory once the Project observer is installed;
there is no reason to populate receivers by a post-hoc subs loop. These are
interface requirements, not edits or an approved native population profile.

Adding only a shared StudyIo-owner accessor would still miss SUB0's earlier
construction and every receiver death. Passing an optional session pointer only
at the read.cpp call would likewise omit the first model. The pre-init owner path
plus lifetime hook is the smallest complete dependency.

## Exact remaining guards and acceptance

No current constructor calls FlowReceiverWitnessRegistry. The registry still does
not assign SetOprHasDoeBindings.registrations. A valid actual-session hook on an
unsuppressed signal resets flag90, then retains the exact Population refusal:

`2signalSetOprHasDoe() prefix: ordered receiver population is unavailable`

After future source binding, population closure still must be qualified for the
selected fresh construction history. A bound live witness has no backend profile,
so delivery must retain BackendWitness refusal rather than use constructor-only
null state at lazy B121 delivery. Native Report QObject/TaskInfo and public
nonempty DoeVariables guards remain independent and unchanged.

Future integration tests should exercise the actual common factory: registry
already bound before initProject; SUB0 birth before StudyIo setup; later normal
SUBs in document construction order; stable flow identity after vector growth;
explicit death on SUB clear/Project release; escaped old witness remaining dead
across address reuse; missing/foreign owner refusal; distinct Copy events; and
Population remaining unavailable until explicitly qualified. Until the owner and
lifetime interface exists, a manual helper fixture cannot stand in for those
tests.

## Static evidence receipt

<!-- Verification and source/evidence hashes appended below. -->

Verification UTC: 2026-10-24T20:29:39.889178+00:00. Static checks passed: convert ordering, common factory allocation/backlink/Study ordering, retained SUB0 reader path, absent shared registry interface, and preserved Population guard. These are current-source observations, not original runtime acceptance. No source/test file was changed.

| Current source | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/model.hpp` | `018c938fa68dabcddc4676b459e76727f127bd5c31346ba179fed7d3c963c857` |
| `source/apps/moproj2mst/include/m2m/doe_io.hpp` | `3207aaa840a4774ac2323fcef675363c08eeca75c8baec2e9b84a7bceff69192` |
| `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp` | `f6d4a46270b7d421e8a78f96db11f990de2e0d61482c01cff2d3662fbe695de4` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `39f1141d2ba59c3d624f7f2be55c50ba0e0e0c15af523b3e9b3430dae15ffbd7` |
| `source/apps/moproj2mst/src/core/convert.cpp` | `ea57d098969ab37d498314662dd8cd5829f200e73c97b3ceab191beaa05c21d5` |
| `source/apps/moproj2mst/src/model/project.cpp` | `45694a582d9462f5119000ae134bd5867ccef2f382e11bfbb8c221e1df6bea15` |
| `source/apps/moproj2mst/src/model/subproject.cpp` | `92b5361a7fd540b7c55d82f7d22e41ce21f8bce24676f484667d8dfd69f0d7a6` |
| `source/apps/moproj2mst/src/moproj/read.cpp` | `da7bb6b87bd70c771c5b0fa25a9eaec2ef6a0139684d00ce503ce0270bc5e145` |
| `source/apps/moproj2mst/src/doe/flow_receiver_witness.hpp` | `2b5f788308d2f41dc9dcaa70d8c5557d1a65a6931ee6563d5eb8f79683459152` |
| `source/apps/moproj2mst/src/doe/flow_receiver_witness.cpp` | `53ca78012fb718c7818d334cec1138306387e9d3a6461c4dfbe9f66707decab1` |
| `source/apps/moproj2mst/src/doe/variable_notify.cpp` | `592ddf00afa584c67584ea01f2e3c8fd426b3e49e381cb5ec298b314637b6c83` |

| Evidence | SHA-256 |
| --- | --- |
| `analysis/resume56-doe-receiver-witness.md` | `a0ee9e0431c03702b6af799af031ebcfd8a2077a053cb4cb06e5ddf97cec6f49` |
| `analysis/resume56-doe-receiver-population.md` | `69cc4fd68264c1328fb668bd80a5e8c9f7e94d46c947f2ef4b7c4d7bd0101232` |
| `analysis/resume53-doe-notify-integration.md` | `aa06d3bed5b49357d11966e7b250194555773ccdbfdf7fe7710fec1307c56f21` |
| `analysis/resume51-doe-variable-notify.md` | `05a5e27222dcb1df263997322b1ca676e2f9bd5306592e72ac8605e207a771b3` |
| `analysis/resume51-doe-variable-notify-model-ctor.asm` | `ece4c8d343dbf75c024c56faaeeafe921a75b31f197996c53455c429bb6e144d` |
| `analysis/resume51-doe-variable-notify-receipt.json` | `159d1e29113b720b20ba949b721bd4a9d7bb73aae31fff28c11c736a28ecf8ab` |
| `evidence/gui_mo/s1/s1-01/dc/open1.c` | `b1d91727449c591c316b7b0497996f59b47d9e72309898e6f07fd3695b6ef7d1` |
| `source/docs/re/moproj2mst/s1-01/spec.md` | `779c3a01f3bff4cb496199507f43abb1ea7414b2346a21c018193299034e02da` |
| `build/wave3-checkpoint56-verified/receipt.json` | `fee0b352d5596aaf3a5e46592b4cd0bff622b9cd2d1af7c83bd018597b1282d6` |

Token measurements are unavailable.
