# CP196 Report-null binding source review

## Decision: PASS for the repaired source

The previously blocking private-allocation identity issue is resolved in the released repair. The reviewed source satisfies the bounded CP196 Report-null binding contract. This is source approval; root still owns compiling and running the required suites before freezing.

## Resolved finding: same-address private-state replacement

The first review blocked address-only private identity. The repaired `SparseQObjectPrivate` now owns `allocationOccurrence` (`doe_task_runtime.hpp:86`), independently of the frame/token occurrence witnesses. Its default constructor allocates a new identity; copy construction and copy assignment generate a new identity, while nonthrowing move preserves the prepared identity. The immutable issued binding now stores `privateOccurrence`.

Fresh source issuance binds the original private occurrence at `task_factory.cpp:290`. `validateSessionReportNullCaller()` compares its live pointer and shared ownership control block at line 185, alongside the existing raw address/size checks and before frame/token checks. A replacement at the same raw address therefore cannot pass merely by copying all sparse fields. No issuer retry or retained-frame repair was added.

The new deterministic regression at `test_doe_task_factory.cpp:8356` copies the qualified private object, proves the copy has a different occurrence, destroys the original object and move-constructs the copy into exactly the same owned storage. A static assertion covers nonthrowing private move construction. The test verifies unchanged raw identity and every sparse model value, plus all four shared-root ownership bindings; only the private occurrence differs. It retains the original witness alive, preventing witness expiry from disguising the actual mismatch. It requires the exact CP196 source-binding refusal and complete damaged-state/count equality for Report, DOE and Custom keys under budgets 0 and 17. The original different-address replacement case remains.

Complete snapshots now include both the binding's private occurrence and the live private allocation occurrence, with weak-owner comparison for the binding. This covers the requested same-storage failure and its nonmutation contract at source level. The initial blocker is closed; no remaining concrete source blocker was found in this repair.

## Source checks that passed

The actual Session associates its allocation/widget owners and then invokes the private issuer (`conversion_session.cpp:172..173`). Only `ConversionSession` and the existing diagnostic friend can access the operation. Native profile, a paired standalone factory, a task key, and parsed DOE projection do not independently issue a capability. Pristine store counts and ordinal zero restrict issuance to the first Report occurrence.

Independent `sessionReportSourceEligible_` and `sessionReportFrameIssued_` state survive nullable-handle removal. Exact capability control-block identity and receiver/root comparisons reject simultaneous nullable-handle downgrade, foreign/aliased owner control blocks, and copied capabilities. The fresh-frame block sets known-null on the new token before publication; it does not upgrade a retained token. Separate frame/token identities reject ordinary reset/copy replacement, and issued history prevents the fresh-frame block from reissuing after prefix removal. Scope release revokes admission.

The new source guard runs before initialization writes and before every qualified resume. Existing saved/restored RDI, stack lineage, RBX, private/public backlinks, TEST, branch, cursor and incomplete-state validators remain in the predecessor code. Nullness is copied through POP, recomputed by TEST, and consumed by the unchanged JE. Valid Session execution reaches `BeforeCallerNullParentCall` / RVA `0x131449` with budget 17. No new call, parent load, thread/TLS query, helper effect, public task completion, cache insertion or constructor completion was added by the reviewed diff.

Standalone symbolic fixtures retain unknown/null/nonnull behavior. Old Session-default unknown tests now explicitly use standalone factories; new positive cases obtain the capability from an actual Session. Earlier rewinds clear restored nullness consistently. The seven new cases inspect fresh/earlier-budget issuance, terminal reentry, TEST/JE rewind, downgrade/foreign/stale/nullness/lineage corruption, prefix/token removal, late binding, parsed-profile separation, invalid budgets and post-scope refusal. These inspected assertions do not substitute for running the suite.

## Comparison and evidence limits

Compared all five released files against the exact entries in `build/wave3-checkpoint195-verified/source.zip`, whose paths omit the leading `source/`. Compared decoded lines after accounting for archived CRLF versus working-file newline handling; hashes below identify the exact working bytes reviewed. Production changes are limited to the new source binding/validation and one Session issuer call. The existing CP194/CP195 instruction bodies and terminal stops are unchanged.

Read `resume196-report-null-binding-next.md`, `resume196-report-null-plan-review.md`, `resume196-report-null-implementation.md`, and the source qualifications in the parent-slot issuer/import-binding reports. The retained PE/import evidence supports only the selected static Report caller's zero RDX argument and listed QObject entry. It does not prove runtime loader resolution, native execution, reachability, generic parent ownership, or helper semantics. This review did not independently parse, execute or rehash the original images.

Working-file SHA-256 at review:

| File under `source/apps/moproj2mst/` | SHA-256 |
| --- | --- |
| `include/m2m/doe_task_runtime.hpp` | `b622957c904e83331a1c714690ce1db734c47f0f261f0bd63a84b8c00dbb9cba` |
| `include/m2m/doe_task_factory.hpp` | `46ca2cef3976e628a2b241314b8c7503a5b717667052821bec3a49302c4ef723` |
| `src/doe/task_factory.cpp` | `6e7b113a11cf73aafc3d4ac2687b019e6fbe5118d67323cd681d0c64f6a7aeb2` |
| `src/engine/conversion_session.cpp` | `4435ed3f888560fa9829616dbf912b8e20d10d6fd8d5e77846f61b72efbceb3b` |
| `tests/mdt/test_doe_task_factory.cpp` | `569ef40abf52eb3b8ac19cb64867cebc2943475c469e50200c0c872ba5bfba00` |

Only this review was written. No source/test/build, GUI, mutable REA database, parent/DMO/Claude/original artifact or deployment was changed. No executable test was run. The repair and regression were reviewed statically; no runtime validation result is claimed.
