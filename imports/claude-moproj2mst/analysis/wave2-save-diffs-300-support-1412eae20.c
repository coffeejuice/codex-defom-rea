
void FUN_1412eae20(undefined8 param_1)

{
  longlong lVar1;
  longlong lVar2;
  int iVar3;
  int iVar4;
  longlong lVar5;
  longlong lVar6;
  longlong lVar7;
  undefined8 unaff_RBX;
  undefined8 unaff_RBP;
  longlong lStackX_8;
  longlong lStackX_10;
  longlong lStackX_18;
  undefined4 auStackX_20 [2];
  undefined *puStack_20;
  
  puStack_20 = &UNK_1412eae2f;
  lVar5 = FUN_1417f5860();
  lVar5 = -lVar5;
  *(undefined8 *)(&stack0x00000050 + lVar5) = 0xfffffffffffffffe;
  *(undefined8 *)(&stack0x00001320 + lVar5) = unaff_RBX;
  *(undefined8 *)(&stack0x00001328 + lVar5) = unaff_RBP;
  *(ulonglong *)(&stack0x000012e8 + lVar5) =
       uRam0000000143b5e030 ^ (ulonglong)(&stack0xffffffffffffffe8 + lVar5);
  *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eae70;
  FUN_1412e46a0(&stack0x00000058 + lVar5,0,1);
  *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eae7d;
  func_0x0001412f7e50(&stack0x00000058 + lVar5,0);
  *(undefined4 *)((longlong)auStackX_20 + lVar5) = 3;
  *(undefined4 *)((longlong)auStackX_20 + lVar5 + 4) = 0x68;
  *(undefined4 *)(&stack0x00000030 + lVar5) = 1;
  *(undefined4 *)(&stack0x00000028 + lVar5) = 0;
  *(undefined4 *)(&stack0x00000034 + lVar5) = 0;
  *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eaeb1;
  func_0x0001412f2200(&stack0x00000058 + lVar5,(longlong)auStackX_20 + lVar5);
  *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eaec0;
  iVar3 = func_0x0001412f21e0(&stack0x00000058 + lVar5,&stack0x00000038 + lVar5);
  do {
    if (iVar3 != 0x18) {
      *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eafcc;
      FUN_1412e43e0(&stack0x00000058 + lVar5);
code_r0x0001412eafcc:
      *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eafdc;
      func_0x00014146df10(*(ulonglong *)(&stack0x000012e8 + lVar5) ^
                          (ulonglong)(&stack0xffffffffffffffe8 + lVar5));
      return;
    }
    if (((*(int *)(&stack0x00000038 + lVar5) != 2) || (*(int *)(&stack0x0000003c + lVar5) != 0x69))
       || (iVar3 = *(int *)(&stack0x0000004c + lVar5), iVar3 < 1)) {
code_r0x0001412eafb6:
      *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eafc0;
      FUN_1412e43e0(&stack0x00000058 + lVar5);
      goto code_r0x0001412eafcc;
    }
    *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eaefd;
    lVar6 = func_0x00014146c25c((longlong)(iVar3 + 10));
    if (lVar6 == 0) goto code_r0x0001412eafb6;
    *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eaf19;
    iVar4 = func_0x0001412f3600(&stack0x00000058 + lVar5,lVar6,iVar3);
    if (iVar4 != iVar3) {
      *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eafb5;
      func_0x00014146d898(lVar6);
      goto code_r0x0001412eafb6;
    }
    *(undefined8 *)((longlong)&lStackX_18 + lVar5) = 0;
    *(undefined8 *)((longlong)&lStackX_10 + lVar5) = 0;
    *(undefined8 *)((longlong)&lStackX_8 + lVar5) = 0;
    *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eaf47;
    FUN_1412e7080((longlong)&lStackX_18 + lVar5,(longlong)&lStackX_10 + lVar5,
                  (longlong)&lStackX_8 + lVar5,lVar6);
    lVar7 = *(longlong *)((longlong)&lStackX_18 + lVar5);
    if (((lVar7 != 0) && (lVar1 = *(longlong *)((longlong)&lStackX_10 + lVar5), lVar1 != 0)) &&
       (lVar2 = *(longlong *)((longlong)&lStackX_8 + lVar5), lVar2 != 0)) {
      *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eaf73;
      func_0x0001412e75b0(param_1,lVar7,lVar1,lVar2);
      *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eaf7b;
      func_0x0001414dfc68(lVar7);
      *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eaf83;
      func_0x00014146d898(lVar1);
      *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eaf8b;
      func_0x00014146d898(lVar2);
    }
    *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eaf93;
    func_0x00014146d898(lVar6);
    *(undefined **)((longlong)&puStack_20 + lVar5) = &UNK_1412eafa2;
    iVar3 = func_0x0001412f21e0(&stack0x00000058 + lVar5,&stack0x00000038 + lVar5);
  } while( true );
}

