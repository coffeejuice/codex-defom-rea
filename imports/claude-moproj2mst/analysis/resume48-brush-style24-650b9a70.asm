Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650B9A70: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000650B9A75: 57                 push        rdi
  00000000650B9A76: 48 83 EC 50        sub         rsp,50h
  00000000650B9A7A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650B9A83: 48 89 5C 24 68     mov         qword ptr [rsp+68h],rbx
  00000000650B9A88: 48 89 74 24 70     mov         qword ptr [rsp+70h],rsi
  00000000650B9A8D: 48 8B FA           mov         rdi,rdx
  00000000650B9A90: 48 8B D9           mov         rbx,rcx
  00000000650B9A93: E8 A8 E6 07 00     call        0000000065138140
  00000000650B9A98: 90                 nop
  00000000650B9A99: 48 8D 35 B8 8C 5F  lea         rsi,[00000000656B2758h]
                    00
  00000000650B9AA0: 48 89 33           mov         qword ptr [rbx],rsi
  00000000650B9AA3: 66 83 7F 08 00     cmp         word ptr [rdi+8],0
  00000000650B9AA8: 74 74              je          00000000650B9B1E
  00000000650B9AAA: 48 C7 43 10 00 00  mov         qword ptr [rbx+10h],0
                    00 00
  00000000650B9AB2: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650B9AB7: FF 15 7B CD 5D 00  call        qword ptr [0000000065696838h]
  00000000650B9ABD: 4C 8B C0           mov         r8,rax
  00000000650B9AC0: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000000650B9AC5: 48 8B CF           mov         rcx,rdi
  00000000650B9AC8: E8 F3 C2 FF FF     call        00000000650B5DC0
  00000000650B9ACD: 90                 nop
  00000000650B9ACE: 48 8B D0           mov         rdx,rax
  00000000650B9AD1: 48 8B CB           mov         rcx,rbx
  00000000650B9AD4: E8 47 F4 FF FF     call        00000000650B8F20
  00000000650B9AD9: 90                 nop
  00000000650B9ADA: 48 89 74 24 38     mov         qword ptr [rsp+38h],rsi
  00000000650B9ADF: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000000650B9AE4: 48 85 C9           test        rcx,rcx
  00000000650B9AE7: 74 25              je          00000000650B9B0E
  00000000650B9AE9: FF 15 89 CE 5D 00  call        qword ptr [0000000065696978h]
  00000000650B9AEF: 84 C0              test        al,al
  00000000650B9AF1: 75 1B              jne         00000000650B9B0E
  00000000650B9AF3: 48 8B 7C 24 48     mov         rdi,qword ptr [rsp+48h]
  00000000650B9AF8: 48 85 FF           test        rdi,rdi
  00000000650B9AFB: 74 11              je          00000000650B9B0E
  00000000650B9AFD: 48 8B CF           mov         rcx,rdi
  00000000650B9B00: E8 3B 8D FF FF     call        00000000650B2840
  00000000650B9B05: 48 8B CF           mov         rcx,rdi
  00000000650B9B08: E8 A1 75 52 00     call        00000000655E10AE
  00000000650B9B0D: 90                 nop
  00000000650B9B0E: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000650B9B13: E8 78 E6 07 00     call        0000000065138190
  00000000650B9B18: 90                 nop
  00000000650B9B19: 48 8B C3           mov         rax,rbx
  00000000650B9B1C: EB 1C              jmp         00000000650B9B3A
  00000000650B9B1E: 48 8B 4F 10        mov         rcx,qword ptr [rdi+10h]
  00000000650B9B22: 48 89 4B 10        mov         qword ptr [rbx+10h],rcx
  00000000650B9B26: 48 85 C9           test        rcx,rcx
  00000000650B9B29: 74 0C              je          00000000650B9B37
  00000000650B9B2B: FF 15 8F CD 5D 00  call        qword ptr [00000000656968C0h]
  00000000650B9B31: 90                 nop
  00000000650B9B32: 48 8B C3           mov         rax,rbx
  00000000650B9B35: EB 03              jmp         00000000650B9B3A
  00000000650B9B37: 48 8B C3           mov         rax,rbx
  00000000650B9B3A: 48 8B 5C 24 68     mov         rbx,qword ptr [rsp+68h]
  00000000650B9B3F: 48 8B 74 24 70     mov         rsi,qword ptr [rsp+70h]
  00000000650B9B44: 48 83 C4 50        add         rsp,50h
  00000000650B9B48: 5F                 pop         rdi
  00000000650B9B49: C3                 ret

  Summary

      695000 .text
