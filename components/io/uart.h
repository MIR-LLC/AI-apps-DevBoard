#ifndef _UART_H_
#define _UART_H_

#include "stddef.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

int uart_init();
int uart_release();
int uart_send(void *data, size_t len, size_t xTicksToWait);
int uart_read(void *data, size_t len, size_t xTicksToWait);
int uart_flush_buffer();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _UART_H_
