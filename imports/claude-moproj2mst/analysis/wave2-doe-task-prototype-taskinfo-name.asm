
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014140E5D0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014140E5D5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014140E5DA: 48 83 EC 38        sub         rsp,38h
  000000014140E5DE: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014140E5E6: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  000000014140E5EB: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  000000014140E5EF: 48 83 C2 10        add         rdx,10h
  000000014140E5F3: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000014140E5F8: FF 15 DA 73 9F 00  call        qword ptr [0000000141E059D8h]
  000000014140E5FE: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014140E602: 83 C8 01           or          eax,1
  000000014140E605: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014140E609: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014140E60E: 48 83 C4 38        add         rsp,38h
  000000014140E612: C3                 ret

  Summary

     1E02000 .text
