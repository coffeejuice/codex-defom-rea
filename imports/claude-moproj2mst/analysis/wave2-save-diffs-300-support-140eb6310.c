
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140eb6310(longlong *param_1,int *param_2)

{
  int iVar1;
  longlong lVar2;
  longlong lVar3;
  int iStack_1c;
  
  *param_2 = 1;
  lVar2 = (**(code **)(*param_1 + 0x198))(param_1);
  if (lVar2 != 0) {
    iVar1 = FUN_140c7c1a0(lVar2);
    for (iStack_1c = 0; iStack_1c < iVar1; iStack_1c = iStack_1c + 1) {
      lVar3 = FUN_140c7aed0(lVar2,iStack_1c + 1);
      if ((lVar3 != 0) &&
         (((*(double *)(lVar3 + 0x29e8) != _UNK_1421ad1d0 ||
           (*(double *)(lVar3 + 0x29f0) != _UNK_1421ad1d0)) ||
          (*(double *)(lVar3 + 0x29f8) != _UNK_1421ad1d0)))) {
        *param_2 = iStack_1c + 1;
        _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x838),iStack_1c);
        return;
      }
    }
  }
  return;
}

