#include <windows.h>
#include <stdlib.h>
#include <stdint.h>
int g_fail=5;
unsigned char g_main[0x2000];
struct H {int ref,capacity,length,pad;const wchar_t *data;unsigned short flags;} g_header;
const wchar_t g_path[]=L"C:\\PROBLEM\\p213a\\Gear_blank.moproj";
int g_regcheck=0;
extern uintptr_t synthetic_open(void);
int main(int argc,char **argv){
 SetErrorMode(SEM_FAILCRITICALERRORS|SEM_NOGPFAULTERRORBOX|SEM_NOOPENFILEERRORBOX);
 if(argc!=2)return 73;
 g_fail=atoi(argv[1]);
 if(g_fail<0||g_fail>7)return 74;
 if(g_fail==6)return 42;
 if(g_fail==7)for(;;)Sleep(100);
 g_header.ref=1;g_header.capacity=34;g_header.length=34;g_header.data=g_path;
 *(void**)(g_main+0x18d8)=&g_header;
 volatile uintptr_t value=synthetic_open();
 if(g_regcheck!=1)return 77;
 if((g_fail==5&&value!=0x100)||(g_fail<5&&value!=0))return 75;
 for(;;) Sleep(100);
 return 76;
}
