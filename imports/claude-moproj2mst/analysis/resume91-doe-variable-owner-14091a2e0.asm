Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091A2E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091A2E5: 48 83 EC 58        sub         rsp,58h
  000000014091A2E9: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091A2F2: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  000000014091A2F7: 48 83 C2 38        add         rdx,38h
  000000014091A2FB: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091A300: E8 CB F7 93 FF     call        0000000140259AD0
  000000014091A305: 90                 nop
  000000014091A306: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091A30E: EB 0B              jmp         000000014091A31B
  000000014091A310: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091A314: 83 C0 01           add         eax,1
  000000014091A317: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091A31B: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091A320: E8 BB A7 6E FF     call        0000000140004AE0
  000000014091A325: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014091A329: 7D 49              jge         000000014091A374
  000000014091A32B: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091A32F: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091A334: E8 D7 68 84 FF     call        0000000140160C10
  000000014091A339: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A33C: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014091A341: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014091A346: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014091A34B: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014091A351: 74 16              je          000000014091A369
  000000014091A353: BA 01 00 00 00     mov         edx,1
  000000014091A358: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014091A35D: E8 0E 38 00 00     call        000000014091DB70
  000000014091A362: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014091A367: EB 09              jmp         000000014091A372
  000000014091A369: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  000000014091A372: EB 9C              jmp         000000014091A310
  000000014091A374: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014091A379: 48 83 C1 38        add         rcx,38h
  000000014091A37D: E8 0E 38 8F FF     call        000000014020DB90
  000000014091A382: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091A38A: EB 0B              jmp         000000014091A397
  000000014091A38C: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091A390: 83 C0 01           add         eax,1
  000000014091A393: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091A397: 83 7C 24 20 03     cmp         dword ptr [rsp+20h],3
  000000014091A39C: 7D 11              jge         000000014091A3AF
  000000014091A39E: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014091A3A3: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014091A3A8: C6 44 08 70 01     mov         byte ptr [rax+rcx+70h],1
  000000014091A3AD: EB DD              jmp         000000014091A38C
  000000014091A3AF: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091A3B4: E8 E7 DD 8E FF     call        00000001402081A0
  000000014091A3B9: 48 83 C4 58        add         rsp,58h
  000000014091A3BD: C3                 ret

  Summary

     1E02000 .text
