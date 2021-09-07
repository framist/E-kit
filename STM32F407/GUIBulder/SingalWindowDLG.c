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
#include "wave_output.h"
#include "main.h"
#include <string.h>
#include <math.h>
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0    (GUI_ID_USER + 0x00)
#define ID_SPINBOX_0    (GUI_ID_USER + 0x01)
#define ID_RADIO_0    (GUI_ID_USER + 0x02)
#define ID_BUTTON_0    (GUI_ID_USER + 0x03)
#define ID_GRAPH_0    (GUI_ID_USER + 0x04)
#define ID_SPINBOX_1    (GUI_ID_USER + 0x05)
#define ID_SPINBOX_2    (GUI_ID_USER + 0x06)
#define ID_SPINBOX_3    (GUI_ID_USER + 0x07)
#define ID_LISTBOX_0    (GUI_ID_USER + 0x08)
#define ID_RADIO_1    (GUI_ID_USER + 0x09)
#define ID_HEADER_0    (GUI_ID_USER + 0x0A)
#define ID_SLIDER_0    (GUI_ID_USER + 0x0B)
#define ID_SLIDER_1    (GUI_ID_USER + 0x0C)
#define ID_SLIDER_2    (GUI_ID_USER + 0x0D)
#define ID_SLIDER_3    (GUI_ID_USER + 0x0E)
#define ID_TEXT_0    (GUI_ID_USER + 0x0F)


// USER START (Optionally insert additional defines)
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
  { WINDOW_CreateIndirect, "SingalWindow", ID_WINDOW_0, 0, 0, 480, 300, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox_f", ID_SPINBOX_0, 280, 30, 100, 25, 0, 0x0, 0 },
  { RADIO_CreateIndirect, "Radio_WaveForm", ID_RADIO_0, 5, 30, 110, 162, 0, 0x1408, 0 },
  { BUTTON_CreateIndirect, "Button_OUTPUT", ID_BUTTON_0, 400, 181, 60, 60, 0, 0x0, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 5, 192, 100, 100, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox_Vpp", ID_SPINBOX_1, 279, 85, 100, 25, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox_offset", ID_SPINBOX_2, 280, 141, 100, 25, 0, 0x0, 0 },
  { SPINBOX_CreateIndirect, "Spinbox_duty", ID_SPINBOX_3, 280, 194, 100, 25, 0, 0x0, 0 },
  { LISTBOX_CreateIndirect, "Listbox_step", ID_LISTBOX_0, 400, 35, 60, 121, 0, 0x0, 0 },
  { RADIO_CreateIndirect, "Radio_shortcut", ID_RADIO_1, 113, 34, 158, 225, 0, 0x3704, 0 },
  { HEADER_CreateIndirect, "Header", ID_HEADER_0, -1, 0, 480, 20, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "Slider_offset", ID_SLIDER_0, 130, 170, 250, 25, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "Slider_Vpp", ID_SLIDER_1, 130, 115, 250, 25, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "Slider_f", ID_SLIDER_2, 130, 60, 250, 25, 0, 0x0, 0 },
  { SLIDER_CreateIndirect, "Slider_duty", ID_SLIDER_3, 130, 225, 250, 25, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "LogText", ID_TEXT_0, 114, 255, 344, 37, 0, 0x64, 0 },
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
//log 相关
#define LOGS_MAX_LENGTH 1000
static char Logs[LOGS_MAX_LENGTH] = "* framist * --- all rights reserved ---\n"
                                    "Logs:==================================";

static void LogPrint(char *log,WM_HWIN  hWin)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(hWin, ID_TEXT_0);
    if (strlen(log) + strlen(Logs) >= LOGS_MAX_LENGTH)
    {
        strcpy(Logs, Logs + strlen(log) + 10); //有待更精确计算
    }
    //strcat(Logs,"\n");
    strcat(Logs, log);
    TEXT_SetText(hItem, Logs);
}
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
  char stemp[100] = "";
  // USER END

  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    //
    // Initialization of 'Spinbox_f'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
    SPINBOX_SetFont(hItem, GUI_FONT_8X16);
    //
    // Initialization of 'Radio_WaveForm'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
    RADIO_SetText(hItem, "OFF     .  .  .  .", 0);
    RADIO_SetText(hItem, "SIN    ~~~~", 1);
    RADIO_SetText(hItem, "TRI    VVVVV", 2);
    RADIO_SetText(hItem, "SQU   _||_|", 3);
    RADIO_SetText(hItem, "SAW1 /|/|/", 4);
    RADIO_SetText(hItem, "SAW2 |\\|\\|", 5);
    RADIO_SetText(hItem, "RAD    ?????", 6);
    RADIO_SetFont(hItem, GUI_FONT_13B_ASCII);
    RADIO_SetText(hItem, "DC       ------", 7);
    //
    // Initialization of 'Button_OUTPUT'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetText(hItem, "OUT\nPUT");
    BUTTON_SetFont(hItem, GUI_FONT_16B_ASCII);
    //
    // Initialization of 'Spinbox_Vpp'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_1);
    SPINBOX_SetFont(hItem, GUI_FONT_8X16);
    //
    // Initialization of 'Spinbox_offset'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_2);
    SPINBOX_SetFont(hItem, GUI_FONT_8X16);
    //
    // Initialization of 'Spinbox_duty'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_3);
    SPINBOX_SetFont(hItem, GUI_FONT_8X16);
    //
    // Initialization of 'Listbox_step'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTBOX_0);
    LISTBOX_AddString(hItem, "1");
    LISTBOX_AddString(hItem, "5");
    LISTBOX_AddString(hItem, "10");
    LISTBOX_SetFont(hItem, GUI_FONT_16_1);
    LISTBOX_AddString(hItem, "50");
    LISTBOX_AddString(hItem, "100");
    LISTBOX_AddString(hItem, "500");
    LISTBOX_AddString(hItem, "1000");
    //
    // Initialization of 'Radio_shortcut'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_1);
    RADIO_SetText(hItem, "Frequency (+Hz)", 0);
    RADIO_SetText(hItem, "Vpp (+mV)", 1);
    RADIO_SetText(hItem, "Offset (+-mV)", 2);
    RADIO_SetText(hItem, "Duty (if SQU) (+%)", 3);
    RADIO_SetFont(hItem, GUI_FONT_16B_ASCII);
    //
    // Initialization of 'Header'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_HEADER_0);
    HEADER_AddItem(hItem, 80, "wave form", 14);
    HEADER_AddItem(hItem, 75, "shortcut", 14);
    HEADER_AddItem(hItem, 240, "parameter setting", 14);
    HEADER_AddItem(hItem, 81, "step length", 14);
    //
    // Initialization of 'LogText'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetText(hItem, "Log:");
    // USER START (Optionally insert additional code for further widget initialization)

    // SPINBOX 初始化
    SPINBOX_SetEdge(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0),SPINBOX_EDGE_CENTER);
    SPINBOX_SetEdge(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_1),SPINBOX_EDGE_CENTER);
    SPINBOX_SetEdge(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_2),SPINBOX_EDGE_CENTER);
    SPINBOX_SetEdge(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_3),SPINBOX_EDGE_CENTER);
    SPINBOX_SetRange(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0),1,100000); //暂时的数值设定
    SPINBOX_SetRange(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_1),0,10000);
    SPINBOX_SetRange(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_2),0,5000);
    SPINBOX_SetRange(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_3),0,100);
    
    LogPrint("\ninit OK!",pMsg->hWin);
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_SPINBOX_0: // Notifications sent by 'Spinbox_f'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
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
    case ID_RADIO_0: // Notifications sent by 'Radio_WaveForm'
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
    case ID_BUTTON_0: // Notifications sent by 'Button_OUTPUT'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_SPINBOX_1: // Notifications sent by 'Spinbox_Vpp'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
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
    case ID_SPINBOX_2: // Notifications sent by 'Spinbox_offset'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
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
    case ID_SPINBOX_3: // Notifications sent by 'Spinbox_duty'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
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
    case ID_LISTBOX_0: // Notifications sent by 'Listbox_step'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_SEL_CHANGED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_RADIO_1: // Notifications sent by 'Radio_shortcut'
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
    case ID_HEADER_0: // Notifications sent by 'Header'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_MOVED_OUT:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_SLIDER_0: // Notifications sent by 'Slider_offset'
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

        SPINBOX_SetValue(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_2),(long int)SLIDER_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0)));
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_SLIDER_1: // Notifications sent by 'Slider_Vpp'
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
    case ID_SLIDER_2: // Notifications sent by 'Slider_f'
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
    case ID_SLIDER_3: // Notifications sent by 'Slider_duty'
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
*       CreateSingalWindow
*/
WM_HWIN CreateSingalWindow(void);
WM_HWIN CreateSingalWindow(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)


// USER END

/*************************** End of file ****************************/
