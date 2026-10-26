
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407AF170: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407AF175: 48 83 EC 38        sub         rsp,38h
  00000001407AF179: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001407AF182: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407AF187: E8 44 F9 DC 00     call        000000014157EAD0
  00000001407AF18C: 90                 nop
  00000001407AF18D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407AF192: 48 8D 05 9F 79 F0  lea         rax,[00000001426B6B38h]
                    01
  00000001407AF199: 48 89 01           mov         qword ptr [rcx],rax
  00000001407AF19C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407AF1A1: E8 6A 00 00 00     call        00000001407AF210
  00000001407AF1A6: 90                 nop
  00000001407AF1A7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001407AF1AC: 48 83 C4 38        add         rsp,38h
  00000001407AF1B0: C3                 ret

  Summary

     1E02000 .text
