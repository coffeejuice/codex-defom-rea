Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653C61E0: 48 8B C4           mov         rax,rsp
  00000000653C61E3: 56                 push        rsi
  00000000653C61E4: 57                 push        rdi
  00000000653C61E5: 41 54              push        r12
  00000000653C61E7: 41 55              push        r13
  00000000653C61E9: 41 56              push        r14
  00000000653C61EB: 48 83 EC 40        sub         rsp,40h
  00000000653C61EF: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653C61F8: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  00000000653C61FC: 48 89 68 20        mov         qword ptr [rax+20h],rbp
  00000000653C6200: 45 0F B6 E9        movzx       r13d,r9b
  00000000653C6204: 41 8B E8           mov         ebp,r8d
  00000000653C6207: 48 8B DA           mov         rbx,rdx
  00000000653C620A: 48 8B F9           mov         rdi,rcx
  00000000653C620D: 45 33 F6           xor         r14d,r14d
  00000000653C6210: 45 8B E6           mov         r12d,r14d
  00000000653C6213: 44 89 70 18        mov         dword ptr [rax+18h],r14d
  00000000653C6217: 8B 51 38           mov         edx,dword ptr [rcx+38h]
  00000000653C621A: 45 8B D6           mov         r10d,r14d
  00000000653C621D: 8B 81 B0 00 00 00  mov         eax,dword ptr [rcx+00000000000000B0h]
  00000000653C6223: 39 81 B4 00 00 00  cmp         dword ptr [rcx+00000000000000B4h],eax
  00000000653C6229: 41 0F 9F C2        setg        r10b
  00000000653C622D: 41 C1 E2 07        shl         r10d,7
  00000000653C6231: 44 0B D2           or          r10d,edx
  00000000653C6234: 44 33 D2           xor         r10d,edx
  00000000653C6237: 41 81 E2 80 00 00  and         r10d,80h
                    00
  00000000653C623E: 44 33 D2           xor         r10d,edx
  00000000653C6241: 44 89 51 38        mov         dword ptr [rcx+38h],r10d
  00000000653C6245: 44 89 B1 B4 00 00  mov         dword ptr [rcx+00000000000000B4h],r14d
                    00
  00000000653C624C: 44 89 B1 B0 00 00  mov         dword ptr [rcx+00000000000000B0h],r14d
                    00
  00000000653C6253: E8 08 EE 20 00     call        00000000655D5060
  00000000653C6258: 48 8D 57 10        lea         rdx,[rdi+10h]
  00000000653C625C: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000000653C6261: FF 15 89 07 2D 00  call        qword ptr [00000000656969F0h]
  00000000653C6267: 90                 nop
  00000000653C6268: 4C 39 B7 98 00 00  cmp         qword ptr [rdi+0000000000000098h],r14
                    00
  00000000653C626F: 74 7A              je          00000000653C62EB
  00000000653C6271: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  00000000653C6276: 4C 8B CB           mov         r9,rbx
  00000000653C6279: 45 33 C0           xor         r8d,r8d
  00000000653C627C: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000653C6281: 48 8B CF           mov         rcx,rdi
  00000000653C6284: E8 47 F2 FF FF     call        00000000653C54D0
  00000000653C6289: 90                 nop
  00000000653C628A: 48 8B D0           mov         rdx,rax
  00000000653C628D: 48 8D 4F 10        lea         rcx,[rdi+10h]
  00000000653C6291: FF 15 E9 04 2D 00  call        qword ptr [0000000065696780h]
  00000000653C6297: 90                 nop
  00000000653C6298: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000653C629D: FF 15 BD 04 2D 00  call        qword ptr [0000000065696760h]
  00000000653C62A3: 48 8D 4F 10        lea         rcx,[rdi+10h]
  00000000653C62A7: FF 15 83 0E 2D 00  call        qword ptr [0000000065697130h]
  00000000653C62AD: 8B 5F 4C           mov         ebx,dword ptr [rdi+4Ch]
  00000000653C62B0: 2B D8              sub         ebx,eax
  00000000653C62B2: 48 8D 4F 10        lea         rcx,[rdi+10h]
  00000000653C62B6: FF 15 74 0E 2D 00  call        qword ptr [0000000065697130h]
  00000000653C62BC: 44 8B C0           mov         r8d,eax
  00000000653C62BF: 44 8B CB           mov         r9d,ebx
  00000000653C62C2: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000653C62C7: 48 8B CF           mov         rcx,rdi
  00000000653C62CA: E8 F1 E8 FF FF     call        00000000653C4BC0
  00000000653C62CF: 90                 nop
  00000000653C62D0: 48 8B D0           mov         rdx,rax
  00000000653C62D3: 48 8D 4F 10        lea         rcx,[rdi+10h]
  00000000653C62D7: FF 15 4B 0D 2D 00  call        qword ptr [0000000065697028h]
  00000000653C62DD: 90                 nop
  00000000653C62DE: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000653C62E3: FF 15 77 04 2D 00  call        qword ptr [0000000065696760h]
  00000000653C62E9: EB 4F              jmp         00000000653C633A
  00000000653C62EB: 48 8B CB           mov         rcx,rbx
  00000000653C62EE: FF 15 F4 06 2D 00  call        qword ptr [00000000656969E8h]
  00000000653C62F4: 84 C0              test        al,al
  00000000653C62F6: 75 23              jne         00000000653C631B
  00000000653C62F8: 44 8B 47 4C        mov         r8d,dword ptr [rdi+4Ch]
  00000000653C62FC: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000653C6301: 48 8B CB           mov         rcx,rbx
  00000000653C6304: FF 15 C6 09 2D 00  call        qword ptr [0000000065696CD0h]
  00000000653C630A: 48 8B D8           mov         rbx,rax
  00000000653C630D: 41 BC 01 00 00 00  mov         r12d,1
  00000000653C6313: 44 89 A4 24 80 00  mov         dword ptr [rsp+0000000000000080h],r12d
                    00 00
  00000000653C631B: 48 8B D3           mov         rdx,rbx
  00000000653C631E: 48 8D 4F 10        lea         rcx,[rdi+10h]
  00000000653C6322: FF 15 58 04 2D 00  call        qword ptr [0000000065696780h]
  00000000653C6328: 90                 nop
  00000000653C6329: 41 F6 C4 01        test        r12b,1
  00000000653C632D: 74 0B              je          00000000653C633A
  00000000653C632F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000653C6334: FF 15 26 04 2D 00  call        qword ptr [0000000065696760h]
  00000000653C633A: 48 8D 8F A8 00 00  lea         rcx,[rdi+00000000000000A8h]
                    00
  00000000653C6341: E8 4A 4F FB FF     call        000000006537B290
  00000000653C6346: 44 89 B7 A4 00 00  mov         dword ptr [rdi+00000000000000A4h],r14d
                    00
  00000000653C634D: 44 89 B7 A0 00 00  mov         dword ptr [rdi+00000000000000A0h],r14d
                    00
  00000000653C6354: 85 ED              test        ebp,ebp
  00000000653C6356: 78 0E              js          00000000653C6366
  00000000653C6358: 48 8D 4F 10        lea         rcx,[rdi+10h]
  00000000653C635C: FF 15 CE 0D 2D 00  call        qword ptr [0000000065697130h]
  00000000653C6362: 3B E8              cmp         ebp,eax
  00000000653C6364: 7E 0C              jle         00000000653C6372
  00000000653C6366: 48 8D 4F 10        lea         rcx,[rdi+10h]
  00000000653C636A: FF 15 C0 0D 2D 00  call        qword ptr [0000000065697130h]
  00000000653C6370: 8B E8              mov         ebp,eax
  00000000653C6372: 89 6F 28           mov         dword ptr [rdi+28h],ebp
  00000000653C6375: 48 8D 57 10        lea         rdx,[rdi+10h]
  00000000653C6379: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000000653C637E: FF 15 9C 0E 2D 00  call        qword ptr [0000000065697220h]
  00000000653C6384: 44 0F B6 D8        movzx       r11d,al
  00000000653C6388: 83 67 38 BF        and         dword ptr [rdi+38h],0FFFFFFBFh
  00000000653C638C: 41 83 E3 01        and         r11d,1
  00000000653C6390: 41 C1 E3 06        shl         r11d,6
  00000000653C6394: 44 09 5F 38        or          dword ptr [rdi+38h],r11d
  00000000653C6398: 45 0F B6 CD        movzx       r9d,r13b
  00000000653C639C: 41 B0 01           mov         r8b,1
  00000000653C639F: 83 CA FF           or          edx,0FFFFFFFFh
  00000000653C63A2: 48 8B CF           mov         rcx,rdi
  00000000653C63A5: E8 C6 19 00 00     call        00000000653C7D70
  00000000653C63AA: 84 C0              test        al,al
  00000000653C63AC: 74 1A              je          00000000653C63C8
  00000000653C63AE: 48 8B CF           mov         rcx,rdi
  00000000653C63B1: FF 15 B1 04 2D 00  call        qword ptr [0000000065696868h]
  00000000653C63B7: 48 8B C8           mov         rcx,rax
  00000000653C63BA: 33 D2              xor         edx,edx
  00000000653C63BC: 41 B8 20 01 00 00  mov         r8d,120h
  00000000653C63C2: E8 99 C5 0D 00     call        00000000654A2960
  00000000653C63C7: 90                 nop
  00000000653C63C8: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000000653C63CD: FF 15 8D 03 2D 00  call        qword ptr [0000000065696760h]
  00000000653C63D3: 48 8B 5C 24 78     mov         rbx,qword ptr [rsp+78h]
  00000000653C63D8: 48 8B AC 24 88 00  mov         rbp,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000653C63E0: 48 83 C4 40        add         rsp,40h
  00000000653C63E4: 41 5E              pop         r14
  00000000653C63E6: 41 5D              pop         r13
  00000000653C63E8: 41 5C              pop         r12
  00000000653C63EA: 5F                 pop         rdi
  00000000653C63EB: 5E                 pop         rsi
  00000000653C63EC: C3                 ret

  Summary

      695000 .text
