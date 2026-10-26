
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140A99AF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140A99AF5: 48 83 EC 38        sub         rsp,38h
  0000000140A99AF9: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140A99B02: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140A99B07: 48 83 C1 08        add         rcx,8
  0000000140A99B0B: 41 B8 01 00 00 00  mov         r8d,1
  0000000140A99B11: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140A99B16: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140A99B19: E8 22 3E 61 FF     call        00000001400AD940
  0000000140A99B1E: 90                 nop
  0000000140A99B1F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140A99B24: E8 07 26 21 00     call        0000000140CAC130
  0000000140A99B29: 48 83 C4 38        add         rsp,38h
  0000000140A99B2D: C3                 ret

  Summary

     1E02000 .text
