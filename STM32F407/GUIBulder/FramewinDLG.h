#ifndef _FRAMEWINDLG_H
#define _FRAMEWINDLG_H
#include "sys.h"
#include "WM.h"

WM_HWIN CreatemainFramewin(void);
void mainLog_init(void);
void mainLogPrint(const char *log);
void mainLogPrintf(const char *fmt, ...);

extern WM_HWIN hWin_oscilloscopeFramewin;
extern WM_HWIN hWin_SingalWindow;
extern WM_HWIN hWin_AFCFramewin;
extern WM_HWIN hWin_LogFramewin;

#endif

/*************************** End of file ****************************/
