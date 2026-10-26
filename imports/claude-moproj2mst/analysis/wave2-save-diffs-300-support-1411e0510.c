
longlong FUN_1411e0510(undefined8 param_1,undefined8 param_2)

{
  char cVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong lVar5;
  undefined1 auStack_78 [8];
  longlong lStack_70;
  undefined1 auStack_68 [8];
  longlong lStack_60;
  longlong lStack_58;
  undefined1 auStack_50 [8];
  longlong lStack_48;
  longlong lStack_40;
  undefined1 auStack_38 [8];
  undefined8 uStack_30;
  longlong lStack_28;
  longlong lStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  uStack_30 = 0xfffffffffffffffe;
  lStack_70 = 0;
  func_0x000140d4e9a0(param_2,auStack_78);
  uVar3 = func_0x0001405ff070(param_1);
  lVar4 = FUN_1405ef2b0(uVar3);
  if (lVar4 == 0) {
    lStack_70 = 0;
  }
  else {
    lVar5 = FUN_14005f770(lVar4);
    if (lVar5 == 0) {
      lStack_70 = 0;
    }
    else {
      uVar2 = func_0x000140d4ea70(param_2);
      FUN_140c7b1f0(lVar5,uVar2,0);
      func_0x0001401424c0(param_1);
      uVar3 = __0QLatin1String__QEAA_PEBD_Z(auStack_68,&UNK_142cd26e8);
      cVar1 = FUN_140142460(auStack_78,uVar3);
      if (cVar1 == '\0') {
        uVar3 = __0QLatin1String__QEAA_PEBD_Z(auStack_50,&UNK_142cd26f4);
        cVar1 = FUN_140142460(auStack_78,uVar3);
        if (cVar1 != '\0') {
          lStack_40 = FUN_14146eabc(0x50);
          if (lStack_40 == 0) {
            lStack_20 = 0;
          }
          else {
            lStack_20 = FUN_1411e2480(lStack_40,lVar4);
          }
          lStack_48 = lStack_20;
          lStack_70 = lStack_20;
        }
      }
      else {
        lStack_58 = FUN_14146eabc(0x58);
        if (lStack_58 == 0) {
          lStack_28 = 0;
        }
        else {
          lStack_28 = FUN_1411e3970(lStack_58,lVar4);
        }
        lStack_60 = lStack_28;
        lStack_70 = lStack_28;
      }
      if (lStack_70 == 0) {
        lStack_70 = FUN_1405ff0b0(param_1,param_2);
      }
      FUN_1406008e0(param_1);
      if (lStack_70 != 0) {
        uStack_18 = FUN_140d4e9d0(param_2,auStack_38);
        uStack_10 = uStack_18;
        _setWindowTitle_QWidget__QEAAXAEBVQString___Z(lStack_70,uStack_18);
        __1QString__QEAA_XZ(auStack_38);
      }
    }
  }
  return lStack_70;
}

