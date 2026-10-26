; DUMPBIN decoded from 0x670629d0 through 0x67063b24; selected intervals [(1728457486, 1728457519), (1728457963, 1728458010)]
  0000000067062B0E: 8D 43 D0           lea         eax,[rbx-30h]
  0000000067062B11: 83 F8 48           cmp         eax,48h
  0000000067062B14: 0F 87 D8 0E 00 00  ja          00000000670639F2
  0000000067062B1A: 48 98              cdqe
  0000000067062B1C: 0F B6 84 06 DC 3A  movzx       eax,byte ptr [rsi+rax+0000000000063ADCh]
                    06 00
  0000000067062B24: 8B 8C 86 98 3A 06  mov         ecx,dword ptr [rsi+rax*4+0000000000063A98h]
                    00
  0000000067062B2B: 48 03 CE           add         rcx,rsi
  0000000067062B2E: FF E1              jmp         rcx
  0000000067062CEB: BA 80 03 00 00     mov         edx,380h
  0000000067062CF0: 48 8B 8F 98 00 00  mov         rcx,qword ptr [rdi+0000000000000098h]
                    00
  0000000067062CF7: E8 14 82 FF FF     call        000000006705AF10
  0000000067062CFC: BA 09 00 00 00     mov         edx,9
  0000000067062D01: 44 8D 42 04        lea         r8d,[rdx+4]
  0000000067062D05: 48 8B 8F 98 00 00  mov         rcx,qword ptr [rdi+0000000000000098h]
                    00
  0000000067062D0C: E8 0F F7 FF FF     call        0000000067062420
  0000000067062D11: B8 07 00 00 00     mov         eax,7
  0000000067062D16: E9 51 0D 00 00     jmp         0000000067063A6C
