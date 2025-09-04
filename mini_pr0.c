# include<LPC21xx.h>
# include<string.h>
# include "delay.h"
# include "lcd.h"
# define sw1 4
# define sw2 5
# define sw3 6
# define sw4 1<<3   \\external interrupt 1
# define id_limit 10
# define irq_enable 1<<5
# define eint1 1<<15
void uart0_config();
unsigned char uart0_rx(void);
void uart0_tx(unsigned char);
void interrupt_configure(void);
unsigned char id0[id_limit][50]={{"0600682384C90"},{"060067A552965"},{"060067E061E00"}};
unsigned char in[]="tap your card";
unsigned char entry[]="your id is verified";
unsigned char one[]="1-BATERY";
unsigned char two[]="2-VOLT";
unsigned char three[]="3-BUTTON";
unsigned char exite[]="welcome";
unsigned char error[]="you have already scanned";
unsigned char registered[]="your card not registered";
unsigned char out1[]="BATERY=";
unsigned char out2[]="VOLT=";
unsigned char out3[]="BUTTON=";
unsigned int batery=0,volt=0,button=0;
void tag_value_reader(void);
void isr1(void) __irq
{
	EXTINT=0x02;
	lcd_string(out1);
	lcd_integer(batery);
	lcd_string(out2);
	lcd_integer(volt);
	lcd_string(out3);
	lcd_integer(button);
	VICVectAddr=0;
}
int main()
{
	unsigned int i=0,j=0;
	unsigned int flag[]={0,0,0};
	unsigned char a[14];
	init();
	uart0_config();
	//uart0_tx('a');
	//tag_value_reader();
	PINSEL0|=(3<<6);
	interrupt_configure();
	EXTMODE=(1<<1);
	EXTPOLAR=(0<<1);
	VICIntEnable=eint1;
	label:
	while(1)
	{
		j=0;
		comand(0x01);
		comand(0x80);
		lcd_string(in);
		//delay_milliseconds(300);
		for(i=0;i<13;i++)
		{
			a[i]=uart0_rx();
		}
		a[i]='\0';
		while(1)
		{
			if(flag[j]!=1)
			{
				if(strcmp(a,id0[j])==0)
				{
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
						lcd_string(registered);
						//delay_milliseconds(500);
						goto label;
					}
				}
			}
			else
			{
				comand(0x01);
				comand(0x80);
				lcd_string(error);
				delay_seconds(2);
				goto label;
			}
		}
comand(0x01);
comand(0x80);		
		lcd_string(entry);
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
				++batery;
				while(((IOPIN0>>sw1)&1)==0);
				break;
			}
			else if(((IOPIN0>>sw2)&1)==0)
			{
				delay_milliseconds(200);
				++volt;
				while(((IOPIN0>>sw2)&1)==0);
				break;
			}
			else if(((IOPIN0>>sw3)&1)==0)
			{
				delay_milliseconds(200);
				++button;
				while(((IOPIN0>>sw3)&1)==0);
				break;
			}
		}
		comand(0x01);
		comand(0x80);
		lcd_string(exite);
		delay_seconds(2);
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
