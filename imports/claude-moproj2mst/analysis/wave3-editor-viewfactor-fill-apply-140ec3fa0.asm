Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EC3FA0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140EC3FA5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EC3FAA: 48 83 EC 58        sub         rsp,58h
  0000000140EC3FAE: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140EC3FB7: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC3FBC: FF 15 FE 04 F4 00  call        qword ptr [0000000141E044C0h]
  0000000140EC3FC2: 90                 nop
  0000000140EC3FC3: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC3FC8: E8 D3 CE 26 FF     call        0000000140130EA0
  0000000140EC3FCD: 0F B6 C0           movzx       eax,al
  0000000140EC3FD0: 85 C0              test        eax,eax
  0000000140EC3FD2: 74 1B              je          0000000140EC3FEF
  0000000140EC3FD4: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC3FD9: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC3FE0: 48 8B 89 A0 0D 00  mov         rcx,qword ptr [rcx+0000000000000DA0h]
                    00
  0000000140EC3FE7: FF 15 C3 7B F4 00  call        qword ptr [0000000141E0BBB0h]
  0000000140EC3FED: EB 19              jmp         0000000140EC4008
  0000000140EC3FEF: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC3FF4: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC3FFB: 48 8B 89 A0 0D 00  mov         rcx,qword ptr [rcx+0000000000000DA0h]
                    00
  0000000140EC4002: FF 15 A8 7F F4 00  call        qword ptr [0000000141E0BFB0h]
  0000000140EC4008: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC400D: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4014: B2 01              mov         dl,1
  0000000140EC4016: 48 8B 89 D0 0C 00  mov         rcx,qword ptr [rcx+0000000000000CD0h]
                    00
  0000000140EC401D: FF 15 1D 7A F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC4023: 4C 8B 5C 24 68     mov         r11,qword ptr [rsp+68h]
  0000000140EC4028: F2 41 0F 10 93 D0  movsd       xmm2,mmword ptr [r11+00000000000003D0h]
                    03 00 00
  0000000140EC4031: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EC4036: 48 8D 15 33 0C C5  lea         rdx,[0000000142B14C70h]
                    01
  0000000140EC403D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC4042: FF 15 D0 FE F3 00  call        qword ptr [0000000141E03F18h]
  0000000140EC4048: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC404D: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4054: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140EC4059: 48 8B 89 00 0D 00  mov         rcx,qword ptr [rcx+0000000000000D00h]
                    00
  0000000140EC4060: FF 15 52 26 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EC4066: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC406B: 83 B8 CC 03 00 00  cmp         dword ptr [rax+00000000000003CCh],0
                    00
  0000000140EC4072: 75 0A              jne         0000000140EC407E
  0000000140EC4074: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EC407C: EB 08              jmp         0000000140EC4086
  0000000140EC407E: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140EC4086: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140EC408A: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC408F: E8 BC 8A 00 00     call        0000000140ECCB50
  0000000140EC4094: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4099: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC40A0: B2 01              mov         dl,1
  0000000140EC40A2: 48 8B 89 38 0D 00  mov         rcx,qword ptr [rcx+0000000000000D38h]
                    00
  0000000140EC40A9: FF 15 91 79 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC40AF: 4C 8B 5C 24 68     mov         r11,qword ptr [rsp+68h]
  0000000140EC40B4: F2 41 0F 10 93 C8  movsd       xmm2,mmword ptr [r11+00000000000004C8h]
                    04 00 00
  0000000140EC40BD: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EC40C2: 48 8D 15 AF 0B C5  lea         rdx,[0000000142B14C78h]
                    01
  0000000140EC40C9: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC40CE: FF 15 44 FE F3 00  call        qword ptr [0000000141E03F18h]
  0000000140EC40D4: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC40D9: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC40E0: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140EC40E5: 48 8B 89 60 0D 00  mov         rcx,qword ptr [rcx+0000000000000D60h]
                    00
  0000000140EC40EC: FF 15 C6 25 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EC40F2: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC40F7: 83 B8 C4 04 00 00  cmp         dword ptr [rax+00000000000004C4h],0
                    00
  0000000140EC40FE: 75 0A              jne         0000000140EC410A
  0000000140EC4100: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EC4108: EB 08              jmp         0000000140EC4112
  0000000140EC410A: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140EC4112: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140EC4116: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC411B: E8 70 8B 00 00     call        0000000140ECCC90
  0000000140EC4120: E8 EB 27 42 00     call        00000001412E6910
  0000000140EC4125: 85 C0              test        eax,eax
  0000000140EC4127: 0F 84 8E 00 00 00  je          0000000140EC41BB
  0000000140EC412D: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4132: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4139: B2 01              mov         dl,1
  0000000140EC413B: 48 8B 89 D8 0D 00  mov         rcx,qword ptr [rcx+0000000000000DD8h]
                    00
  0000000140EC4142: FF 15 F8 78 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC4148: 4C 8B 5C 24 68     mov         r11,qword ptr [rsp+68h]
  0000000140EC414D: F2 41 0F 10 93 40  movsd       xmm2,mmword ptr [r11+0000000000000540h]
                    05 00 00
  0000000140EC4156: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EC415B: 48 8D 15 1E 0B C5  lea         rdx,[0000000142B14C80h]
                    01
  0000000140EC4162: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC4167: FF 15 AB FD F3 00  call        qword ptr [0000000141E03F18h]
  0000000140EC416D: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4172: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4179: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140EC417E: 48 8B 89 18 0E 00  mov         rcx,qword ptr [rcx+0000000000000E18h]
                    00
  0000000140EC4185: FF 15 2D 25 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EC418B: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC4190: 83 B8 3C 05 00 00  cmp         dword ptr [rax+000000000000053Ch],0
                    00
  0000000140EC4197: 75 0A              jne         0000000140EC41A3
  0000000140EC4199: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EC41A1: EB 08              jmp         0000000140EC41AB
  0000000140EC41A3: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140EC41AB: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140EC41AF: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC41B4: E8 17 8C 00 00     call        0000000140ECCDD0
  0000000140EC41B9: EB 26              jmp         0000000140EC41E1
  0000000140EC41BB: 33 C9              xor         ecx,ecx
  0000000140EC41BD: E8 CE 6C 43 00     call        00000001412FAE90
  0000000140EC41C2: 85 C0              test        eax,eax
  0000000140EC41C4: 75 1B              jne         0000000140EC41E1
  0000000140EC41C6: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC41CB: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC41D2: 33 D2              xor         edx,edx
  0000000140EC41D4: 48 8B 89 D8 0D 00  mov         rcx,qword ptr [rcx+0000000000000DD8h]
                    00
  0000000140EC41DB: FF 15 5F 78 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC41E1: E8 2A 27 42 00     call        00000001412E6910
  0000000140EC41E6: 85 C0              test        eax,eax
  0000000140EC41E8: 0F 84 CC 00 00 00  je          0000000140EC42BA
  0000000140EC41EE: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC41F3: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC41FA: B2 01              mov         dl,1
  0000000140EC41FC: 48 8B 89 40 0E 00  mov         rcx,qword ptr [rcx+0000000000000E40h]
                    00
  0000000140EC4203: FF 15 37 78 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC4209: 4C 8B 5C 24 68     mov         r11,qword ptr [rsp+68h]
  0000000140EC420E: F2 41 0F 10 93 B8  movsd       xmm2,mmword ptr [r11+00000000000005B8h]
                    05 00 00
  0000000140EC4217: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EC421C: 48 8D 15 65 0A C5  lea         rdx,[0000000142B14C88h]
                    01
  0000000140EC4223: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC4228: FF 15 EA FC F3 00  call        qword ptr [0000000141E03F18h]
  0000000140EC422E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4233: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC423A: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140EC423F: 48 8B 89 60 0E 00  mov         rcx,qword ptr [rcx+0000000000000E60h]
                    00
  0000000140EC4246: FF 15 6C 24 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EC424C: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC4251: 83 B8 C0 05 00 00  cmp         dword ptr [rax+00000000000005C0h],3
                    03
  0000000140EC4258: 75 0A              jne         0000000140EC4264
  0000000140EC425A: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140EC4262: EB 46              jmp         0000000140EC42AA
  0000000140EC4264: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC4269: 83 B8 C0 05 00 00  cmp         dword ptr [rax+00000000000005C0h],0Ch
                    0C
  0000000140EC4270: 75 0A              jne         0000000140EC427C
  0000000140EC4272: C7 44 24 20 02 00  mov         dword ptr [rsp+20h],2
                    00 00
  0000000140EC427A: EB 2E              jmp         0000000140EC42AA
  0000000140EC427C: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC4281: 83 B8 C0 05 00 00  cmp         dword ptr [rax+00000000000005C0h],1
                    01
  0000000140EC4288: 75 0A              jne         0000000140EC4294
  0000000140EC428A: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140EC4292: EB 16              jmp         0000000140EC42AA
  0000000140EC4294: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC4299: 83 B8 B4 05 00 00  cmp         dword ptr [rax+00000000000005B4h],0
                    00
  0000000140EC42A0: 75 08              jne         0000000140EC42AA
  0000000140EC42A2: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EC42AA: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140EC42AE: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC42B3: E8 58 8C 00 00     call        0000000140ECCF10
  0000000140EC42B8: EB 26              jmp         0000000140EC42E0
  0000000140EC42BA: 33 C9              xor         ecx,ecx
  0000000140EC42BC: E8 CF 6B 43 00     call        00000001412FAE90
  0000000140EC42C1: 85 C0              test        eax,eax
  0000000140EC42C3: 75 1B              jne         0000000140EC42E0
  0000000140EC42C5: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC42CA: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC42D1: 33 D2              xor         edx,edx
  0000000140EC42D3: 48 8B 89 40 0E 00  mov         rcx,qword ptr [rcx+0000000000000E40h]
                    00
  0000000140EC42DA: FF 15 60 77 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC42E0: 0F B6 05 9F A2 82  movzx       eax,byte ptr [00000001446EE586h]
                    03
  0000000140EC42E7: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140EC42EB: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  0000000140EC42F0: 0F 84 96 00 00 00  je          0000000140EC438C
  0000000140EC42F6: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC42FB: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4302: B2 01              mov         dl,1
  0000000140EC4304: 48 8B 89 D0 0E 00  mov         rcx,qword ptr [rcx+0000000000000ED0h]
                    00
  0000000140EC430B: FF 15 2F 77 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC4311: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4316: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC431D: B2 01              mov         dl,1
  0000000140EC431F: 48 8B 89 C8 0E 00  mov         rcx,qword ptr [rcx+0000000000000EC8h]
                    00
  0000000140EC4326: FF 15 14 77 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC432C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4331: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4338: B2 01              mov         dl,1
  0000000140EC433A: 48 8B 89 D8 0E 00  mov         rcx,qword ptr [rcx+0000000000000ED8h]
                    00
  0000000140EC4341: FF 15 F9 76 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC4347: 4C 8B 5C 24 68     mov         r11,qword ptr [rsp+68h]
  0000000140EC434C: F2 41 0F 10 93 28  movsd       xmm2,mmword ptr [r11+0000000000000628h]
                    06 00 00
  0000000140EC4355: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EC435A: 48 8D 15 2F 09 C5  lea         rdx,[0000000142B14C90h]
                    01
  0000000140EC4361: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC4366: FF 15 AC FB F3 00  call        qword ptr [0000000141E03F18h]
  0000000140EC436C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4371: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4378: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140EC437D: 48 8B 89 D0 0E 00  mov         rcx,qword ptr [rcx+0000000000000ED0h]
                    00
  0000000140EC4384: FF 15 2E 23 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EC438A: EB 51              jmp         0000000140EC43DD
  0000000140EC438C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4391: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4398: 33 D2              xor         edx,edx
  0000000140EC439A: 48 8B 89 D0 0E 00  mov         rcx,qword ptr [rcx+0000000000000ED0h]
                    00
  0000000140EC43A1: FF 15 99 76 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC43A7: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC43AC: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC43B3: 33 D2              xor         edx,edx
  0000000140EC43B5: 48 8B 89 C8 0E 00  mov         rcx,qword ptr [rcx+0000000000000EC8h]
                    00
  0000000140EC43BC: FF 15 7E 76 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC43C2: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC43C7: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC43CE: 33 D2              xor         edx,edx
  0000000140EC43D0: 48 8B 89 D8 0E 00  mov         rcx,qword ptr [rcx+0000000000000ED8h]
                    00
  0000000140EC43D7: FF 15 63 76 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC43DD: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  0000000140EC43E2: 0F 84 96 00 00 00  je          0000000140EC447E
  0000000140EC43E8: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC43ED: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC43F4: B2 01              mov         dl,1
  0000000140EC43F6: 48 8B 89 E8 0E 00  mov         rcx,qword ptr [rcx+0000000000000EE8h]
                    00
  0000000140EC43FD: FF 15 3D 76 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC4403: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4408: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC440F: B2 01              mov         dl,1
  0000000140EC4411: 48 8B 89 E0 0E 00  mov         rcx,qword ptr [rcx+0000000000000EE0h]
                    00
  0000000140EC4418: FF 15 22 76 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC441E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4423: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC442A: B2 01              mov         dl,1
  0000000140EC442C: 48 8B 89 F0 0E 00  mov         rcx,qword ptr [rcx+0000000000000EF0h]
                    00
  0000000140EC4433: FF 15 07 76 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC4439: 4C 8B 5C 24 68     mov         r11,qword ptr [rsp+68h]
  0000000140EC443E: F2 41 0F 10 93 30  movsd       xmm2,mmword ptr [r11+0000000000000630h]
                    06 00 00
  0000000140EC4447: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EC444C: 48 8D 15 45 08 C5  lea         rdx,[0000000142B14C98h]
                    01
  0000000140EC4453: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC4458: FF 15 BA FA F3 00  call        qword ptr [0000000141E03F18h]
  0000000140EC445E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4463: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC446A: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140EC446F: 48 8B 89 E8 0E 00  mov         rcx,qword ptr [rcx+0000000000000EE8h]
                    00
  0000000140EC4476: FF 15 3C 22 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EC447C: EB 51              jmp         0000000140EC44CF
  0000000140EC447E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4483: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC448A: 33 D2              xor         edx,edx
  0000000140EC448C: 48 8B 89 E8 0E 00  mov         rcx,qword ptr [rcx+0000000000000EE8h]
                    00
  0000000140EC4493: FF 15 A7 75 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC4499: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC449E: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC44A5: 33 D2              xor         edx,edx
  0000000140EC44A7: 48 8B 89 E0 0E 00  mov         rcx,qword ptr [rcx+0000000000000EE0h]
                    00
  0000000140EC44AE: FF 15 8C 75 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC44B4: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC44B9: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC44C0: 33 D2              xor         edx,edx
  0000000140EC44C2: 48 8B 89 F0 0E 00  mov         rcx,qword ptr [rcx+0000000000000EF0h]
                    00
  0000000140EC44C9: FF 15 71 75 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC44CF: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC44D4: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC44DB: B2 01              mov         dl,1
  0000000140EC44DD: 48 8B 89 00 0F 00  mov         rcx,qword ptr [rcx+0000000000000F00h]
                    00
  0000000140EC44E4: FF 15 56 75 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC44EA: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC44EF: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC44F6: B2 01              mov         dl,1
  0000000140EC44F8: 48 8B 89 F8 0E 00  mov         rcx,qword ptr [rcx+0000000000000EF8h]
                    00
  0000000140EC44FF: FF 15 3B 75 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC4505: 4C 8B 5C 24 68     mov         r11,qword ptr [rsp+68h]
  0000000140EC450A: F2 41 0F 10 93 38  movsd       xmm2,mmword ptr [r11+0000000000000638h]
                    06 00 00
  0000000140EC4513: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EC4518: 48 8D 15 81 07 C5  lea         rdx,[0000000142B14CA0h]
                    01
  0000000140EC451F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC4524: FF 15 EE F9 F3 00  call        qword ptr [0000000141E03F18h]
  0000000140EC452A: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC452F: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4536: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140EC453B: 48 8B 89 00 0F 00  mov         rcx,qword ptr [rcx+0000000000000F00h]
                    00
  0000000140EC4542: FF 15 70 21 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EC4548: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  0000000140EC454D: 0F 84 ED 00 00 00  je          0000000140EC4640
  0000000140EC4553: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4558: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC455F: 48 8B 89 A8 00 00  mov         rcx,qword ptr [rcx+00000000000000A8h]
                    00
  0000000140EC4566: FF 15 FC 17 F4 00  call        qword ptr [0000000141E05D68h]
  0000000140EC456C: 0F B6 C0           movzx       eax,al
  0000000140EC456F: 85 C0              test        eax,eax
  0000000140EC4571: 0F 84 C9 00 00 00  je          0000000140EC4640
  0000000140EC4577: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140EC457C: 48 8B 92 40 01 00  mov         rdx,qword ptr [rdx+0000000000000140h]
                    00
  0000000140EC4583: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4588: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC458F: 48 8B 92 B8 0E 00  mov         rdx,qword ptr [rdx+0000000000000EB8h]
                    00
  0000000140EC4596: 48 8B 89 B8 0C 00  mov         rcx,qword ptr [rcx+0000000000000CB8h]
                    00
  0000000140EC459D: FF 15 ED 6D F4 00  call        qword ptr [0000000141E0B390h]
  0000000140EC45A3: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC45A8: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC45AF: 41 B0 01           mov         r8b,1
  0000000140EC45B2: 8B D0              mov         edx,eax
  0000000140EC45B4: 48 8B 89 B8 0C 00  mov         rcx,qword ptr [rcx+0000000000000CB8h]
                    00
  0000000140EC45BB: FF 15 D7 15 F4 00  call        qword ptr [0000000141E05B98h]
  0000000140EC45C1: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC45C6: E8 D5 C8 26 FF     call        0000000140130EA0
  0000000140EC45CB: 0F B6 C0           movzx       eax,al
  0000000140EC45CE: 85 C0              test        eax,eax
  0000000140EC45D0: 74 37              je          0000000140EC4609
  0000000140EC45D2: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC45D7: 83 B8 18 01 00 00  cmp         dword ptr [rax+0000000000000118h],1
                    01
  0000000140EC45DE: 7C 27              jl          0000000140EC4607
  0000000140EC45E0: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC45E5: 8B 90 18 01 00 00  mov         edx,dword ptr [rax+0000000000000118h]
  0000000140EC45EB: 83 EA 01           sub         edx,1
  0000000140EC45EE: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC45F3: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC45FA: 48 8B 89 B8 00 00  mov         rcx,qword ptr [rcx+00000000000000B8h]
                    00
  0000000140EC4601: FF 15 C1 6E F4 00  call        qword ptr [0000000141E0B4C8h]
  0000000140EC4607: EB 35              jmp         0000000140EC463E
  0000000140EC4609: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC460E: 83 B8 18 01 00 00  cmp         dword ptr [rax+0000000000000118h],1
                    01
  0000000140EC4615: 7C 27              jl          0000000140EC463E
  0000000140EC4617: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC461C: 8B 90 18 01 00 00  mov         edx,dword ptr [rax+0000000000000118h]
  0000000140EC4622: 83 EA 01           sub         edx,1
  0000000140EC4625: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC462A: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4631: 48 8B 89 B8 00 00  mov         rcx,qword ptr [rcx+00000000000000B8h]
                    00
  0000000140EC4638: FF 15 8A 6E F4 00  call        qword ptr [0000000141E0B4C8h]
  0000000140EC463E: EB 4A              jmp         0000000140EC468A
  0000000140EC4640: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140EC4645: 48 8B 92 40 01 00  mov         rdx,qword ptr [rdx+0000000000000140h]
                    00
  0000000140EC464C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4651: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4658: 48 8B 92 B8 0E 00  mov         rdx,qword ptr [rdx+0000000000000EB8h]
                    00
  0000000140EC465F: 48 8B 89 B8 0C 00  mov         rcx,qword ptr [rcx+0000000000000CB8h]
                    00
  0000000140EC4666: FF 15 24 6D F4 00  call        qword ptr [0000000141E0B390h]
  0000000140EC466C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4671: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4678: 45 33 C0           xor         r8d,r8d
  0000000140EC467B: 8B D0              mov         edx,eax
  0000000140EC467D: 48 8B 89 B8 0C 00  mov         rcx,qword ptr [rcx+0000000000000CB8h]
                    00
  0000000140EC4684: FF 15 0E 15 F4 00  call        qword ptr [0000000141E05B98h]
  0000000140EC468A: E8 C1 70 93 FF     call        00000001407FB750
  0000000140EC468F: 89 44 24 2C        mov         dword ptr [rsp+2Ch],eax
  0000000140EC4693: E8 B8 70 93 FF     call        00000001407FB750
  0000000140EC4698: 83 F8 02           cmp         eax,2
  0000000140EC469B: 75 07              jne         0000000140EC46A4
  0000000140EC469D: C6 44 24 40 01     mov         byte ptr [rsp+40h],1
  0000000140EC46A2: EB 05              jmp         0000000140EC46A9
  0000000140EC46A4: C6 44 24 40 00     mov         byte ptr [rsp+40h],0
  0000000140EC46A9: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140EC46AE: 48 8B 92 40 01 00  mov         rdx,qword ptr [rdx+0000000000000140h]
                    00
  0000000140EC46B5: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC46BA: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC46C1: 48 8B 92 10 0F 00  mov         rdx,qword ptr [rdx+0000000000000F10h]
                    00
  0000000140EC46C8: 48 8B 89 B8 0C 00  mov         rcx,qword ptr [rcx+0000000000000CB8h]
                    00
  0000000140EC46CF: FF 15 BB 6C F4 00  call        qword ptr [0000000141E0B390h]
  0000000140EC46D5: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC46DA: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC46E1: 44 0F B6 44 24 40  movzx       r8d,byte ptr [rsp+40h]
  0000000140EC46E7: 8B D0              mov         edx,eax
  0000000140EC46E9: 48 8B 89 B8 0C 00  mov         rcx,qword ptr [rcx+0000000000000CB8h]
                    00
  0000000140EC46F0: FF 15 A2 14 F4 00  call        qword ptr [0000000141E05B98h]
  0000000140EC46F6: 4C 8B 5C 24 68     mov         r11,qword ptr [rsp+68h]
  0000000140EC46FB: F2 41 0F 10 93 A8  movsd       xmm2,mmword ptr [r11+00000000000003A8h]
                    03 00 00
  0000000140EC4704: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EC4709: 48 8D 15 98 05 C5  lea         rdx,[0000000142B14CA8h]
                    01
  0000000140EC4710: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC4715: FF 15 FD F7 F3 00  call        qword ptr [0000000141E03F18h]
  0000000140EC471B: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4720: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4727: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140EC472C: 48 8B 89 40 0F 00  mov         rcx,qword ptr [rcx+0000000000000F40h]
                    00
  0000000140EC4733: FF 15 7F 1F F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EC4739: 4C 8B 5C 24 68     mov         r11,qword ptr [rsp+68h]
  0000000140EC473E: F2 41 0F 10 93 B0  movsd       xmm2,mmword ptr [r11+00000000000003B0h]
                    03 00 00
  0000000140EC4747: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EC474C: 48 8D 15 5D 05 C5  lea         rdx,[0000000142B14CB0h]
                    01
  0000000140EC4753: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC4758: FF 15 BA F7 F3 00  call        qword ptr [0000000141E03F18h]
  0000000140EC475E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4763: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC476A: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140EC476F: 48 8B 89 70 0F 00  mov         rcx,qword ptr [rcx+0000000000000F70h]
                    00
  0000000140EC4776: FF 15 3C 1F F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EC477C: 4C 8B 5C 24 68     mov         r11,qword ptr [rsp+68h]
  0000000140EC4781: F2 41 0F 10 93 B8  movsd       xmm2,mmword ptr [r11+00000000000003B8h]
                    03 00 00
  0000000140EC478A: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EC478F: 48 8D 15 22 05 C5  lea         rdx,[0000000142B14CB8h]
                    01
  0000000140EC4796: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC479B: FF 15 77 F7 F3 00  call        qword ptr [0000000141E03F18h]
  0000000140EC47A1: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC47A6: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC47AD: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140EC47B2: 48 8B 89 50 0F 00  mov         rcx,qword ptr [rcx+0000000000000F50h]
                    00
  0000000140EC47B9: FF 15 F9 1E F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EC47BF: 4C 8B 5C 24 68     mov         r11,qword ptr [rsp+68h]
  0000000140EC47C4: F2 41 0F 10 93 B8  movsd       xmm2,mmword ptr [r11+00000000000004B8h]
                    04 00 00
  0000000140EC47CD: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EC47D2: 48 8D 15 E7 04 C5  lea         rdx,[0000000142B14CC0h]
                    01
  0000000140EC47D9: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC47DE: FF 15 34 F7 F3 00  call        qword ptr [0000000141E03F18h]
  0000000140EC47E4: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC47E9: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC47F0: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140EC47F5: 48 8B 89 60 0F 00  mov         rcx,qword ptr [rcx+0000000000000F60h]
                    00
  0000000140EC47FC: FF 15 B6 1E F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EC4802: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC4807: 83 B8 D8 09 00 00  cmp         dword ptr [rax+00000000000009D8h],0
                    00
  0000000140EC480E: 74 38              je          0000000140EC4848
  0000000140EC4810: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4815: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC481C: B2 01              mov         dl,1
  0000000140EC481E: 48 8B 89 98 0D 00  mov         rcx,qword ptr [rcx+0000000000000D98h]
                    00
  0000000140EC4825: FF 15 6D 2A F4 00  call        qword ptr [0000000141E07298h]
  0000000140EC482B: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4830: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4837: B2 01              mov         dl,1
  0000000140EC4839: 48 8B 89 A0 0D 00  mov         rcx,qword ptr [rcx+0000000000000DA0h]
                    00
  0000000140EC4840: FF 15 FA 71 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC4846: EB 36              jmp         0000000140EC487E
  0000000140EC4848: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC484D: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4854: 33 D2              xor         edx,edx
  0000000140EC4856: 48 8B 89 98 0D 00  mov         rcx,qword ptr [rcx+0000000000000D98h]
                    00
  0000000140EC485D: FF 15 35 2A F4 00  call        qword ptr [0000000141E07298h]
  0000000140EC4863: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4868: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC486F: 33 D2              xor         edx,edx
  0000000140EC4871: 48 8B 89 A0 0D 00  mov         rcx,qword ptr [rcx+0000000000000DA0h]
                    00
  0000000140EC4878: FF 15 C2 71 F4 00  call        qword ptr [0000000141E0BA40h]
  0000000140EC487E: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140EC4883: C7 80 60 01 00 00  mov         dword ptr [rax+0000000000000160h],0
                    00 00 00 00
  0000000140EC488D: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC4892: 83 B8 74 0A 00 00  cmp         dword ptr [rax+0000000000000A74h],0
                    00
  0000000140EC4899: 0F 8E 86 00 00 00  jle         0000000140EC4925
  0000000140EC489F: C7 44 24 24 00 00  mov         dword ptr [rsp+24h],0
                    00 00
  0000000140EC48A7: EB 0B              jmp         0000000140EC48B4
  0000000140EC48A9: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  0000000140EC48AD: 83 C0 01           add         eax,1
  0000000140EC48B0: 89 44 24 24        mov         dword ptr [rsp+24h],eax
  0000000140EC48B4: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC48B9: 8B 80 74 0A 00 00  mov         eax,dword ptr [rax+0000000000000A74h]
  0000000140EC48BF: 39 44 24 24        cmp         dword ptr [rsp+24h],eax
  0000000140EC48C3: 7D 60              jge         0000000140EC4925
  0000000140EC48C5: 48 63 4C 24 24     movsxd      rcx,dword ptr [rsp+24h]
  0000000140EC48CA: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140EC48CF: 48 8B 80 80 0A 00  mov         rax,qword ptr [rax+0000000000000A80h]
                    00
  0000000140EC48D6: F2 0F 10 04 C8     movsd       xmm0,mmword ptr [rax+rcx*8]
  0000000140EC48DB: 66 0F 2E 05 FD 00  ucomisd     xmm0,mmword ptr [00000001421249E0h]
                    26 01
  0000000140EC48E3: 7A 3E              jp          0000000140EC4923
  0000000140EC48E5: 75 3C              jne         0000000140EC4923
  0000000140EC48E7: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140EC48EC: C7 80 60 01 00 00  mov         dword ptr [rax+0000000000000160h],1
                    01 00 00 00
  0000000140EC48F6: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC48FB: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4902: B2 01              mov         dl,1
  0000000140EC4904: 48 8B 89 B8 0D 00  mov         rcx,qword ptr [rcx+0000000000000DB8h]
                    00
  0000000140EC490B: FF 15 87 29 F4 00  call        qword ptr [0000000141E07298h]
  0000000140EC4911: 4C 8B 5C 24 60     mov         r11,qword ptr [rsp+60h]
  0000000140EC4916: 41 C7 83 60 01 00  mov         dword ptr [r11+0000000000000160h],0
                    00 00 00 00 00
  0000000140EC4921: EB 02              jmp         0000000140EC4925
  0000000140EC4923: EB 84              jmp         0000000140EC48A9
  0000000140EC4925: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  0000000140EC492A: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC492F: E8 3C 00 00 00     call        0000000140EC4970
  0000000140EC4934: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140EC4939: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EC4940: BA 04 00 00 00     mov         edx,4
  0000000140EC4945: 48 8B 89 B8 0C 00  mov         rcx,qword ptr [rcx+0000000000000CB8h]
                    00
  0000000140EC494C: FF 15 46 6A F4 00  call        qword ptr [0000000141E0B398h]
  0000000140EC4952: 90                 nop
  0000000140EC4953: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140EC4958: FF 15 82 10 F4 00  call        qword ptr [0000000141E059E0h]
  0000000140EC495E: 48 83 C4 58        add         rsp,58h
  0000000140EC4962: C3                 ret

  Summary

     1E02000 .text
