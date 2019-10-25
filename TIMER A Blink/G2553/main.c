#include <msp430.h>

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 //stop watchdog timer

  P1DIR |= BIT0;
  P1DIR |= BIT6;                   //set direction to output for pins 1.0 and 1.6
  P1OUT |= ~BIT0;
  P1OUT |= ~BIT6;                  //turn off LED pins
  
  TA0CCTL0 = CCIE;                          //enable timer interrupt in compare mode
  TA0CTL = TASSEL_2 + MC_1 + ID_3;          //use smclk, in Up mode, clock divided by 8
  TA0CCR0 = 30000;                          //clk = 10^6 Hz / (8*TA0CCR0)

  TA1CCTL0 = CCIE;                          //enable timer interrupt in compare mode
  TA1CTL = TASSEL_2 + MC_1 + ID_3;          //smclk, Up mode, clock divided by 8
  TA1CCR0 = 20000;                          //clk = 10^6 Hz / (8*TA0CCR0)

  __bis_SR_register(LPM0 + GIE);            //enable low power mode and interrupts
}

#pragma vector=TIMER0_A0_VECTOR             //interrupt from Timer0_A0
__interrupt void TIMER_A0(void)             //interrupt name
{
  P1OUT ^= BIT0;                            //toggle LED
}

#pragma vector=TIMER1_A0_VECTOR             //interrrupt from Timer1_a0
__interrupt void TIMER_A1(void)
{
  P1OUT ^= BIT6;                            //toggle LED
}
