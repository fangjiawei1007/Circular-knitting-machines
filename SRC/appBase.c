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

#include "..\inc\appBase.h"

char duanweiadd;
int fangdaomi;//用于防止工人盗米以及提升计米抗干扰

extern INTERALMEMORY g_InteralMemory;

//extern GLOBALVAR g_GlobalVar;

//extern INT16U PreScrNum;
extern char Pulseout;
extern char stopweixianduan;
unsigned int InPulseCount[7]={0};		//记录6路高速输入的脉冲数

unsigned int OutPulse_PreSet[4]={0};	// 3路脉冲输出的  脉冲预设值
unsigned int OutPulse_Counter[4]={0};	// 3路脉冲输出的  当前输出脉冲数
extern U16 midu,midu1;
extern char miduxiaoshu,miduxiaoshu1;

U16 Inpulse;
///////////////////////////////////////////////////////////////////
////	高速脉冲输入X1 中断服务程序    ///////////////
////	InPulseCount[1] 为X1  输入的脉冲数	//////////////////
///////////////////////////////////////////////////////////////////
void __irq PulseIn_1_Process(void)	
{
	int i;
	//for(i=0; i<20; i++);
		
	if(((rGPFDAT >> 1) & 0x1))
	{
		/*midu1=g_InteralMemory.KeepWord[3] * 0.677  * g_InteralMemory.KeepWord[7]/100/ g_InteralMemory.KeepWord[5]*g_InteralMemory.KeepWord[6];
		midu=(midu1-(midu1%10))/10 + 1;
		miduxiaoshu1=g_InteralMemory.KeepWord[3] * 0.677  * g_InteralMemory.KeepWord[7]/100/ g_InteralMemory.KeepWord[5]*g_InteralMemory.KeepWord[6];
		miduxiaoshu=10-miduxiaoshu1%10;
		
		*/
		Inpulse=Inpulse+10;
		
		if(Inpulse >= (midu*10))
		{
			Inpulse=Inpulse-midu*10+miduxiaoshu;
				
			fangdaomi++;
			
			rGPBDAT &=( ~(1<<1));	// GPB1
			
			for(i=0;i<2500;i++);
			rGPBDAT |= (1<<1);
			
			for(i=0;i<500;i++);
			
			
		}
		
	}
	
	
	rEINTPEND=(1<<1);
	ClearPending(BIT_EINT1);
	
}
///////////////////////////////////////////////////////////////////
////	高速脉冲输入X2 中断服务程序    ///////////////
////	InPulseCount[1] 为X1  输入的脉冲数	//////////////////
///////////////////////////////////////////////////////////////////
void __irq PulseIn_2_Process(void)	
{
	int i;
	for(i=0; i<20; i++);

	if((rGPFDAT >> 2) & 0x1)
	{
		g_InteralMemory.Bit[5]=1;//纬尾故障标志位
	}

	rEINTPEND=(1<<2);
	ClearPending(BIT_EINT2);
}
///////////////////////////////////////////////////////////////////
////	高速脉冲输入X3 中断服务程序    ///////////////
////	InPulseCount[1] 为X1  输入的脉冲数	//////////////////
///////////////////////////////////////////////////////////////////
void __irq PulseIn_3_Process(void)	
{
	int i;
	for(i=0; i<20; i++);

	if((rGPFDAT >> 3) & 0x1)
	{
		InPulseCount[3]++;
	}

	rEINTPEND=(1<<3);
	ClearPending(BIT_EINT3);
}


//////////////////////////////////////////////////////////////////////
////	高速脉冲输入X4-X6  中断服务程序     //////////////
////	InPulseCount[2] 为X2  输入的脉冲数	//////////////////
////	InPulseCount[3] 为X3  输入的脉冲数	//////////////////
/////////////////////////////////////////////////////////////////////
void __irq PulseIn_4_6_Process(void)
{
	int i;
	for(i=0; i<20; i++);

	if (rEINTPEND & (1<<4))	// Eint4	// 高速输入X2
	{
		if((rGPFDAT >> 4) & 0x1)
		{
			InPulseCount[4]++;
		}

		rEINTPEND=(1<<4);		
	}
	else if (rEINTPEND & (1<<5))	// Eint6	// 高速输入X3
	{
		if((rGPFDAT >> 5) & 0x1)
		{
			InPulseCount[5]++;
		}

		rEINTPEND=(1<<5);		
	}
	else if (rEINTPEND & (1<<6))	// Eint6	// 高速输入X3
	{
		if((rGPFDAT >> 6) & 0x1)
		{
			InPulseCount[6]++;
		}

		rEINTPEND=(1<<6);		
	}

	ClearPending(BIT_EINT4_7);
}

void __irq PulseIn_14_15_Process(void)
{
	int i;
	for(i=0; i<20; i++);

	if (rEINTPEND & (1<<14))	// Eint14	
	{
		if(!((rGPGDAT >> 6) & 0x1))
		{	
			duanweiadd++;
			
			if((duanweiadd==1) && (stopweixianduan==0))
			{
				g_InteralMemory.Bit[5]=2;//纬断故障标志位
				if(g_InteralMemory.KeepBit[610])//g_InteralMemory.KeepBit[610]四梭六梭标志位
				{
					g_InteralMemory.KeepWord[103]=g_InteralMemory.KeepWord[3];
					g_InteralMemory.KeepWord[104]=1;
					g_InteralMemory.KeepWord[3]=g_InteralMemory.KeepWord[103] * 6 / 4;//六梭纬断障时提升速度变为正常的4/6
					//g_InteralMemory.KeepWord[3]=g_InteralMemory.KeepWord[103] ;//六梭纬断障时提升速度变为正常的4/6
				}
				else 
				{
					g_InteralMemory.KeepWord[103]=g_InteralMemory.KeepWord[3];
					g_InteralMemory.KeepWord[104]=1;
					g_InteralMemory.KeepWord[3]=g_InteralMemory.KeepWord[103] * 2;//四梭纬断障时提升速度变为正常的1/2
					//g_InteralMemory.KeepWord[3]=g_InteralMemory.KeepWord[103];//四梭纬断障时提升速度变为正常的1/2
				}
			}
			
		}

		rEINTPEND=(1<<14);		
	}
	else if (rEINTPEND & (1<<15))	// Eint15 记米传感器	
	{
		if((!((rGPGDAT >> 7) & 0x1)) && fangdaomi > 9000)
		
		{
			g_InteralMemory.KeepWord[351]++;
			fangdaomi=0;
			
			if(g_InteralMemory.KeepWord[351] >= 2)
			{
				g_InteralMemory.KeepWord[102]+=1;//每转2圈增加1米
				g_InteralMemory.KeepWord[158]+=1;//每转2圈增加1米  订单用
				
				g_InteralMemory.KeepWord[351]=0;
			}
			
			
		}
		
		

		rEINTPEND=(1<<15);		
	}
	

	ClearPending(BIT_EINT8_23);
}



///////////////////////////////////////////////////////////////
////	高速脉冲输入X1  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse1_Start(void)
{
	rEINTMASK &= ~(1<<1);
	rINTMSK1 &= ~BIT_EINT1;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X1  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse1_Stop(void)
{
	rEINTMASK |=(1<<1);
	rINTMSK1 |=BIT_EINT1;
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X2  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse2_Start(void)
{
	rEINTMASK &= ~(1<<2);
	rINTMSK1 &= ~BIT_EINT2;
	
	rEINTMASK &= ~(1<<14);
	rEINTMASK &= ~(1<<15);
	rINTMSK1 &= ~BIT_EINT8_23;
	
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X2  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse2_Stop(void)
{
	rEINTMASK |=(1<<2);
	rINTMSK1 |=BIT_EINT2;
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X3  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse3_Start(void)
{
	rEINTMASK &= ~(1<<3);
	rINTMSK1 &= ~BIT_EINT3;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X3  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse3_Stop(void)
{
	rEINTMASK |=(1<<3);
	rINTMSK1 |=BIT_EINT3;
}

///////////////////////////////////////////////////////////////
////	高速脉冲输入X4  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse4_Start(void)
{
	rEINTMASK &= ~(1<<4);
	rINTMSK1 &= ~BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X4  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse4_Stop(void)
{
	rEINTMASK |=(1<<4);
	//rINTMSK1 |=BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X5  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse5_Start(void)
{
	rEINTMASK &= ~(1<<5);
	rINTMSK1 &= ~BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X5  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse5_Stop(void)
{
	rEINTMASK |=(1<<5);
	//rINTMSK1 |=BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X6  启动//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse6_Start(void)
{
	rEINTMASK &= ~(1<<6);
	rINTMSK1 &= ~BIT_EINT4_7;
}
///////////////////////////////////////////////////////////////
////	高速脉冲输入X6  停止//////////////////////////
///////////////////////////////////////////////////////////////
void InPulse6_Stop(void)
{
	rEINTMASK |=(1<<6);
	//rINTMSK1 |=BIT_EINT4_7;
}


/////////////////////////////////////////////////
//////	高速脉冲输入初始化     //////////////
//////	IN 1~6   6路高速输入	    //////////////
////////////////////////////////////////////////
void Pulse_In_Init(void)	
{
	DWORD tmp;

	tmp = rGPFCON & (~(0x3<< 2)) & (~(0x3<< 4))   & (~(0x3<< 10)) & (~(0x3<< 12));
	rGPFCON = tmp | (0x2<<2) | (0x2<<4)  | (0x2<<10) | (0x2<<12);	//set GPF1~6 as EINT	

	rGPFUDP &=  (~(0x3<< 2)) & (~(0x3<< 4))  & (~(0x3<< 10)) & (~(0x3<< 12)); //GPF1~6 up down disable

	tmp=rGPGCON & (~(0x3<< 12))& (~(0x3<< 14));
	rGPGCON = tmp | (0x2<<12) | (0x2<<14);
	rGPGUDP &=  (~(0x3<< 12)) & (~(0x3<< 14)); //set gpg6.7as enit
	
	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<4))) | (0x4<<4);		// Eint1	Rising edge triggered
	rEXTINT0 = (rEXTINT0 & (~(0x7<<8))) | (0x4<<8);		// Eint2	Rising edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<12))) | (0x4<<12);	// Eint3	Rising edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<16))) | (0x4<<16);	// Eint4	Rising edge triggered
	rEXTINT0 = (rEXTINT0 & (~(0x7<<20))) | (0x4<<20);	// Eint5	Rising edge triggered	
	rEXTINT0 = (rEXTINT0 & (~(0x7<<24))) | (0x4<<24);	// Eint6	Rising edge triggered
	
	rEXTINT1 = (rEXTINT1 & (~(0x7<<24))) | (0x2<<24);	// Eint14	Falling edge triggered
	rEXTINT1 = (rEXTINT1 & (~(0x7<<28))) | (0x2<<28);	// Eint15	Falling edge triggered	

	pISR_EINT1= (U32)PulseIn_1_Process;		// X1
	pISR_EINT2= (U32)PulseIn_2_Process;		// X2
	pISR_EINT3= (U32)PulseIn_3_Process;		// X3
	pISR_EINT4_7= (U32)PulseIn_4_6_Process;	// X4_6
	pISR_EINT8_23=(U32)PulseIn_14_15_Process;//X1415


	rEINTPEND = 0xffffff;

	rSRCPND1 |= BIT_EINT1|BIT_EINT2|BIT_EINT3|BIT_EINT4_7|BIT_EINT8_23; //to clear the previous pending states
	rINTPND1 |= BIT_EINT1|BIT_EINT2|BIT_EINT3|BIT_EINT4_7|BIT_EINT8_23;

	//rEINTMASK &= ~((1<<2)|(1<<3)|(1<<4)|(1<<6));
	//rINTMSK1 &= ~(BIT_EINT2|BIT_EINT3|BIT_EINT4_7);
	
	InPulse1_Stop();
	InPulse2_Stop();
	InPulse3_Stop();
	InPulse4_Stop();
	InPulse5_Stop();
	InPulse6_Stop();
	
}



///////////////////////////////////////////////////////////////////////////
////	Y1 脉冲发送中断服务程序////////
///////////////////////////////////////////////////////////////////////////
void __irq PulseOut_1_Process(void)
{
int tmp;
	OutPulse_Counter[1]++;
	
	if(OutPulse_Counter[1] >= OutPulse_PreSet[1])
	{
		PulseOut_1_Stop();
		tmp = rGPBCON & (~(0x3<< 2));
		rGPBCON = tmp | (0x1<<2);	
		rGPBDAT |= (1<<1);
	}

	ClearPending((U32)BIT_TIMER1);

}



///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲输出启动//////////////////////////////
////	 每发送一段脉冲 PulseOut_2_Start 只需启动一次//////
////	frequence: 脉冲频率///////////////////////////////
////	pulse_PreSet: 发送脉冲数 ///////////////////////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_1_Start(unsigned int frequence, unsigned int pulse_PreSet)
{
	DWORD tmp;
	
	if(0 == pulse_PreSet)
	{
		PulseOut_1_Stop();
		return;
	}
	else
	{
		OutPulse_PreSet[1] = pulse_PreSet;
		OutPulse_Counter[1] = 0;	// 确保每次启动PulseOut_1_Start ，发送脉冲数 pulse_PreSet
	}
	
	if(0 == frequence)
	{
		frequence = 1;
		PulseOut_1_Stop();
		return;
	}
	
	rTCNTB1= 30*100909/frequence;	// 100909  	100K  	100000
	rTCMPB1 = rTCNTB1/2;
	
	rINTMSK1 &=~(BIT_TIMER1);
	tmp = rTCON & (~(0xf << 8));	// dead zone Disable
	rTCON = tmp | (2 << 8);		/* update TCVNTB0, stop					*/
	rTCON = tmp | (9 << 8);		/* interval mode,  start				*/	
}

///////////////////////////////////////////////////////////////////////////
////	Y1 脉冲发送停止////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_1_Stop(void)
{
	DWORD tmp;

	rINTMSK1 |= BIT_TIMER1;
	rTCON &= ~(1 << 8);			/* Timer1, stop							*/


}


///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲发送中断服务程序////////
///////////////////////////////////////////////////////////////////////////
void __irq PulseOut_2_Process(void)
{
	OutPulse_Counter[2]++;
	
	if(OutPulse_Counter[2] >= OutPulse_PreSet[2])
	{
		PulseOut_2_Stop();
	}

	ClearPending((U32)BIT_TIMER2);

}



///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲输出启动//////////////////////////////
////	 每发送一段脉冲 PulseOut_2_Start 只需启动一次//////
////	frequence: 脉冲频率///////////////////////////////
////	pulse_PreSet: 发送脉冲数 ///////////////////////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_2_Start(unsigned int frequence, unsigned int pulse_PreSet)
{
	DWORD tmp;
	
	if(0 == pulse_PreSet)
	{
		PulseOut_2_Stop();
		return;
	}
	else
	{
		OutPulse_PreSet[2] = pulse_PreSet;
		OutPulse_Counter[2] = 0;	// 确保每次启动PulseOut_1_Start ，发送脉冲数 pulse_PreSet
	}
	
	if(0 == frequence)
	{
		frequence = 1;
		PulseOut_2_Stop();
		return;
	}
	
	rTCNTB2= 30*100909/frequence;	// 100909  	100K  	100000
	rTCMPB2 = rTCNTB2/2;
	
	rINTMSK1 &=~(BIT_TIMER2);
	tmp = rTCON & (~(0xf << 12));	// dead zone Disable
	rTCON = tmp | (2 << 12)	;/* update TCVNTB0, stop					*/
	rTCON = tmp | (9 << 12)	;/* interval mode,  start				*/	
}

///////////////////////////////////////////////////////////////////////////
////	Y2 脉冲发送停止////////
///////////////////////////////////////////////////////////////////////////
void PulseOut_2_Stop(void)
{
	DWORD tmp;

	rINTMSK1 |= BIT_TIMER2;
	rTCON &= ~(1 << 12);		/* Timer1, stop							*/

}






/////////////////////////////////////////////////
//////	高速脉冲输出初始化     //////////////
//////	OUT 1~3   3路高速输出 //////////////
////////////////////////////////////////////////
void Pulse_Out_Init()	
{
	DWORD tmp;
	char i;

	tmp = rGPBCON & (~(0x3<< 2)) & (~(0x3<< 4))& (~(0x3<< 6));
	rGPBCON = tmp | (0x2<<2) | (0x2<<4) | (0x2<<6);	//set GPB1 2 3 as TOUT	

	// Timer1 Initialize	----HS Out2
	pISR_TIMER1 = (int)PulseOut_1_Process;	// Timer ISR for HS Out
	rTCFG0 &= ~(0xff << 0); 
	rTCFG0 |= (10 << 0); 	// Dead zone=0, Prescaler0=10(0xff)
	rTCFG1 &= ~(0xf << 4); 
	rTCFG1 |= (0x0 << 4); 	// Interrupt, Mux0=1/2
	rTCNTB1 = 30;    //30.27273	// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	100K
	rTCMPB1 = 15;
	PulseOut_1_Stop();
		
	// Timer3 Initialize	----HS Out3
//	pISR_TIMER3 = (int)PulseOut_3_Process;	// Timer ISR for HS Out
//	rTCFG0 &= ~(0xff << 0); 
//	rTCFG0 |= (10 << 0); 	// Dead zone=0, Prescaler0=10(0xff)
//	rTCFG1 &= ~(0xf << 0); 
//	rTCFG1 |= (0x0 << 0); 	// Interrupt, Mux0=1/2
//	rTCNTB3 = 30;    //30.27273	// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	100K
//	rTCMPB3 = 15;
//	PulseOut_3_Stop();

	for(i=0;i<100;i++);
}






//////////////////////////////////////////////////////////////////////
////	DA 输出中断服务程序///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void __irq DA0_10_Process(void) // 可通过示波器确定频率
{
	if(0xffff == (g_InteralMemory.Word[32] & 0xffff))
	{
		g_InteralMemory.Word[32] = 0;
		g_InteralMemory.Word[33]++;
	}
	else
		g_InteralMemory.Word[32]++;

	ClearPending((U32)BIT_TIMER3);
}


//////////////////////////////////////////////////////////////////////
////	DA 输出启动(只需启动一次)//////////////////////////
////	voltage: 输出电压(0.000~10.000V )  小数点后3 位//////////
//////////////////////////////////////////////////////////////////////
void DA0_10_Start(float voltage)
{
	DWORD tmp;

	if(voltage>10*1000)
		voltage = 10*1000;
	else if(voltage<0)
		voltage = 0;
	
	rTCNTB3= 300;	
	//rTCMPB3 = (rTCNTB3*g_InteralMemory.Word[30])/(10000*1.326);// 最大10V , 小数点后3 位10*1000
	rTCMPB3 = (rTCNTB3*voltage)/(10*1000*1.326);// 最大10V , 小数点后3 位10*1000
	if(rTCMPB3 == rTCNTB3)
		rTCMPB3 -= 1;
	
	tmp = rGPBCON & (~(0x3<< 6));
	rGPBCON = tmp | (0x2<<6);	//set GPB3 as TOUT	

	rINTMSK1 &=~(BIT_TIMER3);
	tmp = rTCON & (~(0xf << 16));	// dead zone Disable
	rTCON = tmp | (2 << 16);		/* update TCVNTB3, stop					*/
	rTCON = tmp | (9 << 16);		/* interval mode,  start				*/	
}

//////////////////////////////////////////////////////////////////////
////	DA 输出停止//////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void DA0_10_Stop(void)
{
	DWORD tmp;

	rTCNTB3= 300;	
	rINTMSK1 |= BIT_TIMER3;
	rTCON &= ~(1 << 16);			/* Timer3, stop							*/

	tmp = rGPBCON & (~(0x3<< 6));
	rGPBCON = tmp | (1<<6);	//set GPB3 as OUT	
	rGPBDAT &= ~(1<<3);
}


//////////////////////////////////////////////////////////////////////
////	DA 输出初始化///////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void DA0_10_Init(void)
{
	DWORD tmp;

	tmp = rGPBCON & (~(0x3<< 6));
	rGPBCON = tmp | (0x2<<6);	//set GPB3 as TOUT	

	// Timer3 Initialize	----DA
	pISR_TIMER3 = (int)DA0_10_Process;	// Timer ISR for DA
	rTCFG0 &= ~(0xff << 8); 
	rTCFG0 |= (110 << 8); 	// Dead zone=0, Prescaler0=110(0xff)   Timer 2 3 4 共用
	rTCFG1 &= ~(0xf << 12); 
	rTCFG1 |= (0x0 << 12); 	// Interrupt, Mux0=1/2
	rTCNTB3 = 300;    			// Timer input clock Frequency = PCLK / {prescaler value+1} / {divider value}	1K
	rTCMPB3 = 150;
	DA0_10_Stop();
}



/////////////////////////////////////////////////
//////	普通输入初始化     //////////////
//////	IN7~IN15 共9 路		//////////////
////////////////////////////////////////////////
void X_In_Init(void)	
{
	DWORD tmp;

	tmp = rGPFCON & (~(0x3<< 14)) & (~(0x3<< 6)) & (~(0x3<< 8));
	rGPFCON = tmp |(0x0<<14) |(0x0<<6)|(0x0<<8);			//set GPF3,4,7 as input
	//////////////
	tmp = rGPGCON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8))
					 & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14));
	
	rGPGCON = tmp | (0x0<<0) | (0x0<<2) | (0x0<<4) | (0x0<<6) | (0x0<<8) | (0x0<<10) | (0x0<<12) | (0x0<<14);		//set GPF5 7 as input	
	//////////	
}



/////////////////////////////////////////////////
//////	普通输出初始化     //////////////
//////	Y2~Y15 共14 路	 //////////////
////////////////////////////////////////////////
void Y_Out_Init(void)		
{
	DWORD tmp;

	tmp = rGPECON & (~(0x3<< 0)) & (~(0x3<< 2)) & (~(0x3<< 4)) & (~(0x3<< 6)) & (~(0x3<< 8)) & (~(0x3<< 10)) & (~(0x3<< 12)) & (~(0x3<< 14));
	
	
	rGPECON = tmp | (0x1<<0) | (0x1<<2) | (0x1<<4) | (0x1<<6) | (0x1<<8) | (0x1<<10) | (0x1<<12) | (0x1<<14);	//set GPB4 5 9 10 as output	
	// 输入输出都反向
	rGPEDAT |= (1<<0);	// OUT4		
	rGPEDAT |= (1<<1);	// OUT5		
	rGPEDAT |= (1<<2);	// OUT6		
	rGPEDAT |= (1<<3);	// OUT7		
	rGPEDAT |= (1<<4);	// OUT8		
	rGPEDAT |= (1<<5);	// OUT9		
	rGPEDAT |= (1<<6);	// OUT10	
	rGPEDAT |= (1<<7);	// OUT11
	
	tmp = rGPBCON & (~(0x3<< 2));
	rGPBCON = tmp | (0x1<<2);	
	rGPBDAT |= (1<<1);

}





////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
////	输入   IN7-IN15//////////////////////
////	返回: 端口状态
////////////////////////////////////////////////////////////////
unsigned char Get_X_Value(unsigned char X_num)
{
	unsigned char X_value=0xff;

	switch(X_num)
	{
		case 8:
			X_value = (rGPGDAT >> 0) & 0x1;	// X8	
			break;
		case 9:
			X_value = (rGPGDAT >> 1) & 0x1;	// X9
			break;
		case 10:
			X_value = (rGPGDAT >> 2) & 0x1;	// X10
			break;
		case 11:
			X_value = (rGPGDAT >> 3) & 0x1;	// X11
			break;
		case 12:
			X_value = (rGPGDAT >> 4) & 0x1;	// X12
			break;
		case 13:
			X_value = (rGPGDAT >> 5) & 0x1;	// X13
			break;
		case 14:
			X_value = (rGPGDAT >> 6) & 0x1;	// X14
			break;
		case 15:
			X_value = (rGPGDAT >> 7) & 0x1;	// X15
			break;
		case 7:
			X_value = (rGPFDAT >> 7) & 0x1;	// X7
			break;
		case 4:
			X_value = (rGPFDAT >> 4) & 0x1;	// X4
			break;
		case 3:
			X_value = (rGPFDAT >> 3) & 0x1;	// X3
			break;
		
		
	}

	return !X_value;// xuzhiqin tixing xiugai
}


////////////////////////////////////////////////////////////////
////	输出   Y2~Y15    /////////////////////////
////	Y_num:2~15		Value:   0或1    		//////
////////////////////////////////////////////////////////////////
void Set_Y_Value(unsigned char Y_num, unsigned char Value)
{
	Value = !Value;// xuzhiqin tixing xiugai
	
	switch(Y_num)
	{
	case 4:
		(Value) ? (rGPEDAT |= (1<<0)) : (rGPEDAT &= ~(1<<0));	// Y4	
		break;
	case 5:
		(Value) ? (rGPEDAT |= (1<<1)) : (rGPEDAT &= ~(1<<1));	// Y5	
		break;
	case 6:
		(Value) ? (rGPEDAT |= (1<<2)) : (rGPEDAT &= ~(1<<2));	// Y6	
		break;
	case 7:
		(Value) ? (rGPEDAT |= (1<<3)) : (rGPEDAT &= ~(1<<3));	// Y7	
		break;
	case 8:
		(Value) ? (rGPEDAT |= (1<<4)) : (rGPEDAT &= ~(1<<4));	// Y8	
		break;
	case 9:
		(Value) ? (rGPEDAT |= (1<<5)) : (rGPEDAT &= ~(1<<5));	// Y9	
		break;
	case 10:
		(Value) ? (rGPEDAT |= (1<<6)) : (rGPEDAT &= ~(1<<6));	// Y10	
		break;
	case 11:
		(Value) ? (rGPEDAT |= (1<<7)) : (rGPEDAT &= ~(1<<7));	// Y11	
		break;
	}

	
}





