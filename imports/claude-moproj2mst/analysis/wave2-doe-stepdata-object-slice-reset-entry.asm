
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411FD690: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001411FD694: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411FD699: 48 83 EC 28        sub         rsp,28h
  00000001411FD69D: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001411FD6A2: 48 83 B8 38 03 00  cmp         qword ptr [rax+0000000000000338h],0
                    00 00
  00000001411FD6AA: 75 04              jne         00000001411FD6B0
  00000001411FD6AC: 33 C0              xor         eax,eax
  00000001411FD6AE: EB 15              jmp         00000001411FD6C5
  00000001411FD6B0: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  00000001411FD6B4: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001411FD6B9: 48 8B 89 38 03 00  mov         rcx,qword ptr [rcx+0000000000000338h]
                    00
  00000001411FD6C0: E8 1B C5 BF FF     call        0000000140DF9BE0
  00000001411FD6C5: 48 83 C4 28        add         rsp,28h
  00000001411FD6C9: C3                 ret

  Summary

     1E02000 .text
