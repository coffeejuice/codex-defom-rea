
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412F0DA0: 40 56              push        rsi
  00000001412F0DA2: 57                 push        rdi
  00000001412F0DA3: 41 54              push        r12
  00000001412F0DA5: B8 20 21 00 00     mov         eax,2120h
  00000001412F0DAA: E8 B1 4A 50 00     call        00000001417F5860
  00000001412F0DAF: 48 2B E0           sub         rsp,rax
  00000001412F0DB2: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001412F0DBB: 48 89 9C 24 50 21  mov         qword ptr [rsp+0000000000002150h],rbx
                    00 00
  00000001412F0DC3: 48 89 AC 24 58 21  mov         qword ptr [rsp+0000000000002158h],rbp
                    00 00
  00000001412F0DCB: 48 8B 05 5E D2 86  mov         rax,qword ptr [0000000143B5E030h]
                    02
  00000001412F0DD2: 48 33 C4           xor         rax,rsp
  00000001412F0DD5: 48 89 84 24 10 21  mov         qword ptr [rsp+0000000000002110h],rax
                    00 00
  00000001412F0DDD: 48 8B DA           mov         rbx,rdx
  00000001412F0DE0: 48 8B E9           mov         rbp,rcx
  00000001412F0DE3: 48 89 54 24 38     mov         qword ptr [rsp+38h],rdx
  00000001412F0DE8: C6 44 24 30 00     mov         byte ptr [rsp+30h],0
  00000001412F0DED: 45 33 C9           xor         r9d,r9d
  00000001412F0DF0: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  00000001412F0DF5: E8 C6 68 FF FF     call        00000001412E76C0
  00000001412F0DFA: 48 8B F0           mov         rsi,rax
  00000001412F0DFD: 44 38 4C 24 30     cmp         byte ptr [rsp+30h],r9b
  00000001412F0E02: 0F 84 2F 02 00 00  je          00000001412F1037
  00000001412F0E08: 44 38 08           cmp         byte ptr [rax],r9b
  00000001412F0E0B: 0F 84 26 02 00 00  je          00000001412F1037
  00000001412F0E11: E8 6A 49 FF FF     call        00000001412E5780
  00000001412F0E16: 44 8B E0           mov         r12d,eax
  00000001412F0E19: 85 C0              test        eax,eax
  00000001412F0E1B: 0F 88 16 02 00 00  js          00000001412F1037
  00000001412F0E21: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001412F0E26: 48 8D 8D 58 14 00  lea         rcx,[rbp+0000000000001458h]
                    00
  00000001412F0E2D: E8 7E FC FF FF     call        00000001412F0AB0
  00000001412F0E32: 48 8B 38           mov         rdi,qword ptr [rax]
  00000001412F0E35: 48 85 FF           test        rdi,rdi
  00000001412F0E38: 74 1F              je          00000001412F0E59
  00000001412F0E3A: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001412F0E3F: 48 8D 8D 58 14 00  lea         rcx,[rbp+0000000000001458h]
                    00
  00000001412F0E46: E8 65 FD FF FF     call        00000001412F0BB0
  00000001412F0E4B: 4C 8B 1F           mov         r11,qword ptr [rdi]
  00000001412F0E4E: BA 01 00 00 00     mov         edx,1
  00000001412F0E53: 48 8B CF           mov         rcx,rdi
  00000001412F0E56: 41 FF 13           call        qword ptr [r11]
  00000001412F0E59: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001412F0E61: E8 2A 73 01 00     call        0000000141308190
  00000001412F0E66: 90                 nop
  00000001412F0E67: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001412F0E6F: E8 8C 64 01 00     call        0000000141307300
  00000001412F0E74: 48 8B D0           mov         rdx,rax
  00000001412F0E77: 4C 8B C3           mov         r8,rbx
  00000001412F0E7A: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001412F0E82: E8 B9 68 01 00     call        0000000141307740
  00000001412F0E87: 48 85 C0           test        rax,rax
  00000001412F0E8A: 75 22              jne         00000001412F0EAE
  00000001412F0E8C: 48 8B D3           mov         rdx,rbx
  00000001412F0E8F: 48 8D 0D 42 51 AC  lea         rcx,[0000000142DB5FD8h]
                    01
  00000001412F0E96: E8 F5 18 18 00     call        0000000141472790
  00000001412F0E9B: 90                 nop
  00000001412F0E9C: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001412F0EA4: E8 17 6D 01 00     call        0000000141307BC0
  00000001412F0EA9: E9 89 01 00 00     jmp         00000001412F1037
  00000001412F0EAE: 48 8B C8           mov         rcx,rax
  00000001412F0EB1: E8 F6 28 18 00     call        00000001414737AC
  00000001412F0EB6: 8B D8              mov         ebx,eax
  00000001412F0EB8: E8 E3 49 01 00     call        00000001413058A0
  00000001412F0EBD: 8B F8              mov         edi,eax
  00000001412F0EBF: E8 2C C9 00 00     call        00000001412FD7F0
  00000001412F0EC4: C7 84 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],1
                    00 01 00 00 00
  00000001412F0ECF: 89 5C 24 7C        mov         dword ptr [rsp+7Ch],ebx
  00000001412F0ED3: 33 DB              xor         ebx,ebx
  00000001412F0ED5: 89 5C 24 70        mov         dword ptr [rsp+70h],ebx
  00000001412F0ED9: 89 7C 24 74        mov         dword ptr [rsp+74h],edi
  00000001412F0EDD: 89 44 24 78        mov         dword ptr [rsp+78h],eax
  00000001412F0EE1: C7 44 24 68 02 00  mov         dword ptr [rsp+68h],2
                    00 00
  00000001412F0EE9: C7 44 24 6C 02 00  mov         dword ptr [rsp+6Ch],2
                    00 00
  00000001412F0EF1: 44 89 A4 24 B8 00  mov         dword ptr [rsp+00000000000000B8h],r12d
                    00 00
  00000001412F0EF9: 48 8D 8C 24 D0 05  lea         rcx,[rsp+00000000000005D0h]
                    00 00
  00000001412F0F01: E8 4A 8E 01 00     call        0000000141309D50
  00000001412F0F06: 90                 nop
  00000001412F0F07: 48 8D 8C 24 D0 05  lea         rcx,[rsp+00000000000005D0h]
                    00 00
  00000001412F0F0F: E8 6C 8C 01 00     call        0000000141309B80
  00000001412F0F14: 48 8B C8           mov         rcx,rax
  00000001412F0F17: 48 8D 84 24 AC 00  lea         rax,[rsp+00000000000000ACh]
                    00 00
  00000001412F0F1F: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001412F0F24: 48 8D 84 24 A8 00  lea         rax,[rsp+00000000000000A8h]
                    00 00
  00000001412F0F2C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001412F0F31: 4C 8D 8C 24 A4 00  lea         r9,[rsp+00000000000000A4h]
                    00 00
  00000001412F0F39: 4C 8D 84 24 A0 00  lea         r8,[rsp+00000000000000A0h]
                    00 00
  00000001412F0F41: 48 8D 15 80 3A AC  lea         rdx,[0000000142DB49C8h]
                    01
  00000001412F0F48: E8 87 D0 17 00     call        000000014146DFD4
  00000001412F0F4D: C7 44 24 40 03 00  mov         dword ptr [rsp+40h],3
                    00 00
  00000001412F0F55: C7 44 24 44 05 00  mov         dword ptr [rsp+44h],5
                    00 00
  00000001412F0F5D: 89 5C 24 48        mov         dword ptr [rsp+48h],ebx
  00000001412F0F61: C7 44 24 50 01 00  mov         dword ptr [rsp+50h],1
                    00 00
  00000001412F0F69: C7 44 24 54 B0 00  mov         dword ptr [rsp+54h],0B0h
                    00 00
  00000001412F0F71: 33 D2              xor         edx,edx
  00000001412F0F73: 44 8D 43 01        lea         r8d,[rbx+1]
  00000001412F0F77: 48 8D 8C 24 80 0E  lea         rcx,[rsp+0000000000000E80h]
                    00 00
  00000001412F0F7F: E8 1C 37 FF FF     call        00000001412E46A0
  00000001412F0F84: 90                 nop
  00000001412F0F85: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001412F0F8A: 48 8D 8C 24 80 0E  lea         rcx,[rsp+0000000000000E80h]
                    00 00
  00000001412F0F92: E8 69 12 00 00     call        00000001412F2200
  00000001412F0F97: 83 F8 18           cmp         eax,18h
  00000001412F0F9A: 74 2B              je          00000001412F0FC7
  00000001412F0F9C: 48 8D 8C 24 80 0E  lea         rcx,[rsp+0000000000000E80h]
                    00 00
  00000001412F0FA4: E8 37 34 FF FF     call        00000001412E43E0
  00000001412F0FA9: 90                 nop
  00000001412F0FAA: 48 8D 8C 24 D0 05  lea         rcx,[rsp+00000000000005D0h]
                    00 00
  00000001412F0FB2: E8 69 8B 01 00     call        0000000141309B20
  00000001412F0FB7: 90                 nop
  00000001412F0FB8: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001412F0FC0: E8 FB 6B 01 00     call        0000000141307BC0
  00000001412F0FC5: EB 70              jmp         00000001412F1037
  00000001412F0FC7: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000001412F0FCC: 48 8D 8C 24 80 0E  lea         rcx,[rsp+0000000000000E80h]
                    00 00
  00000001412F0FD4: E8 47 12 00 00     call        00000001412F2220
  00000001412F0FD9: 3D B8 00 00 00     cmp         eax,0B8h
  00000001412F0FDE: 74 2B              je          00000001412F100B
  00000001412F0FE0: 48 8D 8C 24 80 0E  lea         rcx,[rsp+0000000000000E80h]
                    00 00
  00000001412F0FE8: E8 F3 33 FF FF     call        00000001412E43E0
  00000001412F0FED: 90                 nop
  00000001412F0FEE: 48 8D 8C 24 D0 05  lea         rcx,[rsp+00000000000005D0h]
                    00 00
  00000001412F0FF6: E8 25 8B 01 00     call        0000000141309B20
  00000001412F0FFB: 90                 nop
  00000001412F0FFC: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001412F1004: E8 B7 6B 01 00     call        0000000141307BC0
  00000001412F1009: EB 2C              jmp         00000001412F1037
  00000001412F100B: C6 06 00           mov         byte ptr [rsi],0
  00000001412F100E: 48 8D 8C 24 80 0E  lea         rcx,[rsp+0000000000000E80h]
                    00 00
  00000001412F1016: E8 C5 33 FF FF     call        00000001412E43E0
  00000001412F101B: 90                 nop
  00000001412F101C: 48 8D 8C 24 D0 05  lea         rcx,[rsp+00000000000005D0h]
                    00 00
  00000001412F1024: E8 F7 8A 01 00     call        0000000141309B20
  00000001412F1029: 90                 nop
  00000001412F102A: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001412F1032: E8 89 6B 01 00     call        0000000141307BC0
  00000001412F1037: 48 8B 8C 24 10 21  mov         rcx,qword ptr [rsp+0000000000002110h]
                    00 00
  00000001412F103F: 48 33 CC           xor         rcx,rsp
  00000001412F1042: E8 C9 CE 17 00     call        000000014146DF10
  00000001412F1047: 4C 8D 9C 24 20 21  lea         r11,[rsp+0000000000002120h]
                    00 00
  00000001412F104F: 49 8B 5B 30        mov         rbx,qword ptr [r11+30h]
  00000001412F1053: 49 8B 6B 38        mov         rbp,qword ptr [r11+38h]
  00000001412F1057: 49 8B E3           mov         rsp,r11
  00000001412F105A: 41 5C              pop         r12
  00000001412F105C: 5F                 pop         rdi
  00000001412F105D: 5E                 pop         rsi
  00000001412F105E: C3                 ret

  Summary

     1E02000 .text
