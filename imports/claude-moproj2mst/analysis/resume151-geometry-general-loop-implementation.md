# CP151: actual-bound same-profile geometry recipient loop

The initial high geometry reader now processes every reached eligible recipient in its actual current main-object range under the existing n1/d3/first1/fresh-storage/zero-variable-width contract. The arbitrary capacity3 ceiling is removed. Each actual object retains its own allocation/BC/sizing/copy receipt; a later refusal preserves all earlier copied bytes and reached partial effects. Nonempty variable sizing and other row/span profiles remain guarded.

Qualification: `analysis/resume151-geometry-next.md` SHA-256 `3a454b9b97e3c20f74ec45ec9548f569ac06a6d636a49f70e129dc2dfc42bc45`, and `analysis/resume151-geometry-general-design-review.md`. The native current-count/helper/backedge evidence is unchanged: `14082d9bf..d9d1` reloads current u1378 each iteration, `dbe9` calls the node helper and `dbee -> dc66 -> d9b4` continues; only exhaustion reaches Stop. Original EXE SHA-256 remains qualified as `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. No new REA/runtime evidence was acquired.

## Store, resource boundary and unchanged native order

The frame now stores `std::vector<GeometryMainRecipientProgress>` and uses its size as the sole started-record count. Fixed array/limit and redundant count are removed. Static assertions require records to be trivially copyable, nothrow movable and nothrow default constructible. They own no native allocation or COW body. Relocation changes host bookkeeping addresses only.

Before append, current frame/source/actual current bound/prior live identities/fresh recipient eligibility and ordinal ordering pass. Admission requires positive current signed u1378, record count below that actual bound, current ordinal in range and host max_size capacity. The only caught block is the metadata observation/emplace. A bad_alloc or length_error there parks with the prior vector unchanged, previous native effects retained and zero reset/allocation/leaf effects on the new recipient. This is an explicit reconstruction bookkeeping resource guard, not an original native OOM behavior or identical heap-call/address ordering claim. Actual native buffer allocators and callbacks remain outside that catch.

The active reference is taken from vector.back only after successful emplace. All its native work returns through the caller backedge before the next append, so no reference into vector storage crosses an append. After append, the Started entry/current frame/owner/fresh-storage facts are rechecked before actual free/reset. The global arrays-to-main stage remains one-way; there is no checkpoint rewind.

The optional private void metadata observer reports prior size/capacity/actual ordinal for deterministic tests and cannot substitute any native allocation. Normal execution has a null observer. Test-injected resource failure is explicitly distinguished from actual native allocation failure. There is no shared header, public service or ABI change.

Per-recipient native work remains real empty reset, diagnostic calloc3/publication, six BC attempts/publications including null results, node n1, actual28 mutable lookups/virtual zero-size leaves,24-byte actual-source copy and tail. The same installed frame/current initial borrow/model/ProjectDb/reader/current directory/source A pointer/count/version/cache/dimension/selectors and all former storage/registry/width guards remain. At every callback-sensitive seam and before copy/Stop/retirement, earlier recipients are recovered from the current step table/recorded ordinal, identity-compared before dereference, then receipt-checked. Native MAR preload remains before every skip without constructing an unused MAR pointer. The loop uses current u1378 rather than a frozen initial count.

All started receipts must complete before the single node Stop and exact high retirement. Prior zero-main/low paths and optional null-connectivity first getter/populated cached second getter remain. Parked raw repair cannot restart records, copy older rows again, requery, checkpoint, Stop or retire. No whole-loop performance improvement is claimed; the existing correctness-first live-table checks remain.

## Compact discriminating tests

Existing observation forwarding always performs the actual mutable lookup. Its test fixture can now select an actual receiver set. The new private metadata observation records real host size/capacity. Source/list/owner/width/fault mutations are explicitly adversarial tests, not original runtime observations.

- `geometry_general_fresh_recipients_copy_independent_rows_and_complete_once`:24 normal profiles, counts0/1/2/3/4/8, versions6000/11000 and null/populated optional connectivity. Mixed BC widths are used except the count2 zero-BC control. Captures actual registry bodies/ordered28 receiver identities per object, real primary and positive BC allocation distinctness, variable empty/null state and node/reset/seventh-slot facts. Each final lookup seeds actual A with a different nonzero row while keeping primary sentinels; exact24-byte comparisons retain every historical row through metadata relocation and later source changes. Verifies one arrays checkpoint/Stop, expected getter/bounds, cached no append/sizing/copy replay. Count8 observes multiple actual capacity-growth boundaries on the target standard-library implementation.
- `geometry_general_loop_observes_genuine_owned_list_growth_in_current_bound`: begins with two real positive objects; first final lookup uses actual get-or-create to append six owned objects and updates real u1378 from3 to9. All eight receive exactly224 real ordered lookup events and eight metadata admissions, correct copied rows and one reader/arrays/Stop. This detects a stale initial-count cap; it never fabricates count without list nodes.
- `geometry_general_metadata_resource_guard_retains_prior_copies_before_new_reset`: two resource modes inject bad_alloc or length_error at an observed actual vector growth boundary after at least four completed recipients. Confirms prior row bytes/counts, no new or later primary/BC reset/publication, reset marker9, no Stop/retirement, and no replay after removing the injected fault. There is no real OOM request or native allocator-failure claim.
- `geometry_general_late_lookup_preserves_all_prior_rows_and_live_identity_guards`: four modes stop the sixth recipient at lookup5 (146 observed lookups), through ordinary width, current foreign owner, displaced first prior identity or displaced nearer prior identity. Real spare nodes change the actual table while preserving the current ordinal; RAII restores all links and owner before cleanup. Five copied rows remain exact, current primary/five leaves/sentinels persist, later objects remain fresh, no Stop occurs and repair cannot resume.

The obsolete CP150 fourth-positive capacity test is renamed `geometry_three_recipient_copies_remain_before_unqualified_fourth_or_later_MAR_guard`; its positive fourth is explicitly logical n0. Its malformed fourth MAR preload still refuses after three copied rows. Fully qualified fourth and eighth recipients are tested as successful normal cases. Earlier explicit n0 third, same-owner/source/BC/body/receiver guards and nonzero-variable-width refusal cohorts remain.

## Static preflight and release

Static preflight passed: balanced delimiters, whitespace, project/Testkit include resolution, CHECK/REQUIRE outer arity, unique27 cases,378 CHECK/138 REQUIRE calls, one Testkit main, matching private helper call arities, plain/nothrow record assertions, vector-size-only count and absence of stale numeric capacity/count fields. No build, compiler invocation or runtime test was executed in this lane.

Target `m2m_mdt_geometry_a_producer`; exact focused CTest `m2m.mdt.geometry_a_producer`. Root owns changed-test binding, build/focused/full regression and any broader Viewer targets. Runtime results are pending root validation.

Frozen exclusive files:
- `source/apps/moproj2mst/src/engine/viewer.cpp` SHA-256 `3ad0117fdc715f4dbb8f0495d5d09213987519e4209a330ea07a1c1e0905abc2`
- `source/apps/moproj2mst/tests/mdt/test_geometry_a_producer.cpp` SHA-256 `6f2bb9273d9d02a847acca6ed49ad5dd9ea19b730a09bf93a8498ebd5228fcfa`
- This new implementation receipt.

Viewer/test are frozen and released to root. No shared header/CMake/CTest registration, other source, original/evidence binary, GUI or mutable REA state was edited.
