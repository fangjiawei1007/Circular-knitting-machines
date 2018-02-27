#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "def.h"
#include "2416addr.h"
#include "2416lib.h"
#include "option.h"
#include "LCD.h"
#include "mmu.h"
#include "Nand.h"
#include "..\inc\download.h"
#include "..\inc\systemconf.h"
#include "..\inc\systemmenu.h"
#include "..\inc\function.h"
#include "..\inc\jiema.h"
#include "..\inc\communication.h"
#include "..\inc\test.h"
#include "..\inc\Rtc.h"
#include "..\inc\encrypt.h"

#include "..\inc\app.h"
#include "..\inc\appBase.h"





extern INTERALMEMORY g_InteralMemory;

//extern GLOBALVAR g_GlobalVar;

//extern INT16U PreScrNum;


extern unsigned int InPulseCount[4];		//记录4路编码器输入的脉冲数

extern unsigned int OutPulse_PreSet[2];	// 2路脉冲输出的  脉冲预设值
extern unsigned int OutPulse_Counter[2];	// 2路脉冲输出的  当前输出脉冲数




///////////////////////////////////////////////////////////////
//////	初始化     //////////////////////////////////////////
//////	每次上电App_Init 只执行一次	 //////////////////
//////////////////////////////////////////////////////////////
void App_Init(void)
{
	int i,tmp;
	
	X_In_Init();
	Y_Out_Init();
	Pulse_In_Init();
//	Pulse_Out_Init();
//	DA0_10_Init();

	// 脉冲输入初始化
	for(i=0; i<4; i++)
	{
		InPulseCount[i] = 0;
	}
}


///////////////////////////////////////////////////////////////
//////	应用主程序     //////////////////////////////////////////
//////	系统每个循环会调用一次App_Operation	 //////////////////
//////////////////////////////////////////////////////////////
void App_Operation(void)
{
//	int i;

	//Test_Run();
	//Test_RTC_SetTime();  // 按钮控制
	//Test_RTC_ReadTime(); 


	// DA
/*		if(0 == g_InteralMemory.Bit[211])	// DA 当前状态停止
	{
		if(1 == g_InteralMemory.Bit[210])	// Set DA
		{
			DA0_10_Start(g_InteralMemory.Word[30]);
			g_InteralMemory.Bit[211] = g_InteralMemory.Bit[210];
		}
	}
	if(1 == g_InteralMemory.Bit[211])	// DA 当前状态启动
	{
		if(0 == g_InteralMemory.Bit[210])	// ReSet DA
		{
			DA0_10_Stop();
			g_InteralMemory.Bit[211] = g_InteralMemory.Bit[210];
		}
	}

 

	for(i=2; i<16; i++)
	{
		Set_Y_Value(i, TRUE);
		Set_Y_Value(i, FALSE);
	}
*/	
//////////////////////////////////////////////////////////////
/////////////////////从此开始添加用户程序/////////////////////
//////////////////////////////////////////////////////////////





}





