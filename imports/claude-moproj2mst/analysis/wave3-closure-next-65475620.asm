Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065475620: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065475625: 53                 push        rbx
  0000000065475626: 48 83 EC 30        sub         rsp,30h
  000000006547562A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065475633: 48 8B D9           mov         rbx,rcx
  0000000065475636: E8 95 99 FD FF     call        000000006544EFD0
  000000006547563B: 90                 nop
  000000006547563C: 48 8D 05 5D C0 2A  lea         rax,[00000000657216A0h]
                    00
  0000000065475643: 48 89 03           mov         qword ptr [rbx],rax
  0000000065475646: 48 8B 05 7B 12 22  mov         rax,qword ptr [00000000656968C8h]
                    00
  000000006547564D: 48 89 83 60 02 00  mov         qword ptr [rbx+0000000000000260h],rax
                    00
  0000000065475654: 48 8B 0D 6D 12 22  mov         rcx,qword ptr [00000000656968C8h]
                    00
  000000006547565B: FF 15 5F 12 22 00  call        qword ptr [00000000656968C0h]
  0000000065475661: 90                 nop
  0000000065475662: 33 C0              xor         eax,eax
  0000000065475664: 48 89 83 68 02 00  mov         qword ptr [rbx+0000000000000268h],rax
                    00
  000000006547566B: 48 89 83 70 02 00  mov         qword ptr [rbx+0000000000000270h],rax
                    00
  0000000065475672: 48 89 83 78 02 00  mov         qword ptr [rbx+0000000000000278h],rax
                    00
  0000000065475679: 48 89 83 80 02 00  mov         qword ptr [rbx+0000000000000280h],rax
                    00
  0000000065475680: 48 89 83 88 02 00  mov         qword ptr [rbx+0000000000000288h],rax
                    00
  0000000065475687: 88 83 90 02 00 00  mov         byte ptr [rbx+0000000000000290h],al
  000000006547568D: C6 83 91 02 00 00  mov         byte ptr [rbx+0000000000000291h],1
                    01
  0000000065475694: 48 89 83 98 02 00  mov         qword ptr [rbx+0000000000000298h],rax
                    00
  000000006547569B: 48 89 83 A0 02 00  mov         qword ptr [rbx+00000000000002A0h],rax
                    00
  00000000654756A2: 48 89 83 A8 02 00  mov         qword ptr [rbx+00000000000002A8h],rax
                    00
  00000000654756A9: 48 8D 8B B0 02 00  lea         rcx,[rbx+00000000000002B0h]
                    00
  00000000654756B0: FF 15 B2 18 22 00  call        qword ptr [0000000065696F68h]
  00000000654756B6: 90                 nop
  00000000654756B7: 48 8D 8B B8 02 00  lea         rcx,[rbx+00000000000002B8h]
                    00
  00000000654756BE: FF 15 A4 18 22 00  call        qword ptr [0000000065696F68h]
  00000000654756C4: 90                 nop
  00000000654756C5: 48 8B C3           mov         rax,rbx
  00000000654756C8: 48 83 C4 30        add         rsp,30h
  00000000654756CC: 5B                 pop         rbx
  00000000654756CD: C3                 ret

  Summary

      695000 .text
