
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014079F1E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014079F1E5: 48 83 EC 38        sub         rsp,38h
  000000014079F1E9: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014079F1F2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014079F1F7: E8 D4 F8 DD 00     call        000000014157EAD0
  000000014079F1FC: 90                 nop
  000000014079F1FD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014079F202: 48 8D 05 4F F3 EF  lea         rax,[000000014269E558h]
                    01
  000000014079F209: 48 89 01           mov         qword ptr [rcx],rax
  000000014079F20C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014079F211: E8 6A 00 00 00     call        000000014079F280
  000000014079F216: 90                 nop
  000000014079F217: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014079F21C: 48 83 C4 38        add         rsp,38h
  000000014079F220: C3                 ret

  Summary

     1E02000 .text
