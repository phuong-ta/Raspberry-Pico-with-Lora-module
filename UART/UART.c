#include "uart.h"

void UART_init(){
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
}

char* on_uart_rx() {
    static char str[STRLEN]; // Make it static to preserve its value between function calls
    int pos = 0;
    while (uart_is_readable_within_us(UART_ID, 500000)) {
        char c = uart_getc(UART_ID);
        if ( c == '\n') {
            str[pos] = '\0';
            pos = 0; // start over after line is processed
            if(strlen(str))
            {
                return str;                
            }
        }else 
        {
            if (pos < STRLEN - 1) {
                str[pos++] = c;
            }
        }
    }
    // If no complete string is received yet, return NULL or an empty string
    return NULL;
}
