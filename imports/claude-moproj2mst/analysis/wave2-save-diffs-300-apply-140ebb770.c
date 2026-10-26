
void FUN_140ebb770(longlong param_1,longlong param_2)

{
  char cVar1;
  undefined4 uVar2;
  int iVar3;
  longlong lVar4;
  longlong *plVar5;
  undefined1 auStack_28 [8];
  undefined8 uStack_20;
  undefined8 uStack_18;
  undefined8 uStack_10;
  
  uStack_20 = 0xfffffffffffffffe;
  uVar2 = _value_QSpinBox__QEBAHXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1718));
  *(undefined4 *)(param_2 + 0x160) = uVar2;
  lVar4 = FUN_140130af0(param_1);
  iVar3 = func_0x0001402cb7a0(0x143d766b8);
  if ((iVar3 == 5) && (lVar4 != 0)) {
    cVar1 = FUN_14005f230(lVar4);
    if (cVar1 == '\0') {
      plVar5 = (longlong *)FUN_140054e90(lVar4);
      if ((plVar5 == (longlong *)0x0) ||
         (cVar1 = (**(code **)(*plVar5 + 0x200))(plVar5), cVar1 == '\0')) {
        func_0x00014005a2c0(lVar4,0);
      }
      else {
        cVar1 = _isChecked_QAbstractButton__QEBA_NXZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x16f0));
        func_0x00014005a2c0(lVar4,cVar1);
        if (cVar1 != '\0') {
          func_0x00014005a350(lVar4,uVar2);
        }
      }
    }
    else {
      func_0x00014005a2c0(lVar4,1);
      func_0x00014005a350(lVar4,uVar2);
    }
  }
  uVar2 = _value_QSpinBox__QEBAHXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1740));
  *(undefined4 *)(param_2 + 0x16c) = uVar2;
  uVar2 = _value_QSpinBox__QEBAHXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1750));
  *(undefined4 *)(param_2 + 0x164) = uVar2;
  uStack_18 = _currentText_QComboBox__QEBA_AVQString__XZ
                        (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x16c8),auStack_28);
  uStack_10 = uStack_18;
  uVar2 = FUN_140ec9980(param_1,uStack_18);
  *(undefined4 *)(param_2 + 0x168) = uVar2;
  __1QString__QEAA_XZ(auStack_28);
  return;
}

