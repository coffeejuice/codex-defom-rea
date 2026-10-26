# CP59: isolated true busy-transition prefix

The new helper commits only the deterministic true-setter prefix: global+9=1, +A=0, +B=0, preserving143d91230. It retains an explicit continuation guard before TreeEnableSelection(false) argument construction and delivery. No result denotes accepted/completed Close.

The assigned write set is exactly these three new files plus this receipt:

- `source/apps/moproj2mst/include/m2m/engine/close_busy_prefix.hpp`
- `source/apps/moproj2mst/src/engine/close_busy_prefix.cpp`
- `source/apps/moproj2mst/tests/mdt/test_close_busy_prefix.cpp`

No shared Session/model/CMake/public Close/plan files were changed. This worker ran no build, executable tests, GUI, original application, or REA operation. Root owns the combined build and test run.

## Source-qualified boundary

`analysis/resume59-close-next.md` contains the controlling bounded PE analysis, original addresses, import bindings, native hashes and inherited metadata qualifications. Original reference: DEF_GUI_MO_64 v11.0.2, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.

At accepted Close029a54, native2ca9f0(true) writes global+9 at2caa12 and skips the false-only counter reset at2caa21. Its2ca920(show=false) call short-circuits on the freshly stored nonzero byte, avoiding window/virtual/modal effects. Then2caa4f ->2cb543 writes+A=0 and2caa5e ->2cb583 writes+B=0. These leaves have no calls. The implementation preserves that source order.

The next implemented-state boundary is2caa63, before QGenericArgument construction,2cf3c0 map/suppression work and TreeEnableSelection(false) delivery. The result identifies the pending Boolean argument but does not execute it. The known tree registration/Boolean slot evidence means no empty-key or no-receiver assumption is valid. Suppression remains a later phase; even a suppressed candidate notifier does not bypass the helper's guard.

## Ownership and continuation API

`CloseBusyGlobalOwner` owns one isolated mutable field projection associated with the actual live ConversionSession and its manager identity. All four initial values are required by `CloseBusyNativeFields`' four-argument constructor; default construction is disabled. The owner constructor requires the exact `closeBusyTrueSourceOccurrence` and nonblank qualification. Qualification labels caller-supplied evidence rather than authenticating a native observation.

The class is neither copyable nor movable. The manager binding is weak, so it does not prolong Session lifetime. Each attempt takes an explicit live Session reference, retrieves that Session's actual manager, and verifies both the recorded manager identity and actual manager->ownerSession(). The stored Session address is compared only, never dereferenced. A surviving external old-manager lease cannot authorize a replacement Session, including if an allocator later reuses an address.

`attemptCloseBusyTruePrefix` commits the three stores only at `BeforeTrueStore`, then retains `BeforeTreeEnableSelectionDispatch`. The result is always `TreeEnableSelectionDispatchUnavailable`; `prefixCommittedThisAttempt` distinguishes the first advancement from a retry. Retrying keeps the exact pending boundary and does not replay stores. There is no bypass, false-reset API, arbitrary field setter, notifier callback, modal callback or destructor invocation.

This is an isolated owner projection, not a new binding to process-global storage or an integration into ConversionSession. Constructing it does not prove that can-close guards allowed; a future coordinator must invoke it at the qualified accepted occurrence after those guards. It must also arrange a single actual mutable global owner, preserving the current shared notifier instead of duplicating it. CP58's immutable busy observations are not used as mutable storage. Current public Close remains guarded.

## Focused tests and source freeze

Target: `m2m_mdt_close_busy_prefix`. CTest: `m2m.mdt.close_busy_prefix`.

```text
ctest --test-dir <combined-build-directory> -C Release -R ^m2m\.mdt\.close_busy_prefix$ --output-on-failure
```

Eight Testkit cases cover:

1. Nondefault initial bytes become1/0/0 while an asymmetric32-bit counter remains unchanged.
2. Repeated attempts retain the notifier guard and report no repeated commit.
3. Unknown tree population produces no flag90 reset, map insertion or notification attempt.
4. Suppression does not silently complete the pending notification stage.
5. CP58 actual Mo-null can-close continuation followed by this prefix preserves actual Project/backlink, manager/list/selection/connections, pending timers and live OPI/current guard. The original busy observations remain unchanged.
6. A foreign live Session is refused before any store.
7. Wrong source occurrence, including the false setter, and whitespace qualification are refused.
8. State outliving its original Session cannot bind to a replacement Session even with an externally retained old manager.

Testkit entry point, resolvable quoted includes, Testkit include location, supported CHECK/REQUIRE macros, static ownership assertions and absence of C assert/NDEBUG-dependent checks were verified statically. Whitespace checks passed. These are source checks; executable validation is pending root's build.

| Frozen file | SHA-256 |
| --- | --- |
| close_busy_prefix.hpp | 0525cb171077159e502f0d11a454a94c9b5f7e4332bd18d26d6fc80c18d59712 |
| close_busy_prefix.cpp | 75c841b4464ed3f4a38b30b24c5cd51a4fa6c501ee564127e5a98b810cba4204 |
| test_close_busy_prefix.cpp | c1cf72ca4f10f33cdec5f63cebf899733f25dcb952d094010c0d24266c11018a |

Remaining work is the real global-storage binding, actual TreeEnableSelection notifier/receiver/Qt effects, subsequent null refreshes, accepted manager/Project/OPI teardown, and the independently guarded false setter tail. The false setter can still emit selection=false when main-window busy remains true; no inverse-transition behavior is inferred from this true-only helper.
