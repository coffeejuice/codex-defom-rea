# CP121 canonical handler state and the first InputTable engine

The canonical handler flag/list producers are now source-bound. QtCore's QAbstractFileEngineHandler constructor sets the flag and prepends the actual handler; its destructor removes the handler and clears the flag when the list is empty. The five retained PE modules contain no ordinary imports of that constructor. This narrows the default branch but does not establish the original process's handler state after startup or dynamic module loading.

For a same-occurrence **flag-zero** profile and the bounded normalized drive/UNC filename, the first InputTable resolver returns no custom engine without an existence check or search-prefix rewrite. The QFile engine creator then constructs QFSFileEngine. This is an exact conditional branch contract, not permission to set the candidate process flag to zero or release Geometry admission.

## Canonical state owner and producers

All new evidence is from retained QtCore PE SHA-256 `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.

- Handler gate byte: `0x672f3ed0`, in the `.data` section's unbacked virtual tail (RVA `0x2f3ed0`; section raw size `0x3c00`, virtual size `0x4a90`). This is loader-zero-fill storage, not an on-disk nonzero initializer or a captured post-startup byte.
- Canonical handler-list object pointer: `0x672f3f08`, obtained through `0x670a7c60`. Its complete logical body spans the four adjacent unwind fragments now captured. Lazy construction allocates an eight-byte QList wrapper, installs QList shared-null data `0x672f0a50`, increments that data's ref, and publishes the wrapper with atomic compare/exchange. Destruction/initialization flags and cleanup registration are separate globals. This QList shared-null is **not** QString shared-null `0x672f0b30`.
- Canonical lock owner pointer: `0x672f3ee8`, obtained through `0x670a7b00`. Its lazy path allocates/constructs a lock, atomically publishes it, destroys a losing allocation, and registers cleanup. Tagged local lock handles track whether the current path acquired the lock.

Export `QAbstractFileEngineHandler::QAbstractFileEngineHandler()` is `0x670a7d30`. It installs the base vtable, obtains/locks the canonical lock, then writes handler flag1 at `0x670a7d7c` **before** obtaining the canonical list and inserting this handler. The insertion call is `0x670a7d95 -> 0x6714bdd0`. In the unshared list case that helper calls exported `QListData::prepend` at `0x67048c60`; the shared case passes insertion index0/count1 to the detach/grow helper. Newest constructed handlers therefore precede older ones in the stored traversal order.

The copy-constructor export `0x670a73d0` only installs the base vtable and returns in the selected leaf; it does not register another handler or set the flag. Its padding/neighbor bytes are excluded from this claim.

Export destructor `0x670a7ed0` obtains/locks the canonical lock and, unless the handler-list teardown guard `0x672f3ed1` is set, obtains the list, finds/removes this handler's entry if present, then compares the resulting begin/end indexes. At `0x670a7fb9..0x670a7fcd` it preserves the existing flag for a nonempty list and stores zero when empty. It does not unconditionally clear the process flag when any one handler dies.

Two direct RIP-relative byte-store candidates to `0x672f3ed0` were found and matched to those bodies: constructor `0x670a7d7c` and destructor `0x670a7fcd`. This pattern inventory excludes arbitrary wider/indirect writes and is not a whole-program memory-write proof. The store-before-insertion order must also be retained: without a qualified allocation/unwind contract, a future registry model cannot replace the separately stored flag with an assumed `!list.empty()` invariant for every partial-failure state.

## Lookup and applicable first-input branch

CP120 binds normal InputTable QFile::open to actual QFile engine selector `0x670b4800`, which passes the retained filename+0xe8 to `0x670a7620`. That function constructs the normalized file entry at `0x670a7642`, then calls `0x670f0080` at **`0x670a765a`**.

The source sequence is:

1. `0x670f0080` copies the entry and initializes its engine output null. It calls resolver `0x670efbc0` at `0x670f00e4` with existence-check flag zero.
2. Resolver gets a path QString lease and calls `0x670a7dd0` before deciding native/resource/search syntax.
3. The lookup first reads byte `0x672f3ed0` at `0x670a7df7`. If zero, it returns null via `0x670a7ea8`, without constructing/acquiring the handler lock/list or calling any handler. Prior file-entry QString/shared-null work still occurred; the flag-zero branch does not erase it.
4. If nonzero, lookup obtains/locks the canonical lock, obtains the list and visits its entries in stored order. Each call is handler virtual+8 at `0x670a7e61`, with the filename QString. The first nonnull engine stops traversal; null results continue. Lock release and the normal result occur at the corresponding tail. No rule derives a null engine from an absolute filename before these calls.
5. On the flag-zero route, normalized absolute drive/UNC filenames enter the native branch (`0x670efc93` leading slash, or colon at index1). With existence checking false, `0x670f0014` goes directly to the successful native tail `0x670f0056`; it does not probe existence or traverse search-path candidates. The entry is not rewritten by a search-prefix result. The resolver success is distinct from the still-null engine output.
6. Returning to `0x670a7620`, null engine at `0x670a7662` triggers a `0x10` engine allocation. It obtains the normalized filename at `0x670a7690` and constructs **QFSFileEngine** at `0x670a76a8 -> 0x670eabf0`. The export table independently names that constructor. It allocates its private payload, constructs/copies a file entry and installs its own vtable. This is native engine creation, not yet proof that the subsequent virtual file open succeeded.
7. A nonnull custom engine instead takes the earlier return/cleanup path. A flag-nonzero list in which every handler returns null can also fall back to QFSFileEngine, but it has additional lock/list/callback effects and cannot be treated as the flag-zero branch.

The admitted filename syntax above is a reconstruction bound inherited from CP113/120, not native input validation. Relative/search/resource names retain their separate decisions. Native constructor/null allocation and handler failure/unwind behavior remain qualified by the existing captures; no ordinary-success result is invented for those paths.

## What the fixed module inventory establishes

Ordinary import tables were read for the five retained binaries: DEF_GUI_MO_64.exe, DEF_PRE_64.exe, QtCore4_64.dll, QtGui4_64.dll and QtXml4_64.dll. None imports a symbol containing `QAbstractFileEngineHandler`. A selected direct-call/RIP reference scan inside QtCore found no call candidates to the default or copy handler constructor exports. The compact inventory records each binary's actual hash.

This means there is no recovered ordinary fixed-module constructor caller to assign as the process's handler producer. It does **not** mean the running list is empty. Delay/dynamic symbol lookup, imported helper modules outside this retained set, plugin code, indirect calls and raw state changes are not closed by that inventory. QtCore's zero-fill initial state is earlier than those possible registration events. The single-project scope does not by itself make application/module startup trivial or supply an actual flag observation.

The exact remaining producer edge is therefore **registration/lifetime activity from the actual loaded module set before the first InputTable QFile engine creation**, or an observed canonical flag/list snapshot at that point. The canonical handler API itself and its list ordering are no longer the unknown.

## Admission consequence

The smallest conditional first-input profile is now precise: actual same-process handler gate0 at this read's resolver invocation, bounded normalized drive/UNC filename, and normal reached QString/file-entry ownership. It selects the no-handler/native-engine route above. Unknown gate/list state must remain a guard, and a retained PE zero-fill byte or the candidate's lack of a handler API is not a substitute for that actual process profile.

This investigation does not close canonical QString shared-null's current reference owner, the closed QFile/engine alias inventory at Geometry entry, or numeric parsing state. CP120's fresh final-concat data origin and explicit copy leases remain valid; they still must not be relabeled a complete count from a guessed no-handler state. A future private context must use this process-global owner, rather than creating a Session-local empty registry or accepting a caller Boolean named “no handlers.” No isolated source helper can turn the static import negative into that missing occurrence.

Next evidence action: bind the actual startup/dynamic module registration history or canonical flag/list at the first `0x670a765a` call, retaining registration order, live handler identities and the specific handler return if flag1. If that yields flag0, the fixed branch and QFSFileEngine constructor are source-ready; the next owner work is the concrete normalized QString/file-entry/QFS engine lifetime and its shared-null leases, not the GUI GEO loader.

## Receipt and scope

New receipts include `resume121-doe-handler-exports.json`, `resume121-doe-handler-owner-static.json`, `resume121-doe-handler-list-fragments-static.json`, `resume121-doe-handler-insertion-static.json`, `resume121-doe-native-engine-fallback-static.json`, `resume121-doe-handler-store-candidates.json`, `resume121-doe-handler-ctor-call-candidates.json`, and `resume121-doe-handler-import-inventory.json`. The capture index binds their hashes and bounded assembly files. The first export probe correctly failed to read the BSS flag as file-backed data; the final receipt records its section/zero-fill classification instead. Unwind fragments and leaf captures retain their boundary qualifications. Existing CP79/113/120 resolver captures were reused; no repeated full decoder or application dump was needed.

No source/test/CMake edit, build/compiler, GUI/original execution or mutable REA access occurred.
