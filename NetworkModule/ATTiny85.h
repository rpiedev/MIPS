
#ifndef F_CPU
#define F_CPU               8000000
#endif

#ifndef BAUDRATE
#define BAUDRATE            9600
#endif

#define CYCLES_PER_BIT      ( F_CPU / BAUDRATE )
#if ( CYCLES_PER_BIT > 255 )
#define DIVISOR             8
#define PRESCALE            2
#else
#define DIVISOR             1
#define PRESCALE            1
#endif
#define FULL_BIT_TICKS      ( CYCLES_PER_BIT / DIVISOR )