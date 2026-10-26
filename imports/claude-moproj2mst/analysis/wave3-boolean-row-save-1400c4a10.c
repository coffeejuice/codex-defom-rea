
ulonglong FUN_1400c4a10(undefined8 param_1,undefined8 param_2,int param_3,undefined8 param_4,
                       undefined4 param_5)

{
  char cVar1;
  char cVar2;
  char cVar3;
  char cVar4;
  longlong lVar5;
  longlong lVar6;
  undefined8 uVar7;
  undefined8 in_stack_ffffffffffffff38;
  undefined4 uVar9;
  ulonglong uVar8;
  undefined1 auStack_80 [8];
  undefined8 uStack_78;
  ulonglong uStack_70;
  longlong lStack_68;
  undefined4 uStack_60;
  undefined8 uStack_58;
  undefined1 uStack_50;
  undefined8 uStack_48;
  undefined1 uStack_40;
  undefined1 uStack_3f;
  ulonglong uStack_38;
  undefined1 auStack_30 [8];
  undefined1 *puStack_28;
  ulonglong uStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  uVar9 = (undefined4)((ulonglong)in_stack_ffffffffffffff38 >> 0x20);
  uStack_18 = 0xfffffffffffffffe;
  uStack_70 = 0;
  uStack_58 = 0;
  uStack_78 = 0;
  lVar5 = FUN_1400c3c60(param_2,param_3 + -1);
  if (lVar5 == 0) {
    uStack_38 = 0;
  }
  else {
    lVar6 = FUN_1400c0530(lVar5);
    if (lVar6 == 0) {
      uStack_38 = 0;
    }
    else {
      uStack_60 = FUN_1400c06b0(lVar5);
      FUN_1400c0580(lVar5,auStack_80);
      lStack_68 = FUN_1400c04e0(lVar5);
      if (lStack_68 == 0) {
        uStack_38 = 0;
        __1QString__QEAA_XZ(auStack_80);
      }
      else {
        puStack_28 = auStack_30;
        uStack_10 = __0QString__QEAA_AEBV0__Z(puStack_28,auStack_80);
        uVar8 = CONCAT44(uVar9,uStack_60);
        uStack_70 = FUN_1400c5d40(param_1,lStack_68,param_3,uStack_10,uVar8,param_4,param_5);
        FUN_140834d50(lStack_68);
        cVar1 = FUN_140834c80(lStack_68);
        cVar2 = FUN_14084d4a0(lStack_68);
        cVar3 = FUN_140834f40(lStack_68);
        cVar4 = func_0x0001400c0740(lVar5);
        if (cVar4 != '\0') {
          uStack_50 = func_0x0001400c3240(lVar5);
          uVar8 = CONCAT71((int7)(uVar8 >> 8),uStack_50);
          FUN_1400c6030(param_1,lStack_68,param_3,uStack_70,uVar8);
        }
        cVar4 = func_0x0001400c0870(lVar5);
        if (cVar4 != '\0') {
          FUN_1400c6260(param_1,lStack_68,param_3,uStack_70);
        }
        cVar4 = func_0x0001400c07d0(lVar5);
        if ((cVar4 != '\0') || (cVar1 != '\0')) {
          uStack_48 = FUN_1400c6300(param_1,lStack_68,param_3,uStack_70);
          cVar4 = func_0x0001400c0820(lVar5);
          if ((cVar4 != '\0') || (cVar3 != '\0')) {
            FUN_1400c64a0(param_1,lStack_68,param_3,uStack_48);
            func_0x0001400eb5e0(uStack_48,1);
          }
        }
        cVar3 = func_0x0001400c3010(lVar5);
        if ((cVar3 != '\0') || (cVar1 != '\0')) {
          uVar7 = func_0x0001400c35b0(lStack_68);
          uVar8 = uStack_70;
          FUN_1400c67c0(param_1,lVar6,uVar7,param_3,uStack_70);
        }
        cVar3 = func_0x0001400c3060(lVar5);
        if ((cVar3 != '\0') || (cVar1 != '\0')) {
          FUN_1400c6ae0(param_1,lStack_68,param_3,uStack_70);
        }
        cVar1 = func_0x0001400c30b0(lVar5);
        if ((cVar1 != '\0') || (cVar2 != '\0')) {
          uStack_40 = func_0x0001400c3100(lVar5);
          uVar8 = CONCAT71((int7)(uVar8 >> 8),uStack_40);
          FUN_1400c6bb0(param_1,lStack_68,param_3,uStack_70,uVar8);
        }
        cVar1 = func_0x0001400c3150(lVar5);
        if (cVar1 != '\0') {
          FUN_1400c6dd0(param_1,lStack_68,param_3,uStack_70);
        }
        cVar1 = func_0x0001400c31a0(lVar5);
        if (cVar1 != '\0') {
          uStack_3f = 0;
          FUN_1400c6e70(param_1,lStack_68,param_3,uStack_70,uVar8 & 0xffffffffffffff00);
        }
        uStack_20 = uStack_70;
        __1QString__QEAA_XZ(auStack_80);
        uStack_38 = uStack_20;
      }
    }
  }
  return uStack_38;
}

