Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400DB1A0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001400DB1A5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001400DB1AA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400DB1AE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400DB1B3: 53                 push        rbx
  00000001400DB1B4: 48 81 EC 50 03 00  sub         rsp,350h
                    00
  00000001400DB1BB: 48 C7 84 24 28 02  mov         qword ptr [rsp+0000000000000228h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400DB1C7: 83 BC 24 68 03 00  cmp         dword ptr [rsp+0000000000000368h],0
                    00 00
  00000001400DB1CF: 0F 85 1F 17 00 00  jne         00000001400DC8F4
  00000001400DB1D5: E8 A6 99 F2 FF     call        0000000140004B80
  00000001400DB1DA: 4C 8B 9C 24 60 03  mov         r11,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001400DB1E2: 4C 89 5C 24 30     mov         qword ptr [rsp+30h],r11
  00000001400DB1E7: 8B 84 24 70 03 00  mov         eax,dword ptr [rsp+0000000000000370h]
                    00
  00000001400DB1EE: 89 84 24 30 02 00  mov         dword ptr [rsp+0000000000000230h],eax
                    00
  00000001400DB1F5: 81 BC 24 30 02 00  cmp         dword ptr [rsp+0000000000000230h],84h
                    00 84 00 00 00
  00000001400DB200: 0F 87 EE 16 00 00  ja          00000001400DC8F4
  00000001400DB206: 48 63 84 24 30 02  movsxd      rax,dword ptr [rsp+0000000000000230h]
                    00 00
  00000001400DB20E: 48 8D 0D EB 4D F2  lea         rcx,[0000000140000000h]
                    FF
  00000001400DB215: 8B 84 81 00 C9 0D  mov         eax,dword ptr [rcx+rax*4+00000000000DC900h]
                    00
  00000001400DB21C: 48 03 C1           add         rax,rcx
  00000001400DB21F: FF E0              jmp         rax
  00000001400DB221: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400DB226: E8 35 1A 00 00     call        00000001400DCC60
  00000001400DB22B: E9 C4 16 00 00     jmp         00000001400DC8F4
  00000001400DB230: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400DB235: E8 66 57 F3 FF     call        00000001400109A0
  00000001400DB23A: E9 B5 16 00 00     jmp         00000001400DC8F4
  00000001400DB23F: 48

  Summary

     1E02000 .text
