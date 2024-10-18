#define F_CPU 8000000UL
#define BAUD 38400
#define UBRRval 12

#define bufferSize 256

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

const unsigned char mod[] = {0xa,0x18,0x0,0x0,0x0,0x1,0x0,0x1,0x0,0x1};

unsigned char buffer[bufferSize];
uint8_t bufferReadIndex = 0;
uint8_t bufferWriteIndex = 0;
uint8_t workingLen = 0;

//* LED
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

//* Hardware USART
void USART_Init () {
    // Sets baud rate for RX1 and RX0
    UBRR1H = (unsigned char) (UBRRval>>8);
    UBRR1L = (unsigned char) UBRRval;

    UBRR0H = (unsigned char) (UBRRval>>8);
    UBRR0L = (unsigned char) UBRRval;
    // Enable reciever and reciever complete interrupt
    UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);

    UCSR0B = (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0);
    // Set frame data 8data, 2stop
    UCSR1C = (1<<USBS1) | (3<<UCSZ10);

    UCSR0C = (1<<USBS0) | (3<<UCSZ00);
}
void USART1_Transmit (unsigned char in) {
    while (!(UCSR1A & (1<<UDRE1)));
    UDR1 = in;
}
void USART0_Transmit (unsigned char in) {
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = in;
}

//* Buffer-Packet Functions
static inline unsigned char read() {
    uint8_t oldIndex = bufferReadIndex;
    bufferReadIndex+=1;
    return buffer[oldIndex];
}
static inline void write(unsigned char in) {
    buffer[bufferWriteIndex] = in;
    bufferWriteIndex+=1;
}
static inline void recHub() {
    for(uint8_t i=0;i<10;i++) {
        USART1_Transmit(mod[i]);
    }
    USART0_Transmit(2);
    USART0_Transmit(0x9);
}
static inline void recMod() {
    USART1_Transmit(0xa);
    USART1_Transmit(0x18);
    for(uint8_t i=0;i<8;i++) {
        USART1_Transmit(read());
    }
}
static inline void recAct(unsigned char len) {
    _Bool forThis = 1;
    for(uint8_t i=0;i<4;i++) {
        if(buffer[bufferReadIndex+i] != mod[i+2])
            forThis = 0;
    }

    if(!forThis) {
        USART0_Transmit(len);
        USART0_Transmit(0x25);
        for(uint8_t i=0;i<len-2;i++) {
            USART0_Transmit(read());
        }
    } else {
        bufferReadIndex+=4;
        if(read() == 0x1)
            LEDon();
        else
            LEDoff();
    }
}

void checkBuffer() {
    uint8_t diff = bufferWriteIndex >= bufferReadIndex ? bufferWriteIndex - bufferReadIndex : bufferWriteIndex + (bufferSize - bufferReadIndex);
    if(bufferReadIndex == bufferWriteIndex) return; // maybe enable sleep mode?
    if(diff < buffer[bufferReadIndex]) return;
    unsigned char packetLen = read();
    unsigned char packetType = read();
    switch (packetType) {
        case 0x9:
            recHub();
            break;
        case 0x18:
            recMod();
            break;
        case 0x25:
            recAct(packetLen);
            break;
    }

}

//* Interrupts
ISR(USART1_RX_vect) {
    while ( !(UCSR1A & (1<<RXC1)) ); //Wait for RXC
    write(UDR1);
}
ISR(USART0_RX_vect) {
    while ( !(UCSR0A & (1<<RXC0)) ); //Wait for RXC
    write(UDR0);
}

//* MAIN
int main() {
    setupLED();
    sei();
    USART_Init();
    
    while (1) {
        checkBuffer();
    }
}