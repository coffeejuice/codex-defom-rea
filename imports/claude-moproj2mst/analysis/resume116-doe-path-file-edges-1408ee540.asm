Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001408EE540: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001408EE545: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001408EE54A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001408EE54F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001408EE554: 48 81 EC 28 05 00  sub         rsp,528h
                    00
  00000001408EE55B: 48 C7 84 24 10 05  mov         qword ptr [rsp+0000000000000510h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001408EE567: 48 8B 05 C2 FA 26  mov         rax,qword ptr [0000000143B5E030h]
                    03
  00000001408EE56E: 48 33 C4           xor         rax,rsp
  00000001408EE571: 48 89 84 24 18 05  mov         qword ptr [rsp+0000000000000518h],rax
                    00 00
  00000001408EE579: C7 84 24 68 04 00  mov         dword ptr [rsp+0000000000000468h],0
                    00 00 00 00 00
  00000001408EE584: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001408EE588: F2 0F 11 84 24 60  movsd       mmword ptr [rsp+0000000000000460h],xmm0
                    04 00 00
  00000001408EE591: 48 83 BC 24 38 05  cmp         qword ptr [rsp+0000000000000538h],0
                    00 00 00
  00000001408EE59A: 75 0A              jne         00000001408EE5A6
  00000001408EE59C: B8 FF FF FF FF     mov         eax,0FFFFFFFFh
  00000001408EE5A1: E9 1B 02 00 00     jmp         00000001408EE7C1
  00000001408EE5A6: 48 8B 8C 24 30 05  mov         rcx,qword ptr [rsp+0000000000000530h]
                    00 00
  00000001408EE5AE: E8 1D E8 FF FF     call        00000001408ECDD0
  00000001408EE5B3: 83 BC 24 40 05 00  cmp         dword ptr [rsp+0000000000000540h],6
                    00 06
  00000001408EE5BB: 0F 85 B6 00 00 00  jne         00000001408EE677
  00000001408EE5C1: 48 8B 94 24 38 05  mov         rdx,qword ptr [rsp+0000000000000538h]
                    00 00
  00000001408EE5C9: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001408EE5CE: E8 5D E3 FF FF     call        00000001408EC930
  00000001408EE5D3: 89 84 24 68 04 00  mov         dword ptr [rsp+0000000000000468h],eax
                    00
  00000001408EE5DA: 83 BC 24 68 04 00  cmp         dword ptr [rsp+0000000000000468h],0
                    00 00
  00000001408EE5E2: 0F 85 8A 00 00 00  jne         00000001408EE672
  00000001408EE5E8: 33 D2              xor         edx,edx
  00000001408EE5EA: 48 8D 8C 24 70 04  lea         rcx,[rsp+0000000000000470h]
                    00 00
  00000001408EE5F2: E8 69 40 13 00     call        0000000140A22660
  00000001408EE5F7: 90                 nop
  00000001408EE5F8: C7 44 24 40 2C 01  mov         dword ptr [rsp+40h],12Ch
                    00 00
  00000001408EE600: C7 44 24 38 01 00  mov         dword ptr [rsp+38h],1
                    00 00
  00000001408EE608: C6 44 24 30 00     mov         byte ptr [rsp+30h],0
  00000001408EE60D: C7 44 24 28 03 00  mov         dword ptr [rsp+28h],3
                    00 00
  00000001408EE615: 48 8D 84 24 60 04  lea         rax,[rsp+0000000000000460h]
                    00 00
  00000001408EE61D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001408EE622: 4C 8B 8C 24 48 05  mov         r9,qword ptr [rsp+0000000000000548h]
                    00 00
  00000001408EE62A: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  00000001408EE62F: 48 8B 94 24 38 05  mov         rdx,qword ptr [rsp+0000000000000538h]
                    00 00
  00000001408EE637: 48 8D 8C 24 70 04  lea         rcx,[rsp+0000000000000470h]
                    00 00
  00000001408EE63F: E8 3C 41 13 00     call        0000000140A22780
  00000001408EE644: 89 84 24 68 04 00  mov         dword ptr [rsp+0000000000000468h],eax
                    00
  00000001408EE64B: 8B 84 24 68 04 00  mov         eax,dword ptr [rsp+0000000000000468h]
                    00
  00000001408EE652: 89 84 24 00 05 00  mov         dword ptr [rsp+0000000000000500h],eax
                    00
  00000001408EE659: 48 8D 8C 24 70 04  lea         rcx,[rsp+0000000000000470h]
                    00 00
  00000001408EE661: E8 BA 40 13 00     call        0000000140A22720
  00000001408EE666: 8B 84 24 00 05 00  mov         eax,dword ptr [rsp+0000000000000500h]
                    00
  00000001408EE66D: E9 4F 01 00 00     jmp         00000001408EE7C1
  00000001408EE672: E9 45 01 00 00     jmp         00000001408EE7BC
  00000001408EE677: 83 BC 24 40 05 00  cmp         dword ptr [rsp+0000000000000540h],7
                    00 07
  00000001408EE67F: 0F 85 98 00 00 00  jne         00000001408EE71D
  00000001408EE685: 48 8B 94 24 38 05  mov         rdx,qword ptr [rsp+0000000000000538h]
                    00 00
  00000001408EE68D: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001408EE692: E8 99 E2 FF FF     call        00000001408EC930
  00000001408EE697: 89 84 24 68 04 00  mov         dword ptr [rsp+0000000000000468h],eax
                    00
  00000001408EE69E: 83 BC 24 68 04 00  cmp         dword ptr [rsp+0000000000000468h],0
                    00 00
  00000001408EE6A6: 75 70              jne         00000001408EE718
  00000001408EE6A8: 33 D2              xor         edx,edx
  00000001408EE6AA: 48 8D 8C 24 98 04  lea         rcx,[rsp+0000000000000498h]
                    00 00
  00000001408EE6B2: E8 E9 D2 28 00     call        0000000140B7B9A0
  00000001408EE6B7: 90                 nop
  00000001408EE6B8: C7 44 24 28 2C 01  mov         dword ptr [rsp+28h],12Ch
                    00 00
  00000001408EE6C0: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  00000001408EE6C8: 4C 8B 8C 24 48 05  mov         r9,qword ptr [rsp+0000000000000548h]
                    00 00
  00000001408EE6D0: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  00000001408EE6D5: 48 8B 94 24 38 05  mov         rdx,qword ptr [rsp+0000000000000538h]
                    00 00
  00000001408EE6DD: 48 8D 8C 24 98 04  lea         rcx,[rsp+0000000000000498h]
                    00 00
  00000001408EE6E5: E8 A6 D3 28 00     call        0000000140B7BA90
  00000001408EE6EA: 89 84 24 68 04 00  mov         dword ptr [rsp+0000000000000468h],eax
                    00
  00000001408EE6F1: 8B 84 24 68 04 00  mov         eax,dword ptr [rsp+0000000000000468h]
                    00
  00000001408EE6F8: 89 84 24 04 05 00  mov         dword ptr [rsp+0000000000000504h],eax
                    00
  00000001408EE6FF: 48 8D 8C 24 98 04  lea         rcx,[rsp+0000000000000498h]
                    00 00
  00000001408EE707: E8 54 D3 28 00     call        0000000140B7BA60
  00000001408EE70C: 8B 84 24 04 05 00  mov         eax,dword ptr [rsp+0000000000000504h]
                    00
  00000001408EE713: E9 A9 00 00 00     jmp         00000001408EE7C1
  00000001408EE718: E9 9F 00 00 00     jmp         00000001408EE7BC
  00000001408EE71D: 83 BC 24 40 05 00  cmp         dword ptr [rsp+0000000000000540h],2
                    00 02
  00000001408EE725: 0F 85 91 00 00 00  jne         00000001408EE7BC
  00000001408EE72B: 48 8D 8C 24 C0 04  lea         rcx,[rsp+00000000000004C0h]
                    00 00
  00000001408EE733: E8 D8 85 00 00     call        00000001408F6D10
  00000001408EE738: 90                 nop
  00000001408EE739: 4C 8B 84 24 30 05  mov         r8,qword ptr [rsp+0000000000000530h]
                    00 00
  00000001408EE741: 48 8B 94 24 38 05  mov         rdx,qword ptr [rsp+0000000000000538h]
                    00 00
  00000001408EE749: 48 8D 8C 24 C0 04  lea         rcx,[rsp+00000000000004C0h]
                    00 00
  00000001408EE751: E8 BA 88 00 00     call        00000001408F7010
  00000001408EE756: 0F B6 C0           movzx       eax,al
  00000001408EE759: 89 84 24 68 04 00  mov         dword ptr [rsp+0000000000000468h],eax
                    00
  00000001408EE760: 83 BC 24 68 04 00  cmp         dword ptr [rsp+0000000000000468h],0
                    00 00
  00000001408EE768: 74 23              je          00000001408EE78D
  00000001408EE76A: C7 84 24 08 05 00  mov         dword ptr [rsp+0000000000000508h],0
                    00 00 00 00 00
  00000001408EE775: 48 8D 8C 24 C0 04  lea         rcx,[rsp+00000000000004C0h]
                    00 00
  00000001408EE77D: E8 CE 85 00 00     call        00000001408F6D50
  00000001408EE782: 8B 84 24 08 05 00  mov         eax,dword ptr [rsp+0000000000000508h]
                    00
  00000001408EE789: EB 36              jmp         00000001408EE7C1
  00000001408EE78B: EB 21              jmp         00000001408EE7AE
  00000001408EE78D: C7 84 24 0C 05 00  mov         dword ptr [rsp+000000000000050Ch],0FFFFFFFFh
                    00 FF FF FF FF
  00000001408EE798: 48 8D 8C 24 C0 04  lea         rcx,[rsp+00000000000004C0h]
                    00 00
  00000001408EE7A0: E8 AB 85 00 00     call        00000001408F6D50
  00000001408EE7A5: 8B 84 24 0C 05 00  mov         eax,dword ptr [rsp+000000000000050Ch]
                    00
  00000001408EE7AC: EB 13              jmp         00000001408EE7C1
  00000001408EE7AE: 48 8D 8C 24 C0 04  lea         rcx,[rsp+00000000000004C0h]
                    00 00
  00000001408EE7B6: E8 95 85 00 00     call        00000001408F6D50
  00000001408EE7BB: 90                 nop
  00000001408EE7BC: B8 FE FF FF FF     mov         eax,0FFFFFFFEh
  00000001408EE7C1: 48 8B 8C 24 18 05  mov         rcx,qword ptr [rsp+0000000000000518h]
                    00 00
  00000001408EE7C9: 48 33 CC           xor         rcx,rsp
  00000001408EE7CC: E8 3F F7 B7 00     call        000000014146DF10
  00000001408EE7D1: 48 81 C4 28 05 00  add         rsp,528h
                    00
  00000001408EE7D8: C3                 ret

  Summary

     1E02000 .text
