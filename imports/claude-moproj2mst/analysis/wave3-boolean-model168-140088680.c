
/* WARNING: Removing unreachable block (ram,0x000140088827) */
/* WARNING: Removing unreachable block (ram,0x000140088782) */
/* WARNING: Removing unreachable block (ram,0x00014008873f) */
/* WARNING: Removing unreachable block (ram,0x000140088703) */
/* WARNING: Removing unreachable block (ram,0x0001400887cb) */

void FUN_140088680(longlong param_1)

{
  char cVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined8 *puVar5;
  undefined1 auStack_88 [8];
  undefined1 *puStack_80;
  undefined1 auStack_78 [32];
  longlong *plStack_58;
  longlong lStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  int *piStack_20;
  undefined8 uStack_18;
  
  uStack_48 = 0xfffffffffffffffe;
  if (*(longlong *)(param_1 + 8) != 0) {
    FUN_140084ad0(param_1 + -0x10,auStack_88);
    cVar1 = FUN_140307410(auStack_88);
    if (cVar1 == '\0') {
      FUN_14006daf0(auStack_78,auStack_88);
      puStack_80 = auStack_78;
      while( true ) {
        uStack_38 = 0;
        uVar3 = func_0x000140dfc380(puStack_80,0);
        cVar1 = FUN_140dfc080(uVar3);
        if (cVar1 == '\0') break;
        uStack_30 = 0;
        lVar4 = func_0x000140dfc380(puStack_80,0);
        puVar5 = (undefined8 *)FUN_14064f400(lVar4 + 0x10);
        plStack_58 = (longlong *)*puVar5;
        while( true ) {
          uStack_18 = 0;
          lVar4 = func_0x000140dfc380(puStack_80,0);
          if (*(int *)(lVar4 + 8) == 0) break;
          lStack_50 = FUN_1400871c0(param_1 + -0x10,plStack_58);
          if ((lStack_50 != 0) && (cVar1 = func_0x000140064740(lStack_50), cVar1 != '\0')) {
            uVar2 = (**(code **)(*plStack_58 + 0x28))(plStack_58);
            FUN_140086fe0(param_1 + -0x10,uVar2,1);
          }
          uStack_28 = 0;
          lVar4 = func_0x000140dfc380(puStack_80,0);
          piStack_20 = (int *)(lVar4 + 8);
          *piStack_20 = *piStack_20 + -1;
        }
        uStack_40 = 0;
        lVar4 = func_0x000140dfc380(puStack_80,0);
        func_0x000140de9800(lVar4 + 0x10);
      }
      FUN_140c51520(auStack_78);
    }
    FUN_1402081a0(auStack_88);
  }
  return;
}

