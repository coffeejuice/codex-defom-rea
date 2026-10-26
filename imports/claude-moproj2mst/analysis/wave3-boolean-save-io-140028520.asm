Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140028520: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140028525: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  000000014002852C: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140028535: C6 44 24 28 01     mov         byte ptr [rsp+28h],1
  000000014002853A: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140028542: E8 C9 4F FF FF     call        000000014001D510
  0000000140028547: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014002854C: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140028552: 75 07              jne         000000014002855B
  0000000140028554: 32 C0              xor         al,al
  0000000140028556: E9 2B 01 00 00     jmp         0000000140028686
  000000014002855B: 48 8D 0D 56 E1 D4  lea         rcx,[0000000143D766B8h]
                    03
  0000000140028562: E8 09 33 2A 00     call        00000001402CB870
  0000000140028567: 0F B6 C0           movzx       eax,al
  000000014002856A: 85 C0              test        eax,eax
  000000014002856C: 75 4D              jne         00000001400285BB
  000000014002856E: 45 33 C0           xor         r8d,r8d
  0000000140028571: 48 8D 15 50 78 0D  lea         rdx,[00000001420FFDC8h]
                    02
  0000000140028578: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014002857D: E8 FE C7 FF FF     call        0000000140024D80
  0000000140028582: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140028587: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014002858C: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140028591: 45 33 C9           xor         r9d,r9d
  0000000140028594: 45 33 C0           xor         r8d,r8d
  0000000140028597: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  000000014002859C: 48 8D 0D AD DD D4  lea         rcx,[0000000143D76350h]
                    03
  00000001400285A3: E8 28 86 2A 00     call        00000001402D0BD0
  00000001400285A8: 90                 nop
  00000001400285A9: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001400285AE: FF 15 2C D4 DD 01  call        qword ptr [0000000141E059E0h]
  00000001400285B4: B0 01              mov         al,1
  00000001400285B6: E9 CB 00 00 00     jmp         0000000140028686
  00000001400285BB: B2 01              mov         dl,1
  00000001400285BD: 48 8D 0D F4 E0 D4  lea         rcx,[0000000143D766B8h]
                    03
  00000001400285C4: E8 27 24 2A 00     call        00000001402CA9F0
  00000001400285C9: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400285CE: E8 FD FA 06 00     call        00000001400980D0
  00000001400285D3: 89 44 24 2C        mov         dword ptr [rsp+2Ch],eax
  00000001400285D7: 81 7C 24 2C 00 00  cmp         dword ptr [rsp+2Ch],10000h
                    01 00
  00000001400285DF: 75 48              jne         0000000140028629
  00000001400285E1: 45 33 C0           xor         r8d,r8d
  00000001400285E4: 48 8D 15 15 78 0D  lea         rdx,[00000001420FFE00h]
                    02
  00000001400285EB: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001400285F0: E8 8B C7 FF FF     call        0000000140024D80
  00000001400285F5: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001400285FA: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400285FF: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140028604: 45 33 C9           xor         r9d,r9d
  0000000140028607: 45 33 C0           xor         r8d,r8d
  000000014002860A: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  000000014002860F: 48 8D 0D 3A DD D4  lea         rcx,[0000000143D76350h]
                    03
  0000000140028616: E8 B5 85 2A 00     call        00000001402D0BD0
  000000014002861B: 90                 nop
  000000014002861C: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140028621: FF 15 B9 D3 DD 01  call        qword ptr [0000000141E059E0h]
  0000000140028627: EB 4B              jmp         0000000140028674
  0000000140028629: C6 44 24 28 00     mov         byte ptr [rsp+28h],0
  000000014002862E: 45 33 C0           xor         r8d,r8d
  0000000140028631: 48 8D 15 E8 77 0D  lea         rdx,[00000001420FFE20h]
                    02
  0000000140028638: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014002863D: E8 3E C7 FF FF     call        0000000140024D80
  0000000140028642: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140028647: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  000000014002864C: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140028651: 45 33 C9           xor         r9d,r9d
  0000000140028654: 45 33 C0           xor         r8d,r8d
  0000000140028657: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  000000014002865C: 48 8D 0D ED DC D4  lea         rcx,[0000000143D76350h]
                    03
  0000000140028663: E8 68 85 2A 00     call        00000001402D0BD0
  0000000140028668: 90                 nop
  0000000140028669: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014002866E: FF 15 6C D3 DD 01  call        qword ptr [0000000141E059E0h]
  0000000140028674: 33 D2              xor         edx,edx
  0000000140028676: 48 8D 0D 3B E0 D4  lea         rcx,[0000000143D766B8h]
                    03
  000000014002867D: E8 6E 23 2A 00     call        00000001402CA9F0
  0000000140028682: 8A 44 24 28        mov         al,byte ptr [rsp+28h]
  0000000140028686: 48 81 C4 88 00 00  add         rsp,88h
                    00
  000000014002868D: C3                 ret

  Summary

     1E02000 .text
