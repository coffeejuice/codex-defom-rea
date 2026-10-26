Bounded DUMPBIN receipt: 0x140307410 local-list gate
Target: evidence/original-binaries/DEF_GUI_MO_64.exe
Target SHA-256: fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc
Image base: 0x140000000
Tool: Microsoft DUMPBIN 14.44.35228.0
Window: 0x140307410..0x140307450

  0000000140307410: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140307415: 48 83 EC 28        sub         rsp,28h
  0000000140307419: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014030741E: FF 15 DC CA AF 01  call        qword ptr [0000000141E03F00h]
  0000000140307424: 48 83 C4 28        add         rsp,28h
  0000000140307428: C3                 ret

Indirect target qualification:
- The referenced slot is VA 0x141E03F00, in .rdata (section VA 0x141E03000,
  raw pointer 0x01E01800).
- The slot's raw qword is 0x00000000035171AA. Treating that value as an RVA
  maps to .rdata raw file offset 0x035459AA.
- Bytes at that RVA begin `B1 0A` (import hint) followed by ASCII
  `?isEmpty@QListData@@QEBA_NXZ` and NUL.
- The imported symbol is `QListData::isEmpty() const`; thus this wrapper's
  returned bool is the QListData emptiness predicate, not an opaque data test.

The call site in retained complete writer receipt
analysis/resume48-ppv-writer-receipt.txt branches on this predicate before
QFile construction: true skips writing and returns false; false enters the
writer path. The caller/list ordering is qualified separately in
evidence/gui_mo/out_s/s1.c at FUN_1400597d0 and summarized in
source/docs/re/moproj2mst/s/spec.md.
