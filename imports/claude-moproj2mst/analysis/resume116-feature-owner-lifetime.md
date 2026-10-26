# CP116: feature context registration and destruction ownership

The feature context's native lifecycle has two distinct cleanup phases. Accepted Mo cleanup first invokes per-feature release, including selected provider-object deleting virtual calls. Later Project destruction frees the inline context's tree storage, sentinel and proxy; that container destruction does **not** delete the raw provider payloads. Replacing this with an owning string-keyed STL map would change both identity and deletion behavior. This is bounded static architecture evidence, not a claim that the candidate has constructed the same owner or executed accepted Close.

Original retained DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No original/GUI launch, REA import or database mutation, source/CMake edit, build or test execution occurred. All writes are uniquely named bounded ASM/metadata receipts and this report. No speculative helper was added.

## Same-owner storage graph

Let P be the native Project, C=P+60 its inline0x14a0-byte feature context, and M=C+1458 its inline map. The CP114 constructor creates a separate8-byte proxy A with `[A]=M`, a48-byte sentinel H in M+30, and size0 in M+38. The context also has the independent optional field C+1450 and cache C+1498. There is no separately allocated C to free: its lifetime ends inside P's destructor, before P's outer deleting destructor releases P.

Normal map nodes are48 bytes. `12ed9f0` initializes child/parent/right pointers at+0/+8/+10, copies the input pair's **canonical feature pointer into node+18** and **raw provider-object pointer into node+20**, then color/nil bytes+28/+29. No key-string copy, key reference-count operation or provider ownership wrapper appears in that node constructor. Numeric unsigned pointer comparisons drive map insertion, lookup and key erase.

`12e7be0` converts a recognized C string into a canonical static literal pointer, otherwise returnsnull. All29 literal addresses passed by accepted cleanup `12f1190` exactly match canonical addresses returned by this normalizer; this was checked as addresses against the same PE, not merely by equal decoded text. Those literals are module-owned/borrowed, not node-owned allocations. Content equality with an unrelated duplicate buffer is not this map's equality contract. Module lifetime must span registration and cleanup.

## Registration and partial effects

Actual Open0aa960 obtains feature strings from the selected diagram/operation path, queries this same C, and conditionally calls12f08d0(C,name,true). CP112/114's unknown initial PST bytes and missing source-bound provider/query results remain unresolved; no always-false or always-available input is inferred.

The bounded successful registration tail has this order:

1. Negative C+1498 triggers query-cache producer12eee10 **before** null/empty-name checks; normalize the feature name with12e7be0 and resolve the selected state byte/type via12e76c0.
2. Allocate0x3ea0 and construct the provider object with12ee6a0. Set object+3e9c=1 and+35b8 from C+1498. Clear process-global qword `0x1446edcf0`, then call12ecbb0 on the actual object. This global write belongs to registration, not a Close-wide reset.
3. If12ecbb0 returns nonnegative, call12ee870. Convert the sign of the original result into Boolean success. When the original true argument is set, call12f1eb0 before publishing that Boolean into the selected C state byte.
4. Write the selected flag byte at12f0a4d. Success passes `(canonical key pointer, actual object pointer)` to map insertion12f01d0 at12f0a71; failure invokes the object's deleting virtual0 with EDX1 at12f0a83.

Logical12f01d0 spans several adjacent unwind fragments; the explicit complete bounded capture ends at RET12f037d. On a duplicate numeric key it returns an insertion Boolean false, rather than replacing the existing payload. The12f08d0 caller ignores that insertion result and returns its prior success Boolean; no duplicate-path provider deletion is present in that caller. Reachability of a duplicate for a fresh supported project is not established here, so this is not a claimed live leak. It is enough to rule out an invented replacement/deletion guarantee.

Registration exception state is narrow. The12f08d0 IP map activates state0 at12f09dc for provider construction and clears it at12f0a06 after that construction returns. Its action141b9c650 frees the raw allocation saved at frame+a0 via14dfc68. No action in this function covers the later query/flag-publication/map-insertion tail. Thus an adapter must not silently supply RAII deletion/rollback for every failure after construction; transitive ownership and exception behavior there remain dependencies. A flag publication before a failed insertion is a partial effect, not a transaction to undo.

## Accepted feature release precedes container destruction

Native manager clear deletes the actual Mo child. For a nonnull Mo+19c0,044bf0 calls0982c0→0aab30→12f1190(C) **before** the Project deleting destructor. The29-name release loop uses the same context and canonical module literals in source order; it does not construct a fresh empty context.

Per-feature12f0da0 first performs its qualified state-byte lookup. Absent name or zero state skips later provider/map work. A nonzero state reaches12e5780; a negative result skips further work. Otherwise, critically:

- `12f0ab0(M,&key)` is a subscript-style operation. It searches by numeric pointer key and, on missing key, builds `(key,null)` and enters insertion12f0380. It returns the address of node+20. Cleanup can therefore allocate an absent null-payload entry; this must not be replaced with a read-only find/no-op.
- A nonnull payload is saved locally. `12f0bb0(M,&key)` erases/frees the matching **tree node first**, through12f06b0/12efde0. Only after erase returns does12f0da0 call that saved payload's deleting virtual0 with EDX1. The key/storage removal precedes provider destruction and its possible callbacks.
- Later external/provider work can fail or branch without resetting the feature flag. Only the success branch at12f100b writes the selected byte0. Map removal, provider deleting call and flag clear are separate committed effects; no rollback or unconditional final reset follows from the unconditional Boolean return of0aab30.

The map erase paths do not invoke a provider deleting virtual or free a key string.12efde0 rebalances, frees the erased node through14dfc68 and decrements map size when nonzero. Provider deletion is the separate explicit edge in12f0da0. If some payload survives or is no longer reachable from this map, its ultimate lifetime cannot be inferred from these bodies; do not promote a local raw pointer to automatic unique/shared ownership or assert a whole-program leak.

## Project/context destruction order

After that feature-release phase, Project deleting destructor0453e0 calls08a3c0(P). Its normal-return order is:

1. Resolve diagram view and, if present, call its null-model virtual+1b8.
2. Call0a80e0 to clear Project SubProjects and their effects.
3. At08a412 call **12f0d20(C)**.
4. Destroy the remaining SubProject-list storage/QString/base members, return from08a3c0, then0453e0 releases P through14dfc68 when deleting flag bit1 is set.
5. Only after Project deletion returns does044bf0 null Mo+19c0. Mo's later045000 deletes the five model slots; manager current is cleared only after its child-deletion/list-clear phase.

The context destructor12f0d20 first calls12e6910. This actual bounded leaf is `eax=1; ret` and preserves RCX, so the current profile then tests the actual C+1450 field. If nonnull, it passes the **low32 bits of that field** to12f51b0 on process-global owner `0x1446efe90`, calls1471b7c on the full pointer, then writes C+1450=null.12f51b0 contains its own global-list/lock/state-dependent work and is not an invented Session callback. The producer and live backing of C+1450 remain unqualified; constructor-null must not be reused after Open/registration without a same-owner write/liveness history.

Next12f0d20 destroys M:

| Stage | Exact storage effect |
| --- | --- |
|12f0860→12f06b0 full-range path→12ee410|Clear all ordinary tree nodes. For each subtree, recursively free the right subtree, save the left link, free the current node, then continue left. This is right/node/left order, not an invented generic postorder. Node+18/+20 key/payload are not dereferenced for destruction. Reset sentinel links/self and map size0.|
|12f08ac..12f08bc|Free the sentinel H via14dfc68; only afterward store M+30=0 and M+38=0.|
|12f0d87..tail|Load proxy A from M+0 and free it via14dfc68. No M+0 null store is present.|

Even an initially empty map follows its sentinel-reset/free/proxy-free effects; a missing provider entry does not erase container ownership. The borrowed module literal and provider payload are not freed by node/container destruction. This is the direct-body reason the29-feature phase cannot be replaced by simply clearing an owning C++ map in Project/Session destruction.

## Exception cleanup edges

Project08a3c0 IP state5 covers diagram/null/SubProject-clear calls; its action141818b40 invokes12f0d20(P+60), then the unwind chain continues to the Project+58 list/string/base cleanup actions. The feature context can therefore be destroyed during Project destructor unwind as a member even when the normal path has not reached08a412. It is not permission to run accepted0299a0 Close from a converter catch block.

Context12f0d20 has two different actions: state0 over the optional C+1450 phase invokes map destructor12f0cf0 through141b9c690; state1 over12f0860 invokes proxy-only cleanup12f5e30 through141b9c6b0. This avoids inventing a second full container walk after a partially completed map destruction.12f0cf0 itself performs12f0860 then proxy free; its own failure action and underlying allocator/free/provider exception behavior remain subject to native EH execution, not observed runtime behavior here. The metadata and action bodies are retained; no generic destructor equivalence or suppression of nested exceptions is claimed.

## Candidate boundary and next source task

Current public ConversionSession performs adapter Project/storage/receiver/dispatcher cleanup and has no actual C owner, canonical module-symbol binding, provider allocations, optional C+1450 producer or accepted FileCloseAll cursor. The isolated CLI's candidate worker/process observations do not supply those GUI/main/project feature facts. Preserve that distinction and CP110's busy/modal/nested-Save/receiver guards; no process-global remesh reset or native Close result follows from this report.

The next architecture increment must bind one source-qualified feature context to the actual Project constructor/lifetime, preserve unknown bytes and canonical symbol identities, and represent proxy/head/node allocation separately from provider payload lifetime. Node erase/clear must release only source-selected node storage; explicit feature-release calls must own their exact deleting-virtual edge. Actual provider/query results and allocation/EH/deallocation outcomes require source-bound inputs, not synthetic success. Optional C+1450 needs a producer and global-owner witness before its nonnull destructor route can run. A future pure model cannot certify these actual owner births.

Focused future acceptance: same context across actual Open registration/Save/nested Save/Close; matching canonical pointers despite temporary equal-text inputs; missing cleanup subscript insertion and resulting allocation; erase-before-provider-delete callbacks; state flag unchanged on later failure; duplicate insert result preserved without replacement; constructor unknown PST bytes retained; provider-construction throw raw allocation cleanup versus later-tail failure; optional-member throw invoking full map cleanup once; map-stage throw invoking proxy-only cleanup; empty/nonempty node→sentinel→proxy free order; Project destructor callbacks observe Mo backlink before its later null store; no cleanup after default terminal remesh dispatch. These require actual same-owner/native admissions and do not widen public success now.

## Evidence binding

`analysis/resume116-feature-lifetime-evidence.json` SHA-256 `215c68d512628362459b92755baf4c71c664a78683acc55e16d605867f768a94` binds all23 uniquely named raw ASM captures, .pdata/.xdata IP/action maps,29 canonical Close keys and partial-capture qualification. Capture receipts under `resume116-feature-...-static.json` preserve commands and bounds. The initial12f0860 capture was retained before the no-unwind12e6910 stopped its batch; that partial artifact is explicitly bound in the metadata. The latter leaf's semantic bound ends12e6916; its captured later-function fragment is excluded. Logical insertion/subscript explicit spans cross .pdata fragments and are bound independently through their final RET and next-function padding, rather than claiming that the first unwind region was the complete function.

Key independent raw identities: complete insertion span `resume116-feature-insert-logical.asm` SHA-256 `953ba4f273ec24c3760dadf96f8ca4254b063e910557faf3494434657eae5db4`; existing08a3c0/0453e0/044bf0 and29-name12f1190 retained bodies establish the enclosing order; existing CP112/114 constructor/query evidence remains unchanged. The metadata's full artifact list is the authoritative hash inventory rather than an execution receipt. No source, test, shared report, installed program, mutable REA or live process state was changed.
