# CP236 editor lifecycle identity implementation review

**PASS — the two identity predicates preserve their source contract; no unresolved defect found after test strengthening.** This is an independent static implementation/test review. Root owns integration, full-suite validation and freeze.

Reviewed the CP236 editor next-leaf proposal, independent design review, implementation note, bounded two-file diff/current bodies, the existing CP235 query, and relevant lifecycle test/fixture behavior.

## Preserved ordering and effects

The initial predicate remains: complete page state, matching source model, non-null source control, current-control identity query, then matching remesh. Its short-circuit positions are unchanged. Therefore incomplete pages, wrong source model or null token suppress geometry admission; wrong remesh does not suppress it. Actual-operation project/model/OPI admission still follows this predicate and precedes curve checks, cached-control dereference and page/I/O effects.

The reused `matchesCurrentSimControl` checks geometry once before resolving nullable current control and comparing addresses. Expected is not dereferenced and no pointer is returned. Its null/null equality does not weaken lifecycle admission because the caller retains the non-null cached-control guard. This Apply path remains potentially throwing: parked geometry propagates the existing Error rather than returning an unsupported report or acquiring pageState's noexcept termination behavior.

The second query remains after successful/non-unsupported Extra3D completion and after `pages.meshing.objects.clear()`. Short-circuit `actualOperation &&` preserves the absence of this second query on the no-operation path. Mismatch records unsupported recovery tail and outcome, then returns before the owned tail. Earlier recorded page results, model stores, Extra3D effects and the meshing clear remain committed. A thrown guard likewise leaves the reached prefix effects intact without returning a report. The two-expression diff introduces no rollback, cache1 access, coordinate query, publication or additional native-field access.

## Regression quality and repaired checks

The new preflight replacement test retains the original allocation, installs a distinct replacement and gives their nstep fields different sentinels. It verifies unsupported outcome, absent page/tail results, no effect-log entries, and unchanged sentinels. The retained allocation keeps the cached control valid throughout the test. The no-I/O check observes Apply's effect log; read-side calls are not instrumented there. Source preflight establishes that no page/I/O path is reached.

The post-Extra3D case retains the old allocation inside the callback, so replacement happens after admission and sc remains safe to inspect. The test observes callback execution, an Extra3D report/effect, unsupported tail/outcome and untouched replacement recovery length. Review found that its first prefix assertion was vacuous because nstep already equaled 23, and that it lacked the proposed meshing clear assertion. Both are repaired: nstep is set to 99 after Open and must become 23, a valid MeshObjectSnapshot seeds the temporary list, and that list must be empty after refusal. The seeded object is compatible with the existing meshing Apply path, which skips missing live objects on copy. These checks now establish meaningful retained prefix/clear behavior rather than merely checking defaults.

Old and replacement controls remain alive where inspected. The post-callback retained step is destroyed before the session, but session teardown does not dereference its cached source token; no later Apply is made. Tests retain the existing Testkit entry point and supported macros, with no new quoted include or C assert. The distinct actual-operation binding-mismatch regression remains unchanged.

The implementation note and root report focused build/CTest success (33 lifecycle cases), including the repaired test. This reviewer ran no build or tests and does not independently certify runtime receipts. No parked lifecycle Apply was executed by this review; preserved query position/exception precedence is established statically, while CP235 viewer tests exercise the query's throwing guard separately. Complete integration and freeze remain root gates.

## Identity and qualifications

Reviewed SHA-256 values match the repaired note:

- `simcontrol300_lifecycle.cpp`: `F9C89697030C287D399543852F645F4246F36DF7F149F10BAAADEB580C8CDF47`
- `test_simcontrol300_lifecycle.cpp`: `D1F77F02A3B961EC7A03D082157AAFCD35DA4776718AD472E6401F9E79EFE431`

The adapter migration does not establish a recovered native identity routine, fresh original parity, lifetime/ABA protection or ownership closure. Cached sc and other aliases remain. No C ABI or Stopping/cache1 changes belong to this leaf; simcontrol300.cpp and stopping_cached test have no working diff. The implementation note retains these limits accurately.

Only this report was written. No source/test/build/shared recovery state, original process, GUI or REA state was changed by this reviewer.
