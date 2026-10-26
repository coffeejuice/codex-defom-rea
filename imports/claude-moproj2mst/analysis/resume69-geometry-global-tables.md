# CP69 complete 3D global index-table mapping

The high-version reader's **3D dimension-selection prefix is statically recovered
by existing source**. All53 native MAR initializer stores and all129 A initializer
stores match the existing source's indices, values and store order exactly.
No new table implementation or assumed initial global state is needed. This
closes the table-initializer dependency identified in CP67, but does not yet
admit the complete high-version coordinate getter.

Owner: `gradient_state`. Only this report was written. No source/build/test,
GUI or mutable REA action was performed. Bounded immutable PE/disassembler reads
were retained as exact hashes below without new sidecars. Token measurement is
unavailable.

## Source and original identity

Original target `evidence/original-binaries/DEF_GUI_MO_64.exe` SHA-256:
`fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`.
The VS dumpbin identity remains the CP67-qualified SHA-256
`12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`.

Compared complete source bodies in
`source/port/defmodel/src/step/index_tables.cpp`, SHA-256
`f5e2cf0e20186999fbf4c3c8a1930ef9598551f371cfe3da73d35128b5947501`.
Their storage/API declarations are in
`source/port/defmodel/include/defmodel/step/db_step.hpp`, SHA-256
`eef902dcb1eb68aa864a361e7098c5370ee1e8d3fab40e4edd737c53c0bbccaa`.
The source uses DEF_PRE counterpart addresses in comments; this audit compares
the GUI artifact's actual instructions rather than assuming the counterparts.

## Bounded complete leaf evidence

| Initializer | Complete interval, end exclusive | Native-byte SHA-256 | Dumpbin stdout SHA-256 |
|---|---|---|---|
| GUI MAR3D `140820320` | `140820320..140820533` (531 bytes) | `63717edc0716e67575076e024e5395f79e8866a2692c55a16d3dbd9b4cf8f13c` | `b6d9b5c6dd9dd042124aba4c1b0cdfe337c4828f155fe3ed5bd95f9a40391d78` |
| GUI A3D `140820a60` | `140820a60..140820f6b` (1291 bytes) | `8b263b71a7daf80585f6fdb3bbaf14fbd76c10d651f7cda7b624d8d98cc37dd6` | `c2be4b704835a8371eff197b9b670f34b25a7f93282803f4182eb0da7cce4687` |

Commands were `/disasm:bytes /range:START,(END-1)` on the absolute retained PE.
Each body is a sequence of `C7 05 disp32 imm32` RIP-relative int32 stores followed
by one RET and then INT3 padding. There are no calls, conditional paths, stack
allocations, dynamic allocations or additional stores in these bodies. Bounds
exclude padding/next functions; they are explicit complete leaf bodies through
RET, not a claim of exception-funclet coverage.

The static decoder required the exact ten-byte store opcode at every instruction,
resolved destination as PC+10+signed disp32, decoded signed int32 values, and
required RET immediately after the expected stores. Normalized destination
indices and values were compared in instruction order against every source
assignment. All53/129 indices were unique and every declared slot assigned;
there were zero mismatches. Removing comments and those assignments left only
the source opening brace, so no unexamined source call/control flow was hidden
by the assignment comparison. Independent dumpbin text confirmed53/129 global
stores, zero calls and one RET per body.

## Exact state mapping and ordering

| Native GUI process-global storage | Existing source state | Width/extent |
|---|---|---|
| `143DBDAD8` | `g_index_tables_dim` | signed int32 |
| `143DBDADC..143DBDBB0` | `g_mar_index` |53 signed int32 values |
| `143DBDBB0..143DBDDB4` | `g_a_index` |129 signed int32 values |

The combined region is732 bytes including dimension. These identities are one
module's mutable process-global state, independent of the actual DB step's
ownership and the optional geometry mesh allocation. This mapping does not mean
their current contents can be defaulted to3D at a viewer call.

Source `init_mar_index_3d` matches native `140820320`; source
`init_a_index_3d` matches `140820a60`. The A body writes slot7 before slot6 and
source preserves that exact order. No sorting/normalization was used to hide
this nonascending pair in the instruction-order comparison.

For reproducible final-state validation, the SHA-256 of final array contents in
ascending slot order, with every element encoded little-endian signed32, is:

- MAR3D: `6a670b518b65a75c55c418f05488994e7b22c1322abbe07112a3128a61ed2816`.
- A3D: `adc81bb0ecf0478c33fbab011ccaef4e5d6f205b7ffb4cfb04004eb9f53fb77e`.

These are final-array hashes, not hashes of store-order values. The latter A
hash is different because of the7/6 inversion. Two reader-relevant cells are
g_mar_index[3]=3 (native `143DBDAE8`) and g_a_index[128]=280 (native
`143DBDDB0`). A cache-path proof must respect their actual existing values when
the selector takes its no-fill path; dimension3 alone does not force canonical
array contents.

The selectors were bounded in CP67: native `140820fe0` selects2D only for input2,
otherwise3D. Native `140820fb0` compares current dimension with3, returns when
equal, otherwise calls MAR3D then A3D and commits dimension3 only after both.
`select_index_tables` and `select_index_tables_3d` have exactly that source
ordering. With current dimension3, both native and source preserve even altered
table cells; they do not repair or refill them. The source reset API is test
support, not permission to erase production history during prefix execution.

## Smallest qualified prefix and remaining boundary

For actual public3D step S, the existing call
`select_index_tables(&actualDimension)` now qualifies the reader prefix at
`14082d7fc`, including its process-global writes when a dimension switch is
needed. It must use the actual existing module state and actual S+1c value.
There is no external table dependency left for this dimension-selection prefix,
and no need for a second table implementation or a default-3 observation API.

Keep the full native order documented in CP67: actual coordinate key0 dispatch
and possible COW detach; zero-flag/high-version gate false; sync which0's
reader(id0x0b,flag1); actual dimension read; global dimension/table selection;
then reader index/MAR arithmetic, id range and S+26c4[id] cache tests. If a
suspended prefix is implemented later, it must retain stage/owners so retry
does not repeat COW/global operations. These mappings alone do not create such
a cursor or authorize publication of a successful viewer rectangle.

The exact next dependency is the remainder of `14082d7d0` for its first id0x0b
call and the outer `14082dca0` which0 continuation. Shared counterpart
`fun_1401d4400` remains DEF_NOT_PORTED. A nonzero S+26c4[0x0b] may yield a bounded
cached early return after the now-qualified selector, but its earlier global
index reads/MAR pointer calculation and the remaining outer route must be
closed before using it as a complete sync fold. The cache-zero route additionally
needs actual DB-region/object-array mutation/diagnostic ownership recovery.
Do not replace the independent optional mesh with an object's main FE arrays.

Retain the current public high-version guard until that contract is complete.
Neither g_index_tables_dim==3 nor a populated coordinate pointer/cache flag
alone admits the full getter. Initializer recovery removes a specific dependency;
it does not prove subsequent storage/DB reads are safe or no-ops.

## Focused validation prerequisites

Existing source test `index_tables_per_dimension` in
`source/port/defmodel/tests/test_step_parts.cpp` checks representative2D/3D cells,
2D no-refill behavior and reset. That file's reviewed SHA-256 is
`c8f2227f2493dfc503572186a22496a926567d95e6302110255a042c15d8b930`.
I did not run it. It does not by itself compare every cell or prove a viewer's
high-version reader reaches normal return.

Before integrating a state prefix, root should validate all53/129 final values
against the pinned native values/hashes, actual MAR-before-A-before-dimension
ordering, and3D no-refill preservation of altered cells. Current dimension0/2
must select actual3D state; current3 must preserve actual arrays. Test the7/6
store ordering in an effect ledger if intermediate state is modeled; final
array hashes alone cannot validate store order.

For a resumed coordinate/reader prefix, require shared-list detach once before
table selection, no replay after suspension, actual N/S/module ownership and no
borrowed-array substitution. A cache-nonzero reader test must retain table
selection even on early return, and a cache-zero case must stop before unresolved
DB writes. Current viewer tests for version>=6000/zero flag must remain refused
until that integration is separately assigned. The known viewer target remains
`m2m_mdt_viewer_box` / `m2m.mdt.viewer_box`; no test was authored or executed here.
