// Default clock source is internal 8MHz RC oscillator
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

int main()
{
  DDRA |= (1 << PA7);

  while (1)
  {
    PORTA |= (1 << PA7);
    _delay_ms(1000);
    PORTA &= ~(1 << PA7);
    _delay_ms(1000);
  }
  return 0;
}