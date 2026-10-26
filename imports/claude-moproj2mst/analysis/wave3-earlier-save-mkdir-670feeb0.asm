Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670FEEB0: 40 53              push        rbx
  00000000670FEEB2: 55                 push        rbp
  00000000670FEEB3: 56                 push        rsi
  00000000670FEEB4: 57                 push        rdi
  00000000670FEEB5: 41 54              push        r12
  00000000670FEEB7: 48 83 EC 70        sub         rsp,70h
  00000000670FEEBB: 48 C7 44 24 68 FE  mov         qword ptr [rsp+68h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670FEEC4: 0F B6 DA           movzx       ebx,dl
  00000000670FEEC7: 48 8B F9           mov         rdi,rcx
  00000000670FEECA: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670FEECF: E8 6C 01 FF FF     call        00000000670EF040
  00000000670FEED4: 90                 nop
  00000000670FEED5: 84 DB              test        bl,bl
  00000000670FEED7: 0F 84 52 02 00 00  je          00000000670FF12F
  00000000670FEEDD: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670FEEE2: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000670FEEE7: E8 34 D7 FA FF     call        00000000670AC620
  00000000670FEEEC: 90                 nop
  00000000670FEEED: 48 8B D0           mov         rdx,rax
  00000000670FEEF0: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000000670FEEF5: E8 06 E7 FA FF     call        00000000670AD600
  00000000670FEEFA: 90                 nop
  00000000670FEEFB: 48 8B D0           mov         rdx,rax
  00000000670FEEFE: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000670FEF03: E8 C8 14 F7 FF     call        00000000670703D0
  00000000670FEF08: 90                 nop
  00000000670FEF09: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000000670FEF0E: 83 CF FF           or          edi,0FFFFFFFFh
  00000000670FEF11: F0 01 39           lock add    dword ptr [rcx],edi
  00000000670FEF14: 75 0B              jne         00000000670FEF21
  00000000670FEF16: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000000670FEF1B: E8 B0 11 F7 FF     call        00000000670700D0
  00000000670FEF20: 90                 nop
  00000000670FEF21: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000000670FEF26: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FEF2A: 75 0A              jne         00000000670FEF36
  00000000670FEF2C: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000000670FEF31: E8 9A 11 F7 FF     call        00000000670700D0
  00000000670FEF36: 8B EF              mov         ebp,edi
  00000000670FEF38: 48 8D 05 4D 35 0F  lea         rax,[00000000671F248Ch]
                    00
  00000000670FEF3F: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000000670FEF44: 41 B8 01 00 00 00  mov         r8d,1
  00000000670FEF4A: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000670FEF4F: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000670FEF54: E8 A7 F1 F6 FF     call        000000006706E100
  00000000670FEF59: 84 C0              test        al,al
  00000000670FEF5B: 74 73              je          00000000670FEFD0
  00000000670FEF5D: 33 F6              xor         esi,esi
  00000000670FEF5F: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670FEF64: 39 70 08           cmp         dword ptr [rax+8],esi
  00000000670FEF67: 7E 67              jle         00000000670FEFD0
  00000000670FEF69: 4C 8B E6           mov         r12,rsi
  00000000670FEF6C: 0F 1F 40 00        nop         dword ptr [rax]
  00000000670FEF70: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  00000000670FEF74: 41 0F B7 1C 0C     movzx       ebx,word ptr [r12+rcx]
  00000000670FEF79: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000000670FEF81: E8 EA C9 FA FF     call        00000000670AB970
  00000000670FEF86: 66 3B 18           cmp         bx,word ptr [rax]
  00000000670FEF89: 75 12              jne         00000000670FEF9D
  00000000670FEF8B: FF C6              inc         esi
  00000000670FEF8D: 49 83 C4 02        add         r12,2
  00000000670FEF91: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670FEF96: 3B 70 08           cmp         esi,dword ptr [rax+8]
  00000000670FEF99: 7C D5              jl          00000000670FEF70
  00000000670FEF9B: EB 33              jmp         00000000670FEFD0
  00000000670FEF9D: 8B EE              mov         ebp,esi
  00000000670FEF9F: 83 FE FF           cmp         esi,0FFFFFFFFh
  00000000670FEFA2: 74 2C              je          00000000670FEFD0
  00000000670FEFA4: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  00000000670FEFAC: E8 BF C9 FA FF     call        00000000670AB970
  00000000670FEFB1: 41 B9 01 00 00 00  mov         r9d,1
  00000000670FEFB7: 44 8B C6           mov         r8d,esi
  00000000670FEFBA: 0F B7 10           movzx       edx,word ptr [rax]
  00000000670FEFBD: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000670FEFC2: E8 A9 EE F6 FF     call        000000006706DE70
  00000000670FEFC7: 8B E8              mov         ebp,eax
  00000000670FEFC9: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  00000000670FEFD0: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000670FEFD5: E8 96 C9 FA FF     call        00000000670AB970
  00000000670FEFDA: 44 8D 45 01        lea         r8d,[rbp+1]
  00000000670FEFDE: 41 B9 01 00 00 00  mov         r9d,1
  00000000670FEFE4: 0F B7 10           movzx       edx,word ptr [rax]
  00000000670FEFE7: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000670FEFEC: E8 7F EE F6 FF     call        000000006706DE70
  00000000670FEFF1: 8B D8              mov         ebx,eax
  00000000670FEFF3: 83 F8 FF           cmp         eax,0FFFFFFFFh
  00000000670FEFF6: 75 10              jne         00000000670FF008
  00000000670FEFF8: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670FEFFD: 8B 58 08           mov         ebx,dword ptr [rax+8]
  00000000670FF000: 3B EB              cmp         ebp,ebx
  00000000670FF002: 0F 84 0E 01 00 00  je          00000000670FF116
  00000000670FF008: 85 DB              test        ebx,ebx
  00000000670FF00A: 0F 84 96 00 00 00  je          00000000670FF0A6
  00000000670FF010: 44 8B C3           mov         r8d,ebx
  00000000670FF013: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000670FF018: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000670FF01D: E8 CE 4E F7 FF     call        0000000067073EF0
  00000000670FF022: 90                 nop
  00000000670FF023: C6 84 24 A8 00 00  mov         byte ptr [rsp+00000000000000A8h],0
                    00 00
  00000000670FF02B: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  00000000670FF033: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670FF038: E8 E3 FC FF FF     call        00000000670FED20
  00000000670FF03D: 84 C0              test        al,al
  00000000670FF03F: 75 50              jne         00000000670FF091
  00000000670FF041: 38 84 24 A8 00 00  cmp         byte ptr [rsp+00000000000000A8h],al
                    00
  00000000670FF048: 0F 85 96 00 00 00  jne         00000000670FF0E4
  00000000670FF04E: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000670FF053: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000000670FF058: E8 63 8F FF FF     call        00000000670F7FC0
  00000000670FF05D: 90                 nop
  00000000670FF05E: 48 8B C8           mov         rcx,rax
  00000000670FF061: E8 EA 19 F7 FF     call        0000000067070A50
  00000000670FF066: 48 8B C8           mov         rcx,rax
  00000000670FF069: 33 D2              xor         edx,edx
  00000000670FF06B: FF 15 BF 91 0A 00  call        qword ptr [00000000671A8230h]
  00000000670FF071: 85 C0              test        eax,eax
  00000000670FF073: 40 0F 95 C6        setne       sil
  00000000670FF077: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000000670FF07C: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FF080: 75 0A              jne         00000000670FF08C
  00000000670FF082: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000000670FF087: E8 44 10 F7 FF     call        00000000670700D0
  00000000670FF08C: 40 84 F6           test        sil,sil
  00000000670FF08F: 74 21              je          00000000670FF0B2
  00000000670FF091: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670FF096: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FF09A: 75 0A              jne         00000000670FF0A6
  00000000670FF09C: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670FF0A1: E8 2A 10 F7 FF     call        00000000670700D0
  00000000670FF0A6: 8B EB              mov         ebp,ebx
  00000000670FF0A8: 83 FB FF           cmp         ebx,0FFFFFFFFh
  00000000670FF0AB: 74 69              je          00000000670FF116
  00000000670FF0AD: E9 1E FF FF FF     jmp         00000000670FEFD0
  00000000670FF0B2: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670FF0B7: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FF0BB: 75 0B              jne         00000000670FF0C8
  00000000670FF0BD: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670FF0C2: E8 09 10 F7 FF     call        00000000670700D0
  00000000670FF0C7: 90                 nop
  00000000670FF0C8: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670FF0CD: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FF0D1: 75 0A              jne         00000000670FF0DD
  00000000670FF0D3: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670FF0D8: E8 F3 0F F7 FF     call        00000000670700D0
  00000000670FF0DD: 32 C0              xor         al,al
  00000000670FF0DF: E9 C2 00 00 00     jmp         00000000670FF1A6
  00000000670FF0E4: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670FF0E9: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FF0ED: 75 0B              jne         00000000670FF0FA
  00000000670FF0EF: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670FF0F4: E8 D7 0F F7 FF     call        00000000670700D0
  00000000670FF0F9: 90                 nop
  00000000670FF0FA: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670FF0FF: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FF103: 75 0A              jne         00000000670FF10F
  00000000670FF105: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670FF10A: E8 C1 0F F7 FF     call        00000000670700D0
  00000000670FF10F: 32 C0              xor         al,al
  00000000670FF111: E9 90 00 00 00     jmp         00000000670FF1A6
  00000000670FF116: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670FF11B: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FF11F: 75 0A              jne         00000000670FF12B
  00000000670FF121: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670FF126: E8 A5 0F F7 FF     call        00000000670700D0
  00000000670FF12B: B0 01              mov         al,1
  00000000670FF12D: EB 77              jmp         00000000670FF1A6
  00000000670FF12F: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000000670FF134: 48 8B CF           mov         rcx,rdi
  00000000670FF137: E8 04 FF FE FF     call        00000000670EF040
  00000000670FF13C: 90                 nop
  00000000670FF13D: 48 8B D0           mov         rdx,rax
  00000000670FF140: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670FF145: E8 76 8E FF FF     call        00000000670F7FC0
  00000000670FF14A: 90                 nop
  00000000670FF14B: 48 8B C8           mov         rcx,rax
  00000000670FF14E: E8 FD 18 F7 FF     call        0000000067070A50
  00000000670FF153: 48 8B C8           mov         rcx,rax
  00000000670FF156: 33 D2              xor         edx,edx
  00000000670FF158: FF 15 D2 90 0A 00  call        qword ptr [00000000671A8230h]
  00000000670FF15E: 85 C0              test        eax,eax
  00000000670FF160: 0F 95 C3           setne       bl
  00000000670FF163: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670FF168: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670FF16C: 75 0B              jne         00000000670FF179
  00000000670FF16E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670FF173: E8 58 0F F7 FF     call        00000000670700D0
  00000000670FF178: 90                 nop
  00000000670FF179: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000000670FF17E: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FF182: 75 0B              jne         00000000670FF18F
  00000000670FF184: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000000670FF189: E8 42 0F F7 FF     call        00000000670700D0
  00000000670FF18E: 90                 nop
  00000000670FF18F: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670FF194: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670FF198: 75 0A              jne         00000000670FF1A4
  00000000670FF19A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670FF19F: E8 2C 0F F7 FF     call        00000000670700D0
  00000000670FF1A4: 8A C3              mov         al,bl
  00000000670FF1A6: 48 83 C4 70        add         rsp,70h
  00000000670FF1AA: 41 5C              pop         r12
  00000000670FF1AC: 5F                 pop         rdi
  00000000670FF1AD: 5E                 pop         rsi
  00000000670FF1AE: 5D                 pop         rbp
  00000000670FF1AF: 5B                 pop         rbx
  00000000670FF1B0: C3                 ret

  Summary

      1A7000 .text
