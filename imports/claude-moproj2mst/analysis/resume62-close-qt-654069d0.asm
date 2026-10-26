
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654069D0: 40 57              push        rdi
  00000000654069D2: 48 83 EC 40        sub         rsp,40h
  00000000654069D6: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000654069DF: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000654069E4: 48 89 6C 24 60     mov         qword ptr [rsp+60h],rbp
  00000000654069E9: 48 89 74 24 68     mov         qword ptr [rsp+68h],rsi
  00000000654069EE: 41 8B E8           mov         ebp,r8d
  00000000654069F1: 48 8B F1           mov         rsi,rcx
  00000000654069F4: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000654069F8: 48 8D 8F E8 01 00  lea         rcx,[rdi+00000000000001E8h]
                    00
  00000000654069FF: FF 15 7B FD 28 00  call        qword ptr [0000000065696780h]
  0000000065406A05: 85 ED              test        ebp,ebp
  0000000065406A07: 7E 75              jle         0000000065406A7E
  0000000065406A09: 48 83 BF F8 01 00  cmp         qword ptr [rdi+00000000000001F8h],0
                    00 00
  0000000065406A11: 75 5A              jne         0000000065406A6D
  0000000065406A13: B9 20 00 00 00     mov         ecx,20h
  0000000065406A18: E8 55 A7 1D 00     call        00000000655E1172
  0000000065406A1D: 48 8B D8           mov         rbx,rax
  0000000065406A20: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000065406A25: 48 85 C0           test        rax,rax
  0000000065406A28: 74 18              je          0000000065406A42
  0000000065406A2A: 48 8B D6           mov         rdx,rsi
  0000000065406A2D: 48 8B C8           mov         rcx,rax
  0000000065406A30: FF 15 52 12 29 00  call        qword ptr [0000000065697C88h]
  0000000065406A36: 4C 8D 1D 5B 53 30  lea         r11,[000000006570BD98h]
                    00
  0000000065406A3D: 4C 89 1B           mov         qword ptr [rbx],r11
  0000000065406A40: EB 02              jmp         0000000065406A44
  0000000065406A42: 33 DB              xor         ebx,ebx
  0000000065406A44: 48 89 9F F8 01 00  mov         qword ptr [rdi+00000000000001F8h],rbx
                    00
  0000000065406A4B: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000065406A53: 4C 8D 0D 26 53 30  lea         r9,[000000006570BD80h]
                    00
  0000000065406A5A: 4C 8B C6           mov         r8,rsi
  0000000065406A5D: 48 8D 15 B4 08 2B  lea         rdx,[00000000656B7318h]
                    00
  0000000065406A64: 48 8B CB           mov         rcx,rbx
  0000000065406A67: FF 15 AB FC 28 00  call        qword ptr [0000000065696718h]
  0000000065406A6D: 8B D5              mov         edx,ebp
  0000000065406A6F: 48 8B 8F F8 01 00  mov         rcx,qword ptr [rdi+00000000000001F8h]
                    00
  0000000065406A76: FF 15 EC 11 29 00  call        qword ptr [0000000065697C68h]
  0000000065406A7C: EB 20              jmp         0000000065406A9E
  0000000065406A7E: 48 8B 8F F8 01 00  mov         rcx,qword ptr [rdi+00000000000001F8h]
                    00
  0000000065406A85: 48 85 C9           test        rcx,rcx
  0000000065406A88: 74 14              je          0000000065406A9E
  0000000065406A8A: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065406A8D: BA 01 00 00 00     mov         edx,1
  0000000065406A92: FF 50 18           call        qword ptr [rax+18h]
  0000000065406A95: 33 DB              xor         ebx,ebx
  0000000065406A97: 48 89 9F F8 01 00  mov         qword ptr [rdi+00000000000001F8h],rbx
                    00
  0000000065406A9E: 48 8B CE           mov         rcx,rsi
  0000000065406AA1: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  0000000065406AA6: 48 8B 6C 24 60     mov         rbp,qword ptr [rsp+60h]
  0000000065406AAB: 48 8B 74 24 68     mov         rsi,qword ptr [rsp+68h]
  0000000065406AB0: 48 83 C4 40        add         rsp,40h
  0000000065406AB4: 5F                 pop         rdi
  0000000065406AB5: E9 C6 F9 FF FF     jmp         0000000065406480

  Summary

      695000 .text
