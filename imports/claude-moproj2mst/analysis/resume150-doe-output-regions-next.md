# CP150: exact Regions traversal and the next directory-device boundary

The application-level cleanup algorithm is now qualified, including its different treatment of recursive failure and ordinary removal failure. It cannot be replaced by a host recursive-delete call. The next wired effect after CP149 needs a Qt-compatible directory observation/removal device: the caller constructs a QDir, the helper first probes its absolute path through QFileInfo, then obtains a sorted, filtered entry-name snapshot. The current candidate has no such device. Keep CP149's enumeration guard and its frozen source unchanged until that contract is closed.

This lane changes only this report. It used bounded read-only DUMPBIN decoding of retained immutable GUI/QtCore files and existing source. No source/test edit, build, executable test, GUI, original runtime or REA/database operation occurred. Root's CP149 source remains held.

## Actual active-subproject re-fetch and call position

CP149 already qualifies mkdir return followed by the actual current-operation model lookup 0x14005f770. Native 0x140e2e730 stores that result at stack+0x30; null returns the whole callback. With a nonnull model it then:

1. Calls the class's active-subproject accessor 0x140e32740 at 0x140e2e79d and stores its returned pointer at stack+0x50. This is a **new lookup after the model getter**, not permission to reuse the earlier mkdir helper's SUB or Study snapshot across intervening calls.
2. If this fresh SUB is null, branches at 0x140e2e7ad directly to 0x140e2e922, skipping only the Regions block. It does not return the callback; subsequent Common_Mesh path helpers remain later dependencies. Do not confuse this branch with the earlier null operation-model return.
3. If present, obtains its Control path via 0x1400ba350 at 0x140e2e7bd and assembles Regions using the retained QString/path helpers.
4. Constructs `QDir(QString const&)` at 0x140e2e8bf, makes a by-value QDir copy at 0x140e2e8e3 and passes that object to 0x14081b400 at 0x140e2e8f9. No QFileInfo entry list has yet been produced merely by possessing the supplied Study or path value.
5. Ignores the cleanup helper's return: 0x140e2e8fe is a nop followed by local QDir/QString destruction, then execution continues at 0x140e2e922. Common_Mesh removal and later task/geometry/document work are not cancelled by an ordinary false return from this helper.

The current guard before this block is correct. Moving it after a pure cached-SUB check would not add a meaningful effect. A future wired implementation must perform the fresh accessor at its actual call position and bind its returned owner before deriving the cleanup path. No Project/Session current-database model can stand in for the earlier operation GuiModel or this fresh SUB.

## Root-preserving clear helper 0x14081b400

The helper owns the passed QDir value and releases it on every normal return. It preserves the root directory and follows this order:

1. Call 0x14081aec0 at 0x14081b421. That leaf obtains `QDir::absolutePath()`, constructs `QFileInfo(QString const&)`, asks `isDir()`, destroys the temporaries and returns the Boolean. False immediately returns false from clear, before entry enumeration.
2. Obtain `QDir::entryList(Filter(-1), SortFlag(-1))` at 0x14081b491. Both flags are explicitly constructed as int32 -1 via 0x140004a60. The returned QStringList is a materialized name snapshot, not a streaming host traversal. Iteration proceeds in that list's order.
3. Skip only names equal to literal `.` or `..`, by the exact string equality calls at 0x14081b4ee/50f. Those literals are at 0x1427a5104/108. Their presence is not replaced by a blanket hidden-name skip.
4. Construct `QFileInfo(the QDir, currentName)` at 0x14081b538, then call `QFileInfo::isDir()` at 0x14081b544.
5. For a directory, get **filePath()**, not absoluteFilePath(), at 0x14081b55f; construct a QDir from that string and call recursive remover 0x14081b2b0 at 0x14081b5b0. If that remover returns false, release current temporaries/list/QDir and return false immediately. Later sibling names are not processed by this helper. Successful recursion resumes the next sibling.
6. For a non-directory, obtain QFileInfo::filePath() at 0x14081b644 and call `QDir::remove(filePath)` at 0x14081b672. Its Boolean is ignored. Release the entry's temporaries and continue to the next name even after failed removal.
7. Exhausting the list returns true. No call removes the clear helper's own root.

The directory/non-directory query is not a recovered symlink-special case. The helper invokes isDir and takes its actual result. Do not invent a no-follow policy, a symlink exemption, or cycle prevention from the application code; the concrete engine's classification is an input to this traversal.

## Recursive remover 0x14081b2b0

This helper first copies its QDir argument. Its first test is 0x14081ae50, which obtains `QDir::absolutePath()`, constructs QFileInfo and calls **isFile()** through IAT 0x141e04f98. It is not isSymLink.

- If isFile is true, obtain that QDir's absolutePath and call `QDir::remove(absolutePath)` at 0x14081b313. Ignore the Boolean and return true after cleanup.
- Otherwise copy the QDir again and call root-preserving clear at 0x14081b34e. A false clear result returns false immediately; no later parent navigation/removal is attempted.
- On successful clear, obtain `dirName()` at 0x14081b380, then call `cdUp()` at 0x14081b38c on the helper's local QDir. A failed cdUp returns false after preserving prior child effects.
- On successful cdUp, call that parent QDir's `rmdir(savedDirName)` at 0x14081b3c5. Ignore the rmdir Boolean and return true. Thus a directory can remain after failed rmdir while traversal still proceeds to later siblings.

There is no application-level transaction or rollback. These are normal-return branches; native Qt/allocator exception cleanup and filesystem races are not closed by them.

| Event | Local traversal consequence | Outer DoeOutput consequence after helper returns |
| --- | --- | --- |
| Initial absolute-path isDir false | Clear returns false; no list | Continues after Regions block |
| Non-directory remove false | Continue next sibling | Continues |
| Recursive clear false | Stop current and enclosing sibling traversal through false propagation | Continues after Regions block |
| Recursive cdUp false | Same false propagation; earlier mutations remain | Continues after Regions block |
| Final rmdir false | Recursive remover returns true; later siblings continue | Continues |

This failure ordering is why `std::filesystem::remove_all`, unconditional continue-on-all-errors or stop-on-any-removal-error would change behavior.

## Exact enumeration defaults and still-unclosed result producer

The caller uses a newly constructed QDir, not an arbitrary preconfigured one. QtCore `QDir(QString const&)` at 0x670b0f50 supplies filter value7 and sort value0x10 to its private constructor 0x670b0cb0. The private constructor stores sort at private+0x10 and filters at +0x14, initializes its relevant state bytes and lists, and supplies wildcard `*` when the copied name-filter list is empty. The wildcard bytes are at 0x67229858; the empty-path replacement literal `.` is at 0x671af218. This named absolute path does not authorize taking the empty-path replacement branch.

The entryList wrapper 0x670b1710 invokes the overload 0x670b0370 with that stored name-filter list. The overload replaces filter -1 from private+0x14 and sort -1 from private+0x10. For this unmodified fresh object those resolved numeric values are7/0x10. Their exact filtering, case folding and tie ordering must come from this retained Qt implementation, not host directory iteration order. The application helper's explicit dot-name skips are not a license to exclude arbitrary dot-prefixed Windows names.

The matching-configuration path calls private cache/list producer 0x670aed70 at 0x670b03fb, then returns a retained QStringList snapshot. That producer constructs/advances an iterator through 0x670b3c30, 0x670b2980, 0x670b3800 and 0x670b27a0, then calls sorting/list helper 0x670aeaf0. The alternate entryList configuration path constructs an iterator at 0x670b0482 through 0x670b4010. This lane identified those concrete next callees but did not close their file-engine selection, metadata/error outcomes, full filter rules or sorting comparator. An empty or failed host enumeration cannot be assumed to mean a successful native empty snapshot.

Current `src/qt4/native_directories.cpp` exposes only the qualified mkpath adapter; its FindFirstFileW is an attribute-query fallback, not this enumerator. `FileEffects.deletes` is deferred, and `removalAttempts` alone does not supply entry snapshot order, QFileInfo isDir/isFile or cdUp/rmdir semantics. `StudyIo`'s XML journal likewise is not a directory device. No existing API can faithfully execute this next effect just by adding a caller-side loop.

## Smallest next action and ownership proposal

Keep CP149's successful nonnull-model guard until the immediate directory device is source-qualified. The next bounded static action is to close the fresh-QDir 0x670aed70 ->0x670b3c30 iterator creation/first read and 0x670aeaf0 sort comparator, then QFileInfo classification and QDir remove/cdUp/rmdir behavior for a stated ordinary absolute Windows path profile. Qualify accepted/rejected names, metadata errors, junction/symlink behavior and snapshot ordering before deleting any entry. This is a concrete missing result/device contract; another supplied-entry helper would not establish public enumeration.

A future actual implementation should remain wired in `src/engine/save_current.cpp`, reuse the reached current operation and re-fetch its active SUB, and use one immediate observation/removal device whose ordered effects survive the later guard. Root must assign any shared declaration, likely `include/m2m/native_directories.hpp`; a separate worker could own its source device and focused test only after the native device contract closes. Do not edit CP149's caller during its build/freeze or introduce a new independent Study/output owner.

Focused acceptance should use the real Save dispatcher, a sentinel root, names with known Qt ordering, nested directories and controlled removal failures. Verify that recursive false skips later siblings but the outer callback reaches its next guard, ordinary file/rmdir failure does not, the root remains, and already-completed mutations remain visible. A recording test must receive results from the same device rather than infer native success from requested deletes. Modal-return failure, actual earlier Open/DBLIST/task guards and all later Common_Mesh/geometry/`.DS`/refresh dependencies remain separate.

## Static receipt

GUI SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`; QtCore SHA-256: `bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160`. Imports were resolved from the immutable GUI import table, not guessed from call shapes. DUMPBIN decoded actual entry ranges read-only; these byte intervals exclude following padding.

| Preferred VA / bytes | SHA-256 |
| --- | --- |
| GUI 14081b400 /713 | 3d2c6e9acd15c149fc6ecda8ce9a5bf8acc4654e790dc467b332c22d5b49c399 |
| GUI 14081b2b0 /323 | 7ba2651cf8443699442e6a38d61543ad21484f7d4eb659ac6e83cd9885908e17 |
| GUI 14081aec0 /99 | a785706ddc6296663dfb770bdd55819f4f0e22bfadfa8eef12dc32794b0f3334 |
| GUI 14081ae50 /99 | ce003662c29a8658bcf52f3a5d1aa89ca1de795d77ac0dc9d6b9f533a64cd6e0 |
| GUI 140004a60 /26 | ff8ad4c55c4a0417ec68d56b9b8725dad0c83c0303c9b351ee3a1eb0be7e22c6 |
| QtCore 670b0f50 /267 | fd379f4827c0ea2dd14b8973617a124f2feaffe4c68adf23f7cbec3b349e71b2 |
| QtCore 670b1710 /67 | 66fb2859e6efa1c9b663a540e42b94e43244733b80db92b5314a27161deb3248 |
| QtCore 670b0370 /692 | 223c380f33b4fb030970408f90a8b9bb7e475ec5a17ab5912c0917c0b9adf9d8 |
| QtCore 670b0cb0 /672 | 4a678cd2c24fbad8e64b70df431ddd2c4de0af7350d30637d48812ce9777ff67 |

CP149's report records the full caller byte receipt and corrected Save order. The source-qualified older lifecycle description remains useful for broad order, but CP150 refines its blanket recursive-removal wording with the exact local false-return branches above. No new raw capture file or native observation was required.
