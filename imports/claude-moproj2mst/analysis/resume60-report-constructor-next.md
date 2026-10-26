# Checkpoint 60: smallest Report constructor field independent of GUI scheduling

## Finding

One constructor word transform is statically qualified and independent of GUI event scheduling: QObject private `+0x20`. The retained QtCore private initializer at `+0x131160` reads the existing DWORD, applies `AND 0xfffffcf0`, then `OR 0x60`, and writes it back (`+0x131224..+0x13122f`). The pure helper already exists as `m2m::doe::isolatedQObjectPrivateFlags(previousWord)` in `source/apps/moproj2mst/include/m2m/doe_qobject_private_flags.hpp` and `src/doe/qobject_private_flags.cpp`.

This is the smallest isolated constructor state operation because it consumes one explicit supplied value and returns the exact transformed value. It has no TLS, QThreadData, shared-ref, event-dispatch, or process-count dependency. Existing focused tests in `tests/mdt/test_doe_qobject_private_flags.cpp` check representative values and preservation/clearing of the written bits. These tests establish only the pure function's arithmetic.

## What is not qualified for native application

No retained runtime capture supplies the prior value at a Report private allocation. The live probe004 receipt stops at first editor-grid publication, while the static Report QObject capture plan is not executed. The immutable constructor code shows that the `0x88` private block is allocated and then initialized; it does not establish its pre-initializer `+0x20` value. The adapter's `PendingTaskPrototype` is a separate `0xc0` descriptor, not this private allocation.

The other statically written fields do not provide a smaller usable native update. Null/zero stores and shared-container pointers occur inside the same private initializer, but applying them requires the actual private block and would execute part of the guarded QObject prefix. Shared-container increments additionally need the common `WidgetRuntimeState` owner. Backlinks and current-thread/TLS selection occur after the initializer and bind to real object/private/thread identities. None is available from a GUI-independent synthetic default.

Thus the field operation itself is qualified for pure helper use, but no actual Report constructor field write is currently qualified for integration. Do not manufacture a prior value such as zero, copy the helper result into `PendingTaskPrototype`, or use it to advance `qobjectBaseCompleted`.

## Bounded next step

No implementation change is needed for this field: the helper and focused arithmetic tests already exist. The next new evidence must be a passive same-frame capture at the actual Report call that records the allocated private pointer and its old `+0x20` word, then observes the exact transformed result before returning to the public constructor. That single field can be checked independently of GUI schedule state, but the QObject constructor must still remain guarded until shared-ref, backlink, TLS/QThreadData, process counter, and lifetime ownership prerequisites are closed.

Do not add a GUI event schedule or event-pumping model to test this helper. A synthetic test with an explicit input word is sufficient for the helper's contract and cannot establish native constructor reachability. No source/shared edit, build, GUI action, or mutable REA operation was performed for this audit.
