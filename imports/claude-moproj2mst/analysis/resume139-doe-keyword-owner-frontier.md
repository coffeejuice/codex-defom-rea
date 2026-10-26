# CP139: keyword owner boundary after the supplied-Project dimension prefix

The next persistent keyword effect is not yet safe to implement on the actual Study. CP138 can retain the changed Variable dimension, but its next native step needs a live keyword pointer and later commits owned DYNARY objects. The candidate `Item` contains only two text values. Adding another snapshot-only helper would not advance the persistent Save behavior. This checkpoint therefore writes this report only and preserves the existing guards.

No source/test/build, original process, GUI or mutable REA action occurred. No Type3 owner file was edited. Bounded DUMPBIN reads used the immutable original GUI PE, SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`, reverified here. This is evidence from the original binary, not a reconstructed executable deployment report.

## Exact next native branch

The retained complete caller capture is `analysis/resume103-doe-save-reconcile-1400d2b60.asm`, SHA-256 `4E5A81664574951B2A327A00723BC13C1FF0153292BE67613F21C1F0598A59A2`. Its ordering is:

| Native site | Reached effect or decision |
| --- | --- |
| d2d07 | Equal dimensions skip all keyword work. |
| d2d25 -> d8040 | Store the selected dimension at Variable+24. This is CP138's retained persistent effect. |
| d2d43 -> d0e60 | Copy the Variable+30 keyword pointer list into a local snapshot. |
| d2d5b / d2d74 | Empty snapshot or null first pointer skips this Variable. Only the first keyword is used. |
| d2dd1 / d2e23 / d2e75 | Copy nominal/upper/lower QString values from keyword+10/+18/+20. |
| d2eb7..d2ecf | Initialize three conversion-output pointers to null. |
| d2eeb -> 921930 | Read keyword ID+2c, find registry name with 921f30, construct QString from its C string. |
| d2f01 / d2f17 | Copy parameter name+0 and entities list+50. Build the entity set/list locally. |
| d2fa3 -> d7fa0 | Read the separate signed selector at keyword+84. |
| d30d2 -> 916530 | Convert the three texts using the selected dimension and entity/parameter metadata. |
| d30d7 | Save the returned status locally; no status branch gates the following commits. |
| d30fb / d3106 | Require both lower and upper output pointers nonnull; otherwise skip all commits. |
| d3137 | If nominal output is nonnull, call 9210d0 with the saved +84 selector and final argument false. |
| d314c, then d3161 | Commit upper with 9213f0, then lower with 9212f0. |

The +84 selector is distinct from the +80 parser parameter represented in the older scalar prefix. Constructor9206e0 initializes both to -1 at9207b5 and9208ae respectively. They must not be merged merely because their defaults agree.

The getters and local snapshots can allocate, change Qt refcounts or unwind. Their values alone are not an additional persistent Study mutation. Native list pointer/null/COW semantics are not represented by `std::vector<Item>`; an empty host list is the only already-admitted value branch. A nonempty Item value cannot establish that the native first pointer was live, nor represent the first-null skip.

## The commit is an owned-data write, not a text setter

Bounded normal bodies establish these writes:

- `9210d0` allocates 0x78 and, on its nonnull allocation branch, calls DYNARY copy constructor898f40. It stores the resulting pointer at keyword+98 (92113b), the selector at+84 (92114b), then sets+31 (921156). Only a true final argument clears+A0/+A1; this Save caller passes false.
- `9213f0` similarly stores the new upper pointer at+90 (921451), then sets+A1 (92145d).
- `9212f0` stores the new lower pointer at+88 (921351), then sets+A0 (92135d).

The bodies contain no read/free of the previous pointer before these stores. Do not invent replacement cleanup or rollback. They explicitly contain a null-allocation branch, but this report does not establish allocator policy or whether that branch occurs in the selected process. A later throw cannot justify undoing an earlier completed nominal/upper store. These setters do not replace the QStrings at+18/+20.

The retained `evidence/gui_mo/s1/mdt/DOE/d10.c` export for898f40 shows initialization followed by899010 and89ad60. It is not a complete copy/destruction or failure-lifetime contract. The conversion call916530 can perform scratch-model and `temp.KWD` effects before returning; treating an unsuccessful status or null output as no effects would be wrong.

Current `keywordValues` (`src/doe/doe_study.cpp:865`) is a separate guarded planner: groups1..5 and the default handler3 only, writes planned temp.KWD values, and the supported default handler has no output DYNARY. It is not evidence that the original output-producing registry handler ran. B121 TLOC is group6 and remains refused. Calling this utility on synthesized Item metadata would bypass both the actual owner and handler boundaries.

## Candidate ownership gap

`include/m2m/mst/doe_model.hpp:35` has `Item{before,after}` only; `Var::items` at83 is a vector of those values. It lacks nominal text, keyword ID, parameter name, entities, +84 selector, three DYNARY owners and their flags. CP138 (`src/doe/variable_dimension_prefix.cpp:50`) stops immediately after a changed dimension when this list is nonempty. It does not claim that the native snapshot ran.

`HeaderVariableRecord` is the concrete actual public reader owner (`include/m2m/doe_variable_record.hpp:15`), uniquely retained by Study. Public direct Keyword entry still refuses in `src/doe/variable_record.cpp:177..181`, after header/earlier Description/fresh sample-invalid effects and before keyword temporary/codec/record ownership.

The retained B121 prefix has `ScalarKeywordPrefixRecord` and a separate `ScalarVariablePrefixRecord::keywords` list (`src/doe/variable_prefix.hpp:14..33`). That different concrete Variable is owned by retained prefix state and stops before SetOprHasDoe delivery and subsequent text/selector setters. It cannot be cast, adopted or counted as `HeaderVariableRecord::items`; Var has no virtual destructor. Its constructor-default text is not proof of the Save text after the missing continuation.

## Smallest next evidence action and prospective source scope

The next useful action is to close the native keyword owner, rather than add another value-only list helper:

1. Follow the already-identified producer816b40/append816d24 and constructor9206e0 through the keyword record's list ownership and destruction. Identify the actual deleting path for +88/+90/+98, source-output cleanup after916530, and how aliases survive the Var+30 snapshot. Read the bounded native898f40 ->899010/89ad60 copy path and corresponding destruction, including the first failure/unwind edge. Constructor values alone do not close this lifetime.
2. Select one actual registry handler that produces both upper/lower DYNARY outputs, with parameter/entity ownership and observable temp.KWD ordering. The current default handler's null outputs cannot test the commit branch. Do not substitute callback-return booleans or a TLOC group6 result.
3. Only then request root ownership of the shared canonical Item representation and migrate its existing MST emitter consumer. Prefer one canonical typed keyword owner on the actual HeaderVariableRecord/Var list; do not introduce a second competing keyword list or delete derived objects through Var. A private source file such as `keyword_reconcile_prefix.hpp/.cpp` and focused `test_keyword_reconcile_prefix.cpp` may then receive explicit, source-qualified conversion inputs, while the public manager/codec/delivery path stays guarded.

Prospective tests must distinguish empty/first-null/live-first list states, equal-dimension skip, a retained dimension before any item refusal, ignored conversion status versus null-output gates, nominal-optional then upper/lower commit order, and failure after an earlier completed owner store. Tests must not infer native allocation parity, durable temp.KWD bytes, or a completed public Save. The source/test/API proposal is conditional on the ownership evidence above; no source assignment or implementation is requested by this report.

## Guards that remain

Public nonempty Type2 Save still stops at `VariableDimensionReconciliation` (`src/doe/type2_save_node_prefix.cpp:43..44`). CP137 prune and CP138 dimension helpers are private and do not certify original global main-window/current-MO/lazy OprManager selection or the earlier Variable snapshot interval. Reconciliation still precedes prune, XML and refresh; none may be moved ahead of it.

For zero-variable XML, actual notifier flag90 reset is implemented, but key QString construction, ordered registered receiver occurrence and native return remain unqualified. Type3 stream or owner evidence does not supply the original C-string codec at this invocation, the actual SfMoDoeTable timer/widget owner, or a refresh delivery receipt. Post-refresh cached-manager PAR writes and public completion remain guarded.

## Compact byte receipt

The following immutable byte intervals were decoded with DUMPBIN14.44.35228.0, starting at their function entries. Lengths identify the bounded bytes, not a whole-program or allocator proof. No new raw artifact was needed.

| VA / bytes | SHA-256 |
| --- | --- |
| 1409206e0 /493 | 1d57b0353c920cad5a4d6ab85037727b0035060bf462bead4e1776ad22f062bb |
| 1400d0e20 /62 | a58991ad7b6f8f62eb51f4f2724335d87a950217f726e7a25cb677436ddb828b |
| 140921930 /70 | 797f0159235b25afe05f5cf3a3a35d77d258a3ca85ec7140474f10f1391ba941 |
| 1409210d0 /176 | ba56c203674c9276e6bde0923e539d8b095446aa5253484cd7b06c700bfd868f |
| 1409213f0 /121 | 5a961ec6b0545092b714d4484bcdd7b9a076cde4322ac2c199e0d10e71fe83a3 |
| 1409212f0 /121 | ece2a95e15ed0a8679f2e0f6f90752641d464cb6bbe46b272c272be8f638b659 |
