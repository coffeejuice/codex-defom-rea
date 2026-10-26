# CP254 secondary publication-order implementation independent review

## Verdict: PASS within the canonical empty-secondary scope

The production diff moves `displaySetSecond(d, std::move(m))` to immediately after successful construction, then reads the published model through `dm.second`, assigns its backlink, and performs the existing new-control-before-main-control query/copy. The dimension fallback and both early branches are unchanged. No catch, rollback, flag update, geometry refresh, owner API or named-model change was added.

The parked-main test verifies the intended retained prefix through the production caller: it requires both secondary slots empty, preserves a main GEOTYP sentinel before parking, observes the exact geometry error, then checks guard/owner identity, backlink, dimension and constructor-default GEOTYP on the still-owned secondary. A second call returns the same pointer while the main remains parked; explicit drop expires the independent guard and empties both slots. The test checks the original main step/object and other captured model/display state remains intact. It is placed after the earlier pure-refusal timer/index-table checks, and its own ordinary-model constructor default is observed rather than guessed.

The normal display-selection caller test seeds and verifies GEOTYP propagation for live dimensions 2, 3 and present 0, plus backlink, main identity/flags, and repeated reuse. Existing CP252 tests retain absent-step dimension-3 behavior, main destruction, drop and no-main return. The implementation does not imply the original runtime had the converter's parked geometry exception.

The report correctly preserves the exact scope limits: canonical empty secondary and stable synchronous ownership only; no guarantees for public inconsistent guard/owner states, cross-aliasing, reentrancy or concurrency; original publication order is static evidence, while the retained-owner exception test demonstrates only converter behavior. Constructor/new-handler/Qt guard/SEH behavior remains unobserved. There is no rollback after the new guard throws.

The receipt reports source-stable focused build, binding preflight, `git diff --check`, and 2/2 CTest success for display selection and viewer box. It accurately says no full run or freeze occurred and leaves integration/L5/freeze to root. Source hashes bind the changed files and executables. No implementation review work changed or ran source/build/GUI state.
