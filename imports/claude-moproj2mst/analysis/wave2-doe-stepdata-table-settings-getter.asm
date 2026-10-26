
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411F1A10: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411F1A15: 48 83 EC 38        sub         rsp,38h
  00000001411F1A19: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  00000001411F1A22: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411F1A27: E8 54 FF FF FF     call        00000001411F1980
  00000001411F1A2C: 48 85 C0           test        rax,rax
  00000001411F1A2F: 75 04              jne         00000001411F1A35
  00000001411F1A31: 33 C0              xor         eax,eax
  00000001411F1A33: EB 6E              jmp         00000001411F1AA3
  00000001411F1A35: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411F1A3A: E8 71 00 00 00     call        00000001411F1AB0
  00000001411F1A3F: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001411F1A43: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411F1A48: E8 33 FF FF FF     call        00000001411F1980
  00000001411F1A4D: 48
  00000001411F1A4E: 8D
  00000001411F1A4F: 54                 push        rsp

  Summary

     1E02000 .text
