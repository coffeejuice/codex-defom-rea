# CP132 isolated embedded context prefix: source release

The private runtime now has an explicit context-receiver profile created before its child map begins. That receiver owns the actual M at C+1458; after genuine map return, resumeContextPrefix commits the source-selected55 stores and guards before the first string routine. The standalone-map profile remains separate. No Project/default binding, enclosing constructor completion or native cleanup/Close is enabled.

The four reserved source files are frozen:

| File under source/apps/moproj2mst | SHA-256 |
| --- | --- |
| src/model/feature_crt_runtime.hpp | `c4bdf5a8fd11d07a56a8542abf72ea3aab4c26231efcaf08d8f4134e09ffb9dc` |
| src/model/feature_crt_runtime.cpp | `794205f48c9e488fd94386f291554d763226bb1672d1d12928f64ca631d660a8` |
| tests/model/test_feature_crt_runtime.cpp | `9443779bd53d2a76f5470273c09063fe1e94a276fba8654aaa64abcaf46a2ef9` |
| NEW tests/model/test_feature_context_prefix.cpp | `399d97dc8dae8f3c41a450153a38b9fa898fe71f6fa573545ce896abcf322824` |

## Same receiver from preparation through child and prefix

prepareOwnedTestContextPrefix authenticates the genuine retained lease and actual produced candidate heap, then applies the existing one-frame capacity limit. It does not take a caller C pointer/completion Boolean or start a producer implicitly. Host metadata allocation may throw before native child effects; it is not a source CRT allocation for C.

Private ContextEnvelope has compile-checked DWORD3c/40/44, M at1458, signed cache1498 and host backing size14a0. This is a minimum receiver backing/layout gate, not a recovered native Project size. Its explicit empty constructor leaves unknown fields uninitialized. Raw bytes cover PST/gaps/string/fallback/optional-owner storage without constructing host strings or pointers with invented defaults.

The call owns either this envelope or its existing standalone MapHeader. A map reference is selected once at preparation and never rebound. Actual A therefore receives the genuine embedded M pointer when the existing8-byte child phase runs; the actual48-byte child phase initializes that same member's head/count. No completed standalone M is copied, reparented or interpreted by subtracting1458. Runtime strongly retains the call, the call owns the envelope and weakly borrows runtime, preserving bounded process retention without a strong cycle.

resumeContextPrefix refuses a standalone receiver, returns ChildMapPending before actual child return, and checks real C/member/view identity before stores. It never drives the child implicitly. After its own completion, repeated calls return the retained receipt rather than replaying resets or entering the string routine.

## Source effects and unknown state

The prefix stores signed-1 at C1498, zero DWORDs3c/40, performs the18/14/19 store leaves12e6fb0/12e6ff0/12e7020 with same C/DL0, then stores DWORD44=0. A fixed55-event ledger records original instruction role, offset, width and **readback from the just-written actual candidate field**. In particular DWORD34 remains a four-byte event after byte04, rather than an eager/reordered byte zero-fill. Source reset arguments C/DL0 are diagnostic call-input observations, not live original CPU registers.

Typed scalar stores and byte-representation memcpy avoid invalid type-punning into opaque backing. Readback inspects only each just-written field; unknown memory is not scanned. Byte knownness covers00..0f,12..30,34..3a and the initialized scalar range3c..47. PST10/11, gaps31..33/3b, strings, fallback1448 and optional-owner1450 remain unknown. Context constructor return, string-call entry and native cleanup stay false.

The resulting stage is ContextPrefixComplete, stopped at12f111d/146de38 with actual candidate destinationC+48. The static original source role2146f86 is not used as a candidate pointer or dereferenced. The later C1450=null store is not executed or inferred. Thus neither map/context destruction nor accepted Close follows from this state.

There is no kernel allocation/callback in this outer prefix. Native allocator effects remain the prior8/48-byte child requests against the actual candidate runtime heap. A pending/failed child authorizes no outer stores or rollback. All host receiver/ledger storage exists before child effects. ContextEnvelope/MapHeader host destruction remains trivial metadata cleanup; no HeapFree, HeapDestroy, provider or native destructor was added. Active C/M/A/H remain process-retained when local leases disappear.

## Tests and static preflight

The existing target m2m_model_feature_crt_runtime / m2m.model.feature_crt_runtime remains nine cases and adds checks that the standalone profile has no C, outer resume refuses without effects, and a later context preparation cannot take over its occupied one-map slot.

The NEW target m2m_model_feature_context_prefix / m2m.model.feature_context_prefix has one comprehensive actual-context case in its own fresh process, preserving one-map capacity. It verifies unready/foreign refusal, C/M offset and unknown initial fields before allocation, no implicit child work, actual8/48 child return on the same member, exact55 golden store tuples/readbacks, signed cache/zero scalars and sparse knownness. It checks PST/string/C1450 unknownness, the first string guard, unchanged child identities/counters, no replay, and genuine receiver retention after local leases end. A candidate-only context/map/proxy/head/store/cache receipt is printed when root runs it.

Static preflight binds CP131 source archive build/wave3-checkpoint131-verified/source.zip SHA `eb9dcbaceb99688df05fdc7917f8e4dfa2ccb1355b1d883d8f6d7bf26ecec591`. It independently parsed retained constructor/reset ASM within their semantic bounds and verified all55 sourcePC/offset/width/value tuples against both the test's golden ledger and implementation leaf tables. It also checked old producer/lease/factory/map8/sentinel48/report function bodies unchanged, receiver selection before effects, child-return/no-replay guards before stores, no allocation/child-driving/kernel call in the outer method, explicit layout compile gates, resolved quoted includes, balanced delimiters, supported Release-active Testkit macros/entrypoints and no C assert. These are static checks, not compiler/layout/runtime results.

No CMake/shared model/Project/Session/C ABI/standalone-p11 edit was made. Existing test discovery supplies the new model executable; Project storage cpp acquires no new link dependency. Public model.hpp read-time SHA is `baf657f01683196d78a45cbb180273cfa8d10b57c0523110171fe4fe235e078d`; Project storage cpp remains `af2e1da0fe4bdc582e29731b2f0e6ce7d7f0a882ea2ade477f164807da1199e8`.

## Evidence and release

Design analysis/resume131-feature-context-next.md SHA `73ce08f1fe41b53574d332dc02f549905e6af25f376ffe681bc56836d384ec6f` binds original v11.0.2 DEF_GUI_MO_64 SHA `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, context constructor ASM `38e87349cb26103cb4f4d593fa0ac441ff4b5706283645e6cfdf00255cbe6e62` and reset leaves `198375e4e1a1a89249a0e96cb29a54a86661e88c264dd1aea6e5986192ecb262`, `4d1a4a310102a697c44fda7a47431af41f8023344a80598fffeba614fb27f577`, `57ad77652ec2ac0305084bb87e63fe53df32f0a67563a389a3e48bc70dd2cbfe`. Padding/next-function fragments were excluded from static tuple extraction.

All four source files are released and held through root validation/archive. Only this NEW receipt accompanies them. No build, CTest, native kernel call, GUI, original target execution or mutable REA occurred in this lane. Root owns actual acceptance; default Project provenance, strings, full constructor, reusable-host retirement and native Close remain guarded.
