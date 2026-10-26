
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065406480: 41 54              push        r12
  0000000065406482: 41 55              push        r13
  0000000065406484: 48 83 EC 38        sub         rsp,38h
  0000000065406488: 4C 8B 69 08        mov         r13,qword ptr [rcx+8]
  000000006540648C: 48 89 6C 24 58     mov         qword ptr [rsp+58h],rbp
  0000000065406491: 4C 8B E1           mov         r12,rcx
  0000000065406494: 48 89 74 24 60     mov         qword ptr [rsp+60h],rsi
  0000000065406499: 49 8D 8D E8 01 00  lea         rcx,[r13+00000000000001E8h]
                    00
  00000000654064A0: 48 89 7C 24 68     mov         qword ptr [rsp+68h],rdi
  00000000654064A5: 4C 89 74 24 30     mov         qword ptr [rsp+30h],r14
  00000000654064AA: FF 15 38 05 29 00  call        qword ptr [00000000656969E8h]
  00000000654064B0: 49 8D 8D E0 01 00  lea         rcx,[r13+00000000000001E0h]
                    00
  00000000654064B7: 84 C0              test        al,al
  00000000654064B9: 40 0F 94 C5        sete        bpl
  00000000654064BD: 33 FF              xor         edi,edi
  00000000654064BF: FF 15 F3 03 29 00  call        qword ptr [00000000656968B8h]
  00000000654064C5: 85 C0              test        eax,eax
  00000000654064C7: 0F 8E 98 00 00 00  jle         0000000065406565
  00000000654064CD: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000654064D2: 66 66 66 66 66 66  nop         word ptr [rax+rax+0000000000000000h]
                    0F 1F 84 00 00 00
                    00 00
  00000000654064E0: 49 8D 8D E0 01 00  lea         rcx,[r13+00000000000001E0h]
                    00
  00000000654064E7: 8B D7              mov         edx,edi
  00000000654064E9: FF 15 51 04 29 00  call        qword ptr [0000000065696940h]
  00000000654064EF: 48 8B 18           mov         rbx,qword ptr [rax]
  00000000654064F2: 48 85 DB           test        rbx,rbx
  00000000654064F5: 74 69              je          0000000065406560
  00000000654064F7: 80 7B 10 00        cmp         byte ptr [rbx+10h],0
  00000000654064FB: 75 63              jne         0000000065406560
  00000000654064FD: 40 84 ED           test        bpl,bpl
  0000000065406500: 74 2D              je          000000006540652F
  0000000065406502: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  0000000065406506: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006540650A: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006540650D: C1 EA 0F           shr         edx,0Fh
  0000000065406510: F6 C2 01           test        dl,1
  0000000065406513: 74 38              je          000000006540654D
  0000000065406515: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065406518: 33 D2              xor         edx,edx
  000000006540651A: FF 50 58           call        qword ptr [rax+58h]
  000000006540651D: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  0000000065406521: 45 33 C0           xor         r8d,r8d
  0000000065406524: 41 8D 50 45        lea         edx,[r8+45h]
  0000000065406528: E8 B3 2A C6 FF     call        0000000065068FE0
  000000006540652D: EB 1E              jmp         000000006540654D
  000000006540652F: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  0000000065406533: BA 45 00 00 00     mov         edx,45h
  0000000065406538: E8 D3 7A C5 FF     call        000000006505E010
  000000006540653D: 84 C0              test        al,al
  000000006540653F: 75 0C              jne         000000006540654D
  0000000065406541: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  0000000065406545: B2 01              mov         dl,1
  0000000065406547: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006540654A: FF 50 58           call        qword ptr [rax+58h]
  000000006540654D: 49 8D 8D E0 01 00  lea         rcx,[r13+00000000000001E0h]
                    00
  0000000065406554: FF C7              inc         edi
  0000000065406556: FF 15 5C 03 29 00  call        qword ptr [00000000656968B8h]
  000000006540655C: 3B F8              cmp         edi,eax
  000000006540655E: 7C 80              jl          00000000654064E0
  0000000065406560: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  0000000065406565: 49 8D 95 E8 01 00  lea         rdx,[r13+00000000000001E8h]
                    00
  000000006540656C: 49 8B CC           mov         rcx,r12
  000000006540656F: E8 AC 08 1D 00     call        00000000655D6E20
  0000000065406574: E8 E7 2E 09 00     call        0000000065499460
  0000000065406579: 4C 8B 74 24 30     mov         r14,qword ptr [rsp+30h]
  000000006540657E: 48 8B 7C 24 68     mov         rdi,qword ptr [rsp+68h]
  0000000065406583: 48 8B 74 24 60     mov         rsi,qword ptr [rsp+60h]
  0000000065406588: 48 8B 6C 24 58     mov         rbp,qword ptr [rsp+58h]
  000000006540658D: 84 C0              test        al,al
  000000006540658F: 74 10              je          00000000654065A1
  0000000065406591: 33 D2              xor         edx,edx
  0000000065406593: 41 B8 0C 80 00 00  mov         r8d,800Ch
  0000000065406599: 49 8B CC           mov         rcx,r12
  000000006540659C: E8 BF C3 09 00     call        00000000654A2960
  00000000654065A1: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000654065A6: 49 8B CD           mov         rcx,r13
  00000000654065A9: E8 C2 F8 FF FF     call        0000000065405E70
  00000000654065AE: 49 8B CC           mov         rcx,r12
  00000000654065B1: 48 8B D0           mov         rdx,rax
  00000000654065B4: E8 77 EB C5 FF     call        0000000065065130
  00000000654065B9: 48 83 C4 38        add         rsp,38h
  00000000654065BD: 41 5D              pop         r13
  00000000654065BF: 41

  Summary

      695000 .text
