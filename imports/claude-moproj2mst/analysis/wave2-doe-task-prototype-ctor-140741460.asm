
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140741460: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140741465: 48 83 EC 38        sub         rsp,38h
  0000000140741469: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140741472: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140741477: E8 94 1D CC 00     call        0000000141403210
  000000014074147C: 90                 nop
  000000014074147D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140741482: 48 8D 05 AF 37 E7  lea         rax,[00000001425B4C38h]
                    01
  0000000140741489: 48 89 01           mov         qword ptr [rcx],rax
  000000014074148C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140741491: E8 6A 00 00 00     call        0000000140741500
  0000000140741496: 90                 nop
  0000000140741497: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014074149C: 48 83 C4 38        add         rsp,38h
  00000001407414A0: C3                 ret

  Summary

     1E02000 .text
