# Isolated flow receiver lifetime witnesses

Three new files are frozen for parent integration. This registry records explicit
constructor events and receiver lifetime; it does not install a complete native
registration population or bind the public B121 parser.

## API and state boundary

`FlowReceiverWitnessRegistry(actualNotifier, qualification)` holds a weak lease to
the supplied existing notifier. `recordNormalSubConstruction(identityToken)` and
`recordCopyConstruction(identityToken)` create distinct tagged event witnesses,
with source registration sites140071166 and140071a7f respectively. Each successful
record appends a stable node/ordinal. There is no Project.subs enumeration or
active-subproject selection. Identity tokens are explicitly supplied diagnostic
identities until an actual native constructor adapter qualifies them.

A live repeated token refuses; after explicit `recordReceiverDeath(oldWitness)`,
the same token may create a new generation. The old node remains dead in ordered
history; recording its death again cannot invalidate the new node. Foreign
registry nodes and foreign shared-control-block wrappers refuse before mutation.

`bindModelContext(witness, project, sub)` explicitly links the actual candidate
Project/SubProject/FlowChart after SUB's constructor backlink is available. It
requires sub.project to match and rejects replacement. It may precede append to
the SUB list; the existing dispatcher remains responsible for current membership
validation at consumption. Backend state always remains unavailable: the native
constructor's initial null is not carried forward as a current backend witness.

Destruction or `releaseAdapterScope()` marks escaped diagnostic witnesses dead and
clears their context pointers while retaining diagnostic history. This is adapter
scope cleanup, not a newly recovered native QObject destructor or Close sequence.
Explicit receiver death may still be recorded after notifier expiry; constructing
or rebinding new witnesses then refuses. The registry owns witness nodes, not
Projects, SubProjects, FlowCharts, QObjects, backends or native registration maps.

## Original evidence and retained guards

The source-qualified normal SUB constructor1400b5470 constructs its distinct
flow model through140070f10; normal flow registration140071166 uses the global
notifier and flag0. Copy constructor1400711a0 has a separately inventoried
registration140071a7f. Do not treat that source as a normal fresh-Open constructor.
The fixed direct-call inventory does not prove arbitrary dynamic registration
closure. Actual Qt QObject construction/lifetime and current backend binding
remain prerequisites for production use.

The registry exposes ordered witness history only. It never assigns
SetOprHasDoeBindings.registrations, changes SessionNotificationState flags/maps,
or calls the scalar reader. Actual production population remains unavailable.
Public nonempty DoeVariables guard, isolated B121 prefix's post-commit delivery
guard, and notifier Population refusal are unchanged. Manually qualified fixture
delivery using these witnesses would still require model context/membership;
a live bound receiver then stops at BackendWitness because no current backend
was invented.

## Focused tests and registration

Existing CMake globs discover target `m2m_doe_flow_receiver_witness` and CTest
`m2m.doe.flow_receiver_witness`; no CMake file changed. Four cases cover explicit
normal/copy order, fixed source sites, unavailable production population,
receiver death/history/address reuse, late model-context binding, unknown backend,
escaped lease invalidation, invalid/expired notifier scope, null identity, foreign
registry/control block and idempotent release/death. Testkit main and the complete
OpClass include `m2m/mst/writer.hpp` are present.

No build or executable tests were run by this lane. Parent owns focused execution
and combined regression. This is an isolated source increment; no shared
Project/Session/variable_notify/public B121 files were edited.

## Static freeze receipt

Verification UTC: 2026-10-24T20:24:10.719781+00:00. Static checks passed: balanced delimiters, no trailing whitespace, four cases/Testkit main, complete Project registry type include, no production registry callers, no population installation, and all three retained parser/notifier guards.

| Frozen source | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/flow_receiver_witness.hpp` | `2b5f788308d2f41dc9dcaa70d8c5557d1a65a6931ee6563d5eb8f79683459152` |
| `source/apps/moproj2mst/src/doe/flow_receiver_witness.cpp` | `53ca78012fb718c7818d334cec1138306387e9d3a6461c4dfbe9f66707decab1` |
| `source/apps/moproj2mst/tests/doe/test_flow_receiver_witness.cpp` | `31d4760e324399f5cb9321be1301c7a747ab6d0c61ceb97a50bdfca230cfb4f2` |

| Referenced evidence | SHA-256 |
| --- | --- |
| `analysis/resume51-doe-variable-notify.md` | `05a5e27222dcb1df263997322b1ca676e2f9bd5306592e72ac8605e207a771b3` |
| `analysis/resume51-doe-variable-notify-receipt.json` | `159d1e29113b720b20ba949b721bd4a9d7bb73aae31fff28c11c736a28ecf8ab` |
| `analysis/resume51-doe-variable-notify-model-ctor.asm` | `ece4c8d343dbf75c024c56faaeeafe921a75b31f197996c53455c429bb6e144d` |
| `analysis/resume51-doe-variable-notify-register.asm` | `53c002b76cc2b5ce9ab41e1d9750ac660bb4956bcc855164f3a6e5f73a11300f` |
| `analysis/resume53-doe-notify-integration.md` | `aa06d3bed5b49357d11966e7b250194555773ccdbfdf7fe7710fec1307c56f21` |
| `analysis/wave2-doe-session-notifier-call-instructions.json` | `e1a357a697a2f3fd1de4a56eee1fcbf5470cd25ef7a1bf93e32e60b957dbbd4a` |
| `analysis/wave2-doe-session-notifier-direct-key-candidates.json` | `1a947bb2daf487a9e9d8aee1999d09675f2abb6202b3e526ef1847fcbc6cf0d9` |
| `analysis/wave2-doe-session-notifier-register-binding-decompile.json` | `9aaddb2a4b4c519b81d44d6b2e6c6f7a1d0f2539e4644c632a5b96973868d271` |

Token measurements are unavailable; no savings are estimated.
