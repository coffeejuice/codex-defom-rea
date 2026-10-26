
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014073CFA0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014073CFA5: 48 83 EC 38        sub         rsp,38h
  000000014073CFA9: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014073CFB2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014073CFB7: E8 54 62 CC 00     call        0000000141403210
  000000014073CFBC: 90                 nop
  000000014073CFBD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014073CFC2: 48 8D 05 3F FD E6  lea         rax,[00000001425ACD08h]
                    01
  000000014073CFC9: 48 89 01           mov         qword ptr [rcx],rax
  000000014073CFCC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014073CFD1: E8 6A 00 00 00     call        000000014073D040
  000000014073CFD6: 90                 nop
  000000014073CFD7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014073CFDC: 48 83 C4 38        add         rsp,38h
  000000014073CFE0: C3                 ret

  Summary

     1E02000 .text
