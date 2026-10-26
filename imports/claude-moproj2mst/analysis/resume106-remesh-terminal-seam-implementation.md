# CP106: private failed-open terminal service seam

The remesh writer now has an optional source-private service route. The existing public function delegates to it with nullptr, retaining the legacy guarded failed-open behavior and public declarations/layouts. No core conversion caller, product CLI, C ABI, worker or terminal harness is wired to this seam in CP106.

## Changes and source order

NEW `src/engine/simcontrol300_remesh_services.hpp` declares the private profile/site/proof enums, fixed value request, pure profile classifier, dispatch-failure tags, borrowed service interface and sibling writer entry point. The request identifies first null-FILE fprintf0x1400abcf0, ordinal1, native fresh-default startup proof, modeled errno22 (EINVAL/0x16) and expected process status0xC0000417. It contains no returnedCode or returned Save/Close claim. Root's future wrapper owns the live SaveCallState snapshot; this leaf does not duplicate or retain Session state.

`saveRemeshAtProjectSaveWithTerminalService` shares the existing writer body. Directory preflight, disabled removal, enabled locale preflight, stack CoggingFile open and successful31-write/Close order are unchanged. Only the enabled failed-open branch consults a nonnull service's profile. FreshDefault constructs the fixed request and invokes dispatch while file/writer/Session scopes remain live, before returning a report or throwing. Unknown, Custom and invalid profile values return the existing unsupported report without endpoint dispatch or a modeled errno assignment. Null service performs the original branch. The public wrapper therefore retains its previous source contract, including errno behavior.

Immediately before admitted dispatch, the writer sets errno to request.crtErrno. The service's dispatch method is deliberately allowed to return a failure tag; it has no C++ noreturn promise. PublicationFailed and EndpointReturned are preserved; any other returned tag is normalized to InvalidFailureCode. On any return, the writer calls failClosed exactly once for best-effort harness-failure publication, then parks in a defined infinite Sleep loop without stack unwind, ordinary Save continuation or successful completion. Even if failClosed returns, the parked state remains. The supervisor must enforce its external deadline and stop only its owned opted-in worker. PublicationFailure/EndpointReturned/parked timeout must never classify as source-terminal success. Backend methods are noexcept and must not throw; no host invalid-parameter handler or null FILE printf is used.

This fail-state is candidate execution policy, not a recovered native wait. The native qualified fresh-default route terminates; its debugger/context/filter/debug-flag prelude is unimplemented. Any future endpoint receipt must retain `candidate-fixed-endpoint; native-default-handler-prelude-unimplemented`. Nothing here admits a custom-returning native handler or enables killing embedding C ABI callers.

## Focused cases

NEW `tests/mdt/test_simcontrol300_remesh_terminal_contract.cpp` contains seven Release-active Testkit cases:

1. pure FreshDefault descriptor site/proof/status/errno/ordinal with no errno change;
2. Unknown/Custom/invalid profile rejection;
3. real enabled write-open sharing denial with Unknown/Custom/invalid service, preserving original guard/sentinel/errno and observing profile once with no dispatch;
4. same denied-open public versus null-service sibling compatibility;
5. disabled removal before locale/service admission, even with FreshDefault service;
6. enabled unknown-locale refusal before open/service admission;
7. real successful FreshDefault-service writer output identical to the public writer, without querying or dispatching the service.

The test double has counters solely for branches which must not enter dispatch. No case invokes a FreshDefault service on failed open, executes an endpoint, parks or terminates its Testkit process. Actual admitted dispatch/failure-state observations remain dependent on the separately reviewed owned-child worker/protocol/supervisor. The deterministic test denial permits read sharing and denies writes before truncation; unchanged bytes are asserted only for that fixture.

Target: `m2m_mdt_simcontrol300_remesh_terminal_contract`; CTest: `m2m.mdt.simcontrol300_remesh_terminal_contract`. Existing configure-dependent mdt glob registers the test without CMake edits. Required later regression includes existing31-attempt post-open failure coverage and all CP98–102 reader/errno cases, plus the future isolated vertical cases before any integrated terminal activation.

## Static preflight and frozen release

Compared the owned existing source against CP105 verified source.zip: its baseline SHA is39249f84faa6bef97e53504b7fb22f632a539d42cc7b5270a7bd3c553321814f. Only the added include, guarded dispatch helper, public/null wrapper, private classifier/sibling and failed-open service branch differ. All quoted includes resolve against source/test local directories or actual app include/src roots. Testkit include exists; one main calls run_all; all7 cases use supported CHECK/CHECK_EQ/REQUIRE; no C assert or REQUIRE_EQ; delimiter/whitespace checks pass. No compiler/build/CTest/executable was invoked. These are static checks, not passing-test claims.

Frozen files:

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| src/engine/simcontrol300_remesh.cpp | a865bf7aead8ce3b74f7d670340125538cf7903381b9335d851f55dd2bf4c228 |
| src/engine/simcontrol300_remesh_services.hpp | 1245df3d36fe7d7e16a40d201f459adaa9a74914eac71d5e826bee6fb619b585 |
| tests/mdt/test_simcontrol300_remesh_terminal_contract.cpp | c5daadea68b713b171395cc5ab4deaa7584b43d754f8490a38abf95508e3e807 |

Evidence: CP105 scope SHA2f92c1302e8100d2ad7a7634ca609407564c4e607c3cc99a29ea0a3d19fdd663; CP104 harness design SHA400e282d2d5f7cdab1c8421b21c53d0c7f38eeb0dd5646b46c483f2cb71388b2; native startup contract SHAd53cfdfedf9d4fe8ec2fd8941739ed48f12e6173db6d1c1b805a9cffbc1f343b. All original terminal claims remain static fresh-default/no-subsequent-handler qualifications, not fresh observed original termination.

Only the three approved source/test files and this receipt were written. Public header, shared core/services, C ABI/CLI, CMake and runtime files are untouched. No build/test, GUI, original execution or mutable REA activity occurred. Files are frozen for root build/review. Next dependency: root-owned live saved.call forwarding plus opt-in worker/backend/protocol and sticky-error deadline-bound supervisor; until then this is an isolated source prefix, not recovered public process-terminal behavior. Selected palette/public editor guards remain unchanged.
