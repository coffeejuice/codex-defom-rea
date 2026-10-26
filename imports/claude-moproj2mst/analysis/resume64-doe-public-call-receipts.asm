# Selected call/argument/continuation window
                    00 00
  0000000140099753: E8 78 B2 00 00     call        00000001400A49D0
  0000000140099758: 48 89 84 24 18 04  mov         qword ptr [rsp+0000000000000418h],rax
                    00 00
  0000000140099760: 48 8B 94 24 18 04  mov         rdx,qword ptr [rsp+0000000000000418h]
                    00 00
  0000000140099768: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140099770: E8 8B 9A 03 00     call        00000001400D3200
  0000000140099775: 48 8D 84 24 98 01  lea         rax,[rsp+0000000000000198h]
                    00 00
  000000014009977D: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  0000000140099785: 48 8B 94 24 A0 01  mov         rdx,qword ptr [rsp+00000000000001A0h]
                    00 00

# Selected call/argument/continuation window
                    00 00
  0000000140099795: E8 36 B2 00 00     call        00000001400A49D0
  000000014009979A: 48 89 84 24 20 04  mov         qword ptr [rsp+0000000000000420h],rax
                    00 00
  00000001400997A2: 48 8B 94 24 20 04  mov         rdx,qword ptr [rsp+0000000000000420h]
                    00 00
  00000001400997AA: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001400997B2: E8 E9 DD 03 00     call        00000001400D75A0
  00000001400997B7: 48 8D 15 52 6A 0A  lea         rdx,[0000000142140210h]
                    02
  00000001400997BE: 48 8D 8C 24 A8 01  lea         rcx,[rsp+00000000000001A8h]
                    00 00
  00000001400997C6: FF 15 B4 AC D6 01  call        qword ptr [0000000141E04480h]
  00000001400997CC: 48 89 84 24 28 04  mov         qword ptr [rsp+0000000000000428h],rax

# Selected call/argument/continuation window
  0000000140099A57: FF 15 7B BF D6 01  call        qword ptr [0000000141E059D8h]
  0000000140099A5D: 48 89 84 24 B8 04  mov         qword ptr [rsp+00000000000004B8h],rax
                    00 00
  0000000140099A65: 4C 8B 84 24 B8 04  mov         r8,qword ptr [rsp+00000000000004B8h]
                    00 00
  0000000140099A6D: 33 D2              xor         edx,edx
  0000000140099A6F: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140099A77: E8 14 DD 03 00     call        00000001400D7790
  0000000140099A7C: 48 8D 15 9D 67 0A  lea         rdx,[0000000142140220h]
                    02
  0000000140099A83: 48 8D 8C 24 18 02  lea         rcx,[rsp+0000000000000218h]
                    00 00
  0000000140099A8B: FF 15 EF A9 D6 01  call        qword ptr [0000000141E04480h]
  0000000140099A91: 48 89 84 24 C0 04  mov         qword ptr [rsp+00000000000004C0h],rax

# Selected call/argument/continuation window
  0000000140099D59: FF 15 79 BC D6 01  call        qword ptr [0000000141E059D8h]
  0000000140099D5F: 48 89 84 24 60 05  mov         qword ptr [rsp+0000000000000560h],rax
                    00 00
  0000000140099D67: 4C 8B 84 24 60 05  mov         r8,qword ptr [rsp+0000000000000560h]
                    00 00
  0000000140099D6F: BA 01 00 00 00     mov         edx,1
  0000000140099D74: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140099D7C: E8 0F DA 03 00     call        00000001400D7790
  0000000140099D81: 48 8D 15 A8 64 0A  lea         rdx,[0000000142140230h]
                    02
  0000000140099D88: 48 8D 8C 24 90 02  lea         rcx,[rsp+0000000000000290h]
                    00 00
  0000000140099D90: FF 15 EA A6 D6 01  call        qword ptr [0000000141E04480h]
  0000000140099D96: 48 89 84 24 68 05  mov         qword ptr [rsp+0000000000000568h],rax

# Selected call/argument/continuation window
  000000014009A05E: FF 15 74 B9 D6 01  call        qword ptr [0000000141E059D8h]
  000000014009A064: 48 89 84 24 08 06  mov         qword ptr [rsp+0000000000000608h],rax
                    00 00
  000000014009A06C: 4C 8B 84 24 08 06  mov         r8,qword ptr [rsp+0000000000000608h]
                    00 00
  000000014009A074: BA 02 00 00 00     mov         edx,2
  000000014009A079: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014009A081: E8 0A D7 03 00     call        00000001400D7790
  000000014009A086: 90                 nop
  000000014009A087: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014009A08F: FF 15 4B B9 D6 01  call        qword ptr [0000000141E059E0h]
  000000014009A095: E9 A8 03 00 00     jmp         000000014009A442
  000000014009A09A: 0F B6 84 24 C8 06  movzx       eax,byte ptr [rsp+00000000000006C8h]

