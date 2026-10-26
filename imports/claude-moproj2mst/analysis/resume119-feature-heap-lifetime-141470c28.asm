Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141470C28: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  0000000141470C2D: 57                 push        rdi
  0000000141470C2E: 48 83 EC 30        sub         rsp,30h
  0000000141470C32: B8 4D 5A 00 00     mov         eax,5A4Dh
  0000000141470C37: 66 39 05 C2 F3 B8  cmp         word ptr [0000000140000000h],ax
                    FE
  0000000141470C3E: 75 56              jne         0000000141470C96
  0000000141470C40: 48 63 05 F5 F3 B8  movsxd      rax,dword ptr [000000014000003Ch]
                    FE
  0000000141470C47: 48 8D 0D B2 F3 B8  lea         rcx,[0000000140000000h]
                    FE
  0000000141470C4E: 48 03 C1           add         rax,rcx
  0000000141470C51: 81 38 50 45 00 00  cmp         dword ptr [rax],4550h
  0000000141470C57: 74 08              je          0000000141470C61
  0000000141470C59: 33 DB              xor         ebx,ebx
  0000000141470C5B: 89 5C 24 40        mov         dword ptr [rsp+40h],ebx
  0000000141470C5F: EB 3B              jmp         0000000141470C9C
  0000000141470C61: B9 0B 02 00 00     mov         ecx,20Bh
  0000000141470C66: 66 39 48 18        cmp         word ptr [rax+18h],cx
  0000000141470C6A: 74 08              je          0000000141470C74
  0000000141470C6C: 33 DB              xor         ebx,ebx
  0000000141470C6E: 89 5C 24 40        mov         dword ptr [rsp+40h],ebx
  0000000141470C72: EB 28              jmp         0000000141470C9C
  0000000141470C74: 83 B8 84 00 00 00  cmp         dword ptr [rax+0000000000000084h],0Eh
                    0E
  0000000141470C7B: 77 08              ja          0000000141470C85
  0000000141470C7D: 33 DB              xor         ebx,ebx
  0000000141470C7F: 89 5C 24 40        mov         dword ptr [rsp+40h],ebx
  0000000141470C83: EB 17              jmp         0000000141470C9C
  0000000141470C85: 33 DB              xor         ebx,ebx
  0000000141470C87: 39 98 F8 00 00 00  cmp         dword ptr [rax+00000000000000F8h],ebx
  0000000141470C8D: 0F 95 C3           setne       bl
  0000000141470C90: 89 5C 24 40        mov         dword ptr [rsp+40h],ebx
  0000000141470C94: EB 06              jmp         0000000141470C9C
  0000000141470C96: 33 DB              xor         ebx,ebx
  0000000141470C98: 89 5C 24 40        mov         dword ptr [rsp+40h],ebx
  0000000141470C9C: BF 01 00 00 00     mov         edi,1
  0000000141470CA1: 8B CF              mov         ecx,edi
  0000000141470CA3: E8 58 3B 01 00     call        0000000141484800
  0000000141470CA8: 85 C0              test        eax,eax
  0000000141470CAA: 75 22              jne         0000000141470CCE
  0000000141470CAC: 83 3D A5 EE 28 03  cmp         dword ptr [00000001446FFB58h],2
                    02
  0000000141470CB3: 74 05              je          0000000141470CBA
  0000000141470CB5: E8 E6 DC 00 00     call        000000014147E9A0
  0000000141470CBA: B9 1C 00 00 00     mov         ecx,1Ch
  0000000141470CBF: E8 B4 DA 00 00     call        000000014147E778
  0000000141470CC4: B9 FF 00 00 00     mov         ecx,0FFh
  0000000141470CC9: E8 12 DA FF FF     call        000000014146E6E0
  0000000141470CCE: E8 35 0A 01 00     call        0000000141481708
  0000000141470CD3: 85 C0              test        eax,eax
  0000000141470CD5: 75 22              jne         0000000141470CF9
  0000000141470CD7: 83 3D 7A EE 28 03  cmp         dword ptr [00000001446FFB58h],2
                    02
  0000000141470CDE: 74 05              je          0000000141470CE5
  0000000141470CE0: E8 BB DC 00 00     call        000000014147E9A0
  0000000141470CE5: B9 10 00 00 00     mov         ecx,10h
  0000000141470CEA: E8 89 DA 00 00     call        000000014147E778
  0000000141470CEF: B9 FF 00 00 00     mov         ecx,0FFh
  0000000141470CF4: E8 E7 D9 FF FF     call        000000014146E6E0
  0000000141470CF9: E8 8E 0A 01 00     call        000000014148178C
  0000000141470CFE: 90                 nop
  0000000141470CFF: E8 28 E9 00 00     call        000000014147F62C
  0000000141470D04: 85 C0              test        eax,eax
  0000000141470D06: 79 0A              jns         0000000141470D12
  0000000141470D08: B9 1B 00 00 00     mov         ecx,1Bh
  0000000141470D0D: E8 62 D9 FF FF     call        000000014146E674
  0000000141470D12: FF 15 38 26 99 00  call        qword ptr [0000000141E03350h]
  0000000141470D18: 48 89 05 E1 35 BE  mov         qword ptr [0000000145054300h],rax
                    03
  0000000141470D1F: E8 28 39 01 00     call        000000014148464C
  0000000141470D24: 48 89 05 1D EE 28  mov         qword ptr [00000001446FFB48h],rax
                    03
  0000000141470D2B: E8 24 38 01 00     call        0000000141484554
  0000000141470D30: 85 C0              test        eax,eax
  0000000141470D32: 79 0A              jns         0000000141470D3E
  0000000141470D34: B9 08 00 00 00     mov         ecx,8
  0000000141470D39: E8 36 D9 FF FF     call        000000014146E674
  0000000141470D3E: E8 11 35 01 00     call        0000000141484254
  0000000141470D43: 85 C0              test        eax,eax
  0000000141470D45: 79 0A              jns         0000000141470D51
  0000000141470D47: B9 09 00 00 00     mov         ecx,9
  0000000141470D4C: E8 23 D9 FF FF     call        000000014146E674
  0000000141470D51: 8B CF              mov         ecx,edi
  0000000141470D53: E8 28 DA FF FF     call        000000014146E780
  0000000141470D58: 85 C0              test        eax,eax
  0000000141470D5A: 74 07              je          0000000141470D63
  0000000141470D5C: 8B C8              mov         ecx,eax
  0000000141470D5E: E8 11 D9 FF FF     call        000000014146E674
  0000000141470D63: 4C 8B 05 76 ED 28  mov         r8,qword ptr [00000001446FFAE0h]
                    03
  0000000141470D6A: 4C 89 05 77 ED 28  mov         qword ptr [00000001446FFAE8h],r8
                    03
  0000000141470D71: 48 8B 15 58 ED 28  mov         rdx,qword ptr [00000001446FFAD0h]
                    03
  0000000141470D78: 8B 0D 4E ED 28 03  mov         ecx,dword ptr [00000001446FFACCh]
  0000000141470D7E: E8 8D 06 B9 FE     call        0000000140001410
  0000000141470D83: 8B F8              mov         edi,eax
  0000000141470D85: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000141470D89: 85 DB              test        ebx,ebx
  0000000141470D8B: 75 07              jne         0000000141470D94
  0000000141470D8D: 8B C8              mov         ecx,eax
  0000000141470D8F: E8 28 DC FF FF     call        000000014146E9BC
  0000000141470D94: E8 3B DC FF FF     call        000000014146E9D4
  0000000141470D99: EB 17              jmp         0000000141470DB2
  0000000141470D9B: 8B F8              mov         edi,eax
  0000000141470D9D: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  0000000141470DA2: 75 08              jne         0000000141470DAC
  0000000141470DA4: 8B C8              mov         ecx,eax
  0000000141470DA6: E8 1D DC FF FF     call        000000014146E9C8
  0000000141470DAB: CC                 int         3
  0000000141470DAC: E8 33 DC FF FF     call        000000014146E9E4
  0000000141470DB1: 90                 nop
  0000000141470DB2: 8B C7              mov         eax,edi
  0000000141470DB4: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000141470DB9: 48 83 C4 30        add         rsp,30h
  0000000141470DBD: 5F                 pop         rdi
  0000000141470DBE: C3                 ret

  Summary

     1E02000 .text
