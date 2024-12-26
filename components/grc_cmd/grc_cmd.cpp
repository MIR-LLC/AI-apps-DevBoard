#include <ctype.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"

#include "argtable3/argtable3.h"
#include "esp_console.h"

#include "grc_cmd.h"
#include "grc_defs.h"
#include "protocol.h"
#include "uart.h"

static void register_button_cmd(void);
static void register_flush_cmd(void);
static void register_select_demo_cmd(void);
static void register_restart_cmd(void);
void register_grc_cmd(void) {
  register_button_cmd();
  register_flush_cmd();
  register_select_demo_cmd();
  register_restart_cmd();
}

int uart_msg_send(const char *msg, size_t len) {
  if (uart_send((void *)IO_SEND_HEADER, IO_SEND_HEADER_LEN,
                pdMS_TO_TICKS(1000)) < 0) {
    printf("UART send timeout\n");
    return -1;
  }

  uint8_t l = len;
  if (uart_send((void *)&l, 1, pdMS_TO_TICKS(1000)) < 0) {
    printf("UART send timeout\n");
    return -1;
  }

  if (uart_send((void *)msg, l, pdMS_TO_TICKS(1000)) < 0) {
    printf("UART send timeout\n");
    return -1;
  }
  return 0;
}

static struct {
  struct arg_str *button_name;
  struct arg_str *button_ev;
  struct arg_end *end;
} button_args;

static int button_cmd(int argc, char **argv) {
  int nerrors = arg_parse(argc, argv, (void **)&button_args);
  if (nerrors != 0) {
    arg_print_errors(stderr, button_args.end, argv[0]);
    return 1;
  }
  printf("button cmd: %s, %s\n", button_args.button_name->sval[0],
         button_args.button_ev->sval[0]);

  char msg_buf[IO_MAX_MSG_LEN] = {0};

  int n =
    snprintf(msg_buf, sizeof(msg_buf), "%s_%s_%s", IO_CMD_BUTTON,
             button_args.button_name->sval[0], button_args.button_ev->sval[0]);
  printf("send: bytes=%d, %s\n", n, msg_buf);

  if (n > 0 && n < IO_MAX_MSG_LEN) {
    uart_msg_send(msg_buf, n);
  } else {
    printf("incorrect message length\n");
  }

  return 0;
}

static void register_button_cmd(void) {
  button_args.button_name =
    arg_str1(NULL, NULL, GRC_BUTTON_NAMES_STR, "Choose button");
  button_args.button_ev =
    arg_str1(NULL, NULL, "<CLICK|HOLD>", "Choose button event");
  button_args.end = arg_end(2);

  const esp_console_cmd_t cmd = {
    .command = "button",
    .help = "Generate button event",
    .hint = NULL,
    .func = &button_cmd,
    .argtable = &button_args,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

static int flush(int argc, char **argv) {
  uart_flush_buffer();
  printf("flushed UART buffer\n");
  return 0;
}

static void register_flush_cmd(void) {
  const esp_console_cmd_t cmd = {
    .command = "flush",
    .help = "Flush UART buffer",
    .hint = NULL,
    .func = &flush,
    .argtable = NULL,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

static struct {
  struct arg_str *app_name;
  struct arg_end *end;
} select_demo_args;

static int select_demo_cmd(int argc, char **argv) {
  int nerrors = arg_parse(argc, argv, (void **)&select_demo_args);
  if (nerrors != 0) {
    arg_print_errors(stderr, select_demo_args.end, argv[0]);
    return 1;
  }
  printf("Select app: %s\n", select_demo_args.app_name->sval[0]);

  char msg_buf[IO_MAX_MSG_LEN] = {0};

  int n = snprintf(msg_buf, sizeof(msg_buf), "%s_%s", IO_CMD_SEL_DEMO,
                   select_demo_args.app_name->sval[0]);
  printf("send: bytes=%d, %s\n", n, msg_buf);

  if (n > 0 && n < IO_MAX_MSG_LEN) {
    uart_msg_send(msg_buf, n);
  } else {
    printf("incorrect message length\n");
  }

  return 0;
}

static void register_select_demo_cmd(void) {
  select_demo_args.app_name =
    arg_str1(NULL, NULL, GRC_DEMO_NAMES_STR, "Select demo application");
  select_demo_args.end = arg_end(1);

  const esp_console_cmd_t cmd = {
    .command = "select_demo",
    .help = "Select demo application",
    .hint = NULL,
    .func = &select_demo_cmd,
    .argtable = &select_demo_args,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}

static int restart_cmd(int argc, char **argv) {
  uart_msg_send(IO_CMD_RESTART, sizeof(IO_CMD_RESTART) - 1);
  return 0;
}

static void register_restart_cmd(void) {
  const esp_console_cmd_t cmd = {
    .command = "restart",
    .help = "Restart esp32s3",
    .hint = NULL,
    .func = &restart_cmd,
    .argtable = NULL,
  };
  ESP_ERROR_CHECK(esp_console_cmd_register(&cmd));
}
