#define F_CPU 8000000UL
#define BAUD 9600
#define UBRRval 51

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile char currentChar = 0;

void setupLED() {
    DDRA |= (1 << PA7); //Sets PA7 to output
    PORTA |= (1 << PA7); //Sets PA7 to high
}
void LEDon() {
    PORTA |= (1 << PA7); //Sets PA7 to high
}
void LEDoff() {
    PORTA &= ~(1 << PA7); //Sets PA7 to low
}
void LEDtoggle() {
    PORTA ^= (1 << PA7);
}

void USART1_Init () {
    // Sets baud rate
    UBRR1H = (unsigned char) (UBRRval>>8);
    UBRR1L = (unsigned char) UBRRval;
    // Enable reciever and reciever complete interrupt
    UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
    // Set frame data 8data, 2stop
    UCSR1C = (1<<USBS1) | (3<<UCSZ10);
}
void USART1_Transmit (unsigned char data) {
    while (!(UCSR1A & (1<<UDRE1)));
    // Insert data to buffer, then sends
    UDR1 = data;
}

ISR(USART1_RX_vect) {
    while ( !(UCSR1A & (1<<RXC1)) );
    
    currentChar = UDR1;
}

int main() {
    setupLED();
    sei();
    USART1_Init();
    
    while (1) {
        if(currentChar == 'f') {
            LEDoff();
            USART1_Transmit('c');
        } else if(currentChar == 'n') {
            LEDon();
            USART1_Transmit('l');
        }
    }
}