
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined1 FUN_140874630(longlong param_1,undefined8 param_2)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 *puVar3;
  double dVar4;
  undefined1 auStack_d8 [128];
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined1 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined1 uStack_20;
  undefined1 uStack_1f;
  undefined1 uStack_1e;
  undefined8 uStack_18;
  
  uStack_18 = 0xfffffffffffffffe;
  uStack_58 = 0;
  uStack_50 = 0;
  uStack_48 = 0;
  uStack_38 = 0;
  uStack_30 = 0;
  uStack_28 = 0;
  if (*(int *)(param_1 + 0x21c8) == 6) {
    cVar1 = FUN_140892370(param_1 + 0x2160);
    if (cVar1 == '\0') {
      uStack_1e = 0;
    }
    else {
      FUN_140898dc0(auStack_d8);
      FUN_140892d40(param_1 + 0x2160,auStack_d8);
      FUN_1408a0f30(auStack_d8,param_2,&uStack_58);
      uVar2 = func_0x000140868e50(param_1 + 0x2160);
      func_0x00014093c0e0(&uStack_58,uVar2,&uStack_38);
      uStack_40 = 0;
      dVar4 = (double)FUN_14093c500(&uStack_38);
      if (_UNK_142248190 < dVar4) {
        uStack_20 = 1;
        uStack_1f = 0;
        FUN_140890fb0(param_1,&uStack_38,&uStack_1f,&uStack_20);
        uStack_40 = 1;
      }
      puVar3 = (undefined8 *)func_0x000140868e50(param_1 + 0x2160);
      *(undefined8 *)(param_1 + 0x22d0) = *puVar3;
      *(undefined8 *)(param_1 + 0x22d8) = puVar3[1];
      cVar1 = FUN_14035b400(*(undefined8 *)(param_1 + 15000));
      if (cVar1 != '\0') {
        *(undefined8 *)(param_1 + 0x22e0) = puVar3[2];
      }
      uStack_1e = uStack_40;
      FUN_14089b180(auStack_d8);
    }
  }
  else {
    uStack_1e = 0;
  }
  return uStack_1e;
}

