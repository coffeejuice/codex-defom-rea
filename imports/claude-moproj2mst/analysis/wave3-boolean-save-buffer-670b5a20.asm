Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B5A20: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000670B5A25: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000670B5A2A: 57                 push        rdi
  00000000670B5A2B: 48 83 EC 20        sub         rsp,20h
  00000000670B5A2F: 33 F6              xor         esi,esi
  00000000670B5A31: 29 51 18           sub         dword ptr [rcx+18h],edx
  00000000670B5A34: 8B FA              mov         edi,edx
  00000000670B5A36: 48 8B D9           mov         rbx,rcx
  00000000670B5A39: 79 03              jns         00000000670B5A3E
  00000000670B5A3B: 89 71 18           mov         dword ptr [rcx+18h],esi
  00000000670B5A3E: E8 6D EE FF FF     call        00000000670B48B0
  00000000670B5A43: 3B F8              cmp         edi,eax
  00000000670B5A45: 7C 6E              jl          00000000670B5AB5
  00000000670B5A47: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  00000000670B5A50: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670B5A53: 2B F8              sub         edi,eax
  00000000670B5A55: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  00000000670B5A58: 2B 41 08           sub         eax,dword ptr [rcx+8]
  00000000670B5A5B: 83 F8 01           cmp         eax,1
  00000000670B5A5E: 0F 84 89 00 00 00  je          00000000670B5AED
  00000000670B5A64: 85 C0              test        eax,eax
  00000000670B5A66: 7E 3B              jle         00000000670B5AA3
  00000000670B5A68: 8B 01              mov         eax,dword ptr [rcx]
  00000000670B5A6A: 83 F8 01           cmp         eax,1
  00000000670B5A6D: 74 0E              je          00000000670B5A7D
  00000000670B5A6F: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670B5A72: 48 8B CB           mov         rcx,rbx
  00000000670B5A75: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B5A78: E8 E3 BE F7 FF     call        0000000067031960
  00000000670B5A7D: 48 8B 13           mov         rdx,qword ptr [rbx]
  00000000670B5A80: 4C 63 42 08        movsxd      r8,dword ptr [rdx+8]
  00000000670B5A84: 4A 8B 4C C2 18     mov         rcx,qword ptr [rdx+r8*8+18h]
  00000000670B5A89: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670B5A8D: 75 0A              jne         00000000670B5A99
  00000000670B5A8F: 4A 8B 4C C2 18     mov         rcx,qword ptr [rdx+r8*8+18h]
  00000000670B5A94: E8 17 A9 F6 FF     call        00000000670203B0
  00000000670B5A99: 33 D2              xor         edx,edx
  00000000670B5A9B: 48 8B CB           mov         rcx,rbx
  00000000670B5A9E: E8 AD 33 F9 FF     call        0000000067048E50
  00000000670B5AA3: FF 4B 10           dec         dword ptr [rbx+10h]
  00000000670B5AA6: 48 8B CB           mov         rcx,rbx
  00000000670B5AA9: 89 73 08           mov         dword ptr [rbx+8],esi
  00000000670B5AAC: E8 FF ED FF FF     call        00000000670B48B0
  00000000670B5AB1: 3B F8              cmp         edi,eax
  00000000670B5AB3: 7D 9B              jge         00000000670B5A50
  00000000670B5AB5: 01 7B 08           add         dword ptr [rbx+8],edi
  00000000670B5AB8: 8B 43 08           mov         eax,dword ptr [rbx+8]
  00000000670B5ABB: 3B 43 0C           cmp         eax,dword ptr [rbx+0Ch]
  00000000670B5ABE: 75 0B              jne         00000000670B5ACB
  00000000670B5AC0: 39 73 10           cmp         dword ptr [rbx+10h],esi
  00000000670B5AC3: 75 18              jne         00000000670B5ADD
  00000000670B5AC5: 89 73 08           mov         dword ptr [rbx+8],esi
  00000000670B5AC8: 89 73 0C           mov         dword ptr [rbx+0Ch],esi
  00000000670B5ACB: 39 73 10           cmp         dword ptr [rbx+10h],esi
  00000000670B5ACE: 75 0D              jne         00000000670B5ADD
  00000000670B5AD0: 39 73 0C           cmp         dword ptr [rbx+0Ch],esi
  00000000670B5AD3: 75 08              jne         00000000670B5ADD
  00000000670B5AD5: 48 8B CB           mov         rcx,rbx
  00000000670B5AD8: E8 D3 FD FF FF     call        00000000670B58B0
  00000000670B5ADD: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000670B5AE2: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  00000000670B5AE7: 48 83 C4 20        add         rsp,20h
  00000000670B5AEB: 5F                 pop         rdi
  00000000670B5AEC: C3                 ret
  00000000670B5AED: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  00000000670B5AF1: 48 8B D1           mov         rdx,rcx
  00000000670B5AF4: 48 8B 4C C1 18     mov         rcx,qword ptr [rcx+rax*8+18h]
  00000000670B5AF9: 8B 43 14           mov         eax,dword ptr [rbx+14h]
  00000000670B5AFC: 39 41 08           cmp         dword ptr [rcx+8],eax
  00000000670B5AFF: 74 29              je          00000000670B5B2A
  00000000670B5B01: 8B 02              mov         eax,dword ptr [rdx]
  00000000670B5B03: 83 F8 01           cmp         eax,1
  00000000670B5B06: 74 0E              je          00000000670B5B16
  00000000670B5B08: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670B5B0B: 48 8B CB           mov         rcx,rbx
  00000000670B5B0E: 8B 50 04           mov         edx,dword ptr [rax+4]
  00000000670B5B11: E8 4A BE F7 FF     call        0000000067031960
  00000000670B5B16: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670B5B19: 8B 53 14           mov         edx,dword ptr [rbx+14h]
  00000000670B5B1C: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  00000000670B5B20: 48 8D 4C C1 18     lea         rcx,[rcx+rax*8+18h]
  00000000670B5B25: E8 D6 90 F7 FF     call        000000006702EC00
  00000000670B5B2A: 89 73 10           mov         dword ptr [rbx+10h],esi
  00000000670B5B2D: EB 96              jmp         00000000670B5AC5

  Summary

      1A7000 .text
