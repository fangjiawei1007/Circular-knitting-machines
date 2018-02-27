
#define	GLOBAL_CLK		1

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
#include "..\inc\usbdev.h"

//#include "..\inc\app.h"
void App_Init(void);
void App_Operation(void);

/*
extern char Image$$RO$$Limit[];
extern char Image$$RO$$Base[];
extern char Image$$RW$$Limit[];
extern char Image$$RW$$Base[];
extern char Image$$ZI$$Limit[];
extern char Image$$ZI$$Base[];
*/


GLOBALVAR g_GlobalVar;
INTERALMEMORY g_InteralMemory;
SYSTEMCONFIG g_SystemConf;
SYSTEMINFO g_SystemInfo;


TIMER2_V g_Timer2Uart;
TIMER2_V g_Timer2Virtual[MaxTimer];
U8 g_UartTimeOut = FALSE;


INT16U CurrentScrNum;
INT16U PreScrNum;

INT16U g_PlcCtrlNum,g_PlcStaNum,g_PlcCtrLen,g_PlcStaLen,g_StarScr,g_TotalSrceenNum;
char g_PlcCtrStr[5],g_PlcStaStr[5];

int WatchRefresh[RefreshNum*2]={0};
int NumDispRefresh[RefreshNum]={0};
int StickChartRefresh[RefreshNum][2]={0};
int SetValueRefresh[RefreshNum]={0};
int MultiStateRefresh[RefreshNum][2]={0};
int KeepButtonRefresh[RefreshNum][2]={0};
int KeepButton2Refresh[RefreshNum][2]={0};
int TimerButtonRefresh[RefreshNum]={0};
int OscButtonRefresh[RefreshNum]={0};

int SetValue_PreValue[RefreshNum]={-1};
int NumDisp_PreValue[RefreshNum]={-1};

// 100513 小键盘切换
U8 g_KeyBoardChange = FALSE;


INT16U Current_Scr_Block;
INT8U  Current_Scr_Page;
INT16U Next_Scr_Block;   
INT8U  Next_Scr_Page;
INT16U IndexBlock;             //every screen's block and page index table
INT8U  ScreenDataBuffer[620000];	// 400000  -->  480*640*2=614400
INT32U ScrBufferOffset;

char PlcType;

INT8U IndexBlockBuffer[2048]={0};
//INT8U IndexBlockBuffer[2048];
INT16U IndexBlockOffset;


int adcpsr=0;			// ADCPSR		
int xtal=12000000;		// X-tal clock
//int adcclock=5000000;	// ADC clock
int adcclock=1330000;	// ADC clock  1.33M
//volatile int xdata, ydata;
U8 ValidCount = 0;
int xdata=0, ydata=0;
U8 bTSP_DownFlag = FALSE;
U8 bCalibrateFlag = FALSE;
U8 sampleCount = 0;
int xdataSum=0, ydataSum=0;
INT16U X_result=0,Y_result=0;
INT8U flag = 0;

U8 g_SampleFlag = FALSE;


U8 differCount[24];	// 24 路IN 滤波处理用

U8 com_frequence = 0;	// 指示灯频率控制

U8 g_com_Flag = 0;	// 通讯标志

U8 g_download_Flag = 0;	// 下载画面标志



//TIMER2_V g_Timer2Virtual[MaxTimer];




extern BOOL bLARGEBLOCK;


void __rt_lib_init(void);


char chushihua;
/******************************************************************************
【功能说明】系统主函数
******************************************************************************/
void Main(void)
//int Main(U32 RstStat)
{
	U32 mpll_val = 0 ;

	//U32 BlkIndex;
	//U32 PageIndex;
	//U8 data[2048];
	
	//U8 * writeBuffer;
	//UINT8 * readBuffer;
	//UINT8 readdata[2048];
	
	//int num;
	//int size;
	int i, j, k, m;
	U8 refresh_delay,n,delay;
	int Block;
	//int numTTT = 500000;
	
	//char str[20];
	//sprintf(str,"%f",(float)1.234);

	#if ADS12   
	__rt_lib_init(); //for ADS 1.2 初始化C 库函数
	#endif
	

	mpll_val = (400<<14)|(3<<5)|(1);	// 800 400 3 1
	ChangeMPllValue((mpll_val>>14)&0x3ff, (mpll_val>>5)&0x3f, mpll_val&7);
	ChangeClockDivider(12, 13, 12, 12);	// ARM: 400  HCLK:133	  PCLK:66
	//ChangeClockDivider(13, 13, 12, 12);	// ARM: 266  HCLK:133	  PCLK:66
	cal_cpu_bus_clk();


#if 1			//bank0 modified to RO_START
	MMU_Init();	//MMU should be reconfigured or turned off for the debugger, 
	//After downloading, MMU should be turned off for the MMU based program,such as WinCE.	
#else
	MMU_EnableICache();
	UsbMain();
#endif

	// Initial
	Port_Init();
	Isr_Init();
	//Lcd_Port_Init();
	Lcd_Init();
	Lcd_Port_Init();

	//DisplaySystemScr(FALSE);
	
	Ts_Init();		//Ts_PowerOn();	// 系统菜单选择后才启用触摸采样
	DataSave_Init();
	
	NF_Init();
	if (ReadFlashID() != NandFlashID)
	{
		Uart_SendString("NAND Flash is not supported!!\r\n");
		while(1);
	}	
	Check_Invalid_Block();

	// USB device
	//Usb_Init();
	//AllocateUSBDEV();
	//Usbdmain();
	//rINTMSK1&=~(BIT_USBD);
	//UsbDev_On();


	Beep(1);
	for(i=0;i<BEEP_DELAY;i++);
	Beep(0);

	
	//Test_Adc();		// AD 测试，作为通用的AD 口


#if Nand_SJF
	/* for(i=1; i<1024; i++)
	{
		if(TRUE != EraseBlock(i)) //连续坏块时怎么处理?
		{
			MarkBadBlk(i);
			i++;			
			continue;
		}
	}
	Check_Invalid_Block(); */

	//ReadPage(5, 0, readdata);
	
	Uart_Init_DNW( 0,115200 );
	SDRAM_test(SJF_Ram_Base, SJF_Ram_END);
	K9f1g08_Menu();
	//while(1);
#endif

	GlobalData_WriteZero();
	
	LoadSysConf();
	GetSystenInfo();
	GetScrIndex();

	DataSave_Load();
	
///	Block = Logical_To_Physical_Block(NAND_DATA_SAVE);
//	EraseBlock(Block);///////为掉电数据保存做准备2014.1.7wangliang

	//if(g_GlobalVar.ScrSaveTime)
	//	ScrSave_Init();                 //屏保初始化
	//Timer2_Init();


	if(UART_PrintStr && 0x23==rUBRDIV1) // 115200
	{
		Uart_SendByte('\n');
		Uart_Printf("<***************************************>\n");
		Uart_Printf("               上海三碁\n");
		Uart_Printf("              http://www.sanch.net\n");
		Uart_Printf("<***************************************>\n");
	}


	//Test_Lcd();
	//DisplaySystemScr(FALSE);
	//Lcd_Pwren_Init_On_Off(0);
	//Lcd_Pwren_Init_On_Off(1);

	

	for(i=0; i<RefreshNum; i++)
	{
		WatchRefresh[2*i] = WatchRefresh[2*i+1] = 0; 
		NumDispRefresh[i] = 0;
		StickChartRefresh[i][0] = 0;
		StickChartRefresh[i][1] = 0;
		SetValueRefresh[i] = 0;
		MultiStateRefresh[i][0] = 0;
		MultiStateRefresh[i][1] = 0;
		KeepButtonRefresh[i][0] = 0;
		KeepButtonRefresh[i][1] = 0;
		KeepButton2Refresh[i][0] = 0;
		KeepButton2Refresh[i][1] = 0;
		TimerButtonRefresh[i] = 0;
		OscButtonRefresh[i] = 0;
		
		SetValue_PreValue[i] = -1;
		NumDisp_PreValue[i] = -1;
	}

	g_KeyBoardChange = FALSE;

	bTSP_DownFlag = FALSE;
	bCalibrateFlag = FALSE;
	flag = 0; 
	g_GlobalVar.Keep_flag=0;
	g_GlobalVar.Screen_Flag=1;	//允许切换画面的标志


	i2c_init();
	//RTC_Init();	// 开机的时候把时钟芯片中的当前时间写入  芯片内部RTC 中

#if	Encrypt
	Encrypt_Init();
#elif ProductTest
	/////////// 成品测试初始化////////////////
	Test_Init();
#elif AppMode
	App_Init();
#endif
	
	App_Init();//触摸屏I/O相关初始化
	yuanzhiji_innt();//与圆织机有关的数据初始化

//================  初始化完成======================================


	//if(bTSP_DownFlag)		//按住
	//if((rADCUPDN&0x1) == 0x1)			// Stylus Down
	if( !(rADCDAT0 & (1 << 15)) && !(rADCDAT1 & (1 << 15)) )
	{
		for(i=0;i<BEEP_DELAY;i++);		// 500000
		if( !(rADCDAT0 & (1 << 15)) && !(rADCDAT1 & (1 << 15)) )
		{
			DisplaySystemScr(FALSE);

			// 系统菜单时启用usb device
		//	UsbDev_On();
			
			// 系统菜单时启用usb device
			//Usb_Init();
			//AllocateUSBDEV();
			//Usbdmain();
			//rINTMSK1&=~(BIT_USBD);			
		}		
		else		//防止触摸膜抖动影响060331
		{
			Run();
			// 停用usb device
			//rINTMSK1 |= (BIT_USBD);			

		}
	}
	else
	{
		Run();
	}
	
	Ts_PowerOn();	// 系统菜单选择后才启用触摸采样


	i = j = k = m = 0;
	
	n=0;
	chushihua=0;
	if(g_InteralMemory.KeepWord[110]>1)//语言初始化
		g_InteralMemory.KeepWord[110]=0;
		
	while(1)
	{
		i++;
		j++;
		k++;
		m++;


		/*if(g_SampleFlag)
		{
			Sampling();
		}
		if (bTSP_DownFlag)
		{
			continue;
		}*/
		
		/////////////////////////////////////////////////////开机显示初始化页面	
			
		if((CurrentScrNum!=0) & (chushihua==0) & (CurrentScrNum!=11) & (CurrentScrNum!=0xff04) & (CurrentScrNum!=0xff05))
		{
			chushihua=1;
			for(delay=0;delay<35;delay++)
				for(i=0;i<BEEP_DELAY;i++);
			
			if(g_InteralMemory.KeepWord[150]==2)
			{
				//Write_F0_04();
				//for(i=0;i<20;i++)
				//for(j=0;j<500000;j++);
				Write_Pinlv();
			}
			if(g_InteralMemory.KeepWord[150]==10)
			{
				//Write_F0_04();
				//for(i=0;i<20;i++)
				//for(j=0;j<500000;j++);
				Write_Pinlv_hailipu();
			}
			
			if(g_InteralMemory.KeepWord[110]==0)
				g_InteralMemory.Word[PING_CTRL] =1;
			else
				g_InteralMemory.Word[PING_CTRL] =7;
		}
		else
		{
			chushihua=1;
		}
		if(CurrentScrNum==0)
			chushihua=0;
		
		if(CurrentScrNum==1)
			g_InteralMemory.KeepWord[110]=0;
		else if(CurrentScrNum==7)
			g_InteralMemory.KeepWord[110]=1;
//////////////////////////////////////////////////////

		if (flag == 1)
		{
			KeyScan();
			flag = 0;
		}
		
		if (i > 500)
		{
			i = 0;
			if(!bTSP_DownFlag)
				RefreshButtonUp();
		}

		if (j > 2000)  // 500*4
		{
			j = 0;

			if(CurrentScrNum>0 && CurrentScrNum<255)
			{
#if	Encrypt
				Encrypt_Operation();
			
#elif ProductTest
				////////////// 成品测试////////////////
				Test_Run();
				Test_RTC_SetTime();  // 按钮控制
				Test_RTC_ReadTime(); 
				/*if (10 == g_InteralMemory.Word[PING_STATE])	// 时间设置显示画面
				{
					//FeedWacthDog();					
					Test_Write_DateTime();	// 按钮控制
					Test_Read_DateTime();	// 考虑用定时中断读取
										// IIC中断占用CPU 资源
				}*/	
#elif AppMode
				App_Operation();
#endif
			}
	

			if(g_GlobalVar.Screen_Flag==1)
			{
				if(CurrentScrNum>0 && CurrentScrNum<255)
					Check_Ctrl_State();
			}

			RefreshWatchDispButton();
			if (flag == 1)
			{
				KeyScan();
				flag = 0;
			}

			RefreshStickChart();
			if (flag == 1)
			{
				KeyScan();
				flag = 0;
			}
			
			RefreshMultiState();
			if (flag == 1)
			{
				KeyScan();
				flag = 0;
			}
			
			RefreshKeepButton();
			if (flag == 1)
			{
				KeyScan();
				flag = 0;
			}

			RefreshKeep2Button();
			if (flag == 1)
			{
				KeyScan();
				flag = 0;
			}

			if(!bTSP_DownFlag)
				RefreshButtonUp();


			if(TRUE == g_com_Flag)
			{
				g_com_Flag = FALSE;
				refresh_delay = 1;
			}
			else
			{
				//refresh_delay = 20;
				refresh_delay = 1;
			}

			//if (k > 500000) 	// 2000*250
			if (k > 2000*refresh_delay) 
			{
				k = 0;
				//FeedWacthDog();
				
				RefreshNumDispButton();
				if (flag == 1)
				{
					KeyScan();
					flag = 0;
				}
			
				RefreshSetValue();
				if (flag == 1)
				{
					KeyScan();
					flag = 0;
				}

				if(g_Timer2Virtual[0].Timer_Cur >= g_Timer2Virtual[0].Timer_Set)
				{
					g_Timer2Virtual[0].Timer_Cur = 0;
					g_Timer2Virtual[0].Timer_Status = TIMER_STOP;

					// 系统菜单时启用usb device
					UsbDev_On();
				}

			}
			
			if (m > 500000) 	// 2000*250
			{
				m = 0;
				
				FeedWacthDog();
			}
			
			//////////////*******************//////////////////
			yuanzhiji_run();
			/*if((g_InteralMemory.Bit[500] == 0) && (g_InteralMemory.Bit[501] == 1))
			{
				for(nn=0;nn<1000;nn++);
				FX_WriteWord(useless,1,g_InteralMemory.KeepWord[2]);//触摸后转速恢复
				
				g_InteralMemory.Bit[501]=0;
			}*/
			
			////////联网数据采集
			/*if(g_InteralMemory.KeepBit[612] == 1)
			{
				yuanzhiji_net();
			}*/
			
			if(g_InteralMemory.Bit[603]==1 )
			{
				g_InteralMemory.Bit[603]=0;
				RefreshButtonUp();
				DisplaySystemScr(FALSE);
			}
			
		}
	}
}

