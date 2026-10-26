Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140ECF980: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140ECF984: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140ECF989: 48 83 EC 38        sub         rsp,38h
  0000000140ECF98D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140ECF992: 83 B8 60 01 00 00  cmp         dword ptr [rax+0000000000000160h],0
                    00
  0000000140ECF999: 74 05              je          0000000140ECF9A0
  0000000140ECF99B: E9 01 01 00 00     jmp         0000000140ECFAA1
  0000000140ECF9A0: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECF9A5: E8 E6 15 26 FF     call        0000000140130F90
  0000000140ECF9AA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140ECF9AF: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  0000000140ECF9B4: 75 42              jne         0000000140ECF9F8
  0000000140ECF9B6: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140ECF9BB: 83 B8 74 0A 00 00  cmp         dword ptr [rax+0000000000000A74h],0
                    00
  0000000140ECF9C2: 7E 11              jle         0000000140ECF9D5
  0000000140ECF9C4: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140ECF9C9: 48 81 C1 58 0A 00  add         rcx,0A58h
                    00
  0000000140ECF9D0: E8 1B B8 9C FF     call        000000014089B1F0
  0000000140ECF9D5: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140ECF9DA: C7 80 74 0A 00 00  mov         dword ptr [rax+0000000000000A74h],0
                    00 00 00 00
  0000000140ECF9E4: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140ECF9E9: C7 80 98 0A 00 00  mov         dword ptr [rax+0000000000000A98h],0
                    00 00 00 00
  0000000140ECF9F3: E9 81 00 00 00     jmp         0000000140ECFA79
  0000000140ECF9F8: 83 7C 24 48 02     cmp         dword ptr [rsp+48h],2
  0000000140ECF9FD: 75 7A              jne         0000000140ECFA79
  0000000140ECF9FF: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140ECFA04: 83 B8 74 0A 00 00  cmp         dword ptr [rax+0000000000000A74h],0
                    00
  0000000140ECFA0B: 7E 11              jle         0000000140ECFA1E
  0000000140ECFA0D: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140ECFA12: 48 81 C1 58 0A 00  add         rcx,0A58h
                    00
  0000000140ECFA19: E8 D2 B7 9C FF     call        000000014089B1F0
  0000000140ECFA1E: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140ECFA23: C7 80 74 0A 00 00  mov         dword ptr [rax+0000000000000A74h],1
                    01 00 00 00
  0000000140ECFA2D: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140ECFA32: C7 80 98 0A 00 00  mov         dword ptr [rax+0000000000000A98h],1
                    01 00 00 00
  0000000140ECFA3C: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140ECFA41: 48 81 C1 58 0A 00  add         rcx,0A58h
                    00
  0000000140ECFA48: E8 F3 BD 9C FF     call        000000014089B840
  0000000140ECFA4D: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140ECFA52: 48 8B 80 80 0A 00  mov         rax,qword ptr [rax+0000000000000A80h]
                    00
  0000000140ECFA59: F2 0F 10 05 7F 4F  movsd       xmm0,mmword ptr [00000001421249E0h]
                    25 01
  0000000140ECFA61: F2 0F 11 00        movsd       mmword ptr [rax],xmm0
  0000000140ECFA65: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140ECFA6A: 48 8B 80 A0 0A 00  mov         rax,qword ptr [rax+0000000000000AA0h]
                    00
  0000000140ECFA71: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140ECFA75: F2 0F 11 00        movsd       mmword ptr [rax],xmm0
  0000000140ECFA79: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140ECFA7E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ECFA81: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECFA86: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140ECFA8C: 48 8B C8           mov         rcx,rax
  0000000140ECFA8F: E8 3C B2 DA FF     call        0000000140C7ACD0
  0000000140ECFA94: BA 22 00 00 00     mov         edx,22h
  0000000140ECFA99: 48 8B C8           mov         rcx,rax
  0000000140ECFA9C: E8 FF AF 95 FF     call        000000014082AAA0
  0000000140ECFAA1: 48 83 C4 38        add         rsp,38h
  0000000140ECFAA5: C3                 ret

  Summary

     1E02000 .text
