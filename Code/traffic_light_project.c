/*******************************************************
This program was created by the
CodeWizardAVR V3.14 Advanced
Automatic Program Generator
� Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : trsffic light
Version : 1
Date    : 6/2/2026
Author  : Nazanin Ghobadi
Team:
  Shahzad Hassanpur (Simulation module)
  Mohammad Barati (Hardware interface)
Comments:  
4-way Traffic Light Controller using ATmega16.
The system controls traffic flow for North-South (NS) and West-East (WE) directions
using a state machine with four states: - NS_GREEN - NS_YELLOW - WE_GREEN - WE_YELLOW
A 1-second time base is generated using Timer2 in asynchronous mode with a 32.768 kHz crystal.
Timer0 is used for fast multiplexing of a 4-digit 7-segment display.
The system continuously updates the countdown timer for each direction and switches states automatically when the timer reaches zero. 

Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 1.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/
#include <mega16.h>

//================ 7SEG DATA =================
// COMMON ANODE

flash unsigned char seg[10] =
{
    0b00111111, //0
    0b00000110, //1
    0b01011011, //2
    0b01001111, //3
    0b01100110, //4
    0b01101101, //5
    0b01111101, //6
    0b00000111, //7
    0b01111111, //8
    0b01101111  //9
};

//================ STATES =================

#define NS_GREEN   0
#define NS_YELLOW  1
#define WE_GREEN   2
#define WE_YELLOW  3

//================ VARIABLES =================

volatile unsigned char state = NS_GREEN;
volatile unsigned char timer = 30;

volatile unsigned char ns_value = 30;
volatile unsigned char we_value = 35;

volatile unsigned char digit_select = 0;

volatile bit sec_flag = 0;

//================ LIGHT OUTPUTS =================
// SAME AS YOUR OLD CODE

flash unsigned char outA[4] =
{
    0b00100001,
    0b00100010,
    0b00001100,
    0b00010100
};

flash unsigned char outB[4] =
{
    0b00100001,
    0b00100010,
    0b00001100,
    0b00010100
};

//================ TIMER0 =================
// 7SEG MULTIPLEX

interrupt [TIM0_OVF] void timer0_ovf_isr(void)
{
    PORTC = 0x00;   // all digits off
    PORTD = 0xFF;

    // NS TENS
    if(digit_select == 0)
    {
        PORTC = 1;
        PORTD = ~seg[ns_value / 10];

        digit_select = 1;
    }

    // NS ONES
    else if(digit_select == 1)
    {
        PORTC = 2;
        PORTD = ~seg[ns_value % 10];

        digit_select = 2;
    }

    // WE TENS
    else if(digit_select == 2)
    {
        PORTC = 4;
        PORTD = ~seg[we_value / 10];

        digit_select = 3;
    }

    // WE ONES
    else
    {
        PORTC = 8;
        PORTD = ~seg[we_value % 10];

        digit_select = 0;
    }
}
//================ TIMER2 =================
// 1 SECOND

interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
    sec_flag = 1;
}

//================ DISPLAY UPDATE =================

void update_display(void)
{
    if(state == NS_GREEN)
    {
        ns_value = timer;
        we_value = timer + 5;
    }

    else if(state == NS_YELLOW)
    {
        ns_value = timer;
        we_value = timer;
    }

    else if(state == WE_GREEN)
    {
        we_value = timer;
        ns_value = timer + 5;
    }

    else if(state == WE_YELLOW)
    {
        we_value = timer;
        ns_value = timer;
    }
}

//================ MAIN =================

void main(void)
{
    // LEDS
    DDRA = 0x3F;
    DDRB = 0x3F;

    // digit_select_select SELECT
    DDRC = 0x0F;

    // SEGMENTS
    DDRD = 0xFF;

    // INITIAL VALUES
    PORTC = 0x0F;
    PORTD = 0xFF;

    // INITIAL LIGHTS
    PORTA = outA[state];
    PORTB = outB[state];

    //================ TIMER0 =================

    // PRESCALER = 8
    // FOR MULTIPLEX

    TCCR0 = (1<<CS01);

    TCNT0 = 0;

    //================ TIMER2 =================

    // 32.768kHz CRYSTAL
    // OVERFLOW EACH 1 SECOND

    ASSR = (1<<AS2);

    TCCR2 =
        (1<<CS22) |
        (0<<CS21) |
        (1<<CS20);

    TCNT2 = 0;

    //================ INTERRUPTS =================

    TIMSK =
        (1<<TOIE0) |
        (1<<TOIE2);

    #asm("sei")

    // FIRST DISPLAY UPDATE
    update_display();

    while(1)
    {
        if(sec_flag)
        {
            sec_flag = 0;

            if(timer > 0)
            {
                timer--;
            }
            else
            {
                switch(state)
                {
                    case NS_GREEN:

                        state = NS_YELLOW;
                        timer = 5;

                    break;

                    case NS_YELLOW:

                        state = WE_GREEN;
                        timer = 30;

                    break;

                    case WE_GREEN:

                        state = WE_YELLOW;
                        timer = 5;

                    break;

                    case WE_YELLOW:

                        state = NS_GREEN;
                        timer = 30;

                    break;
                }

                PORTA = outA[state];
                PORTB = outB[state];
            }

            update_display();
        }
    }
}