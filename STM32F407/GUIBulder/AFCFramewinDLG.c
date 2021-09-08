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
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "led.h"
#include "adc.h"
#include "dac.h"
#include "wave_output.h"
#include "delay.h"
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_FRAMEWIN_0     (GUI_ID_USER + 0x00)
#define ID_GRAPH_0     (GUI_ID_USER + 0x01)
#define ID_RADIO_0     (GUI_ID_USER + 0x02)
#define ID_SPINBOX_0     (GUI_ID_USER + 0x03)
#define ID_PROGBAR_0     (GUI_ID_USER + 0x04)
#define ID_BUTTON_0     (GUI_ID_USER + 0x05)
#define ID_CHECKBOX_0     (GUI_ID_USER + 0x06)
#define ID_TEXT_0     (GUI_ID_USER + 0x08)
#define ID_TEXT_1     (GUI_ID_USER + 0x09)
#define ID_SLIDER_0     (GUI_ID_USER + 0x0A)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
//log 相关
#define LOGS_MAX_LENGTH 1000
static char Logs[LOGS_MAX_LENGTH] = "* Yexiavqiufeng & Framist *\n\n--- all rights reserved ---\n\nLogs:";
//* Yexiavqiufeng & Framist *
//--- all rights reserved ---
static void AFC_LogPrint(char *log, WM_MESSAGE *pMsg);


//绘图相关
static GRAPH_SCALE_Handle _hScaleV;   // Handle of vertical scale 垂直刻度
static GRAPH_SCALE_Handle _hScaleH;   // Handle of horizontal scale 水平刻度
static const int   _aNumPoints = 100; //注意栈溢出！！！！！！！！！！
static GUI_POINT _aPoint[100];
static GRAPH_DATA_Handle  _ahDataXY;

//测量相关
static int F_Ouput = 100; //100Hz~10,000Hz
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { FRAMEWIN_CreateIndirect, "AFCFramewin", ID_FRAMEWIN_0, 0, 0, 452, 280, 0, 0x64, 0 },
  { GRAPH_CreateIndirect, "Graph", ID_GRAPH_0, 228, 4, 214, 250, 0, 0x0, 0 },
  { RADIO_CreateIndirect, "Radio", ID_RADIO_0, 14, 9, 143, 61, 0, 0x1403, 0 },
  { SPINBOX_CreateIndirect, "Spinbox", ID_SPINBOX_0, 12, 75, 110, 30, 0, 0x0, 0 },
  { PROGBAR_CreateIndirect, "Progbar", ID_PROGBAR_0, 128, 140, 87, 18, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 138, 78, 83, 30, 0, 0x0, 0 },
  { CHECKBOX_CreateIndirect, "Checkbox", ID_CHECKBOX_0, 13, 137, 120, 21, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "LogText", ID_TEXT_0, 11, 206, 205, 45, 0, 0x64, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_1, 13, 162, 210, 47, 0, 0x64, 0 },
  { SLIDER_CreateIndirect, "Slider", ID_SLIDER_0, 7, 110, 208, 25, 0, 0x0, 0 },
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
static void AFC_LogPrint(char *log, WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    if (strlen(log) + strlen(Logs) >= LOGS_MAX_LENGTH)
    {
        strcpy(Logs, Logs + LOGS_MAX_LENGTH - 150);
    }
    //strcat(Logs,"\n");
    strcat(Logs, log);
    TEXT_SetText(hItem, Logs);
    LED1 = !LED1;
}
/*********************************************************************
*
*       _InitPoints 画初始函数，测试用
*/
static void _InitPoints(void) {
    int i;
    for (i = 0; i < _aNumPoints ; i++) {
        _aPoint[i].x = (int)(i * 5 );
        _aPoint[i].y = (int) 100+ 100*sin(i*5/20.0) ;
    }

}
/*********************************************************************
*
*       @plot_aPoint 绘aPoint函数
*/
static void plot_aPoint(WM_MESSAGE *pMsg){
    WM_HWIN hItem;
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
    //删除上次绘图的数据
    GRAPH_DetachData(WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0),_ahDataXY );
    GRAPH_DATA_XY_Delete(_ahDataXY);
    
    //增加此次绘图的数据
    //GRAPH_SetVSizeX(hItem,550);
    _ahDataXY = GRAPH_DATA_XY_Create(GUI_LIGHTGREEN, _aNumPoints, _aPoint, _aNumPoints);
    GRAPH_AttachData(hItem, _ahDataXY);
    //WM_Paint(hItem);
    
}

/*********************************************************************
*
*       @SignMeasure 单次测量
*/
static float SignMeasure(WM_MESSAGE *pMsg){

    extern float Vpp_measured;
    Wave_Output_Config_F(F_Ouput);

/*    原始方式
    u32 nus = 1.0/F_Ouput*1000000.0 * 30.0; //测30次
//    sprintf(stemp, "\ndelay = %d us", nus);
//    LogPrint(stemp, pMsg);
    
    u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值
	ticks=nus*72; 						//需要的节拍数     
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1){
		tnow=SysTick->VAL;	
		if(tnow!=told){	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
        adcx = Get_Adc(ADC_CHANNEL_1);     //得到ADC原始值	 
        if(adcxMax < adcx){
            adcxMax = adcx;
        }
	};
*/ 
    Measure(); // TODO 不准，应根据频率调节测量
    

    return Vpp_measured/1000.0f;
}
/*********************************************************************
*
*       @cmpfunc 排序所用的比较函数
*/
static int cmpfunc(const void *a, const void *b) {
    const float *aa = a;
    const float *bb = b;
    return (*aa > *bb) - (*aa < *bb);
}
/*********************************************************************
*
*       @AutoMeasure 自动测量
*/
static int AutoMeasure(WM_MESSAGE *pMsg,int ifMeasure){
    int filterType; // 0：低通 1：高通 2：带通 3：带阻
    int i = 0;
    static float pointsX[100] ; //频率-真实值Hz
    static float pointsY[100] ; //电压-真实值V
    static float pointsYS[100] ; //用于排序找最大最小值的电压
    static float mindB;
    PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0),0);//显示 0%
    WM_Paint(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0));
    if(ifMeasure == 1){
        //100 ~ 10 000 Hz  9 900 / 100 + 1 = 100
        for(F_Ouput = 100; F_Ouput <= 10000; F_Ouput += 100){
            pointsX[i] = (float) F_Ouput;
            pointsY[i] = SignMeasure(pMsg) ;
            pointsYS[i] = pointsY[i];
            i++;
        }

        qsort(pointsYS, 100, sizeof(float),cmpfunc );//升序
        mindB = (log10f(pointsYS[0]/2.828f)*20.0f );//最大的db绝对值
        //float maxdB = (log10f(pointsYS[99]/2.828)*20.0 );  
    }
    if(CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0))==0) {
        //普通绘制
        for(i = 0; i < _aNumPoints ; i++ ){   
            if(i < _aNumPoints-1 && fabs(pointsY[i] - pointsY[i-1]) > 0.3) { // 去毛刺
                pointsY[i] = pointsY[i+1];
            }            
            _aPoint[i].x = (int)( pointsX[i]/10000.0f*200.0f  );
            _aPoint[i].y = (int)(225 - (log10f(pointsY[i]/2.42f)*20.0f )/mindB *225 );//先转换为db,再装换为像素
            //plot_aPoint( pMsg);

        }
        //画曲线
        plot_aPoint( pMsg);
        //更新坐标轴
        // 修改垂直刻度
        GRAPH_SCALE_SetOff(_hScaleV, 225);//刻度对象的偏移
        GRAPH_SCALE_SetFactor(_hScaleV, -mindB / 225.0f);//坐标缩放
        GRAPH_SCALE_SetNumDecs(_hScaleV,2);
        
        //GRAPH_AttachScale(hItem, _hScaleV);
        // 修改水平刻度
        GRAPH_SCALE_SetOff(_hScaleH, 0);//刻度对象的偏移
        GRAPH_SCALE_SetFactor(_hScaleH, 10000.0/200.0);//坐标缩放 单位是Hz
        GRAPH_SCALE_SetNumDecs(_hScaleH,0);
        GRAPH_SCALE_SetTextColor(_hScaleH, GUI_DARKGREEN);
    }else {
        //对数绘制
        for(i = 0; i < _aNumPoints ; i++ ){ 
            if(i < _aNumPoints-1 && fabs(pointsY[i] - pointsY[i-1]) > 0.3) { // 去毛刺
                pointsY[i] = pointsY[i+1];
            }            
            _aPoint[i].x = (int)( (log10f(pointsX[i])-2)/2.0f*200.0f  );//log10 : 2~4
            _aPoint[i].y = (int)(225 - (log10f(pointsY[i]/2.42f)*20.0f )/mindB *225 );//先转换为db,再装换为像素
            //plot_aPoint( pMsg);
        }
        //画曲线
        plot_aPoint( pMsg);
        //更新坐标轴
        // 修改垂直刻度
        //GRAPH_SCALE_SetOff(_hScaleV, 100);//刻度对象的偏移
        GRAPH_SCALE_SetFactor(_hScaleV, -mindB / 225.0f);//坐标缩放
        GRAPH_SCALE_SetNumDecs(_hScaleV,1);
        
        //GRAPH_AttachScale(hItem, _hScaleV);
        // 修改水平刻度
        GRAPH_SCALE_SetOff(_hScaleH, -200);//刻度对象的偏移
        GRAPH_SCALE_SetFactor(_hScaleH, 2.0/200.0);//坐标缩放 单位是Hz的指数
        GRAPH_SCALE_SetNumDecs(_hScaleH,2);
        GRAPH_SCALE_SetTextColor(_hScaleH, GUI_DARKCYAN);
        //GRAPH_SCALE_SetTextColor(_hScaleH, GUI_BLACK);//隐藏坐标
    }
    //滤波器判别  0：低通 1：高通 2：带通 3：带阻
    if(pointsY[0] < pointsY[50] && pointsY[50] < pointsY[99]){
        //高通
        filterType = 1;
        TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1), "Filter Type: HPF");
    }else if(pointsY[0] > pointsY[50] && pointsY[50] > pointsY[99]){
        //低通
        filterType = 0;
        TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1), "Filter Type: LPF");
    }else if(pointsY[0] < pointsY[50] && pointsY[50] > pointsY[99]){
        //带通滤波器
        filterType = 2;
        TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1), "Filter Type: BPF");
    }else if(pointsY[0] < pointsY[50] && pointsY[50] > pointsY[99]) {
        filterType = 3;
        TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1), "Filter Type: BRF");
    }else {
        filterType = -1;
        TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1), "Filter Type: ?");
    }
    TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1), GUI_FONT_24_1);
    WM_Paint(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1));
    
    PROGBAR_SetValue(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0),100);//显示 100% 其实没什么用
    WM_Paint(WM_GetDialogItem(pMsg->hWin, ID_PROGBAR_0));
    
    return filterType;
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
    // Initialization of 'AFCFramewin'
    //
    hItem = pMsg->hWin;
    FRAMEWIN_SetText(hItem, "amplitude - frequency characteristics   |   Yexiaqiufeng, Framist");
    FRAMEWIN_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
    FRAMEWIN_SetFont(hItem, GUI_FONT_16B_1);
    //
    // Initialization of 'Radio'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_RADIO_0);
    RADIO_SetText(hItem, "output OFF", 0);
    RADIO_SetText(hItem, "single measurement", 1);
    RADIO_SetText(hItem, "auto measurement", 2);
    RADIO_SetFont(hItem, GUI_FONT_13HB_1);
    //
    // Initialization of 'Spinbox'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0);
    SPINBOX_SetFont(hItem, GUI_FONT_20_1);
    //
    // Initialization of 'Button'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetText(hItem, "Start");
    //
    // Initialization of 'Checkbox'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0);
    CHECKBOX_SetText(hItem, "Log coordinate");
    CHECKBOX_SetFont(hItem, GUI_FONT_16_1);
    //
    // Initialization of 'LogText'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
    TEXT_SetText(hItem, "Logs:");
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_BOTTOM);
    TEXT_SetFont(hItem, GUI_FONT_16_1HK);
    //
    // Initialization of 'Text'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
    TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_TOP);
    TEXT_SetText(hItem, "Discrimination of filter type:");
    TEXT_SetFont(hItem, GUI_FONT_16_1);
    // USER START (Optionally insert additional code for further widget initialization)
    
    //
    // 滑块、微调框初始化
    //

    SLIDER_SetRange(WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0), 100, 10000);
    SPINBOX_SetRange(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0),100,10000);
    SPINBOX_SetStep(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0),10); //频率步进扩展至 10Hz
    //
    // Initialization of 'GRAPH' 图像初始化
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_GRAPH_0);
    //GRAPH_SetVSizeX(hItem,550);
    _InitPoints();
    //
    // Add graphs 绘制函数
    //
    
    _ahDataXY = GRAPH_DATA_XY_Create(GUI_LIGHTGREEN, _aNumPoints, _aPoint, _aNumPoints);
    GRAPH_DATA_XY_SetLineStyle(_ahDataXY, GUI_LS_SOLID);
    GRAPH_AttachData(hItem, _ahDataXY);
    
    //GRAPH_DATA_XY_SetOwnerDraw(_ahDataXY, _OwnerDraw);
    GRAPH_DATA_XY_SetLineVis(_ahDataXY,1);
    GRAPH_DATA_XY_SetPenSize(_ahDataXY,1);
    
    //
    // Set graph attributes 设置从一条网格线到下一网格线的距离
    //
    GRAPH_SetGridDistY(hItem, 25);
    GRAPH_SetGridVis(hItem, 1);
    //
    // Create and add vertical scale 创建并添加垂直刻度
    //
    _hScaleV = GRAPH_SCALE_Create( 35, GUI_TA_RIGHT, GRAPH_SCALE_CF_VERTICAL, 25);
    //GRAPH_SCALE_SetOff(_hScaleV, 100);//刻度对象的偏移
    //GRAPH_SCALE_SetFactor(_hScaleV, 0.04f);//坐标缩放
    GRAPH_SCALE_SetTextColor(_hScaleV, GUI_RED);
    GRAPH_SetLineStyleV(hItem, GUI_LS_DOT);
    GRAPH_AttachScale(hItem, _hScaleV);
    //
    // Create and add horizontal scale 创建并添加水平刻度
    //
    _hScaleH = GRAPH_SCALE_Create(170, GUI_TA_HCENTER, GRAPH_SCALE_CF_HORIZONTAL, 50);
    //GRAPH_SCALE_SetOff(_hScaleH, 220-550);//刻度对象的偏移
    //GRAPH_SCALE_SetFactor(_hScaleH, 0.02f);//坐标缩放
    GRAPH_SCALE_SetTextColor(_hScaleH, GUI_DARKGREEN);
    GRAPH_SetLineStyleH(hItem, GUI_LS_DOT);
    GRAPH_AttachScale(hItem, _hScaleH);
        
    // USER END
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_RADIO_0: // Notifications sent by 'Radio'
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
                sprintf(stemp, "\nRadio Option VALUE CHANGED:%d", RADIO_GetValue(WM_GetDialogItem(pMsg->hWin, ID_RADIO_0)));
                AFC_LogPrint(stemp, pMsg);
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_SPINBOX_0: // Notifications sent by 'Spinbox'
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
        sprintf(stemp, "\nFrequence VALUE CHANGED:%ld", SPINBOX_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0)));
        AFC_LogPrint(stemp, pMsg);
        F_Ouput = (int) SPINBOX_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0));
        SLIDER_SetValue(WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0),F_Ouput);
                
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_BUTTON_0: // Notifications sent by 'Button'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        // USER START (Optionally insert code for reacting on notification message)
        // USER END
        break;
      case WM_NOTIFICATION_RELEASED:
        // USER START (Optionally insert code for reacting on notification message)
      
        AFC_LogPrint("\nStart Button RELEASED: Start Begin!", pMsg);
        switch (RADIO_GetValue(WM_GetDialogItem(pMsg->hWin, ID_RADIO_0)))
        {
        case 0:
            AFC_LogPrint("\nWaring: Mode==output OFF, Can not start!", pMsg);
            break;
        case 1:
            
            AFC_LogPrint("\nMode==single measurement", pMsg);
            //单次测量
            float sm ;
            sm = SignMeasure(pMsg);
            sprintf(stemp, "\nADC max V =%.2f dB =%.2f",sm, (log10f(sm/2.42f)*20.0f ));
            AFC_LogPrint(stemp, pMsg);
            sprintf(stemp, "F   output: %dHz\n: %.2fV  %.2fdB",F_Ouput,sm,(log10f(sm/2.42f)*20.0f));
            TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1), stemp);
            TEXT_SetFont(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1), GUI_FONT_8X16);
            WM_Paint(WM_GetDialogItem(pMsg->hWin, ID_TEXT_1));
            break;
        case 2:
            AFC_LogPrint("\nMode==anto measurement", pMsg);
            //Auto测量
            AutoMeasure( pMsg , 1);
            break;
        default:
            break;
        }
        
        
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_CHECKBOX_0: // Notifications sent by 'Checkbox'
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
        sprintf(stemp, "\nCheckbox VALUE_CHANGED:%d", CHECKBOX_GetState(WM_GetDialogItem(pMsg->hWin, ID_CHECKBOX_0)));
        AFC_LogPrint(stemp, pMsg);
        switch (RADIO_GetValue(WM_GetDialogItem(pMsg->hWin, ID_RADIO_0))) {
            case 2:
                AFC_LogPrint("\nIn auto mode; strat trans to log", pMsg);
                //Auto测量
                AutoMeasure( pMsg ,0);
                break;
            default:
                break;
        }
        // USER END
        break;
      // USER START (Optionally insert additional code for further notification handling)
      // USER END
      }
      break;
    case ID_SLIDER_0: // Notifications sent by 'Slider'
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
        sprintf(stemp, "\nSlider VALUE CHANGED:%d", SLIDER_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0)));
        AFC_LogPrint(stemp, pMsg);
        F_Ouput = SLIDER_GetValue(WM_GetDialogItem(pMsg->hWin, ID_SLIDER_0));
        SPINBOX_SetValue(WM_GetDialogItem(pMsg->hWin, ID_SPINBOX_0),(long int)F_Ouput);
                
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
*       CreateAFCFramewin
*/
WM_HWIN CreateAFCFramewin(void);
WM_HWIN CreateAFCFramewin(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
  return hWin;
}

// USER START (Optionally insert additional public code)



// USER END

/*************************** End of file ****************************/
