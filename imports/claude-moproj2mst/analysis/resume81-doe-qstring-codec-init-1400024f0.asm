Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400024F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400024F5: 53                 push        rbx
  00000001400024F6: 48 81 EC 20 01 00  sub         rsp,120h
                    00
  00000001400024FD: 48 C7 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140002509: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140002512: C7 44 24 38 FF FF  mov         dword ptr [rsp+38h],0FFFFFFFFh
                    FF FF
  000000014000251A: 8B 05 F8 71 D7 03  mov         eax,dword ptr [0000000143D79718h]
  0000000140002520: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140002524: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000140002528: 89 84 24 A8 00 00  mov         dword ptr [rsp+00000000000000A8h],eax
                    00
  000000014000252F: 83 BC 24 A8 00 00  cmp         dword ptr [rsp+00000000000000A8h],1
                    00 01
  0000000140002537: 74 2C              je          0000000140002565
  0000000140002539: 83 BC 24 A8 00 00  cmp         dword ptr [rsp+00000000000000A8h],2
                    00 02
  0000000140002541: 74 30              je          0000000140002573
  0000000140002543: 83 BC 24 A8 00 00  cmp         dword ptr [rsp+00000000000000A8h],3
                    00 03
  000000014000254B: 74 34              je          0000000140002581
  000000014000254D: 83 BC 24 A8 00 00  cmp         dword ptr [rsp+00000000000000A8h],4
                    00 04
  0000000140002555: 74 38              je          000000014000258F
  0000000140002557: 83 BC 24 A8 00 00  cmp         dword ptr [rsp+00000000000000A8h],7
                    00 07
  000000014000255F: 74 40              je          00000001400025A1
  0000000140002561: EB 4A              jmp         00000001400025AD
  0000000140002563: EB 48              jmp         00000001400025AD
  0000000140002565: 48 8D 05 AC BB 0E  lea         rax,[00000001420EE118h]
                    02
  000000014000256C: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140002571: EB 3A              jmp         00000001400025AD
  0000000140002573: 48 8D 05 96 BB 0E  lea         rax,[00000001420EE110h]
                    02
  000000014000257A: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014000257F: EB 2C              jmp         00000001400025AD
  0000000140002581: 48 8D 05 80 BB 0E  lea         rax,[00000001420EE108h]
                    02
  0000000140002588: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014000258D: EB 1E              jmp         00000001400025AD
  000000014000258F: 48 8D 05 6A BB 0E  lea         rax,[00000001420EE100h]
                    02
  0000000140002596: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014000259B: EB 10              jmp         00000001400025AD
  000000014000259D: EB 0E              jmp         00000001400025AD
  000000014000259F: EB 0C              jmp         00000001400025AD
  00000001400025A1: 48 8D 05 54 BB 0E  lea         rax,[00000001420EE0FCh]
                    02
  00000001400025A8: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400025AD: E8 5E 5C 2C 00     call        00000001402C8210
  00000001400025B2: 0F B6 C0           movzx       eax,al
  00000001400025B5: 85 C0              test        eax,eax
  00000001400025B7: 74 7C              je          0000000140002635
  00000001400025B9: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000001400025BE: FF 15 94 1E E0 01  call        qword ptr [0000000141E04458h]
  00000001400025C4: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000001400025CC: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400025D4: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  00000001400025DC: 4C 8D 05 0D BB 0E  lea         r8,[00000001420EE0F0h]
                    02
  00000001400025E3: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001400025EB: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001400025F0: E8 1B FB FF FF     call        0000000140002110
  00000001400025F5: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  00000001400025FD: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140002605: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  000000014000260D: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140002615: FF 15 45 1E E0 01  call        qword ptr [0000000141E04460h]
  000000014000261B: 90                 nop
  000000014000261C: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140002621: FF 15 B9 33 E0 01  call        qword ptr [0000000141E059E0h]
  0000000140002627: 90                 nop
  0000000140002628: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014000262D: FF 15 AD 33 E0 01  call        qword ptr [0000000141E059E0h]
  0000000140002633: EB 7A              jmp         00000001400026AF
  0000000140002635: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000014000263A: FF 15 18 1E E0 01  call        qword ptr [0000000141E04458h]
  0000000140002640: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  0000000140002648: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140002650: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  0000000140002658: 4C 8D 05 81 BA 0E  lea         r8,[00000001420EE0E0h]
                    02
  000000014000265F: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140002667: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014000266C: E8 9F FA FF FF     call        0000000140002110
  0000000140002671: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140002679: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140002681: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  0000000140002689: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140002691: FF 15 C9 1D E0 01  call        qword ptr [0000000141E04460h]
  0000000140002697: 90                 nop
  0000000140002698: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000014000269D: FF 15 3D 33 E0 01  call        qword ptr [0000000141E059E0h]
  00000001400026A3: 90                 nop
  00000001400026A4: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400026A9: FF 15 31 33 E0 01  call        qword ptr [0000000141E059E0h]
  00000001400026AF: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001400026B5: 0F 84 8D 01 00 00  je          0000000140002848
  00000001400026BB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400026C0: FF 15 72 1C E0 01  call        qword ptr [0000000141E04338h]
  00000001400026C6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400026CB: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001400026D1: 74 26              je          00000001400026F9
  00000001400026D3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400026D8: FF 15 62 1C E0 01  call        qword ptr [0000000141E04340h]
  00000001400026DE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400026E3: FF 15 5F 1C E0 01  call        qword ptr [0000000141E04348h]
  00000001400026E9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400026EE: FF 15 5C 1C E0 01  call        qword ptr [0000000141E04350h]
  00000001400026F4: E9 4F 01 00 00     jmp         0000000140002848
  00000001400026F9: 48 8D 44 24 68     lea         rax,[rsp+68h]
  00000001400026FE: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140002703: B2 20              mov         dl,20h
  0000000140002705: 48 8D 8C 24 82 00  lea         rcx,[rsp+0000000000000082h]
                    00 00
  000000014000270D: E8 4E 01 00 00     call        0000000140002860
  0000000140002712: 0F B6 10           movzx       edx,byte ptr [rax]
  0000000140002715: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000014000271D: FF 15 35 1C E0 01  call        qword ptr [0000000141E04358h]
  0000000140002723: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140002728: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140002730: FF 15 4A 1D E0 01  call        qword ptr [0000000141E04480h]
  0000000140002736: 90                 nop
  0000000140002737: BA 00 04 00 00     mov         edx,400h
  000000014000273C: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140002741: E8 1A 23 00 00     call        0000000140004A60
  0000000140002746: 48 8B D8           mov         rbx,rax
  0000000140002749: 45 33 C0           xor         r8d,r8d
  000000014000274C: 48 8D 15 5D B9 0E  lea         rdx,[00000001420EE0B0h]
                    02
  0000000140002753: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140002758: FF 15 DA 97 E0 01  call        qword ptr [0000000141E0BF38h]
  000000014000275E: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140002766: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  000000014000276E: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  0000000140002776: 48 8D 84 24 80 00  lea         rax,[rsp+0000000000000080h]
                    00 00
  000000014000277E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140002783: 45 33 C9           xor         r9d,r9d
  0000000140002786: 4C 8D 84 24 88 00  lea         r8,[rsp+0000000000000088h]
                    00 00
  000000014000278E: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140002796: 48 8B 8C 24 F8 00  mov         rcx,qword ptr [rsp+00000000000000F8h]
                    00 00
  000000014000279E: FF 15 BC 1B E0 01  call        qword ptr [0000000141E04360h]
  00000001400027A4: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  00000001400027AC: 48 8B 84 24 00 01  mov         rax,qword ptr [rsp+0000000000000100h]
                    00 00
  00000001400027B4: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  00000001400027BC: 45 33 C0           xor         r8d,r8d
  00000001400027BF: 48 8D 15 E2 B8 0E  lea         rdx,[00000001420EE0A8h]
                    02
  00000001400027C6: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001400027CE: FF 15 64 97 E0 01  call        qword ptr [0000000141E0BF38h]
  00000001400027D4: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  00000001400027DC: 48 8B 84 24 10 01  mov         rax,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001400027E4: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  00000001400027EC: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400027F4: 4C 8B CB           mov         r9,rbx
  00000001400027F7: 4C 8B 84 24 08 01  mov         r8,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400027FF: 48 8B 94 24 18 01  mov         rdx,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140002807: 33 C9              xor         ecx,ecx
  0000000140002809: FF 15 31 97 E0 01  call        qword ptr [0000000141E0BF40h]
  000000014000280F: 90                 nop
  0000000140002810: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140002818: FF 15 C2 31 E0 01  call        qword ptr [0000000141E059E0h]
  000000014000281E: 90                 nop
  000000014000281F: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140002827: FF 15 B3 31 E0 01  call        qword ptr [0000000141E059E0h]
  000000014000282D: 90                 nop
  000000014000282E: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140002833: FF 15 A7 31 E0 01  call        qword ptr [0000000141E059E0h]
  0000000140002839: 90                 nop
  000000014000283A: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140002842: FF 15 98 31 E0 01  call        qword ptr [0000000141E059E0h]
  0000000140002848: 48 81 C4 20 01 00  add         rsp,120h
                    00
  000000014000284F: 5B                 pop         rbx
  0000000140002850: C3                 ret

  Summary

     1E02000 .text
