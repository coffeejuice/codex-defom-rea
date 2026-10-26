
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140739130: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140739135: 48 83 EC 38        sub         rsp,38h
  0000000140739139: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140739142: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140739147: E8 C4 A0 CC 00     call        0000000141403210
  000000014073914C: 90                 nop
  000000014073914D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140739152: 48 8D 05 5F BA E6  lea         rax,[00000001425A4BB8h]
                    01
  0000000140739159: 48 89 01           mov         qword ptr [rcx],rax
  000000014073915C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140739161: E8 6A 00 00 00     call        00000001407391D0
  0000000140739166: 90                 nop
  0000000140739167: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014073916C: 48 83 C4 38        add         rsp,38h
  0000000140739170: C3                 ret

  Summary

     1E02000 .text
