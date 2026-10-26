Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B4900: 40 57              push        rdi
  00000000670B4902: 48 83 EC 30        sub         rsp,30h
  00000000670B4906: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670B490F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670B4914: 48 8B D9           mov         rbx,rcx
  00000000670B4917: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000670B491B: 48 8B 87 E8 00 00  mov         rax,qword ptr [rdi+00000000000000E8h]
                    00
  00000000670B4922: 83 78 08 00        cmp         dword ptr [rax+8],0
  00000000670B4926: 75 13              jne         00000000670B493B
  00000000670B4928: 48 8D 0D 61 56 17  lea         rcx,[0000000067229F90h]
                    00
  00000000670B492F: E8 AC 85 F6 FF     call        000000006701CEE0
  00000000670B4934: 32 C0              xor         al,al
  00000000670B4936: E9 85 00 00 00     jmp         00000000670B49C0
  00000000670B493B: E8 10 FF FF FF     call        00000000670B4850
  00000000670B4940: 4C 8B 1B           mov         r11,qword ptr [rbx]
  00000000670B4943: 48 8B CB           mov         rcx,rbx
  00000000670B4946: 41 FF 53 68        call        qword ptr [r11+68h]
  00000000670B494A: 4C 8B 5B 08        mov         r11,qword ptr [rbx+8]
  00000000670B494E: 41 83 BB 20 01 00  cmp         dword ptr [r11+0000000000000120h],0
                    00 00
  00000000670B4956: 75 66              jne         00000000670B49BE
  00000000670B4958: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670B495B: 48 8B CB           mov         rcx,rbx
  00000000670B495E: FF 90 D8 00 00 00  call        qword ptr [rax+00000000000000D8h]
  00000000670B4964: 48 8B 10           mov         rdx,qword ptr [rax]
  00000000670B4967: 48 8B C8           mov         rcx,rax
  00000000670B496A: FF 52 40           call        qword ptr [rdx+40h]
  00000000670B496D: 84 C0              test        al,al
  00000000670B496F: 74 0C              je          00000000670B497D
  00000000670B4971: 48 8B CB           mov         rcx,rbx
  00000000670B4974: E8 D7 FE FF FF     call        00000000670B4850
  00000000670B4979: B0 01              mov         al,1
  00000000670B497B: EB 43              jmp         00000000670B49C0
  00000000670B497D: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670B4982: 48 8B 8F F0 00 00  mov         rcx,qword ptr [rdi+00000000000000F0h]
                    00
  00000000670B4989: E8 92 30 FF FF     call        00000000670A7A20
  00000000670B498E: 90                 nop
  00000000670B498F: C7 87 20 01 00 00  mov         dword ptr [rdi+0000000000000120h],9
                    09 00 00 00
  00000000670B4999: 48 8D 8F 90 00 00  lea         rcx,[rdi+0000000000000090h]
                    00
  00000000670B49A0: 48 8B D0           mov         rdx,rax
  00000000670B49A3: E8 28 BA FB FF     call        00000000670703D0
  00000000670B49A8: 90                 nop
  00000000670B49A9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670B49AE: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B49B2: 75 0A              jne         00000000670B49BE
  00000000670B49B4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670B49B9: E8 12 B7 FB FF     call        00000000670700D0
  00000000670B49BE: 32 C0              xor         al,al
  00000000670B49C0: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670B49C5: 48 83 C4 30        add         rsp,30h
  00000000670B49C9: 5F                 pop         rdi
  00000000670B49CA: C3                 ret

  Summary

      1A7000 .text
