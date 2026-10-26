# CP139 independent browser-clear reverse-snapshot review

## Result

No blocker found in the frozen reverse traversal or tests against the CP138 native ordering contract. The helper binds a traversal to one live CP136 clear-request invocation and one declared owner state, retains one immutable snapshot, and advances its reverse cursor only after a matching per-removal normal return. Exhaustion and an empty snapshot stop at `CleanupPending` without generating a CP136 clear return or payload store.

## Owner, cursor and return barriers

`PropertyBrowserClearState::createQualified` requires valid slot-owner identities, a valid browser-private identity, a nonzero parent delivery ordinal, an unexpired CP136 invocation token, and nonempty qualification. Its owner embeds the CP136 slot/browser owner and the declared browser-private identity. The traversal claims the state once; later resumes require the same `shared_ptr` control block and traversal token. Snapshot observations must match that bound owner.

An observed returned snapshot is copied once, preserving order, null pointers and duplicates without dereferencing. For size n, `requestNext` issues `n-1-normalRemovalReturns`; each request carries owner, parent ordinal, index, exact pointer, and unique invocation token. Only a qualified return matching all of those fields and that token increments the count and advances the cursor. Interrupted return remains terminal for that invocation and cannot be relabeled normal. Repeated/foreign/early return attempts cannot consume the next request.

When the last normal return arrives, or the snapshot is empty, the helper enters `CleanupPending`. It exposes no cleanup or clear-completion API, does not synthesize the still-pending CP136 browser-clear normal return, and cannot trigger the CP136 payload store. It also does not claim an item was present, removed, destroyed, or that any Qt/list callback ran.

## Test and qualification review

The 12 cases exercise unknown versus empty snapshot, empty pending-parent behavior, exact LAST-to-FIRST requests and barriers, duplicate/null order, owner or snapshot replacement, early/foreign/wrong/repeated returns, interruption, local/parent expiry, claimed-scope replay, current-list changes without snapshot mutation, and exhausted cleanup. Tests construct their parent through the private CP136 helper and assert its payload remains unstored. They model declared observations; they are not a first-Open or Qt-delivery witness and do not claim Boolean E, OPI, Apply, or public lifecycle completion.

The logic is consistent with the bounded CP138 source contract: native copies the browser list, starts at end, predecrements before passing entries to the removal routine; empty snapshots skip removals but still have separate iterator/list cleanup and return. No synthesized native normal return is present.

## Frozen hash check

The three source/test hashes match `analysis/resume139-property-browser-clear-prefix.md`:

- `include/m2m/engine/property_browser_clear_prefix.hpp`: `d7672615cb12f23d588d4ce8ed62e6dc55cc9661a0a55950b36c8ca0aaa268ea`
- `src/engine/property_browser_clear_prefix.cpp`: `b26529d02908aa4bc146ad1399b27317876745bd4cd003a48e20c210d950b881`
- `tests/mdt/test_property_browser_clear_prefix.cpp`: `db933f7fcad089046123c4b1b5d950f37e9de8bcacfa4d29e5c832d42fd1b8aa`

Static review only; root owns compilation and runtime validation.
