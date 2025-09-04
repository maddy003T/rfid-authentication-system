# include<lpc21xx.h>
# define data (0xff<<8)
# define rs (1<<16)
# define e (1<<17)
void comand(unsigned char ms)
{					    
  IOCLR0=data;
	IOSET0=((ms<<8)&data);
	IOCLR0=rs;
	IOSET0=e;
	delay_milliseconds(2);
	IOCLR0=e;	
}
void dat(unsigned char ms);
void lcd_integer(int p);
	
void init(void)
{
  	IODIR0|=data;
	IODIR0|=rs;
	IODIR0|=e;
	comand(0x01);
	comand(0x02);
	comand(0x0c);
	comand(0x38);
}

void dat(unsigned char ms)
{
  	IOCLR0=data;
	IOSET0=((ms<<8)&data);
	IOSET0=rs;
	IOSET0=e;
	delay_milliseconds(2);
	IOCLR0=e;	
}
void lcd_integer(int p)
{
	signed int i=0;
	unsigned char array[10];
	while(p>0)
		{
			array[i++]=p%10;
			p=p/10;
		}
		for(--i;i>=0;i--)
		{
			dat(array[i]+48);
		}
}
void lcd_string(unsigned char *p)
{
	 unsigned int i;
 	for(i=0;p[i]!='\0';i++)
 	{
 		if(i==16)
 		{
 			comand(0xc0);
 		}
 		dat(p[i]);
 	}
}

