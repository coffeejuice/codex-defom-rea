Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654DFA70: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000654DFA75: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000654DFA7A: 57                 push        rdi
  00000000654DFA7B: 48 83 EC 30        sub         rsp,30h
  00000000654DFA7F: 49 8B F8           mov         rdi,r8
  00000000654DFA82: 48 8B DA           mov         rbx,rdx
  00000000654DFA85: 48 8B F1           mov         rsi,rcx
  00000000654DFA88: E8 C3 B7 FC FF     call        00000000654AB250
  00000000654DFA8D: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  00000000654DFA91: 80 B8 25 04 00 00  cmp         byte ptr [rax+0000000000000425h],0
                    00
  00000000654DFA98: 0F 84 D6 00 00 00  je          00000000654DFB74
  00000000654DFA9E: 83 3F 00           cmp         dword ptr [rdi],0
  00000000654DFAA1: 7C 66              jl          00000000654DFB09
  00000000654DFAA3: 83 7F 04 00        cmp         dword ptr [rdi+4],0
  00000000654DFAA7: 7C 60              jl          00000000654DFB09
  00000000654DFAA9: 48 83 7F 10 00     cmp         qword ptr [rdi+10h],0
  00000000654DFAAE: 74 59              je          00000000654DFB09
  00000000654DFAB0: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000654DFAB3: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000654DFAB8: 4C 8B C7           mov         r8,rdi
  00000000654DFABB: 48 8B CE           mov         rcx,rsi
  00000000654DFABE: FF 90 B8 01 00 00  call        qword ptr [rax+00000000000001B8h]
  00000000654DFAC4: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000654DFAC9: 33 D2              xor         edx,edx
  00000000654DFACB: FF 15 B7 7B 1B 00  call        qword ptr [0000000065697688h]
  00000000654DFAD1: 48 8B CE           mov         rcx,rsi
  00000000654DFAD4: E8 57 89 F4 FF     call        0000000065428430
  00000000654DFAD9: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  00000000654DFADD: 48 83 C1 14        add         rcx,14h
  00000000654DFAE1: FF 15 79 6D 1B 00  call        qword ptr [0000000065696860h]
  00000000654DFAE7: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000654DFAEC: 8B D0              mov         edx,eax
  00000000654DFAEE: FF 15 FC 80 1B 00  call        qword ptr [0000000065697BF0h]
  00000000654DFAF4: 48 8B CE           mov         rcx,rsi
  00000000654DFAF7: E8 34 89 F4 FF     call        0000000065428430
  00000000654DFAFC: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000654DFB01: 48 8B C8           mov         rcx,rax
  00000000654DFB04: E8 97 2A B8 FF     call        00000000650625A0
  00000000654DFB09: 83 3B 00           cmp         dword ptr [rbx],0
  00000000654DFB0C: 7C 66              jl          00000000654DFB74
  00000000654DFB0E: 83 7B 04 00        cmp         dword ptr [rbx+4],0
  00000000654DFB12: 7C 60              jl          00000000654DFB74
  00000000654DFB14: 48 83 7B 10 00     cmp         qword ptr [rbx+10h],0
  00000000654DFB19: 74 59              je          00000000654DFB74
  00000000654DFB1B: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000654DFB1E: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000654DFB23: 4C 8B C3           mov         r8,rbx
  00000000654DFB26: 48 8B CE           mov         rcx,rsi
  00000000654DFB29: FF 90 B8 01 00 00  call        qword ptr [rax+00000000000001B8h]
  00000000654DFB2F: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000654DFB34: 33 D2              xor         edx,edx
  00000000654DFB36: FF 15 4C 7B 1B 00  call        qword ptr [0000000065697688h]
  00000000654DFB3C: 48 8B CE           mov         rcx,rsi
  00000000654DFB3F: E8 EC 88 F4 FF     call        0000000065428430
  00000000654DFB44: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  00000000654DFB48: 48 83 C1 14        add         rcx,14h
  00000000654DFB4C: FF 15 0E 6D 1B 00  call        qword ptr [0000000065696860h]
  00000000654DFB52: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000654DFB57: 8B D0              mov         edx,eax
  00000000654DFB59: FF 15 91 80 1B 00  call        qword ptr [0000000065697BF0h]
  00000000654DFB5F: 48 8B CE           mov         rcx,rsi
  00000000654DFB62: E8 C9 88 F4 FF     call        0000000065428430
  00000000654DFB67: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000654DFB6C: 48 8B C8           mov         rcx,rax
  00000000654DFB6F: E8 2C 2A B8 FF     call        00000000650625A0
  00000000654DFB74: E8 E7 98 FB FF     call        0000000065499460
  00000000654DFB79: 84 C0              test        al,al
  00000000654DFB7B: 74 73              je          00000000654DFBF0
  00000000654DFB7D: 83 3B 00           cmp         dword ptr [rbx],0
  00000000654DFB80: 7C 6E              jl          00000000654DFBF0
  00000000654DFB82: 83 7B 04 00        cmp         dword ptr [rbx+4],0
  00000000654DFB86: 7C 68              jl          00000000654DFBF0
  00000000654DFB88: 48 83 7B 10 00     cmp         qword ptr [rbx+10h],0
  00000000654DFB8D: 74 61              je          00000000654DFBF0
  00000000654DFB8F: 48 8B 7E 08        mov         rdi,qword ptr [rsi+8]
  00000000654DFB93: 80 BF A2 03 00 00  cmp         byte ptr [rdi+00000000000003A2h],0
                    00
  00000000654DFB9A: 74 1E              je          00000000654DFBBA
  00000000654DFB9C: 83 BF F8 02 00 00  cmp         dword ptr [rdi+00000000000002F8h],5
                    05
  00000000654DFBA3: 74 15              je          00000000654DFBBA
  00000000654DFBA5: 48 8B CF           mov         rcx,rdi
  00000000654DFBA8: E8 33 3B FC FF     call        00000000654A36E0
  00000000654DFBAD: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  00000000654DFBB1: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000654DFBB4: FF 90 F0 01 00 00  call        qword ptr [rax+00000000000001F0h]
  00000000654DFBBA: 48 8B D3           mov         rdx,rbx
  00000000654DFBBD: 48 8B CF           mov         rcx,rdi
  00000000654DFBC0: E8 0B D4 FF FF     call        00000000654DCFD0
  00000000654DFBC5: 8D 58 01           lea         ebx,[rax+1]
  00000000654DFBC8: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  00000000654DFBCC: 48 83 B8 00 04 00  cmp         qword ptr [rax+0000000000000400h],0
                    00 00
  00000000654DFBD4: 74 02              je          00000000654DFBD8
  00000000654DFBD6: FF C3              inc         ebx
  00000000654DFBD8: 48 8B CE           mov         rcx,rsi
  00000000654DFBDB: E8 50 88 F4 FF     call        0000000065428430
  00000000654DFBE0: 41 B8 05 80 00 00  mov         r8d,8005h
  00000000654DFBE6: 8B D3              mov         edx,ebx
  00000000654DFBE8: 48 8B C8           mov         rcx,rax
  00000000654DFBEB: E8 70 2D FC FF     call        00000000654A2960
  00000000654DFBF0: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000654DFBF5: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  00000000654DFBFA: 48 83 C4 30        add         rsp,30h
  00000000654DFBFE: 5F                 pop         rdi
  00000000654DFBFF: C3                 ret

  Summary

      695000 .text
