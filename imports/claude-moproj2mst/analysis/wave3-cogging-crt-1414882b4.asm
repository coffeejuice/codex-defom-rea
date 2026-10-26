Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001414882B4: 48 89 74 24 08     mov         qword ptr [rsp+8],rsi
  00000001414882B9: 57                 push        rdi
  00000001414882BA: 48 83 EC 50        sub         rsp,50h
  00000001414882BE: 45 8B D0           mov         r10d,r8d
  00000001414882C1: 4C 8B C1           mov         r8,rcx
  00000001414882C4: 83 64 24 40 00     and         dword ptr [rsp+40h],0
  00000001414882C9: 33 C0              xor         eax,eax
  00000001414882CB: 48 8B B4 24 80 00  mov         rsi,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001414882D3: 48 85 F6           test        rsi,rsi
  00000001414882D6: 0F 95 C0           setne       al
  00000001414882D9: 85 C0              test        eax,eax
  00000001414882DB: 75 28              jne         0000000141488305
  00000001414882DD: E8 52 BE FE FF     call        0000000141474134
  00000001414882E2: BF 16 00 00 00     mov         edi,16h
  00000001414882E7: 89 38              mov         dword ptr [rax],edi
  00000001414882E9: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  00000001414882EF: 45 33 C9           xor         r9d,r9d
  00000001414882F2: 45 33 C0           xor         r8d,r8d
  00000001414882F5: 33 D2              xor         edx,edx
  00000001414882F7: 33 C9              xor         ecx,ecx
  00000001414882F9: E8 6A 82 FE FF     call        0000000141470568
  00000001414882FE: 8B C7              mov         eax,edi
  0000000141488300: E9 DA 00 00 00     jmp         00000001414883DF
  0000000141488305: 83 0E FF           or          dword ptr [rsi],0FFFFFFFFh
  0000000141488308: 33 C0              xor         eax,eax
  000000014148830A: 48 85 C9           test        rcx,rcx
  000000014148830D: 0F 95 C0           setne       al
  0000000141488310: 85 C0              test        eax,eax
  0000000141488312: 75 28              jne         000000014148833C
  0000000141488314: E8 1B BE FE FF     call        0000000141474134
  0000000141488319: BF 16 00 00 00     mov         edi,16h
  000000014148831E: 89 38              mov         dword ptr [rax],edi
  0000000141488320: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  0000000141488326: 45 33 C9           xor         r9d,r9d
  0000000141488329: 45 33 C0           xor         r8d,r8d
  000000014148832C: 33 D2              xor         edx,edx
  000000014148832E: 33 C9              xor         ecx,ecx
  0000000141488330: E8 33 82 FE FF     call        0000000141470568
  0000000141488335: 8B C7              mov         eax,edi
  0000000141488337: E9 A3 00 00 00     jmp         00000001414883DF
  000000014148833C: 8B 8C 24 88 00 00  mov         ecx,dword ptr [rsp+0000000000000088h]
                    00
  0000000141488343: 85 C9              test        ecx,ecx
  0000000141488345: 74 38              je          000000014148837F
  0000000141488347: 41 F7 C1 7F FE FF  test        r9d,0FFFFFE7Fh
                    FF
  000000014148834E: B8 00 00 00 00     mov         eax,0
  0000000141488353: 0F 94 C0           sete        al
  0000000141488356: 85 C0              test        eax,eax
  0000000141488358: 75 25              jne         000000014148837F
  000000014148835A: E8 D5 BD FE FF     call        0000000141474134
  000000014148835F: BF 16 00 00 00     mov         edi,16h
  0000000141488364: 89 38              mov         dword ptr [rax],edi
  0000000141488366: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  000000014148836C: 45 33 C9           xor         r9d,r9d
  000000014148836F: 45 33 C0           xor         r8d,r8d
  0000000141488372: 33 D2              xor         edx,edx
  0000000141488374: 33 C9              xor         ecx,ecx
  0000000141488376: E8 ED 81 FE FF     call        0000000141470568
  000000014148837B: 8B C7              mov         eax,edi
  000000014148837D: EB 60              jmp         00000001414883DF
  000000014148837F: 89 4C 24 30        mov         dword ptr [rsp+30h],ecx
  0000000141488383: 44 89 4C 24 28     mov         dword ptr [rsp+28h],r9d
  0000000141488388: 44 89 54 24 20     mov         dword ptr [rsp+20h],r10d
  000000014148838D: 44 8B CA           mov         r9d,edx
  0000000141488390: 48 8B D6           mov         rdx,rsi
  0000000141488393: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000141488398: E8 27 F6 FF FF     call        00000001414879C4
  000000014148839D: 8B F8              mov         edi,eax
  000000014148839F: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  00000001414883A3: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  00000001414883A8: 74 2C              je          00000001414883D6
  00000001414883AA: 85 C0              test        eax,eax
  00000001414883AC: 74 21              je          00000001414883CF
  00000001414883AE: 48 63 0E           movsxd      rcx,dword ptr [rsi]
  00000001414883B1: 48 8B C1           mov         rax,rcx
  00000001414883B4: 48 C1 F8 05        sar         rax,5
  00000001414883B8: 4C 8D 05 41 BD BC  lea         r8,[0000000145054100h]
                    03
  00000001414883BF: 83 E1 1F           and         ecx,1Fh
  00000001414883C2: 48 6B C9 58        imul        rcx,rcx,58h
  00000001414883C6: 49 8B 04 C0        mov         rax,qword ptr [r8+rax*8]
  00000001414883CA: 80 64 08 08 FE     and         byte ptr [rax+rcx+8],0FEh
  00000001414883CF: 8B 0E              mov         ecx,dword ptr [rsi]
  00000001414883D1: E8 02 45 00 00     call        000000014148C8D8
  00000001414883D6: 85 FF              test        edi,edi
  00000001414883D8: 74 03              je          00000001414883DD
  00000001414883DA: 83 0E FF           or          dword ptr [rsi],0FFFFFFFFh
  00000001414883DD: 8B C7              mov         eax,edi
  00000001414883DF: 48 8B 74 24 60     mov         rsi,qword ptr [rsp+60h]
  00000001414883E4: 48 83 C4 50        add         rsp,50h
  00000001414883E8: 5F                 pop         rdi
  00000001414883E9: C3                 ret

  Summary

     1E02000 .text
