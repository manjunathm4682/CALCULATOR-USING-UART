extern void delay_ms(unsigned int ms);
extern void delay_sec(unsigned int sec);

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;
typedef signed int s32;

extern void uart0_init(u32 baud);
extern void uart0_tx(u8 data);
extern void uart0_tx_integer(int );
extern u8 uart0_rx(void);
