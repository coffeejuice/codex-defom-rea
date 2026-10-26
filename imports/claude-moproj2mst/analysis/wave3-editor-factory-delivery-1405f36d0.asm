Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001405F36D0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001405F36D5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001405F36DA: 56                 push        rsi
  00000001405F36DB: 57                 push        rdi
  00000001405F36DC: 48 81 EC 98 0F 00  sub         rsp,0F98h
                    00
  00000001405F36E3: 48 C7 84 24 60 0F  mov         qword ptr [rsp+0000000000000F60h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001405F36EF: 48 8B 8C 24 B0 0F  mov         rcx,qword ptr [rsp+0000000000000FB0h]
                    00 00
  00000001405F36F7: E8 B4 BB FF FF     call        00000001405EF2B0
  00000001405F36FC: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001405F3701: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  00000001405F3707: 75 05              jne         00000001405F370E
  00000001405F3709: E9 56 19 00 00     jmp         00000001405F5064
  00000001405F370E: 48 8B 8C 24 B0 0F  mov         rcx,qword ptr [rsp+0000000000000FB0h]
                    00 00
  00000001405F3716: E8 C5 BB FF FF     call        00000001405EF2E0
  00000001405F371B: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001405F3720: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  00000001405F3726: 75 05              jne         00000001405F372D
  00000001405F3728: E9 37 19 00 00     jmp         00000001405F5064
  00000001405F372D: 48 8B 8C 24 B8 0F  mov         rcx,qword ptr [rsp+0000000000000FB8h]
                    00 00
  00000001405F3735: E8 36 B3 75 00     call        0000000140D4EA70
  00000001405F373A: 89 84 24 8C 00 00  mov         dword ptr [rsp+000000000000008Ch],eax
                    00
  00000001405F3741: B2 01              mov         dl,1
  00000001405F3743: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001405F3748: E8 53 7A 68 00     call        0000000140C7B1A0
  00000001405F374D: 3B 84 24 8C 00 00  cmp         eax,dword ptr [rsp+000000000000008Ch]
                    00
  00000001405F3754: 74 22              je          00000001405F3778
  00000001405F3756: C6 84 24 90 00 00  mov         byte ptr [rsp+0000000000000090h],1
                    00 01
  00000001405F375E: 44 0F B6 84 24 90  movzx       r8d,byte ptr [rsp+0000000000000090h]
                    00 00 00
  00000001405F3767: 8B 94 24 8C 00 00  mov         edx,dword ptr [rsp+000000000000008Ch]
                    00
  00000001405F376E: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001405F3773: E8 78 7A 68 00     call        0000000140C7B1F0
  00000001405F3778: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001405F377D: E8 8E D1 A5 FF     call        0000000140050910
  00000001405F3782: 89 84 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],eax
                    00
  00000001405F3789: 4C 8D 84 24 8C 00  lea         r8,[rsp+000000000000008Ch]
                    00 00
  00000001405F3791: 48 8D 15 EC 06 EA  lea         rdx,[0000000142493E84h]
                    01
  00000001405F3798: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  00000001405F37A0: E8 DB 18 A3 FF     call        0000000140025080
  00000001405F37A5: 48 8D BC 24 50 0F  lea         rdi,[rsp+0000000000000F50h]
                    00 00
  00000001405F37AD: 48 8B F0           mov         rsi,rax
  00000001405F37B0: B9 10 00 00 00     mov         ecx,10h
  00000001405F37B5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F37B7: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  00000001405F37BF: 48 8D B4 24 50 0F  lea         rsi,[rsp+0000000000000F50h]
                    00 00
  00000001405F37C7: B9 10 00 00 00     mov         ecx,10h
  00000001405F37CC: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F37CE: 4C 8D 84 24 84 00  lea         r8,[rsp+0000000000000084h]
                    00 00
  00000001405F37D6: 48 8D 15 BF 08 EA  lea         rdx,[000000014249409Ch]
                    01
  00000001405F37DD: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  00000001405F37E5: E8 96 18 A3 FF     call        0000000140025080
  00000001405F37EA: 48 8D BC 24 40 0F  lea         rdi,[rsp+0000000000000F40h]
                    00 00
  00000001405F37F2: 48 8B F0           mov         rsi,rax
  00000001405F37F5: B9 10 00 00 00     mov         ecx,10h
  00000001405F37FA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F37FC: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  00000001405F3804: 48 8D B4 24 40 0F  lea         rsi,[rsp+0000000000000F40h]
                    00 00
  00000001405F380C: B9 10 00 00 00     mov         ecx,10h
  00000001405F3811: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3813: 45 33 C0           xor         r8d,r8d
  00000001405F3816: 33 D2              xor         edx,edx
  00000001405F3818: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001405F3820: FF 15 42 09 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3826: 48 8D BC 24 A0 08  lea         rdi,[rsp+00000000000008A0h]
                    00 00
  00000001405F382E: 48 8B F0           mov         rsi,rax
  00000001405F3831: B9 10 00 00 00     mov         ecx,10h
  00000001405F3836: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3838: 45 33 C0           xor         r8d,r8d
  00000001405F383B: 33 D2              xor         edx,edx
  00000001405F383D: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000001405F3845: FF 15 1D 09 81 01  call        qword ptr [0000000141E04168h]
  00000001405F384B: 48 8D BC 24 90 08  lea         rdi,[rsp+0000000000000890h]
                    00 00
  00000001405F3853: 48 8B F0           mov         rsi,rax
  00000001405F3856: B9 10 00 00 00     mov         ecx,10h
  00000001405F385B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F385D: 45 33 C0           xor         r8d,r8d
  00000001405F3860: 33 D2              xor         edx,edx
  00000001405F3862: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001405F386A: FF 15 F8 08 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3870: 48 8D BC 24 80 08  lea         rdi,[rsp+0000000000000880h]
                    00 00
  00000001405F3878: 48 8B F0           mov         rsi,rax
  00000001405F387B: B9 10 00 00 00     mov         ecx,10h
  00000001405F3880: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3882: 45 33 C0           xor         r8d,r8d
  00000001405F3885: 33 D2              xor         edx,edx
  00000001405F3887: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  00000001405F388F: FF 15 D3 08 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3895: 48 8D BC 24 70 08  lea         rdi,[rsp+0000000000000870h]
                    00 00
  00000001405F389D: 48 8B F0           mov         rsi,rax
  00000001405F38A0: B9 10 00 00 00     mov         ecx,10h
  00000001405F38A5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F38A7: 45 33 C0           xor         r8d,r8d
  00000001405F38AA: 33 D2              xor         edx,edx
  00000001405F38AC: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  00000001405F38B4: FF 15 AE 08 81 01  call        qword ptr [0000000141E04168h]
  00000001405F38BA: 48 8D BC 24 60 08  lea         rdi,[rsp+0000000000000860h]
                    00 00
  00000001405F38C2: 48 8B F0           mov         rsi,rax
  00000001405F38C5: B9 10 00 00 00     mov         ecx,10h
  00000001405F38CA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F38CC: 45 33 C0           xor         r8d,r8d
  00000001405F38CF: 33 D2              xor         edx,edx
  00000001405F38D1: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  00000001405F38D9: FF 15 89 08 81 01  call        qword ptr [0000000141E04168h]
  00000001405F38DF: 48 8D BC 24 50 08  lea         rdi,[rsp+0000000000000850h]
                    00 00
  00000001405F38E7: 48 8B F0           mov         rsi,rax
  00000001405F38EA: B9 10 00 00 00     mov         ecx,10h
  00000001405F38EF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F38F1: 45 33 C0           xor         r8d,r8d
  00000001405F38F4: 33 D2              xor         edx,edx
  00000001405F38F6: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  00000001405F38FE: FF 15 64 08 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3904: 48 8D BC 24 40 08  lea         rdi,[rsp+0000000000000840h]
                    00 00
  00000001405F390C: 48 8B F0           mov         rsi,rax
  00000001405F390F: B9 10 00 00 00     mov         ecx,10h
  00000001405F3914: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3916: 45 33 C0           xor         r8d,r8d
  00000001405F3919: 33 D2              xor         edx,edx
  00000001405F391B: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  00000001405F3923: FF 15 3F 08 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3929: 48 8D BC 24 30 08  lea         rdi,[rsp+0000000000000830h]
                    00 00
  00000001405F3931: 48 8B F0           mov         rsi,rax
  00000001405F3934: B9 10 00 00 00     mov         ecx,10h
  00000001405F3939: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F393B: 48 8D BC 24 20 08  lea         rdi,[rsp+0000000000000820h]
                    00 00
  00000001405F3943: 48 8D B4 24 20 01  lea         rsi,[rsp+0000000000000120h]
                    00 00
  00000001405F394B: B9 10 00 00 00     mov         ecx,10h
  00000001405F3950: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3952: 48 8D BC 24 10 08  lea         rdi,[rsp+0000000000000810h]
                    00 00
  00000001405F395A: 48 8D B4 24 40 01  lea         rsi,[rsp+0000000000000140h]
                    00 00
  00000001405F3962: B9 10 00 00 00     mov         ecx,10h
  00000001405F3967: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3969: 48 8D 84 24 A0 08  lea         rax,[rsp+00000000000008A0h]
                    00 00
  00000001405F3971: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001405F3976: 48 8D 84 24 90 08  lea         rax,[rsp+0000000000000890h]
                    00 00
  00000001405F397E: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001405F3983: 48 8D 84 24 80 08  lea         rax,[rsp+0000000000000880h]
                    00 00
  00000001405F398B: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001405F3990: 48 8D 84 24 70 08  lea         rax,[rsp+0000000000000870h]
                    00 00
  00000001405F3998: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001405F399D: 48 8D 84 24 60 08  lea         rax,[rsp+0000000000000860h]
                    00 00
  00000001405F39A5: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001405F39AA: 48 8D 84 24 50 08  lea         rax,[rsp+0000000000000850h]
                    00 00
  00000001405F39B2: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001405F39B7: 48 8D 84 24 40 08  lea         rax,[rsp+0000000000000840h]
                    00 00
  00000001405F39BF: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405F39C4: 48 8D 84 24 30 08  lea         rax,[rsp+0000000000000830h]
                    00 00
  00000001405F39CC: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405F39D1: 4C 8D 8C 24 20 08  lea         r9,[rsp+0000000000000820h]
                    00 00
  00000001405F39D9: 4C 8D 84 24 10 08  lea         r8,[rsp+0000000000000810h]
                    00 00
  00000001405F39E1: 48 8D 15 F8 3C EA  lea         rdx,[00000001424976E0h]
                    01
  00000001405F39E8: 48 8D 0D 11 2D 78  lea         rcx,[0000000143D76700h]
                    03
  00000001405F39EF: E8 CC B9 CD FF     call        00000001402CF3C0
  00000001405F39F4: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00
  00000001405F39FF: 4C 8D 84 24 80 00  lea         r8,[rsp+0000000000000080h]
                    00 00
  00000001405F3A07: 48 8D 15 1E 0D EA  lea         rdx,[000000014249472Ch]
                    01
  00000001405F3A0E: 48 8D 8C 24 10 02  lea         rcx,[rsp+0000000000000210h]
                    00 00
  00000001405F3A16: E8 D5 8C AB FF     call        00000001400AC6F0
  00000001405F3A1B: 48 8D BC 24 30 0F  lea         rdi,[rsp+0000000000000F30h]
                    00 00
  00000001405F3A23: 48 8B F0           mov         rsi,rax
  00000001405F3A26: B9 10 00 00 00     mov         ecx,10h
  00000001405F3A2B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3A2D: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  00000001405F3A35: 48 8D B4 24 30 0F  lea         rsi,[rsp+0000000000000F30h]
                    00 00
  00000001405F3A3D: B9 10 00 00 00     mov         ecx,10h
  00000001405F3A42: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3A44: 45 33 C0           xor         r8d,r8d
  00000001405F3A47: 33 D2              xor         edx,edx
  00000001405F3A49: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  00000001405F3A51: FF 15 11 07 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3A57: 48 8D BC 24 50 09  lea         rdi,[rsp+0000000000000950h]
                    00 00
  00000001405F3A5F: 48 8B F0           mov         rsi,rax
  00000001405F3A62: B9 10 00 00 00     mov         ecx,10h
  00000001405F3A67: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3A69: 45 33 C0           xor         r8d,r8d
  00000001405F3A6C: 33 D2              xor         edx,edx
  00000001405F3A6E: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  00000001405F3A76: FF 15 EC 06 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3A7C: 48 8D BC 24 40 09  lea         rdi,[rsp+0000000000000940h]
                    00 00
  00000001405F3A84: 48 8B F0           mov         rsi,rax
  00000001405F3A87: B9 10 00 00 00     mov         ecx,10h
  00000001405F3A8C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3A8E: 45 33 C0           xor         r8d,r8d
  00000001405F3A91: 33 D2              xor         edx,edx
  00000001405F3A93: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  00000001405F3A9B: FF 15 C7 06 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3AA1: 48 8D BC 24 30 09  lea         rdi,[rsp+0000000000000930h]
                    00 00
  00000001405F3AA9: 48 8B F0           mov         rsi,rax
  00000001405F3AAC: B9 10 00 00 00     mov         ecx,10h
  00000001405F3AB1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3AB3: 45 33 C0           xor         r8d,r8d
  00000001405F3AB6: 33 D2              xor         edx,edx
  00000001405F3AB8: 48 8D 8C 24 90 01  lea         rcx,[rsp+0000000000000190h]
                    00 00
  00000001405F3AC0: FF 15 A2 06 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3AC6: 48 8D BC 24 20 09  lea         rdi,[rsp+0000000000000920h]
                    00 00
  00000001405F3ACE: 48 8B F0           mov         rsi,rax
  00000001405F3AD1: B9 10 00 00 00     mov         ecx,10h
  00000001405F3AD6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3AD8: 45 33 C0           xor         r8d,r8d
  00000001405F3ADB: 33 D2              xor         edx,edx
  00000001405F3ADD: 48 8D 8C 24 A0 01  lea         rcx,[rsp+00000000000001A0h]
                    00 00
  00000001405F3AE5: FF 15 7D 06 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3AEB: 48 8D BC 24 10 09  lea         rdi,[rsp+0000000000000910h]
                    00 00
  00000001405F3AF3: 48 8B F0           mov         rsi,rax
  00000001405F3AF6: B9 10 00 00 00     mov         ecx,10h
  00000001405F3AFB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3AFD: 45 33 C0           xor         r8d,r8d
  00000001405F3B00: 33 D2              xor         edx,edx
  00000001405F3B02: 48 8D 8C 24 B0 01  lea         rcx,[rsp+00000000000001B0h]
                    00 00
  00000001405F3B0A: FF 15 58 06 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3B10: 48 8D BC 24 00 09  lea         rdi,[rsp+0000000000000900h]
                    00 00
  00000001405F3B18: 48 8B F0           mov         rsi,rax
  00000001405F3B1B: B9 10 00 00 00     mov         ecx,10h
  00000001405F3B20: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3B22: 45 33 C0           xor         r8d,r8d
  00000001405F3B25: 33 D2              xor         edx,edx
  00000001405F3B27: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  00000001405F3B2F: FF 15 33 06 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3B35: 48 8D BC 24 F0 08  lea         rdi,[rsp+00000000000008F0h]
                    00 00
  00000001405F3B3D: 48 8B F0           mov         rsi,rax
  00000001405F3B40: B9 10 00 00 00     mov         ecx,10h
  00000001405F3B45: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3B47: 45 33 C0           xor         r8d,r8d
  00000001405F3B4A: 33 D2              xor         edx,edx
  00000001405F3B4C: 48 8D 8C 24 D0 01  lea         rcx,[rsp+00000000000001D0h]
                    00 00
  00000001405F3B54: FF 15 0E 06 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3B5A: 48 8D BC 24 E0 08  lea         rdi,[rsp+00000000000008E0h]
                    00 00
  00000001405F3B62: 48 8B F0           mov         rsi,rax
  00000001405F3B65: B9 10 00 00 00     mov         ecx,10h
  00000001405F3B6A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3B6C: 45 33 C0           xor         r8d,r8d
  00000001405F3B6F: 33 D2              xor         edx,edx
  00000001405F3B71: 48 8D 8C 24 E0 01  lea         rcx,[rsp+00000000000001E0h]
                    00 00
  00000001405F3B79: FF 15 E9 05 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3B7F: 48 8D BC 24 D0 08  lea         rdi,[rsp+00000000000008D0h]
                    00 00
  00000001405F3B87: 48 8B F0           mov         rsi,rax
  00000001405F3B8A: B9 10 00 00 00     mov         ecx,10h
  00000001405F3B8F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3B91: 45 33 C0           xor         r8d,r8d
  00000001405F3B94: 33 D2              xor         edx,edx
  00000001405F3B96: 48 8D 8C 24 F0 01  lea         rcx,[rsp+00000000000001F0h]
                    00 00
  00000001405F3B9E: FF 15 C4 05 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3BA4: 48 8D BC 24 C0 08  lea         rdi,[rsp+00000000000008C0h]
                    00 00
  00000001405F3BAC: 48 8B F0           mov         rsi,rax
  00000001405F3BAF: B9 10 00 00 00     mov         ecx,10h
  00000001405F3BB4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3BB6: 48 8D BC 24 B0 08  lea         rdi,[rsp+00000000000008B0h]
                    00 00
  00000001405F3BBE: 48 8D B4 24 00 02  lea         rsi,[rsp+0000000000000200h]
                    00 00
  00000001405F3BC6: B9 10 00 00 00     mov         ecx,10h
  00000001405F3BCB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3BCD: 48 8D 84 24 50 09  lea         rax,[rsp+0000000000000950h]
                    00 00
  00000001405F3BD5: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001405F3BDA: 48 8D 84 24 40 09  lea         rax,[rsp+0000000000000940h]
                    00 00
  00000001405F3BE2: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001405F3BE7: 48 8D 84 24 30 09  lea         rax,[rsp+0000000000000930h]
                    00 00
  00000001405F3BEF: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001405F3BF4: 48 8D 84 24 20 09  lea         rax,[rsp+0000000000000920h]
                    00 00
  00000001405F3BFC: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001405F3C01: 48 8D 84 24 10 09  lea         rax,[rsp+0000000000000910h]
                    00 00
  00000001405F3C09: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001405F3C0E: 48 8D 84 24 00 09  lea         rax,[rsp+0000000000000900h]
                    00 00
  00000001405F3C16: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001405F3C1B: 48 8D 84 24 F0 08  lea         rax,[rsp+00000000000008F0h]
                    00 00
  00000001405F3C23: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001405F3C28: 48 8D 84 24 E0 08  lea         rax,[rsp+00000000000008E0h]
                    00 00
  00000001405F3C30: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405F3C35: 48 8D 84 24 D0 08  lea         rax,[rsp+00000000000008D0h]
                    00 00
  00000001405F3C3D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405F3C42: 4C 8D 8C 24 C0 08  lea         r9,[rsp+00000000000008C0h]
                    00 00
  00000001405F3C4A: 4C 8D 84 24 B0 08  lea         r8,[rsp+00000000000008B0h]
                    00 00
  00000001405F3C52: 48 8D 15 A7 3A EA  lea         rdx,[0000000142497700h]
                    01
  00000001405F3C59: 48 8D 0D A0 2A 78  lea         rcx,[0000000143D76700h]
                    03
  00000001405F3C60: E8 DB BA CD FF     call        00000001402CF740
  00000001405F3C65: 83 BC 24 80 00 00  cmp         dword ptr [rsp+0000000000000080h],0
                    00 00
  00000001405F3C6D: 75 0D              jne         00000001405F3C7C
  00000001405F3C6F: C7 84 24 68 0F 00  mov         dword ptr [rsp+0000000000000F68h],1
                    00 01 00 00 00
  00000001405F3C7A: EB 0B              jmp         00000001405F3C87
  00000001405F3C7C: C7 84 24 68 0F 00  mov         dword ptr [rsp+0000000000000F68h],0
                    00 00 00 00 00
  00000001405F3C87: 0F B6 84 24 68 0F  movzx       eax,byte ptr [rsp+0000000000000F68h]
                    00 00
  00000001405F3C8F: 88 84 24 88 00 00  mov         byte ptr [rsp+0000000000000088h],al
                    00
  00000001405F3C96: 48 8D 15 8B 3A EA  lea         rdx,[0000000142497728h]
                    01
  00000001405F3C9D: 48 8D 8C 24 20 02  lea         rcx,[rsp+0000000000000220h]
                    00 00
  00000001405F3CA5: FF 15 65 04 81 01  call        qword ptr [0000000141E04110h]
  00000001405F3CAB: 48 8B F8           mov         rdi,rax
  00000001405F3CAE: 48 8D 94 24 28 02  lea         rdx,[rsp+0000000000000228h]
                    00 00
  00000001405F3CB6: 48 8B 8C 24 B8 0F  mov         rcx,qword ptr [rsp+0000000000000FB8h]
                    00 00
  00000001405F3CBE: E8 DD AC 75 00     call        0000000140D4E9A0
  00000001405F3CC3: 48 8B D7           mov         rdx,rdi
  00000001405F3CC6: 48 8B C8           mov         rcx,rax
  00000001405F3CC9: E8 92 E7 B4 FF     call        0000000140142460
  00000001405F3CCE: 0F B6 C0           movzx       eax,al
  00000001405F3CD1: 85 C0              test        eax,eax
  00000001405F3CD3: 0F 84 2D 06 00 00  je          00000001405F4306
  00000001405F3CD9: 0F B6 84 24 88 00  movzx       eax,byte ptr [rsp+0000000000000088h]
                    00 00
  00000001405F3CE1: 85 C0              test        eax,eax
  00000001405F3CE3: 0F 85 0F 04 00 00  jne         00000001405F40F8
  00000001405F3CE9: 45 33 C0           xor         r8d,r8d
  00000001405F3CEC: 33 D2              xor         edx,edx
  00000001405F3CEE: 48 8D 8C 24 30 02  lea         rcx,[rsp+0000000000000230h]
                    00 00
  00000001405F3CF6: FF 15 6C 04 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3CFC: 48 8D BC 24 F0 09  lea         rdi,[rsp+00000000000009F0h]
                    00 00
  00000001405F3D04: 48 8B F0           mov         rsi,rax
  00000001405F3D07: B9 10 00 00 00     mov         ecx,10h
  00000001405F3D0C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3D0E: 45 33 C0           xor         r8d,r8d
  00000001405F3D11: 33 D2              xor         edx,edx
  00000001405F3D13: 48 8D 8C 24 40 02  lea         rcx,[rsp+0000000000000240h]
                    00 00
  00000001405F3D1B: FF 15 47 04 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3D21: 48 8D BC 24 E0 09  lea         rdi,[rsp+00000000000009E0h]
                    00 00
  00000001405F3D29: 48 8B F0           mov         rsi,rax
  00000001405F3D2C: B9 10 00 00 00     mov         ecx,10h
  00000001405F3D31: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3D33: 45 33 C0           xor         r8d,r8d
  00000001405F3D36: 33 D2              xor         edx,edx
  00000001405F3D38: 48 8D 8C 24 50 02  lea         rcx,[rsp+0000000000000250h]
                    00 00
  00000001405F3D40: FF 15 22 04 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3D46: 48 8D BC 24 D0 09  lea         rdi,[rsp+00000000000009D0h]
                    00 00
  00000001405F3D4E: 48 8B F0           mov         rsi,rax
  00000001405F3D51: B9 10 00 00 00     mov         ecx,10h
  00000001405F3D56: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3D58: 45 33 C0           xor         r8d,r8d
  00000001405F3D5B: 33 D2              xor         edx,edx
  00000001405F3D5D: 48 8D 8C 24 60 02  lea         rcx,[rsp+0000000000000260h]
                    00 00
  00000001405F3D65: FF 15 FD 03 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3D6B: 48 8D BC 24 C0 09  lea         rdi,[rsp+00000000000009C0h]
                    00 00
  00000001405F3D73: 48 8B F0           mov         rsi,rax
  00000001405F3D76: B9 10 00 00 00     mov         ecx,10h
  00000001405F3D7B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3D7D: 45 33 C0           xor         r8d,r8d
  00000001405F3D80: 33 D2              xor         edx,edx
  00000001405F3D82: 48 8D 8C 24 70 02  lea         rcx,[rsp+0000000000000270h]
                    00 00
  00000001405F3D8A: FF 15 D8 03 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3D90: 48 8D BC 24 B0 09  lea         rdi,[rsp+00000000000009B0h]
                    00 00
  00000001405F3D98: 48 8B F0           mov         rsi,rax
  00000001405F3D9B: B9 10 00 00 00     mov         ecx,10h
  00000001405F3DA0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3DA2: 45 33 C0           xor         r8d,r8d
  00000001405F3DA5: 33 D2              xor         edx,edx
  00000001405F3DA7: 48 8D 8C 24 80 02  lea         rcx,[rsp+0000000000000280h]
                    00 00
  00000001405F3DAF: FF 15 B3 03 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3DB5: 48 8D BC 24 A0 09  lea         rdi,[rsp+00000000000009A0h]
                    00 00
  00000001405F3DBD: 48 8B F0           mov         rsi,rax
  00000001405F3DC0: B9 10 00 00 00     mov         ecx,10h
  00000001405F3DC5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3DC7: 45 33 C0           xor         r8d,r8d
  00000001405F3DCA: 33 D2              xor         edx,edx
  00000001405F3DCC: 48 8D 8C 24 90 02  lea         rcx,[rsp+0000000000000290h]
                    00 00
  00000001405F3DD4: FF 15 8E 03 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3DDA: 48 8D BC 24 90 09  lea         rdi,[rsp+0000000000000990h]
                    00 00
  00000001405F3DE2: 48 8B F0           mov         rsi,rax
  00000001405F3DE5: B9 10 00 00 00     mov         ecx,10h
  00000001405F3DEA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3DEC: 45 33 C0           xor         r8d,r8d
  00000001405F3DEF: 33 D2              xor         edx,edx
  00000001405F3DF1: 48 8D 8C 24 A0 02  lea         rcx,[rsp+00000000000002A0h]
                    00 00
  00000001405F3DF9: FF 15 69 03 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3DFF: 48 8D BC 24 80 09  lea         rdi,[rsp+0000000000000980h]
                    00 00
  00000001405F3E07: 48 8B F0           mov         rsi,rax
  00000001405F3E0A: B9 10 00 00 00     mov         ecx,10h
  00000001405F3E0F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3E11: 45 33 C0           xor         r8d,r8d
  00000001405F3E14: 33 D2              xor         edx,edx
  00000001405F3E16: 48 8D 8C 24 B0 02  lea         rcx,[rsp+00000000000002B0h]
                    00 00
  00000001405F3E1E: FF 15 44 03 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3E24: 48 8D BC 24 70 09  lea         rdi,[rsp+0000000000000970h]
                    00 00
  00000001405F3E2C: 48 8B F0           mov         rsi,rax
  00000001405F3E2F: B9 10 00 00 00     mov         ecx,10h
  00000001405F3E34: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3E36: 45 33 C0           xor         r8d,r8d
  00000001405F3E39: 33 D2              xor         edx,edx
  00000001405F3E3B: 48 8D 8C 24 C0 02  lea         rcx,[rsp+00000000000002C0h]
                    00 00
  00000001405F3E43: FF 15 1F 03 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3E49: 48 8D BC 24 60 09  lea         rdi,[rsp+0000000000000960h]
                    00 00
  00000001405F3E51: 48 8B F0           mov         rsi,rax
  00000001405F3E54: B9 10 00 00 00     mov         ecx,10h
  00000001405F3E59: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3E5B: 48 8D 84 24 F0 09  lea         rax,[rsp+00000000000009F0h]
                    00 00
  00000001405F3E63: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001405F3E68: 48 8D 84 24 E0 09  lea         rax,[rsp+00000000000009E0h]
                    00 00
  00000001405F3E70: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001405F3E75: 48 8D 84 24 D0 09  lea         rax,[rsp+00000000000009D0h]
                    00 00
  00000001405F3E7D: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001405F3E82: 48 8D 84 24 C0 09  lea         rax,[rsp+00000000000009C0h]
                    00 00
  00000001405F3E8A: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001405F3E8F: 48 8D 84 24 B0 09  lea         rax,[rsp+00000000000009B0h]
                    00 00
  00000001405F3E97: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001405F3E9C: 48 8D 84 24 A0 09  lea         rax,[rsp+00000000000009A0h]
                    00 00
  00000001405F3EA4: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001405F3EA9: 48 8D 84 24 90 09  lea         rax,[rsp+0000000000000990h]
                    00 00
  00000001405F3EB1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405F3EB6: 48 8D 84 24 80 09  lea         rax,[rsp+0000000000000980h]
                    00 00
  00000001405F3EBE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405F3EC3: 4C 8D 8C 24 70 09  lea         r9,[rsp+0000000000000970h]
                    00 00
  00000001405F3ECB: 4C 8D 84 24 60 09  lea         r8,[rsp+0000000000000960h]
                    00 00
  00000001405F3ED3: 48 8D 15 56 38 EA  lea         rdx,[0000000142497730h]
                    01
  00000001405F3EDA: 48 8D 0D 1F 28 78  lea         rcx,[0000000143D76700h]
                    03
  00000001405F3EE1: E8 DA B4 CD FF     call        00000001402CF3C0
  00000001405F3EE6: 45 33 C0           xor         r8d,r8d
  00000001405F3EE9: 33 D2              xor         edx,edx
  00000001405F3EEB: 48 8D 8C 24 D0 02  lea         rcx,[rsp+00000000000002D0h]
                    00 00
  00000001405F3EF3: FF 15 6F 02 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3EF9: 48 8D BC 24 90 0A  lea         rdi,[rsp+0000000000000A90h]
                    00 00
  00000001405F3F01: 48 8B F0           mov         rsi,rax
  00000001405F3F04: B9 10 00 00 00     mov         ecx,10h
  00000001405F3F09: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3F0B: 45 33 C0           xor         r8d,r8d
  00000001405F3F0E: 33 D2              xor         edx,edx
  00000001405F3F10: 48 8D 8C 24 E0 02  lea         rcx,[rsp+00000000000002E0h]
                    00 00
  00000001405F3F18: FF 15 4A 02 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3F1E: 48 8D BC 24 80 0A  lea         rdi,[rsp+0000000000000A80h]
                    00 00
  00000001405F3F26: 48 8B F0           mov         rsi,rax
  00000001405F3F29: B9 10 00 00 00     mov         ecx,10h
  00000001405F3F2E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3F30: 45 33 C0           xor         r8d,r8d
  00000001405F3F33: 33 D2              xor         edx,edx
  00000001405F3F35: 48 8D 8C 24 F0 02  lea         rcx,[rsp+00000000000002F0h]
                    00 00
  00000001405F3F3D: FF 15 25 02 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3F43: 48 8D BC 24 70 0A  lea         rdi,[rsp+0000000000000A70h]
                    00 00
  00000001405F3F4B: 48 8B F0           mov         rsi,rax
  00000001405F3F4E: B9 10 00 00 00     mov         ecx,10h
  00000001405F3F53: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3F55: 45 33 C0           xor         r8d,r8d
  00000001405F3F58: 33 D2              xor         edx,edx
  00000001405F3F5A: 48 8D 8C 24 00 03  lea         rcx,[rsp+0000000000000300h]
                    00 00
  00000001405F3F62: FF 15 00 02 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3F68: 48 8D BC 24 60 0A  lea         rdi,[rsp+0000000000000A60h]
                    00 00
  00000001405F3F70: 48 8B F0           mov         rsi,rax
  00000001405F3F73: B9 10 00 00 00     mov         ecx,10h
  00000001405F3F78: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3F7A: 45 33 C0           xor         r8d,r8d
  00000001405F3F7D: 33 D2              xor         edx,edx
  00000001405F3F7F: 48 8D 8C 24 10 03  lea         rcx,[rsp+0000000000000310h]
                    00 00
  00000001405F3F87: FF 15 DB 01 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3F8D: 48 8D BC 24 50 0A  lea         rdi,[rsp+0000000000000A50h]
                    00 00
  00000001405F3F95: 48 8B F0           mov         rsi,rax
  00000001405F3F98: B9 10 00 00 00     mov         ecx,10h
  00000001405F3F9D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3F9F: 45 33 C0           xor         r8d,r8d
  00000001405F3FA2: 33 D2              xor         edx,edx
  00000001405F3FA4: 48 8D 8C 24 20 03  lea         rcx,[rsp+0000000000000320h]
                    00 00
  00000001405F3FAC: FF 15 B6 01 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3FB2: 48 8D BC 24 40 0A  lea         rdi,[rsp+0000000000000A40h]
                    00 00
  00000001405F3FBA: 48 8B F0           mov         rsi,rax
  00000001405F3FBD: B9 10 00 00 00     mov         ecx,10h
  00000001405F3FC2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3FC4: 45 33 C0           xor         r8d,r8d
  00000001405F3FC7: 33 D2              xor         edx,edx
  00000001405F3FC9: 48 8D 8C 24 30 03  lea         rcx,[rsp+0000000000000330h]
                    00 00
  00000001405F3FD1: FF 15 91 01 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3FD7: 48 8D BC 24 30 0A  lea         rdi,[rsp+0000000000000A30h]
                    00 00
  00000001405F3FDF: 48 8B F0           mov         rsi,rax
  00000001405F3FE2: B9 10 00 00 00     mov         ecx,10h
  00000001405F3FE7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F3FE9: 45 33 C0           xor         r8d,r8d
  00000001405F3FEC: 33 D2              xor         edx,edx
  00000001405F3FEE: 48 8D 8C 24 40 03  lea         rcx,[rsp+0000000000000340h]
                    00 00
  00000001405F3FF6: FF 15 6C 01 81 01  call        qword ptr [0000000141E04168h]
  00000001405F3FFC: 48 8D BC 24 20 0A  lea         rdi,[rsp+0000000000000A20h]
                    00 00
  00000001405F4004: 48 8B F0           mov         rsi,rax
  00000001405F4007: B9 10 00 00 00     mov         ecx,10h
  00000001405F400C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F400E: 45 33 C0           xor         r8d,r8d
  00000001405F4011: 33 D2              xor         edx,edx
  00000001405F4013: 48 8D 8C 24 50 03  lea         rcx,[rsp+0000000000000350h]
                    00 00
  00000001405F401B: FF 15 47 01 81 01  call        qword ptr [0000000141E04168h]
  00000001405F4021: 48 8D BC 24 10 0A  lea         rdi,[rsp+0000000000000A10h]
                    00 00
  00000001405F4029: 48 8B F0           mov         rsi,rax
  00000001405F402C: B9 10 00 00 00     mov         ecx,10h
  00000001405F4031: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4033: 45 33 C0           xor         r8d,r8d
  00000001405F4036: 33 D2              xor         edx,edx
  00000001405F4038: 48 8D 8C 24 60 03  lea         rcx,[rsp+0000000000000360h]
                    00 00
  00000001405F4040: FF 15 22 01 81 01  call        qword ptr [0000000141E04168h]
  00000001405F4046: 48 8D BC 24 00 0A  lea         rdi,[rsp+0000000000000A00h]
                    00 00
  00000001405F404E: 48 8B F0           mov         rsi,rax
  00000001405F4051: B9 10 00 00 00     mov         ecx,10h
  00000001405F4056: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4058: 48 8D 84 24 90 0A  lea         rax,[rsp+0000000000000A90h]
                    00 00
  00000001405F4060: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001405F4065: 48 8D 84 24 80 0A  lea         rax,[rsp+0000000000000A80h]
                    00 00
  00000001405F406D: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001405F4072: 48 8D 84 24 70 0A  lea         rax,[rsp+0000000000000A70h]
                    00 00
  00000001405F407A: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001405F407F: 48 8D 84 24 60 0A  lea         rax,[rsp+0000000000000A60h]
                    00 00
  00000001405F4087: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001405F408C: 48 8D 84 24 50 0A  lea         rax,[rsp+0000000000000A50h]
                    00 00
  00000001405F4094: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001405F4099: 48 8D 84 24 40 0A  lea         rax,[rsp+0000000000000A40h]
                    00 00
  00000001405F40A1: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001405F40A6: 48 8D 84 24 30 0A  lea         rax,[rsp+0000000000000A30h]
                    00 00
  00000001405F40AE: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405F40B3: 48 8D 84 24 20 0A  lea         rax,[rsp+0000000000000A20h]
                    00 00
  00000001405F40BB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405F40C0: 4C 8D 8C 24 10 0A  lea         r9,[rsp+0000000000000A10h]
                    00 00
  00000001405F40C8: 4C 8D 84 24 00 0A  lea         r8,[rsp+0000000000000A00h]
                    00 00
  00000001405F40D0: 48 8D 15 79 36 EA  lea         rdx,[0000000142497750h]
                    01
  00000001405F40D7: 48 8D 0D 22 26 78  lea         rcx,[0000000143D76700h]
                    03
  00000001405F40DE: E8 DD B2 CD FF     call        00000001402CF3C0
  00000001405F40E3: 48 8B 8C 24 B0 0F  mov         rcx,qword ptr [rsp+0000000000000FB0h]
                    00 00
  00000001405F40EB: E8 C0 B1 FF FF     call        00000001405EF2B0
  00000001405F40F0: 48 8B C8           mov         rcx,rax
  00000001405F40F3: E8 58 02 A6 FF     call        0000000140054350
  00000001405F40F8: 48 8D 0D 01 26 78  lea         rcx,[0000000143D76700h]
                    03
  00000001405F40FF: E8 FC A8 CD FF     call        00000001402CEA00
  00000001405F4104: 45 33 C0           xor         r8d,r8d
  00000001405F4107: 33 D2              xor         edx,edx
  00000001405F4109: 48 8D 8C 24 70 03  lea         rcx,[rsp+0000000000000370h]
                    00 00
  00000001405F4111: FF 15 51 00 81 01  call        qword ptr [0000000141E04168h]
  00000001405F4117: 48 8D BC 24 30 0B  lea         rdi,[rsp+0000000000000B30h]
                    00 00
  00000001405F411F: 48 8B F0           mov         rsi,rax
  00000001405F4122: B9 10 00 00 00     mov         ecx,10h
  00000001405F4127: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4129: 45 33 C0           xor         r8d,r8d
  00000001405F412C: 33 D2              xor         edx,edx
  00000001405F412E: 48 8D 8C 24 80 03  lea         rcx,[rsp+0000000000000380h]
                    00 00
  00000001405F4136: FF 15 2C 00 81 01  call        qword ptr [0000000141E04168h]
  00000001405F413C: 48 8D BC 24 20 0B  lea         rdi,[rsp+0000000000000B20h]
                    00 00
  00000001405F4144: 48 8B F0           mov         rsi,rax
  00000001405F4147: B9 10 00 00 00     mov         ecx,10h
  00000001405F414C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F414E: 45 33 C0           xor         r8d,r8d
  00000001405F4151: 33 D2              xor         edx,edx
  00000001405F4153: 48 8D 8C 24 90 03  lea         rcx,[rsp+0000000000000390h]
                    00 00
  00000001405F415B: FF 15 07 00 81 01  call        qword ptr [0000000141E04168h]
  00000001405F4161: 48 8D BC 24 10 0B  lea         rdi,[rsp+0000000000000B10h]
                    00 00
  00000001405F4169: 48 8B F0           mov         rsi,rax
  00000001405F416C: B9 10 00 00 00     mov         ecx,10h
  00000001405F4171: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4173: 45 33 C0           xor         r8d,r8d
  00000001405F4176: 33 D2              xor         edx,edx
  00000001405F4178: 48 8D 8C 24 A0 03  lea         rcx,[rsp+00000000000003A0h]
                    00 00
  00000001405F4180: FF 15 E2 FF 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4186: 48 8D BC 24 00 0B  lea         rdi,[rsp+0000000000000B00h]
                    00 00
  00000001405F418E: 48 8B F0           mov         rsi,rax
  00000001405F4191: B9 10 00 00 00     mov         ecx,10h
  00000001405F4196: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4198: 45 33 C0           xor         r8d,r8d
  00000001405F419B: 33 D2              xor         edx,edx
  00000001405F419D: 48 8D 8C 24 B0 03  lea         rcx,[rsp+00000000000003B0h]
                    00 00
  00000001405F41A5: FF 15 BD FF 80 01  call        qword ptr [0000000141E04168h]
  00000001405F41AB: 48 8D BC 24 F0 0A  lea         rdi,[rsp+0000000000000AF0h]
                    00 00
  00000001405F41B3: 48 8B F0           mov         rsi,rax
  00000001405F41B6: B9 10 00 00 00     mov         ecx,10h
  00000001405F41BB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F41BD: 45 33 C0           xor         r8d,r8d
  00000001405F41C0: 33 D2              xor         edx,edx
  00000001405F41C2: 48 8D 8C 24 C0 03  lea         rcx,[rsp+00000000000003C0h]
                    00 00
  00000001405F41CA: FF 15 98 FF 80 01  call        qword ptr [0000000141E04168h]
  00000001405F41D0: 48 8D BC 24 E0 0A  lea         rdi,[rsp+0000000000000AE0h]
                    00 00
  00000001405F41D8: 48 8B F0           mov         rsi,rax
  00000001405F41DB: B9 10 00 00 00     mov         ecx,10h
  00000001405F41E0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F41E2: 45 33 C0           xor         r8d,r8d
  00000001405F41E5: 33 D2              xor         edx,edx
  00000001405F41E7: 48 8D 8C 24 D0 03  lea         rcx,[rsp+00000000000003D0h]
                    00 00
  00000001405F41EF: FF 15 73 FF 80 01  call        qword ptr [0000000141E04168h]
  00000001405F41F5: 48 8D BC 24 D0 0A  lea         rdi,[rsp+0000000000000AD0h]
                    00 00
  00000001405F41FD: 48 8B F0           mov         rsi,rax
  00000001405F4200: B9 10 00 00 00     mov         ecx,10h
  00000001405F4205: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4207: 45 33 C0           xor         r8d,r8d
  00000001405F420A: 33 D2              xor         edx,edx
  00000001405F420C: 48 8D 8C 24 E0 03  lea         rcx,[rsp+00000000000003E0h]
                    00 00
  00000001405F4214: FF 15 4E FF 80 01  call        qword ptr [0000000141E04168h]
  00000001405F421A: 48 8D BC 24 C0 0A  lea         rdi,[rsp+0000000000000AC0h]
                    00 00
  00000001405F4222: 48 8B F0           mov         rsi,rax
  00000001405F4225: B9 10 00 00 00     mov         ecx,10h
  00000001405F422A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F422C: 45 33 C0           xor         r8d,r8d
  00000001405F422F: 33 D2              xor         edx,edx
  00000001405F4231: 48 8D 8C 24 F0 03  lea         rcx,[rsp+00000000000003F0h]
                    00 00
  00000001405F4239: FF 15 29 FF 80 01  call        qword ptr [0000000141E04168h]
  00000001405F423F: 48 8D BC 24 B0 0A  lea         rdi,[rsp+0000000000000AB0h]
                    00 00
  00000001405F4247: 48 8B F0           mov         rsi,rax
  00000001405F424A: B9 10 00 00 00     mov         ecx,10h
  00000001405F424F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4251: 45 33 C0           xor         r8d,r8d
  00000001405F4254: 33 D2              xor         edx,edx
  00000001405F4256: 48 8D 8C 24 00 04  lea         rcx,[rsp+0000000000000400h]
                    00 00
  00000001405F425E: FF 15 04 FF 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4264: 48 8D BC 24 A0 0A  lea         rdi,[rsp+0000000000000AA0h]
                    00 00
  00000001405F426C: 48 8B F0           mov         rsi,rax
  00000001405F426F: B9 10 00 00 00     mov         ecx,10h
  00000001405F4274: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4276: 48 8D 84 24 30 0B  lea         rax,[rsp+0000000000000B30h]
                    00 00
  00000001405F427E: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001405F4283: 48 8D 84 24 20 0B  lea         rax,[rsp+0000000000000B20h]
                    00 00
  00000001405F428B: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001405F4290: 48 8D 84 24 10 0B  lea         rax,[rsp+0000000000000B10h]
                    00 00
  00000001405F4298: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001405F429D: 48 8D 84 24 00 0B  lea         rax,[rsp+0000000000000B00h]
                    00 00
  00000001405F42A5: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001405F42AA: 48 8D 84 24 F0 0A  lea         rax,[rsp+0000000000000AF0h]
                    00 00
  00000001405F42B2: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001405F42B7: 48 8D 84 24 E0 0A  lea         rax,[rsp+0000000000000AE0h]
                    00 00
  00000001405F42BF: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001405F42C4: 48 8D 84 24 D0 0A  lea         rax,[rsp+0000000000000AD0h]
                    00 00
  00000001405F42CC: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405F42D1: 48 8D 84 24 C0 0A  lea         rax,[rsp+0000000000000AC0h]
                    00 00
  00000001405F42D9: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405F42DE: 4C 8D 8C 24 B0 0A  lea         r9,[rsp+0000000000000AB0h]
                    00 00
  00000001405F42E6: 4C 8D 84 24 A0 0A  lea         r8,[rsp+0000000000000AA0h]
                    00 00
  00000001405F42EE: 48 8D 15 83 34 EA  lea         rdx,[0000000142497778h]
                    01
  00000001405F42F5: 48 8D 0D 04 24 78  lea         rcx,[0000000143D76700h]
                    03
  00000001405F42FC: E8 BF B0 CD FF     call        00000001402CF3C0
  00000001405F4301: E9 61 0B 00 00     jmp         00000001405F4E67
  00000001405F4306: 0F B6 84 24 88 00  movzx       eax,byte ptr [rsp+0000000000000088h]
                    00 00
  00000001405F430E: 85 C0              test        eax,eax
  00000001405F4310: 74 1A              je          00000001405F432C
  00000001405F4312: 48 8B 8C 24 B0 0F  mov         rcx,qword ptr [rsp+0000000000000FB0h]
                    00 00
  00000001405F431A: E8 91 AF FF FF     call        00000001405EF2B0
  00000001405F431F: 48 8B C8           mov         rcx,rax
  00000001405F4322: E8 79 00 A6 FF     call        00000001400543A0
  00000001405F4327: E9 FA 03 00 00     jmp         00000001405F4726
  00000001405F432C: 45 33 C0           xor         r8d,r8d
  00000001405F432F: 33 D2              xor         edx,edx
  00000001405F4331: 48 8D 8C 24 10 04  lea         rcx,[rsp+0000000000000410h]
                    00 00
  00000001405F4339: FF 15 29 FE 80 01  call        qword ptr [0000000141E04168h]
  00000001405F433F: 48 8D BC 24 D0 0B  lea         rdi,[rsp+0000000000000BD0h]
                    00 00
  00000001405F4347: 48 8B F0           mov         rsi,rax
  00000001405F434A: B9 10 00 00 00     mov         ecx,10h
  00000001405F434F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4351: 45 33 C0           xor         r8d,r8d
  00000001405F4354: 33 D2              xor         edx,edx
  00000001405F4356: 48 8D 8C 24 20 04  lea         rcx,[rsp+0000000000000420h]
                    00 00
  00000001405F435E: FF 15 04 FE 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4364: 48 8D BC 24 C0 0B  lea         rdi,[rsp+0000000000000BC0h]
                    00 00
  00000001405F436C: 48 8B F0           mov         rsi,rax
  00000001405F436F: B9 10 00 00 00     mov         ecx,10h
  00000001405F4374: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4376: 45 33 C0           xor         r8d,r8d
  00000001405F4379: 33 D2              xor         edx,edx
  00000001405F437B: 48 8D 8C 24 30 04  lea         rcx,[rsp+0000000000000430h]
                    00 00
  00000001405F4383: FF 15 DF FD 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4389: 48 8D BC 24 B0 0B  lea         rdi,[rsp+0000000000000BB0h]
                    00 00
  00000001405F4391: 48 8B F0           mov         rsi,rax
  00000001405F4394: B9 10 00 00 00     mov         ecx,10h
  00000001405F4399: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F439B: 45 33 C0           xor         r8d,r8d
  00000001405F439E: 33 D2              xor         edx,edx
  00000001405F43A0: 48 8D 8C 24 40 04  lea         rcx,[rsp+0000000000000440h]
                    00 00
  00000001405F43A8: FF 15 BA FD 80 01  call        qword ptr [0000000141E04168h]
  00000001405F43AE: 48 8D BC 24 A0 0B  lea         rdi,[rsp+0000000000000BA0h]
                    00 00
  00000001405F43B6: 48 8B F0           mov         rsi,rax
  00000001405F43B9: B9 10 00 00 00     mov         ecx,10h
  00000001405F43BE: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F43C0: 45 33 C0           xor         r8d,r8d
  00000001405F43C3: 33 D2              xor         edx,edx
  00000001405F43C5: 48 8D 8C 24 50 04  lea         rcx,[rsp+0000000000000450h]
                    00 00
  00000001405F43CD: FF 15 95 FD 80 01  call        qword ptr [0000000141E04168h]
  00000001405F43D3: 48 8D BC 24 90 0B  lea         rdi,[rsp+0000000000000B90h]
                    00 00
  00000001405F43DB: 48 8B F0           mov         rsi,rax
  00000001405F43DE: B9 10 00 00 00     mov         ecx,10h
  00000001405F43E3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F43E5: 45 33 C0           xor         r8d,r8d
  00000001405F43E8: 33 D2              xor         edx,edx
  00000001405F43EA: 48 8D 8C 24 60 04  lea         rcx,[rsp+0000000000000460h]
                    00 00
  00000001405F43F2: FF 15 70 FD 80 01  call        qword ptr [0000000141E04168h]
  00000001405F43F8: 48 8D BC 24 80 0B  lea         rdi,[rsp+0000000000000B80h]
                    00 00
  00000001405F4400: 48 8B F0           mov         rsi,rax
  00000001405F4403: B9 10 00 00 00     mov         ecx,10h
  00000001405F4408: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F440A: 45 33 C0           xor         r8d,r8d
  00000001405F440D: 33 D2              xor         edx,edx
  00000001405F440F: 48 8D 8C 24 70 04  lea         rcx,[rsp+0000000000000470h]
                    00 00
  00000001405F4417: FF 15 4B FD 80 01  call        qword ptr [0000000141E04168h]
  00000001405F441D: 48 8D BC 24 70 0B  lea         rdi,[rsp+0000000000000B70h]
                    00 00
  00000001405F4425: 48 8B F0           mov         rsi,rax
  00000001405F4428: B9 10 00 00 00     mov         ecx,10h
  00000001405F442D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F442F: 45 33 C0           xor         r8d,r8d
  00000001405F4432: 33 D2              xor         edx,edx
  00000001405F4434: 48 8D 8C 24 80 04  lea         rcx,[rsp+0000000000000480h]
                    00 00
  00000001405F443C: FF 15 26 FD 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4442: 48 8D BC 24 60 0B  lea         rdi,[rsp+0000000000000B60h]
                    00 00
  00000001405F444A: 48 8B F0           mov         rsi,rax
  00000001405F444D: B9 10 00 00 00     mov         ecx,10h
  00000001405F4452: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4454: 45 33 C0           xor         r8d,r8d
  00000001405F4457: 33 D2              xor         edx,edx
  00000001405F4459: 48 8D 8C 24 90 04  lea         rcx,[rsp+0000000000000490h]
                    00 00
  00000001405F4461: FF 15 01 FD 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4467: 48 8D BC 24 50 0B  lea         rdi,[rsp+0000000000000B50h]
                    00 00
  00000001405F446F: 48 8B F0           mov         rsi,rax
  00000001405F4472: B9 10 00 00 00     mov         ecx,10h
  00000001405F4477: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4479: 45 33 C0           xor         r8d,r8d
  00000001405F447C: 33 D2              xor         edx,edx
  00000001405F447E: 48 8D 8C 24 A0 04  lea         rcx,[rsp+00000000000004A0h]
                    00 00
  00000001405F4486: FF 15 DC FC 80 01  call        qword ptr [0000000141E04168h]
  00000001405F448C: 48 8D BC 24 40 0B  lea         rdi,[rsp+0000000000000B40h]
                    00 00
  00000001405F4494: 48 8B F0           mov         rsi,rax
  00000001405F4497: B9 10 00 00 00     mov         ecx,10h
  00000001405F449C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F449E: 48 8D 84 24 D0 0B  lea         rax,[rsp+0000000000000BD0h]
                    00 00
  00000001405F44A6: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001405F44AB: 48 8D 84 24 C0 0B  lea         rax,[rsp+0000000000000BC0h]
                    00 00
  00000001405F44B3: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001405F44B8: 48 8D 84 24 B0 0B  lea         rax,[rsp+0000000000000BB0h]
                    00 00
  00000001405F44C0: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001405F44C5: 48 8D 84 24 A0 0B  lea         rax,[rsp+0000000000000BA0h]
                    00 00
  00000001405F44CD: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001405F44D2: 48 8D 84 24 90 0B  lea         rax,[rsp+0000000000000B90h]
                    00 00
  00000001405F44DA: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001405F44DF: 48 8D 84 24 80 0B  lea         rax,[rsp+0000000000000B80h]
                    00 00
  00000001405F44E7: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001405F44EC: 48 8D 84 24 70 0B  lea         rax,[rsp+0000000000000B70h]
                    00 00
  00000001405F44F4: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405F44F9: 48 8D 84 24 60 0B  lea         rax,[rsp+0000000000000B60h]
                    00 00
  00000001405F4501: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405F4506: 4C 8D 8C 24 50 0B  lea         r9,[rsp+0000000000000B50h]
                    00 00
  00000001405F450E: 4C 8D 84 24 40 0B  lea         r8,[rsp+0000000000000B40h]
                    00 00
  00000001405F4516: 48 8D 15 7B 32 EA  lea         rdx,[0000000142497798h]
                    01
  00000001405F451D: 48 8D 0D DC 21 78  lea         rcx,[0000000143D76700h]
                    03
  00000001405F4524: E8 97 AE CD FF     call        00000001402CF3C0
  00000001405F4529: 45 33 C0           xor         r8d,r8d
  00000001405F452C: 33 D2              xor         edx,edx
  00000001405F452E: 48 8D 8C 24 B0 04  lea         rcx,[rsp+00000000000004B0h]
                    00 00
  00000001405F4536: FF 15 2C FC 80 01  call        qword ptr [0000000141E04168h]
  00000001405F453C: 48 8D BC 24 70 0C  lea         rdi,[rsp+0000000000000C70h]
                    00 00
  00000001405F4544: 48 8B F0           mov         rsi,rax
  00000001405F4547: B9 10 00 00 00     mov         ecx,10h
  00000001405F454C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F454E: 45 33 C0           xor         r8d,r8d
  00000001405F4551: 33 D2              xor         edx,edx
  00000001405F4553: 48 8D 8C 24 C0 04  lea         rcx,[rsp+00000000000004C0h]
                    00 00
  00000001405F455B: FF 15 07 FC 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4561: 48 8D BC 24 60 0C  lea         rdi,[rsp+0000000000000C60h]
                    00 00
  00000001405F4569: 48 8B F0           mov         rsi,rax
  00000001405F456C: B9 10 00 00 00     mov         ecx,10h
  00000001405F4571: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4573: 45 33 C0           xor         r8d,r8d
  00000001405F4576: 33 D2              xor         edx,edx
  00000001405F4578: 48 8D 8C 24 D0 04  lea         rcx,[rsp+00000000000004D0h]
                    00 00
  00000001405F4580: FF 15 E2 FB 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4586: 48 8D BC 24 50 0C  lea         rdi,[rsp+0000000000000C50h]
                    00 00
  00000001405F458E: 48 8B F0           mov         rsi,rax
  00000001405F4591: B9 10 00 00 00     mov         ecx,10h
  00000001405F4596: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4598: 45 33 C0           xor         r8d,r8d
  00000001405F459B: 33 D2              xor         edx,edx
  00000001405F459D: 48 8D 8C 24 E0 04  lea         rcx,[rsp+00000000000004E0h]
                    00 00
  00000001405F45A5: FF 15 BD FB 80 01  call        qword ptr [0000000141E04168h]
  00000001405F45AB: 48 8D BC 24 40 0C  lea         rdi,[rsp+0000000000000C40h]
                    00 00
  00000001405F45B3: 48 8B F0           mov         rsi,rax
  00000001405F45B6: B9 10 00 00 00     mov         ecx,10h
  00000001405F45BB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F45BD: 45 33 C0           xor         r8d,r8d
  00000001405F45C0: 33 D2              xor         edx,edx
  00000001405F45C2: 48 8D 8C 24 F0 04  lea         rcx,[rsp+00000000000004F0h]
                    00 00
  00000001405F45CA: FF 15 98 FB 80 01  call        qword ptr [0000000141E04168h]
  00000001405F45D0: 48 8D BC 24 30 0C  lea         rdi,[rsp+0000000000000C30h]
                    00 00
  00000001405F45D8: 48 8B F0           mov         rsi,rax
  00000001405F45DB: B9 10 00 00 00     mov         ecx,10h
  00000001405F45E0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F45E2: 45 33 C0           xor         r8d,r8d
  00000001405F45E5: 33 D2              xor         edx,edx
  00000001405F45E7: 48 8D 8C 24 00 05  lea         rcx,[rsp+0000000000000500h]
                    00 00
  00000001405F45EF: FF 15 73 FB 80 01  call        qword ptr [0000000141E04168h]
  00000001405F45F5: 48 8D BC 24 20 0C  lea         rdi,[rsp+0000000000000C20h]
                    00 00
  00000001405F45FD: 48 8B F0           mov         rsi,rax
  00000001405F4600: B9 10 00 00 00     mov         ecx,10h
  00000001405F4605: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4607: 45 33 C0           xor         r8d,r8d
  00000001405F460A: 33 D2              xor         edx,edx
  00000001405F460C: 48 8D 8C 24 10 05  lea         rcx,[rsp+0000000000000510h]
                    00 00
  00000001405F4614: FF 15 4E FB 80 01  call        qword ptr [0000000141E04168h]
  00000001405F461A: 48 8D BC 24 10 0C  lea         rdi,[rsp+0000000000000C10h]
                    00 00
  00000001405F4622: 48 8B F0           mov         rsi,rax
  00000001405F4625: B9 10 00 00 00     mov         ecx,10h
  00000001405F462A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F462C: 45 33 C0           xor         r8d,r8d
  00000001405F462F: 33 D2              xor         edx,edx
  00000001405F4631: 48 8D 8C 24 20 05  lea         rcx,[rsp+0000000000000520h]
                    00 00
  00000001405F4639: FF 15 29 FB 80 01  call        qword ptr [0000000141E04168h]
  00000001405F463F: 48 8D BC 24 00 0C  lea         rdi,[rsp+0000000000000C00h]
                    00 00
  00000001405F4647: 48 8B F0           mov         rsi,rax
  00000001405F464A: B9 10 00 00 00     mov         ecx,10h
  00000001405F464F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4651: 45 33 C0           xor         r8d,r8d
  00000001405F4654: 33 D2              xor         edx,edx
  00000001405F4656: 48 8D 8C 24 30 05  lea         rcx,[rsp+0000000000000530h]
                    00 00
  00000001405F465E: FF 15 04 FB 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4664: 48 8D BC 24 F0 0B  lea         rdi,[rsp+0000000000000BF0h]
                    00 00
  00000001405F466C: 48 8B F0           mov         rsi,rax
  00000001405F466F: B9 10 00 00 00     mov         ecx,10h
  00000001405F4674: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4676: 45 33 C0           xor         r8d,r8d
  00000001405F4679: 33 D2              xor         edx,edx
  00000001405F467B: 48 8D 8C 24 40 05  lea         rcx,[rsp+0000000000000540h]
                    00 00
  00000001405F4683: FF 15 DF FA 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4689: 48 8D BC 24 E0 0B  lea         rdi,[rsp+0000000000000BE0h]
                    00 00
  00000001405F4691: 48 8B F0           mov         rsi,rax
  00000001405F4694: B9 10 00 00 00     mov         ecx,10h
  00000001405F4699: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F469B: 48 8D 84 24 70 0C  lea         rax,[rsp+0000000000000C70h]
                    00 00
  00000001405F46A3: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001405F46A8: 48 8D 84 24 60 0C  lea         rax,[rsp+0000000000000C60h]
                    00 00
  00000001405F46B0: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001405F46B5: 48 8D 84 24 50 0C  lea         rax,[rsp+0000000000000C50h]
                    00 00
  00000001405F46BD: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001405F46C2: 48 8D 84 24 40 0C  lea         rax,[rsp+0000000000000C40h]
                    00 00
  00000001405F46CA: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001405F46CF: 48 8D 84 24 30 0C  lea         rax,[rsp+0000000000000C30h]
                    00 00
  00000001405F46D7: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001405F46DC: 48 8D 84 24 20 0C  lea         rax,[rsp+0000000000000C20h]
                    00 00
  00000001405F46E4: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001405F46E9: 48 8D 84 24 10 0C  lea         rax,[rsp+0000000000000C10h]
                    00 00
  00000001405F46F1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405F46F6: 48 8D 84 24 00 0C  lea         rax,[rsp+0000000000000C00h]
                    00 00
  00000001405F46FE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405F4703: 4C 8D 8C 24 F0 0B  lea         r9,[rsp+0000000000000BF0h]
                    00 00
  00000001405F470B: 4C 8D 84 24 E0 0B  lea         r8,[rsp+0000000000000BE0h]
                    00 00
  00000001405F4713: 48 8D 15 9E 30 EA  lea         rdx,[00000001424977B8h]
                    01
  00000001405F471A: 48 8D 0D DF 1F 78  lea         rcx,[0000000143D76700h]
                    03
  00000001405F4721: E8 9A AC CD FF     call        00000001402CF3C0
  00000001405F4726: 48 8B 8C 24 B0 0F  mov         rcx,qword ptr [rsp+0000000000000FB0h]
                    00 00
  00000001405F472E: E8 4D AC FF FF     call        00000001405EF380
  00000001405F4733: 48 89 84 24 70 0F  mov         qword ptr [rsp+0000000000000F70h],rax
                    00 00
  00000001405F473B: 48 8B 84 24 70 0F  mov         rax,qword ptr [rsp+0000000000000F70h]
                    00 00
  00000001405F4743: 48 8B 00           mov         rax,qword ptr [rax]
  00000001405F4746: 48 8B 94 24 B8 0F  mov         rdx,qword ptr [rsp+0000000000000FB8h]
                    00 00
  00000001405F474E: 48 8B 8C 24 70 0F  mov         rcx,qword ptr [rsp+0000000000000F70h]
                    00 00
  00000001405F4756: FF 50 08           call        qword ptr [rax+8]
  00000001405F4759: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  00000001405F4761: 48 83 BC 24 98 00  cmp         qword ptr [rsp+0000000000000098h],0
                    00 00 00
  00000001405F476A: 0F 84 39 02 00 00  je          00000001405F49A9
  00000001405F4770: 4C 8D 84 24 98 00  lea         r8,[rsp+0000000000000098h]
                    00 00
  00000001405F4778: 48 8D 15 61 30 EA  lea         rdx,[00000001424977E0h]
                    01
  00000001405F477F: 48 8D 8C 24 F0 05  lea         rcx,[rsp+00000000000005F0h]
                    00 00
  00000001405F4787: E8 F4 08 A3 FF     call        0000000140025080
  00000001405F478C: 48 8D BC 24 20 0F  lea         rdi,[rsp+0000000000000F20h]
                    00 00
  00000001405F4794: 48 8B F0           mov         rsi,rax
  00000001405F4797: B9 10 00 00 00     mov         ecx,10h
  00000001405F479C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F479E: 48 8D BC 24 E0 05  lea         rdi,[rsp+00000000000005E0h]
                    00 00
  00000001405F47A6: 48 8D B4 24 20 0F  lea         rsi,[rsp+0000000000000F20h]
                    00 00
  00000001405F47AE: B9 10 00 00 00     mov         ecx,10h
  00000001405F47B3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F47B5: 45 33 C0           xor         r8d,r8d
  00000001405F47B8: 33 D2              xor         edx,edx
  00000001405F47BA: 48 8D 8C 24 50 05  lea         rcx,[rsp+0000000000000550h]
                    00 00
  00000001405F47C2: FF 15 A0 F9 80 01  call        qword ptr [0000000141E04168h]
  00000001405F47C8: 48 8D BC 24 10 0D  lea         rdi,[rsp+0000000000000D10h]
                    00 00
  00000001405F47D0: 48 8B F0           mov         rsi,rax
  00000001405F47D3: B9 10 00 00 00     mov         ecx,10h
  00000001405F47D8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F47DA: 45 33 C0           xor         r8d,r8d
  00000001405F47DD: 33 D2              xor         edx,edx
  00000001405F47DF: 48 8D 8C 24 60 05  lea         rcx,[rsp+0000000000000560h]
                    00 00
  00000001405F47E7: FF 15 7B F9 80 01  call        qword ptr [0000000141E04168h]
  00000001405F47ED: 48 8D BC 24 00 0D  lea         rdi,[rsp+0000000000000D00h]
                    00 00
  00000001405F47F5: 48 8B F0           mov         rsi,rax
  00000001405F47F8: B9 10 00 00 00     mov         ecx,10h
  00000001405F47FD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F47FF: 45 33 C0           xor         r8d,r8d
  00000001405F4802: 33 D2              xor         edx,edx
  00000001405F4804: 48 8D 8C 24 70 05  lea         rcx,[rsp+0000000000000570h]
                    00 00
  00000001405F480C: FF 15 56 F9 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4812: 48 8D BC 24 F0 0C  lea         rdi,[rsp+0000000000000CF0h]
                    00 00
  00000001405F481A: 48 8B F0           mov         rsi,rax
  00000001405F481D: B9 10 00 00 00     mov         ecx,10h
  00000001405F4822: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4824: 45 33 C0           xor         r8d,r8d
  00000001405F4827: 33 D2              xor         edx,edx
  00000001405F4829: 48 8D 8C 24 80 05  lea         rcx,[rsp+0000000000000580h]
                    00 00
  00000001405F4831: FF 15 31 F9 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4837: 48 8D BC 24 E0 0C  lea         rdi,[rsp+0000000000000CE0h]
                    00 00
  00000001405F483F: 48 8B F0           mov         rsi,rax
  00000001405F4842: B9 10 00 00 00     mov         ecx,10h
  00000001405F4847: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4849: 45 33 C0           xor         r8d,r8d
  00000001405F484C: 33 D2              xor         edx,edx
  00000001405F484E: 48 8D 8C 24 90 05  lea         rcx,[rsp+0000000000000590h]
                    00 00
  00000001405F4856: FF 15 0C F9 80 01  call        qword ptr [0000000141E04168h]
  00000001405F485C: 48 8D BC 24 D0 0C  lea         rdi,[rsp+0000000000000CD0h]
                    00 00
  00000001405F4864: 48 8B F0           mov         rsi,rax
  00000001405F4867: B9 10 00 00 00     mov         ecx,10h
  00000001405F486C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F486E: 45 33 C0           xor         r8d,r8d
  00000001405F4871: 33 D2              xor         edx,edx
  00000001405F4873: 48 8D 8C 24 A0 05  lea         rcx,[rsp+00000000000005A0h]
                    00 00
  00000001405F487B: FF 15 E7 F8 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4881: 48 8D BC 24 C0 0C  lea         rdi,[rsp+0000000000000CC0h]
                    00 00
  00000001405F4889: 48 8B F0           mov         rsi,rax
  00000001405F488C: B9 10 00 00 00     mov         ecx,10h
  00000001405F4891: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4893: 45 33 C0           xor         r8d,r8d
  00000001405F4896: 33 D2              xor         edx,edx
  00000001405F4898: 48 8D 8C 24 B0 05  lea         rcx,[rsp+00000000000005B0h]
                    00 00
  00000001405F48A0: FF 15 C2 F8 80 01  call        qword ptr [0000000141E04168h]
  00000001405F48A6: 48 8D BC 24 B0 0C  lea         rdi,[rsp+0000000000000CB0h]
                    00 00
  00000001405F48AE: 48 8B F0           mov         rsi,rax
  00000001405F48B1: B9 10 00 00 00     mov         ecx,10h
  00000001405F48B6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F48B8: 45 33 C0           xor         r8d,r8d
  00000001405F48BB: 33 D2              xor         edx,edx
  00000001405F48BD: 48 8D 8C 24 C0 05  lea         rcx,[rsp+00000000000005C0h]
                    00 00
  00000001405F48C5: FF 15 9D F8 80 01  call        qword ptr [0000000141E04168h]
  00000001405F48CB: 48 8D BC 24 A0 0C  lea         rdi,[rsp+0000000000000CA0h]
                    00 00
  00000001405F48D3: 48 8B F0           mov         rsi,rax
  00000001405F48D6: B9 10 00 00 00     mov         ecx,10h
  00000001405F48DB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F48DD: 45 33 C0           xor         r8d,r8d
  00000001405F48E0: 33 D2              xor         edx,edx
  00000001405F48E2: 48 8D 8C 24 D0 05  lea         rcx,[rsp+00000000000005D0h]
                    00 00
  00000001405F48EA: FF 15 78 F8 80 01  call        qword ptr [0000000141E04168h]
  00000001405F48F0: 48 8D BC 24 90 0C  lea         rdi,[rsp+0000000000000C90h]
                    00 00
  00000001405F48F8: 48 8B F0           mov         rsi,rax
  00000001405F48FB: B9 10 00 00 00     mov         ecx,10h
  00000001405F4900: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4902: 48 8D BC 24 80 0C  lea         rdi,[rsp+0000000000000C80h]
                    00 00
  00000001405F490A: 48 8D B4 24 E0 05  lea         rsi,[rsp+00000000000005E0h]
                    00 00
  00000001405F4912: B9 10 00 00 00     mov         ecx,10h
  00000001405F4917: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4919: 48 8D 84 24 10 0D  lea         rax,[rsp+0000000000000D10h]
                    00 00
  00000001405F4921: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001405F4926: 48 8D 84 24 00 0D  lea         rax,[rsp+0000000000000D00h]
                    00 00
  00000001405F492E: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001405F4933: 48 8D 84 24 F0 0C  lea         rax,[rsp+0000000000000CF0h]
                    00 00
  00000001405F493B: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001405F4940: 48 8D 84 24 E0 0C  lea         rax,[rsp+0000000000000CE0h]
                    00 00
  00000001405F4948: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001405F494D: 48 8D 84 24 D0 0C  lea         rax,[rsp+0000000000000CD0h]
                    00 00
  00000001405F4955: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001405F495A: 48 8D 84 24 C0 0C  lea         rax,[rsp+0000000000000CC0h]
                    00 00
  00000001405F4962: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001405F4967: 48 8D 84 24 B0 0C  lea         rax,[rsp+0000000000000CB0h]
                    00 00
  00000001405F496F: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405F4974: 48 8D 84 24 A0 0C  lea         rax,[rsp+0000000000000CA0h]
                    00 00
  00000001405F497C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405F4981: 4C 8D 8C 24 90 0C  lea         r9,[rsp+0000000000000C90h]
                    00 00
  00000001405F4989: 4C 8D 84 24 80 0C  lea         r8,[rsp+0000000000000C80h]
                    00 00
  00000001405F4991: 48 8D 15 58 2E EA  lea         rdx,[00000001424977F0h]
                    01
  00000001405F4998: 48 8D 0D 61 1D 78  lea         rcx,[0000000143D76700h]
                    03
  00000001405F499F: E8 1C AA CD FF     call        00000001402CF3C0
  00000001405F49A4: E9 8A 02 00 00     jmp         00000001405F4C33
  00000001405F49A9: 48 C7 84 24 B0 06  mov         qword ptr [rsp+00000000000006B0h],0
                    00 00 00 00 00 00
  00000001405F49B5: 4C 8D 84 24 B0 06  lea         r8,[rsp+00000000000006B0h]
                    00 00
  00000001405F49BD: 48 8D 15 4C 2E EA  lea         rdx,[0000000142497810h]
                    01
  00000001405F49C4: 48 8D 8C 24 A0 06  lea         rcx,[rsp+00000000000006A0h]
                    00 00
  00000001405F49CC: E8 AF 06 A3 FF     call        0000000140025080
  00000001405F49D1: 48 8D BC 24 10 0F  lea         rdi,[rsp+0000000000000F10h]
                    00 00
  00000001405F49D9: 48 8B F0           mov         rsi,rax
  00000001405F49DC: B9 10 00 00 00     mov         ecx,10h
  00000001405F49E1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F49E3: 48 8D BC 24 90 06  lea         rdi,[rsp+0000000000000690h]
                    00 00
  00000001405F49EB: 48 8D B4 24 10 0F  lea         rsi,[rsp+0000000000000F10h]
                    00 00
  00000001405F49F3: B9 10 00 00 00     mov         ecx,10h
  00000001405F49F8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F49FA: 45 33 C0           xor         r8d,r8d
  00000001405F49FD: 33 D2              xor         edx,edx
  00000001405F49FF: 48 8D 8C 24 00 06  lea         rcx,[rsp+0000000000000600h]
                    00 00
  00000001405F4A07: FF 15 5B F7 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4A0D: 48 8D BC 24 B0 0D  lea         rdi,[rsp+0000000000000DB0h]
                    00 00
  00000001405F4A15: 48 8B F0           mov         rsi,rax
  00000001405F4A18: B9 10 00 00 00     mov         ecx,10h
  00000001405F4A1D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4A1F: 45 33 C0           xor         r8d,r8d
  00000001405F4A22: 33 D2              xor         edx,edx
  00000001405F4A24: 48 8D 8C 24 10 06  lea         rcx,[rsp+0000000000000610h]
                    00 00
  00000001405F4A2C: FF 15 36 F7 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4A32: 48 8D BC 24 A0 0D  lea         rdi,[rsp+0000000000000DA0h]
                    00 00
  00000001405F4A3A: 48 8B F0           mov         rsi,rax
  00000001405F4A3D: B9 10 00 00 00     mov         ecx,10h
  00000001405F4A42: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4A44: 45 33 C0           xor         r8d,r8d
  00000001405F4A47: 33 D2              xor         edx,edx
  00000001405F4A49: 48 8D 8C 24 20 06  lea         rcx,[rsp+0000000000000620h]
                    00 00
  00000001405F4A51: FF 15 11 F7 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4A57: 48 8D BC 24 90 0D  lea         rdi,[rsp+0000000000000D90h]
                    00 00
  00000001405F4A5F: 48 8B F0           mov         rsi,rax
  00000001405F4A62: B9 10 00 00 00     mov         ecx,10h
  00000001405F4A67: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4A69: 45 33 C0           xor         r8d,r8d
  00000001405F4A6C: 33 D2              xor         edx,edx
  00000001405F4A6E: 48 8D 8C 24 30 06  lea         rcx,[rsp+0000000000000630h]
                    00 00
  00000001405F4A76: FF 15 EC F6 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4A7C: 48 8D BC 24 80 0D  lea         rdi,[rsp+0000000000000D80h]
                    00 00
  00000001405F4A84: 48 8B F0           mov         rsi,rax
  00000001405F4A87: B9 10 00 00 00     mov         ecx,10h
  00000001405F4A8C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4A8E: 45 33 C0           xor         r8d,r8d
  00000001405F4A91: 33 D2              xor         edx,edx
  00000001405F4A93: 48 8D 8C 24 40 06  lea         rcx,[rsp+0000000000000640h]
                    00 00
  00000001405F4A9B: FF 15 C7 F6 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4AA1: 48 8D BC 24 70 0D  lea         rdi,[rsp+0000000000000D70h]
                    00 00
  00000001405F4AA9: 48 8B F0           mov         rsi,rax
  00000001405F4AAC: B9 10 00 00 00     mov         ecx,10h
  00000001405F4AB1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4AB3: 45 33 C0           xor         r8d,r8d
  00000001405F4AB6: 33 D2              xor         edx,edx
  00000001405F4AB8: 48 8D 8C 24 50 06  lea         rcx,[rsp+0000000000000650h]
                    00 00
  00000001405F4AC0: FF 15 A2 F6 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4AC6: 48 8D BC 24 60 0D  lea         rdi,[rsp+0000000000000D60h]
                    00 00
  00000001405F4ACE: 48 8B F0           mov         rsi,rax
  00000001405F4AD1: B9 10 00 00 00     mov         ecx,10h
  00000001405F4AD6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4AD8: 45 33 C0           xor         r8d,r8d
  00000001405F4ADB: 33 D2              xor         edx,edx
  00000001405F4ADD: 48 8D 8C 24 60 06  lea         rcx,[rsp+0000000000000660h]
                    00 00
  00000001405F4AE5: FF 15 7D F6 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4AEB: 48 8D BC 24 50 0D  lea         rdi,[rsp+0000000000000D50h]
                    00 00
  00000001405F4AF3: 48 8B F0           mov         rsi,rax
  00000001405F4AF6: B9 10 00 00 00     mov         ecx,10h
  00000001405F4AFB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4AFD: 45 33 C0           xor         r8d,r8d
  00000001405F4B00: 33 D2              xor         edx,edx
  00000001405F4B02: 48 8D 8C 24 70 06  lea         rcx,[rsp+0000000000000670h]
                    00 00
  00000001405F4B0A: FF 15 58 F6 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4B10: 48 8D BC 24 40 0D  lea         rdi,[rsp+0000000000000D40h]
                    00 00
  00000001405F4B18: 48 8B F0           mov         rsi,rax
  00000001405F4B1B: B9 10 00 00 00     mov         ecx,10h
  00000001405F4B20: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4B22: 45 33 C0           xor         r8d,r8d
  00000001405F4B25: 33 D2              xor         edx,edx
  00000001405F4B27: 48 8D 8C 24 80 06  lea         rcx,[rsp+0000000000000680h]
                    00 00
  00000001405F4B2F: FF 15 33 F6 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4B35: 48 8D BC 24 30 0D  lea         rdi,[rsp+0000000000000D30h]
                    00 00
  00000001405F4B3D: 48 8B F0           mov         rsi,rax
  00000001405F4B40: B9 10 00 00 00     mov         ecx,10h
  00000001405F4B45: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4B47: 48 8D BC 24 20 0D  lea         rdi,[rsp+0000000000000D20h]
                    00 00
  00000001405F4B4F: 48 8D B4 24 90 06  lea         rsi,[rsp+0000000000000690h]
                    00 00
  00000001405F4B57: B9 10 00 00 00     mov         ecx,10h
  00000001405F4B5C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4B5E: 48 8D 84 24 B0 0D  lea         rax,[rsp+0000000000000DB0h]
                    00 00
  00000001405F4B66: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001405F4B6B: 48 8D 84 24 A0 0D  lea         rax,[rsp+0000000000000DA0h]
                    00 00
  00000001405F4B73: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001405F4B78: 48 8D 84 24 90 0D  lea         rax,[rsp+0000000000000D90h]
                    00 00
  00000001405F4B80: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001405F4B85: 48 8D 84 24 80 0D  lea         rax,[rsp+0000000000000D80h]
                    00 00
  00000001405F4B8D: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001405F4B92: 48 8D 84 24 70 0D  lea         rax,[rsp+0000000000000D70h]
                    00 00
  00000001405F4B9A: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001405F4B9F: 48 8D 84 24 60 0D  lea         rax,[rsp+0000000000000D60h]
                    00 00
  00000001405F4BA7: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001405F4BAC: 48 8D 84 24 50 0D  lea         rax,[rsp+0000000000000D50h]
                    00 00
  00000001405F4BB4: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405F4BB9: 48 8D 84 24 40 0D  lea         rax,[rsp+0000000000000D40h]
                    00 00
  00000001405F4BC1: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405F4BC6: 4C 8D 8C 24 30 0D  lea         r9,[rsp+0000000000000D30h]
                    00 00
  00000001405F4BCE: 4C 8D 84 24 20 0D  lea         r8,[rsp+0000000000000D20h]
                    00 00
  00000001405F4BD6: 48 8D 15 43 2C EA  lea         rdx,[0000000142497820h]
                    01
  00000001405F4BDD: 48 8D 0D 1C 1B 78  lea         rcx,[0000000143D76700h]
                    03
  00000001405F4BE4: E8 D7 A7 CD FF     call        00000001402CF3C0
  00000001405F4BE9: 48 8D 8C 24 B8 06  lea         rcx,[rsp+00000000000006B8h]
                    00 00
  00000001405F4BF1: FF 15 91 F7 80 01  call        qword ptr [0000000141E04388h]
  00000001405F4BF7: 48 89 84 24 78 0F  mov         qword ptr [rsp+0000000000000F78h],rax
                    00 00
  00000001405F4BFF: 48 8B 84 24 78 0F  mov         rax,qword ptr [rsp+0000000000000F78h]
                    00 00
  00000001405F4C07: 48 89 84 24 80 0F  mov         qword ptr [rsp+0000000000000F80h],rax
                    00 00
  00000001405F4C0F: 48 8D 15 2A 2C EA  lea         rdx,[0000000142497840h]
                    01
  00000001405F4C16: 48 8B 8C 24 80 0F  mov         rcx,qword ptr [rsp+0000000000000F80h]
                    00 00
  00000001405F4C1E: FF 15 4C F7 80 01  call        qword ptr [0000000141E04370h]
  00000001405F4C24: 90                 nop
  00000001405F4C25: 48 8D 8C 24 B8 06  lea         rcx,[rsp+00000000000006B8h]
                    00 00
  00000001405F4C2D: FF 15 45 F7 80 01  call        qword ptr [0000000141E04378h]
  00000001405F4C33: 4C 8D 84 24 98 00  lea         r8,[rsp+0000000000000098h]
                    00 00
  00000001405F4C3B: 48 8D 15 26 2C EA  lea         rdx,[0000000142497868h]
                    01
  00000001405F4C42: 48 8D 8C 24 60 07  lea         rcx,[rsp+0000000000000760h]
                    00 00
  00000001405F4C4A: E8 31 04 A3 FF     call        0000000140025080
  00000001405F4C4F: 48 8D BC 24 00 0F  lea         rdi,[rsp+0000000000000F00h]
                    00 00
  00000001405F4C57: 48 8B F0           mov         rsi,rax
  00000001405F4C5A: B9 10 00 00 00     mov         ecx,10h
  00000001405F4C5F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4C61: 48 8D BC 24 50 07  lea         rdi,[rsp+0000000000000750h]
                    00 00
  00000001405F4C69: 48 8D B4 24 00 0F  lea         rsi,[rsp+0000000000000F00h]
                    00 00
  00000001405F4C71: B9 10 00 00 00     mov         ecx,10h
  00000001405F4C76: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4C78: 45 33 C0           xor         r8d,r8d
  00000001405F4C7B: 33 D2              xor         edx,edx
  00000001405F4C7D: 48 8D 8C 24 C0 06  lea         rcx,[rsp+00000000000006C0h]
                    00 00
  00000001405F4C85: FF 15 DD F4 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4C8B: 48 8D BC 24 50 0E  lea         rdi,[rsp+0000000000000E50h]
                    00 00
  00000001405F4C93: 48 8B F0           mov         rsi,rax
  00000001405F4C96: B9 10 00 00 00     mov         ecx,10h
  00000001405F4C9B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4C9D: 45 33 C0           xor         r8d,r8d
  00000001405F4CA0: 33 D2              xor         edx,edx
  00000001405F4CA2: 48 8D 8C 24 D0 06  lea         rcx,[rsp+00000000000006D0h]
                    00 00
  00000001405F4CAA: FF 15 B8 F4 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4CB0: 48 8D BC 24 40 0E  lea         rdi,[rsp+0000000000000E40h]
                    00 00
  00000001405F4CB8: 48 8B F0           mov         rsi,rax
  00000001405F4CBB: B9 10 00 00 00     mov         ecx,10h
  00000001405F4CC0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4CC2: 45 33 C0           xor         r8d,r8d
  00000001405F4CC5: 33 D2              xor         edx,edx
  00000001405F4CC7: 48 8D 8C 24 E0 06  lea         rcx,[rsp+00000000000006E0h]
                    00 00
  00000001405F4CCF: FF 15 93 F4 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4CD5: 48 8D BC 24 30 0E  lea         rdi,[rsp+0000000000000E30h]
                    00 00
  00000001405F4CDD: 48 8B F0           mov         rsi,rax
  00000001405F4CE0: B9 10 00 00 00     mov         ecx,10h
  00000001405F4CE5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4CE7: 45 33 C0           xor         r8d,r8d
  00000001405F4CEA: 33 D2              xor         edx,edx
  00000001405F4CEC: 48 8D 8C 24 F0 06  lea         rcx,[rsp+00000000000006F0h]
                    00 00
  00000001405F4CF4: FF 15 6E F4 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4CFA: 48 8D BC 24 20 0E  lea         rdi,[rsp+0000000000000E20h]
                    00 00
  00000001405F4D02: 48 8B F0           mov         rsi,rax
  00000001405F4D05: B9 10 00 00 00     mov         ecx,10h
  00000001405F4D0A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4D0C: 45 33 C0           xor         r8d,r8d
  00000001405F4D0F: 33 D2              xor         edx,edx
  00000001405F4D11: 48 8D 8C 24 00 07  lea         rcx,[rsp+0000000000000700h]
                    00 00
  00000001405F4D19: FF 15 49 F4 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4D1F: 48 8D BC 24 10 0E  lea         rdi,[rsp+0000000000000E10h]
                    00 00
  00000001405F4D27: 48 8B F0           mov         rsi,rax
  00000001405F4D2A: B9 10 00 00 00     mov         ecx,10h
  00000001405F4D2F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4D31: 45 33 C0           xor         r8d,r8d
  00000001405F4D34: 33 D2              xor         edx,edx
  00000001405F4D36: 48 8D 8C 24 10 07  lea         rcx,[rsp+0000000000000710h]
                    00 00
  00000001405F4D3E: FF 15 24 F4 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4D44: 48 8D BC 24 00 0E  lea         rdi,[rsp+0000000000000E00h]
                    00 00
  00000001405F4D4C: 48 8B F0           mov         rsi,rax
  00000001405F4D4F: B9 10 00 00 00     mov         ecx,10h
  00000001405F4D54: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4D56: 45 33 C0           xor         r8d,r8d
  00000001405F4D59: 33 D2              xor         edx,edx
  00000001405F4D5B: 48 8D 8C 24 20 07  lea         rcx,[rsp+0000000000000720h]
                    00 00
  00000001405F4D63: FF 15 FF F3 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4D69: 48 8D BC 24 F0 0D  lea         rdi,[rsp+0000000000000DF0h]
                    00 00
  00000001405F4D71: 48 8B F0           mov         rsi,rax
  00000001405F4D74: B9 10 00 00 00     mov         ecx,10h
  00000001405F4D79: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4D7B: 45 33 C0           xor         r8d,r8d
  00000001405F4D7E: 33 D2              xor         edx,edx
  00000001405F4D80: 48 8D 8C 24 30 07  lea         rcx,[rsp+0000000000000730h]
                    00 00
  00000001405F4D88: FF 15 DA F3 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4D8E: 48 8D BC 24 E0 0D  lea         rdi,[rsp+0000000000000DE0h]
                    00 00
  00000001405F4D96: 48 8B F0           mov         rsi,rax
  00000001405F4D99: B9 10 00 00 00     mov         ecx,10h
  00000001405F4D9E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4DA0: 45 33 C0           xor         r8d,r8d
  00000001405F4DA3: 33 D2              xor         edx,edx
  00000001405F4DA5: 48 8D 8C 24 40 07  lea         rcx,[rsp+0000000000000740h]
                    00 00
  00000001405F4DAD: FF 15 B5 F3 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4DB3: 48 8D BC 24 D0 0D  lea         rdi,[rsp+0000000000000DD0h]
                    00 00
  00000001405F4DBB: 48 8B F0           mov         rsi,rax
  00000001405F4DBE: B9 10 00 00 00     mov         ecx,10h
  00000001405F4DC3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4DC5: 48 8D BC 24 C0 0D  lea         rdi,[rsp+0000000000000DC0h]
                    00 00
  00000001405F4DCD: 48 8D B4 24 50 07  lea         rsi,[rsp+0000000000000750h]
                    00 00
  00000001405F4DD5: B9 10 00 00 00     mov         ecx,10h
  00000001405F4DDA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4DDC: 48 8D 84 24 50 0E  lea         rax,[rsp+0000000000000E50h]
                    00 00
  00000001405F4DE4: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001405F4DE9: 48 8D 84 24 40 0E  lea         rax,[rsp+0000000000000E40h]
                    00 00
  00000001405F4DF1: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001405F4DF6: 48 8D 84 24 30 0E  lea         rax,[rsp+0000000000000E30h]
                    00 00
  00000001405F4DFE: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001405F4E03: 48 8D 84 24 20 0E  lea         rax,[rsp+0000000000000E20h]
                    00 00
  00000001405F4E0B: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001405F4E10: 48 8D 84 24 10 0E  lea         rax,[rsp+0000000000000E10h]
                    00 00
  00000001405F4E18: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001405F4E1D: 48 8D 84 24 00 0E  lea         rax,[rsp+0000000000000E00h]
                    00 00
  00000001405F4E25: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001405F4E2A: 48 8D 84 24 F0 0D  lea         rax,[rsp+0000000000000DF0h]
                    00 00
  00000001405F4E32: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405F4E37: 48 8D 84 24 E0 0D  lea         rax,[rsp+0000000000000DE0h]
                    00 00
  00000001405F4E3F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405F4E44: 4C 8D 8C 24 D0 0D  lea         r9,[rsp+0000000000000DD0h]
                    00 00
  00000001405F4E4C: 4C 8D 84 24 C0 0D  lea         r8,[rsp+0000000000000DC0h]
                    00 00
  00000001405F4E54: 48 8D 15 1D 2A EA  lea         rdx,[0000000142497878h]
                    01
  00000001405F4E5B: 48 8D 0D 9E 18 78  lea         rcx,[0000000143D76700h]
                    03
  00000001405F4E62: E8 59 A5 CD FF     call        00000001402CF3C0
  00000001405F4E67: 45 33 C0           xor         r8d,r8d
  00000001405F4E6A: 33 D2              xor         edx,edx
  00000001405F4E6C: 48 8D 8C 24 70 07  lea         rcx,[rsp+0000000000000770h]
                    00 00
  00000001405F4E74: FF 15 EE F2 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4E7A: 48 8D BC 24 F0 0E  lea         rdi,[rsp+0000000000000EF0h]
                    00 00
  00000001405F4E82: 48 8B F0           mov         rsi,rax
  00000001405F4E85: B9 10 00 00 00     mov         ecx,10h
  00000001405F4E8A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4E8C: 45 33 C0           xor         r8d,r8d
  00000001405F4E8F: 33 D2              xor         edx,edx
  00000001405F4E91: 48 8D 8C 24 80 07  lea         rcx,[rsp+0000000000000780h]
                    00 00
  00000001405F4E99: FF 15 C9 F2 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4E9F: 48 8D BC 24 E0 0E  lea         rdi,[rsp+0000000000000EE0h]
                    00 00
  00000001405F4EA7: 48 8B F0           mov         rsi,rax
  00000001405F4EAA: B9 10 00 00 00     mov         ecx,10h
  00000001405F4EAF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4EB1: 45 33 C0           xor         r8d,r8d
  00000001405F4EB4: 33 D2              xor         edx,edx
  00000001405F4EB6: 48 8D 8C 24 90 07  lea         rcx,[rsp+0000000000000790h]
                    00 00
  00000001405F4EBE: FF 15 A4 F2 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4EC4: 48 8D BC 24 D0 0E  lea         rdi,[rsp+0000000000000ED0h]
                    00 00
  00000001405F4ECC: 48 8B F0           mov         rsi,rax
  00000001405F4ECF: B9 10 00 00 00     mov         ecx,10h
  00000001405F4ED4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4ED6: 45 33 C0           xor         r8d,r8d
  00000001405F4ED9: 33 D2              xor         edx,edx
  00000001405F4EDB: 48 8D 8C 24 A0 07  lea         rcx,[rsp+00000000000007A0h]
                    00 00
  00000001405F4EE3: FF 15 7F F2 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4EE9: 48 8D BC 24 C0 0E  lea         rdi,[rsp+0000000000000EC0h]
                    00 00
  00000001405F4EF1: 48 8B F0           mov         rsi,rax
  00000001405F4EF4: B9 10 00 00 00     mov         ecx,10h
  00000001405F4EF9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4EFB: 45 33 C0           xor         r8d,r8d
  00000001405F4EFE: 33 D2              xor         edx,edx
  00000001405F4F00: 48 8D 8C 24 B0 07  lea         rcx,[rsp+00000000000007B0h]
                    00 00
  00000001405F4F08: FF 15 5A F2 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4F0E: 48 8D BC 24 B0 0E  lea         rdi,[rsp+0000000000000EB0h]
                    00 00
  00000001405F4F16: 48 8B F0           mov         rsi,rax
  00000001405F4F19: B9 10 00 00 00     mov         ecx,10h
  00000001405F4F1E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4F20: 45 33 C0           xor         r8d,r8d
  00000001405F4F23: 33 D2              xor         edx,edx
  00000001405F4F25: 48 8D 8C 24 C0 07  lea         rcx,[rsp+00000000000007C0h]
                    00 00
  00000001405F4F2D: FF 15 35 F2 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4F33: 48 8D BC 24 A0 0E  lea         rdi,[rsp+0000000000000EA0h]
                    00 00
  00000001405F4F3B: 48 8B F0           mov         rsi,rax
  00000001405F4F3E: B9 10 00 00 00     mov         ecx,10h
  00000001405F4F43: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4F45: 45 33 C0           xor         r8d,r8d
  00000001405F4F48: 33 D2              xor         edx,edx
  00000001405F4F4A: 48 8D 8C 24 D0 07  lea         rcx,[rsp+00000000000007D0h]
                    00 00
  00000001405F4F52: FF 15 10 F2 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4F58: 48 8D BC 24 90 0E  lea         rdi,[rsp+0000000000000E90h]
                    00 00
  00000001405F4F60: 48 8B F0           mov         rsi,rax
  00000001405F4F63: B9 10 00 00 00     mov         ecx,10h
  00000001405F4F68: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4F6A: 45 33 C0           xor         r8d,r8d
  00000001405F4F6D: 33 D2              xor         edx,edx
  00000001405F4F6F: 48 8D 8C 24 E0 07  lea         rcx,[rsp+00000000000007E0h]
                    00 00
  00000001405F4F77: FF 15 EB F1 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4F7D: 48 8D BC 24 80 0E  lea         rdi,[rsp+0000000000000E80h]
                    00 00
  00000001405F4F85: 48 8B F0           mov         rsi,rax
  00000001405F4F88: B9 10 00 00 00     mov         ecx,10h
  00000001405F4F8D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4F8F: 45 33 C0           xor         r8d,r8d
  00000001405F4F92: 33 D2              xor         edx,edx
  00000001405F4F94: 48 8D 8C 24 F0 07  lea         rcx,[rsp+00000000000007F0h]
                    00 00
  00000001405F4F9C: FF 15 C6 F1 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4FA2: 48 8D BC 24 70 0E  lea         rdi,[rsp+0000000000000E70h]
                    00 00
  00000001405F4FAA: 48 8B F0           mov         rsi,rax
  00000001405F4FAD: B9 10 00 00 00     mov         ecx,10h
  00000001405F4FB2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4FB4: 45 33 C0           xor         r8d,r8d
  00000001405F4FB7: 33 D2              xor         edx,edx
  00000001405F4FB9: 48 8D 8C 24 00 08  lea         rcx,[rsp+0000000000000800h]
                    00 00
  00000001405F4FC1: FF 15 A1 F1 80 01  call        qword ptr [0000000141E04168h]
  00000001405F4FC7: 48 8D BC 24 60 0E  lea         rdi,[rsp+0000000000000E60h]
                    00 00
  00000001405F4FCF: 48 8B F0           mov         rsi,rax
  00000001405F4FD2: B9 10 00 00 00     mov         ecx,10h
  00000001405F4FD7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001405F4FD9: 48 8D 84 24 F0 0E  lea         rax,[rsp+0000000000000EF0h]
                    00 00
  00000001405F4FE1: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001405F4FE6: 48 8D 84 24 E0 0E  lea         rax,[rsp+0000000000000EE0h]
                    00 00
  00000001405F4FEE: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001405F4FF3: 48 8D 84 24 D0 0E  lea         rax,[rsp+0000000000000ED0h]
                    00 00
  00000001405F4FFB: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001405F5000: 48 8D 84 24 C0 0E  lea         rax,[rsp+0000000000000EC0h]
                    00 00
  00000001405F5008: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001405F500D: 48 8D 84 24 B0 0E  lea         rax,[rsp+0000000000000EB0h]
                    00 00
  00000001405F5015: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001405F501A: 48 8D 84 24 A0 0E  lea         rax,[rsp+0000000000000EA0h]
                    00 00
  00000001405F5022: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001405F5027: 48 8D 84 24 90 0E  lea         rax,[rsp+0000000000000E90h]
                    00 00
  00000001405F502F: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405F5034: 48 8D 84 24 80 0E  lea         rax,[rsp+0000000000000E80h]
                    00 00
  00000001405F503C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405F5041: 4C 8D 8C 24 70 0E  lea         r9,[rsp+0000000000000E70h]
                    00 00
  00000001405F5049: 4C 8D 84 24 60 0E  lea         r8,[rsp+0000000000000E60h]
                    00 00
  00000001405F5051: 48 8D 15 38 28 EA  lea         rdx,[0000000142497890h]
                    01
  00000001405F5058: 48 8D 0D A1 16 78  lea         rcx,[0000000143D76700h]
                    03
  00000001405F505F: E8 5C A3 CD FF     call        00000001402CF3C0
  00000001405F5064: 48 81 C4 98 0F 00  add         rsp,0F98h
                    00
  00000001405F506B: 5F                 pop         rdi
  00000001405F506C: 5E                 pop         rsi
  00000001405F506D: C3                 ret

  Summary

     1E02000 .text
