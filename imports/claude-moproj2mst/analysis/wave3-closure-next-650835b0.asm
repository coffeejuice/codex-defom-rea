Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650835B0: 48 8B C4           mov         rax,rsp
  00000000650835B3: 57                 push        rdi
  00000000650835B4: 48 81 EC 00 01 00  sub         rsp,100h
                    00
  00000000650835BB: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000000650835C7: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  00000000650835CB: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  00000000650835CF: 48 8B 05 3A 60 94  mov         rax,qword ptr [00000000659C9610h]
                    00
  00000000650835D6: 48 33 C4           xor         rax,rsp
  00000000650835D9: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  00000000650835E1: 48 8B F1           mov         rsi,rcx
  00000000650835E4: 48 8B 81 88 00 00  mov         rax,qword ptr [rcx+0000000000000088h]
                    00
  00000000650835EB: 48 63 10           movsxd      rdx,dword ptr [rax]
  00000000650835EE: 4C 8B 91 90 00 00  mov         r10,qword ptr [rcx+0000000000000090h]
                    00
  00000000650835F5: 33 FF              xor         edi,edi
  00000000650835F7: 44 8B C7           mov         r8d,edi
  00000000650835FA: 85 D2              test        edx,edx
  00000000650835FC: 41 0F 95 C0        setne       r8b
  0000000065083600: 41 B9 01 00 00 00  mov         r9d,1
  0000000065083606: 49 63 C0           movsxd      rax,r8d
  0000000065083609: 4C 8B DA           mov         r11,rdx
  000000006508360C: 4C 3B CA           cmp         r9,rdx
  000000006508360F: 7D 30              jge         0000000065083641
  0000000065083611: 49 8D 04 C2        lea         rax,[r10+rax*8]
  0000000065083615: 4B 8B 0C CA        mov         rcx,qword ptr [r10+r9*8]
  0000000065083619: 48 85 C9           test        rcx,rcx
  000000006508361C: 74 11              je          000000006508362F
  000000006508361E: 80 39 2D           cmp         byte ptr [rcx],2Dh
  0000000065083621: 74 0C              je          000000006508362F
  0000000065083623: 48 89 08           mov         qword ptr [rax],rcx
  0000000065083626: 41 FF C0           inc         r8d
  0000000065083629: 48 83 C0 08        add         rax,8
  000000006508362D: EB 0A              jmp         0000000065083639
  000000006508362F: 48 89 08           mov         qword ptr [rax],rcx
  0000000065083632: 41 FF C0           inc         r8d
  0000000065083635: 48 83 C0 08        add         rax,8
  0000000065083639: 49 FF C1           inc         r9
  000000006508363C: 4C 3B CA           cmp         r9,rdx
  000000006508363F: 7C D4              jl          0000000065083615
  0000000065083641: 48 8B 86 88 00 00  mov         rax,qword ptr [rsi+0000000000000088h]
                    00
  0000000065083648: 44 3B 00           cmp         r8d,dword ptr [rax]
  000000006508364B: 7D 18              jge         0000000065083665
  000000006508364D: 49 63 C8           movsxd      rcx,r8d
  0000000065083650: 48 8B 86 90 00 00  mov         rax,qword ptr [rsi+0000000000000090h]
                    00
  0000000065083657: 48 89 3C C8        mov         qword ptr [rax+rcx*8],rdi
  000000006508365B: 48 8B 86 88 00 00  mov         rax,qword ptr [rsi+0000000000000088h]
                    00
  0000000065083662: 44 89 00           mov         dword ptr [rax],r8d
  0000000065083665: 33 C9              xor         ecx,ecx
  0000000065083667: FF 15 4B 2C 61 00  call        qword ptr [00000000656962B8h]
  000000006508366D: 0D 01 80 00 00     or          eax,8001h
  0000000065083672: 8B C8              mov         ecx,eax
  0000000065083674: FF 15 3E 2C 61 00  call        qword ptr [00000000656962B8h]
  000000006508367A: 33 C9              xor         ecx,ecx
  000000006508367C: FF 15 F6 64 61 00  call        qword ptr [0000000065699B78h]
  0000000065083682: 85 C0              test        eax,eax
  0000000065083684: 74 14              je          000000006508369A
  0000000065083686: 83 F8 01           cmp         eax,1
  0000000065083689: 74 0F              je          000000006508369A
  000000006508368B: 8B D0              mov         edx,eax
  000000006508368D: 48 8D 0D 2C AB 62  lea         rcx,[00000000656AE1C0h]
                    00
  0000000065083694: FF 15 FE 32 61 00  call        qword ptr [0000000065696998h]
  000000006508369A: E8 D1 05 FF FF     call        0000000065073C70
  000000006508369F: E8 BC 04 16 00     call        00000000651E3B60
  00000000650836A4: E8 07 BC 01 00     call        000000006509F2B0
  00000000650836A9: E8 92 35 F9 FF     call        0000000065016C40
  00000000650836AE: 85 C0              test        eax,eax
  00000000650836B0: 74 05              je          00000000650836B7
  00000000650836B2: E8 E9 B7 FA FF     call        000000006502EEA0
  00000000650836B7: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000650836BA: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000000650836BF: 48 8B CE           mov         rcx,rsi
  00000000650836C2: FF 50 08           call        qword ptr [rax+8]
  00000000650836C5: 48 8B D8           mov         rbx,rax
  00000000650836C8: FF 15 52 31 61 00  call        qword ptr [0000000065696820h]
  00000000650836CE: 48 8B C8           mov         rcx,rax
  00000000650836D1: 48 8B D3           mov         rdx,rbx
  00000000650836D4: FF 15 5E 33 61 00  call        qword ptr [0000000065696A38h]
  00000000650836DA: 90                 nop
  00000000650836DB: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000650836E0: FF 15 7A 30 61 00  call        qword ptr [0000000065696760h]
  00000000650836E6: B9 11 00 00 00     mov         ecx,11h
  00000000650836EB: FF 15 4F 2B 61 00  call        qword ptr [0000000065696240h]
  00000000650836F1: 4C 8D 84 24 90 00  lea         r8,[rsp+0000000000000090h]
                    00 00
  00000000650836F9: BA 5C 00 00 00     mov         edx,5Ch
  00000000650836FE: 48 8B C8           mov         rcx,rax
  0000000065083701: FF 15 49 2B 61 00  call        qword ptr [0000000065696250h]
  0000000065083707: 41 B0 01           mov         r8b,1
  000000006508370A: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000065083712: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000065083717: E8 84 E5 21 00     call        00000000652A1CA0
  000000006508371C: 90                 nop
  000000006508371D: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000065083722: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000065083727: E8 64 EC 16 00     call        00000000651F2390
  000000006508372C: 48 8B D8           mov         rbx,rax
  000000006508372F: 48 8D 15 7A AA 62  lea         rdx,[00000000656AE1B0h]
                    00
  0000000065083736: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006508373B: FF 15 9F 32 61 00  call        qword ptr [00000000656969E0h]
  0000000065083741: 48 8B D0           mov         rdx,rax
  0000000065083744: 48 8B CB           mov         rcx,rbx
  0000000065083747: FF 15 B3 33 61 00  call        qword ptr [0000000065696B00h]
  000000006508374D: 0F B6 D8           movzx       ebx,al
  0000000065083750: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000065083755: FF 15 05 30 61 00  call        qword ptr [0000000065696760h]
  000000006508375B: 84 DB              test        bl,bl
  000000006508375D: 74 3C              je          000000006508379B
  000000006508375F: 48 8D 15 3A AA 62  lea         rdx,[00000000656AE1A0h]
                    00
  0000000065083766: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006508376B: FF 15 6F 32 61 00  call        qword ptr [00000000656969E0h]
  0000000065083771: 48 8B D0           mov         rdx,rax
  0000000065083774: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065083779: FF 15 89 34 61 00  call        qword ptr [0000000065696C08h]
  000000006508377F: 90                 nop
  0000000065083780: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000065083785: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006508378A: E8 51 FE 16 00     call        00000000651F35E0
  000000006508378F: 90                 nop
  0000000065083790: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065083795: FF 15 C5 2F 61 00  call        qword ptr [0000000065696760h]
  000000006508379B: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000000650837A0: E8 CB E2 F9 FF     call        0000000065021A70
  00000000650837A5: E8 C6 39 F9 FF     call        0000000065017170
  00000000650837AA: 84 C0              test        al,al
  00000000650837AC: 74 18              je          00000000650837C6
  00000000650837AE: E8 8D 34 F9 FF     call        0000000065016C40
  00000000650837B3: 85 C0              test        eax,eax
  00000000650837B5: 74 05              je          00000000650837BC
  00000000650837B7: E8 44 29 FA FF     call        0000000065026100
  00000000650837BC: E8 4F BF FF FF     call        000000006507F710
  00000000650837C1: E8

  Summary

      695000 .text
