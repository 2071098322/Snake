#include <intrins.h>
#include <stdlib.h>
#include <reg52.h>

//IO����
sbit LEDARRAY_D = P2^0;
sbit LEDARRAY_C = P2^1;
sbit LEDARRAY_B = P2^2;
sbit LEDARRAY_A = P2^3;
sbit LEDARRAY_G = P2^4;
sbit LEDARRAY_DI = P2^5;
sbit LEDARRAY_CLK = P2^6;
sbit LEDARRAY_LAT = P2^7;

sbit KEY_Right=P1^7;			//��
sbit KEY_Down=P1^6;			//��
sbit KEY_Left=P1^5;			//��
sbit KEY_Up=P1^4;			//��
sbit KEY_Star=P3^7;		//��ʼ
sbit KEY_Restar=P3^2;		//���¿�ʼ
sbit KEY_Switch=P3^6;		//ѡ���Ѷ�
#define Right 0
#define Left 2
#define Up 3
#define Down 1

//�������߱�������
 void Send( unsigned char dat);
 void Scan_Line( unsigned char m);
void ReInit();
void Display();
void Init();
void Shift();
void Random_Dot();
void Scan_Key();

void Delay_ms( unsigned int m);
void delay(int n);
unsigned char Display_Buffer[2];

unsigned char table[31]={0x00,0x10,0x20};
unsigned char count = 3;
unsigned char Direction;
unsigned char T;
int tt,n,z;
unsigned char Flag_Shift,Flag_Over,flag3,Flag_Dot,Flag_Draw;
unsigned char u;
//************************************************************
//������
//*************************************************************
void main()
{  
	Init();
	while(1)
	{
		if (Flag_Dot == 1)			//������������
			Random_Dot();			//�漴��ʾһ����

		if (Flag_Shift == 1)		
		
		Shift();				//̰ʳ���ƶ�
		Scan_Key();			//ɨ�谴��
		Display();			//��ʾ
						if (z==0)		//�Ƿ�����ͣ����
				{
					while (1)
					{
						Display();
						if (KEY_Star == 0)						
						{
						break;
						}		
	
			      }		
				 }
	}
}


//**********************************************************
//��ʱ����
//**********************************************************
void Delay_ms( unsigned int m)
{
	unsigned int j;
	unsigned int i;
	for(i = 0 ; i < m ; i++)
	{
		for(j = 0 ; j < 123 ; j++);
	}
}


//**********************************************************
//��ʱ����
//**********************************************************
void delay(int n)
{
	while(n--);
}
//**********************************************************
//���³�ʼ��
//**********************************************************
void ReInit()
{	
	TH0=(65535-50000)/256;
	TL0=(65535-50000)%256;
	TR0=0;
	z=0;
	T=0;
	Direction=0;
	count=3;
	table[0]=0x00;
	table[1]=0x10;
	table[2]=0x20;
	Delay_ms(500);
	Flag_Over=0;
	Flag_Dot=2;
	Flag_Draw=1;
	table[count]=0;

}
//**********************************************************
//��ʼ��
//**********************************************************
void Init()
{
	Flag_Draw=1;
	Flag_Dot=2;
	tt=0;
	T=0;
	z=0;
	count=0;
	Flag_Shift=0;
	Flag_Over=0;
	n=0;						//��ʼ�Ѷ�				
	EA=1;						//�����ж�
	IT0=1;						//�͵�ƽ����
	ET0=1;						//��ʱ��0�ж�ʹ��
	ET1=1;						//��ʱ��1�ж�ʹ��
	TMOD=0x11;					//��ʱ��0ģʽ1����ʱ��1ģʽ1
	TH0=(65535-50000)/256;		//��ʱ��0�趨��ֵ
	TL0=(65535-50000)%256;
	TH1=(65535-10000)/256;		//��ʱ��1�趨��ֵ
	TL1=(65535-10000)%256;
	Direction=0;						//����
	TR0=0;						//��ʱ��0������
	TR1=1;						//��ʱ��1����
}

//**********************************************************
//����ɨ��
//**********************************************************
void Scan_Key()
{
	if(KEY_Switch == 0)
	{
			n=n+1;
			if (n>2)
			{ 
				n=n-3;
			}
			count=n;
			Delay_ms (500);
	}
	if(KEY_Star == 0)
		{	

			if (Flag_Dot==2)
			{	
				count=3;
				Flag_Dot=1;			//������Ϸ
				TR0 = 1;			//��ʼ�ƶ�
			}	
				z=z+1;
				z=z%2;
				Delay_ms (500);
		}

	if(KEY_Restar == 0)
	{
			ReInit();
	}

	if(KEY_Right == 0)
	{
		if(Direction != Left)
			Direction = Right;
	}

	if(KEY_Down == 0)
	{
		if(Direction != Up)
		Direction = Down;	
	}

	if(KEY_Left == 0)
	{
		if(Direction != Right)
		Direction = Left;
	
	}

	if(KEY_Up == 0)
	{
		if(Direction != Down)
		Direction = Up;	
	}
}

//**********************************************************
//
//**********************************************************
void Shift()
{
	int k;
	if ( Flag_Draw == 1 )
	{
		for(k=0;k<count-1;k++)				//��������
		{
			table[k]=table[k+1];
		}
	}
	switch (Direction)
	{
		case 0:							//����
			if (table[count-1]/16<15)				//��ͷû�г����ұ߿�
				table[count-1]=table[count-1]+16;	//����
			else
				Flag_Over=1;							//��ͷ�����߿���Ϸ����
			break;
		case 1:							//����
			if (table[count-1]%16<15)
				table[count-1]=table[count-1]+1;
			else
				Flag_Over=1;
			break;
		case 2:							//����
 			if (table[count-1]/16>0)
				table[count-1]=table[count-1]-16;
			else
				Flag_Over=1;
			break;
		case 3:							//����
			if (table[count-1]%16>0)
				table[count-1]=table[count-1]-1;
			else
				Flag_Over=1;
			break;
		default:
			break;
	}
	Flag_Shift=0;
	if (Flag_Over == 0)
	{
		for (k=0;k<count-1;k++)							//�ж��Ƿ���ͷ��ײ������
		{
			if (table[count-1]==table[k])
			{
				Flag_Over=1;
				break;
			}
		}
	}
	if (Flag_Over == 1)
	{ 
		ReInit();					//���ֿ�ʼ��Ϸ
	}
	else if (table[count-1]==table[count])		//��ͷ ���� ʳ��
	{
		if (count<30)							
		{
			count++;							//������+1
			Flag_Draw=0;
		}
		Flag_Dot=1;
		table[count]=0xff;						//�µ�ʳ���������û���ǰ������ʾ
	}
	else
	{
		Flag_Draw=1;
	}		
}

//**********************************************************
//�������
//**********************************************************
void Random_Dot()
{
	int k;
	Flag_Dot=0;
	do
	{
		flag3=1;
		u=rand()%256;				//ȡ��0-255�е�����һ��ֵ
		u=u/16*10+u%16;				//ȡ���漴����кŷŸ�4λ���кŷŵ�4λ
		for (k=0;k<count;k++)		//�Ƿ��������غ�
		{
			if (u==table[k])		//�������غϣ������漴
			{
				flag3=0;
				break;
			}
		}
	}
	while(!flag3);					//����flag3 == 1����ѭ�������Ϊ0�������漴��
	table[count]=u;					//ȡ���漴���ֵ����table[count]��
}

//**********************************************************
//��ʱ��0�ж�
//**********************************************************
void time0() interrupt 1
{
	TH0=(65535-50000)/256;
	TL0=(65535-50000)%256;
	T++;
	if (T>=3*n+4)
	{
		T=0;
		Flag_Shift=1;
	}
}

//**********************************************************
//��ʱ���ж�1
//**********************************************************
void time1() interrupt 3
{
	TH1=(65535-10000)/256;		//��ʱ��1�趨��ֵ
	TL1=(65535-10000)%256;
	tt++;			  //��ȡ�����������
	srand(tt);
	if 	(tt>=2157)
	tt=0;

}


//************************************************************
//��ʾ����
//*************************************************************
void Display()					
{
	unsigned char i,j;
	unsigned int temp = 0x7fff;
	unsigned char x,y;

	for( j = 0 ; j <= count ; j++ )
	{
		LEDARRAY_G = 1;			//��������ʱ��ر���ʾ���ȸ��������ݣ���138��ʾ�С���ֹ��Ӱ��

		y=table[j]/16;				//��4λ����ֵ��  ��ֵ��0 = ѡͨ  1 = ��ѡ
		x=table[j]%16;				//��4λ����ֵ��  ��ֵ��1 = ѡͨ  0 = ��ѡ
		y=15-y;

		temp = 0x7fff;		
		for(i = 0 ; i < y ; i++)
		{
			temp = (temp>>1)|0x8000;
		} 

		Display_Buffer[1] = temp&0x00ff;
		Display_Buffer[0] = (temp>>8)&0x00ff;
				
		Send(Display_Buffer[1]);
		Send(Display_Buffer[0]);

		LEDARRAY_LAT = 1;					//��������
		_nop_();
	
		LEDARRAY_LAT = 0;
		_nop_();

		Scan_Line(x);						//ѡ���i��

		LEDARRAY_G = 0;
		
		delay(50);		//��ʱһ��ʱ�䣬��LED��������				
	}	
}

//****************************************************
//ɨ��ĳһ��
//****************************************************
void Scan_Line( unsigned char m)
{	
	switch(m)
	{
		case 0:			
			LEDARRAY_D = 0;LEDARRAY_C = 0;LEDARRAY_B = 0;LEDARRAY_A = 0; 					
			break;
		case 1:					
			LEDARRAY_D = 1;LEDARRAY_C = 0;LEDARRAY_B = 0;LEDARRAY_A = 0;		
			break;
		case 2:					
			LEDARRAY_D = 0;LEDARRAY_C = 1;LEDARRAY_B = 0;LEDARRAY_A = 0; 		
			break;
		case 3:					
			LEDARRAY_D = 1;LEDARRAY_C = 1;LEDARRAY_B = 0;LEDARRAY_A = 0; 		
			break;
		case 4:
			LEDARRAY_D = 0;LEDARRAY_C = 0;LEDARRAY_B = 1;LEDARRAY_A = 0;		
			break;
		case 5:
			LEDARRAY_D = 1;LEDARRAY_C = 0;LEDARRAY_B = 1;LEDARRAY_A = 0; 		
			break;
		case 6:
			LEDARRAY_D = 0;LEDARRAY_C = 1;LEDARRAY_B = 1;LEDARRAY_A = 0; 		
			break;
		case 7:
			LEDARRAY_D = 1;LEDARRAY_C = 1;LEDARRAY_B = 1;LEDARRAY_A = 0; 		
			break;
		case 8:
			LEDARRAY_D = 0;LEDARRAY_C = 0;LEDARRAY_B = 0;LEDARRAY_A = 1; 		
			break;
		case 9:
			LEDARRAY_D = 1;LEDARRAY_C = 0;LEDARRAY_B = 0;LEDARRAY_A = 1; 		
			break;	
		case 10:
			LEDARRAY_D = 0;LEDARRAY_C = 1;LEDARRAY_B = 0;LEDARRAY_A = 1; 		
			break;
		case 11:
			LEDARRAY_D = 1;LEDARRAY_C = 1;LEDARRAY_B = 0;LEDARRAY_A = 1; 		
			break;
		case 12:
			LEDARRAY_D = 0;LEDARRAY_C = 0;LEDARRAY_B = 1;LEDARRAY_A = 1; 		
			break;
		case 13:
			LEDARRAY_D = 1;LEDARRAY_C = 0;LEDARRAY_B = 1;LEDARRAY_A = 1; 		
			break;
		case 14:
			LEDARRAY_D = 0;LEDARRAY_C = 1;LEDARRAY_B = 1;LEDARRAY_A = 1; 		
			break;
		case 15:
			LEDARRAY_D = 1;LEDARRAY_C = 1;LEDARRAY_B = 1;LEDARRAY_A = 1; 		
			break;
		default : break;	
	}
}

//****************************************************
//��������
//****************************************************
void Send( unsigned char dat)
{
	unsigned char i;
	char j = 0;
	LEDARRAY_CLK = 0;
	_nop_();	
	LEDARRAY_LAT = 0;
	_nop_();

	for( i = 0 ; i < 8 ; i++ )
	{
		if( dat&0x01 )
		{
			LEDARRAY_DI = 1;	
		}
		else
		{
			LEDARRAY_DI = 0;
		}


		LEDARRAY_CLK = 1;				//�����ط�������
			_nop_();
		LEDARRAY_CLK = 0;
			_nop_();

		
		dat >>= 1;			
	}			
}
