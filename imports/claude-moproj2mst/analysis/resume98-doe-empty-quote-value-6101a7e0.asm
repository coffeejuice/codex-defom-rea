Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  000000006101A7E0: 40 53              push        rbx
  000000006101A7E2: 55                 push        rbp
  000000006101A7E3: 56                 push        rsi
  000000006101A7E4: 57                 push        rdi
  000000006101A7E5: 41 54              push        r12
  000000006101A7E7: 41 55              push        r13
  000000006101A7E9: 48 83 EC 68        sub         rsp,68h
  000000006101A7ED: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006101A7F6: 48 8B F9           mov         rdi,rcx
  000000006101A7F9: 48 8B 89 D8 00 00  mov         rcx,qword ptr [rcx+00000000000000D8h]
                    00
  000000006101A800: 48 85 C9           test        rcx,rcx
  000000006101A803: 74 6F              je          000000006101A874
  000000006101A805: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006101A808: 83 78 08 00        cmp         dword ptr [rax+8],0
  000000006101A80C: 74 66              je          000000006101A874
  000000006101A80E: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006101A813: E8 08 A7 FF FF     call        0000000061014F20
  000000006101A818: 0F B6 70 08        movzx       esi,byte ptr [rax+8]
  000000006101A81C: 48 8B 8F D8 00 00  mov         rcx,qword ptr [rdi+00000000000000D8h]
                    00
  000000006101A823: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006101A826: 83 78 08 00        cmp         dword ptr [rax+8],0
  000000006101A82A: 74 4B              je          000000006101A877
  000000006101A82C: E8 9F A7 FF FF     call        0000000061014FD0
  000000006101A831: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006101A834: 48 8D 05 35 C8 FF  lea         rax,[0000000061017070h]
                    FF
  000000006101A83B: 48 3B D8           cmp         rbx,rax
  000000006101A83E: 75 11              jne         000000006101A851
  000000006101A840: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006101A845: 48 8B 8F D8 00 00  mov         rcx,qword ptr [rdi+00000000000000D8h]
                    00
  000000006101A84C: E8 CF A6 FF FF     call        0000000061014F20
  000000006101A851: 48 8B CF           mov         rcx,rdi
  000000006101A854: FF D3              call        rbx
  000000006101A856: 84 C0              test        al,al
  000000006101A858: 75 1D              jne         000000006101A877
  000000006101A85A: 44 0F BE C6        movsx       r8d,sil
  000000006101A85E: 48 8D 15 7B FF FF  lea         rdx,[000000006101A7E0h]
                    FF
  000000006101A865: 48 8B CF           mov         rcx,rdi
  000000006101A868: E8 43 BC FF FF     call        00000000610164B0
  000000006101A86D: 32 C0              xor         al,al
  000000006101A86F: E9 AB 02 00 00     jmp         000000006101AB1F
  000000006101A874: 40 32 F6           xor         sil,sil
  000000006101A877: 40 80 FE FF        cmp         sil,0FFh
  000000006101A87B: 0F 84 56 02 00 00  je          000000006101AAD7
  000000006101A881: 41 BD FF FF 00 00  mov         r13d,0FFFFh
  000000006101A887: 4C 8D 25 72 57 FE  lea         r12,[0000000061000000h]
                    FF
  000000006101A88E: 66 90              nop
  000000006101A890: 40 80 FE 07        cmp         sil,7
  000000006101A894: 0F 84 39 02 00 00  je          000000006101AAD3
  000000006101A89A: 48 8D 8F 20 01 00  lea         rcx,[rdi+0000000000000120h]
                    00
  000000006101A8A1: FF 15 21 90 02 00  call        qword ptr [00000000610438C8h]
  000000006101A8A7: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101A8AA: 66 83 C9 01        or          cx,1
  000000006101A8AE: 66 41 3B CD        cmp         cx,r13w
  000000006101A8B2: 0F 84 04 02 00 00  je          000000006101AABC
  000000006101A8B8: B2 22              mov         dl,22h
  000000006101A8BA: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006101A8BF: FF 15 C3 8F 02 00  call        qword ptr [0000000061043888h]
  000000006101A8C5: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101A8C8: 66 89 8C 24 A0 00  mov         word ptr [rsp+00000000000000A0h],cx
                    00 00
  000000006101A8D0: 0F B7 87 20 01 00  movzx       eax,word ptr [rdi+0000000000000120h]
                    00
  000000006101A8D7: 66 89 84 24 A8 00  mov         word ptr [rsp+00000000000000A8h],ax
                    00 00
  000000006101A8DF: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006101A8E7: FF 15 DB 8F 02 00  call        qword ptr [00000000610438C8h]
  000000006101A8ED: 48 8B D8           mov         rbx,rax
  000000006101A8F0: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006101A8F8: FF 15 CA 8F 02 00  call        qword ptr [00000000610438C8h]
  000000006101A8FE: 0F B7 0B           movzx       ecx,word ptr [rbx]
  000000006101A901: 66 39 08           cmp         word ptr [rax],cx
  000000006101A904: 75 07              jne         000000006101A90D
  000000006101A906: 32 D2              xor         dl,dl
  000000006101A908: E9 E1 00 00 00     jmp         000000006101A9EE
  000000006101A90D: B2 27              mov         dl,27h
  000000006101A90F: 48 8D 4C 24 44     lea         rcx,[rsp+44h]
  000000006101A914: FF 15 6E 8F 02 00  call        qword ptr [0000000061043888h]
  000000006101A91A: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101A91D: 66 89 8C 24 B0 00  mov         word ptr [rsp+00000000000000B0h],cx
                    00 00
  000000006101A925: 0F B7 87 20 01 00  movzx       eax,word ptr [rdi+0000000000000120h]
                    00
  000000006101A92C: 66 89 84 24 B8 00  mov         word ptr [rsp+00000000000000B8h],ax
                    00 00
  000000006101A934: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  000000006101A93C: FF 15 86 8F 02 00  call        qword ptr [00000000610438C8h]
  000000006101A942: 48 8B D8           mov         rbx,rax
  000000006101A945: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000006101A94D: FF 15 75 8F 02 00  call        qword ptr [00000000610438C8h]
  000000006101A953: 0F B7 0B           movzx       ecx,word ptr [rbx]
  000000006101A956: 66 39 08           cmp         word ptr [rax],cx
  000000006101A959: 75 07              jne         000000006101A962
  000000006101A95B: B2 01              mov         dl,1
  000000006101A95D: E9 8C 00 00 00     jmp         000000006101A9EE
  000000006101A962: B2 26              mov         dl,26h
  000000006101A964: 48 8D 4C 24 46     lea         rcx,[rsp+46h]
  000000006101A969: FF 15 19 8F 02 00  call        qword ptr [0000000061043888h]
  000000006101A96F: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101A972: 66 89 4C 24 30     mov         word ptr [rsp+30h],cx
  000000006101A977: 0F B7 87 20 01 00  movzx       eax,word ptr [rdi+0000000000000120h]
                    00
  000000006101A97E: 66 89 44 24 20     mov         word ptr [rsp+20h],ax
  000000006101A983: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006101A988: FF 15 3A 8F 02 00  call        qword ptr [00000000610438C8h]
  000000006101A98E: 48 8B D8           mov         rbx,rax
  000000006101A991: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006101A996: FF 15 2C 8F 02 00  call        qword ptr [00000000610438C8h]
  000000006101A99C: 0F B7 0B           movzx       ecx,word ptr [rbx]
  000000006101A99F: 66 39 08           cmp         word ptr [rax],cx
  000000006101A9A2: 75 04              jne         000000006101A9A8
  000000006101A9A4: B2 02              mov         dl,2
  000000006101A9A6: EB 46              jmp         000000006101A9EE
  000000006101A9A8: B2 3C              mov         dl,3Ch
  000000006101A9AA: 48 8D 4C 24 42     lea         rcx,[rsp+42h]
  000000006101A9AF: FF 15 D3 8E 02 00  call        qword ptr [0000000061043888h]
  000000006101A9B5: 0F B7 08           movzx       ecx,word ptr [rax]
  000000006101A9B8: 66 89 4C 24 28     mov         word ptr [rsp+28h],cx
  000000006101A9BD: 0F B7 87 20 01 00  movzx       eax,word ptr [rdi+0000000000000120h]
                    00
  000000006101A9C4: 66 89 44 24 38     mov         word ptr [rsp+38h],ax
  000000006101A9C9: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006101A9CE: FF 15 F4 8E 02 00  call        qword ptr [00000000610438C8h]
  000000006101A9D4: 48 8B D8           mov         rbx,rax
  000000006101A9D7: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006101A9DC: FF 15 E6 8E 02 00  call        qword ptr [00000000610438C8h]
  000000006101A9E2: 0F B7 0B           movzx       ecx,word ptr [rbx]
  000000006101A9E5: 66 39 08           cmp         word ptr [rax],cx
  000000006101A9E8: 0F 95 C2           setne       dl
  000000006101A9EB: 80 C2 03           add         dl,3
  000000006101A9EE: 48 0F BE C6        movsx       rax,sil
  000000006101A9F2: 48 8D 0C 80        lea         rcx,[rax+rax*4]
  000000006101A9F6: 48 0F BE C2        movsx       rax,dl
  000000006101A9FA: 48 03 C8           add         rcx,rax
  000000006101A9FD: 42 0F BE B4 21 A8  movsx       esi,byte ptr [rcx+r12+0000000000047DA8h]
                    7D 04 00
  000000006101AA06: 8D 46 FF           lea         eax,[rsi-1]
  000000006101AA09: 83 F8 06           cmp         eax,6
  000000006101AA0C: 77 5F              ja          000000006101AA6D
  000000006101AA0E: 48 98              cdqe
  000000006101AA10: 41 8B 8C 84 2C AB  mov         ecx,dword ptr [r12+rax*4+000000000001AB2Ch]
                    01 00
  000000006101AA18: 49 03 CC           add         rcx,r12
  000000006101AA1B: FF E1              jmp         rcx
  000000006101AA1D: C7 87 5C 07 00 00  mov         dword ptr [rdi+000000000000075Ch],0
                    00 00 00 00
  000000006101AA27: C7 87 58 07 00 00  mov         dword ptr [rdi+0000000000000758h],0
                    00 00 00 00
  000000006101AA31: 48 8B CF           mov         rcx,rdi
  000000006101AA34: E8 07 C4 FF FF     call        0000000061016E40
  000000006101AA39: EB 32              jmp         000000006101AA6D
  000000006101AA3B: C7 87 C4 00 00 00  mov         dword ptr [rdi+00000000000000C4h],1
                    01 00 00 00
  000000006101AA45: 48 8B CF           mov         rcx,rdi
  000000006101AA48: E8 63 DF FF FF     call        00000000610189B0
  000000006101AA4D: 84 C0              test        al,al
  000000006101AA4F: 74 27              je          000000006101AA78
  000000006101AA51: EB 1A              jmp         000000006101AA6D
  000000006101AA53: 48 8B CF           mov         rcx,rdi
  000000006101AA56: E8 F5 5A FF FF     call        0000000061010550
  000000006101AA5B: 48 8B CF           mov         rcx,rdi
  000000006101AA5E: E8 DD C3 FF FF     call        0000000061016E40
  000000006101AA63: EB 08              jmp         000000006101AA6D
  000000006101AA65: 48 8B CF           mov         rcx,rdi
  000000006101AA68: E8 D3 C3 FF FF     call        0000000061016E40
  000000006101AA6D: 40 80 FE FF        cmp         sil,0FFh
  000000006101AA71: 74 64              je          000000006101AAD7
  000000006101AA73: E9 18 FE FF FF     jmp         000000006101A890
  000000006101AA78: 48 83 BF D8 00 00  cmp         qword ptr [rdi+00000000000000D8h],0
                    00 00
  000000006101AA80: 74 36              je          000000006101AAB8
  000000006101AA82: 48 8D 8F B8 00 00  lea         rcx,[rdi+00000000000000B8h]
                    00
  000000006101AA89: FF 15 E1 89 02 00  call        qword ptr [0000000061043470h]
  000000006101AA8F: 84 C0              test        al,al
  000000006101AA91: 74 25              je          000000006101AAB8
  000000006101AA93: 48 8D 15 46 FD FF  lea         rdx,[000000006101A7E0h]
                    FF
  000000006101AA9A: 48 89 54 24 48     mov         qword ptr [rsp+48h],rdx
  000000006101AA9F: 40 0F BE C6        movsx       eax,sil
  000000006101AAA3: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  000000006101AAA7: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006101AAAC: 48 8B 8F D8 00 00  mov         rcx,qword ptr [rdi+00000000000000D8h]
                    00
  000000006101AAB3: E8 38 8A FF FF     call        00000000610134F0
  000000006101AAB8: 32 C0              xor         al,al
  000000006101AABA: EB 63              jmp         000000006101AB1F
  000000006101AABC: 44 0F BE C6        movsx       r8d,sil
  000000006101AAC0: 48 8D 15 19 FD FF  lea         rdx,[000000006101A7E0h]
                    FF
  000000006101AAC7: 48 8B CF           mov         rcx,rdi
  000000006101AACA: E8 D1 B8 FF FF     call        00000000610163A0
  000000006101AACF: 32 C0              xor         al,al
  000000006101AAD1: EB 4C              jmp         000000006101AB1F
  000000006101AAD3: B0 01              mov         al,1
  000000006101AAD5: EB 48              jmp         000000006101AB1F
  000000006101AAD7: 48 8D 15 4A D0 02  lea         rdx,[0000000061047B28h]
                    00
  000000006101AADE: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006101AAE6: FF 15 AC 8D 02 00  call        qword ptr [0000000061043898h]
  000000006101AAEC: 48 8B D0           mov         rdx,rax
  000000006101AAEF: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006101AAF7: FF 15 C3 8C 02 00  call        qword ptr [00000000610437C0h]
  000000006101AAFD: 90                 nop
  000000006101AAFE: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  000000006101AB06: 48 8B CF           mov         rcx,rdi
  000000006101AB09: E8 42 93 FF FF     call        0000000061013E50
  000000006101AB0E: 90                 nop
  000000006101AB0F: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006101AB17: FF 15 C3 8D 02 00  call        qword ptr [00000000610438E0h]
  000000006101AB1D: 32 C0              xor         al,al
  000000006101AB1F: 48 83 C4 68        add         rsp,68h
  000000006101AB23: 41 5D              pop         r13
  000000006101AB25: 41 5C              pop         r12
  000000006101AB27: 5F                 pop         rdi
  000000006101AB28: 5E                 pop         rsi
  000000006101AB29: 5D                 pop         rbp
  000000006101AB2A: 5B                 pop         rbx
  000000006101AB2B: C3                 ret
  000000006101AB2C: 1D AA 01 00 3B     sbb         eax,3B0001AAh
  000000006101AB31: AA                 stos        byte ptr [rdi]
  000000006101AB32: 01 00              add         dword ptr [rax],eax
  000000006101AB34: 53                 push        rbx
  000000006101AB35: AA                 stos        byte ptr [rdi]
  000000006101AB36: 01 00              add         dword ptr [rax],eax
  000000006101AB38: 1D AA 01 00 3B     sbb         eax,3B0001AAh
  000000006101AB3D: AA                 stos        byte ptr [rdi]
  000000006101AB3E: 01 00              add         dword ptr [rax],eax
  000000006101AB40: 53                 push        rbx
  000000006101AB41: AA                 stos        byte ptr [rdi]
  000000006101AB42: 01 00              add         dword ptr [rax],eax
  000000006101AB44: 65 AA              stos        byte ptr [rdi]
  000000006101AB46: 01 00              add         dword ptr [rax],eax

  Summary

       42000 .text
