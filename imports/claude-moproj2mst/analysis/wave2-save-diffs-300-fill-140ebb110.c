
void FUN_140ebb110(longlong param_1,longlong param_2)

{
  char cVar1;
  undefined1 uVar2;
  int iVar3;
  longlong lVar4;
  longlong *plVar5;
  int iStack_28;
  
  iStack_28 = *(int *)(param_2 + 0x160);
  lVar4 = FUN_140130af0(param_1);
  iVar3 = func_0x0001402cb7a0(0x143d766b8);
  if (iVar3 == 5) {
    if ((lVar4 == 0) || (cVar1 = FUN_14005f230(lVar4), cVar1 != '\0')) {
      _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x16f0));
      _setCurrentIndex_QStackedWidget__QEAAXH_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1700),0);
    }
    else {
      plVar5 = (longlong *)FUN_140054e90(lVar4);
      if ((plVar5 == (longlong *)0x0) ||
         (cVar1 = (**(code **)(*plVar5 + 0x200))(plVar5), cVar1 == '\0')) {
        _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x16f0));
        _setCurrentIndex_QStackedWidget__QEAAXH_Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1700),1);
        _blockSignals_QObject__QEAA_N_N_Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x16f0),1);
        _setChecked_QAbstractButton__QEAAX_N_Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x16f0),0);
        _blockSignals_QObject__QEAA_N_N_Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x16f0),0);
      }
      else {
        _show_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x16f0));
        cVar1 = func_0x00014005a2a0(lVar4);
        if (cVar1 == '\0') {
          _setCurrentIndex_QStackedWidget__QEAAXH_Z
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1700),1);
        }
        else {
          _setCurrentIndex_QStackedWidget__QEAAXH_Z
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1700),0);
        }
        _blockSignals_QObject__QEAA_N_N_Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x16f0),1);
        uVar2 = func_0x00014005a2a0(lVar4);
        _setChecked_QAbstractButton__QEAAX_N_Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x16f0),uVar2);
        _blockSignals_QObject__QEAA_N_N_Z
                  (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x16f0),0);
        cVar1 = func_0x00014005a2a0(lVar4);
        if (cVar1 != '\0') {
          iStack_28 = func_0x00014005a2e0(lVar4);
        }
      }
    }
  }
  if (0 < iStack_28) {
    iStack_28 = -(*(int *)(param_2 + 0x160) + 1);
  }
  _setValue_QSpinBox__QEAAXH_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1718),iStack_28);
  _setValue_QSpinBox__QEAAXH_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1740),
             *(undefined4 *)(param_2 + 0x16c));
  _setValue_QSpinBox__QEAAXH_Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1750),
             *(undefined4 *)(param_2 + 0x164));
  FUN_140ebb3f0(param_1);
  return;
}

