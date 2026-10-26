# CP76 failed reload after header mutation: focused test design

A disposable copy of the existing COG fixture can make header loading succeed
and directory loading fail deterministically. The expected result is logical
uac8=33 with the original 32-entry allocation and the same valid receipt. This
closes the missing failed-reload regression without enabling nonnull geometry.

This report began as a read-only audit while CP75 was frozen. After CP75's archive,
root assigned the exclusive test implementation described below. Only the focused
test and this report changed; production files remain frozen. No build, test
execution, GUI or mutable REA action was performed. This worker did not mutate a
DB copy or execute the loader; the test performs those actions when root runs it.

## Exact disposable-file mutations

Inspected immutable input:
`evidence/test-rea/COG_P1_B1/COG_P1_B1.DB`, 9,216,000 bytes, SHA-256
`707326e90e4f8fb0fee2aa1fa9f174e8d6e40b213fe2a7c0a828be00829671df`.
Its first 20 bytes are `SFTC-DEF-3D-DB100-00`; record size is 512. It uses
big-endian numeric words. Record2 words29..34 decode to
`31, 1, 32, 18001, 1, 0`. Record3 first word is4. SMI1 is `11.00   `.

Use the existing FixtureCopy under configured M2M_TEST_SCRATCH; retain its input
fixture unchanged. First load its X.DB normally into a real DbHandle and capture
the ready receipt. Then close/flush a binary fstream that performs exactly these
two writes to that disposable X.DB:

| File offset | Existing bytes | Test bytes | Meaning |
|---|---|---|---|
| 632 = 512 + 30*4 | 00 00 00 20 | 00 00 00 21 | Record2 word31 NRSETS:32 to33 |
| 1024 = 2*512 | 00 00 00 04 | 00 00 00 00 | Record3 word1: directory chunk1 maps to record0 |

Require the original bytes before patching, rather than silently assuming any
configured fixture has this shape. Do not alter the signature, file length,
record2's remaining fields or the readable index record3. A fresh copy per test
prevents this deliberate bad index from affecting the input or other cases.

## Why the failure occurs after the desired mutation

1. The signature check (`step/ext_wiring.cpp:23`) still succeeds.
2. `resultset::read_db_header` executes dbcore op0. `op_open` at dbops.cpp:292
   reads record2 and index record3, but does not dereference the chunk-record
   values. With positive NRSETS33, it reads the single existing index record and
   exits normally; the zero chunk pointer is not an error at this stage.
3. `load_db_info` writes its parsed metadata and **d.uac8=33** at
   db_info_read.cpp:112. Full-load family/version selection accepts this existing
   combined signature. The temporary word allocation has room for33 records.
4. `read_result_sets` at db_result_sets.cpp:94 dispatches op1. `op_directory`
   at dbops.cpp:347 requests slot1. `chunk_record_word(1)` at39 selects index
   record1/word1, now zero. `chunk_io` at381 sets cur_chunk=1 and calls the
   block reader with record0.
5. `ftn_io.cpp:140` rejects rec<1 with err1 before computing an offset or reading
   a payload. chunk_io changes it to0x65. op_directory returns on this error,
   and the result-set wrapper returns it after closing the unit.
6. `load_db_info` takes its directory-error branch at145, frees its temporary
   words and returns3. It has not reached the old directory deletion at152,
   replacement new[] at157, allocation publication at158 or ready tail at201.

This route avoids reliance on truncation, allocator failure, stale globals or a
concurrent change between the two opens. An unsupported-family signature is not
the preferred test: accepted on-disk signatures map to the recognized families,
so merely corrupting the signature can fail too early.

## Evidence-backed test contract

The assigned Testkit case in test_db_directory_receipt.cpp reuses FixtureCopy.
Before mutation:

- Require actual load_db_info(...,false)==0 and a ready current(&handle,reader).
- Require old allocation extent32 and logical uac8=32; keep owner generation,
  allocation generation, pointer, receipt and the 32 records' object bytes.
- Read and require the two expected original byte sequences before modifying
  only the copied file; close the stream before invoking the port again.

After load_db_info(...,false) on that same copied path:

- Require return3; return1/2/4 means the intended boundary was not reached.
- Require logical reader.uac8==33 and reader.uad8==oldAllocation.
- Require current(&handle,reader) remains present; allocation==oldAllocation,
  elementCount==32, ownerGeneration and allocationGeneration are **identical**
  to the captured values; isCurrent(oldReceipt) remains true.
- Require the actual 32 records' bytes unchanged. Never inspect record32: it is
  beyond the proven allocation even though the logical field now claims33.
- Preserve the native early partial writes: u1900=0 and uacc=0. Do not restore
  logical count or receipt state as error recovery. No query, A/cache/model-object
  write or geometry activation belongs in this test.
- Optionally restore the two bytes sequences in the same disposable file and
  perform another successful full load: the next real new[] must produce a new
  allocation generation under the same owner generation; oldReceipt becomes
  stale, even if the allocator reuses the old address. This adds a useful
  failure-to-recovery transition to the single focused case.

Normal teardown remains valid with uac8=33: native reset's nonzero gate uses
delete[] on the actual pointer, whose allocation metadata owns its real32 count.
Do not manufacture a 33-entry receipt or repair a DB field merely for teardown.

A separate wrapper test may use actual dbOpen then a deterministic changed-file
stamp and dbRefreshIfChanged to check that loaded remains true on failure. That
is optional extra coverage; the direct real-loader regression above already
establishes the receipt's required extent/partial-field behavior and avoids
timestamp granularity dependencies.

## Source binding and integration limit

Source SHA-256:

| Source | SHA-256 |
|---|---|
| dbcore/dbops.cpp | 425f2ef23644058962627bcb1f135a30a4a91023a430faa9102f1742bb97fedc |
| dbcore/ftn_io.cpp | f2e5ea275a4cf3713b2d35d556f3787ece62b219b44df29df50316c42232a9ad |
| resultset/db_result_sets.cpp | 86d199267d09b3534a8cf9b0c6d574a7dec4837520d8d363c3f342088aae0511 |
| step/ext_wiring.cpp | 5266c7d78e0da475b9cd78e35104072fe7f5ed9d4ceb0b4056666d6b831e0c60 |
| step/db_info_read.cpp (CP75) | 512550efd22c460fb08730c069ba53b78aac473ea8d594cd9a5f1425fc03fd13 |

The registered target remains m2m_mdt_db_directory_receipt / CTest
m2m.mdt.db_directory_receipt. Root owns build and runtime validation. CP75
production files and their frozen hashes remain unchanged. No passing-test or
public high-version success claim is made here; token measurement is unavailable.

## Implemented test and static release

`directory_receipt_failed_reload_keeps_actual_extent_after_header_count_changes`
implements the exact two-word patch, directory error3, retained receipt/records,
native partial uacc/u1900 effects, and restored-file successful reload transition.
It requires expected original file words before each mutation, flushes/closes
the stream before loader calls, checks file length remains unchanged, and never
reads record32 or changes a model/geometry admission. It does not repair native
fields after the failed reload; recovery comes through another real load.

The test file now contains11 unique Release-active Testkit cases. Static preflight
verified its entry point, supported macros, quoted/new library includes, absence
of C assert/REQUIRE_EQ, and whitespace. Source release SHA-256:

`source/apps/moproj2mst/tests/mdt/test_db_directory_receipt.cpp`
`09e42b73625b46b7a415a6235b45f1a21536bc6837a7ff299265a08edd28c071`.

All five CP75 production receipt files retain the exact hashes in the CP75
report. The focused test and this report are released frozen to root; this worker
ran no compiler/build/CTest or file-mutation fixture action.
