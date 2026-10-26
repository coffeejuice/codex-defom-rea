
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DD60D0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140DD60D5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140DD60DA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140DD60DE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DD60E3: 53                 push        rbx
  0000000140DD60E4: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  0000000140DD60EB: 83 BC 24 98 00 00  cmp         dword ptr [rsp+0000000000000098h],0
                    00 00
  0000000140DD60F3: 0F 85 9B 01 00 00  jne         0000000140DD6294
  0000000140DD60F9: E8 82 EA 22 FF     call        0000000140004B80
  0000000140DD60FE: 4C 8B 9C 24 90 00  mov         r11,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140DD6106: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  0000000140DD610B: 8B 84 24 A0 00 00  mov         eax,dword ptr [rsp+00000000000000A0h]
                    00
  0000000140DD6112: 89 44 24 70        mov         dword ptr [rsp+70h],eax
  0000000140DD6116: 83 7C 24 70 06     cmp         dword ptr [rsp+70h],6
  0000000140DD611B: 0F 87 73 01 00 00  ja          0000000140DD6294
  0000000140DD6121: 48 63 44 24 70     movsxd      rax,dword ptr [rsp+70h]
  0000000140DD6126: 48 8D 0D D3 9E 22  lea         rcx,[0000000140000000h]
                    FF
  0000000140DD612D: 8B 84 81 A0 62 DD  mov         eax,dword ptr [rcx+rax*4+0000000000DD62A0h]
                    00
  0000000140DD6134: 48 03 C1           add         rax,rcx
  0000000140DD6137: FF E0              jmp         rax
  0000000140DD6139: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140DD6141: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140DD6145: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140DD614D: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140DD6151: 44 0F B6 09        movzx       r9d,byte ptr [rcx]
  0000000140DD6155: 44 8B 00           mov         r8d,dword ptr [rax]
  0000000140DD6158: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140DD6160: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140DD6164: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD6169: E8 F2 EF F3 FF     call        0000000140D15160
  0000000140DD616E: E9 21 01 00 00     jmp         0000000140DD6294
  0000000140DD6173: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140DD617B: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140DD617F: 45 33 C9           xor         r9d,r9d
  0000000140DD6182: 44 8B 00           mov         r8d,dword ptr [rax]
  0000000140DD6185: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140DD618D: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140DD6191: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD6196: E8 C5 EF F3 FF     call        0000000140D15160
  0000000140DD619B: E9 F4 00 00 00     jmp         0000000140DD6294
  0000000140DD61A0: 48 8D 44 24 28     lea         rax,[rsp+28h]
  0000000140DD61A5: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140DD61AA: 48 8B 9C 24 A8 00  mov         rbx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140DD61B2: 48 8B 5B 18        mov         rbx,qword ptr [rbx+18h]
  0000000140DD61B6: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140DD61BE: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  0000000140DD61C2: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140DD61C7: FF 15 C3 57 03 01  call        qword ptr [0000000141E0B990h]
  0000000140DD61CD: 44 0F B6 0B        movzx       r9d,byte ptr [rbx]
  0000000140DD61D1: 4C 8B C0           mov         r8,rax
  0000000140DD61D4: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140DD61DC: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140DD61E0: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD61E5: E8 06 F0 F3 FF     call        0000000140D151F0
  0000000140DD61EA: E9 A5 00 00 00     jmp         0000000140DD6294
  0000000140DD61EF: 48 8D 44 24 40     lea         rax,[rsp+40h]
  0000000140DD61F4: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140DD61F9: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140DD6201: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  0000000140DD6205: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140DD620A: FF 15 80 57 03 01  call        qword ptr [0000000141E0B990h]
  0000000140DD6210: 45 33 C9           xor         r9d,r9d
  0000000140DD6213: 4C 8B C0           mov         r8,rax
  0000000140DD6216: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140DD621E: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140DD6222: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD6227: E8 C4 EF F3 FF     call        0000000140D151F0
  0000000140DD622C: EB 66              jmp         0000000140DD6294
  0000000140DD622E: 48 8D 44 24 58     lea         rax,[rsp+58h]
  0000000140DD6233: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140DD6238: BA 02 00 00 00     mov         edx,2
  0000000140DD623D: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140DD6242: FF 15 70 5D 03 01  call        qword ptr [0000000141E0BFB8h]
  0000000140DD6248: 45 33 C9           xor         r9d,r9d
  0000000140DD624B: 4C 8B C0           mov         r8,rax
  0000000140DD624E: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140DD6256: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140DD625A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD625F: E8 8C EF F3 FF     call        0000000140D151F0
  0000000140DD6264: EB 2E              jmp         0000000140DD6294
  0000000140DD6266: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140DD626E: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  0000000140DD6272: 48 8B 12           mov         rdx,qword ptr [rdx]
  0000000140DD6275: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD627A: E8 91 F0 F3 FF     call        0000000140D15310
  0000000140DD627F: EB 13              jmp         0000000140DD6294
  0000000140DD6281: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140DD6286: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140DD6289: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140DD628E: FF 90 C0 01 00 00  call        qword ptr [rax+00000000000001C0h]
  0000000140DD6294: 48 81 C4 80 00 00  add         rsp,80h
                    00
  0000000140DD629B: 5B                 pop         rbx
  0000000140DD629C: C3                 ret
  0000000140DD629D: 0F 1F 00           nop         dword ptr [rax]
  0000000140DD62A0: 39 61 DD           cmp         dword ptr [rcx-23h],esp
  0000000140DD62A3: 00 73 61           add         byte ptr [rbx+61h],dh
  0000000140DD62A6: DD 00              fld         qword ptr [rax]
  0000000140DD62A8: A0 61 DD 00 EF 61  mov         al,byte ptr [2E00DD61EF00DD61h]
                    DD 00 2E
  0000000140DD62B1: 62
  0000000140DD62B2: DD 00              fld         qword ptr [rax]
  0000000140DD62B4: 66
  0000000140DD62B5: 62
  0000000140DD62B6: DD 00              fld         qword ptr [rax]
  0000000140DD62B8: 81
  0000000140DD62B9: 62
  0000000140DD62BA: DD 00              fld         qword ptr [rax]

  Summary

     1E02000 .text
