# Session-owned normal SUB receiver witnesses

Five source/test files are frozen for parent build. The actual ConversionSession
now provides the existing notifier owner before initProject constructs SUB0.
Normal SUB birth and death are recorded at the common factory/lifetime boundary.
Actual SetOprHasDoe population, backend delivery and public B121 remain guarded.

## Touched files and ownership

Implementation touched model.hpp, model/subproject.cpp, conversion_session.hpp
and conversion_session.cpp. Root explicitly extended ownership to subproject.cpp
for the exact common construction/death point. One new focused test file is
added. Assigned moproj/read.cpp and core/convert.cpp remain byte-identical to
checkpoint57; their existing common-factory calls already supply the proper
SUB0/later-SUB order. No shared CMake/plan or other receiver/parser file changed.

## Pre-SUB0 owner contract

ConversionSession constructs one FlowReceiverWitnessRegistry from its existing
notificationState_, retains it in the Session, and exposes its actual owner via
flowReceiverWitnessRegistryOwner(). Immediately after Project allocation it binds
a weak observer on that Project. This occurs before convert.cpp calls initProject
and before Project.doeIo exists. It does not recover an owner from a raw I/O
pointer or create a second notifier.

Project::bindFlowReceiverWitnessRegistry requires a live existing notifier owner,
rejects null/foreign/replacement control blocks and is idempotent only for the
same registry. First binding after any common-factory construction or preexisting
SUB storage refuses. An expired previously bound registry cannot silently become
an unbound construction profile. Standalone Projects that were never bound retain
their previous unbound factory behavior; they do not get fabricated witnesses or
qualified production population.

## Exact construction and lifetime order

makeSubProject allocates a stable SubProject and its embedded FlowChart, marks
construction history started, and records NormalSub birth with &s->flow before
writing the SUB Project backlink. After assigning s->project, it late-binds that
witness to the actual Project/SubProject/FlowChart, then preserves the existing
type/ID and Type2 makeStudy sequence. Thus directory effects inside the Type2
study constructor can already see the correct live model witness even before
that SUB is appended to Project.subs. Existing loaded/type/ID/study/dbName values
and constructor directory failure handling are unchanged.

SUB0 uses the same factory through initProject; the reader retains it and appends
later SUBs through that factory. Unique_ptr-list growth does not move the SUB or
its FlowChart. This binding does not reconstruct a post-hoc active-only list.
Normal registration source140071166 remains distinct from Copy140071a7f. No
normal factory call emits a Copy event.

Each SUB privately retains its exact witness plus a weak registry lease. Its
out-of-line destructor records that generation's death before FlowChart/member
storage is destroyed. Escaped diagnostic witness leases therefore become dead
on SUB clear, pending-SUB destruction or Project release. If registry scope ended
first, that registry already invalidated its witness leases. Session retains the
registry through explicit Project reset, so normal Project teardown records each
SUB death. This is adapter lifetime coupling, not full native Qt destruction,
exception-unwind or Close parity.

## Guards and frozen Boolean bridge

The registry still records witnesses only. Nothing assigns a qualified list to
SetOprHasDoeBindings.registrations. Current backend witnesses remain unavailable;
no constructor-only null is assumed at later delivery. The same actual-session
notifier hook therefore still resets flag90 on unsuppressed delivery and stops
at Population. Public nonempty DoeVariables, B121 post-commit delivery and native
Report QObject/TaskInfo guards remain unchanged.

The entire conversion_session.cpp suffix from its destructor through EOF is
identical to checkpoint57, including every frozen Boolean bind/dispatch/apply
method and releaseProjectStorage implementation. Boolean owner-scope construction
is unchanged. Only the new registry include/initializer and pre-init Project
binding were added to that implementation.

## Focused test and pending validation

Target: m2m_doe_flow_receiver_constructor_binding.
CTest: m2m.doe.flow_receiver_constructor_binding. Five cases cover:

- Actual Session registry/notifier identity before SUB0 and before StudyIo.
- Type2 directory-effect callback sees a live/context-bound witness before study
  assignment, plus stable ordered flow identities after vector growth.
- SUB clear and actual Session Project release invalidate escaped witnesses;
  replacement models retain new ordinals and old dead history.
- Same-registry idempotence, foreign registry refusal, unavailable population and
  retained flag90 reset at Population.
- Unbound standalone Project behavior, refusal of late first binding, and refusal
  after a previously bound owner expires.

Testkit main, complete OpClass include and existing CONFIGURE_DEPENDS discovery
were checked. The test uses recording I/O only; no original app or disk directory
creation is involved. Worker ran no build or executable test. Root owns grouped
build, focused test execution and required full regression.

## Static freeze receipt

Verification UTC: 2026-10-24T20:40:15.266984+00:00. Static checks passed: initializer/pre-init order, factory birth/backlink/context/Study order, death call, late-bind rejection, five cases/Testkit includes, whitespace/delimiters, unchanged assigned callers/parser guards, and exact frozen Boolean implementation comparison.

| Modified/new source | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/include/m2m/model.hpp` | `084075fefce8572de158c8dbe4d926d693e8e19ecee8f85771ef7958789d4e5e` |
| `source/apps/moproj2mst/src/model/subproject.cpp` | `6ee605503b0f3e9a04402b8fa3aeec1a3a243def1f5e4cd4b2386a65ce90ee0c` |
| `source/apps/moproj2mst/include/m2m/engine/conversion_session.hpp` | `09feedab6ca1ac321deee04eecf5ff51bd6c1b2302c396ee4de0a3af5acfcebe` |
| `source/apps/moproj2mst/src/engine/conversion_session.cpp` | `fb54fd5f79dbbe6660c0cc0284015bff8d33a8ae9495bf88de8b985b7f55a8fd` |
| `source/apps/moproj2mst/tests/doe/test_flow_receiver_constructor_binding.cpp` | `545f6803d37f5e44f70147bcf5556c4c7f721c6f88dc00c827e711b8ec994097` |

| Unchanged assigned/guard source | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/moproj/read.cpp` | `da7bb6b87bd70c771c5b0fa25a9eaec2ef6a0139684d00ce503ce0270bc5e145` |
| `source/apps/moproj2mst/src/core/convert.cpp` | `ea57d098969ab37d498314662dd8cd5829f200e73c97b3ceab191beaa05c21d5` |
| `source/apps/moproj2mst/src/doe/variable_notify.hpp` | `ebcffb6e89500157ecddce8ba43b60cc885fe8159ba5831c727e225d951d0bb3` |
| `source/apps/moproj2mst/src/doe/variable_notify.cpp` | `592ddf00afa584c67584ea01f2e3c8fd426b3e49e381cb5ec298b314637b6c83` |
| `source/apps/moproj2mst/src/doe/variable_prefix.cpp` | `648f27d48011d18a467806da90ceaab045811ff4998db8fe80e37926b47e759e` |
| `source/apps/moproj2mst/src/doe/doe_study.cpp` | `988b9599de5d47426bef8fd34554b436636e0af2932ecbd1f43e1458629532e4` |

| Evidence | SHA-256 |
| --- | --- |
| `analysis/resume57-doe-constructor-binding.md` | `4a9fd7bc3ed7005c78d7402ec3baf5db49844253027af5b3693507db81b76a8d` |
| `analysis/resume56-doe-receiver-witness.md` | `a0ee9e0431c03702b6af799af031ebcfd8a2077a053cb4cb06e5ddf97cec6f49` |
| `analysis/resume56-doe-receiver-population.md` | `69cc4fd68264c1328fb668bd80a5e8c9f7e94d46c947f2ef4b7c4d7bd0101232` |
| `analysis/resume53-doe-notify-integration.md` | `aa06d3bed5b49357d11966e7b250194555773ccdbfdf7fe7710fec1307c56f21` |
| `analysis/resume51-doe-variable-notify.md` | `05a5e27222dcb1df263997322b1ca676e2f9bd5306592e72ac8605e207a771b3` |
| `analysis/resume51-doe-variable-notify-model-ctor.asm` | `ece4c8d343dbf75c024c56faaeeafe921a75b31f197996c53455c429bb6e144d` |
| `analysis/resume51-doe-variable-notify-receipt.json` | `159d1e29113b720b20ba949b721bd4a9d7bb73aae31fff28c11c736a28ecf8ab` |
| `build/wave3-checkpoint57-verified/receipt.json` | `554bcd724904ad95c08061dbec42e73d38a6c99aab76d2dbad507ec5e8760687` |

Token measurement is unavailable. No efficiency savings are claimed.
