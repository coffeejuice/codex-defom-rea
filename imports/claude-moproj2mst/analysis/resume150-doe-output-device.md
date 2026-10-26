# CP150: Regions directory-device selection and cache ownership

The next native device is not selected by path syntax alone. QDir construction resolves and retains an actual engine pointer before the first root classification or enumeration. QFileInfo then resolves an independent engine pointer for each constructed information object. Those same-occurrence results are absent from the candidate. There is no source-closed production cleanup profile yet, even for an ordinary absolute ASCII path; retain CP149's guard rather than selecting the native branch by policy.

This report follows `resume150-doe-output-regions-next.md`. It changes no source/test, invokes no compiler/build, GUI, original runtime or REA/database operation, and keeps the released CP149 files held. All addresses below are preferred QtCore evidence addresses, not observed pointers in a running process.

## First exact unresolved pointer: QDirPrivate+0x20

The fresh QDir path constructor 0x670b0f50 invokes private construction 0x670b0cb0. That initializes its engine field private+0x20 to null, but **does not leave it there as an invariant**. Its path setup calls 0x670b0080.

Path setup normalizes the supplied path, initializes/replaces its file-entry fields, zeros metadata state at private+0x70, and calls resolver 0x670f0080 at **0x670b01a6**. It compares the returned RAX with the previous engine at 0x670b01af; a changed result is published at **0x670b01b4 -> private+0x20**, with deleting-virtual cleanup of a replaced nonnull engine. The constructor's initial zero cannot certify the completed QDir engine as null.

Resolver 0x670f0080 initializes its output engine null, calls 0x670efbc0 at 0x670f00e4 with existence-check flag0, conditionally publishes the resolved entry, cleans its temporary QString/file-entry leases, and returns the engine output. CP121 already closes its first canonical handler gate: 0x670a7dd0 reads flag **0x672f3ed0** at 0x670a7df7. Flag0 bypasses all handler callbacks; flag-nonzero traverses the canonical handler list with virtual+8 callbacks. For normalized drive/UNC syntax and qualified flag0, the resolver returns null engine along the native path without an existence probe/search-prefix rewrite.

CP122 established why the static module inventory does not prove that flag0 survived actual startup/plugin/event activity. CP148 additionally exposes plugin-path setup before named-codec setup; a process profile must not silently equate lack of an ordinary handler import with absence of runtime handlers. No new scan or observer was used here to promote that old uncertainty into knowledge.

The minimal missing occurrence input is therefore the returned engine at **0x670b01a6**, bound to this actual QDir's normalized path/private lifetime and the selected Output Save call. Observing the earlier constructor zero, a historical handler byte, or the fact that mkdir succeeded does not supply this return. An actual null return can qualify this object's native path independently of proving a whole-program no-handler history; it still does not certify separate later engine resolutions.

## Cache and iterator ordering after that selection

The CP150 application helper first obtains QDir::absolutePath, constructs QFileInfo and asks isDir before entryList. AbsolutePath export 0x670ad5c0 calls 0x670accb0 for its cached absolute entry, then obtains the corresponding QString through 0x670ef040. Its body contains distinct native and custom-engine paths, including a virtual+0xa0 call for the latter. The QDir pointer must be qualified before treating this path computation as a plain host string operation.

For matching name/filter/sort configuration, entryList 0x670b0370 calls cache producer 0x670aed70. That producer checks **private+0x28** at 0x670aed84. A populated cache avoids a new iteration. A fresh path setup explicitly set this flag0 at 0x670b01c7, so this first query must populate it:

1. Construct an iterator through 0x670b3c30 at 0x670aedb7.
2. Test/advance it using 0x670b2980 and 0x670b3800; get its current QFileInfo through 0x670b27a0 and append the observed objects to the working list.
3. After exhausting that iteration, call sort/list projection 0x670aeaf0 at 0x670aee50.
4. Only after that normal return set **private+0x28=1 at 0x670aee55**. Iterator destruction follows. The caller returns a retained name-list snapshot; it does not delete while streaming the filesystem iterator.

Source defaults from the prior report remain filters7/sort0x10/wildcard `*`. The sorting routine skips sorting when list count<=1 or low sort bits==3; otherwise it builds temporary rows and invokes sorting helper 0x670adf60 at 0x670aec0b with the actual sort flags. Full comparator/tie/failure semantics are not closed by this call edge. A host enumeration order or generic case-insensitive sort is not an established substitute.

The iterator's engine rule is more specific than simply copying QDirPrivate+0x20. At 0x670b3c75 it tests that pointer and passes the Boolean `pointer != null` to private iterator constructor 0x670b38e0. The new iterator starts with engine field+0 null. If that Boolean is false, it skips a new resolution and stays on its native path. If true, it **re-resolves** its own copied file entry at 0x670b3b93 through 0x670f0080 and stores that result in iterator+0. It does not just borrow the old QDir engine pointer. That second result can itself be null or nonnull and has its own ownership/deletion boundary.

When iterator+0 is nonnull, 0x670b2f90 calls the engine's virtual+0xc0 and virtual+0xd0 to configure/obtain its entry iterator. Advancing uses that returned iterator's virtual slots at 0x670b3473/348c/34b6/34ca. Those callback returns remain actual device inputs.

When iterator+0 is null, 0x670b2f90 instead constructs the native filesystem iterator at **0x670b3138 ->0x671028e0**; native advance is **0x670b35eb ->0x67102b20**. The surrounding code turns returned file-entry/metadata into QFileInfo, applies helpers 0x670b31d0 and 0x670b24f0, and publishes accepted entries. This closes the branch and first native call targets, not the concrete Windows read errors/filter results. Failed/empty host iteration must not be classified as a successful empty native snapshot.

## Each QFileInfo has separate classification state

The application helper's root QFileInfo and each `QFileInfo(QDir, name)` are fresh objects, not borrowed directory-entry metadata from the prior QStringList. The latter constructor export 0x670b9b10 obtains a path through 0x670abda0 and invokes private constructor **0x670b8030**.

That private constructor builds a normalized file entry at +0x08, zeros known-metadata/result words at +0x20/+0x24, then **calls 0x670f0080 at 0x670b808e and stores the result at private+0x50 at 0x670b8093**. It clears status bit0 and sets caching bit1 in byte+0xb4. This is a new resolver occurrence. Null QDir engine is not proof that these QFileInfo engines are null, especially if callbacks/global state change between operations.

`isDir` 0x670b8fa0 and `isFile` 0x670b8f20 first reject their invalid-state bit. If engine+0x50 is null, they check their own cache-known bitmask; an unknown required bit invokes native metadata fill **0x67102090** with mask0x40000 for directory or0x20000 for file. The return reads the corresponding bit from metadata result+0x24. A cached bit avoids that fill on the same object. Nonnull engine takes helper **0x670b7950** instead. No type result can be fabricated from a name or from a prior object's isDir query.

The application-level recursive failure order is already closed in the preceding report. This independent classification state explains why even a complete supplied entry-name list would not yet implement its device.

## Removal and navigation do not use one universal delete primitive

`QDir::remove` export **0x670ac3b0** rejects an empty name, joins/resolves the requested path through 0x670abda0, then calls static QFile removal **0x670b61d0**. That wrapper constructs a QFile, assigns its filename and invokes instance removal **0x670b5f30**. The instance method obtains its actual engine through 0x670b58b0 and calls virtual+0x10 at **0x670b5f8d**; it also has close/error/cleanup work. Thus application-level ignored Boolean does not make this an unqualified DeleteFileW operation or erase its engine ownership.

`QDir::rmdir` **0x670abfc0** uses a different decision. After path resolution it reads its existing QDirPrivate+0x20 at 0x670ac001. Null invokes native **0x670ff1c0** at 0x670ac01f with second argument0; nonnull calls that engine's **virtual+0x68** at 0x670ac078 with the path and argument0. These normal Booleans are ignored by the application helper, but the calls and attempted effects still occur in the specified order.

`cdUp` export **0x670b1600** constructs the `..` argument and invokes QDir navigation **0x670b11d0**. That routine has path/classification/detach updates and possible false returns. The application propagates a false cdUp and preserves earlier removals. It cannot be replaced with lexical parent extraction that always succeeds. Its full navigation/device error behavior remains a later static dependency after the first unqualified engine return.

## Admission decision and concrete next edge

No production cleanup implementation is assigned or justified by these static branches alone. The first exact missing pointer is the same-occurrence QDir resolver return published at private+0x20; separate QFileInfo+0x50 and QFile removal engine returns must then be covered at their own calls. The canonical handler flag/list blocker is the same previously bounded CP121/122 state, now tied to this **new actual QDir producer**, not a reason to repeat import scans indefinitely.

A future narrow native profile could admit only a qualified null QDir engine, the iterator's resulting native branch, separately qualified null QFileInfo engines, and actual native filesystem read/classification/removal results for that named ordinary absolute path. It would also need exact sorting/filtering and cdUp behavior. This report does not grant such a profile from ASCII, drive/UNC syntax, lack of visible plugins or successful mkdir. Nonnull engine callbacks must remain guarded unless their actual owner/virtual returns are recovered.

The smallest evidence action is an independently authorized same-occurrence resolver-return observation at 0x670b01a6 with the actual Output caller/path/object identity, followed by separate classification/removal occurrences if execution reaches them. No observation is started here. If runtime evidence remains unavailable, static work can close conditional native iterator0x671028e0/0x67102b20 and sorter0x670adf60, but it still cannot choose them for this unobserved original invocation. Current CP149 wiring must stay at its explicit guard rather than grow another caller-supplied-entry helper.

## Compact immutable receipt

QtCore SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`; GUI import identity is the retained `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. Import/export names were checked against these files. The byte ranges below were read/decoded from their actual entries with bounded DUMPBIN; no analysis database was opened.

| QtCore preferred VA / bytes | SHA-256 |
| --- | --- |
| 670b0080 /494 | 3e539fc3965184108be93bd3d0b9500a3b598a42e56c2e7f9f387251b13a7e80 |
| 670f0080 /245 | b2a47f8910f3833f4e1d70470499ef4c313c1fe47bb95f6f6f48815b0667e6b2 |
| 670b3c30 /194 | 8ec1a1664b947ddf385b85ddcb840a08b860e4caf031156a59b35edfa5303e62 |
| 670b38e0 /837 | 648c5d1d5768fb3d04b21a60d1c5feecc0b7063eda8cf1f09ac73b5ea75c1aab |
| 670b2f90 /563 | faf328eba32f7898f087662ca2743be461c7121870d2e6e3d929cd6a282b02b9 |
| 670b33f0 /935 | d49ecaa12eef514ae6c998493ee3d1482de718f57c106937990f57252b71be4e |
| 670aed70 /322 | 1f668dccdf5aa73667a7668d751234ee021858c2778bf524030d2dea6d902360 |
| 670aeaf0 /636 | 31601dc828809b495d3917fb88c12db14e6e238c4689d0f642bca44614b952cf |
| 670b8030 /272 | 96cea90fbc376d425a7e43d474d8c7a650dd10254a15fb459b41dbf9aec158e2 |
| 670b8fa0 /118 | 97e59e2b7df22a586631b9c9d06da8540690d94b91850f66a996a457cf052c4d |
| 670b8f20 /118 | 5426801ca2c4f3a79ff1e8197fea993c7b2699a02d3d2466de12cdb87574287f |
| 670abfc0 /225 | 3d2ba6afd037488bcd375118c2022523e7398094207073ea895e2af9db5be43a |
| 670ac3b0 /94 | c02b2ad03a6f29aeec83949962cf353b6254d6e5d5e97688bc9c6a657ab1a032 |
| 670b61d0 /157 | 740f219a422327dc1ab8ec06da8f318439912bb0bf9bf6698617614d34d2ffb8 |
| 670b5f30 /211 | b9f60c84838c1783537474028b7e5699ca1723fdb9a65cd31b74d325c3dd83f0 |

Some Qt routines have adjacent/split unwind regions; bounded normal bodies were inspected at their actual entries. This receipt asserts only the stated decoded call/field/cache relationships, not whole-program callback absence, native allocation/unwind parity or a successful original cleanup occurrence. Earlier CP149/150 reports retain the application caller and local recursion return receipts.
