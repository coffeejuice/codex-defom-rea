# CP152 DBLIST selector-to-constructor map

## Result

The selector-to-constructor branch is now byte-bound in the immutable original PE. The string operand at `0x1421b0908` maps, through the PE `.rdata` section’s raw/RVA mapping, to the retained string-table entry `SfTaskReportGenerate` at raw file offset `0x21af108`. In factory routine `0x140137ca0`, the equality check against that literal has a true branch that allocates `0xc0`, calls constructor `0x14079f1e0`, then calls registration routine `0x1407c8140` with that returned task pointer. The constructor writes the `SfTaskReportGenerate` address point `0x14269e558`.

This closes the native selector → concrete constructor → registration-call edge. The registration routine indexes by the constructed object’s `TaskInfo` metadata-name field, and stores the raw task pointer in a second map. The retained evidence does not directly read that runtime metadata-name value for this constructor, nor capture the exact pointer returned from the later MainWindow getter occurrence. Thus the static factory creation path supports the matching task type, but no occurrence-level pointer/vptr value is observed. The separate AppState `+0x1c` public gate is unchanged.

## PE byte evidence

Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`, SHA-256 `FD8AE1F8DD50D068267B3B7E49EFDF11EAA178889131E59EC4B6F8DE64725ACC`. DUMPBIN 14.44.35228.0, executable SHA-256 `12A1CD87238BD66DFDB788B4FCDCB91CE4B3F81236AAB12A7A29E5AE1D85AF50`.

The PE section mapping is `.rdata` RVA `0x1e03000`, raw pointer `0x1e01800`. Therefore raw string offset `0x21af108` maps to RVA `0x21b0908` and VA `0x1421b0908`:

`0x1e03000 + (0x21af108 - 0x1e01800) = 0x21b0908`; add image base `0x140000000`.

`evidence/gui_mo/strings.txt` (SHA-256 `677D8A9A2731AEC19BFF0075DF8B7D886754D9F44798B76D661C9EF29B41A982`) records raw offset `021af108` as `SfTaskReportGenerate`. DUMPBIN `/DISASM /RANGE:0x140137ca0,0x140138600` on the target gives this local branch:

```
140137ca0: ...                         ; factory create routine
140137d1: lea rdx,[1421b0908]          ; exact selector literal
140137d8: mov rcx,[rsp+1d8h]            ; requested QString
140137e0: call [QString comparison IAT]
140137e6: movzx eax,al
140137e9: test eax,eax
140137eb: je 140138257                 ; mismatch continues to next key
140137ed: mov ecx,0c0h
140137f2: call 14146eabc               ; allocate task storage
140138212: call 14079f1e0               ; SfTaskReportGenerate constructor
140138245: mov rdx,[rsp+0c0h]            ; constructed task pointer
14013824d: mov rcx,[rsp+1d0h]            ; factory data receiver
14013824d: call 1407c8140                ; register under task metadata
```

The exact instruction window around the true branch is `0x1401381d1..0x140138252`. The disassembly output shows at `0x140138212` the relative call bytes `E8 C9 6F 66 00` to `0x14079f1e0`, and at `0x14013824d` `E8 EE FE 68 00` to `0x1407c8140`. The constructor capture `analysis/wave2-doe-task-prototype-ctor-14079f1e0.asm` (SHA-256 `0072B6C52A9C19DD54714036FDFCEABC7F034A50D2A8DF23C4DA53590225DB4B`) stores address point `0x14269e558` at object offset zero; `evidence/gui_mo/s1/vtables.tsv` (SHA-256 `B5C2274F95BEBD3A0D7F16917937D0974D5F16E1840125F6E314A21787A571F4`) labels the primary table `SfTaskReportGenerate`.

## Insertion and getter boundary

`analysis/wave2-doe-session-task-payload-entry-decompile.json` (SHA-256 `ED5205A9142040BB4CACB28D68B6351175F3F0F1A300BBCA30408FE9C99ECEF5`) decompiles `FUN_1407c8140` as follows: it obtains TaskInfo from the task (`FUN_141403790`), binds its factory backlink, obtains its metadata name (`FUN_14140e5d0`), and inserts that TaskInfo into factory data map `+8` under that name. It then obtains the metadata name again and inserts the raw task pointer into map `+0x10` under that name. The original MainWindow `+0x4a0` owner and its lazy factory initialization are described in `analysis/wave2-doe-live-task-registry-ownership-contract.json` (SHA-256 `23494CB67A8A58FFF7F70ECACBCD6BE6FDE4C1ABFBD80F1E434D6FEDA9913E2A`). Its initialization routine `FUN_140137610` constructs/registers the prototype sequence containing `0x14079f1e0` through the same insertion routine.

The same ownership contract distinguishes `FUN_1407c7d00` as lookup-only: for an existing requested key it returns the raw value from factory data map `+0x10`; it does not create or clone. The MainWindow `slotGetTaskBase(QString)` body is a lookup in the lazily owned factory and returns the stored pointer (`analysis/resume65-gettaskbase-dispatch.md`, SHA-256 `E88FE6C24238B0B3E04EB0B5611B9C8C6B776C6BB0CCE73000AE1093C3FDE495`). CP149 binds DBLIST’s generic invocation to this QString getter with selector `SfTaskReportGenerate` and return type `SfTaskBase*` (`analysis/resume149-connector-iat-resolution.md`).

This shows how the factory can hold and return the ReportGenerate prototype/task under its metadata key. However, `FUN_1407c8140` derives the insertion key from TaskInfo’s metadata-name field; the available capture does not show that field’s value for this concrete constructor. Nor is there a static per-occurrence output capture showing the getter’s returned address and dereferenced vptr. That is the remaining identity witness if the requirement is the exact DBLIST occurrence rather than a static factory path. Do not treat the constructor call alone as an observed non-null return.

## Qualification and limits

The evidence proves the exact selector branch and registration call in the original PE, and the constructor’s class address point. It materially narrows the previous key-to-branch gap. It does not prove successful DBLIST execution, the runtime factory map contents at an actual Open, or AppState `+0x1c` admission. No source, build, GUI, target runtime, or mutable REA database was touched.

