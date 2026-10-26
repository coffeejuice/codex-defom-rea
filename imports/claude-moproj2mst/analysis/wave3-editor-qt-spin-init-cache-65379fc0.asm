Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065379FC0: 40 57              push        rdi
  0000000065379FC2: 48 81 EC 90 00 00  sub         rsp,90h
                    00
  0000000065379FC9: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065379FD2: 48 89 9C 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rbx
                    00 00
  0000000065379FDA: 48 8B F9           mov         rdi,rcx
  0000000065379FDD: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  0000000065379FE1: B9 28 00 00 00     mov         ecx,28h
  0000000065379FE6: E8 87 71 26 00     call        00000000655E1172
  0000000065379FEB: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000065379FF3: 48 85 C0           test        rax,rax
  0000000065379FF6: 74 0D              je          000000006537A005
  0000000065379FF8: 48 8B D3           mov         rdx,rbx
  0000000065379FFB: 48 8B C8           mov         rcx,rax
  0000000065379FFE: E8 3D 55 04 00     call        00000000653BF540
  000000006537A003: EB 02              jmp         000000006537A007
  000000006537A005: 33 C0              xor         eax,eax
  000000006537A007: 48 8B D0           mov         rdx,rax
  000000006537A00A: 48 8B CB           mov         rcx,rbx
  000000006537A00D: E8 EE E4 FF FF     call        0000000065378500
  000000006537A012: 48 8D 15 DF AA 37  lea         rdx,[00000000656F4AF8h]
                    00
  000000006537A019: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006537A021: FF 15 B9 C9 31 00  call        qword ptr [00000000656969E0h]
  000000006537A027: 48 8B D0           mov         rdx,rax
  000000006537A02A: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006537A032: FF 15 D0 CB 31 00  call        qword ptr [0000000065696C08h]
  000000006537A038: 90                 nop
  000000006537A039: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  000000006537A041: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  000000006537A048: FF 15 EA C9 31 00  call        qword ptr [0000000065696A38h]
  000000006537A04E: 90                 nop
  000000006537A04F: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006537A057: FF 15 03 C7 31 00  call        qword ptr [0000000065696760h]
  000000006537A05D: B9 20 00 00 00     mov         ecx,20h
  000000006537A062: E8 0B 71 26 00     call        00000000655E1172
  000000006537A067: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  000000006537A06F: 48 85 C0           test        rax,rax
  000000006537A072: 74 10              je          000000006537A084
  000000006537A074: 4C 8B C7           mov         r8,rdi
  000000006537A077: 48 8B D3           mov         rdx,rbx
  000000006537A07A: 48 8B C8           mov         rcx,rax
  000000006537A07D: E8 9E C8 FF FF     call        0000000065376920
  000000006537A082: EB 02              jmp         000000006537A086
  000000006537A084: 33 C0              xor         eax,eax
  000000006537A086: 48 89 87 B0 02 00  mov         qword ptr [rdi+00000000000002B0h],rax
                    00
  000000006537A08D: 48 8B D0           mov         rdx,rax
  000000006537A090: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  000000006537A097: E8 74 5B 04 00     call        00000000653BFC10
  000000006537A09C: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006537A0A1: E8 6A 04 F3 FF     call        00000000652AA510
  000000006537A0A6: 90                 nop
  000000006537A0A7: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006537A0AC: 48 8B CB           mov         rcx,rbx
  000000006537A0AF: E8 5C F3 FF FF     call        0000000065379410
  000000006537A0B4: 48 8B CB           mov         rcx,rbx
  000000006537A0B7: E8 A4 35 CE FF     call        000000006505D660
  000000006537A0BC: 4C 8B 10           mov         r10,qword ptr [rax]
  000000006537A0BF: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  000000006537A0C8: 4C 8B CB           mov         r9,rbx
  000000006537A0CB: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  000000006537A0D0: BA 2C 00 00 00     mov         edx,2Ch
  000000006537A0D5: 48 8B C8           mov         rcx,rax
  000000006537A0D8: 41 FF 92 E8 00 00  call        qword ptr [r10+00000000000000E8h]
                    00
  000000006537A0DF: 89 87 48 02 00 00  mov         dword ptr [rdi+0000000000000248h],eax
  000000006537A0E5: 48 8B CB           mov         rcx,rbx
  000000006537A0E8: E8 73 35 CE FF     call        000000006505D660
  000000006537A0ED: 4C 8B 10           mov         r10,qword ptr [rax]
  000000006537A0F0: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  000000006537A0F9: 4C 8B CB           mov         r9,rbx
  000000006537A0FC: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  000000006537A101: BA 55 00 00 00     mov         edx,55h
  000000006537A106: 48 8B C8           mov         rcx,rax
  000000006537A109: 41 FF 92 E8 00 00  call        qword ptr [r10+00000000000000E8h]
                    00
  000000006537A110: 89 87 50 02 00 00  mov         dword ptr [rdi+0000000000000250h],eax
  000000006537A116: BA 0F 00 00 00     mov         edx,0Fh
  000000006537A11B: 48 8B CB           mov         rcx,rbx
  000000006537A11E: E8 3D 3C CE FF     call        000000006505DD60
  000000006537A123: C7 84 24 A0 00 00  mov         dword ptr [rsp+00000000000000A0h],1
                    00 01 00 00 00
  000000006537A12E: BA 00 10 00 00     mov         edx,1000h
  000000006537A133: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006537A13B: E8 50 9B CC FF     call        0000000065043C90
  000000006537A140: 8B 94 24 A0 00 00  mov         edx,dword ptr [rsp+00000000000000A0h]
                    00
  000000006537A147: 48 8B CB           mov         rcx,rbx
  000000006537A14A: E8 D1 39 CF FF     call        000000006506DB20
  000000006537A14F: 41 B0 01           mov         r8b,1
  000000006537A152: BA 0E 00 00 00     mov         edx,0Eh
  000000006537A157: 48 8B CB           mov         rcx,rbx
  000000006537A15A: E8 81 EE CE FF     call        0000000065068FE0
  000000006537A15F: 41 B0 01           mov         r8b,1
  000000006537A162: BA 58 00 00 00     mov         edx,58h
  000000006537A167: 48 8B CB           mov         rcx,rbx
  000000006537A16A: E8 71 EE CE FF     call        0000000065068FE0
  000000006537A16F: 90                 nop
  000000006537A170: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006537A175: E8 E6 F2 F2 FF     call        00000000652A9460
  000000006537A17A: 48 8B 9C 24 B0 00  mov         rbx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000006537A182: 48 81 C4 90 00 00  add         rsp,90h
                    00
  000000006537A189: 5F                 pop         rdi
  000000006537A18A: C3                 ret

  Summary

      695000 .text
