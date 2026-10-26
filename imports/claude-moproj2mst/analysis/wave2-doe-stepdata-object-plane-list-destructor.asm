
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402081A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402081A5: 48 83 EC 28        sub         rsp,28h
  00000001402081A9: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001402081AE: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001402081B1: FF 15 31 C1 BF 01  call        qword ptr [0000000141E042E8h]
  00000001402081B7: 0F B6 C0           movzx       eax,al
  00000001402081BA: 85 C0              test        eax,eax
  00000001402081BC: 75 12              jne         00000001402081D0
  00000001402081BE: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  00000001402081C3: 48 8B 12           mov         rdx,qword ptr [rdx]
  00000001402081C6: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001402081CB: E8 80 D4 E1 FF     call        0000000140025650
  00000001402081D0: 48 83 C4 28        add         rsp,28h
  00000001402081D4: C3                 ret

  Summary

     1E02000 .text
