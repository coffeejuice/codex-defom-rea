Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140358190: 44 88 4C 24 20     mov         byte ptr [rsp+20h],r9b
  0000000140358195: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000014035819A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014035819F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001403581A4: 48 81 EC C8 01 00  sub         rsp,1C8h
                    00
  00000001403581AB: 48 C7 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001403581B7: C7 84 24 E4 00 00  mov         dword ptr [rsp+00000000000000E4h],0
                    00 00 00 00 00
  00000001403581C2: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000001403581CB: 48 8D 15 AA 4A FC  lea         rdx,[000000014231CC7Ch]
                    01
  00000001403581D2: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001403581D7: FF 15 A3 C2 AA 01  call        qword ptr [0000000141E04480h]
  00000001403581DD: 90                 nop
  00000001403581DE: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000001403581E3: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000001403581EB: FF 15 47 C0 AA 01  call        qword ptr [0000000141E04238h]
  00000001403581F1: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  00000001403581F9: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140358201: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  0000000140358209: 41 B8 01 00 00 00  mov         r8d,1
  000000014035820F: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  0000000140358214: 48 8B 8C 24 F8 00  mov         rcx,qword ptr [rsp+00000000000000F8h]
                    00 00
  000000014035821C: FF 15 9E BD AA 01  call        qword ptr [0000000141E03FC0h]
  0000000140358222: 0F B6 C0           movzx       eax,al
  0000000140358225: 85 C0              test        eax,eax
  0000000140358227: 75 0D              jne         0000000140358236
  0000000140358229: C7 84 24 00 01 00  mov         dword ptr [rsp+0000000000000100h],1
                    00 01 00 00 00
  0000000140358234: EB 0B              jmp         0000000140358241
  0000000140358236: C7 84 24 00 01 00  mov         dword ptr [rsp+0000000000000100h],0
                    00 00 00 00 00
  0000000140358241: 0F B6 84 24 00 01  movzx       eax,byte ptr [rsp+0000000000000100h]
                    00 00
  0000000140358249: 88 44 24 4C        mov         byte ptr [rsp+4Ch],al
  000000014035824D: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140358252: FF 15 88 D7 AA 01  call        qword ptr [0000000141E059E0h]
  0000000140358258: 90                 nop
  0000000140358259: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  000000014035825E: FF 15 7C D7 AA 01  call        qword ptr [0000000141E059E0h]
  0000000140358264: 44 0F B6 5C 24 4C  movzx       r11d,byte ptr [rsp+4Ch]
  000000014035826A: 45 85 DB           test        r11d,r11d
  000000014035826D: 74 15              je          0000000140358284
  000000014035826F: 48 8D 15 0E 4A FC  lea         rdx,[000000014231CC84h]
                    01
  0000000140358276: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  000000014035827E: FF 15 44 C2 AA 01  call        qword ptr [0000000141E044C8h]
  0000000140358284: 48 8B 94 24 E0 01  mov         rdx,qword ptr [rsp+00000000000001E0h]
                    00 00
  000000014035828C: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140358291: FF 15 C9 BC AA 01  call        qword ptr [0000000141E03F60h]
  0000000140358297: 90                 nop
  0000000140358298: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  000000014035829D: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001403582A2: FF 15 C0 BC AA 01  call        qword ptr [0000000141E03F68h]
  00000001403582A8: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  00000001403582B0: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001403582B8: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  00000001403582C0: 48 8B 94 24 10 01  mov         rdx,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001403582C8: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000001403582D0: FF 15 9A C1 AA 01  call        qword ptr [0000000141E04470h]
  00000001403582D6: 90                 nop
  00000001403582D7: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001403582DC: FF 15 FE D6 AA 01  call        qword ptr [0000000141E059E0h]
  00000001403582E2: 48 8D 15 C3 10 FC  lea         rdx,[00000001423193ACh]
                    01
  00000001403582E9: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000001403582F1: FF 15 D1 C1 AA 01  call        qword ptr [0000000141E044C8h]
  00000001403582F7: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  00000001403582FC: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140358301: FF 15 41 BA AA 01  call        qword ptr [0000000141E03D48h]
  0000000140358307: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  000000014035830F: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140358317: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  000000014035831F: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140358327: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  000000014035832F: FF 15 03 C1 AA 01  call        qword ptr [0000000141E04438h]
  0000000140358335: 90                 nop
  0000000140358336: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000014035833B: FF 15 9F D6 AA 01  call        qword ptr [0000000141E059E0h]
  0000000140358341: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140358346: FF 15 74 C1 AA 01  call        qword ptr [0000000141E044C0h]
  000000014035834C: 90                 nop
  000000014035834D: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140358352: FF 15 F8 BE AA 01  call        qword ptr [0000000141E04250h]
  0000000140358358: 0F B7 10           movzx       edx,word ptr [rax]
  000000014035835B: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140358360: FF 15 EA CE AA 01  call        qword ptr [0000000141E05250h]
  0000000140358366: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  000000014035836B: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140358370: FF 15 0A BC AA 01  call        qword ptr [0000000141E03F80h]
  0000000140358376: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  000000014035837E: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  0000000140358386: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  000000014035838E: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  0000000140358393: 48 8B 94 24 30 01  mov         rdx,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014035839B: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000001403583A3: E8 48 C5 CA FF     call        00000001400048F0
  00000001403583A8: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  00000001403583B0: 48 8B 84 24 38 01  mov         rax,qword ptr [rsp+0000000000000138h]
                    00 00
  00000001403583B8: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  00000001403583C0: 4C 8B 84 24 E0 01  mov         r8,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000001403583C8: 48 8B 94 24 40 01  mov         rdx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001403583D0: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000001403583D8: E8 13 C5 CA FF     call        00000001400048F0
  00000001403583DD: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  00000001403583E5: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001403583ED: 48 89 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],rax
                    00 00
  00000001403583F5: 48 8B 94 24 50 01  mov         rdx,qword ptr [rsp+0000000000000150h]
                    00 00
  00000001403583FD: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140358405: FF 15 65 BB AA 01  call        qword ptr [0000000141E03F70h]
  000000014035840B: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  0000000140358413: 48 8B 84 24 58 01  mov         rax,qword ptr [rsp+0000000000000158h]
                    00 00
  000000014035841B: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  0000000140358423: 48 8B 94 24 60 01  mov         rdx,qword ptr [rsp+0000000000000160h]
                    00 00
  000000014035842B: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140358433: FF 15 37 C0 AA 01  call        qword ptr [0000000141E04470h]
  0000000140358439: 90                 nop
  000000014035843A: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140358442: FF 15 98 D5 AA 01  call        qword ptr [0000000141E059E0h]
  0000000140358448: 90                 nop
  0000000140358449: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140358451: FF 15 89 D5 AA 01  call        qword ptr [0000000141E059E0h]
  0000000140358457: 90                 nop
  0000000140358458: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140358460: FF 15 7A D5 AA 01  call        qword ptr [0000000141E059E0h]
  0000000140358466: 90                 nop
  0000000140358467: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000014035846C: FF 15 6E D5 AA 01  call        qword ptr [0000000141E059E0h]
  0000000140358472: 48 8B 94 24 E0 01  mov         rdx,qword ptr [rsp+00000000000001E0h]
                    00 00
  000000014035847A: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140358482: FF 15 50 BB AA 01  call        qword ptr [0000000141E03FD8h]
  0000000140358488: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  0000000140358490: 48 8B 84 24 68 01  mov         rax,qword ptr [rsp+0000000000000168h]
                    00 00
  0000000140358498: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  00000001403584A0: 8B 84 24 E4 00 00  mov         eax,dword ptr [rsp+00000000000000E4h]
                    00
  00000001403584A7: 83 C8 01           or          eax,1
  00000001403584AA: 89 84 24 E4 00 00  mov         dword ptr [rsp+00000000000000E4h],eax
                    00
  00000001403584B1: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  00000001403584B9: FF 15 89 BA AA 01  call        qword ptr [0000000141E03F48h]
  00000001403584BF: 0F B6 C0           movzx       eax,al
  00000001403584C2: 85 C0              test        eax,eax
  00000001403584C4: 74 19              je          00000001403584DF
  00000001403584C6: 0F B6 84 24 E8 01  movzx       eax,byte ptr [rsp+00000000000001E8h]
                    00 00
  00000001403584CE: 85 C0              test        eax,eax
  00000001403584D0: 75 0D              jne         00000001403584DF
  00000001403584D2: C7 84 24 78 01 00  mov         dword ptr [rsp+0000000000000178h],1
                    00 01 00 00 00
  00000001403584DD: EB 0B              jmp         00000001403584EA
  00000001403584DF: C7 84 24 78 01 00  mov         dword ptr [rsp+0000000000000178h],0
                    00 00 00 00 00
  00000001403584EA: 0F B6 84 24 78 01  movzx       eax,byte ptr [rsp+0000000000000178h]
                    00 00
  00000001403584F2: 88 84 24 98 00 00  mov         byte ptr [rsp+0000000000000098h],al
                    00
  00000001403584F9: 8B 84 24 E4 00 00  mov         eax,dword ptr [rsp+00000000000000E4h]
                    00
  0000000140358500: 83 E0 01           and         eax,1
  0000000140358503: 85 C0              test        eax,eax
  0000000140358505: 74 16              je          000000014035851D
  0000000140358507: 83 A4 24 E4 00 00  and         dword ptr [rsp+00000000000000E4h],0FFFFFFFEh
                    00 FE
  000000014035850F: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140358517: FF 15 03 BB AA 01  call        qword ptr [0000000141E04020h]
  000000014035851D: 0F B6 84 24 98 00  movzx       eax,byte ptr [rsp+0000000000000098h]
                    00 00
  0000000140358525: 85 C0              test        eax,eax
  0000000140358527: 0F 84 68 01 00 00  je          0000000140358695
  000000014035852D: B2 20              mov         dl,20h
  000000014035852F: 48 8D 8C 24 BA 00  lea         rcx,[rsp+00000000000000BAh]
                    00 00
  0000000140358537: E8 24 A3 CA FF     call        0000000140002860
  000000014035853C: 0F B6 10           movzx       edx,byte ptr [rax]
  000000014035853F: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  0000000140358547: FF 15 0B BE AA 01  call        qword ptr [0000000141E04358h]
  000000014035854D: 45 33 C0           xor         r8d,r8d
  0000000140358550: 48 8D 15 39 47 FC  lea         rdx,[000000014231CC90h]
                    01
  0000000140358557: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  000000014035855F: FF 15 DB BE AA 01  call        qword ptr [0000000141E04440h]
  0000000140358565: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  000000014035856D: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  0000000140358575: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  000000014035857D: 48 8D 84 24 B8 00  lea         rax,[rsp+00000000000000B8h]
                    00 00
  0000000140358585: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014035858A: 45 33 C9           xor         r9d,r9d
  000000014035858D: 4C 8B 84 24 E0 01  mov         r8,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140358595: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  000000014035859D: 48 8B 8C 24 88 01  mov         rcx,qword ptr [rsp+0000000000000188h]
                    00 00
  00000001403585A5: FF 15 B5 BD AA 01  call        qword ptr [0000000141E04360h]
  00000001403585AB: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  00000001403585B3: 48 8B 84 24 90 01  mov         rax,qword ptr [rsp+0000000000000190h]
                    00 00
  00000001403585BB: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  00000001403585C3: 45 33 C0           xor         r8d,r8d
  00000001403585C6: 48 8D 15 03 47 FC  lea         rdx,[000000014231CCD0h]
                    01
  00000001403585CD: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  00000001403585D5: FF 15 65 BE AA 01  call        qword ptr [0000000141E04440h]
  00000001403585DB: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  00000001403585E3: 48 8B 84 24 A0 01  mov         rax,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001403585EB: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  00000001403585F3: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  00000001403585FB: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],10000h
                    01 00
  0000000140358603: 41 B9 00 40 00 00  mov         r9d,4000h
  0000000140358609: 4C 8B 84 24 98 01  mov         r8,qword ptr [rsp+0000000000000198h]
                    00 00
  0000000140358611: 48 8B 94 24 A8 01  mov         rdx,qword ptr [rsp+00000000000001A8h]
                    00 00
  0000000140358619: 33 C9              xor         ecx,ecx
  000000014035861B: FF 15 5F 33 AB 01  call        qword ptr [0000000141E0B980h]
  0000000140358621: 89 44 24 48        mov         dword ptr [rsp+48h],eax
  0000000140358625: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  000000014035862D: FF 15 AD D3 AA 01  call        qword ptr [0000000141E059E0h]
  0000000140358633: 90                 nop
  0000000140358634: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  000000014035863C: FF 15 9E D3 AA 01  call        qword ptr [0000000141E059E0h]
  0000000140358642: 90                 nop
  0000000140358643: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  000000014035864B: FF 15 8F D3 AA 01  call        qword ptr [0000000141E059E0h]
  0000000140358651: 81 7C 24 48 00 00  cmp         dword ptr [rsp+48h],10000h
                    01 00
  0000000140358659: 75 3A              jne         0000000140358695
  000000014035865B: C6 84 24 D0 00 00  mov         byte ptr [rsp+00000000000000D0h],0
                    00 00
  0000000140358663: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140358668: FF 15 72 D3 AA 01  call        qword ptr [0000000141E059E0h]
  000000014035866E: 90                 nop
  000000014035866F: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140358674: FF 15 0E B9 AA 01  call        qword ptr [0000000141E03F88h]
  000000014035867A: 90                 nop
  000000014035867B: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140358683: FF 15 57 D3 AA 01  call        qword ptr [0000000141E059E0h]
  0000000140358689: 8A 84 24 D0 00 00  mov         al,byte ptr [rsp+00000000000000D0h]
                    00
  0000000140358690: E9 9C 00 00 00     jmp         0000000140358731
  0000000140358695: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  000000014035869D: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000001403586A5: FF 15 A5 B8 AA 01  call        qword ptr [0000000141E03F50h]
  00000001403586AB: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  00000001403586B3: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001403586BB: 48 89 84 24 B8 01  mov         qword ptr [rsp+00000000000001B8h],rax
                    00 00
  00000001403586C3: 48 8B 8C 24 B8 01  mov         rcx,qword ptr [rsp+00000000000001B8h]
                    00 00
  00000001403586CB: FF 15 6F C0 AA 01  call        qword ptr [0000000141E04740h]
  00000001403586D1: 48 8B D0           mov         rdx,rax
  00000001403586D4: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001403586DC: 44 8B 40 08        mov         r8d,dword ptr [rax+8]
  00000001403586E0: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  00000001403586E8: E8 53 E1 53 00     call        0000000140896840
  00000001403586ED: 90                 nop
  00000001403586EE: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  00000001403586F6: FF 15 94 BB AA 01  call        qword ptr [0000000141E04290h]
  00000001403586FC: C6 84 24 E0 00 00  mov         byte ptr [rsp+00000000000000E0h],1
                    00 01
  0000000140358704: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140358709: FF 15 D1 D2 AA 01  call        qword ptr [0000000141E059E0h]
  000000014035870F: 90                 nop
  0000000140358710: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140358715: FF 15 6D B8 AA 01  call        qword ptr [0000000141E03F88h]
  000000014035871B: 90                 nop
  000000014035871C: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140358724: FF 15 B6 D2 AA 01  call        qword ptr [0000000141E059E0h]
  000000014035872A: 8A 84 24 E0 00 00  mov         al,byte ptr [rsp+00000000000000E0h]
                    00
  0000000140358731: 48 81 C4 C8 01 00  add         rsp,1C8h
                    00
  0000000140358738: C3                 ret

  Summary

     1E02000 .text
