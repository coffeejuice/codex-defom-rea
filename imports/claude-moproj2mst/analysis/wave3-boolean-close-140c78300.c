
/* WARNING: Removing unreachable block (ram,0x000140c78493) */
/* WARNING: Removing unreachable block (ram,0x000140c783e0) */
/* WARNING: Removing unreachable block (ram,0x000140c78396) */
/* WARNING: Removing unreachable block (ram,0x000140c78353) */
/* WARNING: Removing unreachable block (ram,0x000140c78430) */

void FUN_140c78300(longlong param_1,undefined8 param_2,undefined8 param_3,undefined8 param_4)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  undefined8 *puVar4;
  longlong *plVar5;
  undefined1 auStack_80 [32];
  undefined1 *puStack_60;
  longlong *plStack_58;
  longlong *plStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  int *piStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  uStack_48 = 0xfffffffffffffffe;
  FUN_14006daf0(auStack_80,param_1 + 0x18,param_3,param_4,0);
  puStack_60 = auStack_80;
  while( true ) {
    uStack_38 = 0;
    uVar2 = func_0x000140dfc380(puStack_60,0);
    cVar1 = FUN_140dfc080(uVar2);
    if (cVar1 == '\0') break;
    uStack_30 = 0;
    lVar3 = func_0x000140dfc380(puStack_60,0);
    puVar4 = (undefined8 *)FUN_14064f400(lVar3 + 0x10);
    plVar5 = (longlong *)*puVar4;
    while( true ) {
      uStack_18 = 0;
      lVar3 = func_0x000140dfc380(puStack_60,0);
      if (*(int *)(lVar3 + 8) == 0) break;
      plStack_58 = plVar5;
      plStack_50 = plVar5;
      if (plVar5 == (longlong *)0x0) {
        uStack_10 = 0;
      }
      else {
        uStack_10 = (**(code **)(*plVar5 + 0x18))(plVar5,1);
      }
      uStack_28 = 0;
      lVar3 = func_0x000140dfc380(puStack_60,0);
      piStack_20 = (int *)(lVar3 + 8);
      *piStack_20 = *piStack_20 + -1;
    }
    uStack_40 = 0;
    lVar3 = func_0x000140dfc380(puStack_60,0);
    func_0x000140de9800(lVar3 + 0x10);
  }
  FUN_140c51520(auStack_80);
  FUN_14020db90(param_1 + 0x18);
  FUN_140cbc920(param_1 + 0x20,0);
  return;
}

