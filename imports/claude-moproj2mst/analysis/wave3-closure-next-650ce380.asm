Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650CE380: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000650CE385: 57                 push        rdi
  00000000650CE386: 48 83 EC 30        sub         rsp,30h
  00000000650CE38A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650CE393: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000650CE398: 48 8B D9           mov         rbx,rcx
  00000000650CE39B: E8 A0 9D 06 00     call        0000000065138140
  00000000650CE3A0: 90                 nop
  00000000650CE3A1: 48 8D 05 88 73 5E  lea         rax,[00000000656B5730h]
                    00
  00000000650CE3A8: 48 89 03           mov         qword ptr [rbx],rax
  00000000650CE3AB: 48 C7 43 10 00 00  mov         qword ptr [rbx+10h],0
                    00 00
  00000000650CE3B3: E8 48 FB FF FF     call        00000000650CDF00
  00000000650CE3B8: 83 3D B1 B2 8F 00  cmp         dword ptr [00000000659C9670h],0
                    00
  00000000650CE3BF: 75 40              jne         00000000650CE401
  00000000650CE3C1: 48 8D 0D F8 72 5E  lea         rcx,[00000000656B56C0h]
                    00
  00000000650CE3C8: FF 15 CA 85 5C 00  call        qword ptr [0000000065696998h]
  00000000650CE3CE: 48 8B 7B 10        mov         rdi,qword ptr [rbx+10h]
  00000000650CE3D2: 48 85 FF           test        rdi,rdi
  00000000650CE3D5: 74 5C              je          00000000650CE433
  00000000650CE3D7: 48 C7 43 10 00 00  mov         qword ptr [rbx+10h],0
                    00 00
  00000000650CE3DF: 48 85 FF           test        rdi,rdi
  00000000650CE3E2: 74 4F              je          00000000650CE433
  00000000650CE3E4: 48 8D 4F 18        lea         rcx,[rdi+18h]
  00000000650CE3E8: FF 15 8A 85 5C 00  call        qword ptr [0000000065696978h]
  00000000650CE3EE: 84 C0              test        al,al
  00000000650CE3F0: 75 41              jne         00000000650CE433
  00000000650CE3F2: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000650CE3F5: BA 01 00 00 00     mov         edx,1
  00000000650CE3FA: 48 8B CF           mov         rcx,rdi
  00000000650CE3FD: FF 10              call        qword ptr [rax]
  00000000650CE3FF: EB 32              jmp         00000000650CE433
  00000000650CE401: 48 8B 7B 10        mov         rdi,qword ptr [rbx+10h]
  00000000650CE405: 48 85 FF           test        rdi,rdi
  00000000650CE408: 74 29              je          00000000650CE433
  00000000650CE40A: 48 C7 43 10 00 00  mov         qword ptr [rbx+10h],0
                    00 00
  00000000650CE412: 48 85 FF           test        rdi,rdi
  00000000650CE415: 74 1C              je          00000000650CE433
  00000000650CE417: 48 8D 4F 18        lea         rcx,[rdi+18h]
  00000000650CE41B: FF 15 57 85 5C 00  call        qword ptr [0000000065696978h]
  00000000650CE421: 84 C0              test        al,al
  00000000650CE423: 75 0E              jne         00000000650CE433
  00000000650CE425: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000650CE428: BA 01 00 00 00     mov         edx,1
  00000000650CE42D: 48 8B CF           mov         rcx,rdi
  00000000650CE430: FF 10              call        qword ptr [rax]
  00000000650CE432: 90                 nop
  00000000650CE433: 48 8B C3           mov         rax,rbx
  00000000650CE436: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000650CE43B: 48 83 C4 30        add         rsp,30h
  00000000650CE43F: 5F                 pop         rdi
  00000000650CE440: C3                 ret

  Summary

      695000 .text
