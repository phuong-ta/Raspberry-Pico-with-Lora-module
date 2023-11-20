#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include"UART.h"

#define SW_PIN 8
#define UART_ID uart0

int main() {
    // Initialize chosen serial port
    stdio_init_all();
    UART_init();

    gpio_init(SW_PIN);
    gpio_set_dir(SW_PIN, GPIO_IN);
    gpio_pull_up(SW_PIN);

    uint count = 0;
    bool flag = 0;
    const uint8_t command1[] = "AT\r\n";
    const uint8_t command2[] = "AT+VER\r\n";
    const uint8_t command3[] = "AT+ID=DevEui\r\n";
    char* receivedString;
    
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
                for (size_t i = 0; i < 5; i++)
                {
                    uart_write_blocking(UART_ID, command1, strlen(command1));
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
                break;
            case 2:
                uart_write_blocking(UART_ID, command2,  strlen(command2));
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
                uart_write_blocking(UART_ID, command3,  strlen(command3));
                receivedString = on_uart_rx();
                if (receivedString != NULL) 
                {
                    printf("%s\n", receivedString);
                }
                count=0;
                break;
            default:
                break;
            }
        sleep_ms(100);
    }
    return 0;
}
