Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400A7B90: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400A7B94: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400A7B99: 56                 push        rsi
  00000001400A7B9A: 57                 push        rdi
  00000001400A7B9B: 48 81 EC 18 03 00  sub         rsp,318h
                    00
  00000001400A7BA2: 48 C7 84 24 F0 02  mov         qword ptr [rsp+00000000000002F0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400A7BAE: 44 8B 84 24 38 03  mov         r8d,dword ptr [rsp+0000000000000338h]
                    00 00
  00000001400A7BB6: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000001400A7BBB: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7BC3: E8 18 FD FF FF     call        00000001400A78E0
  00000001400A7BC8: 90                 nop
  00000001400A7BC9: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7BD1: 8B 84 24 38 03 00  mov         eax,dword ptr [rsp+0000000000000338h]
                    00
  00000001400A7BD8: 89 41 50           mov         dword ptr [rcx+50h],eax
  00000001400A7BDB: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7BE3: 48 83 C1 58        add         rcx,58h
  00000001400A7BE7: 8B 94 24 38 03 00  mov         edx,dword ptr [rsp+0000000000000338h]
                    00
  00000001400A7BEE: E8 1D 90 0B 00     call        0000000140160C10
  00000001400A7BF3: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400A7BF6: 0F B6 40 08        movzx       eax,byte ptr [rax+8]
  00000001400A7BFA: 85 C0              test        eax,eax
  00000001400A7BFC: 75 49              jne         00000001400A7C47
  00000001400A7BFE: 48 8B 84 24 30 03  mov         rax,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7C06: C6 40 54 01        mov         byte ptr [rax+54h],1
  00000001400A7C0A: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000001400A7C0F: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7C17: E8 14 79 FE FF     call        000000014008F530
  00000001400A7C1C: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7C24: 48 83 C1 58        add         rcx,58h
  00000001400A7C28: 8B 94 24 38 03 00  mov         edx,dword ptr [rsp+0000000000000338h]
                    00
  00000001400A7C2F: E8 DC 8F 0B 00     call        0000000140160C10
  00000001400A7C34: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400A7C37: C6 40 08 01        mov         byte ptr [rax+8],1
  00000001400A7C3B: 48 8B 84 24 30 03  mov         rax,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7C43: C6 40 54 00        mov         byte ptr [rax+54h],0
  00000001400A7C47: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7C4F: E8 8C 0D 00 00     call        00000001400A89E0
  00000001400A7C54: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001400A7C59: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001400A7C61: 48 83 C1 58        add         rcx,58h
  00000001400A7C65: 8B 94 24 38 03 00  mov         edx,dword ptr [rsp+0000000000000338h]
                    00
  00000001400A7C6C: E8 9F 8F 0B 00     call        0000000140160C10
  00000001400A7C71: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400A7C74: E8 F7 DD 00 00     call        00000001400B5A70
  00000001400A7C79: 48 89 84 24 F8 02  mov         qword ptr [rsp+00000000000002F8h],rax
                    00 00
  00000001400A7C81: 48 83 BC 24 F8 02  cmp         qword ptr [rsp+00000000000002F8h],0
                    00 00 00
  00000001400A7C8A: 74 16              je          00000001400A7CA2
  00000001400A7C8C: 48 8B 84 24 F8 02  mov         rax,qword ptr [rsp+00000000000002F8h]
                    00 00
  00000001400A7C94: 48 83 C0 10        add         rax,10h
  00000001400A7C98: 48 89 84 24 00 03  mov         qword ptr [rsp+0000000000000300h],rax
                    00 00
  00000001400A7CA0: EB 0C              jmp         00000001400A7CAE
  00000001400A7CA2: 48 C7 84 24 00 03  mov         qword ptr [rsp+0000000000000300h],0
                    00 00 00 00 00 00
  00000001400A7CAE: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  00000001400A7CB3: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400A7CB6: 48 8B 94 24 00 03  mov         rdx,qword ptr [rsp+0000000000000300h]
                    00 00
  00000001400A7CBE: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400A7CC3: FF 90 B8 01 00 00  call        qword ptr [rax+00000000000001B8h]
  00000001400A7CC9: 45 33 C0           xor         r8d,r8d
  00000001400A7CCC: 33 D2              xor         edx,edx
  00000001400A7CCE: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001400A7CD3: FF 15 8F C4 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7CD9: 48 8D BC 24 40 02  lea         rdi,[rsp+0000000000000240h]
                    00 00
  00000001400A7CE1: 48 8B F0           mov         rsi,rax
  00000001400A7CE4: B9 10 00 00 00     mov         ecx,10h
  00000001400A7CE9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7CEB: 45 33 C0           xor         r8d,r8d
  00000001400A7CEE: 33 D2              xor         edx,edx
  00000001400A7CF0: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000001400A7CF8: FF 15 6A C4 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7CFE: 48 8D BC 24 30 02  lea         rdi,[rsp+0000000000000230h]
                    00 00
  00000001400A7D06: 48 8B F0           mov         rsi,rax
  00000001400A7D09: B9 10 00 00 00     mov         ecx,10h
  00000001400A7D0E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7D10: 45 33 C0           xor         r8d,r8d
  00000001400A7D13: 33 D2              xor         edx,edx
  00000001400A7D15: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400A7D1D: FF 15 45 C4 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7D23: 48 8D BC 24 20 02  lea         rdi,[rsp+0000000000000220h]
                    00 00
  00000001400A7D2B: 48 8B F0           mov         rsi,rax
  00000001400A7D2E: B9 10 00 00 00     mov         ecx,10h
  00000001400A7D33: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7D35: 45 33 C0           xor         r8d,r8d
  00000001400A7D38: 33 D2              xor         edx,edx
  00000001400A7D3A: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001400A7D42: FF 15 20 C4 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7D48: 48 8D BC 24 10 02  lea         rdi,[rsp+0000000000000210h]
                    00 00
  00000001400A7D50: 48 8B F0           mov         rsi,rax
  00000001400A7D53: B9 10 00 00 00     mov         ecx,10h
  00000001400A7D58: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7D5A: 45 33 C0           xor         r8d,r8d
  00000001400A7D5D: 33 D2              xor         edx,edx
  00000001400A7D5F: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000001400A7D67: FF 15 FB C3 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7D6D: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  00000001400A7D75: 48 8B F0           mov         rsi,rax
  00000001400A7D78: B9 10 00 00 00     mov         ecx,10h
  00000001400A7D7D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7D7F: 45 33 C0           xor         r8d,r8d
  00000001400A7D82: 33 D2              xor         edx,edx
  00000001400A7D84: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001400A7D8C: FF 15 D6 C3 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7D92: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  00000001400A7D9A: 48 8B F0           mov         rsi,rax
  00000001400A7D9D: B9 10 00 00 00     mov         ecx,10h
  00000001400A7DA2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7DA4: 45 33 C0           xor         r8d,r8d
  00000001400A7DA7: 33 D2              xor         edx,edx
  00000001400A7DA9: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  00000001400A7DB1: FF 15 B1 C3 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7DB7: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  00000001400A7DBF: 48 8B F0           mov         rsi,rax
  00000001400A7DC2: B9 10 00 00 00     mov         ecx,10h
  00000001400A7DC7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7DC9: 45 33 C0           xor         r8d,r8d
  00000001400A7DCC: 33 D2              xor         edx,edx
  00000001400A7DCE: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  00000001400A7DD6: FF 15 8C C3 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7DDC: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  00000001400A7DE4: 48 8B F0           mov         rsi,rax
  00000001400A7DE7: B9 10 00 00 00     mov         ecx,10h
  00000001400A7DEC: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7DEE: 45 33 C0           xor         r8d,r8d
  00000001400A7DF1: 33 D2              xor         edx,edx
  00000001400A7DF3: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  00000001400A7DFB: FF 15 67 C3 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7E01: 48 8D BC 24 C0 01  lea         rdi,[rsp+00000000000001C0h]
                    00 00
  00000001400A7E09: 48 8B F0           mov         rsi,rax
  00000001400A7E0C: B9 10 00 00 00     mov         ecx,10h
  00000001400A7E11: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7E13: 45 33 C0           xor         r8d,r8d
  00000001400A7E16: 33 D2              xor         edx,edx
  00000001400A7E18: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  00000001400A7E20: FF 15 42 C3 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7E26: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  00000001400A7E2E: 48 8B F0           mov         rsi,rax
  00000001400A7E31: B9 10 00 00 00     mov         ecx,10h
  00000001400A7E36: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7E38: 48 8D 84 24 40 02  lea         rax,[rsp+0000000000000240h]
                    00 00
  00000001400A7E40: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001400A7E45: 48 8D 84 24 30 02  lea         rax,[rsp+0000000000000230h]
                    00 00
  00000001400A7E4D: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400A7E52: 48 8D 84 24 20 02  lea         rax,[rsp+0000000000000220h]
                    00 00
  00000001400A7E5A: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400A7E5F: 48 8D 84 24 10 02  lea         rax,[rsp+0000000000000210h]
                    00 00
  00000001400A7E67: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400A7E6C: 48 8D 84 24 00 02  lea         rax,[rsp+0000000000000200h]
                    00 00
  00000001400A7E74: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400A7E79: 48 8D 84 24 F0 01  lea         rax,[rsp+00000000000001F0h]
                    00 00
  00000001400A7E81: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400A7E86: 48 8D 84 24 E0 01  lea         rax,[rsp+00000000000001E0h]
                    00 00
  00000001400A7E8E: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400A7E93: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  00000001400A7E9B: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400A7EA0: 4C 8D 8C 24 C0 01  lea         r9,[rsp+00000000000001C0h]
                    00 00
  00000001400A7EA8: 4C 8D 84 24 B0 01  lea         r8,[rsp+00000000000001B0h]
                    00 00
  00000001400A7EB0: 48 8D 15 59 8E 09  lea         rdx,[0000000142140D10h]
                    02
  00000001400A7EB7: 48 8D 0D 42 E8 CC  lea         rcx,[0000000143D76700h]
                    03
  00000001400A7EBE: E8 FD 74 22 00     call        00000001402CF3C0
  00000001400A7EC3: 45 33 C0           xor         r8d,r8d
  00000001400A7EC6: 33 D2              xor         edx,edx
  00000001400A7EC8: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  00000001400A7ED0: FF 15 92 C2 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7ED6: 48 8D BC 24 E0 02  lea         rdi,[rsp+00000000000002E0h]
                    00 00
  00000001400A7EDE: 48 8B F0           mov         rsi,rax
  00000001400A7EE1: B9 10 00 00 00     mov         ecx,10h
  00000001400A7EE6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7EE8: 45 33 C0           xor         r8d,r8d
  00000001400A7EEB: 33 D2              xor         edx,edx
  00000001400A7EED: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  00000001400A7EF5: FF 15 6D C2 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7EFB: 48 8D BC 24 D0 02  lea         rdi,[rsp+00000000000002D0h]
                    00 00
  00000001400A7F03: 48 8B F0           mov         rsi,rax
  00000001400A7F06: B9 10 00 00 00     mov         ecx,10h
  00000001400A7F0B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7F0D: 45 33 C0           xor         r8d,r8d
  00000001400A7F10: 33 D2              xor         edx,edx
  00000001400A7F12: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  00000001400A7F1A: FF 15 48 C2 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7F20: 48 8D BC 24 C0 02  lea         rdi,[rsp+00000000000002C0h]
                    00 00
  00000001400A7F28: 48 8B F0           mov         rsi,rax
  00000001400A7F2B: B9 10 00 00 00     mov         ecx,10h
  00000001400A7F30: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7F32: 45 33 C0           xor         r8d,r8d
  00000001400A7F35: 33 D2              xor         edx,edx
  00000001400A7F37: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  00000001400A7F3F: FF 15 23 C2 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7F45: 48 8D BC 24 B0 02  lea         rdi,[rsp+00000000000002B0h]
                    00 00
  00000001400A7F4D: 48 8B F0           mov         rsi,rax
  00000001400A7F50: B9 10 00 00 00     mov         ecx,10h
  00000001400A7F55: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7F57: 45 33 C0           xor         r8d,r8d
  00000001400A7F5A: 33 D2              xor         edx,edx
  00000001400A7F5C: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  00000001400A7F64: FF 15 FE C1 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7F6A: 48 8D BC 24 A0 02  lea         rdi,[rsp+00000000000002A0h]
                    00 00
  00000001400A7F72: 48 8B F0           mov         rsi,rax
  00000001400A7F75: B9 10 00 00 00     mov         ecx,10h
  00000001400A7F7A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7F7C: 45 33 C0           xor         r8d,r8d
  00000001400A7F7F: 33 D2              xor         edx,edx
  00000001400A7F81: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  00000001400A7F89: FF 15 D9 C1 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7F8F: 48 8D BC 24 90 02  lea         rdi,[rsp+0000000000000290h]
                    00 00
  00000001400A7F97: 48 8B F0           mov         rsi,rax
  00000001400A7F9A: B9 10 00 00 00     mov         ecx,10h
  00000001400A7F9F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7FA1: 45 33 C0           xor         r8d,r8d
  00000001400A7FA4: 33 D2              xor         edx,edx
  00000001400A7FA6: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  00000001400A7FAE: FF 15 B4 C1 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7FB4: 48 8D BC 24 80 02  lea         rdi,[rsp+0000000000000280h]
                    00 00
  00000001400A7FBC: 48 8B F0           mov         rsi,rax
  00000001400A7FBF: B9 10 00 00 00     mov         ecx,10h
  00000001400A7FC4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7FC6: 45 33 C0           xor         r8d,r8d
  00000001400A7FC9: 33 D2              xor         edx,edx
  00000001400A7FCB: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  00000001400A7FD3: FF 15 8F C1 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7FD9: 48 8D BC 24 70 02  lea         rdi,[rsp+0000000000000270h]
                    00 00
  00000001400A7FE1: 48 8B F0           mov         rsi,rax
  00000001400A7FE4: B9 10 00 00 00     mov         ecx,10h
  00000001400A7FE9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A7FEB: 45 33 C0           xor         r8d,r8d
  00000001400A7FEE: 33 D2              xor         edx,edx
  00000001400A7FF0: 48 8D 8C 24 90 01  lea         rcx,[rsp+0000000000000190h]
                    00 00
  00000001400A7FF8: FF 15 6A C1 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A7FFE: 48 8D BC 24 60 02  lea         rdi,[rsp+0000000000000260h]
                    00 00
  00000001400A8006: 48 8B F0           mov         rsi,rax
  00000001400A8009: B9 10 00 00 00     mov         ecx,10h
  00000001400A800E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A8010: 45 33 C0           xor         r8d,r8d
  00000001400A8013: 33 D2              xor         edx,edx
  00000001400A8015: 48 8D 8C 24 A0 01  lea         rcx,[rsp+00000000000001A0h]
                    00 00
  00000001400A801D: FF 15 45 C1 D5 01  call        qword ptr [0000000141E04168h]
  00000001400A8023: 48 8D BC 24 50 02  lea         rdi,[rsp+0000000000000250h]
                    00 00
  00000001400A802B: 48 8B F0           mov         rsi,rax
  00000001400A802E: B9 10 00 00 00     mov         ecx,10h
  00000001400A8033: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400A8035: 48 8D 84 24 E0 02  lea         rax,[rsp+00000000000002E0h]
                    00 00
  00000001400A803D: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001400A8042: 48 8D 84 24 D0 02  lea         rax,[rsp+00000000000002D0h]
                    00 00
  00000001400A804A: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400A804F: 48 8D 84 24 C0 02  lea         rax,[rsp+00000000000002C0h]
                    00 00
  00000001400A8057: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400A805C: 48 8D 84 24 B0 02  lea         rax,[rsp+00000000000002B0h]
                    00 00
  00000001400A8064: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400A8069: 48 8D 84 24 A0 02  lea         rax,[rsp+00000000000002A0h]
                    00 00
  00000001400A8071: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400A8076: 48 8D 84 24 90 02  lea         rax,[rsp+0000000000000290h]
                    00 00
  00000001400A807E: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400A8083: 48 8D 84 24 80 02  lea         rax,[rsp+0000000000000280h]
                    00 00
  00000001400A808B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400A8090: 48 8D 84 24 70 02  lea         rax,[rsp+0000000000000270h]
                    00 00
  00000001400A8098: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400A809D: 4C 8D 8C 24 60 02  lea         r9,[rsp+0000000000000260h]
                    00 00
  00000001400A80A5: 4C 8D 84 24 50 02  lea         r8,[rsp+0000000000000250h]
                    00 00
  00000001400A80AD: 48 8D 15 7C 8C 09  lea         rdx,[0000000142140D30h]
                    02
  00000001400A80B4: 48 8D 0D 45 E6 CC  lea         rcx,[0000000143D76700h]
                    03
  00000001400A80BB: E8 00 73 22 00     call        00000001402CF3C0
  00000001400A80C0: 90                 nop
  00000001400A80C1: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400A80C6: FF 15 E4 45 D6 01  call        qword ptr [0000000141E0C6B0h]
  00000001400A80CC: 48 81 C4 18 03 00  add         rsp,318h
                    00
  00000001400A80D3: 5F                 pop         rdi
  00000001400A80D4: 5E                 pop         rsi
  00000001400A80D5: C3                 ret

  Summary

     1E02000 .text
