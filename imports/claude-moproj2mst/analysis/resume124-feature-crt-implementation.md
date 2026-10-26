# CP124 isolated candidate CRT producer: source release

The isolated private helper now executes the retained producer's Windows call sequence in an explicitly opted-in test process. It records candidate kernel results, candidate handle-slot publication and the bounded producer return, then stops before unported startup work. It supplies no Project binding, allocator interface, default hook, proxy block or native cleanup.

Only the three reserved NEW files were created:

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| src/model/feature_crt_runtime.hpp | `6af8c9839e8626b8e2461754a585b6518eef24b6306a5f4a802d908d91d7d220` |
| src/model/feature_crt_runtime.cpp | `f33a1d43fee15ac580f087ca9135eed85dd69a9466a83bdd9fb575e75a07e8e0` |
| tests/model/test_feature_crt_runtime.cpp | `a5ad8b00c98ccd9d65392ad1346cac515122ab14a6e0cd3d52cc46a31498750a` |

Before source creation, discovery checks confirmed all three paths were absent; the existing CONFIGURE_DEPENDS core source glob includes src/model cpp files and excludes only cli/dll. The model test glob supplies m2m_model_feature_crt_runtime / m2m.model.feature_crt_runtime. Existing standalone targets and p11 compile/include the old storage cpp, which gains no runtime reference or include. No CMake/interface request was necessary. CMake read-time hash was `582ebc7f8e015a9a2cf8834d3e28f1b650073288bf8712093b8bffea694368b2`.

## Executed prefix when root runs the test

FeatureCrtRuntime::prepareOwnedTestProcess is the only factory. It prepares all host metadata first, obtains the candidate current process/containing-code-module identity and pins that module for this bounded experiment. This is candidate lifetime instrumentation, not an original CRT API call. Its deliberately process-retained shared owner has no static destructor or reciprocal ownership cycle. Merely linking/loading this source performs no kernel producer call; default CLI/C ABI/Session paths do not call it.

attemptProducer serializes observation/execution with a host SRW lock. It calls actual HeapCreate(0,4096,0), records return and publishes the actual handle into its candidate Storage member before null handling. Null produces return0 and guards before the original caller's fatal-mode read141470cac/1446ffb58. It calls no failure diagnostic, host new-handler or later startup. Nonnull calls HeapSetInformation(handle,HeapCompatibilityInformation,&DWORD(2),4), with static checks that the enum class value is0 and DWORD is4 bytes. It retains the raw BOOL but always records producer return1, matching141484842. It guards before later startup141470cce/141481708. Repeat attempts return the same receipt without a second HeapCreate.

The report's original binary hash and original slot address are static source-role qualifications. Its origin=ActualWin32, identities, calls and candidateSlotPublished refer exclusively to the candidate test process/module and candidate storage. No original EXE global or live heap was read. Independent review caught the initial sourceSlotObserved name, which overclaimed that distinction; the final source renames the field, counts, effect and storage to candidate terminology and adds an explicit identity comment. Only naming/comments changed in that repair.

The actual heap stays retained until the owned test process ends; there is no HeapAlloc8, HeapFree, HeapDestroy, GetProcessHeap fallback or inferred CRT/native teardown. No Project/epoch is retained or bound. Final OS reclamation is not a native Close result. This explicit test-only policy is unsuitable as an unqualified reusable-library lifetime contract; default/public conversion remains unbound.

## Tests and qualification

Five Release-active Testkit cases distinguish declared observations from real candidate kernel execution:

1. Declared null result is published before producer0/fatal-mode guard; compatibility is absent.
2. Declared compatibility false preserves a nonnull modeled slot and producer1, with exact effect order.
3. Declared raw BOOL17 stays raw; missing compatibility stays incomplete and contradictory null-plus-compatibility input refuses completion.
4. Actual candidate process/module preparation starts with no publication; one real producer call records both kernel outcomes and subsequent observations do not replay it. The test prints the actual process/module/call/compatibility/return receipt.
5. Dropping local inspection leases leaves the same explicitly retained runtime available until process end.

The pure declared projection cannot construct a runtime, supply a qualified handle or become a Project binding. Real null HeapCreate and real false compatibility are not forced or claimed by the declared cases. The actual kernel-positive test requires a successful allocation in its owned process; environmental failure will fail that case with its guarded receipt rather than be relabeled success. No native startup completion, proxy allocation, original global observation or native Close completion is asserted.

Static receipt analysis/resume124-feature-crt-static.json SHA is `0302ca3815e6bd50dfcb1875861383640c929fd500e63461233b7c4949f5a352`. It records source discovery, exact kernel arguments, candidate store-before-branch, resolved quoted includes, balanced delimiter counts, supported Testkit macros/entrypoint, absence of C assert, absence of model/Session includes, absence of allocation/free/destroy/fallback heap APIs and absence of any factory reference outside the three new files. Public model.hpp remains `6ee1d43bf8112ba37d224e3f626ac88d99ca6a89f4835169d8d13242a13c65b6`; existing storage cpp remains `af2e1da0fe4bdc582e29731b2f0e6ce7d7f0a882ea2ade477f164807da1199e8`.

Source evidence remains retained original DEF_GUI_MO_64 v11.0.2 SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; bounded producer ASM `a9efa00fe5209dffb9572fbeb0b604120b79a7a42d410255a199d00d0ca098bd`; caller ASM `dce84f25fc57bf7a407e23126fb051e9bd1f6b59dc24aa5bb0a598682836c481`. CP123 design `d81523343047e4efdf228eb0c69120017ff094169e744a135d0da4e53c83ba4d` describes the still-missing public startup, first-constructor-entry and unresolved-proxy retirement prerequisites.

All three source files are frozen and released for root's grouped build. This lane performed static checks only: no compiler, build, CTest, live HeapCreate, original process, GUI or mutable REA execution. Root owns observed runtime/compile outcomes and source-stable acceptance. Further source edits are held through validation/archive.
