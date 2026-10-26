
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140643400: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140643405: 48 83 EC 58        sub         rsp,58h
  0000000140643409: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014064340E: 48 83 C1 18        add         rcx,18h
  0000000140643412: 48 8B 15 77 0D 7C  mov         rdx,qword ptr [0000000141E04190h]
                    01
  0000000140643419: FF 15 C9 24 7C 01  call        qword ptr [0000000141E058E8h]
  000000014064341F: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140643424: 48 83 C1 20        add         rcx,20h
  0000000140643428: E8 C3 7A 5D 00     call        0000000140C1AEF0
  000000014064342D: 48 85 C0           test        rax,rax
  0000000140643430: 74 5A              je          000000014064348C
  0000000140643432: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140643437: 48 83 C1 20        add         rcx,20h
  000000014064343B: E8 B0 7A 5D 00     call        0000000140C1AEF0
  0000000140643440: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140643445: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014064344A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014064344F: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140643455: 74 1C              je          0000000140643473
  0000000140643457: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014064345C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014064345F: BA 01 00 00 00     mov         edx,1
  0000000140643464: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140643469: FF 50 18           call        qword ptr [rax+18h]
  000000014064346C: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140643471: EB 09              jmp         000000014064347C
  0000000140643473: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  000000014064347C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140643481: 48 83 C1 20        add         rcx,20h
  0000000140643485: 33 D2              xor         edx,edx
  0000000140643487: E8 94 94 67 00     call        0000000140CBC920
  000000014064348C: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140643491: 48 83 78 30 00     cmp         qword ptr [rax+30h],0
  0000000140643496: 74 52              je          00000001406434EA
  0000000140643498: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014064349D: 48 8B 40 30        mov         rax,qword ptr [rax+30h]
  00000001406434A1: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001406434A6: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001406434AB: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001406434B0: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001406434B6: 74 1C              je          00000001406434D4
  00000001406434B8: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001406434BD: 48 8B 00           mov         rax,qword ptr [rax]
  00000001406434C0: BA 01 00 00 00     mov         edx,1
  00000001406434C5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001406434CA: FF 50 18           call        qword ptr [rax+18h]
  00000001406434CD: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001406434D2: EB 09              jmp         00000001406434DD
  00000001406434D4: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  00000001406434DD: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001406434E2: 48 C7 40 30 00 00  mov         qword ptr [rax+30h],0
                    00 00
  00000001406434EA: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001406434EF: E8 9C 39 00 00     call        0000000140646E90
  00000001406434F4: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001406434F9: E8 52 89 00 00     call        000000014064BE50
  00000001406434FE: 48 83 C4 58        add         rsp,58h
  0000000140643502: C3                 ret

  Summary

     1E02000 .text
