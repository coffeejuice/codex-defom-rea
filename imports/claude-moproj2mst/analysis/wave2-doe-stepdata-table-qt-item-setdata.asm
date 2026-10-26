
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006552C920: 48 8B C4           mov         rax,rsp
  000000006552C923: 57                 push        rdi
  000000006552C924: 41 54              push        r12
  000000006552C926: 41 55              push        r13
  000000006552C928: 48 83 EC 50        sub         rsp,50h
  000000006552C92C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006552C935: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  000000006552C939: 48 89 68 18        mov         qword ptr [rax+18h],rbp
  000000006552C93D: 48 89 70 20        mov         qword ptr [rax+20h],rsi
  000000006552C941: 41 8B F0           mov         esi,r8d
  000000006552C944: 48 8B EA           mov         rbp,rdx
  000000006552C947: 4C 8B E1           mov         r12,rcx
  000000006552C94A: 4C 8B 69 08        mov         r13,qword ptr [rcx+8]
  000000006552C94E: 33 C0              xor         eax,eax
  000000006552C950: 41 83 F8 02        cmp         r8d,2
  000000006552C954: 0F 44 F0           cmove       esi,eax
  000000006552C957: 8D 50 01           lea         edx,[rax+1]
  000000006552C95A: 49 8B 4D 18        mov         rcx,qword ptr [r13+18h]
  000000006552C95E: FF 15 0C A1 16 00  call        qword ptr [0000000065696A70h]
  000000006552C964: 84 C0              test        al,al
  000000006552C966: 74 14              je          000000006552C97C
  000000006552C968: 49 8B 45 18        mov         rax,qword ptr [r13+18h]
  000000006552C96C: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006552C970: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006552C973: 49 8D 4D 18        lea         rcx,[r13+18h]
  000000006552C977: E8 84 2A FD FF     call        00000000654FF400
  000000006552C97C: 49 8B 5D 18        mov         rbx,qword ptr [r13+18h]
  000000006552C980: 48 83 C3 10        add         rbx,10h
  000000006552C984: 66 66 66 66 0F 1F  nop         word ptr [rax+rax+0000000000000000h]
                    84 00 00 00 00 00
  000000006552C990: BA 01 00 00 00     mov         edx,1
  000000006552C995: 49 8B 4D 18        mov         rcx,qword ptr [r13+18h]
  000000006552C999: FF 15 D1 A0 16 00  call        qword ptr [0000000065696A70h]
  000000006552C99F: 84 C0              test        al,al
  000000006552C9A1: 74 14              je          000000006552C9B7
  000000006552C9A3: 49 8B 45 18        mov         rax,qword ptr [r13+18h]
  000000006552C9A7: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006552C9AB: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006552C9AE: 49 8D 4D 18        lea         rcx,[r13+18h]
  000000006552C9B2: E8 49 2A FD FF     call        00000000654FF400
  000000006552C9B7: 49 8B 55 18        mov         rdx,qword ptr [r13+18h]
  000000006552C9BB: 48 63 42 08        movsxd      rax,dword ptr [rdx+8]
  000000006552C9BF: 48 8D 0C 40        lea         rcx,[rax+rax*2]
  000000006552C9C3: 48 8D 44 CA 10     lea         rax,[rdx+rcx*8+10h]
  000000006552C9C8: 48 3B D8           cmp         rbx,rax
  000000006552C9CB: 74 7C              je          000000006552CA49
  000000006552C9CD: 39 33              cmp         dword ptr [rbx],esi
  000000006552C9CF: 74 06              je          000000006552C9D7
  000000006552C9D1: 48 83 C3 18        add         rbx,18h
  000000006552C9D5: EB B9              jmp         000000006552C990
  000000006552C9D7: 48 8B CD           mov         rcx,rbp
  000000006552C9DA: FF 15 00 B8 16 00  call        qword ptr [00000000656981E0h]
  000000006552C9E0: 84 C0              test        al,al
  000000006552C9E2: 74 3E              je          000000006552CA22
  000000006552C9E4: 48 8D 7B 08        lea         rdi,[rbx+8]
  000000006552C9E8: 48 8B CF           mov         rcx,rdi
  000000006552C9EB: FF 15 57 A6 16 00  call        qword ptr [0000000065697048h]
  000000006552C9F1: 8B D8              mov         ebx,eax
  000000006552C9F3: 48 8B CD           mov         rcx,rbp
  000000006552C9F6: FF 15 4C A6 16 00  call        qword ptr [0000000065697048h]
  000000006552C9FC: 3B D8              cmp         ebx,eax
  000000006552C9FE: 75 14              jne         000000006552CA14
  000000006552CA00: 48 8B D5           mov         rdx,rbp
  000000006552CA03: 48 8B CF           mov         rcx,rdi
  000000006552CA06: FF 15 1C B9 16 00  call        qword ptr [0000000065698328h]
  000000006552CA0C: 84 C0              test        al,al
  000000006552CA0E: 0F 85 97 00 00 00  jne         000000006552CAAB
  000000006552CA14: 48 8B D5           mov         rdx,rbp
  000000006552CA17: 48 8B CF           mov         rcx,rdi
  000000006552CA1A: FF 15 70 9D 16 00  call        qword ptr [0000000065696790h]
  000000006552CA20: EB 10              jmp         000000006552CA32
  000000006552CA22: 4C 8D 43 18        lea         r8,[rbx+18h]
  000000006552CA26: 48 8B D3           mov         rdx,rbx
  000000006552CA29: 49 8D 4D 18        lea         rcx,[r13+18h]
  000000006552CA2D: E8 6E 49 FE FF     call        00000000655113A0
  000000006552CA32: 49 8B 4D 08        mov         rcx,qword ptr [r13+8]
  000000006552CA36: 48 85 C9           test        rcx,rcx
  000000006552CA39: 74 70              je          000000006552CAAB
  000000006552CA3B: 49 8B D4           mov         rdx,r12
  000000006552CA3E: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006552CA42: E8 F9 E0 FF FF     call        000000006552AB40
  000000006552CA47: EB 62              jmp         000000006552CAAB
  000000006552CA49: 48 8B D5           mov         rdx,rbp
  000000006552CA4C: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006552CA51: FF 15 91 9C 16 00  call        qword ptr [00000000656966E8h]
  000000006552CA57: 48 8B D8           mov         rbx,rax
  000000006552CA5A: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000006552CA5F: 89 74 24 38        mov         dword ptr [rsp+38h],esi
  000000006552CA63: 48 8B D0           mov         rdx,rax
  000000006552CA66: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006552CA6B: FF 15 77 9C 16 00  call        qword ptr [00000000656966E8h]
  000000006552CA71: 90                 nop
  000000006552CA72: 48 8B CB           mov         rcx,rbx
  000000006552CA75: FF 15 65 9C 16 00  call        qword ptr [00000000656966E0h]
  000000006552CA7B: 90                 nop
  000000006552CA7C: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006552CA81: 49 8D 4D 18        lea         rcx,[r13+18h]
  000000006552CA85: E8 76 45 FD FF     call        0000000065501000
  000000006552CA8A: 90                 nop
  000000006552CA8B: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006552CA90: FF 15 4A 9C 16 00  call        qword ptr [00000000656966E0h]
  000000006552CA96: 49 8B 4D 08        mov         rcx,qword ptr [r13+8]
  000000006552CA9A: 48 85 C9           test        rcx,rcx
  000000006552CA9D: 74 0C              je          000000006552CAAB
  000000006552CA9F: 49 8B D4           mov         rdx,r12
  000000006552CAA2: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006552CAA6: E8 95 E0 FF FF     call        000000006552AB40
  000000006552CAAB: 4C 8D 5C 24 50     lea         r11,[rsp+50h]
  000000006552CAB0: 49 8B 5B 28        mov         rbx,qword ptr [r11+28h]
  000000006552CAB4: 49 8B 6B 30        mov         rbp,qword ptr [r11+30h]
  000000006552CAB8: 49 8B 73 38        mov         rsi,qword ptr [r11+38h]
  000000006552CABC: 49 8B E3           mov         rsp,r11
  000000006552CABF: 41 5D              pop         r13
  000000006552CAC1: 41 5C              pop         r12
  000000006552CAC3: 5F                 pop         rdi
  000000006552CAC4: C3                 ret

  Summary

      695000 .text
