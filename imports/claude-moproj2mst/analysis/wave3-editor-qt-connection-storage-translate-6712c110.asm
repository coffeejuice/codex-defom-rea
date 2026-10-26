Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712C110: 48 85 C9           test        rcx,rcx
  000000006712C113: 0F 84 EC 01 00 00  je          000000006712C305
  000000006712C119: 4C 8B DC           mov         r11,rsp
  000000006712C11C: 4D 89 4B 20        mov         qword ptr [r11+20h],r9
  000000006712C120: 4D 89 43 18        mov         qword ptr [r11+18h],r8
  000000006712C124: 55                 push        rbp
  000000006712C125: 56                 push        rsi
  000000006712C126: 57                 push        rdi
  000000006712C127: 48 83 EC 70        sub         rsp,70h
  000000006712C12B: 48 C7 44 24 68 FE  mov         qword ptr [rsp+68h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006712C134: 49 89 5B 10        mov         qword ptr [r11+10h],rbx
  000000006712C138: 4D 8B D1           mov         r10,r9
  000000006712C13B: 49 8B D8           mov         rbx,r8
  000000006712C13E: 48 8B EA           mov         rbp,rdx
  000000006712C141: 33 C0              xor         eax,eax
  000000006712C143: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006712C148: 41 89 43 08        mov         dword ptr [r11+8],eax
  000000006712C14C: 48 8B B4 24 B0 00  mov         rsi,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000006712C154: 48 85 F6           test        rsi,rsi
  000000006712C157: 0F 84 99 01 00 00  je          000000006712C2F6
  000000006712C15D: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006712C161: 48 89 4C 24 60     mov         qword ptr [rsp+60h],rcx
  000000006712C166: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  000000006712C16A: 8B 51 08           mov         edx,dword ptr [rcx+8]
  000000006712C16D: 4C 8B C8           mov         r9,rax
  000000006712C170: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000006712C175: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  000000006712C178: 2B C2              sub         eax,edx
  000000006712C17A: 85 C0              test        eax,eax
  000000006712C17C: 0F 8E 74 01 00 00  jle         000000006712C2F6
  000000006712C182: 48 63 C2           movsxd      rax,edx
  000000006712C185: 49 03 C1           add         rax,r9
  000000006712C188: 48 8B 7C C1 18     mov         rdi,qword ptr [rcx+rax*8+18h]
  000000006712C18D: 48 8B D7           mov         rdx,rdi
  000000006712C190: 49 8B CA           mov         rcx,r10
  000000006712C193: E8 38 1C FF FF     call        000000006711DDD0
  000000006712C198: 48 85 C0           test        rax,rax
  000000006712C19B: 0F 84 FE 00 00 00  je          000000006712C29F
  000000006712C1A1: 4C 8B 94 24 A0 00  mov         r10,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006712C1A9: 4D 85 D2           test        r10,r10
  000000006712C1AC: 74 6A              je          000000006712C218
  000000006712C1AE: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006712C1B3: 48 8B CF           mov         rcx,rdi
  000000006712C1B6: E8 35 EC FF FF     call        000000006712ADF0
  000000006712C1BB: 90                 nop
  000000006712C1BC: 45 33 C9           xor         r9d,r9d
  000000006712C1BF: 45 33 C0           xor         r8d,r8d
  000000006712C1C2: 48 8B D0           mov         rdx,rax
  000000006712C1C5: 49 8B CA           mov         rcx,r10
  000000006712C1C8: E8 D3 C4 F3 FF     call        00000000670686A0
  000000006712C1CD: 83 F8 FF           cmp         eax,0FFFFFFFFh
  000000006712C1D0: 0F 95 C3           setne       bl
  000000006712C1D3: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000006712C1D8: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006712C1DC: 75 0A              jne         000000006712C1E8
  000000006712C1DE: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000006712C1E3: E8 E8 3E F4 FF     call        00000000670700D0
  000000006712C1E8: 84 DB              test        bl,bl
  000000006712C1EA: 74 1F              je          000000006712C20B
  000000006712C1EC: 48 89 7C 24 50     mov         qword ptr [rsp+50h],rdi
  000000006712C1F1: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006712C1F6: 48 8B CE           mov         rcx,rsi
  000000006712C1F9: E8 E2 FD FF FF     call        000000006712BFE0
  000000006712C1FE: 48 8B 9C 24 A0 00  mov         rbx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006712C206: E9 94 00 00 00     jmp         000000006712C29F
  000000006712C20B: 48 8B 9C 24 A0 00  mov         rbx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006712C213: E9 87 00 00 00     jmp         000000006712C29F
  000000006712C218: 48 8D 05 11 49 1C  lea         rax,[00000000672F0B30h]
                    00
  000000006712C21F: 48 39 45 00        cmp         qword ptr [rbp],rax
  000000006712C223: 74 29              je          000000006712C24E
  000000006712C225: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006712C22A: 48 8B CF           mov         rcx,rdi
  000000006712C22D: E8 BE EB FF FF     call        000000006712ADF0
  000000006712C232: 90                 nop
  000000006712C233: 83 8C 24 90 00 00  or          dword ptr [rsp+0000000000000090h],1
                    00 01
  000000006712C23B: 48 8B D5           mov         rdx,rbp
  000000006712C23E: 48 8B C8           mov         rcx,rax
  000000006712C241: E8 8A F5 F3 FF     call        000000006706B7D0
  000000006712C246: 84 C0              test        al,al
  000000006712C248: 75 04              jne         000000006712C24E
  000000006712C24A: 32 DB              xor         bl,bl
  000000006712C24C: EB 02              jmp         000000006712C250
  000000006712C24E: B3 01              mov         bl,1
  000000006712C250: F6 84 24 90 00 00  test        byte ptr [rsp+0000000000000090h],1
                    00 01
  000000006712C258: 74 1D              je          000000006712C277
  000000006712C25A: 83 A4 24 90 00 00  and         dword ptr [rsp+0000000000000090h],0FFFFFFFEh
                    00 FE
  000000006712C262: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000006712C267: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006712C26B: 75 0A              jne         000000006712C277
  000000006712C26D: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000006712C272: E8 59 3E F4 FF     call        00000000670700D0
  000000006712C277: 84 DB              test        bl,bl
  000000006712C279: 74 1C              je          000000006712C297
  000000006712C27B: 48 89 7C 24 58     mov         qword ptr [rsp+58h],rdi
  000000006712C280: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006712C285: 48 8B CE           mov         rcx,rsi
  000000006712C288: E8 53 FD FF FF     call        000000006712BFE0
  000000006712C28D: 48 8B 9C 24 A0 00  mov         rbx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006712C295: EB 08              jmp         000000006712C29F
  000000006712C297: 48 8B 9C 24 A0 00  mov         rbx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006712C29F: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  000000006712C2A4: 4C 8B 8C 24 A8 00  mov         r9,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006712C2AC: 4C 8B C3           mov         r8,rbx
  000000006712C2AF: 48 8B D5           mov         rdx,rbp
  000000006712C2B2: 48 8B CF           mov         rcx,rdi
  000000006712C2B5: E8 56 FE FF FF     call        000000006712C110
  000000006712C2BA: 4C 8B 44 24 40     mov         r8,qword ptr [rsp+40h]
  000000006712C2BF: 41 FF C0           inc         r8d
  000000006712C2C2: 4C 89 44 24 40     mov         qword ptr [rsp+40h],r8
  000000006712C2C7: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  000000006712C2CC: 49 FF C1           inc         r9
  000000006712C2CF: 4C 89 4C 24 48     mov         qword ptr [rsp+48h],r9
  000000006712C2D4: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000006712C2D9: 48 8B 48 18        mov         rcx,qword ptr [rax+18h]
  000000006712C2DD: 8B 51 08           mov         edx,dword ptr [rcx+8]
  000000006712C2E0: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  000000006712C2E3: 2B C2              sub         eax,edx
  000000006712C2E5: 44 3B C0           cmp         r8d,eax
  000000006712C2E8: 4C 8B 94 24 A8 00  mov         r10,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006712C2F0: 0F 8C 8C FE FF FF  jl          000000006712C182
  000000006712C2F6: 48 8B 9C 24 98 00  mov         rbx,qword ptr [rsp+0000000000000098h]
                    00 00
  000000006712C2FE: 48 83 C4 70        add         rsp,70h
  000000006712C302: 5F                 pop         rdi
  000000006712C303: 5E                 pop         rsi
  000000006712C304: 5D                 pop         rbp
  000000006712C305: F3 C3              rep ret

  Summary

      1A7000 .text
