
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014073F200: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014073F205: 48 83 EC 38        sub         rsp,38h
  000000014073F209: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014073F212: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014073F217: E8 F4 3F CC 00     call        0000000141403210
  000000014073F21C: 90                 nop
  000000014073F21D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014073F222: 48 8D 05 EF 1A E7  lea         rax,[00000001425B0D18h]
                    01
  000000014073F229: 48 89 01           mov         qword ptr [rcx],rax
  000000014073F22C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014073F231: E8 6A 00 00 00     call        000000014073F2A0
  000000014073F236: 90                 nop
  000000014073F237: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014073F23C: 48 83 C4 38        add         rsp,38h
  000000014073F240: C3                 ret

  Summary

     1E02000 .text
