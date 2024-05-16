// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 6000000
void init(void);
void pwmchange(void);
void delay();
unsigned char pwmhigh10,pwmlow10;
unsigned char pwmhigh50,pwmlow50;
unsigned char pwmhigh80,pwmlow80;

void main(void)
{
    init();
    while(1)
    {
        pwmchange();
    }
}

void init()
{
    TRISC=0xFB; //1111 1011   seeting pwm output RC2=0
    CCP1CON=0x0C; //0000 1100 pwm on mode ie 11xx so  0000 1100
    
    T2CON=0x06; //0000 0110 RC2=1 due to timer on and 10 IS prescale is 16
    PR2= 0x5E; //0101 1110   Formula for PR2= FOSC(4*PWM_FREQ*TMR2_PRESCALE))-1 = 6000000(4*1*16)))-1 = 94 (0x5E)
    pwmhigh10=0x09; // 10% valve is 38 binary 0000 1001 so here last 2 bit is low remaing eight is high so 0000 1001 =>0x09
    pwmlow10=0x20;// 10% valve is 38 binary 0010 0110 so here last 2 bit is low remaing eight is high so low is 10 =>0x02 but for 4th and 5bit valve 0x20
    pwmhigh50=0x2F;// 50% valve is 188 binary 0010 1111  so here last 2 bit is low remaing eight is high so high is  =>0010 1111=>0x2F
    pwmlow50=0x00;// 50% valve is 188 binary 1011 1100  so here last 2 bit is low remaing eight is high so low is  =>00=>0x00
    pwmhigh80=0x4B;// 80% valve is 188 binary 0010 1011  so here last 2 bit is low remaing eight is high so high is  =>0x4B
    pwmlow80=0x00;// 80% valve is 188 binary 1011 1100  so here last 2 bit is low remaing eight is high so low is  =>00
}
void pwmchange()
{
    CCPR1L=pwmhigh10;  //  0000 1001  =>0x09
    CCP1CON= (CCP1CON&0xCF)|pwmlow10;   //low 0000 0010 =>0x20
    delay(); //3 sec delay
    CCPR1L=pwmhigh50;  //0010 1111=>0x2F
    CCP1CON=(CCP1CON&0xCF)|pwmlow50;  //0000 0000 =>0x00
    delay();//3 sec delay
    CCPR1L=pwmhigh80;  //0100 1011 =>0x4B
    CCP1CON=(CCP1CON&0xCF)|pwmlow80; //0000 0000 =>0x00
    delay();//3 sec delay
    
}
void delay()
{
    __delay_ms(3000);
}