
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407A51C0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407A51C5: 48 83 EC 38        sub         rsp,38h
  00000001407A51C9: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001407A51D2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407A51D7: E8 94 9F 00 00     call        00000001407AF170
  00000001407A51DC: 90                 nop
  00000001407A51DD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407A51E2: 48 8D 05 AF 53 F0  lea         rax,[00000001426AA598h]
                    01
  00000001407A51E9: 48 89 01           mov         qword ptr [rcx],rax
  00000001407A51EC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407A51F1: E8 6A 00 00 00     call        00000001407A5260
  00000001407A51F6: 90                 nop
  00000001407A51F7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001407A51FC: 48 83 C4 38        add         rsp,38h
  00000001407A5200: C3                 ret

  Summary

     1E02000 .text
