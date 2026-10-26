
Dump of file evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  000000006100C210: 48 8B C4           mov         rax,rsp
  000000006100C213: 57                 push        rdi
  000000006100C214: 48 81 EC 60 01 00  sub         rsp,160h
                    00
  000000006100C21B: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006100C224: 48 89 58 20        mov         qword ptr [rax+20h],rbx
  000000006100C228: 0F 29 70 E8        movaps      xmmword ptr [rax-18h],xmm6
  000000006100C22C: 48 8B 05 1D E4 05  mov         rax,qword ptr [000000006106A650h]
                    00
  000000006100C233: 48 33 C4           xor         rax,rsp
  000000006100C236: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  000000006100C23E: 66 0F 28 F2        movapd      xmm6,xmm2
  000000006100C242: 48 8B FA           mov         rdi,rdx
  000000006100C245: 48 8B D9           mov         rbx,rcx
  000000006100C248: 48 83 39 00        cmp         qword ptr [rcx],0
  000000006100C24C: 0F 84 94 00 00 00  je          000000006100C2E6
  000000006100C252: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006100C257: FF 15 73 76 03 00  call        qword ptr [00000000610438D0h]
  000000006100C25D: 90                 nop
  000000006100C25E: 66 0F 28 DE        movapd      xmm3,xmm6
  000000006100C262: 66 49 0F 7E D9     movq        r9,xmm3
  000000006100C267: 4C 8D 05 7A 9C 03  lea         r8,[0000000061045EE8h]
                    00
  000000006100C26E: BA 00 01 00 00     mov         edx,100h
  000000006100C273: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006100C278: FF 15 B2 70 03 00  call        qword ptr [0000000061043330h]
  000000006100C27E: 85 C0              test        eax,eax
  000000006100C280: 7E 30              jle         000000006100C2B2
  000000006100C282: 44 8B C0           mov         r8d,eax
  000000006100C285: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006100C28A: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006100C28F: FF 15 6B 6F 03 00  call        qword ptr [0000000061043200h]
  000000006100C295: 90                 nop
  000000006100C296: 48 8B D0           mov         rdx,rax
  000000006100C299: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006100C29E: FF 15 0C 76 03 00  call        qword ptr [00000000610438B0h]
  000000006100C2A4: 90                 nop
  000000006100C2A5: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006100C2AA: FF 15 30 76 03 00  call        qword ptr [00000000610438E0h]
  000000006100C2B0: EB 18              jmp         000000006100C2CA
  000000006100C2B2: 41 B9 06 00 00 00  mov         r9d,6
  000000006100C2B8: 41 B0 67           mov         r8b,67h
  000000006100C2BB: 66 0F 28 CE        movapd      xmm1,xmm6
  000000006100C2BF: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006100C2C4: FF 15 5E 70 03 00  call        qword ptr [0000000061043328h]
  000000006100C2CA: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  000000006100C2CF: 48 8B D7           mov         rdx,rdi
  000000006100C2D2: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006100C2D5: E8 46 FA FF FF     call        000000006100BD20
  000000006100C2DA: 90                 nop
  000000006100C2DB: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006100C2E0: FF 15 FA 75 03 00  call        qword ptr [00000000610438E0h]
  000000006100C2E6: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  000000006100C2EE: 48 33 CC           xor         rcx,rsp
  000000006100C2F1: E8 FA A5 02 00     call        00000000610368F0
  000000006100C2F6: 48 8B 9C 24 88 01  mov         rbx,qword ptr [rsp+0000000000000188h]
                    00 00
  000000006100C2FE: 0F 28 B4 24 50 01  movaps      xmm6,xmmword ptr [rsp+0000000000000150h]
                    00 00
  000000006100C306: 48 81 C4 60 01 00  add         rsp,160h
                    00
  000000006100C30D: 5F                 pop         rdi
  000000006100C30E: C3                 ret

  Summary

       42000 .text
