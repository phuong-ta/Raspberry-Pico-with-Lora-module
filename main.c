#include <stdio.h>
#include <string.h>
#include <ctype.h>
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
#define STRLEN 80


char* on_uart_rx() {
    static char str[STRLEN]; // Make it static to preserve its value between function calls
    int pos = 0;
    /**/
    while (uart_is_readable_within_us(UART_ID, 500000)) {
       // while (uart_is_readable(UART_ID)) {
            char c = uart_getc(UART_ID);

            if ( c == '\n') {
                str[pos] = '\0';
                pos = 0; // start over after line is processed
                if(strlen(str)){
                    return str;
                }
            } else {
                if (pos < STRLEN - 1) {
                    str[pos++] = c;
                }
            }
        //}
    }
    // If no complete string is received yet, return NULL or an empty string
    //pos=0;
    return NULL;
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
    const uint8_t command1[] = "AT\r\n";
    const uint8_t command2[] = "AT+VER\r\n";
    const uint8_t command3[] = "AT+ID=DevEui\r\n";
    char* receivedString;
    /*
    while(1){
        if(!gpio_get(SW_PIN)) {
            sleep_ms(30);
            if(!gpio_get(SW_PIN)){
                // 
                while (uart_is_readable(UART_ID)){
                    uart_getc(UART_ID);
                }
                uart_write_blocking(uart0, command1, strlen(command1));
                while (uart_is_readable_within_us(UART_ID, 500000)){
                    uart_getc(UART_ID);
                    flag=1;
                }
                if(flag){
                    printf("Connected to LoRa module\n");
                    flag=0;
                }else{
                    printf("Module not responding\n");
                }
                //receivedString = on_uart_rx();
            }
            while(!gpio_get(SW_PIN));
    }
    }

    */
    
    while(true) 
    {    
        if(flag==0 && !gpio_get(SW_PIN)) {
            //count++;
            count=1;
            flag=1;
        }else if (gpio_get(SW_PIN))
        {
            flag=0;
        }
        switch (count)
            {
            case 1:
            printf("Counter: %d\n", count);
                for (size_t i = 0; i < 5; i++)
                {
                    uart_write_blocking(uart0, command1, strlen(command1));
                    receivedString = on_uart_rx();
                    if (receivedString != NULL) 
                    {
                        printf("Connected to LoRa module: %s\n", receivedString);
                        count=2;
                        break;
                    }
                    if (receivedString == NULL )
                    {
                        count=0;
                        printf("Module not responding\n");
                    }  
                }
                //printf("%d\n",count);
                break;
            case 2:
            printf("Counter: %d\n", count);
                uart_write_blocking(uart0, command2,  strlen(command2));
                receivedString = on_uart_rx();
                if (receivedString != NULL) 
                {
                    printf("Version: %s\n", receivedString);
                    count=3;
                }else
                {
                    printf("Module stopped responding\n");
                    count=0;
                }
                break;
            case 3:
                printf("Counter: %d\n", count);
                uart_write_blocking(uart0, command3,  strlen(command3));
                receivedString = on_uart_rx();
                
                if (receivedString != NULL) 
                {
                    printf("%s\n", receivedString);
                }
                break;
            default:
                break;
            }
        sleep_ms(400);
    }

    return 0;
}
