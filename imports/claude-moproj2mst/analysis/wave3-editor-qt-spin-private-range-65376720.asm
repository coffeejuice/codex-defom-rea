Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065376720: 48 8B C4           mov         rax,rsp
  0000000065376723: 57                 push        rdi
  0000000065376724: 41 54              push        r12
  0000000065376726: 41 55              push        r13
  0000000065376728: 48 83 EC 70        sub         rsp,70h
  000000006537672C: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065376735: 48 89 58 08        mov         qword ptr [rax+8],rbx
  0000000065376739: 48 89 68 10        mov         qword ptr [rax+10h],rbp
  000000006537673D: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  0000000065376741: 41 0F B6 F9        movzx       edi,r9b
  0000000065376745: 41 8B E8           mov         ebp,r8d
  0000000065376748: 48 8B DA           mov         rbx,rdx
  000000006537674B: 48 8B F1           mov         rsi,rcx
  000000006537674E: 4C 8B 61 08        mov         r12,qword ptr [rcx+8]
  0000000065376752: 48 8D 91 00 02 00  lea         rdx,[rcx+0000000000000200h]
                    00
  0000000065376759: 48 8D 48 B0        lea         rcx,[rax-50h]
  000000006537675D: FF 15 85 FF 31 00  call        qword ptr [00000000656966E8h]
  0000000065376763: 90                 nop
  0000000065376764: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000065376769: FF 15 21 02 32 00  call        qword ptr [0000000065696990h]
  000000006537676F: 90                 nop
  0000000065376770: 4C 8B 16           mov         r10,qword ptr [rsi]
  0000000065376773: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000006537677B: 4C 8B C8           mov         r9,rax
  000000006537677E: 4C 8B C3           mov         r8,rbx
  0000000065376781: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000065376786: 48 8B CE           mov         rcx,rsi
  0000000065376789: 41 FF 52 28        call        qword ptr [r10+28h]
  000000006537678D: 90                 nop
  000000006537678E: 48 8B D0           mov         rdx,rax
  0000000065376791: 48 8D 8E 00 02 00  lea         rcx,[rsi+0000000000000200h]
                    00
  0000000065376798: FF 15 F2 FF 31 00  call        qword ptr [0000000065696790h]
  000000006537679E: 90                 nop
  000000006537679F: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000000653767A4: FF 15 36 FF 31 00  call        qword ptr [00000000656966E0h]
  00000000653767AA: 90                 nop
  00000000653767AB: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000653767B0: FF 15 2A FF 31 00  call        qword ptr [00000000656966E0h]
  00000000653767B6: 81 A6 8C 02 00 00  and         dword ptr [rsi+000000000000028Ch],0FFFFFF7Eh
                    7E FF FF FF
  00000000653767C0: 40 84 FF           test        dil,dil
  00000000653767C3: 74 09              je          00000000653767CE
  00000000653767C5: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000653767C8: 48 8B CE           mov         rcx,rsi
  00000000653767CB: FF 50 30           call        qword ptr [rax+30h]
  00000000653767CE: 49 8B CC           mov         rcx,r12
  00000000653767D1: E8 DA EA CE FF     call        00000000650652B0
  00000000653767D6: 83 FD 01           cmp         ebp,1
  00000000653767D9: 74 1A              je          00000000653767F5
  00000000653767DB: 85 ED              test        ebp,ebp
  00000000653767DD: 75 27              jne         0000000065376806
  00000000653767DF: 48 8D 96 00 02 00  lea         rdx,[rsi+0000000000000200h]
                    00
  00000000653767E6: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000653767EB: FF 15 27 21 32 00  call        qword ptr [0000000065698918h]
  00000000653767F1: 84 C0              test        al,al
  00000000653767F3: 74 11              je          0000000065376806
  00000000653767F5: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000653767F8: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  00000000653767FD: 8B D5              mov         edx,ebp
  00000000653767FF: 48 8B CE           mov         rcx,rsi
  0000000065376802: FF 50 38           call        qword ptr [rax+38h]
  0000000065376805: 90                 nop
  0000000065376806: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006537680B: FF 15 CF FE 31 00  call        qword ptr [00000000656966E0h]
  0000000065376811: 4C 8D 5C 24 70     lea         r11,[rsp+70h]
  0000000065376816: 49 8B 5B 20        mov         rbx,qword ptr [r11+20h]
  000000006537681A: 49 8B 6B 28        mov         rbp,qword ptr [r11+28h]
  000000006537681E: 49 8B 73 30        mov         rsi,qword ptr [r11+30h]
  0000000065376822: 49 8B E3           mov         rsp,r11
  0000000065376825: 41 5D              pop         r13
  0000000065376827: 41 5C              pop         r12
  0000000065376829: 5F                 pop         rdi
  000000006537682A: C3                 ret

  Summary

      695000 .text
