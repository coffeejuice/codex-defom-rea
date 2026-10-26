# QPaintDevice destructor prefix before the retained cleanup tail

The narrow normal path is the returned temporary's known painting word0: read
that word, install QPaintDevice class identity, and transfer to the already
retained cleanup tail. A nonzero word cannot yet use that tail: qWarning has
formatting, message-handler/output and fatal-warning dependencies before normal
return. This audit proposes an isolated prefix owner/cursor; it changes no source
or public guard and does not admit another style24 copy/conversion branch.

Owner: editor300. Only this report and unique resume57-paint-device-prefix-*.asm
were written. Static PE reads and DUMPBIN only; no DLL loading, original execution,
GUI, mutable REA, build or tests. Token measurement is unavailable. Tests below
are a proposed contract, not authored/executed tests.

## Qualification and controlling sequence

Original QtGui4_64.dll SHA-256:
`c360c971d292a3c84fc59b8e38733a85a111243f1dbc44737d87b6d744725887`.
Original QtCore4_64.dll SHA-256:
`bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`.
Preferred bases are 65000000 and67000000. DUMPBIN path is
`C:/Program Files (x86)/Microsoft Visual Studio/2022/BuildTools/VC/Tools/MSVC/14.44.35207/bin/Hostx64/x64/dumpbin.exe`,
SHA-256 `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`;
its banner reports14.44.35228.0. Commands use `/disasm:bytes` and
`/range:0xSTART,0xEND_INCLUSIVE` against the pinned file, with PE raw-byte SHA
computed independently. Bounds below are end-exclusive.

Read retained resume48-brush-style24-contract.md,
resume53-editor-style24-next.md, resume54-paint-device-cleanup.md,
resume54-style24-ownership-review.md, and resume56-style24-cleanup-integration.md.
The new dump confirms the exact order, correcting the abbreviated phrase
“installs ... tests” in resume53: the word comparison precedes the class store.

| Address | Reached operation |
|---|---|
| 65138196 | Compare unsigned storage word at actual device+8 with0 |
| 6513819b..a5 | Load vtable656c26d8 and store it at actual device+0 |
| 651381a8 | Branch on the earlier comparison; MOV/LEA did not change its flags |
| 651381aa..b1 | Nonzero only: qWarning with literal656c2740 |
| 651381b7..bf | Restore original device receiver and tail-jump65148840 |

The export table identifies65138190 as QPaintDevice::~QPaintDevice and656c26d8
as its vtable. QtGui IAT65696998 is `?qWarning@@YAXPEBDZZ`; QtCore exports it at
6701cee0. Literal656c2740 is exactly:

> QPaintDevice: Cannot destroy paint device that is being painted

There is no zeroing of device+8 and no destruction/free of this object in this
prefix. The zero and nonzero branches both install the base class. Unknown word
must guard before claiming either the read result or class store. For known
nonzero, the class store has already happened when the warning boundary is
reached; holding before the warning must preserve it. Negative reinterpretations
are immaterial: every nonzero uint16 value takes the same branch.

The zero branch makes no call before tail transfer, reads no module cleanup
counter and touches no warning globals. Native invalid-pointer/access violations,
concurrent object mutation and exception unwind are outside this admitted
valid-live-object profile; this audit does not invent error returns for them.

## Why warning return is a separate dependency

`qWarning6701cee0` preserves varargs and calls6701cd90 with type1. The shared
formatter first calls imported MSVCP90 std::uncaught_exception at IAT671a8458.
When true it uses6701cd00's bounded stack buffer/qvsnprintf path before message
output. Otherwise it constructs/formats a QString, converts it with toLocal8Bit,
assigns the QByteArray, releases formatting locals, then calls message output at
6701ce83. It releases the output array after message output returns. The helper
cannot replace this with a host stderr print just because the literal has no
format conversions. Normal/exceptional formatter details and allocation failure
are not fully recovered here.

`qt_message_output6701cc30` does the following in source order:

1. Load function pointer672f3c38. Nonnull calls it with type/message; null calls
   MSVCR90 __iob_func, fprintf using `%s\n`, then fflush at stream base+60.
2. After that call/output returns, test type. Type3 selects termination. Type1
   calls qgetenv6701c900 with `QT_FATAL_WARNINGS`, then QByteArray::isNull6702d100.
3. isNull is an exact data-owner pointer comparison to shared-null672f0900. A
   QByteArray whose owner differs from shared-null selects termination; the
   shared-null owner permits normal return.
   Do not substitute a string-to-bool or numeric1 test. This audit makes no
   current-environment observation or claim about empty variable values.
4. Release the qgetenv local (including its ref decrement/free path) before the
   termination test. On fatal selection call MSVCR90 exit(1), followed by INT3 if
   that call unexpectedly returns. Only the normal branch returns to qWarning.

The qInstallMsgHandler export6701c090 reads/writes the same672f3c38; its full
leaf window also contains a null-install fallback conditional on byte672f43f4 to
67144ee0. That fallback and its output implementation are not recovered here.
The callback can have arbitrary native effects or fail to return; no callback is
invoked in this investigation. The environment lookup occurs after callback or
stdio output, so an observation before the warning cannot certify its later
value in a broader mutable execution model. A callback's normal return alone
still does not certify qWarning return because the fatal check follows it.

Therefore no cleanup getter, counter read, final opacity publication or QWidget
continuation may be claimed merely upon entering qWarning. These effects require
normal return from the complete warning, including output, fatal check and local
release. The exit branch is a terminating process effect, not a recoverable
converter error or a default dialog answer. There is no evidence here for a
skip-warning setting, a swallowed exception, or unconditional warning success.

## Why the admitted returned image can skip warning

The retained QImage copy650b9a70 calls QPaintDevice constructor65138140 at
650b9a93; its store at6513814c initializes the returned value's word+8 to0.
The ordinary copy reads the source image's word separately at650b9aa3. The
admitted format/alpha predicate reads do not start painting on the returned
value. Consequently its destructor's zero-word branch is derived from its own
construction and intervening operations, not from source/cache observations,
image-data refs, brush aliases or global cleanup-counter values.

Checkpoint56 already projects this exact zero-word read/class-store pair in
EditorBase, then waits for the canonical runtime cleanup owner and selected
existing-counter-zero tail. This audit supports that selected prefix; it finds
no new complete warning path and does not require a new runtime observation for
a value already initialized by the modeled constructor. The remaining weakness
is that returned QPaintDevice identity/class state is only projected as editor
effects, while the image helper still reports its bounded data boundary as
Returned/ImageValueDestroyed. Extracting a retained temporary owner makes that
boundary explicit; it should not be reported as newly admitting active-copy,
pixmap conversion or full native destruction.

## Proposed isolated internal helper contract

Proposed new isolated files, only after root review: paint_device_prefix.hpp,
paint_device_prefix.cpp and test_paint_device_prefix.cpp. No immediate editor or
shared-runtime edit is proposed by this report.

- `PaintDevicePrefixState` represents one actual constructed temporary value,
  independently from its QImageData, brush source and module-global counter.
  Its creator owns the native lifetime; a call holds only a weak reference.
  Host diagnostic leases do not extend the native lifetime. Distinct values
  sharing the same image data get distinct prefix states/calls.
- A qualified `PaintDevicePrefixObservation` carries exact receiver/control-block
  identity, nonempty qualification and optional uint16 painting word. Observation
  is atomic and monotonic: unknown-to-known/equal accepted, conflicting word or
  foreign/rewrapped/dead owner rejected without mutation. Valid construction may
  initialize0 directly through a source-qualified constructor transition; do not
  demand or fabricate an external observation for that known constructor store.
  A future painting transition must be separately recovered, not disguised as
  conflicting observation refinement.
- `PaintDevicePrefixCall(shared_ptr<PaintDevicePrefixState>)` resumes
  `PaintingWord -> BaseClassInstall -> WarningContinuation/TailReady`.
  Issues: OwnerUnavailable, PaintingWordUnavailable, WarningUnrecovered.
  Effects: PaintingWordRead (with retained word), BaseClassInstalled,
  CleanupTailReached. The first two commit in precisely that order, once.
  The class-state store is a modeled QPaintDevice identity, not a fabricated
  host-native Qt vtable pointer or a Qt reference-count event.
- Known0 reaches TailReady with all three effects and no warning call. Known
  nonzero installs base identity, then retains WarningUnrecovered; no
  CleanupTailReached and no boolean “warning returned” injection seam. Unknown
  word has no class effect. Loss of owner while pending blocks further progress;
  diagnostic leases or same numeric aliases in another frame cannot revive it.
- TailReady means only that control reached65148840, not full destructor return.
  A caller still retains its separate PaintDeviceCleanupCall, actual runtime
  globals, private opacity and once-only image-data-release cursor. Observations
  never resume either call. Repeated resumes cannot replay read/store/tail
  transfer; an already completed historical receipt cannot certify a new call.
- Host abandonment must not manufacture a native warning/tail success or perform
  a second image-data release. Prefix state owns no Qt image-data ref, global
  counter ref, mutex, allocation registration or native deletion behavior.

For eventual image binding, the returned-value owner must be created by the
actual image-copy/default-construction frame and carried through data release
into this prefix, rather than created from the cached source at destructor time.
Keep that owner live until cleanup finishes or the call is abandoned. The active
null-copy candidate has two temporary values, therefore two prefix/cleanup
frames; completing only the inner prefix cannot publish the outer predicate.
The current image-helper lifetime summary must be reconciled when this hook is
introduced, preserving its C++ API callers or deliberately updating their tests.

Proposed focused tests:

1. Explicit known0: ordered read/class/tail, base class state committed, word
   unchanged, no warning/global/ref effects; resume is idempotent.
2. Unknown word: no class effect; qualified late0 resumes once; observation
   alone does not execute the continuation.
3. Known1, ffff and representative high-bit words: class install persists at
   WarningUnrecovered; retries never reach tail or repeat the install.
4. Null/dead/foreign/rewrapped receivers, conflicting qualifications/words and
   invalid observations preserve state and cursor.
5. Two call frames with distinct temporary owners sharing one image-data owner:
   independent effects/guards, no extra image-data ref/deref.
6. Native-owner release while a word is unknown blocks later completion despite
   host leases; historical TailReady is a receipt only, not a second invocation.
7. Composition harness with the frozen cleanup helper: prefix TailReady still
   waits for globals; unknown/null/nonzero globals keep opacity private and do
   not replay prefix or data release. Prefix warning guard must not enter tail
   even if the separate counter observation is already exactly0.

The minimum source increment is this isolated known0 prefix and nonzero holding
cursor. Recovering actual warning execution is a separate larger dependency;
keep it guarded. No code change is needed merely to reassert checkpoint56's
known0 arithmetic. Root should choose extraction only if making the actual
returned-value owner explicit is the intended next lifecycle increment.

## Reproducible captures

Each artifact is under analysis/ with prefix `resume57-paint-device-prefix-`.
The first six rows use exact PE runtime-function bounds, which qualify only that
unwind region, not every logical error/unwind path. Leaf windows are explicit
bounded dumps. Native and artifact SHA-256 values are independent.

| Artifact suffix | Native range | Native SHA-256 | Artifact SHA-256 |
|---|---|---|---|
| 65138190.asm | 65138190..651381c4 | e9c60b9bad4b5bc82baf7532e5ec266e5e6a48fb6bd4f3dd7d4b18ee64552aa6 | 2ebfa42dd3f33ca1e652fb1986c9fd71a872c4f8c5cea32924f3c00e18c9190e |
| 6701cee0.asm | 6701cee0..6701cf0f | ce65b021f7e790f47ae7a473f3bb24070f4ff3c17216ab44eb178a1be6226beb | f9eb4fee75c0c6f383241d8428e4282b18be63421733798b677f8e89f02bb9e2 |
| 6701cd90.asm | 6701cd90..6701cea9 | 9517431d8ea76bf570ff06e9db0f3faa3425c5dacb2c5b5a490d42c9a3f5c874 | f9e1ac1dc0d4ba025255aa5b892559f84ca18ecb45f6713e65617aec6fe07685 |
| 6701cd00.asm | 6701cd00..6701cd85 | 1ae2b86754bc555438feda4724e08aa159a6f49166e9db15367f0bfd0233c826 | ea932a170b1ffd27a14ab60b4b96ac138e6a26470d347b020d974dcae56e580d |
| 6701cc30.asm | 6701cc30..6701ccfd | 9cf075dd6480688c84bedc6c65f327eb4af5eec6a5b1f16c11b7109aa5e0a73d | 64821686e035895d6dd76c780ff67cba5cfc8777c345071a9d79b2b7c0b2db57 |
| 6701c900.asm | 6701c900..6701ca25 | af0a628bcf46923d687c4c0c148964fde28f5b79da8f4204d9f41e2b9d26c754 | 412034fee13c966c59dd6a5a06a4c407b2b347ffca8a4ab726906951f1f1b372 |
| 6702d100-window.asm | 6702d100..6702d110 | 8be1ea722665d7c3452c8d5875c7b355ac5c8823c1ef5b260c32e5ceeb69111d | b8c1b4c7881d50a2d1b657dcfd7a1e7880d8dc6339651a636bf5ba95a6fe3bd8 |
| 6701c090-extended-window.asm | 6701c090..6701c0c0 | 61c06a0a82a4ba7f481f5b61120e3277930ae644819abb6a745d72a9d79d6004 | 1945e592d267f309c978659319e8c08ea6ddd3809c31714a2254d8f030e4ec35 |

The earlier 6701c090-window.asm capture ends at6701c0a0 mid-instruction and is
retained only as an incomplete acquisition, superseded by the extended window.
Its native window hash is76af9d02d97d90c3b71d94ff133fc25916277bea18393d1622b3b04430074393,
artifact hash9002e91df8a1cd49689d2ac7ff2aa58d20b99787ef8b0ae55352287c099634b8.
Do not use it to claim complete qInstallMsgHandler coverage. The extended window
reaches its REP RET at6701c0bb. isNull's RET is6702d10d with padding thereafter.

Static identity/import/export/address checks were performed; no new behavioral
validation is claimed. Public mode300 and existing unknown/nonempty gradients,
nonfinite/epsilon radial, image conversion/pixmap/active-copy, cleanup lazy and
nonzero, and later wrapper guards remain intact. Next dependency: root review
of this owner boundary, then either isolated prefix implementation or recovery
of a separately qualified next copy path; warning execution remains excluded.
