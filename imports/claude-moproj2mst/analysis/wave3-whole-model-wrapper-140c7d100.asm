Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C7D100: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C7D105: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C7D10A: 53                 push        rbx
  0000000140C7D10B: 48 83 EC 50        sub         rsp,50h
  0000000140C7D10F: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C7D118: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C7D120: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C7D125: E8 A6 DB FF FF     call        0000000140C7ACD0
  0000000140C7D12A: 48 85 C0           test        rax,rax
  0000000140C7D12D: 74 5E              je          0000000140C7D18D
  0000000140C7D12F: C7 44 24 24 00 00  mov         dword ptr [rsp+24h],0
                    00 00
  0000000140C7D137: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000140C7D13C: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140C7D141: FF 15 09 6E 18 01  call        qword ptr [0000000141E03F50h]
  0000000140C7D147: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C7D14C: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140C7D151: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140C7D156: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C7D15B: FF 15 F7 6D 18 01  call        qword ptr [0000000141E03F58h]
  0000000140C7D161: 48 8B D8           mov         rbx,rax
  0000000140C7D164: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C7D169: E8 62 DB FF FF     call        0000000140C7ACD0
  0000000140C7D16E: 4C 8D 44 24 24     lea         r8,[rsp+24h]
  0000000140C7D173: 48 8B D3           mov         rdx,rbx
  0000000140C7D176: 48 8B C8           mov         rcx,rax
  0000000140C7D179: E8 D2 81 C1 FF     call        0000000140895350
  0000000140C7D17E: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140C7D182: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140C7D187: FF 15 03 71 18 01  call        qword ptr [0000000141E04290h]
  0000000140C7D18D: 83 7C 24 20 00     cmp         dword ptr [rsp+20h],0
  0000000140C7D192: 75 0A              jne         0000000140C7D19E
  0000000140C7D194: C7 44 24 48 01 00  mov         dword ptr [rsp+48h],1
                    00 00
  0000000140C7D19C: EB 08              jmp         0000000140C7D1A6
  0000000140C7D19E: C7 44 24 48 00 00  mov         dword ptr [rsp+48h],0
                    00 00
  0000000140C7D1A6: 8A 44 24 48        mov         al,byte ptr [rsp+48h]
  0000000140C7D1AA: 48 83 C4 50        add         rsp,50h
  0000000140C7D1AE: 5B                 pop         rbx
  0000000140C7D1AF: C3                 ret

  Summary

     1E02000 .text
