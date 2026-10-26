
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407C7FE0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407C7FE5: 48 83 EC 38        sub         rsp,38h
  00000001407C7FE9: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001407C7FF2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407C7FF7: 48 8D 05 C2 66 F6  lea         rax,[000000014272E6C0h]
                    01
  00000001407C7FFE: 48 89 01           mov         qword ptr [rcx],rax
  00000001407C8001: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407C8006: 48 83 C1 10        add         rcx,10h
  00000001407C800A: E8 61 B4 83 FF     call        0000000140003470
  00000001407C800F: 90                 nop
  00000001407C8010: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407C8015: 48 83 C1 08        add         rcx,8
  00000001407C8019: E8 52 B4 83 FF     call        0000000140003470
  00000001407C801E: 48 83 C4 38        add         rsp,38h
  00000001407C8022: C3                 ret

  Summary

     1E02000 .text
