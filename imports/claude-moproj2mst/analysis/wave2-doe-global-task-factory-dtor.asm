
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407C79E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407C79E5: 48 83 EC 48        sub         rsp,48h
  00000001407C79E9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001407C79EE: 48 8D 05 33 36 F6  lea         rax,[000000014272B028h]
                    01
  00000001407C79F5: 48 89 01           mov         qword ptr [rcx],rax
  00000001407C79F8: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001407C79FD: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001407C7A01: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001407C7A06: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001407C7A0A: 48 8B 00           mov         rax,qword ptr [rax]
  00000001407C7A0D: FF 50 18           call        qword ptr [rax+18h]
  00000001407C7A10: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001407C7A15: 48 83 78 08 00     cmp         qword ptr [rax+8],0
  00000001407C7A1A: 74 44              je          00000001407C7A60
  00000001407C7A1C: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001407C7A21: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001407C7A25: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001407C7A2A: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001407C7A2F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001407C7A34: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001407C7A3A: 74 1B              je          00000001407C7A57
  00000001407C7A3C: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001407C7A41: 48 8B 00           mov         rax,qword ptr [rax]
  00000001407C7A44: BA 01 00 00 00     mov         edx,1
  00000001407C7A49: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001407C7A4E: FF 10              call        qword ptr [rax]
  00000001407C7A50: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001407C7A55: EB 09              jmp         00000001407C7A60
  00000001407C7A57: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000001407C7A60: 48 83 C4 48        add         rsp,48h
  00000001407C7A64: C3                 ret

  Summary

     1E02000 .text
