#include<LPC21xx.h>
void delay_seconds(unsigned int n){
	T0PR=15000000-1;
	T0TCR=0x01;
	while(T0TC<n);
	T0TCR=0x03;
	T0TCR=0x00;
}
void delay_milliseconds(unsigned int n){
	T0PR=15000-1;
	T0TCR=0x01;
	while(T0TC<n);
	T0TCR=0x03;
	T0TCR=0x00;
}
void delay_microseconds(unsigned int n){
	T0PR=15-1;
	T0TCR=0x01;
	while(T0TC<n);
	T0TCR=0x03;
	T0TCR=0x00;
}
