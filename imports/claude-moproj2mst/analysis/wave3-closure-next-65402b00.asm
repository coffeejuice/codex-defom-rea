Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065402B00: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065402B05: 53                 push        rbx
  0000000065402B06: 55                 push        rbp
  0000000065402B07: 56                 push        rsi
  0000000065402B08: 57                 push        rdi
  0000000065402B09: 41 54              push        r12
  0000000065402B0B: 48 83 EC 30        sub         rsp,30h
  0000000065402B0F: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065402B18: 49 8B E8           mov         rbp,r8
  0000000065402B1B: 44 8B E2           mov         r12d,edx
  0000000065402B1E: 48 8B F1           mov         rsi,rcx
  0000000065402B21: B9 F8 01 00 00     mov         ecx,1F8h
  0000000065402B26: E8 47 E6 1D 00     call        00000000655E1172
  0000000065402B2B: 48 8B D8           mov         rbx,rax
  0000000065402B2E: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000065402B33: 48 85 C0           test        rax,rax
  0000000065402B36: 74 39              je          0000000065402B71
  0000000065402B38: BA 04 08 04 00     mov         edx,40804h
  0000000065402B3D: 48 8B C8           mov         rcx,rax
  0000000065402B40: E8 3B BC C5 FF     call        000000006505E780
  0000000065402B45: 4C 8D 1D 94 85 30  lea         r11,[000000006570B0E0h]
                    00
  0000000065402B4C: 4C 89 1B           mov         qword ptr [rbx],r11
  0000000065402B4F: 33 FF              xor         edi,edi
  0000000065402B51: 48 89 BB E0 01 00  mov         qword ptr [rbx+00000000000001E0h],rdi
                    00
  0000000065402B58: C7 83 E8 01 00 00  mov         dword ptr [rbx+00000000000001E8h],1
                    01 00 00 00
  0000000065402B62: 89 BB EC 01 00 00  mov         dword ptr [rbx+00000000000001ECh],edi
  0000000065402B68: 80 A3 F0 01 00 00  and         byte ptr [rbx+00000000000001F0h],0F8h
                    F8
  0000000065402B6F: EB 05              jmp         0000000065402B76
  0000000065402B71: 33 FF              xor         edi,edi
  0000000065402B73: 48 8B DF           mov         rbx,rdi
  0000000065402B76: 89 7C 24 68        mov         dword ptr [rsp+68h],edi
  0000000065402B7A: 4C 8D 4C 24 68     lea         r9,[rsp+68h]
  0000000065402B7F: 4C 8B C5           mov         r8,rbp
  0000000065402B82: 48 8B D3           mov         rdx,rbx
  0000000065402B85: 48 8B CE           mov         rcx,rsi
  0000000065402B88: E8 73 E3 C6 FF     call        0000000065070F00
  0000000065402B8D: 90                 nop
  0000000065402B8E: 48 8D 05 F3 86 30  lea         rax,[000000006570B288h]
                    00
  0000000065402B95: 48 89 06           mov         qword ptr [rsi],rax
  0000000065402B98: 48 8D 05 B1 86 30  lea         rax,[000000006570B250h]
                    00
  0000000065402B9F: 48 89 46 10        mov         qword ptr [rsi+10h],rax
  0000000065402BA3: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  0000000065402BA7: 48 89 A8 E0 01 00  mov         qword ptr [rax+00000000000001E0h],rbp
                    00
  0000000065402BAE: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  0000000065402BB2: 44 89 A0 E8 01 00  mov         dword ptr [rax+00000000000001E8h],r12d
                    00
  0000000065402BB9: 41 83 FC 01        cmp         r12d,1
  0000000065402BBD: 40 0F 94 C7        sete        dil
  0000000065402BC1: 8D 57 0B           lea         edx,[rdi+0Bh]
  0000000065402BC4: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000065402BC9: E8 C2 C8 C2 FF     call        000000006502F490
  0000000065402BCE: 90                 nop
  0000000065402BCF: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000065402BD4: 48 8B CE           mov         rcx,rsi
  0000000065402BD7: E8 14 7B C6 FF     call        000000006506A6F0
  0000000065402BDC: 90                 nop
  0000000065402BDD: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000065402BE2: E8 B9 C5 C2 FF     call        000000006502F1A0
  0000000065402BE7: 90                 nop
  0000000065402BE8: 48 8B C6           mov         rax,rsi
  0000000065402BEB: 48 83 C4 30        add         rsp,30h
  0000000065402BEF: 41 5C              pop         r12
  0000000065402BF1: 5F                 pop         rdi
  0000000065402BF2: 5E                 pop         rsi
  0000000065402BF3: 5D                 pop         rbp
  0000000065402BF4: 5B                 pop         rbx
  0000000065402BF5: C3                 ret

  Summary

      695000 .text
