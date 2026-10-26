
Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067029600: 40 57              push        rdi
  0000000067029602: 48 83 EC 30        sub         rsp,30h
  0000000067029606: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006702960F: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  0000000067029614: 83 3D CD 72 2C 00  cmp         dword ptr [00000000672F08E8h],0FFFFFFFFh
                    FF
  000000006702961B: 0F 85 9F 00 00 00  jne         00000000670296C0
  0000000067029621: 8B 05 01 A7 2C 00  mov         eax,dword ptr [00000000672F3D28h]
  0000000067029627: 48 8D 1D F2 A6 2C  lea         rbx,[00000000672F3D20h]
                    00
  000000006702962E: A8 01              test        al,1
  0000000067029630: 75 20              jne         0000000067029652
  0000000067029632: 83 C8 01           or          eax,1
  0000000067029635: 89 05 ED A6 2C 00  mov         dword ptr [00000000672F3D28h],eax
  000000006702963B: 33 D2              xor         edx,edx
  000000006702963D: 48 8B CB           mov         rcx,rbx
  0000000067029640: E8 7B C3 FF FF     call        00000000670259C0
  0000000067029645: 48 8D 0D A4 CB 17  lea         rcx,[00000000671A61F0h]
                    00
  000000006702964C: E8 03 F5 14 00     call        0000000067178B54
  0000000067029651: 90                 nop
  0000000067029652: 48 8B 0D C7 A6 2C  mov         rcx,qword ptr [00000000672F3D20h]
                    00
  0000000067029659: BF 01 00 00 00     mov         edi,1
  000000006702965E: 40 84 79 04        test        byte ptr [rcx+4],dil
  0000000067029662: 74 0A              je          000000006702966E
  0000000067029664: 48 8B CB           mov         rcx,rbx
  0000000067029667: E8 44 C7 FF FF     call        0000000067025DB0
  000000006702966C: EB 15              jmp         0000000067029683
  000000006702966E: 33 C0              xor         eax,eax
  0000000067029670: F0 0F B1 39        lock cmpxchg dword ptr [rcx],edi
  0000000067029674: 0F 94 C1           sete        cl
  0000000067029677: 84 C9              test        cl,cl
  0000000067029679: 75 08              jne         0000000067029683
  000000006702967B: 48 8B CB           mov         rcx,rbx
  000000006702967E: E8 2D C5 FF FF     call        0000000067025BB0
  0000000067029683: 48 0B DF           or          rbx,rdi
  0000000067029686: FF 15 EC EC 17 00  call        qword ptr [00000000671A8378h]
  000000006702968C: 89 05 56 72 2C 00  mov         dword ptr [00000000672F08E8h],eax
  0000000067029692: 40 84 DF           test        dil,bl
  0000000067029695: 74 29              je          00000000670296C0
  0000000067029697: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006702969B: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006702969E: 40 84 79 04        test        byte ptr [rcx+4],dil
  00000000670296A2: 74 0A              je          00000000670296AE
  00000000670296A4: 48 8B CB           mov         rcx,rbx
  00000000670296A7: E8 84 C4 FF FF     call        0000000067025B30
  00000000670296AC: EB 12              jmp         00000000670296C0
  00000000670296AE: 33 D2              xor         edx,edx
  00000000670296B0: 8B C7              mov         eax,edi
  00000000670296B2: F0 0F B1 11        lock cmpxchg dword ptr [rcx],edx
  00000000670296B6: 74 08              je          00000000670296C0
  00000000670296B8: 48 8B CB           mov         rcx,rbx
  00000000670296BB: E8 B0 C4 FF FF     call        0000000067025B70
  00000000670296C0: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000670296C5: 48 83 C4 30        add         rsp,30h
  00000000670296C9: 5F                 pop         rdi
  00000000670296CA: C3                 ret

  Summary

      1A7000 .text
