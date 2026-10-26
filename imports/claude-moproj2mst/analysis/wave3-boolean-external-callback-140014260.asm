Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140014260: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140014264: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140014269: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  0000000140014270: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140014279: 0F B6 84 24 A8 00  movzx       eax,byte ptr [rsp+00000000000000A8h]
                    00 00
  0000000140014281: 85 C0              test        eax,eax
  0000000140014283: 75 40              jne         00000001400142C5
  0000000140014285: 48 8D 0D 2C 24 D6  lea         rcx,[0000000143D766B8h]
                    03
  000000014001428C: E8 9F 75 2B 00     call        00000001402CB830
  0000000140014291: 0F B6 C0           movzx       eax,al
  0000000140014294: 85 C0              test        eax,eax
  0000000140014296: 74 2D              je          00000001400142C5
  0000000140014298: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400142A0: E8 1B FF FF FF     call        00000001400141C0
  00000001400142A5: 0F B6 C0           movzx       eax,al
  00000001400142A8: 85 C0              test        eax,eax
  00000001400142AA: 75 14              jne         00000001400142C0
  00000001400142AC: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400142B4: E8 B7 FE FF FF     call        0000000140014170
  00000001400142B9: 0F B6 C0           movzx       eax,al
  00000001400142BC: 85 C0              test        eax,eax
  00000001400142BE: 74 05              je          00000001400142C5
  00000001400142C0: E9 CB 01 00 00     jmp         0000000140014490
  00000001400142C5: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400142CD: 48 83 B8 F0 03 00  cmp         qword ptr [rax+00000000000003F0h],0
                    00 00
  00000001400142D5: 74 5E              je          0000000140014335
  00000001400142D7: 48 8D 0D DA 23 D6  lea         rcx,[0000000143D766B8h]
                    03
  00000001400142DE: E8 4D 75 2B 00     call        00000001402CB830
  00000001400142E3: 0F B6 C0           movzx       eax,al
  00000001400142E6: 85 C0              test        eax,eax
  00000001400142E8: 75 29              jne         0000000140014313
  00000001400142EA: 48 8D 0D C7 23 D6  lea         rcx,[0000000143D766B8h]
                    03
  00000001400142F1: E8 AA 74 2B 00     call        00000001402CB7A0
  00000001400142F6: 83 F8 01           cmp         eax,1
  00000001400142F9: 74 18              je          0000000140014313
  00000001400142FB: 33 D2              xor         edx,edx
  00000001400142FD: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140014305: 48 8B 89 F0 03 00  mov         rcx,qword ptr [rcx+00000000000003F0h]
                    00
  000000014001430C: E8 BF B8 CC 00     call        0000000140CDFBD0
  0000000140014311: EB 22              jmp         0000000140014335
  0000000140014313: 0F B6 84 24 A8 00  movzx       eax,byte ptr [rsp+00000000000000A8h]
                    00 00
  000000014001431B: 85 C0              test        eax,eax
  000000014001431D: 74 16              je          0000000140014335
  000000014001431F: 33 D2              xor         edx,edx
  0000000140014321: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140014329: 48 8B 89 F0 03 00  mov         rcx,qword ptr [rcx+00000000000003F0h]
                    00
  0000000140014330: E8 9B B8 CC 00     call        0000000140CDFBD0
  0000000140014335: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014001433D: E8 CE 91 00 00     call        000000014001D510
  0000000140014342: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140014347: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014001434D: 0F 84 3D 01 00 00  je          0000000140014490
  0000000140014353: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140014358: E8 33 30 09 00     call        00000001400A7390
  000000014001435D: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140014362: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140014368: 0F 84 22 01 00 00  je          0000000140014490
  000000014001436E: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140014373: E8 18 C0 FF FF     call        0000000140010390
  0000000140014378: 0F B6 C0           movzx       eax,al
  000000014001437B: 85 C0              test        eax,eax
  000000014001437D: 0F 85 0D 01 00 00  jne         0000000140014490
  0000000140014383: 48 8D 0D 2E 23 D6  lea         rcx,[0000000143D766B8h]
                    03
  000000014001438A: E8 61 3D 2B 00     call        00000001402C80F0
  000000014001438F: 48 85 C0           test        rax,rax
  0000000140014392: 74 14              je          00000001400143A8
  0000000140014394: 48 8D 0D 1D 23 D6  lea         rcx,[0000000143D766B8h]
                    03
  000000014001439B: E8 50 3D 2B 00     call        00000001402C80F0
  00000001400143A0: 48 8B C8           mov         rcx,rax
  00000001400143A3: E8 28 83 C7 00     call        0000000140C8C6D0
  00000001400143A8: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001400143AD: E8 1E 59 0A 00     call        00000001400B9CD0
  00000001400143B2: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400143B7: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001400143BD: 0F 84 CD 00 00 00  je          0000000140014490
  00000001400143C3: 41 B0 01           mov         r8b,1
  00000001400143C6: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000001400143CB: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400143D0: E8 8B 0B 09 00     call        00000001400A4F60
  00000001400143D5: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001400143DA: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400143DF: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001400143E4: 41 B0 01           mov         r8b,1
  00000001400143E7: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001400143EC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400143F1: E8 9A 8D CC 00     call        0000000140CDD190
  00000001400143F6: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001400143FB: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140014400: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140014405: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  000000014001440A: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  000000014001440F: FF 15 BB FC DE 01  call        qword ptr [0000000141E040D0h]
  0000000140014415: 88 44 24 38        mov         byte ptr [rsp+38h],al
  0000000140014419: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000014001441E: FF 15 BC 15 DF 01  call        qword ptr [0000000141E059E0h]
  0000000140014424: 90                 nop
  0000000140014425: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014001442A: FF 15 B0 15 DF 01  call        qword ptr [0000000141E059E0h]
  0000000140014430: 44 0F B6 5C 24 38  movzx       r11d,byte ptr [rsp+38h]
  0000000140014436: 45 85 DB           test        r11d,r11d
  0000000140014439: 74 55              je          0000000140014490
  000000014001443B: 41 B0 01           mov         r8b,1
  000000014001443E: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000140014443: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140014448: E8 13 0B 09 00     call        00000001400A4F60
  000000014001444D: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140014455: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014001445D: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140014465: 45 33 C0           xor         r8d,r8d
  0000000140014468: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140014470: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140014475: E8 46 89 CC 00     call        0000000140CDCDC0
  000000014001447A: 90                 nop
  000000014001447B: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140014480: FF 15 5A 15 DF 01  call        qword ptr [0000000141E059E0h]
  0000000140014486: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014001448B: E8 70 7F CC 00     call        0000000140CDC400
  0000000140014490: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140014498: E8 F3 00 00 00     call        0000000140014590
  000000014001449D: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400144A5: E8 06 09 00 00     call        0000000140014DB0
  00000001400144AA: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400144B2: E8 A9 0C 00 00     call        0000000140015160
  00000001400144B7: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400144BF: E8 DC 0C 00 00     call        00000001400151A0
  00000001400144C4: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400144CC: E8 1F 16 00 00     call        0000000140015AF0
  00000001400144D1: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400144D9: E8 82 16 00 00     call        0000000140015B60
  00000001400144DE: 48 8D 0D D3 21 D6  lea         rcx,[0000000143D766B8h]
                    03
  00000001400144E5: E8 D6 72 2B 00     call        00000001402CB7C0
  00000001400144EA: 0F B6 C0           movzx       eax,al
  00000001400144ED: 85 C0              test        eax,eax
  00000001400144EF: 74 0F              je          0000000140014500
  00000001400144F1: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400144F9: E8 F2 16 00 00     call        0000000140015BF0
  00000001400144FE: EB 42              jmp         0000000140014542
  0000000140014500: 48 8D 0D B1 21 D6  lea         rcx,[0000000143D766B8h]
                    03
  0000000140014507: E8 E4 72 2B 00     call        00000001402CB7F0
  000000014001450C: 0F B6 C0           movzx       eax,al
  000000014001450F: 85 C0              test        eax,eax
  0000000140014511: 74 0F              je          0000000140014522
  0000000140014513: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014001451B: E8 50 2D 00 00     call        0000000140017270
  0000000140014520: EB 20              jmp         0000000140014542
  0000000140014522: 48 8D 0D 8F 21 D6  lea         rcx,[0000000143D766B8h]
                    03
  0000000140014529: E8 02 73 2B 00     call        00000001402CB830
  000000014001452E: 0F B6 C0           movzx       eax,al
  0000000140014531: 85 C0              test        eax,eax
  0000000140014533: 74 0D              je          0000000140014542
  0000000140014535: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014001453D: E8 8E 2E 00 00     call        00000001400173D0
  0000000140014542: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014001454A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014001454D: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140014555: FF 90 78 07 00 00  call        qword ptr [rax+0000000000000778h]
  000000014001455B: 48 8B C8           mov         rcx,rax
  000000014001455E: E8 FD 0B CC 00     call        0000000140CD5160
  0000000140014563: 4C 8B 9C 24 A0 00  mov         r11,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014001456B: 41 C6 83 C0 03 00  mov         byte ptr [r11+00000000000003C0h],1
                    00 01
  0000000140014573: 48 81 C4 98 00 00  add         rsp,98h
                    00
  000000014001457A: C3                 ret

  Summary

     1E02000 .text
