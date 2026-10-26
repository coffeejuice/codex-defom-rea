Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650688F0: 40 56              push        rsi
  00000000650688F2: 57                 push        rdi
  00000000650688F3: 41 54              push        r12
  00000000650688F5: 48 83 EC 40        sub         rsp,40h
  00000000650688F9: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065068902: 48 89 5C 24 68     mov         qword ptr [rsp+68h],rbx
  0000000065068907: 48 89 6C 24 70     mov         qword ptr [rsp+70h],rbp
  000000006506890C: 48 8B F9           mov         rdi,rcx
  000000006506890F: 4C 8B 61 08        mov         r12,qword ptr [rcx+8]
  0000000065068913: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  0000000065068917: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006506891B: A8 01              test        al,1
  000000006506891D: 0F 85 D3 00 00 00  jne         00000000650689F6
  0000000065068923: FF 15 3F DF 62 00  call        qword ptr [0000000065696868h]
  0000000065068929: 48 8B F0           mov         rsi,rax
  000000006506892C: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000065068930: 48 83 C1 18        add         rcx,18h
  0000000065068934: FF 15 7E DF 62 00  call        qword ptr [00000000656968B8h]
  000000006506893A: 83 F8 02           cmp         eax,2
  000000006506893D: 0F 8C F8 00 00 00  jl          0000000065068A3B
  0000000065068943: 48 8B 6E 08        mov         rbp,qword ptr [rsi+8]
  0000000065068947: 48 83 C5 18        add         rbp,18h
  000000006506894B: 48 8B CD           mov         rcx,rbp
  000000006506894E: FF 15 64 DF 62 00  call        qword ptr [00000000656968B8h]
  0000000065068954: 85 C0              test        eax,eax
  0000000065068956: 7E 4E              jle         00000000650689A6
  0000000065068958: 83 CA FF           or          edx,0FFFFFFFFh
  000000006506895B: 48 8B CD           mov         rcx,rbp
  000000006506895E: FF 15 DC DF 62 00  call        qword ptr [0000000065696940h]
  0000000065068964: 48 8B D8           mov         rbx,rax
  0000000065068967: 48 8B CD           mov         rcx,rbp
  000000006506896A: FF 15 E0 DF 62 00  call        qword ptr [0000000065696950h]
  0000000065068970: 48 83 C3 08        add         rbx,8
  0000000065068974: 48 3B D8           cmp         rbx,rax
  0000000065068977: 74 2D              je          00000000650689A6
  0000000065068979: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  0000000065068980: 48 39 3B           cmp         qword ptr [rbx],rdi
  0000000065068983: 74 0B              je          0000000065068990
  0000000065068985: 48 83 C3 08        add         rbx,8
  0000000065068989: 48 3B D8           cmp         rbx,rax
  000000006506898C: 75 F2              jne         0000000065068980
  000000006506898E: EB 16              jmp         00000000650689A6
  0000000065068990: 48 8B CD           mov         rcx,rbp
  0000000065068993: FF 15 AF DF 62 00  call        qword ptr [0000000065696948h]
  0000000065068999: 48 2B D8           sub         rbx,rax
  000000006506899C: 48 C1 FB 03        sar         rbx,3
  00000000650689A0: 85 DB              test        ebx,ebx
  00000000650689A2: 74 17              je          00000000650689BB
  00000000650689A4: EB 03              jmp         00000000650689A9
  00000000650689A6: 83 CB FF           or          ebx,0FFFFFFFFh
  00000000650689A9: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  00000000650689AD: 48 83 C1 18        add         rcx,18h
  00000000650689B1: 45 33 C0           xor         r8d,r8d
  00000000650689B4: 8B D3              mov         edx,ebx
  00000000650689B6: E8 65 8B 49 00     call        0000000065501520
  00000000650689BB: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  00000000650689BF: 8B 88 C8 01 00 00  mov         ecx,dword ptr [rax+00000000000001C8h]
  00000000650689C5: C1 E9 1C           shr         ecx,1Ch
  00000000650689C8: 80 E1 01           and         cl,1
  00000000650689CB: 75 25              jne         00000000650689F2
  00000000650689CD: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  00000000650689D1: 8B 88 C8 01 00 00  mov         ecx,dword ptr [rax+00000000000001C8h]
  00000000650689D7: C1 E9 1C           shr         ecx,1Ch
  00000000650689DA: 80 E1 01           and         cl,1
  00000000650689DD: 74 13              je          00000000650689F2
  00000000650689DF: 41 B1 01           mov         r9b,1
  00000000650689E2: 45 0F B6 C1        movzx       r8d,r9b
  00000000650689E6: 33 D2              xor         edx,edx
  00000000650689E8: 48 8B CF           mov         rcx,rdi
  00000000650689EB: E8 10 EF FF FF     call        0000000065067900
  00000000650689F0: EB 04              jmp         00000000650689F6
  00000000650689F2: 85 DB              test        ebx,ebx
  00000000650689F4: 74 45              je          0000000065068A3B
  00000000650689F6: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  00000000650689FA: 8B 90 C8 01 00 00  mov         edx,dword ptr [rax+00000000000001C8h]
  0000000065068A00: C1 EA 1C           shr         edx,1Ch
  0000000065068A03: 80 E2 01           and         dl,1
  0000000065068A06: 74 08              je          0000000065068A10
  0000000065068A08: 49 8B CC           mov         rcx,r12
  0000000065068A0B: E8 40 E6 02 00     call        0000000065097050
  0000000065068A10: BA 7E 00 00 00     mov         edx,7Eh
  0000000065068A15: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065068A1A: FF 15 F8 DD 62 00  call        qword ptr [0000000065696818h]
  0000000065068A20: 90                 nop
  0000000065068A21: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000065068A26: 48 8B CF           mov         rcx,rdi
  0000000065068A29: FF 15 E1 DD 62 00  call        qword ptr [0000000065696810h]
  0000000065068A2F: 90                 nop
  0000000065068A30: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065068A35: FF 15 CD DD 62 00  call        qword ptr [0000000065696808h]
  0000000065068A3B: 48 8B 5C 24 68     mov         rbx,qword ptr [rsp+68h]
  0000000065068A40: 48 8B 6C 24 70     mov         rbp,qword ptr [rsp+70h]
  0000000065068A45: 48 83 C4 40        add         rsp,40h
  0000000065068A49: 41 5C              pop         r12
  0000000065068A4B: 5F                 pop         rdi
  0000000065068A4C: 5E                 pop         rsi
  0000000065068A4D: C3                 ret

  Summary

      695000 .text
