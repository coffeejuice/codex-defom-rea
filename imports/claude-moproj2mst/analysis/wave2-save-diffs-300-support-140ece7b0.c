
void FUN_140ece7b0(longlong *param_1,int param_2)

{
  char cVar1;
  undefined8 uVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined4 uStack_58;
  undefined4 uStack_54;
  undefined8 uStack_50;
  longlong lStack_48;
  longlong lStack_40;
  undefined4 auStack_38 [2];
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  longlong lStack_18;
  
  FUN_140eb6210(param_1,&uStack_54,&uStack_58);
  uVar2 = (**(code **)(*param_1 + 0x198))(param_1);
  uStack_50 = FUN_140c7ae80(uVar2,uStack_54);
  uVar3 = FUN_140c7ae80(uVar2,uStack_58);
  lStack_48 = FUN_140c82e40(uVar2,uStack_50);
  lVar4 = FUN_140c82e40(uVar2,uVar3);
  *(int *)((longlong)param_1 + 0xbc) = param_2;
  if (param_2 == 1) {
    FUN_140ecd430(param_1,1);
    FUN_140ecd2f0(param_1,1);
    FUN_140ece670(param_1,1,1);
    FUN_140ece670(param_1,2,0);
    if (lStack_48 != 0) {
      FUN_1411f4990(lStack_48);
    }
    if (lVar4 != 0) {
      FUN_1411f4990(lVar4);
    }
    FUN_140ebf6f0(param_1,0);
  }
  else if (param_2 == 2) {
    FUN_140ecd2f0(param_1,0);
    FUN_140ecd430(param_1,1);
    uVar2 = (**(code **)(*param_1 + 0x1b0))(param_1);
    FUN_14120b2c0(uVar2);
    uVar2 = (**(code **)(*param_1 + 0x198))(param_1);
    FUN_140c83ca0(uVar2,0,0,1);
    if (lStack_48 != 0) {
      FUN_1411f49e0(lStack_48);
    }
    if (lVar4 != 0) {
      FUN_1411f49e0(lVar4);
    }
    auStack_38[0] = 0;
    FUN_140eb62e0(param_1,auStack_38);
    uVar2 = (**(code **)(*param_1 + 0x198))(param_1);
    lStack_40 = FUN_140c7ae80(uVar2,auStack_38[0]);
    if ((lStack_40 != 0) && (lStack_18 = lStack_40 + 0x2160, lStack_18 != 0)) {
      uStack_30 = *(undefined8 *)(lStack_40 + 0x29e8);
      uStack_28 = *(undefined8 *)(lStack_40 + 0x29f0);
      cVar1 = FUN_140130ee0(param_1);
      if (cVar1 != '\0') {
        uStack_20 = *(undefined8 *)(lStack_18 + 0x898);
      }
      FUN_140ebec50(param_1,auStack_38[0],&uStack_30);
    }
  }
  else {
    FUN_140ecd2f0(param_1,0);
    FUN_140ecd430(param_1,1);
    uVar2 = (**(code **)(*param_1 + 0x1b0))(param_1);
    FUN_14120b2c0(uVar2);
    uVar2 = (**(code **)(*param_1 + 0x198))(param_1);
    FUN_140c83ca0(uVar2,0,0,1);
    if (lStack_48 != 0) {
      FUN_1411f49e0(lStack_48);
    }
    if (lVar4 != 0) {
      FUN_1411f49e0(lVar4);
    }
    FUN_140ecd430(param_1,0);
    FUN_140ebf6f0(param_1,0);
  }
  return;
}

