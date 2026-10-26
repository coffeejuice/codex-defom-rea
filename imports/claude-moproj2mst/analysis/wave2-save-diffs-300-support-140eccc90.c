
void FUN_140eccc90(longlong param_1,int param_2)

{
  if (param_2 == 0) {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd58),1);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd70),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd60),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd68),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd80),0);
  }
  else {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd58),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd70),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd60),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd68),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd80),1);
  }
  return;
}

