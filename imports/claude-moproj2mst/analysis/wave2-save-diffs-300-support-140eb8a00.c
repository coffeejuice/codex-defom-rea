
void FUN_140eb8a00(longlong param_1,undefined4 param_2,undefined1 param_3)

{
  undefined8 uVar1;
  
  uVar1 = _item_QListWidget__QEBAPEAVQListWidgetItem__H_Z
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 8),param_2);
  _setCurrentItem_QListWidget__QEAAXPEAVQListWidgetItem___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 8),uVar1);
  uVar1 = _item_QListWidget__QEBAPEAVQListWidgetItem__H_Z
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 8),param_2);
  FUN_140ecb6f0(param_1,uVar1);
  _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 8),param_3);
  return;
}

