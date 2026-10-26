Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067072220: 40 55              push        rbp
  0000000067072222: 56                 push        rsi
  0000000067072223: 57                 push        rdi
  0000000067072224: 41 54              push        r12
  0000000067072226: 41 55              push        r13
  0000000067072228: 48 83 EC 30        sub         rsp,30h
  000000006707222C: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000067072235: 48 89 5C 24 68     mov         qword ptr [rsp+68h],rbx
  000000006707223A: 41 8B E8           mov         ebp,r8d
  000000006707223D: 48 8B F2           mov         rsi,rdx
  0000000067072240: 48 8B F9           mov         rdi,rcx
  0000000067072243: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000067072246: 44 8B 08           mov         r9d,dword ptr [rax]
  0000000067072249: 41 83 F9 01        cmp         r9d,1
  000000006707224D: 0F 85 33 01 00 00  jne         0000000067072386
  0000000067072253: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000067072256: 48 8D 41 1A        lea         rax,[rcx+1Ah]
  000000006707225A: 48 39 41 10        cmp         qword ptr [rcx+10h],rax
  000000006707225E: 75 0A              jne         000000006707226A
  0000000067072260: 83 79 04 00        cmp         dword ptr [rcx+4],0
  0000000067072264: 0F 85 1C 01 00 00  jne         0000000067072386
  000000006707226A: 45 33 E4           xor         r12d,r12d
  000000006707226D: F6 41 18 08        test        byte ptr [rcx+18h],8
  0000000067072271: 0F 84 9E 00 00 00  je          0000000067072315
  0000000067072277: E8 B4 D9 FF FF     call        000000006706FC30
  000000006707227C: 48 8B D8           mov         rbx,rax
  000000006707227F: 45 8D 6C 24 01     lea         r13d,[r12+1]
  0000000067072284: 48 85 C0           test        rax,rax
  0000000067072287: 74 33              je          00000000670722BC
  0000000067072289: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006707228C: 44 84 69 04        test        byte ptr [rcx+4],r13b
  0000000067072290: 74 0A              je          000000006707229C
  0000000067072292: 48 8B C8           mov         rcx,rax
  0000000067072295: E8 16 3B FB FF     call        0000000067025DB0
  000000006707229A: EB 16              jmp         00000000670722B2
  000000006707229C: 33 C0              xor         eax,eax
  000000006707229E: F0 44 0F B1 29     lock cmpxchg dword ptr [rcx],r13d
  00000000670722A3: 0F 94 C1           sete        cl
  00000000670722A6: 84 C9              test        cl,cl
  00000000670722A8: 75 08              jne         00000000670722B2
  00000000670722AA: 48 8B CB           mov         rcx,rbx
  00000000670722AD: E8 FE 38 FB FF     call        0000000067025BB0
  00000000670722B2: 49 0B DD           or          rbx,r13
  00000000670722B5: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  00000000670722BA: EB 08              jmp         00000000670722C4
  00000000670722BC: 49 8B DC           mov         rbx,r12
  00000000670722BF: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  00000000670722C4: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670722C7: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000000670722CC: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670722D1: 48 8B 0D 40 1B 28  mov         rcx,qword ptr [00000000672F3E18h]
                    00
  00000000670722D8: E8 83 DB FF FF     call        000000006706FE60
  00000000670722DD: 90                 nop
  00000000670722DE: F6 C3 01           test        bl,1
  00000000670722E1: 74 32              je          0000000067072315
  00000000670722E3: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  00000000670722E7: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  00000000670722EC: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  00000000670722F0: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670722F3: F6 41 04 01        test        byte ptr [rcx+4],1
  00000000670722F7: 74 0A              je          0000000067072303
  00000000670722F9: 48 8B CB           mov         rcx,rbx
  00000000670722FC: E8 2F 38 FB FF     call        0000000067025B30
  0000000067072301: EB 12              jmp         0000000067072315
  0000000067072303: 41 8B C5           mov         eax,r13d
  0000000067072306: F0 44 0F B1 21     lock cmpxchg dword ptr [rcx],r12d
  000000006707230B: 74 08              je          0000000067072315
  000000006707230D: 48 8B CB           mov         rcx,rbx
  0000000067072310: E8 5B 38 FB FF     call        0000000067025B70
  0000000067072315: 48 85 F6           test        rsi,rsi
  0000000067072318: 74 09              je          0000000067072323
  000000006707231A: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006707231D: 48 89 70 10        mov         qword ptr [rax+10h],rsi
  0000000067072321: EB 0E              jmp         0000000067072331
  0000000067072323: 48 8B 0F           mov         rcx,qword ptr [rdi]
  0000000067072326: 48 8D 41 1A        lea         rax,[rcx+1Ah]
  000000006707232A: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  000000006707232E: 41 8B EC           mov         ebp,r12d
  0000000067072331: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000067072334: 89 68 08           mov         dword ptr [rax+8],ebp
  0000000067072337: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006707233A: 89 68 04           mov         dword ptr [rax+4],ebp
  000000006707233D: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000067072340: 66 44 89 60 1A     mov         word ptr [rax+1Ah],r12w
  0000000067072345: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000067072348: B9 EF FF 00 00     mov         ecx,0FFEFh
  000000006707234D: 66 21 48 18        and         word ptr [rax+18h],cx
  0000000067072351: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000067072354: B9 FB FF 00 00     mov         ecx,0FFFBh
  0000000067072359: 66 21 48 18        and         word ptr [rax+18h],cx
  000000006707235D: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000067072360: B9 FD FF 00 00     mov         ecx,0FFFDh
  0000000067072365: 66 21 48 18        and         word ptr [rax+18h],cx
  0000000067072369: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006707236C: B9 F7 FF 00 00     mov         ecx,0FFF7h
  0000000067072371: 66 21 48 18        and         word ptr [rax+18h],cx
  0000000067072375: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000067072378: B9 FE FF 00 00     mov         ecx,0FFFEh
  000000006707237D: 66 21 48 18        and         word ptr [rax+18h],cx
  0000000067072381: 48 8B C7           mov         rax,rdi
  0000000067072384: EB 48              jmp         00000000670723CE
  0000000067072386: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000006707238B: E8 00 FE FF FF     call        0000000067072190
  0000000067072390: 48 8B F0           mov         rsi,rax
  0000000067072393: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000067072396: F0 83 01 01        lock add    dword ptr [rcx],1
  000000006707239A: 48 8B 17           mov         rdx,qword ptr [rdi]
  000000006707239D: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  00000000670723A1: 75 08              jne         00000000670723AB
  00000000670723A3: 48 8B 0F           mov         rcx,qword ptr [rdi]
  00000000670723A6: E8 25 DD FF FF     call        00000000670700D0
  00000000670723AB: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000670723AE: 48 89 07           mov         qword ptr [rdi],rax
  00000000670723B1: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000000670723B6: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670723BA: 75 0F              jne         00000000670723CB
  00000000670723BC: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000000670723C1: E8 0A DD FF FF     call        00000000670700D0
  00000000670723C6: 48 8B C7           mov         rax,rdi
  00000000670723C9: EB 03              jmp         00000000670723CE
  00000000670723CB: 48 8B C7           mov         rax,rdi
  00000000670723CE: 48 8B 5C 24 68     mov         rbx,qword ptr [rsp+68h]
  00000000670723D3: 48 83 C4 30        add         rsp,30h
  00000000670723D7: 41 5D              pop         r13
  00000000670723D9: 41 5C              pop         r12
  00000000670723DB: 5F                 pop         rdi
  00000000670723DC: 5E                 pop         rsi
  00000000670723DD: 5D                 pop         rbp
  00000000670723DE: C3                 ret

  Summary

      1A7000 .text
