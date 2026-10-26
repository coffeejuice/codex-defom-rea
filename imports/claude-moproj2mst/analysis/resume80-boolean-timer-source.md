# CP80: Boolean timer registration, readiness and callback schedule

## Source-bounded chain

On the selected successful fresh Open route, the lazy diagram view receives two non-null model bindings at virtual `0x140170430`. Each requests `QTimer::singleShot(1000, this, modelUpdated())` at `0x14017085f`; the calls are distinct and the selected body has no equality shortcut, saved ID, cancellation, or coalescing.

Qt's retained implementation then:

1. `67134d90 -> 67134870` allocates a `QSingleShotTimer` whose parent is the current thread event dispatcher. It connects timeout to the diagram receiver/method. The requested raw connection type 3 is normalized to Auto (0) by `671315c0`.
2. `671348dd -> 6712b1e0 -> 6710c5a0` registers the 1000 ms interval on that dispatcher and returns a timer ID. If there is no dispatcher, the timer ID is zero and no registered delivery follows. Reaching the Open call site alone therefore proves a request, not a successful registration.
3. A timer event at `67134ca0` kills the timer's stored positive ID, sets it to `-1`, activates timeout, and deletes the single-shot timer. The retained handler does not compare an incoming event ID with its stored ID. Do not synthesize that comparison in a candidate model.
4. Timeout activation still has gates: sender blocked-signals bit at `6712ed15`, live receiver pointer at `6712eed3..eeda`, and emitter/receiver thread plus connection mode at `6712eef5..ef38`. Auto connection invokes directly only on the selected same-thread route; a cross-thread route posts queued activation, so timer-event handling is not automatically equivalent to `modelUpdated()` entry.
5. The diagram metadata slot resolves local 23 to `modelUpdated()` (`14018bb30 -> 14018c194 -> 140183160`). It fits first, then reads the receiver's **current** model and current item. It may emit the current item selection/update chain only if those reads and downstream project/loading/editor/notifier gates pass. The timer carries the receiver/method connection, not a registration-time item snapshot.

## Save and Boolean-write ordering

The native Save body calls `QCoreApplication::processEvents(QFlags)` at `0x140098129`, flags zero, before subproject validation/current-operation Save. The selected Qt overload calls the current-thread dispatcher once; its Win32 dispatcher path is a zero-timeout poll. It may process a timer message that is already due, but the flags do not request a blocking wait for the 1000 ms interval. The normal Open parse pumps precede the later diagram timer requests. Other indirect Open descendants are not callgraph-exhausted, so this does not prove that no intervening nested loop can deliver one.

The native Boolean helper `0x1411e1820` is the side-file attempt boundary. If an Open timer callback completes before entry to this helper, its conditional current-item/editor changes can precede Boolean BWZ/INI writes. Static writer ordering and the Save poll do not establish that such a callback occurred, that it selected a Boolean item, or that the Boolean helper was reached on the same selected native branch. Candidate source writer locations are secondary corroboration only (`convert.cpp:377-378`, `ops/boolean.cpp:139-143`).

## Exact unresolved event sequence

No retained original capture closes this ordered chain:

`Open binding call` → `QSingleShotTimer allocation + successful dispatcher registration (receiver, thread, returned timer ID and deadline)` → `dispatcher receives due timer event` → `timerEvent` → `sender/receiver/thread-mode activation gates` → `actual direct slot invocation or later queued invocation` → `modelUpdated()` fit completes → `current model/item read and any relevant selection/editor effect` → `native Boolean helper 0x1411e1820 entry`.

The exact missing dependency is the native due/event occurrence and callback phase relative to `0x1411e1820`. Neither 1000 ms delay nor two registrations determines elapsed time or readiness by Save. The flags-zero poll does not infer an empty due set. A declared candidate schedule, including an explicit empty list, cannot stand in for the original occurrence.

## Smallest resolving capture

Once the GUI lifecycle release gate permits a controlled run, use one fresh original process and one monotonic event timeline. Record:

- both `0x14017085f` calls and each `QSingleShotTimer` registration result/returned ID, current dispatcher/thread, receiver identity and interval;
- timer event entry, timeout activation gate outcomes, direct-versus-queued choice, and actual `0x140183160` invocation (or proof it was not reached before the writer);
- after fit, the receiver's current model/item and selected operation/editor identity needed to determine whether Boolean state changed;
- native Save poll entry/return at `0x140098129`, and first Boolean helper entry at `0x1411e1820`.

The essential observation is an actual callback invocation (or a gap-free interval proving none) ordered before/after the Boolean helper. For a queued activation, record both post and receiver invocation; posting alone is insufficient. The capture should include intervening rebind/destruction events and retain the answer whether the callback occurred before Save entry, in its one-poll dispatcher call, in another nested loop, or after the writer.

## Evidence and scope

Static basis: `analysis/wave3-boolean-timer-contract.json` and its bounded `wave3-boolean-timer-*.asm` regions; `analysis/wave3-boolean-open-sequence-contract.json`; `analysis/resume72-boolean-first-open-timer.md`; `analysis/wave3-boolean-save-io-contract.md`; and `analysis/resume79-boolean-save-timer-readiness.md`. These establish source mechanics and conditional branches, not native timer readiness or a historical callback occurrence. Historical captures contain no registration/ID/due/callback sequence through Boolean helper entry. No GUI, mutable REA, source, build, or test action was performed.
