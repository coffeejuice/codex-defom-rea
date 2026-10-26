
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014157EB20: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014157EB25: 48 83 EC 38        sub         rsp,38h
  000000014157EB29: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014157EB32: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014157EB37: 48 8D 05 4A FB 99  lea         rax,[0000000142F1E688h]
                    01
  000000014157EB3E: 48 89 01           mov         qword ptr [rcx],rax
  000000014157EB41: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014157EB46: E8 45 00 00 00     call        000000014157EB90
  000000014157EB4B: 90                 nop
  000000014157EB4C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014157EB51: E8 AA 47 E8 FF     call        0000000141403300
  000000014157EB56: 48 83 C4 38        add         rsp,38h
  000000014157EB5A: C3                 ret

  Summary

     1E02000 .text
