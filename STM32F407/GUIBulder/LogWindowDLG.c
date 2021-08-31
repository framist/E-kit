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
#include <stdio.h>
#include <string.h>
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
#define MAX_MAIN_LOG_LEN 300
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
    MULTIEDIT_SetText(hItem, "");
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
static void _setText(char *log){
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin_LogFramewin, ID_MULTIEDIT_0);
    char nowText[MAX_MAIN_LOG_LEN];
    MULTIEDIT_GetText(hItem,nowText,MAX_MAIN_LOG_LEN);
    strcpy(nowText, nowText + strlen(log) + 20); //有待更精确计算
    strcat(nowText,log);
    MULTIEDIT_SetText(hItem,nowText);
}

void mainLogPrint(char *log)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin_LogFramewin, ID_MULTIEDIT_0);

    int nowLen = MULTIEDIT_GetTextSize(hItem);
    int maxLen = MAX_MAIN_LOG_LEN;

    if (strlen(log) + nowLen >= maxLen)
    {
        _setText(log);
    }else{
        MULTIEDIT_SetCursorOffset(hItem,nowLen);
        MULTIEDIT_AddText(hItem,log);
    }

}


// USER END

/*************************** End of file ****************************/
