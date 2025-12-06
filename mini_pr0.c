# include<LPC21xx.h>
# include<string.h>
# include "delay.h"
# include "lcd.h"
# define sw1 4
# define sw2 5
# define sw3 6
# define id_limit 10
# define irq_enable 1<<5
# define eint1 1<<15
void uart0_config();
unsigned char uart0_rx(void);
void uart0_tx(unsigned char);
void interrupt_configure(void);
unsigned char id0[id_limit][14]={{"062547A95C123"},{"062A47A95C123"},{"762547A95C123"}};
unsigned char tap[]="tap your card";
unsigned char verified[]="your id is verified";
unsigned char one[]="1-BATERY";
unsigned char two[]="2-VOLT";
unsigned char three[]="3-BUTTON";
unsigned char welcome[]="welcome";
unsigned char already_scan[]="you have already scanned";
unsigned char not_registered[]="your card not registered";
unsigned char again[]="tap your card again for verification";
unsigned char miss_match[]="card miss match vote not counted";
unsigned char out1[]="BATERY=";
unsigned char out2[]="VOLT=";
unsigned char out3[]="BUTTON=";
unsigned char spaces[]=" ";
unsigned int batery=0,volt=0,button=0;
void tag_value_reader(void);
void isr1(void) __irq
{
	EXTINT=0x02;
	comand(0x01);
	comand(0x80);
	lcd_string(out1);
	lcd_integer(batery);
	lcd_string(spaces);
	lcd_string(out2);
	lcd_integer(volt);
	comand(0xc4);
	lcd_string(out3);
	lcd_integer(button);
	VICVectAddr=0;
}
int main()
{
	unsigned int i=0,j=0,k=0;
	unsigned int flag[]={0,0,0};
	unsigned char a[14],sam[14];
	init();
	uart0_config();
	//uart0_tx('a');
	//tag_value_reader();
	PINSEL0|=(3<<6);
	interrupt_configure();
	EXTMODE=0x03;
	EXTPOLAR=0x00;
	VICIntEnable=eint1;
	label:
	while(1)
	{
		comand(0x01);
		comand(0x80);
		lcd_string(tap);
		//delay_milliseconds(300);
		for(i=0;i<13;i++)
		{
			a[i]=uart0_rx();
		}
		a[i]='\0';
		comand(0x01);
		comand(0x80);
		lcd_string(a);
		delay_seconds(2);
		while(1)
		{
			if(strcmp(a,id0[j])==0)
			{
				if(flag[j]==1)
				{
					comand(0x01);
					comand(0x80);
					lcd_string(already_scan);
					delay_seconds(2);
					goto label;
				}
				flag[j]=1;
				break;
			}	
			else
			{
				j++;
				if(j==id_limit)
				{
					comand(0x01);
					comand(0x80);
					lcd_string(not_registered);
					delay_milliseconds(2000);
					goto label;
				}
			}
		}
		comand(0x01);
		comand(0x80);		
		lcd_string(verified);
		delay_seconds(2);
		comand(0x01);
		comand(0x80);
		lcd_string(one);
		comand(0x88);
		lcd_string(two);
		comand(0xc0);
		lcd_string(three);
		while(1)
		{
			if(((IOPIN0>>sw1)&1)==0)
			{
				delay_milliseconds(200);
				while(((IOPIN0>>sw1)&1)==0);
				lcd_string(again);
				for(k=0;k<13;k++)
				{
				sam[k]=uart0_rx();
				}
				sam[k]='\0';
				if(strcmp(a,sam)==0)
				{
					++batery;
					comand(0x01);
					comand(0x80);
					lcd_string(welcome);
				}
				else
				{
					comand(0x01);
					comand(0x80);
					flag[j]=0;
					lcd_string(miss_match);
					delay_seconds(1);
				}					
				break;
			}
			else if(((IOPIN0>>sw2)&1)==0)
			{
				delay_milliseconds(200);
				while(((IOPIN0>>sw2)&1)==0);
				comand(0x01);
				comand(0x80);
				lcd_string(again);
				for(k=0;k<13;k++)
				{
				sam[k]=uart0_rx();
				}
				sam[k]='\0';
				if(strcmp(a,sam)==0)
				{
					++volt;
					comand(0x01);
					comand(0x80);
					lcd_string(welcome);
				}
				else
				{
					comand(0x01);
					comand(0x80);
					flag[j]=0;
					lcd_string(miss_match);
					delay_seconds(1);
				}					
				break;
			}
			else if(((IOPIN0>>sw3)&1)==0)
			{
				delay_milliseconds(200);
				while(((IOPIN0>>sw3)&1)==0);
				comand(0x01);
				comand(0x80);
				lcd_string(again);
				for(k=0;k<13;k++)
				{
				sam[k]=uart0_rx();
				}
				sam[k]='\0';
				if(strcmp(a,sam)==0)
				{
					++button;
					comand(0x01);
					comand(0x80);
					lcd_string(welcome);
				}
				else
				{
					comand(0x01);
					comand(0x80);
					flag[j]=0;
					lcd_string(miss_match);
					delay_seconds(1);
				}					
				break;
			}
		}
		delay_seconds(2);
		j=0;
	}
}
void uart0_config(void)
{
	PINSEL0|=0x05;
	U0LCR=0x83;
	U0DLL=97;
	U0DLM=0;
	U0LCR=0x3;
}
void uart0_tx(unsigned char ms)
{
	while(((U0LSR>>5)&1)==0);
	U0THR=(ms);
}
unsigned char uart0_rx(void)
{
	while((U0LSR&1)==0);
	return U0RBR;
}
void tag_value_reader(void)
{
	unsigned char a[13];
	unsigned int i;
	for(i=0;i<13;i++)
		a[i]=uart0_rx();
	delay_milliseconds(100);
	a[i]='\0';
	delay_milliseconds(100);
	for(i=0;i<13;i++)
		uart0_tx(a[i]);
}
void interrupt_configure(void)
{
	VICIntSelect=0;
	VICVectCntl1=(irq_enable|15);
	VICVectAddr1=(int)isr1;
}

