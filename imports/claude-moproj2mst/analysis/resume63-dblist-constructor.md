# CP63 absent DBLIST child construction and object-color dependency

The native absent-child factory target is closed statically, but its lifecycle is not implementable as a complete child merely by allocating DbHandle. It first reaches public QObject(parent=null), whose actual runtime/private/thread/reference inputs remain unavailable. Later path assignment also reaches a process-wide object-color table file reader before manager ID assignment/list append/connections.

An isolated table value consumer is safe to implement with explicit caller-supplied stream/token-reader/table state. That would recover the parser's exact updates and EOF behavior without claiming a native child, global table binding or public B127 admission. This report and uniquely prefixed raw evidence are the only writes in this audit; no source, build/tests, GUI, target process/calls or mutable REA database occurred.

## Exact virtual factory and constructor order

Raw PE slot bytes at manager vtable142115f68+68 are9054044001000000, selecting140045490. The new bounded capture confirms that factory allocates0x19f0, then calls043fe0(allocation,null). A null allocation returnsnull; c79390 subsequently invokes the path setter on its result without a newly qualified safe null branch. Keep allocation failure guarded rather than inventing recovery.

Retained043fe0 calls ModelBase constructorcda460 on that same allocation, installs Mo vtable142112878, then044090->044bd0 initializes the Mo Project backlink. ModelBase's **first** substantive call is imported public QObject(parent=null) atcda47e. Only after it returns does cda460 install the base vtable and initialize the nonowning plane list+18, null path+28, DB reader+40, owning view list+1960 and other lists, then cda5d0 initializes flags/pointers and helper storage. Therefore completing a post-construction append seam or using make_shared<DbHandle> cannot prove this earlier QObject call, its allocation/frame identity, or its partial effects.

The correct production order remains constructor return, cdcdc0 path setter return, ID assignment, manager+18 append, then the two connections. Current selection is unchanged by c79390. Native child creation is separate from constructing a Project; DBLIST must not assign Project.dataDb or force a current child/Project merely to register a referenced DB model.

## Path setter and global owner

cdcdc0 sets child.pathSet(+20) before comparing the supplied path with stored+28. If unequal, it assigns the path; if keepItems isfalse it clears/deletes the owning view list via cdabc0; it clears loaded(+38) and resets the timestamp through d39950. On a fresh constructed child the view list is empty; that does not generalize to populated/reused models. If paths compare equal these change/reset effects skip.

Regardless of equality, the setter then applies the nonempty path suffix rules (.DB unless already matched or exact FOR003), assigns QDir::cleanPath, and constructs the object-color filename. Native calls select QFileInfo(storedPath).path(), QDir::separator(), append .ObjectColor.DAT, QString::toLocal8Bit(), QByteArray::data(). It calls cdeab0 atcdd15f with RCX=**1447c4860**, two local integer output addresses initially0, and the local8bit filename. The local outputs and reader Boolean are unconsumed by this setter. Equal-path calls therefore still reach the color-file reader; do not implement only-on-change palette loading.

The table is process-wide, not stored in DbHandle, SessionPart, Project or a new per-DB palette. Selected PE raw sections do not file-back this address, and no current runtime initial bytes were observed. The existing candidate dbSetPath explicitly omits this route. No current actual-owner interface for this global table was found in the reviewed converter source.

## Original palette-file algorithm

Retained cdeab0 decompile is corroborated by the new full unwind-bounded raw capture140cdeab0..140cdf290. It opens filename in exact mode rt. Open failure returnsfalse without token reads, table changes or output-integer writes. Once opened, it performs **100 row attempts**, not read-until-EOF:

- A persistent local int rowID starts0, and persistent local int components R/G/B/A start0 once. Each row attempt calls9068f0(receiver=globalPalette+75c,type5 Int,pos=-5000) into rowID.
- Only IDs1..100 cause component reads/stores. Invalid ID skips the payload reads and starts the next row attempt.
- Return value13 from the rowID read selects the four-table RGB form. This is the token count13, **not a carriage-return character**: existing TokenReader::read returns the current line's token count for pos=-5000. Any other return value selects the six-table RGBA form for a valid ID.
- The first four table bases are+760,+8f0,+a80,+c10. Each reads R,G,B and immediately stores their low bytes at index(rowID-1)*4. Only RGBA form also reads/stores A. RGB form retains the existing alpha bytes and leaves the final two tables untouched for that row.
- RGBA form additionally reads/stores all four components into table bases+da0 and+f30. Direct low-byte narrowing is established; no range clamp, QColor validation or rollback is present.
- After100 attempts, the reader calls type5,pos=-5000 into output1, then type5,pos0 into output2, closes the FILE and returnstrue. It ignores token-read error/EOF returns for these output writes and all component reads.

EOF is material. Existing9068f0/TokenReader::read returns-2 without overwriting an Int destination. RowID and components therefore retain prior successful values; this algorithm must not clear them every iteration or break at EOF. For example, if a short file ends after a valid RGB row, the next row attempt can retain that valid ID with return-2, select RGBA form, and write retained component values into that row, including alpha/additional tables. Invalid/EOF behavior is part of the proven algorithm, not permission to replace it with a conventional line parser. Existing TokenReader already models the selected numeric read and text-mode stream behavior; its port qualifications remain attached.

## Smallest safe isolated implementation

Use exclusive new files for a six-by100 RGBA-byte table value plus a consumer of an explicitly supplied datafiles::TextFile and datafiles::TokenReader. Accept already observed open-failure separately so false leaves values/output destinations untouched. Reproduce exact100 iterations, Int/kNewLine and Int/0 calls, persistent row/component locals, immediate stores, duplicate-row overwrite order, byte narrowing, two trailing reads and early-refusal/EOF partial-state behavior. Require caller-provided initial table bytes and token-reader mode; do not initialize the unknown global table or bind the helper to dbSetPath/B127.

The borrowed stream/value helper must label fopen/local8bit filename ownership and fclose as outside its isolated seam unless explicitly modeled. Tests should verify open-failure preservation, four-table RGB alpha preservation with enough remaining rows, six-table RGBA update, duplicate/invalid IDs, wraparound byte values, exact100 attempts/tail read order, and short-file EOF reuse of prior row/component/output values. A refused adapter read must preserve writes already reached and skip later ones, without claiming that a host exception reproduces a native CRT error.

## Exact B127 release dependency

Do not assume the absent branch is taken. If the *actual* resolved DBNAME path matches an already registered initial Mo child, lookup returns that child and neither constructor nor path setter/palette reload is reached. B127 therefore needs either a qualified existing-child lookup result for its actual path/owner state, or complete absent-branch construction and path/palette effects. No original/candidate invocation in this audit proves that result.

Independent production prerequisites remain actual param6 and global+1c gate, copied QString resolution context, selected QFileInfo::exists observation, actual Session/mainwindow ModelManager identity, later ReportList behavior and warning/default/lifecycle effects where reached. If absent, also supply same-call QObject runtime/private/TLS/shared-reference owners, full native Mo/DB reader construction, and process-wide palette/file state. No B127 guard removal is authorized.

Freeze UTC: 2026-10-24T21:44:27.479498+00:00. Static evidence only.

| Evidence/source | SHA-256 |
| --- | --- |
| `analysis/resume62-dblist-manager-implementation.md` | `efc243cc449643f64d378960ebed26c1edb38057ae5692dbc868a5624068eb37` |
| `analysis/resume63-dblist-constructor-path.asm` | `3119cf9771cc501d33d7952b749786eb865476d4e725b8ad276ab983e497dc9a` |
| `analysis/resume63-dblist-palette-reader.asm` | `24a9b2a6eb17187f63d3b06cc16bea0c7ad363e56ab7f43066e371664ea39aa2` |
| `analysis/resume63-dblist-constructor-receipt.json` | `ad25c211bad7620d5f60fe6f0fa8d4043da5de1f98ce5687b42a8d647a2824b7` |
| `analysis/wave3-closure-next-140043fe0.asm` | `a73364f9c29ef398d48d6bfaa37f7ec18d6dae0b86365b4934702ab626e9b008` |
| `analysis/wave3-closure-next-140cda460.asm` | `2c49e354137f6a259bef840239396e03ee497c2a6ff296ef275b34c76c2d4609` |
| `analysis/wave3-closure-next-140cda5d0.asm` | `6bf81d365de5bfa628833a4b35607ec9973eff26ddbc32c5d23fa26b9625dd55` |
| `analysis/wave2-doe-model-manager-path-palette-reader-decompile.json` | `20c6a01ef361346567e260d707d87735fd2d0c665f3e3f2b81c073639f774f23` |
| `analysis/wave2-doe-model-manager-path-palette-reader-receipt.json` | `e63128e15739ce1156916ad98b1b89b02ea1b951db076d23113faa0987259872` |
| `analysis/wave2-doe-model-manager-path-open-signals-contract.json` | `6599d07539efbe66fd98d4707a854839c11ceaeb3b9bd8170b385c50393bc7b7` |
| `source/apps/moproj2mst/src/engine/db_load.cpp` | `f87d28001efdba3fc3fd286077a4ef83c7cb106d8c8495daa6eacbc9d5b35b75` |
| `source/apps/moproj2mst/src/datafiles/token.cpp` | `3c56020a595e356e3a6c51a177c6470c88ae63622d85a6256ef4dc298e5bde9e` |
| `source/apps/moproj2mst/include/m2m/datafiles.hpp` | `8bbb05a24dd045212b846f0e5a8049d7395db543bf774413cc9d6cc97804cbbc` |
