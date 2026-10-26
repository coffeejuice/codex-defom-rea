Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065373430: 40 57              push        rdi
  0000000065373432: 48 83 EC 30        sub         rsp,30h
  0000000065373436: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006537343F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  0000000065373444: 48 89 6C 24 50     mov         qword ptr [rsp+50h],rbp
  0000000065373449: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  000000006537344E: 0F B6 F2           movzx       esi,dl
  0000000065373451: 48 8B D9           mov         rbx,rcx
  0000000065373454: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  0000000065373458: 8B 8F 04 02 00 00  mov         ecx,dword ptr [rdi+0000000000000204h]
  000000006537345E: F6 C1 01           test        cl,1
  0000000065373461: 0F 84 F6 00 00 00  je          000000006537355D
  0000000065373467: 0F B6 EA           movzx       ebp,dl
  000000006537346A: 8B C1              mov         eax,ecx
  000000006537346C: D1 E8              shr         eax,1
  000000006537346E: 83 E0 01           and         eax,1
  0000000065373471: 3B C5              cmp         eax,ebp
  0000000065373473: 0F 84 E4 00 00 00  je          000000006537355D
  0000000065373479: 84 D2              test        dl,dl
  000000006537347B: 75 48              jne         00000000653734C5
  000000006537347D: 48 8B CF           mov         rcx,rdi
  0000000065373480: E8 4B F5 FF FF     call        00000000653729D0
  0000000065373485: 48 3B C3           cmp         rax,rbx
  0000000065373488: 75 3B              jne         00000000653734C5
  000000006537348A: 48 8B 8F 08 02 00  mov         rcx,qword ptr [rdi+0000000000000208h]
                    00
  0000000065373491: 48 85 C9           test        rcx,rcx
  0000000065373494: 74 0D              je          00000000653734A3
  0000000065373496: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006537349A: 0F B6 90 98 00 00  movzx       edx,byte ptr [rax+0000000000000098h]
                    00
  00000000653734A1: EB 0C              jmp         00000000653734AF
  00000000653734A3: 8B 97 04 02 00 00  mov         edx,dword ptr [rdi+0000000000000204h]
  00000000653734A9: C1 EA 03           shr         edx,3
  00000000653734AC: 83 E2 01           and         edx,1
  00000000653734AF: 85 D2              test        edx,edx
  00000000653734B1: 0F 85 B7 00 00 00  jne         000000006537356E
  00000000653734B7: 48 85 C9           test        rcx,rcx
  00000000653734BA: 74 09              je          00000000653734C5
  00000000653734BC: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000000653734C0: E8 0B E6 FF FF     call        0000000065371AD0
  00000000653734C5: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000000653734CA: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000653734CF: FF 15 03 34 32 00  call        qword ptr [00000000656968D8h]
  00000000653734D5: 90                 nop
  00000000653734D6: 83 A7 04 02 00 00  and         dword ptr [rdi+0000000000000204h],0FFFFFFFDh
                    FD
  00000000653734DD: 83 E5 01           and         ebp,1
  00000000653734E0: 03 ED              add         ebp,ebp
  00000000653734E2: 09 AF 04 02 00 00  or          dword ptr [rdi+0000000000000204h],ebp
  00000000653734E8: F6 87 04 02 00 00  test        byte ptr [rdi+0000000000000204h],20h
                    20
  00000000653734EF: 75 0C              jne         00000000653734FD
  00000000653734F1: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000653734F4: 48 8B CB           mov         rcx,rbx
  00000000653734F7: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  00000000653734FD: 48 8B 6F 08        mov         rbp,qword ptr [rdi+8]
  0000000065373501: F6 87 04 02 00 00  test        byte ptr [rdi+0000000000000204h],20h
                    20
  0000000065373508: 75 18              jne         0000000065373522
  000000006537350A: 48 8B CD           mov         rcx,rbp
  000000006537350D: E8 9E 1D CF FF     call        00000000650652B0
  0000000065373512: 33 D2              xor         edx,edx
  0000000065373514: 41 B8 0A 80 00 00  mov         r8d,800Ah
  000000006537351A: 48 8B CD           mov         rcx,rbp
  000000006537351D: E8 3E F4 12 00     call        00000000654A2960
  0000000065373522: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000065373527: 48 85 C0           test        rax,rax
  000000006537352A: 74 24              je          0000000065373550
  000000006537352C: 40 84 F6           test        sil,sil
  000000006537352F: 74 0D              je          000000006537353E
  0000000065373531: 48 8B CF           mov         rcx,rdi
  0000000065373534: E8 C7 F5 FF FF     call        0000000065372B00
  0000000065373539: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000006537353E: 48 85 C0           test        rax,rax
  0000000065373541: 74 0D              je          0000000065373550
  0000000065373543: 40 0F B6 D6        movzx       edx,sil
  0000000065373547: 48 8B CB           mov         rcx,rbx
  000000006537354A: E8 41 FC 25 00     call        00000000655D3190
  000000006537354F: 90                 nop
  0000000065373550: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065373555: FF 15 85 33 32 00  call        qword ptr [00000000656968E0h]
  000000006537355B: EB 11              jmp         000000006537356E
  000000006537355D: F6 C1 20           test        cl,20h
  0000000065373560: 75 0C              jne         000000006537356E
  0000000065373562: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065373565: 48 8B CB           mov         rcx,rbx
  0000000065373568: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  000000006537356E: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000065373573: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  0000000065373578: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  000000006537357D: 48 83 C4 30        add         rsp,30h
  0000000065373581: 5F                 pop         rdi
  0000000065373582: C3                 ret

  Summary

      695000 .text
