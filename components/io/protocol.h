#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include "sdkconfig.h"

#define GRC_PREFIX "GRC"

#if CONFIG_IDF_TARGET_ESP32S3
#define IO_RECEIVER_NAME                                                       \
  (GRC_PREFIX "_"                                                              \
              "esp32c3")
#elif CONFIG_IDF_TARGET_ESP32C3
#define IO_RECEIVER_NAME                                                       \
  (GRC_PREFIX "_"                                                              \
              "esp32s3")
#endif

#define IO_SENDER_NAME (GRC_PREFIX "_" CONFIG_IDF_TARGET)

#define IO_SEND_HEADER  IO_SENDER_NAME
#define IO_RECV_HEADER  IO_RECEIVER_NAME
#define IO_CMD_BUTTON   "BUTTON"
#define IO_CMD_SEL_DEMO "SEL_DEMO"
#define IO_CMD_RESTART  "RESTART"

#define IO_SEND_HEADER_LEN (sizeof(IO_SEND_HEADER) - 1)
#define IO_RECV_HEADER_LEN (sizeof(IO_RECV_HEADER) - 1)
#define IO_MAX_MSG_LEN     256

#endif // _PROTOCOL_H_
