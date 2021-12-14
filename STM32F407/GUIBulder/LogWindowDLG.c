/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.26                          *
*        Compiled Aug  8 2014, 14:49:54                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
#include "FramewinDLG.h"
#include <string.h>
#include "linkqueue.h"
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0        (GUI_ID_USER + 0x00)
#define ID_MULTIEDIT_0        (GUI_ID_USER + 0x01)


// USER START (Optionally insert additional defines)
#define MAX_MAIN_LOG_LEN 2048
#define MAX_MAIN_LOG_ROW 20
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "LogWindow", ID_WINDOW_0, 0, 0, 470, 300, 0, 0x0, 0 },
  { MULTIEDIT_CreateIndirect, "LogMultiedit", ID_MULTIEDIT_0, 9, 11, 442, 275, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/

// USER START (Optionally insert additional static code)
// USER END

/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'LogMultiedit'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetText(hItem, "init param: 1LTKvNaux7CjrNHJ09C0y87Eo7/Rrbu3zfm4tKOsus7Ksbb41rmjv9a5zqrWucv51rmjrLTLzsTT1rrOtOajvw==");
    MULTIEDIT_SetFont(hItem, GUI_FONT_8X15B_ASCII);
    // USER START (Optionally insert additional code for further widget initialization)
    hItem = WM_GetDialogItem(pMsg->hWin, ID_MULTIEDIT_0);
    MULTIEDIT_SetReadOnly(hItem,1);//只读
    MULTIEDIT_SetWrapChar(hItem); //按字分行
    MULTIEDIT_SetAutoScrollV(hItem,1);
    MULTIEDIT_SetMaxNumChars(hItem,MAX_MAIN_LOG_LEN);

    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_MULTIEDIT_0: // Notifications sent by 'LogMultiedit'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_VALUE_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    // USER START (Optionally insert additional code for further Ids)
    // USER END
    }
    break;
  // USER START (Optionally insert additional message handling)
  // USER END
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateLogWindow
*/
WM_HWIN CreateLogWindow(void);
WM_HWIN CreateLogWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)

linkQueue *pLogQueue;

static void _setText(const char *log){
    WM_HWIN hItem = WM_GetDialogItem(hWin_LogFramewin, ID_MULTIEDIT_0);


    MULTIEDIT_SetText(hItem,log);
    MULTIEDIT_SetCursorOffset(hItem,(int)strlen(log));

}

/**
 * @brief listqueue 转换为字符串
 * 
 * @param pLQ 
 * @return char* 
 */
void _LQ_to_text(linkQueue *pLQ){
    char buf[MAX_MAIN_LOG_LEN];  
    memset(buf, 0, sizeof(buf));  
    queueData_t data;
    LinkNode *p = pLQ->front->next;

    int i = 0;
    for(i = 0; i < LQ_Length(pLQ); i++){
        data = p->data;
        strcat(buf,data);
        strcat(buf,"\n");
        p = p->next;
    }
    _setText(buf);
}

void mainLog_init(void){
    LQ_init(pLogQueue);
    mainLogPrint("init param: 1LTKvNaux7CjrNHJ09C0y87Eo7/Rrbu3zfm4tKOsus7Ksbb41rmjv9a5zqrWucv51rmjrLTLzsTT1rrOtOajvw==");
}

void mainLogPrint(const char *log) {
    queueData_t data;

    // 最多只保留 MAX_MAIN_LOG_ROW 行
    if(LQ_Length(pLogQueue)>MAX_MAIN_LOG_ROW){
        LQ_Dequeue(pLogQueue,&data);
        free(data);
    }
    data = (queueData_t)malloc(strlen(log)+1);
    LQ_Enqueue(pLogQueue,data);

    _LQ_to_text(pLogQueue);
}


#include <stdarg.h>  

void mainLogPrintf(const char *fmt, ...)  
{  
    va_list va;  
    char buf[1024];  
    memset(buf, 0, 1024);  
      
    va_start(va, fmt);  
    
    vsprintf(buf, fmt, va);
    
    va_end(va);  
      
    mainLogPrint( buf);  
}  

// USER END

/*************************** End of file ****************************/
