Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D16530: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140D16535: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140D1653A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D1653F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D16544: 56                 push        rsi
  0000000140D16545: 57                 push        rdi
  0000000140D16546: 48 81 EC B8 01 00  sub         rsp,1B8h
                    00
  0000000140D1654D: C7 44 24 60 00 00  mov         dword ptr [rsp+60h],0
                    00 00
  0000000140D16555: 48 8B 94 24 D8 01  mov         rdx,qword ptr [rsp+00000000000001D8h]
                    00 00
  0000000140D1655D: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D16565: E8 86 F6 FF FF     call        0000000140D15BF0
  0000000140D1656A: 48 8B 94 24 D8 01  mov         rdx,qword ptr [rsp+00000000000001D8h]
                    00 00
  0000000140D16572: 44 8B 02           mov         r8d,dword ptr [rdx]
  0000000140D16575: 48 8B 94 24 00 02  mov         rdx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140D1657D: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D16585: E8 66 9D 01 00     call        0000000140D302F0
  0000000140D1658A: 48 8B 94 24 D0 01  mov         rdx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D16592: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  0000000140D1659A: 8B 00              mov         eax,dword ptr [rax]
  0000000140D1659C: 89 42 4C           mov         dword ptr [rdx+4Ch],eax
  0000000140D1659F: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D165A7: 48 8B 84 24 E0 01  mov         rax,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140D165AF: 8B 00              mov         eax,dword ptr [rax]
  0000000140D165B1: 89 41 50           mov         dword ptr [rcx+50h],eax
  0000000140D165B4: 48 8B 84 24 E8 01  mov         rax,qword ptr [rsp+00000000000001E8h]
                    00 00
  0000000140D165BC: 0F B6 00           movzx       eax,byte ptr [rax]
  0000000140D165BF: 85 C0              test        eax,eax
  0000000140D165C1: 0F 84 F7 01 00 00  je          0000000140D167BE
  0000000140D165C7: 45 33 C0           xor         r8d,r8d
  0000000140D165CA: 33 D2              xor         edx,edx
  0000000140D165CC: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140D165D1: FF 15 91 DB 0E 01  call        qword ptr [0000000141E04168h]
  0000000140D165D7: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  0000000140D165DF: 48 8B F0           mov         rsi,rax
  0000000140D165E2: B9 10 00 00 00     mov         ecx,10h
  0000000140D165E7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D165E9: 45 33 C0           xor         r8d,r8d
  0000000140D165EC: 33 D2              xor         edx,edx
  0000000140D165EE: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140D165F3: FF 15 6F DB 0E 01  call        qword ptr [0000000141E04168h]
  0000000140D165F9: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  0000000140D16601: 48 8B F0           mov         rsi,rax
  0000000140D16604: B9 10 00 00 00     mov         ecx,10h
  0000000140D16609: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D1660B: 45 33 C0           xor         r8d,r8d
  0000000140D1660E: 33 D2              xor         edx,edx
  0000000140D16610: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140D16618: FF 15 4A DB 0E 01  call        qword ptr [0000000141E04168h]
  0000000140D1661E: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  0000000140D16626: 48 8B F0           mov         rsi,rax
  0000000140D16629: B9 10 00 00 00     mov         ecx,10h
  0000000140D1662E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D16630: 45 33 C0           xor         r8d,r8d
  0000000140D16633: 33 D2              xor         edx,edx
  0000000140D16635: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140D1663D: FF 15 25 DB 0E 01  call        qword ptr [0000000141E04168h]
  0000000140D16643: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  0000000140D1664B: 48 8B F0           mov         rsi,rax
  0000000140D1664E: B9 10 00 00 00     mov         ecx,10h
  0000000140D16653: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D16655: 45 33 C0           xor         r8d,r8d
  0000000140D16658: 33 D2              xor         edx,edx
  0000000140D1665A: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140D16662: FF 15 00 DB 0E 01  call        qword ptr [0000000141E04168h]
  0000000140D16668: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  0000000140D16670: 48 8B F0           mov         rsi,rax
  0000000140D16673: B9 10 00 00 00     mov         ecx,10h
  0000000140D16678: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D1667A: 45 33 C0           xor         r8d,r8d
  0000000140D1667D: 33 D2              xor         edx,edx
  0000000140D1667F: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  0000000140D16687: FF 15 DB DA 0E 01  call        qword ptr [0000000141E04168h]
  0000000140D1668D: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  0000000140D16695: 48 8B F0           mov         rsi,rax
  0000000140D16698: B9 10 00 00 00     mov         ecx,10h
  0000000140D1669D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D1669F: 45 33 C0           xor         r8d,r8d
  0000000140D166A2: 33 D2              xor         edx,edx
  0000000140D166A4: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140D166AC: FF 15 B6 DA 0E 01  call        qword ptr [0000000141E04168h]
  0000000140D166B2: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  0000000140D166BA: 48 8B F0           mov         rsi,rax
  0000000140D166BD: B9 10 00 00 00     mov         ecx,10h
  0000000140D166C2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D166C4: 45 33 C0           xor         r8d,r8d
  0000000140D166C7: 33 D2              xor         edx,edx
  0000000140D166C9: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  0000000140D166D1: FF 15 91 DA 0E 01  call        qword ptr [0000000141E04168h]
  0000000140D166D7: 48 8D BC 24 30 01  lea         rdi,[rsp+0000000000000130h]
                    00 00
  0000000140D166DF: 48 8B F0           mov         rsi,rax
  0000000140D166E2: B9 10 00 00 00     mov         ecx,10h
  0000000140D166E7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D166E9: 45 33 C0           xor         r8d,r8d
  0000000140D166EC: 33 D2              xor         edx,edx
  0000000140D166EE: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  0000000140D166F6: FF 15 6C DA 0E 01  call        qword ptr [0000000141E04168h]
  0000000140D166FC: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  0000000140D16704: 48 8B F0           mov         rsi,rax
  0000000140D16707: B9 10 00 00 00     mov         ecx,10h
  0000000140D1670C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D1670E: 45 33 C0           xor         r8d,r8d
  0000000140D16711: 33 D2              xor         edx,edx
  0000000140D16713: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140D1671B: FF 15 47 DA 0E 01  call        qword ptr [0000000141E04168h]
  0000000140D16721: 48 8D BC 24 10 01  lea         rdi,[rsp+0000000000000110h]
                    00 00
  0000000140D16729: 48 8B F0           mov         rsi,rax
  0000000140D1672C: B9 10 00 00 00     mov         ecx,10h
  0000000140D16731: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D16733: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  0000000140D1673B: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140D16740: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000140D16748: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140D1674D: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  0000000140D16755: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140D1675A: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  0000000140D16762: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D16767: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  0000000140D1676F: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D16774: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  0000000140D1677C: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D16781: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  0000000140D16789: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D1678E: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  0000000140D16796: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D1679B: 4C 8D 8C 24 20 01  lea         r9,[rsp+0000000000000120h]
                    00 00
  0000000140D167A3: 4C 8D 84 24 10 01  lea         r8,[rsp+0000000000000110h]
                    00 00
  0000000140D167AB: 48 8D 15 B6 A9 CE  lea         rdx,[0000000142A01168h]
                    01
  0000000140D167B2: 48 8D 0D 47 FF 05  lea         rcx,[0000000143D76700h]
                    03
  0000000140D167B9: E8 02 8C 5B FF     call        00000001402CF3C0
  0000000140D167BE: 8B 94 24 F0 01 00  mov         edx,dword ptr [rsp+00000000000001F0h]
                    00
  0000000140D167C5: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D167CD: E8 0E 03 00 00     call        0000000140D16AE0
  0000000140D167D2: C6 84 24 08 01 00  mov         byte ptr [rsp+0000000000000108h],1
                    00 01
  0000000140D167DA: 48 8D 94 24 08 01  lea         rdx,[rsp+0000000000000108h]
                    00 00
  0000000140D167E2: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D167EA: E8 91 9C 01 00     call        0000000140D30480
  0000000140D167EF: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  0000000140D167F7: 81 38 20 4E 00 00  cmp         dword ptr [rax],4E20h
  0000000140D167FD: 75 14              jne         0000000140D16813
  0000000140D167FF: 8B 94 24 F8 01 00  mov         edx,dword ptr [rsp+00000000000001F8h]
                    00
  0000000140D16806: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D1680E: E8 5D 99 01 00     call        0000000140D30170
  0000000140D16813: 4C 8B 8C 24 E8 01  mov         r9,qword ptr [rsp+00000000000001E8h]
                    00 00
  0000000140D1681B: 4C 8B 84 24 E0 01  mov         r8,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140D16823: 48 8B 94 24 D8 01  mov         rdx,qword ptr [rsp+00000000000001D8h]
                    00 00
  0000000140D1682B: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D16833: E8 58 13 00 00     call        0000000140D17B90
  0000000140D16838: 45 33 C0           xor         r8d,r8d
  0000000140D1683B: 8B 94 24 F8 01 00  mov         edx,dword ptr [rsp+00000000000001F8h]
                    00
  0000000140D16842: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D1684A: E8 31 21 00 00     call        0000000140D18980
  0000000140D1684F: 48 8B 94 24 D8 01  mov         rdx,qword ptr [rsp+00000000000001D8h]
                    00 00
  0000000140D16857: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D1685F: E8 4C F7 FF FF     call        0000000140D15FB0
  0000000140D16864: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D1686C: E8 0F 02 00 00     call        0000000140D16A80
  0000000140D16871: 48 81 C4 B8 01 00  add         rsp,1B8h
                    00
  0000000140D16878: 5F                 pop         rdi
  0000000140D16879: 5E                 pop         rsi
  0000000140D1687A: C3                 ret

  Summary

     1E02000 .text
