# CP160 RDIALOG: timer connection and modal ordering cut

## Decision

The immutable PE evidence supplies a tighter conditional partial order, but no original occurrence that produces the warning answer. Preserve the public must-be-last warning guard. Default focus still cannot supply an answer, a timer schedule, or an empty event interval. No source/build/test/GUI/live REA operation or mutable database was used; this report is the only written artifact.

This extends CP156 in two useful ways: a delivered single-shot timer is not necessarily an executed `modelUpdated()` callback, and the relevant dialog interval includes show and unwind effects surrounding the nested loop. An observation ending at input activation or `QDialog::done` is insufficient to certify the state returned to Save.

## Source-closed conditional ordering

The selected successful Open route still requests two distinct 1000 ms single-shots from `0x140170430` to the same diagram receiver. This path-qualified request count does not prove successful connection, native registration/readiness, delivery, or callback count.

The retained QtCore constructor `0x67134870..0x671348fd` establishes this local order:

```text
construct dispatcher-parented single-shot object
  -> QObject::connect(timeout, receiver, slot, raw type 3) at 0x671348d3
  -> startTimer(interval) at 0x671348dd
  -> store returned timer ID at object+0x10, 0x671348e2
```

There is no connection-success test between connect and startTimer in this constructor. The normalization selection `0x67131625..0x6713163a` converts raw type 3 to 0 (Auto). Thus scheduling a timer does not certify that its receiver connection succeeded. The existing timer contract separately records `startTimer`'s zero-ID/no-dispatcher branch.

For an actual timerEvent entry at `0x67134ca0`, a positive native ID is killed at `0x67134cb0`, its field is set to -1 at `0x67134cc5`, and timeout is activated at `0x67134ccc`; only after activation returns does `0x67134cd9` tail-call deletion. This is a consumed-before-activation order, not proof that a receiver slot ran.

The retained activation body `0x6712ec70` distinguishes three additional facts. It tests the signal-blocked flag at `0x6712ed15`; skips a cleared receiver at `0x6712eed3..0x6712eeda`; then compares the receiver's thread identity against the activation thread and tests the stored connection type at `0x6712eee5..0x6712ef19`. Auto with unequal thread identity selects the queued helper `0x6712c310` at `0x6712ef38`. Equal-thread Auto takes the synchronous continuation beginning `0x6712f108` instead. These are static branch facts; the current task does not assert a cross-thread diagram view or recover the complete queued helper.

Consequently the safe cut is:

```text
qualified timerEvent occurrence
  -> kill positive ID; commit ID=-1
  -> timeout activation
  -> [actual connected/live/unblocked receiver and actual connection/thread branch]
  -> [actual modelUpdated entry, if reached]
  -> fit first; resolve then-current model/item; selection/editor consequences
```

For an admitted same-thread synchronous connection, callback execution lies inside timeout activation and precedes single-shot deletion. For queued delivery, timeout and slot execution require separate occurrence identities/order; deletion of the single-shot is not a certificate of slot completion. A disconnected or signal-blocked timeout need not invoke the slot at all. A null current model is another case: the invoked slot fits first and then skips item selection. Do not collapse these cases into `delivered=true`.

No order between the two different timers, Save's poll, or answer activation follows from either the two registration requests or their equal requested interval. The already implemented explicit timer-dispatch seam must not receive invented due IDs from these facts.

## Modal interval must include show and unwind

`QDialog::exec` clears result private+0x22c at `0x654594a7` and invokes virtual show at `0x654594ba`, before the nested `QEventLoop::exec(0x40)` call at `0x65459507`. After that loop returns, it performs restoration/cleanup and reads the result at `0x65459556`. Therefore "modal entry" needs distinct markers for warning call, exec entry, show, nested-loop entry/exit, and warning return. Beginning instrumentation only at nested-loop entry misses the earlier show path.

CP121's static click-to-`done(int)` connection and result mapping remain valid conditionally. `done` stores the result, exits the loop, and emits completion signals. An exit request is not itself the end of all synchronous completion/unwind effects. Record callbacks through warning return and Save return, not just those before the activating input. This report does not claim a timer necessarily fires after activation, or enumerate every callback family.

Once a qualified warning actually returns Yes `0x4000`, the original Save caller's integer comparison supplies the existing closed suffix: return before current-operation Save and output writers; nested normal Close maps that Save Yes to Cancel. The suffix cannot manufacture its own preconditions or erase earlier callback effects.

## Smallest resolving occurrence witness

One fresh, owned-original Save occurrence reaching this exact warning is sufficient for this branch; a separate outer Close capture is unnecessary unless Close behavior is claimed. Bind executable/Qt hashes, input/copy identity, process/thread, receiver, active model/item/editor and monotonically ordered occurrence IDs. Retain:

1. Correlated Open single-shot objects, connect success/receiver/method/type, native timer-registration result and lifetime. Distinguish requested timer, registered timer, due timer, timerEvent, timeout activation and receiver-slot entry. A qualified start-state snapshot plus complete earlier-delivery/lifetime history may replace recording every constructor instruction.
2. Save poll entry/exit at original `0x140098129`, followed by warning call, dialog exec/show and nested-loop entry/exit. Record pending/due state at the CP156 boundaries; do not label an undelivered timer "not due" merely from missing callback output.
3. A gap-free event/callback transcript across those boundaries through warning/Save return, with nested dispatch depth. For each timeout/slot, retain the native/logical timer correlation, connection/liveness/thread branch and then-current receiver/model/item/editor before and after its effect. Include other relevant callbacks rather than assuming this family exhausts the loop. Record an explicit zero-delivery interval only with instrumentation-coverage/loss evidence.
4. Actual activating input and selected button, `done(int)` argument, exec return, QMessageBox mapping, final Save return and writer-entry coverage. Retain show, completion-signal and unwind callbacks, including any after activation. Loss, unknown affinity, absent connection evidence or uncovered intervals invalidate a complete replay profile.

An actual delivered-event transcript can replay that occurrence without guessing elapsed time. It does not establish the entire due-but-undelivered set. If a profile claims a due set rather than only delivered events, it additionally needs a qualified readiness observation; an empty delivered list is not an empty due-set proof. The public guard stays closed until the required profile is validated. This report proposes no new automatic answer or implementation authorization.

## Byte receipt and qualifications

Whole-file SHA-256 was recomputed directly from `evidence/original-binaries/` using Python standard-library PE section mapping. Raw selections below are half-open preferred-image VA intervals; they are byte identities, not complete logical-function claims. Retained DUMPBIN 14.44.35228.0 text supplied instruction interpretation; no target/DLL was loaded.

| Artifact | Recomputed SHA-256 |
| --- | --- |
| DEF_GUI_MO_64.exe | `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc` |
| QtCore4_64.dll | `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160` |
| QtGui4_64.dll | `c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887` |

| Raw PE selection | File offset; bytes | SHA-256 |
| --- | --- | --- |
| QtCore `67134870..671348fd` | `133c70`; 141 | `5c18bff0069361403edf333affc137fb21dae5a0fde9d0939e7c2792fc7b954c` |
| QtCore `67134ca0..67134cde` | `1340a0`; 62 | `0475c3932940d2e3a86ad251a410482ef8e37e4491276d02352258f9f1fab27b` |
| QtCore `67131625..6713163a` | `130a25`; 21 | `aaf86f58051e8526d3d58463262072e7b57c2294793851211c84e6b769185c79` |
| QtCore `6712eed3..6712ef54` | `12e2d3`; 129 | `bf5a8967c1ce0f804723662ea9a12ea5ba0ea442c85d37fe933ffefffdbc4d5f` |
| QtCore `6712f108..6712f159` | `12e508`; 81 | `582c8e1c967502c5b6ccfa625df8f27ad1458db55195aa873053e845f2b105f2` |
| QtGui `654594a3..6545950d` | `4588a3`; 106 | `7e5d9b0e274daeedfae292e165e9501e3b777a3d01863e6803e78cac45385203` |
| QtGui `65459530..6545958b` | `458930`; 91 | `04fe45a33bd61054f95a67f3c61c8907ee22d959e12215e32218fc80159febfb` |
| QtGui `654595a0..65459642` | `4589a0`; 162 | `ae6650e56ef1534e84255c124ab253285447c7180d3ebb147328f6bf080a3794` |
| Original `140170430..14017086f` | `16f830`; 1087 | `19bcb242fdde81ff2755044fa588d8bcd4e704768c2762a4534de392fa85d8ef` |
| Original `1400980d0..1400982ac` | `974d0`; 476 | `16c31d2deb0724547b82bf35ecd5384173cc60f1b38f2c4a962997dd9c2b8045` |

Retained interpretation sources, recomputed text hashes: `wave3-boolean-close-flush-6712ec70.asm` = `3d795e15af97ce1582cd8688d42e7db1d9948481ea7c6694a9b9c0ba1e1e74a9`; `wave3-boolean-close-flush-671315c0.asm` = `53f7076dc086f1ef04ac80f9b5a688b91cc75fc13c6b0f0357535f5caefe8028`; `wave3-boolean-timer-67134870.asm` = `a9940d798c6fc735ea25b931234d954699fdcfce17e84a51f2fa00382cd16648`; `wave3-boolean-timer-67134ca0.asm` = `f80b697f34b4a1151dcb7880829e411455a08374e846343ae55da017c8f6d39a`; `wave3-closure-next-65459420.asm` = `4fe2a1475ade838554e36f703e41717c6b3044b88ac5a6ae7dcd7b40506122bd`.

Controlling qualifications remain in `wave3-boolean-timer-contract.json` (recomputed `63ee9c2bca39e6b14ba2866a0ffc7a8caf5b389f49d4b41530d84e4f6145cdb0`), CP121 (`ba7bc7ebb7649146d34c5edca1c91aa7c4720bfe6689b513d589c0cc1f348550`) and CP156 (`27f31a2b4cc5110082f793d7cb09a10d78b19c48396e60a02d092f791c7bbccd`). Earlier contracts' unqualified wording that default Yes "returns" is conditional on actual activation/return, as corrected by CP121/141/156. Static selection coverage is not complete callback-family coverage or runtime observation. Runtime occurrence remains missing; RDIALOG acceptance remains incomplete.
