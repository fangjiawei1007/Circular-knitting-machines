/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////凸轮式圆织机相关功能函数///////////////////////
/////////////////By Wang Liang   上海理工大学数控数显实验室  2012.3 /////////////         
/////////////////////////////////////////////////////////////////////////////////
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "2416addr.h"
#include "2416lib.h"

#include "..\inc\download.h"
#include "..\inc\def.h"
#include "..\inc\lcd.h"
#include "..\inc\systemconf.h"
#include "..\inc\communication.h"
#include "..\inc\yuanzhiji.h"

extern INTERALMEMORY g_InteralMemory;
char currenttime;
char stopweixianduan;//停机、点动停机、经断停机屏蔽纬断信号

char miduxiaoshu1,miduxiaoshu;
char Pulseout;
U16 midu,midu1;
void Status_Monitoring1(void);
extern char timeadd;
extern char duanweiadd;
extern int CurrentScrNum;
void yuanzhiji_innt(void)
{
	g_InteralMemory.Bit[158]=0;//时间设定按钮标志位
	g_InteralMemory.Bit[6]=0;//外部按钮启停
	g_InteralMemory.Bit[7]=0;
	g_InteralMemory.KeepBit[604]=0;//班次切换标志位
	g_InteralMemory.KeepBit[600]=0;//点动提升测试标志位
	g_InteralMemory.KeepBit[601]=0;
	g_InteralMemory.KeepBit[602]=0;//
	
	g_InteralMemory.KeepBit[608]=0;//手动提升标志位
	g_InteralMemory.KeepWord[15]=0;//
	if(g_InteralMemory.KeepWord[16] > 10)
		g_InteralMemory.KeepWord[16]=0;//数据保存位置标志
	g_InteralMemory.KeepWord[101]=0;//初始化脉冲累计值
	g_InteralMemory.KeepWord[100]=0;
	g_InteralMemory.KeepBit[10]=0;//设备状态
	
	if(g_InteralMemory.KeepWord[104] > 1)
		g_InteralMemory.KeepWord[104]=0;//纬断标志位
	
	g_InteralMemory.KeepBit[11]=0;//清空保存设备故障标志位
	
	if(g_InteralMemory.KeepWord[92] > 7)
		g_InteralMemory.KeepWord[92]=0;
	if(g_InteralMemory.KeepWord[2] > 200)//频率初始化
		g_InteralMemory.KeepWord[2]=0;
		
	if(g_InteralMemory.KeepWord[111] > 200)//用于通信错误时恢复之前的转速数据
		g_InteralMemory.KeepWord[111]=0;
		
		
	if((g_InteralMemory.KeepWord[102]>= 65500) | (g_InteralMemory.KeepWord[102] < 0))//加工长度初始化
	{
		g_InteralMemory.KeepWord[102]=0;
	}
	
	if(g_InteralMemory.KeepWord[0]>3)
		g_InteralMemory.KeepWord[0]=1;

//	g_InteralMemory.KeepWord[56]=0;	//定时器使用标志位
	
		
	if(g_InteralMemory.KeepBit[605] > 1)//是否使用自动停经
		g_InteralMemory.KeepBit[605]=0;
	if(g_InteralMemory.KeepBit[606] > 1)//是否使用自动停纬
		g_InteralMemory.KeepBit[606]=0;
	
	g_InteralMemory.KeepWord[95]=0;
		
	if(g_InteralMemory.KeepWord[7] > 300)//电子齿轮比修正系数，为百分比
		g_InteralMemory.KeepWord[7]=100;
		
	if(g_InteralMemory.KeepBit[610] >= 2)//四梭六梭选择
		g_InteralMemory.KeepBit[610]=1; //1.六梭2.四梭
	
	g_InteralMemory.KeepBit[609]=0;
	if(g_InteralMemory.KeepWord[150] > 100)
		g_InteralMemory.KeepWord[150]=1;
		
	if(g_InteralMemory.KeepWord[5] > 1000)
		g_InteralMemory.KeepWord[5]=45;
	if(g_InteralMemory.KeepWord[6] > 1000)
		g_InteralMemory.KeepWord[6]=18;
	if(g_InteralMemory.KeepWord[8] > 1000)
		g_InteralMemory.KeepWord[8]=100;
	if(g_InteralMemory.KeepWord[13] > 100)/////送经延时初始化
		g_InteralMemory.KeepWord[13]=10;
		
	//if(g_InteralMemory.KeepWord[260] > 1000)/////密度微调系数
	//	g_InteralMemory.KeepWord[260]=1;
	if(g_InteralMemory.KeepBit[620] > 2)///停机密度补偿开关
		g_InteralMemory.KeepBit[620]=0;
	
	g_InteralMemory.Word[630]=1;////设备运转状态 0 运行中  1 无故障停机  2 故障停机
	g_InteralMemory.Word[629]=0;///订单产量提醒次数
	
	if(g_InteralMemory.KeepWord[3]>=3000)//密度
		g_InteralMemory.KeepWord[3]=380;
	if(g_InteralMemory.KeepWord[3]<100)//密度
		g_InteralMemory.KeepWord[3]=380;
	
	if(g_InteralMemory.KeepWord[350] > 10)//是否开启小数密度补偿
		g_InteralMemory.KeepWord[350] = 1;
	
	if(g_InteralMemory.KeepWord[351] > 10)//计米标志位
		g_InteralMemory.KeepWord[351] = 0;
	
	Pulseout=0;
	InPulse1_Start();
	InPulse2_Start();
}

void  DataSave_yuanzhiji(void)/////所有数据保存均放到掉电中断
{
	INT8U *PageBuff;
	int Block;
	//int i;

	Block = Logical_To_Physical_Block(NAND_DATA_SAVE);
	
	EraseBlock(Block);
	
	PageBuff = (INT8U *)&g_InteralMemory.KeepWord[0];
	WritePage(Block,0,PageBuff);	// 暂时只用第0页（2K）保存数据

	PageBuff = (INT8U *)&g_InteralMemory.KeepBit[0];
	WritePage(Block,1,PageBuff);	// 暂时只用第1页（2K）保存数据
	
}
//char add12;
void yuanzhiji_run()
{
	char i,j,useless[2],SendSucceed;
	DWORD tmp;
	i=0;
	
	//////////////////////////////////////
	////判断变频器是否启动，启动后开启脉冲输出，在此为了屏蔽停机时的干扰信号
	if(Get_X_Value(3)==1)
		Pulseout=1;
	else
		Pulseout=0;
		
	
	if(g_InteralMemory.KeepBit[610] == 0)////////六梭四梭传动比不一样导致通信时计算参数不一致
		g_InteralMemory.KeepWord[250] = 185;//四梭
	else
		g_InteralMemory.KeepWord[250] = 160;//六梭
	////////////************////////////////
	if(Get_X_Value(11))//外部按钮开机
	{
		g_InteralMemory.Bit[6]=1;
		g_InteralMemory.KeepBit[602]=0;
		
		g_InteralMemory.Word[630]=0;///设备状态，联网通信用
		
		if(g_InteralMemory.KeepWord[104] == 1)//出现纬线断时，恢复生产用的提升参数
		{
			g_InteralMemory.KeepWord[3]=g_InteralMemory.KeepWord[103];
			g_InteralMemory.KeepWord[104]=0;
		}
		
		rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//先屏蔽定时器3中断，为了防止连续点击启动按钮对送经电机的影响
		timeadd=0;//定时器清0
		
		if((g_InteralMemory.KeepBit[606]==0) && (g_InteralMemory.KeepWord[95]==1) )
		{
			Set_Y_Value(7,0);//rGPGDAT |= 0x1;//故障时主机停机
			timer3_start();
			g_InteralMemory.KeepWord[95]=0;
			g_InteralMemory.Word[630]=1;
		}
		duanweiadd=0;
		stopweixianduan=0;
		
		Set_Y_Value(5,0);///停机反转再次启动后恢复转向0906
		tmp = rGPBCON & (~(0x3<< 2));
		rGPBCON = tmp | (0x1<<2);	
		rGPBDAT |= (1<<1);
		Timer2_Stop();
	
	}
	if(Get_X_Value(12))//外部按钮停机
	{
		g_InteralMemory.Bit[7]=1;
		stopweixianduan=1;
		g_InteralMemory.Word[630]=1;
	}
		
	if(Get_X_Value(13))//外部按钮点动
	{
		g_InteralMemory.Bit[8]=1;
		if(g_InteralMemory.KeepWord[104] == 1)
		{
			g_InteralMemory.KeepWord[3]=g_InteralMemory.KeepWord[103];
			g_InteralMemory.KeepWord[104]=0;
		}
		rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//先屏蔽定时器3中断，为了防止连续点击点动按钮对送经电机的影响
		timeadd=0;//定时器清0
		duanweiadd=0;
	//	g_InteralMemory.Word[630]=0;///设备状态，联网通信用
	
		Set_Y_Value(5,0);///停机反转再次启动后恢复转向0906
		
		Timer2_Stop();
	
	}
	else
	{
		g_InteralMemory.Bit[8]=0;
	//	g_InteralMemory.Word[630]=1;///设备状态，联网通信用
	}
	///////////////////////
	/////////*************////////////
	if(g_InteralMemory.Bit[6]==1)
	{
		
		////////////开机写入主机频率
		if(g_InteralMemory.KeepBit[602]==0)
		{
			g_InteralMemory.Bit[1]=0;//开机时先清零故障标志位
			g_InteralMemory.Bit[2]=0;
			g_InteralMemory.Bit[3]=0;
			g_InteralMemory.Bit[4]=0;
			g_InteralMemory.Bit[5]=0;
			
		//	rGPLDAT &= 0x3DFF;	//送经电机启动	
		//	rGPLDAT &= 0x1FFF;   //启动
		//	rGPEDAT &= 0xFF7F;	
			Set_Y_Value(11,1);//送经电机启动 主机启动
			Set_Y_Value(7,1);
			g_InteralMemory.KeepBit[602]=1;
			
		//	rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//先屏蔽定时器3中断，为了防止连续点击启动按钮对送经点击的影响
		}
		
	
	/////////////////////////	user_production();//长度累加，高低密度切换//////需要现场调试	
		Status_Monitoring();//经纬状态监控与故障历史
		
	//	j=FX_ReadWord(useless,1);//读变频器监控状态
		////此处预留变频器报警监控
		////////*********************88/////////

	}
	///////////////////////////////////
	if(g_InteralMemory.Bit[7]==1)
	{
		g_InteralMemory.Bit[6]=0;
		g_InteralMemory.Bit[7]=0;////////////////
		
	//	rGPEDAT |= 0x1;
		Set_Y_Value(7,0);
	//	rGPLDAT |= 0x200;//送经电机停止
		rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//先屏蔽定时器3中断，为了防止连续点击点动按钮对送经电机的影响
		timeadd=0;//定时器清0
		timer3_start();
	}
	
	///////////////////////**********外部按钮点动***********////////////////////
	if(g_InteralMemory.Bit[8]==1)
	{
		g_InteralMemory.KeepBit[600]=1;
		g_InteralMemory.Bit[9]=1;
	}
	if((g_InteralMemory.Bit[8]==0) && (g_InteralMemory.Bit[9]==1))
	{
		g_InteralMemory.KeepBit[600]=0;
		g_InteralMemory.Bit[9]=0;
		stopweixianduan=1;
	}
	user_password();
	Banci_DataSave();//班次切换与数据保存
	Jog_debug();//点动调试	
	Status_Monitoring1();//经纬状态监控与故障历史
	
	if(g_InteralMemory.KeepWord[102] >= 65500)
		g_InteralMemory.KeepWord[102]=0;
		
	
	//////////////////////////////////////手动提升部分
	if((g_InteralMemory.KeepBit[608]==1) && (g_InteralMemory.KeepBit[609]==0))//手动提升用
	{
		if(g_InteralMemory.KeepBit[611]==1)//手动提升换向
			Set_Y_Value(5,1);
		else
			Set_Y_Value(5,0);
			
		Pulse_Out_Init();
		PulseOut_1_Start(800,55000);
		g_InteralMemory.KeepBit[609]=1;
	}
	if((g_InteralMemory.KeepBit[608]==0) && (g_InteralMemory.KeepBit[609]==1))
	{
		PulseOut_1_Stop();
		Set_Y_Value(5,0);//非手动提升时默认不输出。
		g_InteralMemory.KeepBit[609]=0;
		

		tmp = rGPBCON  & (~(0x3<< 2));
		rGPBCON = tmp | (0x1<<2);	//set GPB1 as output	
		// 输入输出都反向
		rGPBDAT |= (1<<1);//GPB1
	}
	/////////////////////////////////////////////2014.3.14注释掉中控内容
/*	if(g_InteralMemory.KeepBit[613] == 1)
	{
		g_InteralMemory.KeepBit[613]=0;
		g_InteralMemory.Word[PING_CTRL] =25;
		
	}
	
	if((g_InteralMemory.KeepWord[158]/10) > g_InteralMemory.KeepWord[155])///当目前订单产量达到时提醒
	{
		if(g_InteralMemory.Word[629] < 3)
		{
			g_InteralMemory.Word[PING_CTRL] =27;
			g_InteralMemory.Bit[7]=1;///停机
			if(g_InteralMemory.Bit[300] == 1)
			{
				
				g_InteralMemory.Word[PING_CTRL]=1;
				g_InteralMemory.Word[629]=g_InteralMemory.Word[629]+1;
				g_InteralMemory.Bit[300]=0;
			}
		}
		
	}*/
	
		midu1=g_InteralMemory.KeepWord[3] * 0.677  * g_InteralMemory.KeepWord[7]/100/ g_InteralMemory.KeepWord[5]*g_InteralMemory.KeepWord[6];
		midu=(midu1-(midu1%10))/10 + 1;
		miduxiaoshu1=g_InteralMemory.KeepWord[3] * 0.677  * g_InteralMemory.KeepWord[7]/100/ g_InteralMemory.KeepWord[5]*g_InteralMemory.KeepWord[6];
		miduxiaoshu=10-miduxiaoshu1%10;
	
}



void Save_lishiguzhang(void)//历史故障保存
{
	if((g_InteralMemory.Word[620]==1)&&(g_InteralMemory.Word[621]==0))
	{
		if(g_InteralMemory.KeepWord[92]==0)
		{
			g_InteralMemory.KeepWord[60]=g_InteralMemory.Word[331];
			g_InteralMemory.KeepWord[61]=g_InteralMemory.Word[332];
			g_InteralMemory.KeepWord[62]=g_InteralMemory.Word[333];
			g_InteralMemory.KeepWord[63]=g_InteralMemory.Word[334];
			g_InteralMemory.KeepBit[30]=g_InteralMemory.KeepBit[10];
			g_InteralMemory.KeepWord[92]++;
		}
		else if(g_InteralMemory.KeepWord[92]==1)
		{
			g_InteralMemory.KeepWord[64]=g_InteralMemory.Word[331];
			g_InteralMemory.KeepWord[65]=g_InteralMemory.Word[332];
			g_InteralMemory.KeepWord[66]=g_InteralMemory.Word[333];
			g_InteralMemory.KeepWord[67]=g_InteralMemory.Word[334];
			g_InteralMemory.KeepBit[31]=g_InteralMemory.KeepBit[10];
			g_InteralMemory.KeepWord[92]++;
		}
		else if(g_InteralMemory.KeepWord[92]==2)
		{
			g_InteralMemory.KeepWord[68]=g_InteralMemory.Word[331];
			g_InteralMemory.KeepWord[69]=g_InteralMemory.Word[332];
			g_InteralMemory.KeepWord[70]=g_InteralMemory.Word[333];
			g_InteralMemory.KeepWord[71]=g_InteralMemory.Word[334];
			g_InteralMemory.KeepBit[32]=g_InteralMemory.KeepBit[10];
			g_InteralMemory.KeepWord[92]++;
		}
		else if(g_InteralMemory.KeepWord[92]==3)
		{
			g_InteralMemory.KeepWord[72]=g_InteralMemory.Word[331];
			g_InteralMemory.KeepWord[73]=g_InteralMemory.Word[332];
			g_InteralMemory.KeepWord[74]=g_InteralMemory.Word[333];
			g_InteralMemory.KeepWord[75]=g_InteralMemory.Word[334];
			g_InteralMemory.KeepBit[33]=g_InteralMemory.KeepBit[10];
			g_InteralMemory.KeepWord[92]++;
		}
		else if(g_InteralMemory.KeepWord[92]==4)
		{
			g_InteralMemory.KeepWord[76]=g_InteralMemory.Word[331];
			g_InteralMemory.KeepWord[77]=g_InteralMemory.Word[332];
			g_InteralMemory.KeepWord[78]=g_InteralMemory.Word[333];
			g_InteralMemory.KeepWord[79]=g_InteralMemory.Word[334];
			g_InteralMemory.KeepBit[34]=g_InteralMemory.KeepBit[10];
			g_InteralMemory.KeepWord[92]++;
		}
		else if(g_InteralMemory.KeepWord[92]==5)
		{
			g_InteralMemory.KeepWord[80]=g_InteralMemory.Word[331];
			g_InteralMemory.KeepWord[81]=g_InteralMemory.Word[332];
			g_InteralMemory.KeepWord[82]=g_InteralMemory.Word[333];
			g_InteralMemory.KeepWord[83]=g_InteralMemory.Word[334];
			g_InteralMemory.KeepBit[35]=g_InteralMemory.KeepBit[10];
			g_InteralMemory.KeepWord[92]++;
		}
		else if(g_InteralMemory.KeepWord[92]==6)
		{
			g_InteralMemory.KeepWord[84]=g_InteralMemory.Word[331];
			g_InteralMemory.KeepWord[85]=g_InteralMemory.Word[332];
			g_InteralMemory.KeepWord[86]=g_InteralMemory.Word[333];
			g_InteralMemory.KeepWord[87]=g_InteralMemory.Word[334];
			g_InteralMemory.KeepBit[36]=g_InteralMemory.KeepBit[10];
			g_InteralMemory.KeepWord[92]++;
		}
		else if(g_InteralMemory.KeepWord[92]==7)
		{
			g_InteralMemory.KeepWord[88]=g_InteralMemory.Word[331];
			g_InteralMemory.KeepWord[89]=g_InteralMemory.Word[332];
			g_InteralMemory.KeepWord[90]=g_InteralMemory.Word[333];
			g_InteralMemory.KeepWord[91]=g_InteralMemory.Word[334];
			g_InteralMemory.KeepBit[37]=g_InteralMemory.KeepBit[10];
			g_InteralMemory.KeepWord[92]=0;
		}
		else
			g_InteralMemory.KeepWord[92]=0; 		
		
		g_InteralMemory.Word[620]=0;
		g_InteralMemory.Word[621]=1;
	}
	
}

///////////////////////////////////////////////////////
////////////////切换班次与数据保存/////////////////////
///////////////////////////////////////////////////////
void Banci_DataSave(void)
{
	int j=0;
	
	if(g_InteralMemory.KeepBit[604]==1)//班次切换标识位
	{
		if(g_InteralMemory.KeepWord[0]==1)
		{	/////一班历史数据保存
			while((j!=650623)&&(((j & 0xffff)!=g_InteralMemory.KeepWord[54])||(((j >> 16)& 0xffff)!=g_InteralMemory.KeepWord[55])) && j!=-1) 
			j=KeyBoard(999999,0,0,0);
			
			if(j != -1)
			{
				if(g_InteralMemory.KeepWord[16]==0)//数据保存位置，一共5组数据
				{
					g_InteralMemory.KeepWord[20]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[21]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[160]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[161]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[190]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[191]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[220]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[221]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[22]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[20]=0;
					else
						g_InteralMemory.KeepBit[20]=1;
											
					g_InteralMemory.KeepWord[16]++;
				}
				else if(g_InteralMemory.KeepWord[16]==1)
				{
					g_InteralMemory.KeepWord[23]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[24]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[162]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[163]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[192]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[193]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[222]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[223]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[25]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[21]=0;
					else
						g_InteralMemory.KeepBit[21]=1;
						
					g_InteralMemory.KeepWord[16]++;
				}
				else if(g_InteralMemory.KeepWord[16]==2)
				{
					g_InteralMemory.KeepWord[26]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[27]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[164]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[165]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[194]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[195]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[224]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[225]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[28]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[22]=0;
					else
						g_InteralMemory.KeepBit[22]=1;
						
					g_InteralMemory.KeepWord[16]++;
				}
				else if(g_InteralMemory.KeepWord[16]==3)
				{
					g_InteralMemory.KeepWord[29]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[30]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[166]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[167]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[196]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[197]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[226]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[227]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[31]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[23]=0;
					else
						g_InteralMemory.KeepBit[23]=1;
						
					g_InteralMemory.KeepWord[16]++;
				}
				else if(g_InteralMemory.KeepWord[16]==4)
				{
					g_InteralMemory.KeepWord[32]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[33]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[168]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[169]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[198]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[199]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[228]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[229]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[34]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[24]=0;
					else
						g_InteralMemory.KeepBit[24]=1;
						
					g_InteralMemory.KeepWord[16]=0;
				}
				else if(g_InteralMemory.KeepWord[16] > 4)
				{
					g_InteralMemory.KeepWord[20]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[21]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[160]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[161]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[190]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[191]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[220]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[221]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[22]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[20]=0;
					else
						g_InteralMemory.KeepBit[20]=1;
											
					g_InteralMemory.KeepWord[16]=1;
				}
				
				g_InteralMemory.KeepWord[0]=2;//班次
				g_InteralMemory.KeepWord[100]=0;//切换班次后默认高密度开始
				g_InteralMemory.KeepWord[102]=0;//清空当前加工长度
				g_InteralMemory.KeepBit[11]=0;//清空设备状态清0
				DataSave_yuanzhiji();
			}
		}
		else if(g_InteralMemory.KeepWord[0]==2)
		{/////二班历史数据保存
			while((j!=650623)&&(((j & 0xffff)!=g_InteralMemory.KeepWord[56])||(((j >> 16)& 0xffff)!=g_InteralMemory.KeepWord[57])) && j!=-1) 
			j=KeyBoard(999999,0,0,0);
			
			if(j != -1)
			{
				if(g_InteralMemory.KeepWord[17]==0)//数据保存位置，一共5组数据
				{
					g_InteralMemory.KeepWord[35]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[36]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[170]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[171]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[200]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[201]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[230]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[231]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[37]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[25]=0;
					else
						g_InteralMemory.KeepBit[25]=1;
						
					g_InteralMemory.KeepWord[17]++;
				}
				else if(g_InteralMemory.KeepWord[17]==1)
				{
					g_InteralMemory.KeepWord[38]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[39]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[172]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[173]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[202]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[203]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[232]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[233]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[40]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[26]=0;
					else
						g_InteralMemory.KeepBit[26]=1;
						
					g_InteralMemory.KeepWord[17]++;
				}
				else if(g_InteralMemory.KeepWord[17]==2)
				{
					g_InteralMemory.KeepWord[41]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[42]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[174]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[175]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[204]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[205]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[234]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[235]=g_InteralMemory.KeepWord[154];//订单号				
					g_InteralMemory.KeepWord[43]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[27]=0;
					else
						g_InteralMemory.KeepBit[27]=1;
						
					g_InteralMemory.KeepWord[17]++;
				}
				else if(g_InteralMemory.KeepWord[17]==3)
				{
					g_InteralMemory.KeepWord[44]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[45]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[176]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[177]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[206]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[207]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[236]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[237]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[46]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[28]=0;
					else
						g_InteralMemory.KeepBit[28]=1;
					
					g_InteralMemory.KeepWord[17]++;
				}
				else if(g_InteralMemory.KeepWord[17]==4)
				{
					g_InteralMemory.KeepWord[47]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[48]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[178]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[179]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[208]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[209]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[238]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[239]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[49]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[29]=0;
					else
						g_InteralMemory.KeepBit[29]=1;
					
					g_InteralMemory.KeepWord[17]=0;
				}
				else if(g_InteralMemory.KeepWord[17] > 4)
				{
					g_InteralMemory.KeepWord[35]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[36]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[170]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[171]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[200]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[201]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[230]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[231]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[37]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[25]=0;
					else
						g_InteralMemory.KeepBit[25]=1;
						
					g_InteralMemory.KeepWord[17]=1;
				}
				
				g_InteralMemory.KeepWord[0]=3;//班次
				g_InteralMemory.KeepWord[100]=0;//切换班次后默认高密度开始
				g_InteralMemory.KeepWord[102]=0;//清空当前加工长度
				g_InteralMemory.KeepBit[11]=0;//清空设备状态
				DataSave_yuanzhiji();
			}
		}
		else
		{/////三班历史数据保存
			while((j!=650623)&&(((j & 0xffff)!=g_InteralMemory.KeepWord[52])||(((j >> 16)& 0xffff)!=g_InteralMemory.KeepWord[53])) && j!=-1) 
			j=KeyBoard(999999,0,0,0);
			
			if(j != -1)
			{
				if(g_InteralMemory.KeepWord[18]==0)//数据保存位置，一共5组数据
				{
					g_InteralMemory.KeepWord[120]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[121]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[180]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[181]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[210]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[211]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[240]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[241]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[122]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[40]=0;
					else
						g_InteralMemory.KeepBit[40]=1;
						
					g_InteralMemory.KeepWord[18]++;
				}
				else if(g_InteralMemory.KeepWord[18]==1)
				{
					g_InteralMemory.KeepWord[123]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[124]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[182]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[183]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[212]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[213]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[242]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[243]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[125]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[41]=0;
					else
						g_InteralMemory.KeepBit[41]=1;
						
					g_InteralMemory.KeepWord[18]++;
				}
				else if(g_InteralMemory.KeepWord[18]==2)
				{
					g_InteralMemory.KeepWord[126]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[127]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[184]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[185]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[214]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[215]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[244]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[245]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[128]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[42]=0;
					else
						g_InteralMemory.KeepBit[42]=1;
						
					g_InteralMemory.KeepWord[18]++;
				}
				else if(g_InteralMemory.KeepWord[18]==3)
				{
					g_InteralMemory.KeepWord[129]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[130]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[186]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[187]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[216]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[217]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[246]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[247]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[131]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[43]=0;
					else
						g_InteralMemory.KeepBit[43]=1;
					
					g_InteralMemory.KeepWord[18]++;
				}
				else if(g_InteralMemory.KeepWord[18]==4)
				{
					g_InteralMemory.KeepWord[132]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[133]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[188]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[189]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[218]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[219]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[248]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[249]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[134]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[44]=0;
					else
						g_InteralMemory.KeepBit[44]=1;
					
					g_InteralMemory.KeepWord[18]=0;
				}
				else if(g_InteralMemory.KeepWord[18] > 4)
				{
					g_InteralMemory.KeepWord[120]=g_InteralMemory.Word[331];//时间
					g_InteralMemory.KeepWord[121]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[180]=g_InteralMemory.Word[333];//小时
					g_InteralMemory.KeepWord[181]=g_InteralMemory.Word[334];//分钟
					g_InteralMemory.KeepWord[210]=g_InteralMemory.KeepWord[151];//工号
					g_InteralMemory.KeepWord[211]=g_InteralMemory.KeepWord[152];//工号
					g_InteralMemory.KeepWord[240]=g_InteralMemory.KeepWord[153];//订单号
					g_InteralMemory.KeepWord[241]=g_InteralMemory.KeepWord[154];//订单号
					g_InteralMemory.KeepWord[122]=g_InteralMemory.KeepWord[102];//生产长度
					if(g_InteralMemory.KeepBit[11]==0)//保存设备状态
						g_InteralMemory.KeepBit[40]=0;
					else
						g_InteralMemory.KeepBit[40]=1;
						
					g_InteralMemory.KeepWord[18]=1;
				}
				
				g_InteralMemory.KeepWord[0]=1;//班次
				g_InteralMemory.KeepWord[100]=0;//切换班次后默认高密度开始
				g_InteralMemory.KeepWord[102]=0;//清空当前加工长度
				g_InteralMemory.KeepBit[11]=0;//清空设备状态
				DataSave_yuanzhiji();
			}
		}
		g_InteralMemory.KeepBit[604]=0;	
		
		/////
		if(j != -1)
		{
			g_InteralMemory.Word[PING_CTRL] =1;///换完班次后自动跳转回工作界面
			////换班后  新工号导入
			g_InteralMemory.KeepWord[151]=g_InteralMemory.Word[622];
			g_InteralMemory.KeepWord[152]=g_InteralMemory.Word[623];
		}
		else 
			g_InteralMemory.Word[PING_CTRL] =1;
	}
}

/////////////////////////////////////////////////////////////
//////////////////点动调试////////////////////////
/////////////////////////////////////////////////////////////
void Jog_debug(void)
{
	char SendSucceed,useless[2],i;
	if((g_InteralMemory.KeepBit[600]==1)&&(g_InteralMemory.KeepBit[601]==0))
	{
	//	rGPLDAT &= 0x3DFF;	//送经电机启动
	//	rGPLDAT &=0x1FFF;
		g_InteralMemory.Bit[6]=0;
	//	rGPLDAT &= 0x1DFF;	
		Set_Y_Value(11,1);//送经电机启动 主机启动
		Set_Y_Value(7,1);
		
		rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//先屏蔽定时器3中断，为了防止连续点击启动按钮对送经点击的影响
		timeadd=0;
		
		g_InteralMemory.Bit[1]=0;//开机时先清零故障标志位
		g_InteralMemory.Bit[2]=0;
		g_InteralMemory.Bit[3]=0;
		g_InteralMemory.Bit[4]=0;
		g_InteralMemory.Bit[5]=0;
		
		g_InteralMemory.KeepWord[100]=2;
		g_InteralMemory.KeepBit[601]=1;

	}
	if((g_InteralMemory.KeepBit[600]==0)&&(g_InteralMemory.KeepBit[601]==1))
	{
		Set_Y_Value(7,0);

//		rGPLDAT |= 0x200;//送经电机停止
		timer3_start();
		
		g_InteralMemory.KeepWord[100]=0;
		g_InteralMemory.KeepBit[601]=0;

	}
	//////////////////
//////////////////////////////////	Status_Monitoring();//经纬状态监控
}

///////////////////////////////////////////////////////////////////////
////经纬状态监控
///////////////////////////////////
void Status_Monitoring(void)
{	
	char useless[2],i,SendSucceed;
	
	if(Get_X_Value(7))//A区经线检测
		g_InteralMemory.Bit[1]=1;
	//else
	//	g_InteralMemory.Bit[1]=0;
	////////////////
	if(Get_X_Value(8))//B区经线检测
		g_InteralMemory.Bit[2]=1;
	//else
	//	g_InteralMemory.Bit[2]=0;
	////////////////////
	if(Get_X_Value(9))//C区经线检测
		g_InteralMemory.Bit[3]=1;
	//else
	//	g_InteralMemory.Bit[3]=0;
	/////////////////////
	if(Get_X_Value(10))//D区经线检测
		g_InteralMemory.Bit[4]=1;
	//else
	//	g_InteralMemory.Bit[4]=0;

	///////////////////////////////////////
	///设备状态显示以及历史故障的保存
	if(( (g_InteralMemory.Bit[1]==1) || (g_InteralMemory.Bit[2]==1) || (g_InteralMemory.Bit[3]==1) || (g_InteralMemory.Bit[4]==1)))
	 {
		 g_InteralMemory.KeepBit[10]=1;//显示"经线故障"
		 g_InteralMemory.Word[620]=1;//故障保存标志位
		 g_InteralMemory.KeepBit[11]=1;//用于保存设备的故障标志位
		 Save_lishiguzhang(); 
		
		if(( g_InteralMemory.KeepBit[605]==0))//停用自动停经时不故障停机
		{
			Set_Y_Value(7,0);
			rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//先屏蔽定时器3中断，为了防止连续点击启动按钮对送经电机的影响
			timer3_start();
			g_InteralMemory.KeepWord[95]=1;
			
			g_InteralMemory.Word[630]=2;///设备状态，联网通信用
		}
		stopweixianduan=1;
	
		 
	 }
		 
	if((g_InteralMemory.Bit[5]==1))
	 {
		 g_InteralMemory.KeepBit[10]=2;//显示"纬尾"
		 g_InteralMemory.Word[620]=1;//故障保存标志位
		 g_InteralMemory.KeepBit[11]=1;//用于保存设备的故障标志位
		 Save_lishiguzhang();
	//FX_WriteWord(useless,2,0);//故障时主机停机
		 if( (g_InteralMemory.KeepBit[606]==0))//停用自动停纬时不故障停机
		 {
			Set_Y_Value(7,0);//故障时主机停机
			rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//先屏蔽定时器3中断，为了防止连续点击启动按钮对送经电机的影响
			timer3_start();
			g_InteralMemory.KeepWord[95]=1;
			g_InteralMemory.Word[630]=2;///设备状态，联网通信用
		 }
		// g_InteralMemory.KeepWord[95]=1;
		stopweixianduan=1;
		
	 }
	 
	if((g_InteralMemory.Bit[5]==2) && (stopweixianduan==0) )	 
	 {
		 g_InteralMemory.KeepBit[10]=3;//显示"纬断"
		 g_InteralMemory.Word[620]=1;//故障保存标志位
		 g_InteralMemory.KeepBit[11]=1;//用于保存设备的故障标志位
		 Save_lishiguzhang();
	//FX_WriteWord(useless,2,0);//故障时主机停机
		 if( (g_InteralMemory.KeepBit[606]==0))//停用自动停纬时不故障停机
		 {
			Set_Y_Value(7,0);//故障时主机停机
			rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//先屏蔽定时器3中断，为了防止连续点击启动按钮对送经电机的影响
			timer3_start();
			g_InteralMemory.KeepWord[95]=1;
			g_InteralMemory.Word[630]=2;///设备状态，联网通信用
		 }
		// g_InteralMemory.KeepWord[95]=1;
		
	 }
	if(g_InteralMemory.KeepWord[95] != 1)
	 {
		 g_InteralMemory.KeepBit[10]=0;
		 g_InteralMemory.Word[621]=0;//故障保存标志位
		 
	//	 if(g_InteralMemory.KeepWord[95]==1)
		 {
		 	g_InteralMemory.KeepWord[95]=0;
		
			
			//rGPLDAT &= 0x1FFF;//开机????????????????
			/////////////****************?????????????
			/////////////****************?????????????
			//////////////待考虑！！！！！////////////
			/////////////****************?????????????
			/////////////****************?????????????
		
		 }
	 }
	
	
}


void Status_Monitoring1(void)
{	
	
	if(Get_X_Value(7))//A区经线检测
		g_InteralMemory.Bit[1]=1;
	//else
	//	g_InteralMemory.Bit[1]=0;
	////////////////
	if(Get_X_Value(8))//B区经线检测
		g_InteralMemory.Bit[2]=1;
	//else
	//	g_InteralMemory.Bit[2]=0;
	////////////////////
	if(Get_X_Value(9))//C区经线检测
		g_InteralMemory.Bit[3]=1;
	//else
	//	g_InteralMemory.Bit[3]=0;
	/////////////////////
	if(Get_X_Value(10))//D区经线检测
		g_InteralMemory.Bit[4]=1;
	//else
	//	g_InteralMemory.Bit[4]=0;

	///////////////////////////////////////
	///经纬线故障 送经送经电机自动延时停机
	if( ((g_InteralMemory.Bit[1]==1) || (g_InteralMemory.Bit[2]==1) || (g_InteralMemory.Bit[3]==1) || (g_InteralMemory.Bit[4]==1)))// && (jingxianguzhang==0)  )
	 {
		g_InteralMemory.KeepBit[10]=1;//显示"经线故障"
		
		if(g_InteralMemory.KeepWord[95]==1)
		{
		//	rGPLDAT |= 0x2000;//故障时主机停机
		//	rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//先屏蔽定时器3中断，为了防止连续点击启动按钮对送经电机的影响
		//	timer3_start();
			g_InteralMemory.KeepWord[95]=0;
		}
		stopweixianduan=1;
		
	 }	
	 else if((g_InteralMemory.Bit[5]==1))
	 {
	 	g_InteralMemory.KeepBit[10]=2;//显示"纬尾故障"
	 	stopweixianduan=1;
	 	
	 }
	 else if((g_InteralMemory.Bit[5]==2)  && (stopweixianduan==0))//纬断
	 {
	 	g_InteralMemory.KeepBit[10]=3;//显示"纬断故障"
	 //	rGPLDAT |= 0x2000;//故障时主机停机
	 //	rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//先屏蔽定时器3中断，为了防止连续点击启动按钮对送经电机的影响
	//	timer3_start();
		
	 }
	
	 else
	 {
		 g_InteralMemory.KeepBit[10]=0;
		 g_InteralMemory.Bit[5]=0;
		 g_InteralMemory.Word[621]=0;//故障保存标志位
		 
		 g_InteralMemory.KeepWord[95]=0;
	 }
	
	
}


	


void user_password(void)
{
	//////////////////////////////////////////
	////密码匹配	
	char SrcNumCheck;
	SrcNumCheck=CurrentScrNum;
	if(g_InteralMemory.Bit[600]==1)
	{
		g_InteralMemory.Bit[600]=0;
		if(g_InteralMemory.Word[600]==g_InteralMemory.Word[602] 
			&&  g_InteralMemory.Word[601]==g_InteralMemory.Word[603])
		{
			g_InteralMemory.KeepWord[50]=g_InteralMemory.Word[600];
			g_InteralMemory.KeepWord[51]=g_InteralMemory.Word[601];
			if(SrcNumCheck==8)
				g_InteralMemory.Word[PING_CTRL] =9;
			else
				g_InteralMemory.Word[PING_CTRL] =20;
		}
		else
		{
			if(SrcNumCheck==8)
				g_InteralMemory.Word[PING_CTRL] =3;
			else
				g_InteralMemory.Word[PING_CTRL] =15;
		}
	}
	if(g_InteralMemory.Bit[601]==1)
	{
		g_InteralMemory.Bit[601]=0;
		if(g_InteralMemory.Word[604]==g_InteralMemory.Word[606] 
			&&  g_InteralMemory.Word[605]==g_InteralMemory.Word[607])
		{
			g_InteralMemory.KeepWord[52]=g_InteralMemory.Word[604];
			g_InteralMemory.KeepWord[53]=g_InteralMemory.Word[605];
			if(SrcNumCheck==8)
				g_InteralMemory.Word[PING_CTRL] =9;
			else
				g_InteralMemory.Word[PING_CTRL] =20;
		}
		else
		{
			if(SrcNumCheck==8)
				g_InteralMemory.Word[PING_CTRL] =3;
			else
				g_InteralMemory.Word[PING_CTRL] =15;
		}
	}
	
	if(g_InteralMemory.Bit[605]==1)
	{
		g_InteralMemory.Bit[605]=0;
		if(g_InteralMemory.Word[612]==g_InteralMemory.Word[614] 
			&&  g_InteralMemory.Word[613]==g_InteralMemory.Word[615])
		{
			g_InteralMemory.KeepWord[56]=g_InteralMemory.Word[612];
			g_InteralMemory.KeepWord[57]=g_InteralMemory.Word[613];
			if(SrcNumCheck==8)
				g_InteralMemory.Word[PING_CTRL] =9;
			else
				g_InteralMemory.Word[PING_CTRL] =20;
		}
		else
		{
			if(SrcNumCheck==8)
				g_InteralMemory.Word[PING_CTRL] =3;
			else
				g_InteralMemory.Word[PING_CTRL] =15;
		}
	}
	////////////////////
	if(g_InteralMemory.Bit[602]==1)
	{
		g_InteralMemory.Bit[602]=0;
		if(g_InteralMemory.Word[608]==g_InteralMemory.Word[610] 
			&&  g_InteralMemory.Word[609]==g_InteralMemory.Word[611])
		{
			g_InteralMemory.KeepWord[54]=g_InteralMemory.Word[608];
			g_InteralMemory.KeepWord[55]=g_InteralMemory.Word[609];
			if(SrcNumCheck==8)
				g_InteralMemory.Word[PING_CTRL] =9;
			else
				g_InteralMemory.Word[PING_CTRL] =20;
		}
		else
		{
			if(SrcNumCheck==8)
				g_InteralMemory.Word[PING_CTRL] =3;
			else
				g_InteralMemory.Word[PING_CTRL] =15;
		}
	}
}
