Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140551AE0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140551AE5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140551AEA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140551AEE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140551AF3: 48 83 EC 38        sub         rsp,38h
  0000000140551AF7: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  0000000140551AFC: 0F 85 CF 01 00 00  jne         0000000140551CD1
  0000000140551B02: E8 79 30 AB FF     call        0000000140004B80
  0000000140551B07: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  0000000140551B0C: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  0000000140551B11: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  0000000140551B15: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140551B19: 83 7C 24 28 15     cmp         dword ptr [rsp+28h],15h
  0000000140551B1E: 0F 87 AD 01 00 00  ja          0000000140551CD1
  0000000140551B24: 48 63 44 24 28     movsxd      rax,dword ptr [rsp+28h]
  0000000140551B29: 48 8D 0D D0 E4 AA  lea         rcx,[0000000140000000h]
                    FF
  0000000140551B30: 8B 84 81 D8 1C 55  mov         eax,dword ptr [rcx+rax*4+0000000000551CD8h]
                    00
  0000000140551B37: 48 03 C1           add         rax,rcx
  0000000140551B3A: FF E0              jmp         rax
  0000000140551B3C: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140551B41: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000140551B45: 8B 10              mov         edx,dword ptr [rax]
  0000000140551B47: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551B4C: E8 1F 03 00 00     call        0000000140551E70
  0000000140551B51: E9 7B 01 00 00     jmp         0000000140551CD1
  0000000140551B56: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551B5B: E8 F0 39 F3 FF     call        0000000140485550
  0000000140551B60: E9 6C 01 00 00     jmp         0000000140551CD1
  0000000140551B65: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551B6A: E8 71 47 F3 FF     call        00000001404862E0
  0000000140551B6F: E9 5D 01 00 00     jmp         0000000140551CD1
  0000000140551B74: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551B79: E8 02 48 F3 FF     call        0000000140486380
  0000000140551B7E: E9 4E 01 00 00     jmp         0000000140551CD1
  0000000140551B83: 4C 8B 44 24 58     mov         r8,qword ptr [rsp+58h]
  0000000140551B88: 4D 8B 40 10        mov         r8,qword ptr [r8+10h]
  0000000140551B8C: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140551B91: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140551B95: 4D 8B 00           mov         r8,qword ptr [r8]
  0000000140551B98: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140551B9B: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551BA0: E8 9B EF F3 FF     call        0000000140490B40
  0000000140551BA5: E9 27 01 00 00     jmp         0000000140551CD1
  0000000140551BAA: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140551BAF: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000140551BB3: 8B 10              mov         edx,dword ptr [rax]
  0000000140551BB5: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551BBA: E8 F1 F2 F3 FF     call        0000000140490EB0
  0000000140551BBF: E9 0D 01 00 00     jmp         0000000140551CD1
  0000000140551BC4: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140551BC9: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000140551BCD: 0F B6 10           movzx       edx,byte ptr [rax]
  0000000140551BD0: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551BD5: E8 26 BE F4 FF     call        000000014049DA00
  0000000140551BDA: E9 F2 00 00 00     jmp         0000000140551CD1
  0000000140551BDF: 33 D2              xor         edx,edx
  0000000140551BE1: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551BE6: E8 15 BE F4 FF     call        000000014049DA00
  0000000140551BEB: E9 E1 00 00 00     jmp         0000000140551CD1
  0000000140551BF0: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140551BF5: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000140551BF9: 8B 10              mov         edx,dword ptr [rax]
  0000000140551BFB: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551C00: E8 BB BE F4 FF     call        000000014049DAC0
  0000000140551C05: E9 C7 00 00 00     jmp         0000000140551CD1
  0000000140551C0A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551C0F: E8 1C AC F4 FF     call        000000014049C830
  0000000140551C14: E9 B8 00 00 00     jmp         0000000140551CD1
  0000000140551C19: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551C1E: E8 9D AD F4 FF     call        000000014049C9C0
  0000000140551C23: E9 A9 00 00 00     jmp         0000000140551CD1
  0000000140551C28: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551C2D: E8 1E AF F4 FF     call        000000014049CB50
  0000000140551C32: E9 9A 00 00 00     jmp         0000000140551CD1
  0000000140551C37: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551C3C: E8 9F B0 F4 FF     call        000000014049CCE0
  0000000140551C41: E9 8B 00 00 00     jmp         0000000140551CD1
  0000000140551C46: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551C4B: E8 20 B2 F4 FF     call        000000014049CE70
  0000000140551C50: EB 7F              jmp         0000000140551CD1
  0000000140551C52: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551C57: E8 A4 B3 F4 FF     call        000000014049D000
  0000000140551C5C: EB 73              jmp         0000000140551CD1
  0000000140551C5E: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551C63: E8 78 B5 F4 FF     call        000000014049D1E0
  0000000140551C68: EB 67              jmp         0000000140551CD1
  0000000140551C6A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551C6F: E8 8C B5 F4 FF     call        000000014049D200
  0000000140551C74: EB 5B              jmp         0000000140551CD1
  0000000140551C76: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551C7B: E8 A0 B5 F4 FF     call        000000014049D220
  0000000140551C80: EB 4F              jmp         0000000140551CD1
  0000000140551C82: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551C87: E8 84 B9 F4 FF     call        000000014049D610
  0000000140551C8C: EB 43              jmp         0000000140551CD1
  0000000140551C8E: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140551C93: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000140551C97: 8B 10              mov         edx,dword ptr [rax]
  0000000140551C99: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551C9E: E8 9D BD F4 FF     call        000000014049DA40
  0000000140551CA3: EB 2C              jmp         0000000140551CD1
  0000000140551CA5: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140551CAA: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000140551CAE: 8B 10              mov         edx,dword ptr [rax]
  0000000140551CB0: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551CB5: E8 C6 BD F4 FF     call        000000014049DA80
  0000000140551CBA: EB 15              jmp         0000000140551CD1
  0000000140551CBC: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140551CC1: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  0000000140551CC5: 8B 10              mov         edx,dword ptr [rax]
  0000000140551CC7: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140551CCC: E8 DF BE F4 FF     call        000000014049DBB0
  0000000140551CD1: 48 83 C4 38        add         rsp,38h
  0000000140551CD5: C3                 ret
  0000000140551CD6: 66 90              nop
  0000000140551CD8: 3C 1B              cmp         al,1Bh
  0000000140551CDA: 55                 push        rbp
  0000000140551CDB: 00 56 1B           add         byte ptr [rsi+1Bh],dl
  0000000140551CDE: 55                 push        rbp
  0000000140551CDF: 00 65 1B           add         byte ptr [rbp+1Bh],ah
  0000000140551CE2: 55                 push        rbp
  0000000140551CE3: 00 74 1B 55        add         byte ptr [rbx+rbx+55h],dh
  0000000140551CE7: 00 83 1B 55 00 AA  add         byte ptr [rbx+FFFFFFFFAA00551Bh],al
  0000000140551CED: 1B 55 00           sbb         edx,dword ptr [rbp]
  0000000140551CF0: C4
  0000000140551CF1: 1B 55 00           sbb         edx,dword ptr [rbp]
  0000000140551CF4: DF 1B              fistp       word ptr [rbx]
  0000000140551CF6: 55                 push        rbp
  0000000140551CF7: 00 F0              add         al,dh
  0000000140551CF9: 1B 55 00           sbb         edx,dword ptr [rbp]
  0000000140551CFC: 0A 1C 55 00 19 1C  or          bl,byte ptr [rdx*2+00000000551C1900h]
                    55
  0000000140551D03: 00 28              add         byte ptr [rax],ch
  0000000140551D05: 1C 55              sbb         al,55h
  0000000140551D07: 00 37              add         byte ptr [rdi],dh
  0000000140551D09: 1C 55              sbb         al,55h
  0000000140551D0B: 00 46 1C           add         byte ptr [rsi+1Ch],al
  0000000140551D0E: 55                 push        rbp
  0000000140551D0F: 00 52 1C           add         byte ptr [rdx+1Ch],dl
  0000000140551D12: 55                 push        rbp
  0000000140551D13: 00 5E 1C           add         byte ptr [rsi+1Ch],bl
  0000000140551D16: 55                 push        rbp
  0000000140551D17: 00 6A 1C           add         byte ptr [rdx+1Ch],ch
  0000000140551D1A: 55                 push        rbp
  0000000140551D1B: 00 76 1C           add         byte ptr [rsi+1Ch],dh
  0000000140551D1E: 55                 push        rbp
  0000000140551D1F: 00 82 1C 55 00 8E  add         byte ptr [rdx+FFFFFFFF8E00551Ch],al
  0000000140551D25: 1C 55              sbb         al,55h
  0000000140551D27: 00 A5 1C 55 00 BC  add         byte ptr [rbp+FFFFFFFFBC00551Ch],ah
  0000000140551D2D: 1C 55              sbb         al,55h
  0000000140551D2F: 00

  Summary

     1E02000 .text
