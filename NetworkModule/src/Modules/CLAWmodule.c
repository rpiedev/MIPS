#define F_CPU 8000000UL
#define BAUD 38400
#define UBRRval 12

#define bufferSize 256

#define ANGchange 30

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
//                           LEN MOD- -=====ID======- -TYPE=- VERSION
const unsigned char mod[] = {0xa,0x18,0x0,0x0,0x0,0x2,0x0,0x2,0x0,0x1};

unsigned char buffer[bufferSize];
uint8_t bufferReadIndex = 0;
uint8_t bufferWriteIndex = 0;
uint8_t workingLen = 0;

uint16_t workingAng;
uint16_t goalAng;

//* SERVO
void setupSERVO() {
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    
    TCCR1A = (1 << COM1B1) | (1 << WGM11); // sets pin OC1B low on match - ICR1 is top
    TCCR1B = (1 << CS11) |(1 << WGM12) | (1 << WGM13); // 8x prescaler - ICR1 is top
    ICR1 = 20000; // freq = 8000000 / (20000 * 8 [prescale]) = 50 HZ or 20ms cycle
    OCR1B = 1500; // ( 1500 / 20000 ) * 20ms = ~1.5ms 
    workingAng = 1500;
    goalAng = 1500;
    // 2500 one side - 1500 middle  - 500 other side


    DDRA |= (1 << PA7); //Sets PA7 to output
    TOCPMSA1=0b00010000; // TOCC6 (PA7) to OC1B
    TOCPMCOE=0b01000000; // enable tocc6

    // Timer 1 Overflow enable
    TIMSK1 = (1<<TOIE1);
}
void SERVOto(uint8_t angle) {
    if(angle < 90)
        angle = 90;
    if(angle > 180) 
        angle = 180;
    goalAng = 500 + (int)(11.11 * angle);
}
void SERVOup(uint8_t scale) {
    int interval = (scale+1) * 10;
    if(goalAng >= 2500 - interval) 
        goalAng = 2500;
    else
        goalAng += interval;
}
void SERVOdown(uint8_t scale) {
    int interval = (scale+1) * 10;
    if(goalAng <= 1500+interval) 
        goalAng = 1500;
    else
        goalAng -= interval;
}
void SERVOupd() {
    if(workingAng == goalAng) return;

    if(workingAng > goalAng) {
        int diff = workingAng - goalAng;
        if (diff > ANGchange) 
            diff = ANGchange;
        workingAng -= diff;
    } else if(workingAng < goalAng) {
        int diff = goalAng - workingAng;
        if (diff > ANGchange) 
            diff = ANGchange;
        workingAng += diff;
    }

    OCR1B = workingAng;
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
        bufferReadIndex+=4; //address
        if(buffer[bufferReadIndex] >= 230 && buffer[bufferReadIndex] < 240) {
            SERVOdown(read()-230);
        } else if(buffer[bufferReadIndex] >= 240 && buffer[bufferReadIndex] < 250) {
            SERVOup(read()-240);
        } else
            SERVOto(read());
    }
}

void checkBuffer() {
    uint8_t diff = bufferWriteIndex >= bufferReadIndex ? bufferWriteIndex - bufferReadIndex : bufferWriteIndex + (bufferSize - bufferReadIndex);
    if(bufferReadIndex == bufferWriteIndex) return; // maybe enable sleep mode?
    if(buffer[bufferReadIndex] < 2) {
        bufferReadIndex+=1;
        return;
    }
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
ISR(TIMER1_OVF_vect) {
    //USART1_Transmit(11);
    SERVOupd();
    //TCCR1B &= ~(1 << CS11); // turn off timer1
}

//* MAIN
int main() {
    setupSERVO();
    sei();
    USART_Init();
    
    while (1) {
        checkBuffer();
    }
}