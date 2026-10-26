
undefined8 FUN_1410da8f0(undefined8 param_1,longlong param_2,uint param_3,undefined8 param_4)

{
  char cVar1;
  char cVar2;
  ulonglong uVar3;
  int iStack_24;
  
  for (iStack_24 = 0; iStack_24 < (int)param_3; iStack_24 = iStack_24 + 1) {
    *(undefined8 *)(param_2 + (longlong)iStack_24 * 8) = 0;
  }
  cVar1 = _atEnd_QTextStream__QEBA_NXZ(param_4);
  uVar3 = 0;
  if (cVar1 == '\0') {
    iStack_24 = 0;
    while( true ) {
      cVar2 = _atEnd_QTextStream__QEBA_NXZ(param_4);
      uVar3 = 0;
      if ((cVar2 != '\0') || (uVar3 = (ulonglong)param_3, (int)param_3 <= iStack_24)) break;
      __5QTextStream__QEAAAEAV0_AEAN_Z(param_4,param_2 + (longlong)iStack_24 * 8);
      iStack_24 = iStack_24 + 1;
    }
  }
  return CONCAT71((int7)(uVar3 >> 8),cVar1 == '\0');
}

