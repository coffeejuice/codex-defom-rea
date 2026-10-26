Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014005A8A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014005A8A5: 48 83 EC 78        sub         rsp,78h
  000000014005A8A9: 48 C7 44 24 50 FE  mov         qword ptr [rsp+50h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014005A8B2: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014005A8BA: E8 E1 4E 00 00     call        000000014005F7A0
  000000014005A8BF: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014005A8C4: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014005A8CA: 75 07              jne         000000014005A8D3
  000000014005A8CC: 32 C0              xor         al,al
  000000014005A8CE: E9 D1 00 00 00     jmp         000000014005A9A4
  000000014005A8D3: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014005A8DB: E8 B0 AE 00 00     call        0000000140065790
  000000014005A8E0: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014005A8E5: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  000000014005A8EB: 74 2D              je          000000014005A91A
  000000014005A8ED: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014005A8F2: E8 89 AE 00 00     call        0000000140065780
  000000014005A8F7: 0F B6 C0           movzx       eax,al
  000000014005A8FA: 85 C0              test        eax,eax
  000000014005A8FC: 74 0D              je          000000014005A90B
  000000014005A8FE: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014005A906: E8 A5 00 00 00     call        000000014005A9B0
  000000014005A90B: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  000000014005A910: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014005A915: E8 B6 AC 00 00     call        00000001400655D0
  000000014005A91A: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000014005A91F: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014005A927: E8 64 FD FF FF     call        000000014005A690
  000000014005A92C: 90                 nop
  000000014005A92D: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000014005A932: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014005A937: FF 15 13 96 DA 01  call        qword ptr [0000000141E03F50h]
  000000014005A93D: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014005A942: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000014005A947: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014005A94C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014005A951: FF 15 01 96 DA 01  call        qword ptr [0000000141E03F58h]
  000000014005A957: 48 8B D0           mov         rdx,rax
  000000014005A95A: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014005A95F: E8 CC B6 83 00     call        0000000140896030
  000000014005A964: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  000000014005A968: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014005A96D: FF 15 1D 99 DA 01  call        qword ptr [0000000141E04290h]
  000000014005A973: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  000000014005A978: 75 0A              jne         000000014005A984
  000000014005A97A: C7 44 24 68 01 00  mov         dword ptr [rsp+68h],1
                    00 00
  000000014005A982: EB 08              jmp         000000014005A98C
  000000014005A984: C7 44 24 68 00 00  mov         dword ptr [rsp+68h],0
                    00 00
  000000014005A98C: 0F B6 44 24 68     movzx       eax,byte ptr [rsp+68h]
  000000014005A991: 88 44 24 48        mov         byte ptr [rsp+48h],al
  000000014005A995: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014005A99A: FF 15 40 B0 DA 01  call        qword ptr [0000000141E059E0h]
  000000014005A9A0: 8A 44 24 48        mov         al,byte ptr [rsp+48h]
  000000014005A9A4: 48 83 C4 78        add         rsp,78h
  000000014005A9A8: C3                 ret

  Summary

     1E02000 .text
