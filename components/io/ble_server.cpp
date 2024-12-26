#include "ble_server.h"

#include "NimBLEDevice.h"

#include "freertos/FreeRTOS.h"
#include "freertos/message_buffer.h"
#include "freertos/projdefs.h"

#include "esp_log.h"

static const char *TAG = "ble_server";

static MessageBufferHandle_t xMessageBuffer = NULL;

#define MAX_SEND_BYTES  20
#define MSG_BUFFER_SIZE 256

BLEServer *pServer = NULL;
BLECharacteristic *pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

#define DEVICE_NAME         "GRC-esp32c3"
#define SERVICE_UUID        "FFE0"
#define CHARACTERISTIC_UUID "FFE1"

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer, BLEConnInfo &connInfo) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer *pServer, BLEConnInfo &connInfo, int reason) {
    deviceConnected = false;
  }
};

class CharacteristicCallbacks : public NimBLECharacteristicCallbacks {
  void onStatus(NimBLECharacteristic *pCharacteristic, int code) {
    ESP_LOGD(TAG, "Notification/Indication return code: %d", code);
  }

  void onSubscribe(NimBLECharacteristic *pCharacteristic,
                   NimBLEConnInfo &connInfo, uint16_t subValue) {
    std::string str = "Client ID: ";
    str += connInfo.getConnHandle();
    str += " Address: ";
    str += connInfo.getAddress().toString();
    if (subValue == 0) {
      str += " Unsubscribed to ";
    } else if (subValue == 1) {
      str += " Subscribed to notfications for ";
    } else if (subValue == 2) {
      str += " Subscribed to indications for ";
    } else if (subValue == 3) {
      str += " Subscribed to notifications and indications for ";
    }
    str += std::string(pCharacteristic->getUUID());

    ESP_LOGD(TAG, "%s", str.c_str());
  }
};

void connectedTask(void *pv) {
  for (;;) {
    // connected
    if (deviceConnected) {
      uint8_t buffer[MAX_SEND_BYTES] = {0};
      auto xBytes = xMessageBufferReceive(xMessageBuffer, buffer,
                                          MAX_SEND_BYTES, pdMS_TO_TICKS(5000));
      ESP_LOGD(TAG, "bytes received=%u", xBytes);
      if (xBytes) {
        pCharacteristic->setValue(buffer, xBytes);
        pCharacteristic->notify();
      }
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
      vTaskDelay(pdMS_TO_TICKS(
        500)); // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising(); // restart advertising
      ESP_LOGD(TAG, "start advertising");
      oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
      // do stuff here on connecting
      oldDeviceConnected = deviceConnected;
    }

    vTaskDelay(
      pdMS_TO_TICKS(100)); // Delay between loops to reset watchdog timer
  }
  vTaskDelete(NULL);
}

int ble_server_init() {
  xMessageBuffer = xMessageBufferCreate(MSG_BUFFER_SIZE);
  if (xMessageBuffer == NULL) {
    ESP_LOGE(TAG, "Error creating msg buffer");
    return -1;
  }

  // Create the BLE Device
  BLEDevice::init(DEVICE_NAME);

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID, NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY);

  static CharacteristicCallbacks chrCallbacks;
  pCharacteristic->setCallbacks(&chrCallbacks);

  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);

  auto xReturned =
    xTaskCreate(connectedTask, "connectedTask", 2048, NULL, 1, NULL);
  if (xReturned == pdFALSE) {
    ESP_LOGE(TAG, "Error creating connectedTask");
    return -1;
  }

  BLEDevice::startAdvertising();
  ESP_LOGD(TAG, "Waiting a client connection to notify...");
  return 0;
}

void ble_server_release() {}

int ble_msg_send(char *buffer, size_t len, size_t timeout) {
  auto xBytes = xMessageBufferSend(xMessageBuffer, buffer, len, timeout);
  ESP_LOGD(TAG, "bytes sent=%u", xBytes);
  return xBytes;
}
