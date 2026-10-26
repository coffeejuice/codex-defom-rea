Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653F8270: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000653F8275: 55                 push        rbp
  00000000653F8276: 56                 push        rsi
  00000000653F8277: 57                 push        rdi
  00000000653F8278: 48 83 EC 30        sub         rsp,30h
  00000000653F827C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653F8285: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000653F828A: 49 8B F0           mov         rsi,r8
  00000000653F828D: 48 8B EA           mov         rbp,rdx
  00000000653F8290: 48 8B F9           mov         rdi,rcx
  00000000653F8293: B9 40 02 00 00     mov         ecx,240h
  00000000653F8298: E8 D5 8E 1E 00     call        00000000655E1172
  00000000653F829D: 48 8B D8           mov         rbx,rax
  00000000653F82A0: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000000653F82A5: 48 85 C0           test        rax,rax
  00000000653F82A8: 74 36              je          00000000653F82E0
  00000000653F82AA: BA 00 02 00 00     mov         edx,200h
  00000000653F82AF: 48 8B C8           mov         rcx,rax
  00000000653F82B2: E8 29 89 F7 FF     call        0000000065370BE0
  00000000653F82B7: 4C 8D 1D 22 1D 30  lea         r11,[00000000656F9FE0h]
                    00
  00000000653F82BE: 4C 89 1B           mov         qword ptr [rbx],r11
  00000000653F82C1: 48 C7 83 30 02 00  mov         qword ptr [rbx+0000000000000230h],0
                    00 00 00 00 00
  00000000653F82CC: 8B 83 38 02 00 00  mov         eax,dword ptr [rbx+0000000000000238h]
  00000000653F82D2: 83 E0 C2           and         eax,0FFFFFFC2h
  00000000653F82D5: 83 C8 02           or          eax,2
  00000000653F82D8: 89 83 38 02 00 00  mov         dword ptr [rbx+0000000000000238h],eax
  00000000653F82DE: EB 02              jmp         00000000653F82E2
  00000000653F82E0: 33 DB              xor         ebx,ebx
  00000000653F82E2: 4C 8B C6           mov         r8,rsi
  00000000653F82E5: 48 8B D3           mov         rdx,rbx
  00000000653F82E8: 48 8B CF           mov         rcx,rdi
  00000000653F82EB: E8 60 8D F7 FF     call        0000000065371050
  00000000653F82F0: 90                 nop
  00000000653F82F1: 48 8D 05 00 0B 31  lea         rax,[0000000065708DF8h]
                    00
  00000000653F82F8: 48 89 07           mov         qword ptr [rdi],rax
  00000000653F82FB: 48 8D 05 B6 0A 31  lea         rax,[0000000065708DB8h]
                    00
  00000000653F8302: 48 89 47 10        mov         qword ptr [rdi+10h],rax
  00000000653F8306: 48 8B 5F 08        mov         rbx,qword ptr [rdi+8]
  00000000653F830A: 48 8B D5           mov         rdx,rbp
  00000000653F830D: 48 8B CF           mov         rcx,rdi
  00000000653F8310: E8 4B 9A F7 FF     call        0000000065371D60
  00000000653F8315: 48 8B CB           mov         rcx,rbx
  00000000653F8318: E8 C3 FC FF FF     call        00000000653F7FE0
  00000000653F831D: 90                 nop
  00000000653F831E: 48 8B C7           mov         rax,rdi
  00000000653F8321: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000653F8326: 48 83 C4 30        add         rsp,30h
  00000000653F832A: 5F                 pop         rdi
  00000000653F832B: 5E                 pop         rsi
  00000000653F832C: 5D                 pop         rbp
  00000000653F832D: C3                 ret

  Summary

      695000 .text
