/////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////͹��ʽԲ֯����ع��ܺ���///////////////////////
/////////////////By Wang Liang   �Ϻ�����ѧ��������ʵ����  2012.3 /////////////         
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
char stopweixianduan;//ͣ�����㶯ͣ��������ͣ������γ���ź�

char miduxiaoshu1,miduxiaoshu;
char Pulseout;
U16 midu,midu1;
void Status_Monitoring1(void);
extern char timeadd;
extern char duanweiadd;
extern int CurrentScrNum;
void yuanzhiji_innt(void)
{
	g_InteralMemory.Bit[158]=0;//ʱ���趨��ť��־λ
	g_InteralMemory.Bit[6]=0;//�ⲿ��ť��ͣ
	g_InteralMemory.Bit[7]=0;
	g_InteralMemory.KeepBit[604]=0;//����л���־λ
	g_InteralMemory.KeepBit[600]=0;//�㶯�������Ա�־λ
	g_InteralMemory.KeepBit[601]=0;
	g_InteralMemory.KeepBit[602]=0;//
	
	g_InteralMemory.KeepBit[608]=0;//�ֶ�������־λ
	g_InteralMemory.KeepWord[15]=0;//
	if(g_InteralMemory.KeepWord[16] > 10)
		g_InteralMemory.KeepWord[16]=0;//���ݱ���λ�ñ�־
	g_InteralMemory.KeepWord[101]=0;//��ʼ�������ۼ�ֵ
	g_InteralMemory.KeepWord[100]=0;
	g_InteralMemory.KeepBit[10]=0;//�豸״̬
	
	if(g_InteralMemory.KeepWord[104] > 1)
		g_InteralMemory.KeepWord[104]=0;//γ�ϱ�־λ
	
	g_InteralMemory.KeepBit[11]=0;//��ձ����豸���ϱ�־λ
	
	if(g_InteralMemory.KeepWord[92] > 7)
		g_InteralMemory.KeepWord[92]=0;
	if(g_InteralMemory.KeepWord[2] > 200)//Ƶ�ʳ�ʼ��
		g_InteralMemory.KeepWord[2]=0;
		
	if(g_InteralMemory.KeepWord[111] > 200)//����ͨ�Ŵ���ʱ�ָ�֮ǰ��ת������
		g_InteralMemory.KeepWord[111]=0;
		
		
	if((g_InteralMemory.KeepWord[102]>= 65500) | (g_InteralMemory.KeepWord[102] < 0))//�ӹ����ȳ�ʼ��
	{
		g_InteralMemory.KeepWord[102]=0;
	}
	
	if(g_InteralMemory.KeepWord[0]>3)
		g_InteralMemory.KeepWord[0]=1;

//	g_InteralMemory.KeepWord[56]=0;	//��ʱ��ʹ�ñ�־λ
	
		
	if(g_InteralMemory.KeepBit[605] > 1)//�Ƿ�ʹ���Զ�ͣ��
		g_InteralMemory.KeepBit[605]=0;
	if(g_InteralMemory.KeepBit[606] > 1)//�Ƿ�ʹ���Զ�ͣγ
		g_InteralMemory.KeepBit[606]=0;
	
	g_InteralMemory.KeepWord[95]=0;
		
	if(g_InteralMemory.KeepWord[7] > 300)//���ӳ��ֱ�����ϵ����Ϊ�ٷֱ�
		g_InteralMemory.KeepWord[7]=100;
		
	if(g_InteralMemory.KeepBit[610] >= 2)//��������ѡ��
		g_InteralMemory.KeepBit[610]=1; //1.����2.����
	
	g_InteralMemory.KeepBit[609]=0;
	if(g_InteralMemory.KeepWord[150] > 100)
		g_InteralMemory.KeepWord[150]=1;
		
	if(g_InteralMemory.KeepWord[5] > 1000)
		g_InteralMemory.KeepWord[5]=45;
	if(g_InteralMemory.KeepWord[6] > 1000)
		g_InteralMemory.KeepWord[6]=18;
	if(g_InteralMemory.KeepWord[8] > 1000)
		g_InteralMemory.KeepWord[8]=100;
	if(g_InteralMemory.KeepWord[13] > 100)/////�;���ʱ��ʼ��
		g_InteralMemory.KeepWord[13]=10;
		
	//if(g_InteralMemory.KeepWord[260] > 1000)/////�ܶ�΢��ϵ��
	//	g_InteralMemory.KeepWord[260]=1;
	if(g_InteralMemory.KeepBit[620] > 2)///ͣ���ܶȲ�������
		g_InteralMemory.KeepBit[620]=0;
	
	g_InteralMemory.Word[630]=1;////�豸��ת״̬ 0 ������  1 �޹���ͣ��  2 ����ͣ��
	g_InteralMemory.Word[629]=0;///�����������Ѵ���
	
	if(g_InteralMemory.KeepWord[3]>=3000)//�ܶ�
		g_InteralMemory.KeepWord[3]=380;
	if(g_InteralMemory.KeepWord[3]<100)//�ܶ�
		g_InteralMemory.KeepWord[3]=380;
	
	if(g_InteralMemory.KeepWord[350] > 10)//�Ƿ���С���ܶȲ���
		g_InteralMemory.KeepWord[350] = 1;
	
	if(g_InteralMemory.KeepWord[351] > 10)//���ױ�־λ
		g_InteralMemory.KeepWord[351] = 0;
	
	Pulseout=0;
	InPulse1_Start();
	InPulse2_Start();
}

void  DataSave_yuanzhiji(void)/////�������ݱ�����ŵ������ж�
{
	INT8U *PageBuff;
	int Block;
	//int i;

	Block = Logical_To_Physical_Block(NAND_DATA_SAVE);
	
	EraseBlock(Block);
	
	PageBuff = (INT8U *)&g_InteralMemory.KeepWord[0];
	WritePage(Block,0,PageBuff);	// ��ʱֻ�õ�0ҳ��2K����������

	PageBuff = (INT8U *)&g_InteralMemory.KeepBit[0];
	WritePage(Block,1,PageBuff);	// ��ʱֻ�õ�1ҳ��2K����������
	
}
//char add12;
void yuanzhiji_run()
{
	char i,j,useless[2],SendSucceed;
	DWORD tmp;
	i=0;
	
	//////////////////////////////////////
	////�жϱ�Ƶ���Ƿ�������������������������ڴ�Ϊ������ͣ��ʱ�ĸ����ź�
	if(Get_X_Value(3)==1)
		Pulseout=1;
	else
		Pulseout=0;
		
	
	if(g_InteralMemory.KeepBit[610] == 0)////////�������󴫶��Ȳ�һ������ͨ��ʱ���������һ��
		g_InteralMemory.KeepWord[250] = 185;//����
	else
		g_InteralMemory.KeepWord[250] = 160;//����
	////////////************////////////////
	if(Get_X_Value(11))//�ⲿ��ť����
	{
		g_InteralMemory.Bit[6]=1;
		g_InteralMemory.KeepBit[602]=0;
		
		g_InteralMemory.Word[630]=0;///�豸״̬������ͨ����
		
		if(g_InteralMemory.KeepWord[104] == 1)//����γ�߶�ʱ���ָ������õ���������
		{
			g_InteralMemory.KeepWord[3]=g_InteralMemory.KeepWord[103];
			g_InteralMemory.KeepWord[104]=0;
		}
		
		rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//�����ζ�ʱ��3�жϣ�Ϊ�˷�ֹ�������������ť���;������Ӱ��
		timeadd=0;//��ʱ����0
		
		if((g_InteralMemory.KeepBit[606]==0) && (g_InteralMemory.KeepWord[95]==1) )
		{
			Set_Y_Value(7,0);//rGPGDAT |= 0x1;//����ʱ����ͣ��
			timer3_start();
			g_InteralMemory.KeepWord[95]=0;
			g_InteralMemory.Word[630]=1;
		}
		duanweiadd=0;
		stopweixianduan=0;
		
		Set_Y_Value(5,0);///ͣ����ת�ٴ�������ָ�ת��0906
		tmp = rGPBCON & (~(0x3<< 2));
		rGPBCON = tmp | (0x1<<2);	
		rGPBDAT |= (1<<1);
		Timer2_Stop();
	
	}
	if(Get_X_Value(12))//�ⲿ��ťͣ��
	{
		g_InteralMemory.Bit[7]=1;
		stopweixianduan=1;
		g_InteralMemory.Word[630]=1;
	}
		
	if(Get_X_Value(13))//�ⲿ��ť�㶯
	{
		g_InteralMemory.Bit[8]=1;
		if(g_InteralMemory.KeepWord[104] == 1)
		{
			g_InteralMemory.KeepWord[3]=g_InteralMemory.KeepWord[103];
			g_InteralMemory.KeepWord[104]=0;
		}
		rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//�����ζ�ʱ��3�жϣ�Ϊ�˷�ֹ��������㶯��ť���;������Ӱ��
		timeadd=0;//��ʱ����0
		duanweiadd=0;
	//	g_InteralMemory.Word[630]=0;///�豸״̬������ͨ����
	
		Set_Y_Value(5,0);///ͣ����ת�ٴ�������ָ�ת��0906
		
		Timer2_Stop();
	
	}
	else
	{
		g_InteralMemory.Bit[8]=0;
	//	g_InteralMemory.Word[630]=1;///�豸״̬������ͨ����
	}
	///////////////////////
	/////////*************////////////
	if(g_InteralMemory.Bit[6]==1)
	{
		
		////////////����д������Ƶ��
		if(g_InteralMemory.KeepBit[602]==0)
		{
			g_InteralMemory.Bit[1]=0;//����ʱ��������ϱ�־λ
			g_InteralMemory.Bit[2]=0;
			g_InteralMemory.Bit[3]=0;
			g_InteralMemory.Bit[4]=0;
			g_InteralMemory.Bit[5]=0;
			
		//	rGPLDAT &= 0x3DFF;	//�;��������	
		//	rGPLDAT &= 0x1FFF;   //����
		//	rGPEDAT &= 0xFF7F;	
			Set_Y_Value(11,1);//�;�������� ��������
			Set_Y_Value(7,1);
			g_InteralMemory.KeepBit[602]=1;
			
		//	rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//�����ζ�ʱ��3�жϣ�Ϊ�˷�ֹ�������������ť���;������Ӱ��
		}
		
	
	/////////////////////////	user_production();//�����ۼӣ��ߵ��ܶ��л�//////��Ҫ�ֳ�����	
		Status_Monitoring();//��γ״̬����������ʷ
		
	//	j=FX_ReadWord(useless,1);//����Ƶ�����״̬
		////�˴�Ԥ����Ƶ���������
		////////*********************88/////////

	}
	///////////////////////////////////
	if(g_InteralMemory.Bit[7]==1)
	{
		g_InteralMemory.Bit[6]=0;
		g_InteralMemory.Bit[7]=0;////////////////
		
	//	rGPEDAT |= 0x1;
		Set_Y_Value(7,0);
	//	rGPLDAT |= 0x200;//�;����ֹͣ
		rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//�����ζ�ʱ��3�жϣ�Ϊ�˷�ֹ��������㶯��ť���;������Ӱ��
		timeadd=0;//��ʱ����0
		timer3_start();
	}
	
	///////////////////////**********�ⲿ��ť�㶯***********////////////////////
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
	Banci_DataSave();//����л������ݱ���
	Jog_debug();//�㶯����	
	Status_Monitoring1();//��γ״̬����������ʷ
	
	if(g_InteralMemory.KeepWord[102] >= 65500)
		g_InteralMemory.KeepWord[102]=0;
		
	
	//////////////////////////////////////�ֶ���������
	if((g_InteralMemory.KeepBit[608]==1) && (g_InteralMemory.KeepBit[609]==0))//�ֶ�������
	{
		if(g_InteralMemory.KeepBit[611]==1)//�ֶ���������
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
		Set_Y_Value(5,0);//���ֶ�����ʱĬ�ϲ������
		g_InteralMemory.KeepBit[609]=0;
		

		tmp = rGPBCON  & (~(0x3<< 2));
		rGPBCON = tmp | (0x1<<2);	//set GPB1 as output	
		// �������������
		rGPBDAT |= (1<<1);//GPB1
	}
	/////////////////////////////////////////////2014.3.14ע�͵��п�����
/*	if(g_InteralMemory.KeepBit[613] == 1)
	{
		g_InteralMemory.KeepBit[613]=0;
		g_InteralMemory.Word[PING_CTRL] =25;
		
	}
	
	if((g_InteralMemory.KeepWord[158]/10) > g_InteralMemory.KeepWord[155])///��Ŀǰ���������ﵽʱ����
	{
		if(g_InteralMemory.Word[629] < 3)
		{
			g_InteralMemory.Word[PING_CTRL] =27;
			g_InteralMemory.Bit[7]=1;///ͣ��
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



void Save_lishiguzhang(void)//��ʷ���ϱ���
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
////////////////�л���������ݱ���/////////////////////
///////////////////////////////////////////////////////
void Banci_DataSave(void)
{
	int j=0;
	
	if(g_InteralMemory.KeepBit[604]==1)//����л���ʶλ
	{
		if(g_InteralMemory.KeepWord[0]==1)
		{	/////һ����ʷ���ݱ���
			while((j!=650623)&&(((j & 0xffff)!=g_InteralMemory.KeepWord[54])||(((j >> 16)& 0xffff)!=g_InteralMemory.KeepWord[55])) && j!=-1) 
			j=KeyBoard(999999,0,0,0);
			
			if(j != -1)
			{
				if(g_InteralMemory.KeepWord[16]==0)//���ݱ���λ�ã�һ��5������
				{
					g_InteralMemory.KeepWord[20]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[21]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[160]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[161]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[190]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[191]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[220]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[221]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[22]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[20]=0;
					else
						g_InteralMemory.KeepBit[20]=1;
											
					g_InteralMemory.KeepWord[16]++;
				}
				else if(g_InteralMemory.KeepWord[16]==1)
				{
					g_InteralMemory.KeepWord[23]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[24]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[162]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[163]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[192]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[193]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[222]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[223]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[25]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[21]=0;
					else
						g_InteralMemory.KeepBit[21]=1;
						
					g_InteralMemory.KeepWord[16]++;
				}
				else if(g_InteralMemory.KeepWord[16]==2)
				{
					g_InteralMemory.KeepWord[26]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[27]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[164]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[165]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[194]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[195]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[224]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[225]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[28]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[22]=0;
					else
						g_InteralMemory.KeepBit[22]=1;
						
					g_InteralMemory.KeepWord[16]++;
				}
				else if(g_InteralMemory.KeepWord[16]==3)
				{
					g_InteralMemory.KeepWord[29]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[30]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[166]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[167]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[196]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[197]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[226]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[227]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[31]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[23]=0;
					else
						g_InteralMemory.KeepBit[23]=1;
						
					g_InteralMemory.KeepWord[16]++;
				}
				else if(g_InteralMemory.KeepWord[16]==4)
				{
					g_InteralMemory.KeepWord[32]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[33]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[168]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[169]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[198]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[199]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[228]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[229]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[34]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[24]=0;
					else
						g_InteralMemory.KeepBit[24]=1;
						
					g_InteralMemory.KeepWord[16]=0;
				}
				else if(g_InteralMemory.KeepWord[16] > 4)
				{
					g_InteralMemory.KeepWord[20]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[21]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[160]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[161]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[190]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[191]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[220]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[221]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[22]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[20]=0;
					else
						g_InteralMemory.KeepBit[20]=1;
											
					g_InteralMemory.KeepWord[16]=1;
				}
				
				g_InteralMemory.KeepWord[0]=2;//���
				g_InteralMemory.KeepWord[100]=0;//�л���κ�Ĭ�ϸ��ܶȿ�ʼ
				g_InteralMemory.KeepWord[102]=0;//��յ�ǰ�ӹ�����
				g_InteralMemory.KeepBit[11]=0;//����豸״̬��0
				DataSave_yuanzhiji();
			}
		}
		else if(g_InteralMemory.KeepWord[0]==2)
		{/////������ʷ���ݱ���
			while((j!=650623)&&(((j & 0xffff)!=g_InteralMemory.KeepWord[56])||(((j >> 16)& 0xffff)!=g_InteralMemory.KeepWord[57])) && j!=-1) 
			j=KeyBoard(999999,0,0,0);
			
			if(j != -1)
			{
				if(g_InteralMemory.KeepWord[17]==0)//���ݱ���λ�ã�һ��5������
				{
					g_InteralMemory.KeepWord[35]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[36]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[170]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[171]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[200]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[201]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[230]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[231]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[37]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[25]=0;
					else
						g_InteralMemory.KeepBit[25]=1;
						
					g_InteralMemory.KeepWord[17]++;
				}
				else if(g_InteralMemory.KeepWord[17]==1)
				{
					g_InteralMemory.KeepWord[38]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[39]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[172]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[173]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[202]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[203]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[232]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[233]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[40]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[26]=0;
					else
						g_InteralMemory.KeepBit[26]=1;
						
					g_InteralMemory.KeepWord[17]++;
				}
				else if(g_InteralMemory.KeepWord[17]==2)
				{
					g_InteralMemory.KeepWord[41]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[42]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[174]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[175]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[204]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[205]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[234]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[235]=g_InteralMemory.KeepWord[154];//������				
					g_InteralMemory.KeepWord[43]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[27]=0;
					else
						g_InteralMemory.KeepBit[27]=1;
						
					g_InteralMemory.KeepWord[17]++;
				}
				else if(g_InteralMemory.KeepWord[17]==3)
				{
					g_InteralMemory.KeepWord[44]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[45]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[176]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[177]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[206]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[207]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[236]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[237]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[46]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[28]=0;
					else
						g_InteralMemory.KeepBit[28]=1;
					
					g_InteralMemory.KeepWord[17]++;
				}
				else if(g_InteralMemory.KeepWord[17]==4)
				{
					g_InteralMemory.KeepWord[47]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[48]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[178]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[179]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[208]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[209]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[238]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[239]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[49]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[29]=0;
					else
						g_InteralMemory.KeepBit[29]=1;
					
					g_InteralMemory.KeepWord[17]=0;
				}
				else if(g_InteralMemory.KeepWord[17] > 4)
				{
					g_InteralMemory.KeepWord[35]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[36]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[170]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[171]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[200]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[201]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[230]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[231]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[37]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[25]=0;
					else
						g_InteralMemory.KeepBit[25]=1;
						
					g_InteralMemory.KeepWord[17]=1;
				}
				
				g_InteralMemory.KeepWord[0]=3;//���
				g_InteralMemory.KeepWord[100]=0;//�л���κ�Ĭ�ϸ��ܶȿ�ʼ
				g_InteralMemory.KeepWord[102]=0;//��յ�ǰ�ӹ�����
				g_InteralMemory.KeepBit[11]=0;//����豸״̬
				DataSave_yuanzhiji();
			}
		}
		else
		{/////������ʷ���ݱ���
			while((j!=650623)&&(((j & 0xffff)!=g_InteralMemory.KeepWord[52])||(((j >> 16)& 0xffff)!=g_InteralMemory.KeepWord[53])) && j!=-1) 
			j=KeyBoard(999999,0,0,0);
			
			if(j != -1)
			{
				if(g_InteralMemory.KeepWord[18]==0)//���ݱ���λ�ã�һ��5������
				{
					g_InteralMemory.KeepWord[120]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[121]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[180]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[181]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[210]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[211]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[240]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[241]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[122]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[40]=0;
					else
						g_InteralMemory.KeepBit[40]=1;
						
					g_InteralMemory.KeepWord[18]++;
				}
				else if(g_InteralMemory.KeepWord[18]==1)
				{
					g_InteralMemory.KeepWord[123]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[124]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[182]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[183]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[212]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[213]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[242]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[243]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[125]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[41]=0;
					else
						g_InteralMemory.KeepBit[41]=1;
						
					g_InteralMemory.KeepWord[18]++;
				}
				else if(g_InteralMemory.KeepWord[18]==2)
				{
					g_InteralMemory.KeepWord[126]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[127]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[184]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[185]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[214]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[215]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[244]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[245]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[128]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[42]=0;
					else
						g_InteralMemory.KeepBit[42]=1;
						
					g_InteralMemory.KeepWord[18]++;
				}
				else if(g_InteralMemory.KeepWord[18]==3)
				{
					g_InteralMemory.KeepWord[129]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[130]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[186]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[187]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[216]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[217]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[246]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[247]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[131]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[43]=0;
					else
						g_InteralMemory.KeepBit[43]=1;
					
					g_InteralMemory.KeepWord[18]++;
				}
				else if(g_InteralMemory.KeepWord[18]==4)
				{
					g_InteralMemory.KeepWord[132]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[133]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[188]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[189]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[218]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[219]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[248]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[249]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[134]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[44]=0;
					else
						g_InteralMemory.KeepBit[44]=1;
					
					g_InteralMemory.KeepWord[18]=0;
				}
				else if(g_InteralMemory.KeepWord[18] > 4)
				{
					g_InteralMemory.KeepWord[120]=g_InteralMemory.Word[331];//ʱ��
					g_InteralMemory.KeepWord[121]=g_InteralMemory.Word[332];
					g_InteralMemory.KeepWord[180]=g_InteralMemory.Word[333];//Сʱ
					g_InteralMemory.KeepWord[181]=g_InteralMemory.Word[334];//����
					g_InteralMemory.KeepWord[210]=g_InteralMemory.KeepWord[151];//����
					g_InteralMemory.KeepWord[211]=g_InteralMemory.KeepWord[152];//����
					g_InteralMemory.KeepWord[240]=g_InteralMemory.KeepWord[153];//������
					g_InteralMemory.KeepWord[241]=g_InteralMemory.KeepWord[154];//������
					g_InteralMemory.KeepWord[122]=g_InteralMemory.KeepWord[102];//��������
					if(g_InteralMemory.KeepBit[11]==0)//�����豸״̬
						g_InteralMemory.KeepBit[40]=0;
					else
						g_InteralMemory.KeepBit[40]=1;
						
					g_InteralMemory.KeepWord[18]=1;
				}
				
				g_InteralMemory.KeepWord[0]=1;//���
				g_InteralMemory.KeepWord[100]=0;//�л���κ�Ĭ�ϸ��ܶȿ�ʼ
				g_InteralMemory.KeepWord[102]=0;//��յ�ǰ�ӹ�����
				g_InteralMemory.KeepBit[11]=0;//����豸״̬
				DataSave_yuanzhiji();
			}
		}
		g_InteralMemory.KeepBit[604]=0;	
		
		/////
		if(j != -1)
		{
			g_InteralMemory.Word[PING_CTRL] =1;///�����κ��Զ���ת�ع�������
			////�����  �¹��ŵ���
			g_InteralMemory.KeepWord[151]=g_InteralMemory.Word[622];
			g_InteralMemory.KeepWord[152]=g_InteralMemory.Word[623];
		}
		else 
			g_InteralMemory.Word[PING_CTRL] =1;
	}
}

/////////////////////////////////////////////////////////////
//////////////////�㶯����////////////////////////
/////////////////////////////////////////////////////////////
void Jog_debug(void)
{
	char SendSucceed,useless[2],i;
	if((g_InteralMemory.KeepBit[600]==1)&&(g_InteralMemory.KeepBit[601]==0))
	{
	//	rGPLDAT &= 0x3DFF;	//�;��������
	//	rGPLDAT &=0x1FFF;
		g_InteralMemory.Bit[6]=0;
	//	rGPLDAT &= 0x1DFF;	
		Set_Y_Value(11,1);//�;�������� ��������
		Set_Y_Value(7,1);
		
		rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//�����ζ�ʱ��3�жϣ�Ϊ�˷�ֹ�������������ť���;������Ӱ��
		timeadd=0;
		
		g_InteralMemory.Bit[1]=0;//����ʱ��������ϱ�־λ
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

//		rGPLDAT |= 0x200;//�;����ֹͣ
		timer3_start();
		
		g_InteralMemory.KeepWord[100]=0;
		g_InteralMemory.KeepBit[601]=0;

	}
	//////////////////
//////////////////////////////////	Status_Monitoring();//��γ״̬���
}

///////////////////////////////////////////////////////////////////////
////��γ״̬���
///////////////////////////////////
void Status_Monitoring(void)
{	
	char useless[2],i,SendSucceed;
	
	if(Get_X_Value(7))//A�����߼��
		g_InteralMemory.Bit[1]=1;
	//else
	//	g_InteralMemory.Bit[1]=0;
	////////////////
	if(Get_X_Value(8))//B�����߼��
		g_InteralMemory.Bit[2]=1;
	//else
	//	g_InteralMemory.Bit[2]=0;
	////////////////////
	if(Get_X_Value(9))//C�����߼��
		g_InteralMemory.Bit[3]=1;
	//else
	//	g_InteralMemory.Bit[3]=0;
	/////////////////////
	if(Get_X_Value(10))//D�����߼��
		g_InteralMemory.Bit[4]=1;
	//else
	//	g_InteralMemory.Bit[4]=0;

	///////////////////////////////////////
	///�豸״̬��ʾ�Լ���ʷ���ϵı���
	if(( (g_InteralMemory.Bit[1]==1) || (g_InteralMemory.Bit[2]==1) || (g_InteralMemory.Bit[3]==1) || (g_InteralMemory.Bit[4]==1)))
	 {
		 g_InteralMemory.KeepBit[10]=1;//��ʾ"���߹���"
		 g_InteralMemory.Word[620]=1;//���ϱ����־λ
		 g_InteralMemory.KeepBit[11]=1;//���ڱ����豸�Ĺ��ϱ�־λ
		 Save_lishiguzhang(); 
		
		if(( g_InteralMemory.KeepBit[605]==0))//ͣ���Զ�ͣ��ʱ������ͣ��
		{
			Set_Y_Value(7,0);
			rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//�����ζ�ʱ��3�жϣ�Ϊ�˷�ֹ�������������ť���;������Ӱ��
			timer3_start();
			g_InteralMemory.KeepWord[95]=1;
			
			g_InteralMemory.Word[630]=2;///�豸״̬������ͨ����
		}
		stopweixianduan=1;
	
		 
	 }
		 
	if((g_InteralMemory.Bit[5]==1))
	 {
		 g_InteralMemory.KeepBit[10]=2;//��ʾ"γβ"
		 g_InteralMemory.Word[620]=1;//���ϱ����־λ
		 g_InteralMemory.KeepBit[11]=1;//���ڱ����豸�Ĺ��ϱ�־λ
		 Save_lishiguzhang();
	//FX_WriteWord(useless,2,0);//����ʱ����ͣ��
		 if( (g_InteralMemory.KeepBit[606]==0))//ͣ���Զ�ͣγʱ������ͣ��
		 {
			Set_Y_Value(7,0);//����ʱ����ͣ��
			rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//�����ζ�ʱ��3�жϣ�Ϊ�˷�ֹ�������������ť���;������Ӱ��
			timer3_start();
			g_InteralMemory.KeepWord[95]=1;
			g_InteralMemory.Word[630]=2;///�豸״̬������ͨ����
		 }
		// g_InteralMemory.KeepWord[95]=1;
		stopweixianduan=1;
		
	 }
	 
	if((g_InteralMemory.Bit[5]==2) && (stopweixianduan==0) )	 
	 {
		 g_InteralMemory.KeepBit[10]=3;//��ʾ"γ��"
		 g_InteralMemory.Word[620]=1;//���ϱ����־λ
		 g_InteralMemory.KeepBit[11]=1;//���ڱ����豸�Ĺ��ϱ�־λ
		 Save_lishiguzhang();
	//FX_WriteWord(useless,2,0);//����ʱ����ͣ��
		 if( (g_InteralMemory.KeepBit[606]==0))//ͣ���Զ�ͣγʱ������ͣ��
		 {
			Set_Y_Value(7,0);//����ʱ����ͣ��
			rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//�����ζ�ʱ��3�жϣ�Ϊ�˷�ֹ�������������ť���;������Ӱ��
			timer3_start();
			g_InteralMemory.KeepWord[95]=1;
			g_InteralMemory.Word[630]=2;///�豸״̬������ͨ����
		 }
		// g_InteralMemory.KeepWord[95]=1;
		
	 }
	if(g_InteralMemory.KeepWord[95] != 1)
	 {
		 g_InteralMemory.KeepBit[10]=0;
		 g_InteralMemory.Word[621]=0;//���ϱ����־λ
		 
	//	 if(g_InteralMemory.KeepWord[95]==1)
		 {
		 	g_InteralMemory.KeepWord[95]=0;
		
			
			//rGPLDAT &= 0x1FFF;//����????????????????
			/////////////****************?????????????
			/////////////****************?????????????
			//////////////�����ǣ���������////////////
			/////////////****************?????????????
			/////////////****************?????????????
		
		 }
	 }
	
	
}


void Status_Monitoring1(void)
{	
	
	if(Get_X_Value(7))//A�����߼��
		g_InteralMemory.Bit[1]=1;
	//else
	//	g_InteralMemory.Bit[1]=0;
	////////////////
	if(Get_X_Value(8))//B�����߼��
		g_InteralMemory.Bit[2]=1;
	//else
	//	g_InteralMemory.Bit[2]=0;
	////////////////////
	if(Get_X_Value(9))//C�����߼��
		g_InteralMemory.Bit[3]=1;
	//else
	//	g_InteralMemory.Bit[3]=0;
	/////////////////////
	if(Get_X_Value(10))//D�����߼��
		g_InteralMemory.Bit[4]=1;
	//else
	//	g_InteralMemory.Bit[4]=0;

	///////////////////////////////////////
	///��γ�߹��� �;��;�����Զ���ʱͣ��
	if( ((g_InteralMemory.Bit[1]==1) || (g_InteralMemory.Bit[2]==1) || (g_InteralMemory.Bit[3]==1) || (g_InteralMemory.Bit[4]==1)))// && (jingxianguzhang==0)  )
	 {
		g_InteralMemory.KeepBit[10]=1;//��ʾ"���߹���"
		
		if(g_InteralMemory.KeepWord[95]==1)
		{
		//	rGPLDAT |= 0x2000;//����ʱ����ͣ��
		//	rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//�����ζ�ʱ��3�жϣ�Ϊ�˷�ֹ�������������ť���;������Ӱ��
		//	timer3_start();
			g_InteralMemory.KeepWord[95]=0;
		}
		stopweixianduan=1;
		
	 }	
	 else if((g_InteralMemory.Bit[5]==1))
	 {
	 	g_InteralMemory.KeepBit[10]=2;//��ʾ"γβ����"
	 	stopweixianduan=1;
	 	
	 }
	 else if((g_InteralMemory.Bit[5]==2)  && (stopweixianduan==0))//γ��
	 {
	 	g_InteralMemory.KeepBit[10]=3;//��ʾ"γ�Ϲ���"
	 //	rGPLDAT |= 0x2000;//����ʱ����ͣ��
	 //	rINTMSK1 =  rINTMSK1 | ((U32)0x1<<13);//�����ζ�ʱ��3�жϣ�Ϊ�˷�ֹ�������������ť���;������Ӱ��
	//	timer3_start();
		
	 }
	
	 else
	 {
		 g_InteralMemory.KeepBit[10]=0;
		 g_InteralMemory.Bit[5]=0;
		 g_InteralMemory.Word[621]=0;//���ϱ����־λ
		 
		 g_InteralMemory.KeepWord[95]=0;
	 }
	
	
}


	


void user_password(void)
{
	//////////////////////////////////////////
	////����ƥ��	
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
