Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400EBC80: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400EBC85: 48 83 EC 38        sub         rsp,38h
  00000001400EBC89: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400EBC92: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400EBC97: 48 8D 05 9A 00 0A  lea         rax,[000000014218BD38h]
                    02
  00000001400EBC9E: 48 89 01           mov         qword ptr [rcx],rax
  00000001400EBCA1: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400EBCA6: 48 8D 05 4B 00 0A  lea         rax,[000000014218BCF8h]
                    02
  00000001400EBCAD: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001400EBCB1: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400EBCB6: 48 8B 40 38        mov         rax,qword ptr [rax+38h]
  00000001400EBCBA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400EBCBF: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400EBCC4: E8 9F 3F 3F 01     call        00000001414DFC68
  00000001400EBCC9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400EBCCE: 48 81 C1 90 00 00  add         rcx,90h
                    00
  00000001400EBCD5: E8 F6 8B F1 FF     call        00000001400048D0
  00000001400EBCDA: 90                 nop
  00000001400EBCDB: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400EBCE0: E8 DB BD 28 00     call        0000000140377AC0
  00000001400EBCE5: 48 83 C4 38        add         rsp,38h
  00000001400EBCE9: C3                 ret

  Summary

     1E02000 .text
