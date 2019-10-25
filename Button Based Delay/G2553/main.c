
#include <msp430.h>

long frequency;                                 //initialize variable as long due to possible size
int reset = 0;                                  //initialize variable
int starttime = 0;                                  //initialize variable
int difference=0;//initialize variable

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                   //stop watchdog timer

    CCTL0 |= CCIE;                              //enable timer interrupt in compare mode
    CCTL0 &= ~TAIFG;                            //clear interrupt flag
    TA0CTL = TASSEL_1 + MC_2 +ID_3;                     //ACLK, continuous mode, divide by 8
    TA0CCR0 = 0;                                //initialize timer A0 capture control register
    frequency = 0;                              //initialize frequency variable

    P1IE  |= BIT3;                              //enable interrupt on P1.3
    P1IES |= BIT3;                              //Hi/Lo edge triggered
    P1REN |= BIT3;                              //enable pull up resistor
    P1OUT |= BIT3;                              //turn on pin
    P1IFG &= ~BIT3;                             //clear interrupt flag
    P1DIR |= BIT0;                            //set direction to output

    __bis_SR_register(GIE);                     //enable interrupts

    while(1){                                   //always remain in while loop
        if(reset == 1)                          //if reset = 1
        {
            reset = 0;                          //reset variable
            if(difference <= 0){                   //if counter resets between holds
                TA0CCR0 = 65535+difference+TA0R; //TA0 = 2^16 - when button is pressed + when button is let go + last button press time
                frequency = 65535+difference;    //update frequency value
            }
            else
            {
                TA0CCR0=difference+TA0R;         //TA0 = when button is released - button pressed + last button press time
                frequency = difference;          //update frequency value
            }
        }
    }
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
    P1OUT ^= BIT0;                                  //toggle LED
    TA0CCR0 = TA0CCR0 + frequency;                  //increment TA0CCR0
}
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    if(P1IES & BIT3)                                //if button is pressed
    {
        starttime = TA0R;                               //time button is pressed
    }
    else
    {
        difference= TA0R-starttime;
        reset = 1;                                  //set reset to 1 to run frequency setting code again
    }

    P1IES ^= BIT3;                                  //Lo/Hi edge triggered
    P1IFG &= ~BIT3;                                 //reset interrupt flag
}
