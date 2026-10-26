Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140032AA0: 24 94              and         al,94h
  0000000140032AA2: 00 00              add         byte ptr [rax],al
  0000000140032AA4: 00 48 8B           add         byte ptr [rax-75h],cl
  0000000140032AA7: 8C 24 88           mov         word ptr [rax+rcx*4],fs
  0000000140032AAA: 00 00              add         byte ptr [rax],al
  0000000140032AAC: 00 E8              add         al,ch
  0000000140032AAE: DE 2F              fisubr      word ptr [rdi]
  0000000140032AB0: 08 00              or          byte ptr [rax],al
  0000000140032AB2: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140032ABA: 48 8B 84 24 C0 06  mov         rax,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032AC2: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140032AC5: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032ACD: FF 90 60 02 00 00  call        qword ptr [rax+0000000000000260h]
  0000000140032AD3: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140032AD7: 4C 8B 8C 24 C8 06  mov         r9,qword ptr [rsp+00000000000006C8h]
                    00 00
  0000000140032ADF: 44 8B 84 24 94 00  mov         r8d,dword ptr [rsp+0000000000000094h]
                    00 00
  0000000140032AE7: 48 8B 94 24 C0 06  mov         rdx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032AEF: 48 8B 92 60 02 00  mov         rdx,qword ptr [rdx+0000000000000260h]
                    00
  0000000140032AF6: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140032AFE: E8 5D 1A 0A 00     call        00000001400D4560
  0000000140032B03: 0F B6 C0           movzx       eax,al
  0000000140032B06: 85 C0              test        eax,eax
  0000000140032B08: 74 1C              je          0000000140032B26
  0000000140032B0A: 48 8B 8C 24 C0 06  mov         rcx,qword ptr [rsp+00000000000006C0h]
                    00 00
  0000000140032B12: 48
  0000000140032B13: 8B
  0000000140032B14: 84
  0000000140032B15: 24

  Summary

     1E02000 .text
