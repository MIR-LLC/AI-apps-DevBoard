#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "esp_log.h"
#include "string.h"

#include "uart.h"

#include "driver/gpio.h"
#include "driver/uart.h"
#define UART_PORT_NUM UART_NUM_1
#define UART_TX_PIN   GPIO_NUM_21
#define UART_RX_PIN   GPIO_NUM_20
#define UART_RTS_PIN  UART_PIN_NO_CHANGE
#define UART_CTS_PIN  UART_PIN_NO_CHANGE

#define BUF_SIZE 2048

int uart_init() {
  /* Configure parameters of an UART driver,
   * communication pins and install the driver */
  uart_config_t uart_config = {
    .baud_rate = 115200,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_DEFAULT,
  };
  int intr_alloc_flags = 0;

  ESP_ERROR_CHECK(
    uart_driver_install(UART_PORT_NUM, BUF_SIZE, 0, 0, NULL, intr_alloc_flags));
  ESP_ERROR_CHECK(uart_param_config(UART_PORT_NUM, &uart_config));
  ESP_ERROR_CHECK(uart_set_pin(UART_PORT_NUM, UART_TX_PIN, UART_RX_PIN,
                               UART_RTS_PIN, UART_CTS_PIN));

  return 0;
}

int uart_release() {
  ESP_ERROR_CHECK(uart_driver_delete(UART_PORT_NUM));
  return 0;
}

int uart_send(void *data, size_t len, size_t xTicksToWait) {
  uart_write_bytes(UART_PORT_NUM, data, len);
  return uart_wait_tx_done(UART_PORT_NUM, xTicksToWait);
}

int uart_read(void *data, size_t len, size_t xTicksToWait) {
  return uart_read_bytes(UART_PORT_NUM, data, len, xTicksToWait);
}

int uart_flush_buffer() {
  size_t buffered_data = 0;
  uart_get_buffered_data_len(UART_PORT_NUM, &buffered_data);
  int ret = ESP_OK;
  if (buffered_data) {
    ret = uart_flush(UART_PORT_NUM);
  }
  return ret;
}

#ifdef __cplusplus
}
#endif // __cplusplus
