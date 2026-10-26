Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065475EB0: 40 57              push        rdi
  0000000065475EB2: 48 83 EC 30        sub         rsp,30h
  0000000065475EB6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065475EBF: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  0000000065475EC4: 48 8B D9           mov         rbx,rcx
  0000000065475EC7: 48 8B 81 68 02 00  mov         rax,qword ptr [rcx+0000000000000268h]
                    00
  0000000065475ECE: 48 85 C0           test        rax,rax
  0000000065475ED1: 74 0C              je          0000000065475EDF
  0000000065475ED3: 48 89 81 98 02 00  mov         qword ptr [rcx+0000000000000298h],rax
                    00
  0000000065475EDA: E9 AB 01 00 00     jmp         000000006547608A
  0000000065475EDF: BA 00 00 40 00     mov         edx,400000h
  0000000065475EE4: 48 8B 89 58 02 00  mov         rcx,qword ptr [rcx+0000000000000258h]
                    00
  0000000065475EEB: E8 50 59 F2 FF     call        000000006539B840
  0000000065475EF0: 48 89 83 98 02 00  mov         qword ptr [rbx+0000000000000298h],rax
                    00
  0000000065475EF7: 48 85 C0           test        rax,rax
  0000000065475EFA: 0F 85 8A 01 00 00  jne         000000006547608A
  0000000065475F00: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000065475F05: 48 8B 8B 58 02 00  mov         rcx,qword ptr [rbx+0000000000000258h]
                    00
  0000000065475F0C: E8 AF 60 F2 FF     call        000000006539BFC0
  0000000065475F11: 90                 nop
  0000000065475F12: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065475F17: FF 15 9B 09 22 00  call        qword ptr [00000000656968B8h]
  0000000065475F1D: 83 F8 01           cmp         eax,1
  0000000065475F20: 75 3A              jne         0000000065475F5C
  0000000065475F22: 33 D2              xor         edx,edx
  0000000065475F24: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065475F29: FF 15 11 0A 22 00  call        qword ptr [0000000065696940h]
  0000000065475F2F: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065475F32: 48 89 8B 98 02 00  mov         qword ptr [rbx+0000000000000298h],rcx
                    00
  0000000065475F39: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000065475F3E: FF 15 34 0A 22 00  call        qword ptr [0000000065696978h]
  0000000065475F44: 84 C0              test        al,al
  0000000065475F46: 0F 85 3E 01 00 00  jne         000000006547608A
  0000000065475F4C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000065475F51: FF 15 09 0A 22 00  call        qword ptr [0000000065696960h]
  0000000065475F57: E9 2E 01 00 00     jmp         000000006547608A
  0000000065475F5C: 33 FF              xor         edi,edi
  0000000065475F5E: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065475F63: FF 15 4F 09 22 00  call        qword ptr [00000000656968B8h]
  0000000065475F69: 85 C0              test        eax,eax
  0000000065475F6B: 7E 63              jle         0000000065475FD0
  0000000065475F6D: 0F 1F 00           nop         dword ptr [rax]
  0000000065475F70: 8B D7              mov         edx,edi
  0000000065475F72: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065475F77: FF 15 C3 09 22 00  call        qword ptr [0000000065696940h]
  0000000065475F7D: 48 8B 10           mov         rdx,qword ptr [rax]
  0000000065475F80: 48 8B 8B 58 02 00  mov         rcx,qword ptr [rbx+0000000000000258h]
                    00
  0000000065475F87: E8 D4 52 F2 FF     call        000000006539B260
  0000000065475F8C: 83 F8 01           cmp         eax,1
  0000000065475F8F: 75 21              jne         0000000065475FB2
  0000000065475F91: 48 83 BB 98 02 00  cmp         qword ptr [rbx+0000000000000298h],0
                    00 00
  0000000065475F99: 75 2A              jne         0000000065475FC5
  0000000065475F9B: 8B D7              mov         edx,edi
  0000000065475F9D: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065475FA2: FF 15 98 09 22 00  call        qword ptr [0000000065696940h]
  0000000065475FA8: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065475FAB: 48 89 8B 98 02 00  mov         qword ptr [rbx+0000000000000298h],rcx
                    00
  0000000065475FB2: FF C7              inc         edi
  0000000065475FB4: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065475FB9: FF 15 F9 08 22 00  call        qword ptr [00000000656968B8h]
  0000000065475FBF: 3B F8              cmp         edi,eax
  0000000065475FC1: 7D 0D              jge         0000000065475FD0
  0000000065475FC3: EB AB              jmp         0000000065475F70
  0000000065475FC5: 48 C7 83 98 02 00  mov         qword ptr [rbx+0000000000000298h],0
                    00 00 00 00 00
  0000000065475FD0: 48 83 BB 98 02 00  cmp         qword ptr [rbx+0000000000000298h],0
                    00 00
  0000000065475FD8: 74 23              je          0000000065475FFD
  0000000065475FDA: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000065475FDF: FF 15 93 09 22 00  call        qword ptr [0000000065696978h]
  0000000065475FE5: 84 C0              test        al,al
  0000000065475FE7: 0F 85 9D 00 00 00  jne         000000006547608A
  0000000065475FED: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000065475FF2: FF 15 68 09 22 00  call        qword ptr [0000000065696960h]
  0000000065475FF8: E9 8D 00 00 00     jmp         000000006547608A
  0000000065475FFD: 33 FF              xor         edi,edi
  0000000065475FFF: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065476004: FF 15 AE 08 22 00  call        qword ptr [00000000656968B8h]
  000000006547600A: 85 C0              test        eax,eax
  000000006547600C: 7E 62              jle         0000000065476070
  000000006547600E: 66 90              nop
  0000000065476010: 8B D7              mov         edx,edi
  0000000065476012: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065476017: FF 15 23 09 22 00  call        qword ptr [0000000065696940h]
  000000006547601D: 48 8B 10           mov         rdx,qword ptr [rax]
  0000000065476020: 48 8B 8B 58 02 00  mov         rcx,qword ptr [rbx+0000000000000258h]
                    00
  0000000065476027: E8 34 52 F2 FF     call        000000006539B260
  000000006547602C: 83 F8 06           cmp         eax,6
  000000006547602F: 75 21              jne         0000000065476052
  0000000065476031: 48 83 BB 98 02 00  cmp         qword ptr [rbx+0000000000000298h],0
                    00 00
  0000000065476039: 75 2A              jne         0000000065476065
  000000006547603B: 8B D7              mov         edx,edi
  000000006547603D: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065476042: FF 15 F8 08 22 00  call        qword ptr [0000000065696940h]
  0000000065476048: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006547604B: 48 89 8B 98 02 00  mov         qword ptr [rbx+0000000000000298h],rcx
                    00
  0000000065476052: FF C7              inc         edi
  0000000065476054: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065476059: FF 15 59 08 22 00  call        qword ptr [00000000656968B8h]
  000000006547605F: 3B F8              cmp         edi,eax
  0000000065476061: 7D 0D              jge         0000000065476070
  0000000065476063: EB AB              jmp         0000000065476010
  0000000065476065: 48 C7 83 98 02 00  mov         qword ptr [rbx+0000000000000298h],0
                    00 00 00 00 00
  0000000065476070: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000065476075: FF 15 FD 08 22 00  call        qword ptr [0000000065696978h]
  000000006547607B: 84 C0              test        al,al
  000000006547607D: 75 0B              jne         000000006547608A
  000000006547607F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000065476084: FF 15 D6 08 22 00  call        qword ptr [0000000065696960h]
  000000006547608A: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006547608F: 48 83 C4 30        add         rsp,30h
  0000000065476093: 5F                 pop         rdi
  0000000065476094: C3                 ret

  Summary

      695000 .text
