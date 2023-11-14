#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define SW_PIN 8
#define UART_ID uart0
#define BAUD_RATE 9600
#define UART_TX_PIN 0
#define UART_RX_PIN 1
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE
#define STRLEN 40


// RX interrupt handler
void on_uart_rx() {
    char str[STRLEN];
    int pos = 0;
    if (uart_is_readable_within_us(UART_ID,500)){
        while(uart_is_readable(UART_ID)) 
        {
            char c = uart_getc(UART_ID);
            printf("%c", c);
            if(c == '\r' || c == '\n') 
            {
                str[pos] = '\0';
                if(strlen(str)>3){
                    printf("received: %s", str);
                }
                pos = 0; // start over after line is printed
            }
            else 
            {
                if(pos < STRLEN - 1) {
                str[pos++] = c;
                }
            }
        }
    }
}

int main() {
    // Initialize chosen serial port
    stdio_init_all();

    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);

    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);

    uint count = 0;
    bool flag = 0;
    //const uint8_t send[] = "AT\r\n";

    while(true) 
    {
        if(flag==0 && !gpio_get(SW_PIN)) {
            count++;
            flag=1;
            printf("Counter: %u\n", count);
            switch (count)
            {
            case 1:
                for (size_t i = 0; i < 5; i++)
                {
                    printf("try to connect 5 times\n");
                }
                break;
            case 2:
                printf("print version!");
                break;
            case 3:
                printf("print DevEui");
                break;
            default:
                break;
            }
        }else if (gpio_get(SW_PIN))
        {
            flag=0;
        }
        
        if (count>=3)
        {
            count=0;
        }
        
        sleep_ms(100);
    }
}
