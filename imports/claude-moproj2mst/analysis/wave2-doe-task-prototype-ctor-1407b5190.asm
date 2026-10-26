
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407B5190: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407B5195: 48 83 EC 38        sub         rsp,38h
  00000001407B5199: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001407B51A2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407B51A7: E8 24 99 DC 00     call        000000014157EAD0
  00000001407B51AC: 90                 nop
  00000001407B51AD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407B51B2: 48 8D 05 CF 9C F0  lea         rax,[00000001426BEE88h]
                    01
  00000001407B51B9: 48 89 01           mov         qword ptr [rcx],rax
  00000001407B51BC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407B51C1: E8 6A 00 00 00     call        00000001407B5230
  00000001407B51C6: 90                 nop
  00000001407B51C7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001407B51CC: 48 83 C4 38        add         rsp,38h
  00000001407B51D0: C3                 ret

  Summary

     1E02000 .text
