Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CA5850: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CA5855: 53                 push        rbx
  0000000140CA5856: 48 81 EC A0 00 00  sub         rsp,0A0h
                    00
  0000000140CA585D: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140CA5869: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140CA586E: FF 15 FC 68 16 01  call        qword ptr [0000000141E0C170h]
  0000000140CA5874: 90                 nop
  0000000140CA5875: 41 B8 01 00 00 00  mov         r8d,1
  0000000140CA587B: BA 01 00 00 00     mov         edx,1
  0000000140CA5880: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140CA5885: FF 15 ED 68 16 01  call        qword ptr [0000000141E0C178h]
  0000000140CA588B: B2 01              mov         dl,1
  0000000140CA588D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140CA5892: FF 15 E8 68 16 01  call        qword ptr [0000000141E0C180h]
  0000000140CA5898: B9 28 00 00 00     mov         ecx,28h
  0000000140CA589D: E8 1A 92 7C 00     call        000000014146EABC
  0000000140CA58A2: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140CA58A7: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140CA58AD: 74 5F              je          0000000140CA590E
  0000000140CA58AF: 48 8D 44 24 50     lea         rax,[rsp+50h]
  0000000140CA58B4: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140CA58B9: 33 D2              xor         edx,edx
  0000000140CA58BB: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140CA58C0: E8 AB 13 8A FF     call        0000000140546C70
  0000000140CA58C5: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CA58CA: 45 33 C9           xor         r9d,r9d
  0000000140CA58CD: 45 33 C0           xor         r8d,r8d
  0000000140CA58D0: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140CA58D5: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140CA58DA: FF 15 D8 67 16 01  call        qword ptr [0000000141E0C0B8h]
  0000000140CA58E0: 4C 8B 5C 24 48     mov         r11,qword ptr [rsp+48h]
  0000000140CA58E5: 48 8D 05 7C 7E C6  lea         rax,[000000014290D768h]
                    01
  0000000140CA58EC: 49 89 03           mov         qword ptr [r11],rax
  0000000140CA58EF: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140CA58F4: 48 8D 05 2D 7E C6  lea         rax,[000000014290D728h]
                    01
  0000000140CA58FB: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000140CA58FF: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140CA5904: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140CA590C: EB 0C              jmp         0000000140CA591A
  0000000140CA590E: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00 00
  0000000140CA591A: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140CA5922: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140CA5927: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA592F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140CA5934: 48 89 81 E0 00 00  mov         qword ptr [rcx+00000000000000E0h],rax
                    00
  0000000140CA593B: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000140CA5940: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5948: 48 8B 89 E0 00 00  mov         rcx,qword ptr [rcx+00000000000000E0h]
                    00
  0000000140CA594F: FF 15 63 5A 16 01  call        qword ptr [0000000141E0B3B8h]
  0000000140CA5955: B9 C0 0C 00 00     mov         ecx,0CC0h
  0000000140CA595A: E8 5D 91 7C 00     call        000000014146EABC
  0000000140CA595F: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140CA5964: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140CA596A: 74 3A              je          0000000140CA59A6
  0000000140CA596C: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140CA5971: FF 15 B1 E5 15 01  call        qword ptr [0000000141E03F28h]
  0000000140CA5977: 8B D8              mov         ebx,eax
  0000000140CA5979: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140CA597E: FF 15 9C E5 15 01  call        qword ptr [0000000141E03F20h]
  0000000140CA5984: 44 8B CB           mov         r9d,ebx
  0000000140CA5987: 44 8B C0           mov         r8d,eax
  0000000140CA598A: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5992: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140CA5997: E8 E4 4C 02 00     call        0000000140CCA680
  0000000140CA599C: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140CA59A4: EB 0C              jmp         0000000140CA59B2
  0000000140CA59A6: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00 00
  0000000140CA59B2: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140CA59BA: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140CA59BF: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA59C7: 48 81 C1 20 01 00  add         rcx,120h
                    00
  0000000140CA59CE: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140CA59D3: E8 48 6F 01 00     call        0000000140CBC920
  0000000140CA59D8: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA59E0: 48 81 C1 20 01 00  add         rcx,120h
                    00
  0000000140CA59E7: E8 04 55 F7 FF     call        0000000140C1AEF0
  0000000140CA59EC: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA59F4: 48 8B 92 E0 00 00  mov         rdx,qword ptr [rdx+00000000000000E0h]
                    00
  0000000140CA59FB: 48 8B C8           mov         rcx,rax
  0000000140CA59FE: E8 8D 8C 02 00     call        0000000140CCE690
  0000000140CA5A03: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5A0B: 48 81 C1 20 01 00  add         rcx,120h
                    00
  0000000140CA5A12: E8 D9 54 F7 FF     call        0000000140C1AEF0
  0000000140CA5A17: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140CA5A1F: 4C 8D 0D 12 34 C6  lea         r9,[0000000142908E38h]
                    01
  0000000140CA5A26: 4C 8B C0           mov         r8,rax
  0000000140CA5A29: 48 8D 15 40 34 C6  lea         rdx,[0000000142908E70h]
                    01
  0000000140CA5A30: 48 8D 0D C9 0C 0D  lea         rcx,[0000000143D76700h]
                    03
  0000000140CA5A37: FF 15 93 E7 15 01  call        qword ptr [0000000141E041D0h]
  0000000140CA5A3D: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5A45: 48 81 C1 20 01 00  add         rcx,120h
                    00
  0000000140CA5A4C: E8 9F 54 F7 FF     call        0000000140C1AEF0
  0000000140CA5A51: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140CA5A59: 4C 8D 0D 48 34 C6  lea         r9,[0000000142908EA8h]
                    01
  0000000140CA5A60: 4C 8B C0           mov         r8,rax
  0000000140CA5A63: 48 8D 15 5E 34 C6  lea         rdx,[0000000142908EC8h]
                    01
  0000000140CA5A6A: 48 8D 0D 8F 0C 0D  lea         rcx,[0000000143D76700h]
                    03
  0000000140CA5A71: FF 15 59 E7 15 01  call        qword ptr [0000000141E041D0h]
  0000000140CA5A77: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5A7F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140CA5A82: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5A8A: FF 90 A8 07 00 00  call        qword ptr [rax+00000000000007A8h]
  0000000140CA5A90: B9 C0 0C 00 00     mov         ecx,0CC0h
  0000000140CA5A95: E8 22 90 7C 00     call        000000014146EABC
  0000000140CA5A9A: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140CA5A9F: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  0000000140CA5AA5: 74 3A              je          0000000140CA5AE1
  0000000140CA5AA7: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140CA5AAC: FF 15 76 E4 15 01  call        qword ptr [0000000141E03F28h]
  0000000140CA5AB2: 8B D8              mov         ebx,eax
  0000000140CA5AB4: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140CA5AB9: FF 15 61 E4 15 01  call        qword ptr [0000000141E03F20h]
  0000000140CA5ABF: 44 8B CB           mov         r9d,ebx
  0000000140CA5AC2: 44 8B C0           mov         r8d,eax
  0000000140CA5AC5: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5ACD: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140CA5AD2: E8 A9 4B 02 00     call        0000000140CCA680
  0000000140CA5AD7: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140CA5ADF: EB 0C              jmp         0000000140CA5AED
  0000000140CA5AE1: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00 00
  0000000140CA5AED: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140CA5AF5: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140CA5AFA: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5B02: 48 81 C1 28 01 00  add         rcx,128h
                    00
  0000000140CA5B09: 48 8B 54 24 70     mov         rdx,qword ptr [rsp+70h]
  0000000140CA5B0E: E8 0D 6E 01 00     call        0000000140CBC920
  0000000140CA5B13: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5B1B: 48 81 C1 28 01 00  add         rcx,128h
                    00
  0000000140CA5B22: E8 C9 53 F7 FF     call        0000000140C1AEF0
  0000000140CA5B27: C6 80 F8 01 00 00  mov         byte ptr [rax+00000000000001F8h],0
                    00
  0000000140CA5B2E: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5B36: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140CA5B39: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5B41: FF 90 B0 07 00 00  call        qword ptr [rax+00000000000007B0h]
  0000000140CA5B47: 4C 8B 9C 24 B0 00  mov         r11,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5B4F: 41 C6 83 30 01 00  mov         byte ptr [r11+0000000000000130h],1
                    00 01
  0000000140CA5B57: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140CA5B5C: 4C 8D 0D 8D 33 C6  lea         r9,[0000000142908EF0h]
                    01
  0000000140CA5B63: 4C 8B 84 24 B0 00  mov         r8,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5B6B: 48 8D 15 9E 33 C6  lea         rdx,[0000000142908F10h]
                    01
  0000000140CA5B72: 48 8D 0D 87 0B 0D  lea         rcx,[0000000143D76700h]
                    03
  0000000140CA5B79: E8 62 A0 62 FF     call        00000001402CFBE0
  0000000140CA5B7E: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140CA5B83: 4C 8D 0D A6 33 C6  lea         r9,[0000000142908F30h]
                    01
  0000000140CA5B8A: 4C 8B 84 24 B0 00  mov         r8,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5B92: 48 8D 15 B7 33 C6  lea         rdx,[0000000142908F50h]
                    01
  0000000140CA5B99: 48 8D 0D 60 0B 0D  lea         rcx,[0000000143D76700h]
                    03
  0000000140CA5BA0: E8 3B A0 62 FF     call        00000001402CFBE0
  0000000140CA5BA5: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140CA5BAA: 4C 8D 0D BF 33 C6  lea         r9,[0000000142908F70h]
                    01
  0000000140CA5BB1: 4C 8B 84 24 B0 00  mov         r8,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CA5BB9: 48 8D 15 D0 33 C6  lea         rdx,[0000000142908F90h]
                    01
  0000000140CA5BC0: 48 8D 0D 39 0B 0D  lea         rcx,[0000000143D76700h]
                    03
  0000000140CA5BC7: E8 14 A0 62 FF     call        00000001402CFBE0
  0000000140CA5BCC: 90                 nop
  0000000140CA5BCD: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140CA5BD2: FF 15 B0 64 16 01  call        qword ptr [0000000141E0C088h]
  0000000140CA5BD8: 48 81 C4 A0 00 00  add         rsp,0A0h
                    00
  0000000140CA5BDF: 5B                 pop         rbx
  0000000140CA5BE0: C3                 ret

  Summary

     1E02000 .text
