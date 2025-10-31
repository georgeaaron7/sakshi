#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "inc\tm4c123gh6pm.h"
#include "driverlib/sysctl.h"

#define LCD_clear()   LCD_command(0x01)
#define LCD_origin()  LCD_command(0x02)
#define LCD_row1()    LCD_command(0x80)
#define LCD_row2()    LCD_command(0xC0)

void shift_out1(unsigned char str);
void LCD_command(unsigned char command);
void LCD_putc(unsigned char ascii);
void LCD_puts(const unsigned char *lcd_string);
void LCD_init(void);
int  isPrime(int n);

static unsigned char PP0 = 0x00;  
static unsigned char PP1 = 0x00;  

int main(void)
{
    SYSCTL_RCGCGPIO_R |= 0x39;
    (void)SYSCTL_RCGCGPIO_R;

    GPIO_PORTD_AFSEL_R |= 0x08;
    GPIO_PORTD_DEN_R   &= ~0x08;
    GPIO_PORTD_AMSEL_R |= 0x08;

    GPIO_PORTE_DIR_R |= 0x0C;
    GPIO_PORTE_DEN_R |= 0x0C;

    GPIO_PORTA_DIR_R |= 0x20;
    GPIO_PORTA_DEN_R |= 0x20;

    GPIO_PORTF_DIR_R |= 0x01;
    GPIO_PORTF_DEN_R |= 0x01;

    LCD_init();
    LCD_clear();
    LCD_origin();

    int count = 0, num = 2, sum = 0;
    while (count < 20) {
        if (isPrime(num)) {
            sum += num;
            count++;
        }
        num++;
    }

    char line1[17];
    snprintf(line1, sizeof(line1), "Sum=%d", sum);
    LCD_row1();
    LCD_puts((const unsigned char*)line1);

    LCD_row2();
    LCD_puts((const unsigned char*)"First 20 Primes");

    while (1) {
        GPIO_PORTF_DATA_R ^= 0x01;
        SysCtlDelay(200000);
    }
}

int isPrime(int n)
{
    if (n <= 1) return 0;
    for (int i = 2; i * i <= n; i++) {
        if ((n % i) == 0) return 0;
    }
    return 1;
}

void LCD_puts(const unsigned char *lcd_string)
{
    while (*lcd_string) {
        LCD_putc(*lcd_string++);
    }
}

void LCD_init(void)
{
    SysCtlDelay(500000);
    LCD_command(0x30);
    SysCtlDelay(500000);
    LCD_command(0x30);
    SysCtlDelay(500000);
    LCD_command(0x30);
    SysCtlDelay(500000);
    LCD_command(0x20);
    SysCtlDelay(500000);

    LCD_command(0x28);
    SysCtlDelay(500000);
    LCD_command(0x0C);
    SysCtlDelay(500000);
    LCD_command(0x06);
    SysCtlDelay(500000);
    LCD_command(0x01);
    SysCtlDelay(500000);
}

void LCD_putc(unsigned char ascii)
{
    unsigned char num = ascii;

    num = ((num & 0x11) << 3) | ((num & 0x22) << 1) |
          ((num & 0x44) >> 1) | ((num & 0x88) >> 3);
    ascii = num;

    PP1 = (PP1 & 0xF0) | ((ascii >> 4) & 0x0F);
    PP1 = PP1 | 0xA0;
    PP1 = PP1 & ~(0x40);
    shift_out1(PP1);
    SysCtlDelay(500);
    PP1 = PP1 & ~(0x20);
    shift_out1(PP1);
    SysCtlDelay(500);

    PP1 = (PP1 & 0xF0) | (ascii & 0x0F);
    PP1 = PP1 | 0xA0;
    PP1 = PP1 & ~(0x40);
    shift_out1(PP1);
    SysCtlDelay(500);
    PP1 = PP1 & ~(0x20);
    shift_out1(PP1);
    SysCtlDelay(500);
}

void LCD_command(unsigned char command)
{
    unsigned char num = command;

    num = ((num & 0x11) << 3) | ((num & 0x22) << 1) |
          ((num & 0x44) >> 1) | ((num & 0x88) >> 3);
    command = num;

    PP0 = (PP0 & 0xF0) | ((command >> 4) & 0x0F);
    PP0 = PP0 & (~(3 << 7));
    PP0 = PP0 | 0x20;
    shift_out1(PP0);
    SysCtlDelay(500);
    PP0 = PP0 & ~(0x20);
    shift_out1(PP0);
    SysCtlDelay(500);

    PP0 = (PP0 & 0xF0) | (command & 0x0F);
    PP0 = PP0 & (~(3 << 7));
    PP0 = PP0 | 0x20;
    shift_out1(PP0);
    SysCtlDelay(500);
    PP0 = PP0 & ~(0x20);
    shift_out1(PP0);
    SysCtlDelay(500);
}

void shift_out1(unsigned char str)
{
    unsigned char j = 0, check;

    GPIO_PORTA_DATA_R &= ~0x20;
    for (j = 0; j <= 7; j++) {
        GPIO_PORTE_DATA_R &= ~0x08;
        check = (str & (1 << j));
        if (check) {
            GPIO_PORTE_DATA_R |= 0x04;
        } else {
            GPIO_PORTE_DATA_R &= ~0x04;
        }
        GPIO_PORTE_DATA_R |= 0x08;
    }
    GPIO_PORTA_DATA_R |= 0x20;
}
