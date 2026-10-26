Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000655000F0: 48 8B C4           mov         rax,rsp
  00000000655000F3: 55                 push        rbp
  00000000655000F4: 56                 push        rsi
  00000000655000F5: 57                 push        rdi
  00000000655000F6: 41 54              push        r12
  00000000655000F8: 41 55              push        r13
  00000000655000FA: 48 83 EC 50        sub         rsp,50h
  00000000655000FE: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065500107: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  000000006550010B: 4D 8B E0           mov         r12,r8
  000000006550010E: 48 8B F1           mov         rsi,rcx
  0000000065500111: 45 33 ED           xor         r13d,r13d
  0000000065500114: 44 89 68 08        mov         dword ptr [rax+8],r13d
  0000000065500118: 48 8B 69 08        mov         rbp,qword ptr [rcx+8]
  000000006550011C: 48 8D 48 18        lea         rcx,[rax+18h]
  0000000065500120: FF 15 22 8A 19 00  call        qword ptr [0000000065698B48h]
  0000000065500126: 90                 nop
  0000000065500127: 48 8B 96 80 02 00  mov         rdx,qword ptr [rsi+0000000000000280h]
                    00
  000000006550012E: 48 8D 0D 53 92 4C  lea         rcx,[00000000659C9388h]
                    00
  0000000065500135: FF 15 CD 68 19 00  call        qword ptr [0000000065696A08h]
  000000006550013B: 48 8B D8           mov         rbx,rax
  000000006550013E: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000065500146: FF 15 F4 86 19 00  call        qword ptr [0000000065698840h]
  000000006550014C: 8B F8              mov         edi,eax
  000000006550014E: 85 C0              test        eax,eax
  0000000065500150: 78 1F              js          0000000065500171
  0000000065500152: 48 8D 4B 10        lea         rcx,[rbx+10h]
  0000000065500156: FF 15 5C 67 19 00  call        qword ptr [00000000656968B8h]
  000000006550015C: 3B F8              cmp         edi,eax
  000000006550015E: 7D 11              jge         0000000065500171
  0000000065500160: 8B D7              mov         edx,edi
  0000000065500162: 48 8D 4B 10        lea         rcx,[rbx+10h]
  0000000065500166: FF 15 D4 67 19 00  call        qword ptr [0000000065696940h]
  000000006550016C: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006550016F: EB 03              jmp         0000000065500174
  0000000065500171: 49 8B DD           mov         rbx,r13
  0000000065500174: 41 8B 3C 24        mov         edi,dword ptr [r12]
  0000000065500178: 48 8B 96 80 02 00  mov         rdx,qword ptr [rsi+0000000000000280h]
                    00
  000000006550017F: 48 8D 0D 02 92 4C  lea         rcx,[00000000659C9388h]
                    00
  0000000065500186: FF 15 7C 68 19 00  call        qword ptr [0000000065696A08h]
  000000006550018C: 48 8D 70 10        lea         rsi,[rax+10h]
  0000000065500190: 85 FF              test        edi,edi
  0000000065500192: 78 1D              js          00000000655001B1
  0000000065500194: 48 8B CE           mov         rcx,rsi
  0000000065500197: FF 15 1B 67 19 00  call        qword ptr [00000000656968B8h]
  000000006550019D: 3B F8              cmp         edi,eax
  000000006550019F: 7D 10              jge         00000000655001B1
  00000000655001A1: 8B D7              mov         edx,edi
  00000000655001A3: 48 8B CE           mov         rcx,rsi
  00000000655001A6: FF 15 94 67 19 00  call        qword ptr [0000000065696940h]
  00000000655001AC: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000655001AF: EB 03              jmp         00000000655001B4
  00000000655001B1: 49 8B CD           mov         rcx,r13
  00000000655001B4: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  00000000655001B9: 48 89 9C 24 98 00  mov         qword ptr [rsp+0000000000000098h],rbx
                    00 00
  00000000655001C1: 4C 89 6C 24 30     mov         qword ptr [rsp+30h],r13
  00000000655001C6: 48 8D 84 24 98 00  lea         rax,[rsp+0000000000000098h]
                    00 00
  00000000655001CE: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000000655001D3: 48 8D 44 24 20     lea         rax,[rsp+20h]
  00000000655001D8: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000655001DD: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  00000000655001E2: 41 B8 06 00 00 00  mov         r8d,6
  00000000655001E8: 48 8D 15 19 4B 23  lea         rdx,[0000000065734D08h]
                    00
  00000000655001EF: 48 8B CD           mov         rcx,rbp
  00000000655001F2: FF 15 B8 66 19 00  call        qword ptr [00000000656968B0h]
  00000000655001F8: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000065500200: FF 15 72 86 19 00  call        qword ptr [0000000065698878h]
  0000000065500206: 84 C0              test        al,al
  0000000065500208: 74 21              je          000000006550022B
  000000006550020A: 48 85 DB           test        rbx,rbx
  000000006550020D: 74 1C              je          000000006550022B
  000000006550020F: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000065500214: 48 8B CB           mov         rcx,rbx
  0000000065500217: E8 24 B2 F4 FF     call        000000006544B440
  000000006550021C: 90                 nop
  000000006550021D: BB 01 00 00 00     mov         ebx,1
  0000000065500222: 89 9C 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ebx
                    00
  0000000065500229: EB 1B              jmp         0000000065500246
  000000006550022B: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000065500233: FF 15 2F 65 19 00  call        qword ptr [0000000065696768h]
  0000000065500239: 90                 nop
  000000006550023A: BB 02 00 00 00     mov         ebx,2
  000000006550023F: 89 9C 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ebx
                    00
  0000000065500246: 4C 89 6C 24 30     mov         qword ptr [rsp+30h],r13
  000000006550024B: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000065500250: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  0000000065500255: 41 B8 07 00 00 00  mov         r8d,7
  000000006550025B: 48 8D 15 A6 4A 23  lea         rdx,[0000000065734D08h]
                    00
  0000000065500262: 48 8B CD           mov         rcx,rbp
  0000000065500265: FF 15 45 66 19 00  call        qword ptr [00000000656968B0h]
  000000006550026B: 90                 nop
  000000006550026C: F6 C3 02           test        bl,2
  000000006550026F: 74 19              je          000000006550028A
  0000000065500271: 83 E3 FD           and         ebx,0FFFFFFFDh
  0000000065500274: 89 9C 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ebx
                    00
  000000006550027B: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000065500283: FF 15 D7 64 19 00  call        qword ptr [0000000065696760h]
  0000000065500289: 90                 nop
  000000006550028A: F6 C3 01           test        bl,1
  000000006550028D: 74 0B              je          000000006550029A
  000000006550028F: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000065500294: FF 15 C6 64 19 00  call        qword ptr [0000000065696760h]
  000000006550029A: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000000655002A2: FF 15 98 85 19 00  call        qword ptr [0000000065698840h]
  00000000655002A8: 89 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],eax
                    00
  00000000655002AF: 4C 89 6C 24 30     mov         qword ptr [rsp+30h],r13
  00000000655002B4: 48 8D 84 24 80 00  lea         rax,[rsp+0000000000000080h]
                    00 00
  00000000655002BC: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000000655002C1: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  00000000655002C6: 41 B8 08 00 00 00  mov         r8d,8
  00000000655002CC: 48 8D 15 35 4A 23  lea         rdx,[0000000065734D08h]
                    00
  00000000655002D3: 48 8B CD           mov         rcx,rbp
  00000000655002D6: FF 15 D4 65 19 00  call        qword ptr [00000000656968B0h]
  00000000655002DC: 90                 nop
  00000000655002DD: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000000655002E5: FF 15 2D 85 19 00  call        qword ptr [0000000065698818h]
  00000000655002EB: 48 8B 9C 24 88 00  mov         rbx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000655002F3: 48 83 C4 50        add         rsp,50h
  00000000655002F7: 41 5D              pop         r13
  00000000655002F9: 41 5C              pop         r12
  00000000655002FB: 5F                 pop         rdi
  00000000655002FC: 5E                 pop         rsi
  00000000655002FD: 5D                 pop         rbp
  00000000655002FE: C3                 ret

  Summary

      695000 .text
