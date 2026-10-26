
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065406D50: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065406D55: 53                 push        rbx
  0000000065406D56: 56                 push        rsi
  0000000065406D57: 57                 push        rdi
  0000000065406D58: 48 83 EC 30        sub         rsp,30h
  0000000065406D5C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065406D65: 48 8B F2           mov         rsi,rdx
  0000000065406D68: 48 8B D9           mov         rbx,rcx
  0000000065406D6B: B9 10 02 00 00     mov         ecx,210h
  0000000065406D70: E8 FD A3 1D 00     call        00000000655E1172
  0000000065406D75: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000065406D7A: 33 FF              xor         edi,edi
  0000000065406D7C: 48 85 C0           test        rax,rax
  0000000065406D7F: 74 0A              je          0000000065406D8B
  0000000065406D81: 48 8B C8           mov         rcx,rax
  0000000065406D84: E8 F7 FA FF FF     call        0000000065406880
  0000000065406D89: EB 03              jmp         0000000065406D8E
  0000000065406D8B: 48 8B C7           mov         rax,rdi
  0000000065406D8E: 89 7C 24 68        mov         dword ptr [rsp+68h],edi
  0000000065406D92: 4C 8D 4C 24 68     lea         r9,[rsp+68h]
  0000000065406D97: 4C 8B C6           mov         r8,rsi
  0000000065406D9A: 48 8B D0           mov         rdx,rax
  0000000065406D9D: 48 8B CB           mov         rcx,rbx
  0000000065406DA0: E8 5B A1 C6 FF     call        0000000065070F00
  0000000065406DA5: 90                 nop
  0000000065406DA6: 48 8D 05 DB 50 30  lea         rax,[000000006570BE88h]
                    00
  0000000065406DAD: 48 89 03           mov         qword ptr [rbx],rax
  0000000065406DB0: 48 8D 05 91 50 30  lea         rax,[000000006570BE48h]
                    00
  0000000065406DB7: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  0000000065406DBB: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  0000000065406DBF: 48 89 B8 F0 01 00  mov         qword ptr [rax+00000000000001F0h],rdi
                    00
  0000000065406DC6: 48 89 B8 F8 01 00  mov         qword ptr [rax+00000000000001F8h],rdi
                    00
  0000000065406DCD: 48 89 B8 00 02 00  mov         qword ptr [rax+0000000000000200h],rdi
                    00
  0000000065406DD4: 48 8B 73 08        mov         rsi,qword ptr [rbx+8]
  0000000065406DD8: 48 83 BE 00 02 00  cmp         qword ptr [rsi+0000000000000200h],0
                    00 00
  0000000065406DE0: 0F 85 84 00 00 00  jne         0000000065406E6A
  0000000065406DE6: B9 28 00 00 00     mov         ecx,28h
  0000000065406DEB: E8 82 A3 1D 00     call        00000000655E1172
  0000000065406DF0: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000065406DF5: 48 85 C0           test        rax,rax
  0000000065406DF8: 74 0E              je          0000000065406E08
  0000000065406DFA: 48 8B D3           mov         rdx,rbx
  0000000065406DFD: 48 8B C8           mov         rcx,rax
  0000000065406E00: E8 1B 57 FF FF     call        00000000653FC520
  0000000065406E05: 48 8B F8           mov         rdi,rax
  0000000065406E08: 48 89 BE 00 02 00  mov         qword ptr [rsi+0000000000000200h],rdi
                    00
  0000000065406E0F: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000065406E12: 33 D2              xor         edx,edx
  0000000065406E14: 48 8B CF           mov         rcx,rdi
  0000000065406E17: FF 50 58           call        qword ptr [rax+58h]
  0000000065406E1A: 48 8B D3           mov         rdx,rbx
  0000000065406E1D: 48 8B 8E 00 02 00  mov         rcx,qword ptr [rsi+0000000000000200h]
                    00
  0000000065406E24: FF 15 06 04 29 00  call        qword ptr [0000000065697230h]
  0000000065406E2A: C6 86 08 02 00 00  mov         byte ptr [rsi+0000000000000208h],1
                    01
  0000000065406E31: 48 8B CB           mov         rcx,rbx
  0000000065406E34: E8 07 F2 FF FF     call        0000000065406040
  0000000065406E39: 48 83 BE 00 02 00  cmp         qword ptr [rsi+0000000000000200h],0
                    00 00
  0000000065406E41: 74 22              je          0000000065406E65
  0000000065406E43: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  0000000065406E47: 8B 50 08           mov         edx,dword ptr [rax+8]
  0000000065406E4A: C1 EA 0F           shr         edx,0Fh
  0000000065406E4D: 80 E2 01           and         dl,1
  0000000065406E50: 74 0E              je          0000000065406E60
  0000000065406E52: 48 8B CE           mov         rcx,rsi
  0000000065406E55: E8 76 ED FF FF     call        0000000065405BD0
  0000000065406E5A: 90                 nop
  0000000065406E5B: 48 8B C3           mov         rax,rbx
  0000000065406E5E: EB 0D              jmp         0000000065406E6D
  0000000065406E60: 48 8B C3           mov         rax,rbx
  0000000065406E63: EB 08              jmp         0000000065406E6D
  0000000065406E65: 48 8B C3           mov         rax,rbx
  0000000065406E68: EB 03              jmp         0000000065406E6D
  0000000065406E6A: 48 8B C3           mov         rax,rbx
  0000000065406E6D: 48 83 C4 30        add         rsp,30h
  0000000065406E71: 5F                 pop         rdi
  0000000065406E72: 5E                 pop         rsi
  0000000065406E73: 5B                 pop         rbx
  0000000065406E74: C3                 ret

  Summary

      695000 .text
