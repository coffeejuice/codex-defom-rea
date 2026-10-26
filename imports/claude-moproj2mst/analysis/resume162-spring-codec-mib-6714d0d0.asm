Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006714D0D0: 89 4C 24 08        mov         dword ptr [rsp+8],ecx
  000000006714D0D4: 53                 push        rbx
  000000006714D0D5: 55                 push        rbp
  000000006714D0D6: 56                 push        rsi
  000000006714D0D7: 57                 push        rdi
  000000006714D0D8: 48 83 EC 78        sub         rsp,78h
  000000006714D0DC: 48 C7 44 24 50 FE  mov         qword ptr [rsp+50h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006714D0E5: 8B F9              mov         edi,ecx
  000000006714D0E7: E8 54 EA FF FF     call        000000006714BB40
  000000006714D0EC: 48 8B D8           mov         rbx,rax
  000000006714D0EF: 33 C0              xor         eax,eax
  000000006714D0F1: 8D 48 01           lea         ecx,[rax+1]
  000000006714D0F4: 48 85 DB           test        rbx,rbx
  000000006714D0F7: 74 30              je          000000006714D129
  000000006714D0F9: 48 8B 13           mov         rdx,qword ptr [rbx]
  000000006714D0FC: 84 4A 04           test        byte ptr [rdx+4],cl
  000000006714D0FF: 74 0A              je          000000006714D10B
  000000006714D101: 48 8B CB           mov         rcx,rbx
  000000006714D104: E8 A7 8C ED FF     call        0000000067025DB0
  000000006714D109: EB 13              jmp         000000006714D11E
  000000006714D10B: F0 0F B1 0A        lock cmpxchg dword ptr [rdx],ecx
  000000006714D10F: 0F 94 C1           sete        cl
  000000006714D112: 84 C9              test        cl,cl
  000000006714D114: 75 08              jne         000000006714D11E
  000000006714D116: 48 8B CB           mov         rcx,rbx
  000000006714D119: E8 92 8A ED FF     call        0000000067025BB0
  000000006714D11E: 48 83 CB 01        or          rbx,1
  000000006714D122: 48 89 5C 24 28     mov         qword ptr [rsp+28h],rbx
  000000006714D127: EB 05              jmp         000000006714D12E
  000000006714D129: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000006714D12E: E8 0D F7 FF FF     call        000000006714C840
  000000006714D133: 41 B8 0A 00 00 00  mov         r8d,0Ah
  000000006714D139: 8B D7              mov         edx,edi
  000000006714D13B: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006714D140: E8 DB 2D EE FF     call        000000006702FF20
  000000006714D145: 90                 nop
  000000006714D146: 48 8D 0D 23 88 0F  lea         rcx,[0000000067245970h]
                    00
  000000006714D14D: 48 89 4C 24 58     mov         qword ptr [rsp+58h],rcx
  000000006714D152: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000006714D157: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006714D15C: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006714D161: E8 4A EC FF FF     call        000000006714BDB0
  000000006714D166: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  000000006714D16B: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  000000006714D170: F0 41 83 03 01     lock add    dword ptr [r11],1
  000000006714D175: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000006714D17A: 83 CD FF           or          ebp,0FFFFFFFFh
  000000006714D17D: F0 01 29           lock add    dword ptr [rcx],ebp
  000000006714D180: 75 0B              jne         000000006714D18D
  000000006714D182: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000006714D187: E8 24 32 ED FF     call        00000000670203B0
  000000006714D18C: 90                 nop
  000000006714D18D: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000006714D192: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006714D196: 75 0A              jne         000000006714D1A2
  000000006714D198: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006714D19D: E8 0E 32 ED FF     call        00000000670203B0
  000000006714D1A2: E8 49 F3 FF FF     call        000000006714C4F0
  000000006714D1A7: 48 8B F0           mov         rsi,rax
  000000006714D1AA: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000006714D1AF: 48 85 C0           test        rax,rax
  000000006714D1B2: 0F 84 F7 00 00 00  je          000000006714D2AF
  000000006714D1B8: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006714D1BB: 83 79 14 00        cmp         dword ptr [rcx+14h],0
  000000006714D1BF: 0F 84 EA 00 00 00  je          000000006714D2AF
  000000006714D1C5: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006714D1CA: E8 81 A7 EF FF     call        0000000067047950
  000000006714D1CF: 44 8B C8           mov         r9d,eax
  000000006714D1D2: 89 84 24 A8 00 00  mov         dword ptr [rsp+00000000000000A8h],eax
                    00
  000000006714D1D9: 48 8B 2E           mov         rbp,qword ptr [rsi]
  000000006714D1DC: 8B 4D 20           mov         ecx,dword ptr [rbp+20h]
  000000006714D1DF: 48 8B 5C 24 20     mov         rbx,qword ptr [rsp+20h]
  000000006714D1E4: 85 C9              test        ecx,ecx
  000000006714D1E6: 74 52              je          000000006714D23A
  000000006714D1E8: 33 D2              xor         edx,edx
  000000006714D1EA: F7 F1              div         eax,ecx
  000000006714D1EC: 48 8B 4D 08        mov         rcx,qword ptr [rbp+8]
  000000006714D1F0: 48 8D 34 D1        lea         rsi,[rcx+rdx*8]
  000000006714D1F4: 48 8B 3E           mov         rdi,qword ptr [rsi]
  000000006714D1F7: 48 3B FD           cmp         rdi,rbp
  000000006714D1FA: 74 3E              je          000000006714D23A
  000000006714D1FC: 0F 1F 40 00        nop         dword ptr [rax]
  000000006714D200: 44 3B 4F 08        cmp         r9d,dword ptr [rdi+8]
  000000006714D204: 75 29              jne         000000006714D22F
  000000006714D206: 48 63 43 08        movsxd      rax,dword ptr [rbx+8]
  000000006714D20A: 48 8B 57 10        mov         rdx,qword ptr [rdi+10h]
  000000006714D20E: 3B 42 08           cmp         eax,dword ptr [rdx+8]
  000000006714D211: 75 1C              jne         000000006714D22F
  000000006714D213: 4C 8B C0           mov         r8,rax
  000000006714D216: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  000000006714D21A: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  000000006714D21E: E8 49 B9 02 00     call        0000000067178B6C
  000000006714D223: 85 C0              test        eax,eax
  000000006714D225: 74 13              je          000000006714D23A
  000000006714D227: 44 8B 8C 24 A8 00  mov         r9d,dword ptr [rsp+00000000000000A8h]
                    00 00
  000000006714D22F: 48 8B F7           mov         rsi,rdi
  000000006714D232: 48 8B 3F           mov         rdi,qword ptr [rdi]
  000000006714D235: 48 3B FD           cmp         rdi,rbp
  000000006714D238: 75 C6              jne         000000006714D200
  000000006714D23A: 48 8B 3E           mov         rdi,qword ptr [rsi]
  000000006714D23D: 48 3B FD           cmp         rdi,rbp
  000000006714D240: 74 5B              je          000000006714D29D
  000000006714D242: 48 8B 7F 18        mov         rdi,qword ptr [rdi+18h]
  000000006714D246: 48 85 FF           test        rdi,rdi
  000000006714D249: 74 5C              je          000000006714D2A7
  000000006714D24B: F0 83 03 FF        lock add    dword ptr [rbx],0FFFFFFFFh
  000000006714D24F: 75 0B              jne         000000006714D25C
  000000006714D251: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006714D256: E8 55 31 ED FF     call        00000000670203B0
  000000006714D25B: 90                 nop
  000000006714D25C: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000006714D261: F6 C1 01           test        cl,1
  000000006714D264: 74 2F              je          000000006714D295
  000000006714D266: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006714D26A: 48 89 4C 24 28     mov         qword ptr [rsp+28h],rcx
  000000006714D26F: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006714D273: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006714D276: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006714D27A: 74 07              je          000000006714D283
  000000006714D27C: E8 AF 88 ED FF     call        0000000067025B30
  000000006714D281: EB 12              jmp         000000006714D295
  000000006714D283: B8 01 00 00 00     mov         eax,1
  000000006714D288: 33 ED              xor         ebp,ebp
  000000006714D28A: F0 0F B1 2A        lock cmpxchg dword ptr [rdx],ebp
  000000006714D28E: 74 05              je          000000006714D295
  000000006714D290: E8 DB 88 ED FF     call        0000000067025B70
  000000006714D295: 48 8B C7           mov         rax,rdi
  000000006714D298: E9 41 05 00 00     jmp         000000006714D7DE
  000000006714D29D: 48 8B 74 24 30     mov         rsi,qword ptr [rsp+30h]
  000000006714D2A2: 83 CD FF           or          ebp,0FFFFFFFFh
  000000006714D2A5: EB 08              jmp         000000006714D2AF
  000000006714D2A7: 48 8B 74 24 30     mov         rsi,qword ptr [rsp+30h]
  000000006714D2AC: 83 CD FF           or          ebp,0FFFFFFFFh
  000000006714D2AF: 45 33 C0           xor         r8d,r8d
  000000006714D2B2: 41 8B D8           mov         ebx,r8d
  000000006714D2B5: 48 8B 05 74 71 1A  mov         rax,qword ptr [00000000672F4430h]
                    00
  000000006714D2BC: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006714D2BF: 8B 51 08           mov         edx,dword ptr [rcx+8]
  000000006714D2C2: 49 8B F8           mov         rdi,r8
  000000006714D2C5: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  000000006714D2C8: 2B C2              sub         eax,edx
  000000006714D2CA: 85 C0              test        eax,eax
  000000006714D2CC: 0F 8E 63 02 00 00  jle         000000006714D535
  000000006714D2D2: 66 66 66 66 66 66  nop         word ptr [rax+rax+0000000000000000h]
                    0F 1F 84 00 00 00
                    00 00
  000000006714D2E0: 48 63 C2           movsxd      rax,edx
  000000006714D2E3: 48 03 C7           add         rax,rdi
  000000006714D2E6: 48 8B 4C C1 18     mov         rcx,qword ptr [rcx+rax*8+18h]
  000000006714D2EB: 48 89 4C 24 38     mov         qword ptr [rsp+38h],rcx
  000000006714D2F0: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006714D2F3: FF 50 10           call        qword ptr [rax+10h]
  000000006714D2F6: 3B 84 24 A0 00 00  cmp         eax,dword ptr [rsp+00000000000000A0h]
                    00
  000000006714D2FD: 74 20              je          000000006714D31F
  000000006714D2FF: FF C3              inc         ebx
  000000006714D301: 48 FF C7           inc         rdi
  000000006714D304: 48 8B 05 25 71 1A  mov         rax,qword ptr [00000000672F4430h]
                    00
  000000006714D30B: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006714D30E: 8B 51 08           mov         edx,dword ptr [rcx+8]
  000000006714D311: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  000000006714D314: 2B C2              sub         eax,edx
  000000006714D316: 3B D8              cmp         ebx,eax
  000000006714D318: 7C C6              jl          000000006714D2E0
  000000006714D31A: E9 16 02 00 00     jmp         000000006714D535
  000000006714D31F: 48 85 F6           test        rsi,rsi
  000000006714D322: 0F 84 B1 01 00 00  je          000000006714D4D9
  000000006714D328: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006714D32B: 8B 40 10           mov         eax,dword ptr [rax+10h]
  000000006714D32E: 83 F8 01           cmp         eax,1
  000000006714D331: 74 08              je          000000006714D33B
  000000006714D333: 48 8B CE           mov         rcx,rsi
  000000006714D336: E8 35 D4 F0 FF     call        000000006705A770
  000000006714D33B: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006714D340: E8 0B A6 EF FF     call        0000000067047950
  000000006714D345: 44 8B C8           mov         r9d,eax
  000000006714D348: 89 84 24 A8 00 00  mov         dword ptr [rsp+00000000000000A8h],eax
                    00
  000000006714D34F: 48 8B 2E           mov         rbp,qword ptr [rsi]
  000000006714D352: 8B 4D 20           mov         ecx,dword ptr [rbp+20h]
  000000006714D355: 85 C9              test        ecx,ecx
  000000006714D357: 74 53              je          000000006714D3AC
  000000006714D359: 33 D2              xor         edx,edx
  000000006714D35B: F7 F1              div         eax,ecx
  000000006714D35D: 48 8B 4D 08        mov         rcx,qword ptr [rbp+8]
  000000006714D361: 48 8D 34 D1        lea         rsi,[rcx+rdx*8]
  000000006714D365: 48 8B 3E           mov         rdi,qword ptr [rsi]
  000000006714D368: 48 3B FD           cmp         rdi,rbp
  000000006714D36B: 74 3F              je          000000006714D3AC
  000000006714D36D: 48 8B 5C 24 20     mov         rbx,qword ptr [rsp+20h]
  000000006714D372: 44 3B 4F 08        cmp         r9d,dword ptr [rdi+8]
  000000006714D376: 75 29              jne         000000006714D3A1
  000000006714D378: 48 63 43 08        movsxd      rax,dword ptr [rbx+8]
  000000006714D37C: 48 8B 57 10        mov         rdx,qword ptr [rdi+10h]
  000000006714D380: 3B 42 08           cmp         eax,dword ptr [rdx+8]
  000000006714D383: 75 1C              jne         000000006714D3A1
  000000006714D385: 4C 8B C0           mov         r8,rax
  000000006714D388: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  000000006714D38C: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  000000006714D390: E8 D7 B7 02 00     call        0000000067178B6C
  000000006714D395: 85 C0              test        eax,eax
  000000006714D397: 74 13              je          000000006714D3AC
  000000006714D399: 44 8B 8C 24 A8 00  mov         r9d,dword ptr [rsp+00000000000000A8h]
                    00 00
  000000006714D3A1: 48 8B F7           mov         rsi,rdi
  000000006714D3A4: 48 8B 3F           mov         rdi,qword ptr [rdi]
  000000006714D3A7: 48 3B FD           cmp         rdi,rbp
  000000006714D3AA: 75 C6              jne         000000006714D372
  000000006714D3AC: 8B 9C 24 A8 00 00  mov         ebx,dword ptr [rsp+00000000000000A8h]
                    00
  000000006714D3B3: 89 9C 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],ebx
                    00
  000000006714D3BA: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006714D3BD: 48 3B C5           cmp         rax,rbp
  000000006714D3C0: 0F 85 06 01 00 00  jne         000000006714D4CC
  000000006714D3C6: 8B 45 20           mov         eax,dword ptr [rbp+20h]
  000000006714D3C9: 39 45 14           cmp         dword ptr [rbp+14h],eax
  000000006714D3CC: 0F 8C 9B 00 00 00  jl          000000006714D46D
  000000006714D3D2: 0F BF 55 1E        movsx       edx,word ptr [rbp+1Eh]
  000000006714D3D6: FF C2              inc         edx
  000000006714D3D8: 48 8B CD           mov         rcx,rbp
  000000006714D3DB: E8 80 A3 EF FF     call        0000000067047760
  000000006714D3E0: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006714D3E5: E8 66 A5 EF FF     call        0000000067047950
  000000006714D3EA: 44 8B C0           mov         r8d,eax
  000000006714D3ED: 89 84 24 A8 00 00  mov         dword ptr [rsp+00000000000000A8h],eax
                    00
  000000006714D3F4: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000006714D3F9: 48 8B 28           mov         rbp,qword ptr [rax]
  000000006714D3FC: 8B 4D 20           mov         ecx,dword ptr [rbp+20h]
  000000006714D3FF: 85 C9              test        ecx,ecx
  000000006714D401: 74 59              je          000000006714D45C
  000000006714D403: 33 D2              xor         edx,edx
  000000006714D405: 41 8B C0           mov         eax,r8d
  000000006714D408: F7 F1              div         eax,ecx
  000000006714D40A: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  000000006714D40E: 48 8D 34 D0        lea         rsi,[rax+rdx*8]
  000000006714D412: 48 8B 3E           mov         rdi,qword ptr [rsi]
  000000006714D415: 48 3B FD           cmp         rdi,rbp
  000000006714D418: 74 45              je          000000006714D45F
  000000006714D41A: 48 8B 5C 24 20     mov         rbx,qword ptr [rsp+20h]
  000000006714D41F: 90                 nop
  000000006714D420: 44 3B 47 08        cmp         r8d,dword ptr [rdi+8]
  000000006714D424: 75 29              jne         000000006714D44F
  000000006714D426: 48 63 43 08        movsxd      rax,dword ptr [rbx+8]
  000000006714D42A: 48 8B 57 10        mov         rdx,qword ptr [rdi+10h]
  000000006714D42E: 3B 42 08           cmp         eax,dword ptr [rdx+8]
  000000006714D431: 75 1C              jne         000000006714D44F
  000000006714D433: 4C 8B C0           mov         r8,rax
  000000006714D436: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  000000006714D43A: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  000000006714D43E: E8 29 B7 02 00     call        0000000067178B6C
  000000006714D443: 85 C0              test        eax,eax
  000000006714D445: 74 18              je          000000006714D45F
  000000006714D447: 44 8B 84 24 A8 00  mov         r8d,dword ptr [rsp+00000000000000A8h]
                    00 00
  000000006714D44F: 48 8B F7           mov         rsi,rdi
  000000006714D452: 48 8B 3F           mov         rdi,qword ptr [rdi]
  000000006714D455: 48 3B FD           cmp         rdi,rbp
  000000006714D458: 74 05              je          000000006714D45F
  000000006714D45A: EB C4              jmp         000000006714D420
  000000006714D45C: 48 8B F0           mov         rsi,rax
  000000006714D45F: 8B 9C 24 A8 00 00  mov         ebx,dword ptr [rsp+00000000000000A8h]
                    00
  000000006714D466: 89 9C 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],ebx
                    00
  000000006714D46D: BA 08 00 00 00     mov         edx,8
  000000006714D472: 48 8B CD           mov         rcx,rbp
  000000006714D475: E8 D6 A1 EF FF     call        0000000067047650
  000000006714D47A: 4C 8B D8           mov         r11,rax
  000000006714D47D: 48 85 C0           test        rax,rax
  000000006714D480: 74 26              je          000000006714D4A8
  000000006714D482: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006714D487: 48 89 48 10        mov         qword ptr [rax+10h],rcx
  000000006714D48B: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006714D490: F0 83 01 01        lock add    dword ptr [rcx],1
  000000006714D494: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  000000006714D499: 48 89 78 18        mov         qword ptr [rax+18h],rdi
  000000006714D49D: 8B 9C 24 B0 00 00  mov         ebx,dword ptr [rsp+00000000000000B0h]
                    00
  000000006714D4A4: 33 ED              xor         ebp,ebp
  000000006714D4A6: EB 0A              jmp         000000006714D4B2
  000000006714D4A8: 33 ED              xor         ebp,ebp
  000000006714D4AA: 4C 8B DD           mov         r11,rbp
  000000006714D4AD: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  000000006714D4B2: 41 89 5B 08        mov         dword ptr [r11+8],ebx
  000000006714D4B6: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006714D4B9: 49 89 03           mov         qword ptr [r11],rax
  000000006714D4BC: 4C 89 1E           mov         qword ptr [rsi],r11
  000000006714D4BF: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000006714D4C4: 48 8B 00           mov         rax,qword ptr [rax]
  000000006714D4C7: FF 40 14           inc         dword ptr [rax+14h]
  000000006714D4CA: EB 14              jmp         000000006714D4E0
  000000006714D4CC: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  000000006714D4D1: 48 89 78 18        mov         qword ptr [rax+18h],rdi
  000000006714D4D5: 33 ED              xor         ebp,ebp
  000000006714D4D7: EB 07              jmp         000000006714D4E0
  000000006714D4D9: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  000000006714D4DE: 33 ED              xor         ebp,ebp
  000000006714D4E0: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000006714D4E5: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006714D4E9: 75 0B              jne         000000006714D4F6
  000000006714D4EB: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006714D4F0: E8 BB 2E ED FF     call        00000000670203B0
  000000006714D4F5: 90                 nop
  000000006714D4F6: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000006714D4FB: F6 C1 01           test        cl,1
  000000006714D4FE: 74 2D              je          000000006714D52D
  000000006714D500: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006714D504: 48 89 4C 24 28     mov         qword ptr [rsp+28h],rcx
  000000006714D509: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006714D50D: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006714D510: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006714D514: 74 07              je          000000006714D51D
  000000006714D516: E8 15 86 ED FF     call        0000000067025B30
  000000006714D51B: EB 10              jmp         000000006714D52D
  000000006714D51D: B8 01 00 00 00     mov         eax,1
  000000006714D522: F0 0F B1 2A        lock cmpxchg dword ptr [rdx],ebp
  000000006714D526: 74 05              je          000000006714D52D
  000000006714D528: E8 43 86 ED FF     call        0000000067025B70
  000000006714D52D: 48 8B C7           mov         rax,rdi
  000000006714D530: E9 A9 02 00 00     jmp         000000006714D7DE
  000000006714D535: 8B 9C 24 A0 00 00  mov         ebx,dword ptr [rsp+00000000000000A0h]
                    00
  000000006714D53C: 8B CB              mov         ecx,ebx
  000000006714D53E: E8 AD EA FF FF     call        000000006714BFF0
  000000006714D543: 48 8B F8           mov         rdi,rax
  000000006714D546: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000006714D54B: 48 85 C0           test        rax,rax
  000000006714D54E: 75 71              jne         000000006714D5C1
  000000006714D550: 81 FB E8 03 00 00  cmp         ebx,3E8h
  000000006714D556: 0F 85 2C 02 00 00  jne         000000006714D788
  000000006714D55C: 8D 4B 0F           lea         ecx,[rbx+0Fh]
  000000006714D55F: E8 6C FB FF FF     call        000000006714D0D0
  000000006714D564: 48 8B D8           mov         rbx,rax
  000000006714D567: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006714D56C: F0 0F C1 29        lock xadd   dword ptr [rcx],ebp
  000000006714D570: 83 C5 FF           add         ebp,0FFFFFFFFh
  000000006714D573: 75 0B              jne         000000006714D580
  000000006714D575: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006714D57A: E8 31 2E ED FF     call        00000000670203B0
  000000006714D57F: 90                 nop
  000000006714D580: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000006714D585: F6 C1 01           test        cl,1
  000000006714D588: 74 2F              je          000000006714D5B9
  000000006714D58A: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006714D58E: 48 89 4C 24 28     mov         qword ptr [rsp+28h],rcx
  000000006714D593: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006714D597: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006714D59A: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006714D59E: 74 07              je          000000006714D5A7
  000000006714D5A0: E8 8B 85 ED FF     call        0000000067025B30
  000000006714D5A5: EB 12              jmp         000000006714D5B9
  000000006714D5A7: B8 01 00 00 00     mov         eax,1
  000000006714D5AC: 33 ED              xor         ebp,ebp
  000000006714D5AE: F0 0F B1 2A        lock cmpxchg dword ptr [rdx],ebp
  000000006714D5B2: 74 05              je          000000006714D5B9
  000000006714D5B4: E8 B7 85 ED FF     call        0000000067025B70
  000000006714D5B9: 48 8B C3           mov         rax,rbx
  000000006714D5BC: E9 1D 02 00 00     jmp         000000006714D7DE
  000000006714D5C1: 48 85 F6           test        rsi,rsi
  000000006714D5C4: 0F 84 C2 01 00 00  je          000000006714D78C
  000000006714D5CA: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006714D5CD: 8B 40 10           mov         eax,dword ptr [rax+10h]
  000000006714D5D0: 83 F8 01           cmp         eax,1
  000000006714D5D3: 74 08              je          000000006714D5DD
  000000006714D5D5: 48 8B CE           mov         rcx,rsi
  000000006714D5D8: E8 93 D1 F0 FF     call        000000006705A770
  000000006714D5DD: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006714D5E2: E8 69 A3 EF FF     call        0000000067047950
  000000006714D5E7: 44 8B C8           mov         r9d,eax
  000000006714D5EA: 89 84 24 A8 00 00  mov         dword ptr [rsp+00000000000000A8h],eax
                    00
  000000006714D5F1: 48 8B 2E           mov         rbp,qword ptr [rsi]
  000000006714D5F4: 8B 4D 20           mov         ecx,dword ptr [rbp+20h]
  000000006714D5F7: 85 C9              test        ecx,ecx
  000000006714D5F9: 74 66              je          000000006714D661
  000000006714D5FB: 33 D2              xor         edx,edx
  000000006714D5FD: F7 F1              div         eax,ecx
  000000006714D5FF: 48 8B 4D 08        mov         rcx,qword ptr [rbp+8]
  000000006714D603: 48 8D 34 D1        lea         rsi,[rcx+rdx*8]
  000000006714D607: 48 8B 3E           mov         rdi,qword ptr [rsi]
  000000006714D60A: 48 3B FD           cmp         rdi,rbp
  000000006714D60D: 74 4D              je          000000006714D65C
  000000006714D60F: 48 8B 5C 24 20     mov         rbx,qword ptr [rsp+20h]
  000000006714D614: 44 3B 4F 08        cmp         r9d,dword ptr [rdi+8]
  000000006714D618: 75 29              jne         000000006714D643
  000000006714D61A: 48 63 43 08        movsxd      rax,dword ptr [rbx+8]
  000000006714D61E: 48 8B 57 10        mov         rdx,qword ptr [rdi+10h]
  000000006714D622: 3B 42 08           cmp         eax,dword ptr [rdx+8]
  000000006714D625: 75 1C              jne         000000006714D643
  000000006714D627: 4C 8B C0           mov         r8,rax
  000000006714D62A: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  000000006714D62E: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  000000006714D632: E8 35 B5 02 00     call        0000000067178B6C
  000000006714D637: 85 C0              test        eax,eax
  000000006714D639: 74 1A              je          000000006714D655
  000000006714D63B: 44 8B 8C 24 A8 00  mov         r9d,dword ptr [rsp+00000000000000A8h]
                    00 00
  000000006714D643: 48 8B F7           mov         rsi,rdi
  000000006714D646: 48 8B 3F           mov         rdi,qword ptr [rdi]
  000000006714D649: 48 3B FD           cmp         rdi,rbp
  000000006714D64C: 75 C6              jne         000000006714D614
  000000006714D64E: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  000000006714D653: EB 0C              jmp         000000006714D661
  000000006714D655: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  000000006714D65A: EB 05              jmp         000000006714D661
  000000006714D65C: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  000000006714D661: 8B 9C 24 A8 00 00  mov         ebx,dword ptr [rsp+00000000000000A8h]
                    00
  000000006714D668: 89 9C 24 B8 00 00  mov         dword ptr [rsp+00000000000000B8h],ebx
                    00
  000000006714D66F: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006714D672: 48 3B C5           cmp         rax,rbp
  000000006714D675: 0F 85 05 01 00 00  jne         000000006714D780
  000000006714D67B: 8B 45 20           mov         eax,dword ptr [rbp+20h]
  000000006714D67E: 39 45 14           cmp         dword ptr [rbp+14h],eax
  000000006714D681: 0F 8C 9A 00 00 00  jl          000000006714D721
  000000006714D687: 0F BF 55 1E        movsx       edx,word ptr [rbp+1Eh]
  000000006714D68B: FF C2              inc         edx
  000000006714D68D: 48 8B CD           mov         rcx,rbp
  000000006714D690: E8 CB A0 EF FF     call        0000000067047760
  000000006714D695: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006714D69A: E8 B1 A2 EF FF     call        0000000067047950
  000000006714D69F: 44 8B C0           mov         r8d,eax
  000000006714D6A2: 89 84 24 A8 00 00  mov         dword ptr [rsp+00000000000000A8h],eax
                    00
  000000006714D6A9: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000006714D6AE: 48 8B 28           mov         rbp,qword ptr [rax]
  000000006714D6B1: 8B 4D 20           mov         ecx,dword ptr [rbp+20h]
  000000006714D6B4: 85 C9              test        ecx,ecx
  000000006714D6B6: 74 58              je          000000006714D710
  000000006714D6B8: 33 D2              xor         edx,edx
  000000006714D6BA: 41 8B C0           mov         eax,r8d
  000000006714D6BD: F7 F1              div         eax,ecx
  000000006714D6BF: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  000000006714D6C3: 48 8D 34 D0        lea         rsi,[rax+rdx*8]
  000000006714D6C7: 48 8B 3E           mov         rdi,qword ptr [rsi]
  000000006714D6CA: 48 3B FD           cmp         rdi,rbp
  000000006714D6CD: 74 44              je          000000006714D713
  000000006714D6CF: 48 8B 5C 24 20     mov         rbx,qword ptr [rsp+20h]
  000000006714D6D4: 44 3B 47 08        cmp         r8d,dword ptr [rdi+8]
  000000006714D6D8: 75 29              jne         000000006714D703
  000000006714D6DA: 48 63 43 08        movsxd      rax,dword ptr [rbx+8]
  000000006714D6DE: 48 8B 57 10        mov         rdx,qword ptr [rdi+10h]
  000000006714D6E2: 3B 42 08           cmp         eax,dword ptr [rdx+8]
  000000006714D6E5: 75 1C              jne         000000006714D703
  000000006714D6E7: 4C 8B C0           mov         r8,rax
  000000006714D6EA: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  000000006714D6EE: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  000000006714D6F2: E8 75 B4 02 00     call        0000000067178B6C
  000000006714D6F7: 85 C0              test        eax,eax
  000000006714D6F9: 74 18              je          000000006714D713
  000000006714D6FB: 44 8B 84 24 A8 00  mov         r8d,dword ptr [rsp+00000000000000A8h]
                    00 00
  000000006714D703: 48 8B F7           mov         rsi,rdi
  000000006714D706: 48 8B 3F           mov         rdi,qword ptr [rdi]
  000000006714D709: 48 3B FD           cmp         rdi,rbp
  000000006714D70C: 74 05              je          000000006714D713
  000000006714D70E: EB C4              jmp         000000006714D6D4
  000000006714D710: 48 8B F0           mov         rsi,rax
  000000006714D713: 8B 9C 24 A8 00 00  mov         ebx,dword ptr [rsp+00000000000000A8h]
                    00
  000000006714D71A: 89 9C 24 B8 00 00  mov         dword ptr [rsp+00000000000000B8h],ebx
                    00
  000000006714D721: BA 08 00 00 00     mov         edx,8
  000000006714D726: 48 8B CD           mov         rcx,rbp
  000000006714D729: E8 22 9F EF FF     call        0000000067047650
  000000006714D72E: 4C 8B D8           mov         r11,rax
  000000006714D731: 48 85 C0           test        rax,rax
  000000006714D734: 74 26              je          000000006714D75C
  000000006714D736: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006714D73B: 48 89 48 10        mov         qword ptr [rax+10h],rcx
  000000006714D73F: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006714D744: F0 83 01 01        lock add    dword ptr [rcx],1
  000000006714D748: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  000000006714D74D: 48 89 78 18        mov         qword ptr [rax+18h],rdi
  000000006714D751: 8B 9C 24 B8 00 00  mov         ebx,dword ptr [rsp+00000000000000B8h]
                    00
  000000006714D758: 33 ED              xor         ebp,ebp
  000000006714D75A: EB 0A              jmp         000000006714D766
  000000006714D75C: 33 ED              xor         ebp,ebp
  000000006714D75E: 4C 8B DD           mov         r11,rbp
  000000006714D761: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  000000006714D766: 41 89 5B 08        mov         dword ptr [r11+8],ebx
  000000006714D76A: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006714D76D: 49 89 03           mov         qword ptr [r11],rax
  000000006714D770: 4C 89 1E           mov         qword ptr [rsi],r11
  000000006714D773: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000006714D778: 48 8B 00           mov         rax,qword ptr [rax]
  000000006714D77B: FF 40 14           inc         dword ptr [rax+14h]
  000000006714D77E: EB 0E              jmp         000000006714D78E
  000000006714D780: 48 89 78 18        mov         qword ptr [rax+18h],rdi
  000000006714D784: 33 ED              xor         ebp,ebp
  000000006714D786: EB 06              jmp         000000006714D78E
  000000006714D788: 33 ED              xor         ebp,ebp
  000000006714D78A: EB 02              jmp         000000006714D78E
  000000006714D78C: 33 ED              xor         ebp,ebp
  000000006714D78E: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000006714D793: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006714D797: 75 0B              jne         000000006714D7A4
  000000006714D799: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006714D79E: E8 0D 2C ED FF     call        00000000670203B0
  000000006714D7A3: 90                 nop
  000000006714D7A4: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000006714D7A9: F6 C1 01           test        cl,1
  000000006714D7AC: 74 2D              je          000000006714D7DB
  000000006714D7AE: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006714D7B2: 48 89 4C 24 28     mov         qword ptr [rsp+28h],rcx
  000000006714D7B7: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006714D7BB: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006714D7BE: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006714D7C2: 74 07              je          000000006714D7CB
  000000006714D7C4: E8 67 83 ED FF     call        0000000067025B30
  000000006714D7C9: EB 10              jmp         000000006714D7DB
  000000006714D7CB: B8 01 00 00 00     mov         eax,1
  000000006714D7D0: F0 0F B1 2A        lock cmpxchg dword ptr [rdx],ebp
  000000006714D7D4: 74 05              je          000000006714D7DB
  000000006714D7D6: E8 95 83 ED FF     call        0000000067025B70
  000000006714D7DB: 48 8B C7           mov         rax,rdi
  000000006714D7DE: 48 83 C4 78        add         rsp,78h
  000000006714D7E2: 5F                 pop         rdi
  000000006714D7E3: 5E                 pop         rsi
  000000006714D7E4: 5D                 pop         rbp
  000000006714D7E5: 5B                 pop         rbx
  000000006714D7E6: C3                 ret

  Summary

      1A7000 .text
