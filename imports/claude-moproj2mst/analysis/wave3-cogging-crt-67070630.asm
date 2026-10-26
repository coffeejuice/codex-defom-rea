Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067070630: 40 53              push        rbx
  0000000067070632: 55                 push        rbp
  0000000067070633: 56                 push        rsi
  0000000067070634: 57                 push        rdi
  0000000067070635: 41 54              push        r12
  0000000067070637: 48 83 EC 40        sub         rsp,40h
  000000006707063B: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000067070644: 48 8B F9           mov         rdi,rcx
  0000000067070647: E8 E4 F5 FF FF     call        000000006706FC30
  000000006707064C: 48 8B D8           mov         rbx,rax
  000000006707064F: 33 ED              xor         ebp,ebp
  0000000067070651: 44 8D 65 01        lea         r12d,[rbp+1]
  0000000067070655: 48 85 C0           test        rax,rax
  0000000067070658: 74 33              je          000000006707068D
  000000006707065A: 48 8B 10           mov         rdx,qword ptr [rax]
  000000006707065D: 44 84 62 04        test        byte ptr [rdx+4],r12b
  0000000067070661: 74 0A              je          000000006707066D
  0000000067070663: 48 8B C8           mov         rcx,rax
  0000000067070666: E8 45 57 FB FF     call        0000000067025DB0
  000000006707066B: EB 16              jmp         0000000067070683
  000000006707066D: 33 C0              xor         eax,eax
  000000006707066F: F0 44 0F B1 22     lock cmpxchg dword ptr [rdx],r12d
  0000000067070674: 0F 94 C1           sete        cl
  0000000067070677: 84 C9              test        cl,cl
  0000000067070679: 75 08              jne         0000000067070683
  000000006707067B: 48 8B CB           mov         rcx,rbx
  000000006707067E: E8 2D 55 FB FF     call        0000000067025BB0
  0000000067070683: 49 0B DC           or          rbx,r12
  0000000067070686: 48 89 5C 24 78     mov         qword ptr [rsp+78h],rbx
  000000006707068B: EB 05              jmp         0000000067070692
  000000006707068D: 48 89 6C 24 78     mov         qword ptr [rsp+78h],rbp
  0000000067070692: 48 83 3D 7E 37 28  cmp         qword ptr [00000000672F3E18h],0
                    00 00
  000000006707069A: 75 2D              jne         00000000670706C9
  000000006707069C: B9 08 00 00 00     mov         ecx,8
  00000000670706A1: E8 E0 83 10 00     call        0000000067178A86
  00000000670706A6: 48 85 C0           test        rax,rax
  00000000670706A9: 74 14              je          00000000670706BF
  00000000670706AB: 48 8D 0D 56 03 28  lea         rcx,[00000000672F0A08h]
                    00
  00000000670706B2: 48 89 08           mov         qword ptr [rax],rcx
  00000000670706B5: F0 83 05 5B 03 28  lock add    dword ptr [00000000672F0A18h],1
                    00 01
  00000000670706BD: EB 03              jmp         00000000670706C2
  00000000670706BF: 48 8B C5           mov         rax,rbp
  00000000670706C2: 48 89 05 4F 37 28  mov         qword ptr [00000000672F3E18h],rax
                    00
  00000000670706C9: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670706CC: 66 83 48 18 08     or          word ptr [rax+18h],8
  00000000670706D1: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  00000000670706D6: 48 8B CF           mov         rcx,rdi
  00000000670706D9: E8 92 DD FF FF     call        000000006706E470
  00000000670706DE: 90                 nop
  00000000670706DF: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670706E2: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000000670706EA: 4C 8D 84 24 88 00  lea         r8,[rsp+0000000000000088h]
                    00 00
  00000000670706F2: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  00000000670706FA: 48 8B 0D 17 37 28  mov         rcx,qword ptr [00000000672F3E18h]
                    00
  0000000067070701: E8 7A F2 FF FF     call        000000006706F980
  0000000067070706: 90                 nop
  0000000067070707: 48 8B 9C 24 80 00  mov         rbx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006707070F: 48 63 4B 08        movsxd      rcx,dword ptr [rbx+8]
  0000000067070713: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000067070718: 3B 48 08           cmp         ecx,dword ptr [rax+8]
  000000006707071B: 75 7E              jne         000000006707079B
  000000006707071D: 48 8B 73 10        mov         rsi,qword ptr [rbx+10h]
  0000000067070721: 4C 8B C1           mov         r8,rcx
  0000000067070724: 48 8B 50 10        mov         rdx,qword ptr [rax+10h]
  0000000067070728: 48 8B CE           mov         rcx,rsi
  000000006707072B: E8 3C 84 10 00     call        0000000067178B6C
  0000000067070730: 85 C0              test        eax,eax
  0000000067070732: 75 67              jne         000000006707079B
  0000000067070734: F0 83 03 FF        lock add    dword ptr [rbx],0FFFFFFFFh
  0000000067070738: 75 0E              jne         0000000067070748
  000000006707073A: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000067070742: E8 69 FC FA FF     call        00000000670203B0
  0000000067070747: 90                 nop
  0000000067070748: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  000000006707074D: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067070751: 75 0B              jne         000000006707075E
  0000000067070753: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000067070758: E8 53 FC FA FF     call        00000000670203B0
  000000006707075D: 90                 nop
  000000006707075E: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000067070763: F6 C1 01           test        cl,1
  0000000067070766: 74 2B              je          0000000067070793
  0000000067070768: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  000000006707076C: 48 89 4C 24 78     mov         qword ptr [rsp+78h],rcx
  0000000067070771: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  0000000067070775: 48 8B 11           mov         rdx,qword ptr [rcx]
  0000000067070778: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006707077C: 74 07              je          0000000067070785
  000000006707077E: E8 AD 53 FB FF     call        0000000067025B30
  0000000067070783: EB 0E              jmp         0000000067070793
  0000000067070785: 41 8B C4           mov         eax,r12d
  0000000067070788: F0 0F B1 2A        lock cmpxchg dword ptr [rdx],ebp
  000000006707078C: 74 05              je          0000000067070793
  000000006707078E: E8 DD 53 FB FF     call        0000000067025B70
  0000000067070793: 48 8B C6           mov         rax,rsi
  0000000067070796: E9 8E 00 00 00     jmp         0000000067070829
  000000006707079B: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006707079E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000000670707A3: 4C 8D 4C 24 70     lea         r9,[rsp+70h]
  00000000670707A8: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  00000000670707AD: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000670707B2: 48 8B 0D 5F 36 28  mov         rcx,qword ptr [00000000672F3E18h]
                    00
  00000000670707B9: E8 C2 F7 FF FF     call        000000006706FF80
  00000000670707BE: 4C 8B 5C 24 70     mov         r11,qword ptr [rsp+70h]
  00000000670707C3: 49 8B 73 10        mov         rsi,qword ptr [r11+10h]
  00000000670707C7: F0 83 03 FF        lock add    dword ptr [rbx],0FFFFFFFFh
  00000000670707CB: 75 0E              jne         00000000670707DB
  00000000670707CD: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000000670707D5: E8 D6 FB FA FF     call        00000000670203B0
  00000000670707DA: 90                 nop
  00000000670707DB: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  00000000670707E0: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670707E4: 75 0B              jne         00000000670707F1
  00000000670707E6: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000000670707EB: E8 C0 FB FA FF     call        00000000670203B0
  00000000670707F0: 90                 nop
  00000000670707F1: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000000670707F6: F6 C1 01           test        cl,1
  00000000670707F9: 74 2B              je          0000000067070826
  00000000670707FB: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  00000000670707FF: 48 89 4C 24 78     mov         qword ptr [rsp+78h],rcx
  0000000067070804: 48 83 E1 FE        and         rcx,0FFFFFFFFFFFFFFFEh
  0000000067070808: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006707080B: F6 42 04 01        test        byte ptr [rdx+4],1
  000000006707080F: 74 07              je          0000000067070818
  0000000067070811: E8 1A 53 FB FF     call        0000000067025B30
  0000000067070816: EB 0E              jmp         0000000067070826
  0000000067070818: 41 8B C4           mov         eax,r12d
  000000006707081B: F0 0F B1 2A        lock cmpxchg dword ptr [rdx],ebp
  000000006707081F: 74 05              je          0000000067070826
  0000000067070821: E8 4A 53 FB FF     call        0000000067025B70
  0000000067070826: 48 8B C6           mov         rax,rsi
  0000000067070829: 48 83 C4 40        add         rsp,40h
  000000006707082D: 41 5C              pop         r12
  000000006707082F: 5F                 pop         rdi
  0000000067070830: 5E                 pop         rsi
  0000000067070831: 5D                 pop         rbp
  0000000067070832: 5B                 pop         rbx
  0000000067070833: C3                 ret

  Summary

      1A7000 .text
