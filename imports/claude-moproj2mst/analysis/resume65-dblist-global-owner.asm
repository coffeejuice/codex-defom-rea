Bounded static DUMPBIN evidence. Original PE: evidence/original-binaries/DEF_GUI_MO_64.exe
Tool: Microsoft COFF/PE Dumper 14.44.35228.0 from MSVC 14.44.35207
Ranges: 0x1402cb780-0x1402cb7ae and 0x1402cbec0-0x1402cbf42

=== object +0x0c setter/getter ===
  00000001402CB780: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001402CB785: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CB78A: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  00000001402CB78F: 48 8B 44 24 10     mov         rax,qword ptr [rsp+10h]
  00000001402CB794: 8B 00              mov         eax,dword ptr [rax]
  00000001402CB796: 89 41 0C           mov         dword ptr [rcx+0Ch],eax
  00000001402CB799: C3                 ret
  00000001402CB79A: CC                 int         3
  00000001402CB79B: CC                 int         3
  00000001402CB79C: CC                 int         3
  00000001402CB79D: CC                 int         3
  00000001402CB79E: CC                 int         3
  00000001402CB79F: CC                 int         3
  00000001402CB7A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CB7A5: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001402CB7AA: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  00000001402CB7AD: C3                 ret
  00000001402CB7AE: CC                 int         3

=== +0x1c predicate and setter bodies ===
  00000001402CBEC0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CBEC5: 48 83 EC 18        sub         rsp,18h
  00000001402CBEC9: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001402CBECE: 83 78 1C 00        cmp         dword ptr [rax+1Ch],0
  00000001402CBED2: 75 09              jne         00000001402CBEDD
  00000001402CBED4: C7 04 24 01 00 00  mov         dword ptr [rsp],1
  00000001402CBEDB: EB 07              jmp         00000001402CBEE4
  00000001402CBEDD: C7 04 24 00 00 00  mov         dword ptr [rsp],0
  00000001402CBEE4: 8A 04 24           mov         al,byte ptr [rsp]
  00000001402CBEE7: 48 83 C4 18        add         rsp,18h
  00000001402CBEEB: C3                 ret
  00000001402CBEEC: CC                 int         3
  00000001402CBEED: CC                 int         3
  00000001402CBEEE: CC                 int         3
  00000001402CBEEF: CC                 int         3
  00000001402CBEF0: CC                 int         3
  00000001402CBEF1: CC                 int         3
  00000001402CBEF2: CC                 int         3
  00000001402CBEF3: CC                 int         3
  00000001402CBEF4: CC                 int         3
  00000001402CBEF5: CC                 int         3
  00000001402CBEF6: CC                 int         3
  00000001402CBEF7: CC                 int         3
  00000001402CBEF8: CC                 int         3
  00000001402CBEF9: CC                 int         3
  00000001402CBEFA: CC                 int         3
  00000001402CBEFB: CC                 int         3
  00000001402CBEFC: CC                 int         3
  00000001402CBEFD: CC                 int         3
  00000001402CBEFE: CC                 int         3
  00000001402CBEFF: CC                 int         3
  00000001402CBF00: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CBF05: 48 83 EC 18        sub         rsp,18h
  00000001402CBF09: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001402CBF0E: 83 78 1C 00        cmp         dword ptr [rax+1Ch],0
  00000001402CBF12: 74 06              je          00000001402CBF1A
  00000001402CBF14: C6 04 24 01        mov         byte ptr [rsp],1
  00000001402CBF18: EB 04              jmp         00000001402CBF1E
  00000001402CBF1A: C6 04 24 00        mov         byte ptr [rsp],0
  00000001402CBF1E: 8A 04 24           mov         al,byte ptr [rsp]
  00000001402CBF21: 48 83 C4 18        add         rsp,18h
  00000001402CBF25: C3                 ret
  00000001402CBF26: CC                 int         3
  00000001402CBF27: CC                 int         3
  00000001402CBF28: CC                 int         3
  00000001402CBF29: CC                 int         3
  00000001402CBF2A: CC                 int         3
  00000001402CBF2B: CC                 int         3
  00000001402CBF2C: CC                 int         3
  00000001402CBF2D: CC                 int         3
  00000001402CBF2E: CC                 int         3
  00000001402CBF2F: CC                 int         3
  00000001402CBF30: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CBF35: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001402CBF3A: C7 40 1C 01 00 00  mov         dword ptr [rax+1Ch],1
  00000001402CBF41: C3                 ret
  00000001402CBF42: CC                 int         3
