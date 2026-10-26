# CP75: actual empty Session manager tail

The new isolated helper clears the actual Session-owned ModelManager only when its real list and connection metadata are empty, then stops before native029a83's post-clear Project lookup. Every nonempty population stays blocked before child teardown, including the failed-Open boundary where a registered Mo child still has moProject=null. Public Close remains guarded.

CP74 freeze was confirmed before the first file write. Only the three new source/test files below and this receipt were written. No shared Session/header, ModelManager implementation, CMake or plan changed. No compilation, build, CTest/test executable, GUI, original execution or mutable REA operation ran. Source ownership is released; these code files are frozen for root validation.

## Implementation and source bounds

`CloseEmptyManagerTailProgress` binds one actual Session and its canonical manager/control block at `0x140029a76:0x140c78300`. Construction requires nonblank provenance but does not authenticate a native observation or certify prior accepted-Close guards, busy changes or refresh effects. Progress cannot be copied/moved and has no bypass/completion setter.

`attemptCloseEmptyManagerTail` obtains the manager through the existing `ConversionSession::modelManagerOwner()` accessor. It inspects actual model identities and connection count. Nonempty population latches `OwnedChildTeardownUnavailable`; zero models with existing connection metadata latches a separate guard rather than allowing `ModelManager::clear()` to erase an unqualified entry. That defensive connection case is not manufactured by a test-only private-state mutation.

Only zero models and zero connections call the existing real `ModelManager::clear()`. Its owning-list clear precedes explicit current reset. The result is `BeforePostClearProjectLookup`, not Close success. Retry retains that boundary and never clears again. Blocked retries retain their original population/ordinal identity snapshot; changed size/order/pointers or connection count throw `M2M_E_UNSUPPORTED` before effects. Clearing a list externally cannot satisfy blocked child teardown, and an old completed occurrence cannot erase later model arrivals. Raw child snapshots are diagnostic comparisons only, with no dereference or allocation retention.

The native qualification is CP74's retained manager direct body140c78300..140c7854b, code SHA-256 `b2c665e2182ec5050431091d4859aafd7d86e1ca1ca2e4e7d14a36c5bf7b1487`, from original DEF_GUI_MO_64 v11.0.2 SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Empty iteration skips child virtual+18, reaches iterator cleanupc7851a, list clearc7852b, then guard resetc7853e. The helper implements the available semantic owner projection; it does not reconstruct physical Qt QList/QPointer bookkeeping or transitive iterator/container allocation effects.

No Project release, diagram null binding, receiver/dispatcher teardown, event delivery, model selection, post-clear lookup, lazy Project reconstruction, notifier, busy/status/global reset or native QObject destructor is called. Native current reset is not substituted with `selectModel(nullptr)`. Previous modal/default/accepted-prefix, nonempty child cleanup and later status-item ownership dependencies remain unresolved.

## Focused tests and static preflight

New target: `m2m_mdt_close_empty_manager_tail`. CTest name: `m2m.mdt.close_empty_manager_tail`.

Ten Testkit cases cover actual fresh empty Session entry and retained retry; Project/slots/registry/scenes/Display/MainView/WidgetRuntime/notification/remesh preservation; registered child before native Project binding; current child/backlink preservation after construction/selection; blocked retry after external clear; same-count replacement; later arrival after completed empty tail; foreign Session and invalid occurrence/qualification; escaped progress with an old retained manager; and unchanged pending timer/receiver identities with diagram attachment retained.

All source/test quoted includes resolve against their actual source directory or converter include/src directories. The test has `testkit::run_all()` main and only supported CHECK/REQUIRE/TEST_CASE macros. No C assert/NDEBUG-dependent checks were added. New files passed direct trailing-whitespace checks; `git diff --check` also returned zero. Static control-flow checks confirm the single real clear call follows both empty guards and the helper contains no Project/event/selection effects. Fixture path strings are below100 characters under C:/PROBLEM; the exercised dbSetPath stores/normalizes paths without writing files.

These are declared candidate owner/phase fixtures, not original modal/lifecycle captures. The empty helper does not prove native accepted-Close reachability. No executable test result is claimed. Root can validate the focused target with:

```text
cmake --build <combined-build-directory> --config Release --target m2m_mdt_close_empty_manager_tail
ctest --test-dir <combined-build-directory> -C Release -R ^m2m\.mdt\.close_empty_manager_tail$ --output-on-failure
```

## Frozen source receipt

| New file, relative to source/apps/moproj2mst | SHA-256 |
| --- | --- |
| include/m2m/engine/close_empty_manager_tail.hpp | `85e5c39e300ef8c7de375f0ee22b18c2f2b170068aefd929af704386e0b39a07` |
| src/engine/close_empty_manager_tail.cpp | `3b6dddd574342c44342e7ab8c321747c35807244205ab97904e706b16fc029d0` |
| tests/mdt/test_close_empty_manager_tail.cpp | `993d2c9d7daeab7e58c0b1022b6f0ec2f2af967e85589a3c303c4b6b689bac16` |

Root's source-stable build/full suite and archive are pending; source files will remain unchanged until that boundary is released.
