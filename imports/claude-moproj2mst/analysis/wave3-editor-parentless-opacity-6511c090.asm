Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006511C090: 48 8B C4           mov         rax,rsp
  000000006511C093: 57                 push        rdi
  000000006511C094: 48 83 EC 70        sub         rsp,70h
  000000006511C098: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006511C0A1: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  000000006511C0A5: 48 8B F9           mov         rdi,rcx
  000000006511C0A8: 33 DB              xor         ebx,ebx
  000000006511C0AA: 89 58 08           mov         dword ptr [rax+8],ebx
  000000006511C0AD: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000006511C0B0: 48 83 C1 08        add         rcx,8
  000000006511C0B4: E8 17 26 00 00     call        000000006511E6D0
  000000006511C0B9: 3D FF 00 00 00     cmp         eax,0FFh
  000000006511C0BE: 0F 94 C0           sete        al
  000000006511C0C1: 48 8B 17           mov         rdx,qword ptr [rdi]
  000000006511C0C4: 83 7A 04 01        cmp         dword ptr [rdx+4],1
  000000006511C0C8: 0F 84 C9 01 00 00  je          000000006511C297
  000000006511C0CE: 48 8B CF           mov         rcx,rdi
  000000006511C0D1: E8 CA EA FF FF     call        000000006511ABA0
  000000006511C0D6: 84 C0              test        al,al
  000000006511C0D8: 74 07              je          000000006511C0E1
  000000006511C0DA: 32 C0              xor         al,al
  000000006511C0DC: E9 B6 01 00 00     jmp         000000006511C297
  000000006511C0E1: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006511C0E4: 83 78 04 0F        cmp         dword ptr [rax+4],0Fh
  000000006511C0E8: 0F 84 FC 00 00 00  je          000000006511C1EA
  000000006511C0EE: 83 78 04 10        cmp         dword ptr [rax+4],10h
  000000006511C0F2: 0F 84 F2 00 00 00  je          000000006511C1EA
  000000006511C0F8: 83 78 04 11        cmp         dword ptr [rax+4],11h
  000000006511C0FC: 0F 84 E8 00 00 00  je          000000006511C1EA
  000000006511C102: 83 78 04 18        cmp         dword ptr [rax+4],18h
  000000006511C106: 0F 85 D7 00 00 00  jne         000000006511C1E3
  000000006511C10C: 83 78 04 18        cmp         dword ptr [rax+4],18h
  000000006511C110: 75 5E              jne         000000006511C170
  000000006511C112: 38 98 90 00 00 00  cmp         byte ptr [rax+0000000000000090h],bl
  000000006511C118: 74 56              je          000000006511C170
  000000006511C11A: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006511C11F: 48 8B CF           mov         rcx,rdi
  000000006511C122: E8 39 E4 FF FF     call        000000006511A560
  000000006511C127: 90                 nop
  000000006511C128: BB 01 00 00 00     mov         ebx,1
  000000006511C12D: 89 9C 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ebx
                    00
  000000006511C134: 48 8B C8           mov         rcx,rax
  000000006511C137: E8 F4 2A FB FF     call        00000000650CEC30
  000000006511C13C: 84 C0              test        al,al
  000000006511C13E: 75 2B              jne         000000006511C16B
  000000006511C140: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006511C145: 48 8B CF           mov         rcx,rdi
  000000006511C148: E8 13 E4 FF FF     call        000000006511A560
  000000006511C14D: 90                 nop
  000000006511C14E: BB 03 00 00 00     mov         ebx,3
  000000006511C153: 89 9C 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ebx
                    00
  000000006511C15A: 48 8B C8           mov         rcx,rax
  000000006511C15D: E8 1E 28 FB FF     call        00000000650CE980
  000000006511C162: 84 C0              test        al,al
  000000006511C164: 75 05              jne         000000006511C16B
  000000006511C166: 40 B7 01           mov         dil,1
  000000006511C169: EB 2D              jmp         000000006511C198
  000000006511C16B: 40 32 FF           xor         dil,dil
  000000006511C16E: EB 28              jmp         000000006511C198
  000000006511C170: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006511C175: 48 8B CF           mov         rcx,rdi
  000000006511C178: E8 93 E4 FF FF     call        000000006511A610
  000000006511C17D: 90                 nop
  000000006511C17E: BB 04 00 00 00     mov         ebx,4
  000000006511C183: 89 9C 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ebx
                    00
  000000006511C18A: 48 8B C8           mov         rcx,rax
  000000006511C18D: E8 AE E1 F8 FF     call        00000000650AA340
  000000006511C192: 84 C0              test        al,al
  000000006511C194: 40 0F 94 C7        sete        dil
  000000006511C198: F6 C3 04           test        bl,4
  000000006511C19B: 74 15              je          000000006511C1B2
  000000006511C19D: 83 E3 FB           and         ebx,0FFFFFFFBh
  000000006511C1A0: 89 9C 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ebx
                    00
  000000006511C1A7: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006511C1AC: E8 4F 71 F9 FF     call        00000000650B3300
  000000006511C1B1: 90                 nop
  000000006511C1B2: F6 C3 02           test        bl,2
  000000006511C1B5: 74 15              je          000000006511C1CC
  000000006511C1B7: 83 E3 FD           and         ebx,0FFFFFFFDh
  000000006511C1BA: 89 9C 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ebx
                    00
  000000006511C1C1: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006511C1C6: E8 55 27 FB FF     call        00000000650CE920
  000000006511C1CB: 90                 nop
  000000006511C1CC: F6 C3 01           test        bl,1
  000000006511C1CF: 74 0A              je          000000006511C1DB
  000000006511C1D1: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000006511C1D6: E8 45 27 FB FF     call        00000000650CE920
  000000006511C1DB: 40 8A C7           mov         al,dil
  000000006511C1DE: E9 B4 00 00 00     jmp         000000006511C297
  000000006511C1E3: 32 C0              xor         al,al
  000000006511C1E5: E9 AD 00 00 00     jmp         000000006511C297
  000000006511C1EA: 83 78 04 0F        cmp         dword ptr [rax+4],0Fh
  000000006511C1EE: 74 11              je          000000006511C201
  000000006511C1F0: 83 78 04 10        cmp         dword ptr [rax+4],10h
  000000006511C1F4: 74 0B              je          000000006511C201
  000000006511C1F6: 83 78 04 11        cmp         dword ptr [rax+4],11h
  000000006511C1FA: 74 05              je          000000006511C201
  000000006511C1FC: 48 8B CB           mov         rcx,rbx
  000000006511C1FF: EB 04              jmp         000000006511C205
  000000006511C201: 48 8D 48 70        lea         rcx,[rax+70h]
  000000006511C205: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  000000006511C20D: E8 7E F1 FF FF     call        000000006511B390
  000000006511C212: 90                 nop
  000000006511C213: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006511C21B: 83 79 08 00        cmp         dword ptr [rcx+8],0
  000000006511C21F: 7E 57              jle         000000006511C278
  000000006511C221: 48 63 C3           movsxd      rax,ebx
  000000006511C224: 48 8D 14 40        lea         rdx,[rax+rax*2]
  000000006511C228: 48 8D 4C D1 18     lea         rcx,[rcx+rdx*8+18h]
  000000006511C22D: E8 9E 24 00 00     call        000000006511E6D0
  000000006511C232: 3D FF 00 00 00     cmp         eax,0FFh
  000000006511C237: 75 11              jne         000000006511C24A
  000000006511C239: FF C3              inc         ebx
  000000006511C23B: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006511C243: 3B 59 08           cmp         ebx,dword ptr [rcx+8]
  000000006511C246: 7C D9              jl          000000006511C221
  000000006511C248: EB 2E              jmp         000000006511C278
  000000006511C24A: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006511C252: 48 85 C9           test        rcx,rcx
  000000006511C255: 74 1D              je          000000006511C274
  000000006511C257: FF 15 1B A7 57 00  call        qword ptr [0000000065696978h]
  000000006511C25D: 84 C0              test        al,al
  000000006511C25F: 75 13              jne         000000006511C274
  000000006511C261: BA 08 00 00 00     mov         edx,8
  000000006511C266: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006511C26E: FF 15 84 A6 57 00  call        qword ptr [00000000656968F8h]
  000000006511C274: 32 C0              xor         al,al
  000000006511C276: EB 1F              jmp         000000006511C297
  000000006511C278: FF 15 FA A6 57 00  call        qword ptr [0000000065696978h]
  000000006511C27E: 84 C0              test        al,al
  000000006511C280: 75 13              jne         000000006511C295
  000000006511C282: BA 08 00 00 00     mov         edx,8
  000000006511C287: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006511C28F: FF 15 63 A6 57 00  call        qword ptr [00000000656968F8h]
  000000006511C295: B0 01              mov         al,1
  000000006511C297: 48 8B 9C 24 88 00  mov         rbx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000006511C29F: 48 83 C4 70        add         rsp,70h
  000000006511C2A3: 5F                 pop         rdi
  000000006511C2A4: C3                 ret

  Summary

      695000 .text
