
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DF89C0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DF89C5: 48 83 EC 38        sub         rsp,38h
  0000000140DF89C9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DF89CE: 48 81 C1 88 3F 00  add         rcx,3F88h
                    00
  0000000140DF89D5: E8 06 C1 20 FF     call        0000000140004AE0
  0000000140DF89DA: 85 C0              test        eax,eax
  0000000140DF89DC: 7E 0A              jle         0000000140DF89E8
  0000000140DF89DE: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140DF89E6: EB 08              jmp         0000000140DF89F0
  0000000140DF89E8: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140DF89F0: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  0000000140DF89F4: 48 83 C4 38        add         rsp,38h
  0000000140DF89F8: C3                 ret

  Summary

     1E02000 .text
