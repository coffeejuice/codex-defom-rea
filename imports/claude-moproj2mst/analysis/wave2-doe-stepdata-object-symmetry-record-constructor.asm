
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412479E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001412479E5: 48 83 EC 18        sub         rsp,18h
  00000001412479E9: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001412479EE: C6 80 F8 00 00 00  mov         byte ptr [rax+00000000000000F8h],1
                    01
  00000001412479F5: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001412479FA: C6 80 F9 00 00 00  mov         byte ptr [rax+00000000000000F9h],0
                    00
  0000000141247A01: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  0000000141247A08: EB 09              jmp         0000000141247A13
  0000000141247A0A: 8B 04 24           mov         eax,dword ptr [rsp]
  0000000141247A0D: 83 C0 01           add         eax,1
  0000000141247A10: 89 04 24           mov         dword ptr [rsp],eax
  0000000141247A13: 83 3C 24 04        cmp         dword ptr [rsp],4
  0000000141247A17: 7D 18              jge         0000000141247A31
  0000000141247A19: 48 63 0C 24        movsxd      rcx,dword ptr [rsp]
  0000000141247A1D: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000141247A22: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000141247A26: F2 0F 11 84 C8 C0  movsd       mmword ptr [rax+rcx*8+00000000000000C0h],xmm0
                    00 00 00
  0000000141247A2F: EB D9              jmp         0000000141247A0A
  0000000141247A31: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000141247A36: C7 80 FC 00 00 00  mov         dword ptr [rax+00000000000000FCh],0FFFFFFFFh
                    FF FF FF FF
  0000000141247A40: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000141247A45: C7 80 00 01 00 00  mov         dword ptr [rax+0000000000000100h],0FFFFFFFFh
                    FF FF FF FF
  0000000141247A4F: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000141247A54: 48 83 C4 18        add         rsp,18h
  0000000141247A58: C3                 ret

  Summary

     1E02000 .text
