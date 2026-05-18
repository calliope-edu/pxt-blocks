#include "pxt.h"

#include "MicroBit.h"
#include "MicroBitConfig.h"

#if !MICROBIT_CODAL

#include "MicroBitButton.h"

#include "BlocksServiceDAL.h"

#define BLOCKS_DATA_FORMAT_BUTTON_EVENT 0x11

/**
 * @brief Service ID of Microbit More.
 *
 */
const uint8_t BLOCKS_SERVICE[] = {0x0b, 0x50, 0xf3, 0xe4, 0x60, 0x7f, 0x41, 0x51, 0x90, 0x91, 0x7d, 0x00, 0x8d, 0x6f, 0xfc, 0x5c};

/**
 * @brief Characteristics in Microbit More Service.
 *
 */
const uint8_t BLOCKS_CH_COMMAND[] = {0x0b, 0x50, 0x01, 0x00, 0x60, 0x7f, 0x41, 0x51, 0x90, 0x91, 0x7d, 0x00, 0x8d, 0x6f, 0xfc, 0x5c};
const uint8_t BLOCKS_CH_STATE[] = {0x0b, 0x50, 0x01, 0x01, 0x60, 0x7f, 0x41, 0x51, 0x90, 0x91, 0x7d, 0x00, 0x8d, 0x6f, 0xfc, 0x5c};
const uint8_t BLOCKS_CH_DIRECTION[] = {0x0b, 0x50, 0x01, 0x02, 0x60, 0x7f, 0x41, 0x51, 0x90, 0x91, 0x7d, 0x00, 0x8d, 0x6f, 0xfc, 0x5c};
const uint8_t BLOCKS_CH_PIN_EVENT[] = {0x0b, 0x50, 0x01, 0x10, 0x60, 0x7f, 0x41, 0x51, 0x90, 0x91, 0x7d, 0x00, 0x8d, 0x6f, 0xfc, 0x5c};
const uint8_t BLOCKS_CH_ACTION_EVENT[] = {0x0b, 0x50, 0x01, 0x11, 0x60, 0x7f, 0x41, 0x51, 0x90, 0x91, 0x7d, 0x00, 0x8d, 0x6f, 0xfc, 0x5c};
const uint8_t BLOCKS_CH_ANALOG_IN_P0[] = {0x0b, 0x50, 0x01, 0x20, 0x60, 0x7f, 0x41, 0x51, 0x90, 0x91, 0x7d, 0x00, 0x8d, 0x6f, 0xfc, 0x5c};
const uint8_t BLOCKS_CH_ANALOG_IN_P1[] = {0x0b, 0x50, 0x01, 0x21, 0x60, 0x7f, 0x41, 0x51, 0x90, 0x91, 0x7d, 0x00, 0x8d, 0x6f, 0xfc, 0x5c};
const uint8_t BLOCKS_CH_ANALOG_IN_P2[] = {0x0b, 0x50, 0x01, 0x22, 0x60, 0x7f, 0x41, 0x51, 0x90, 0x91, 0x7d, 0x00, 0x8d, 0x6f, 0xfc, 0x5c};
const uint8_t BLOCKS_CH_ANALOG_IN_P3[] = {0x0b, 0x50, 0x01, 0x23, 0x60, 0x7f, 0x41, 0x51, 0x90, 0x91, 0x7d, 0x00, 0x8d, 0x6f, 0xfc, 0x5c};

/**
 * Class definition for the the blocks editor MicroBit More Service.
 * Provides a BLE service to remotely controll Micro:bit from the blocks editor3.
 */

/**
 * Constructor.
 * Create a representation of the Microbit More BLE Service
 */
BlocksServiceDAL::BlocksServiceDAL() : uBit(pxt::uBit) {
  blocks = &BlocksDevice::getInstance();
  blocks->moreService = this;

  commandCh = new GattCharacteristic(
      BLOCKS_CH_COMMAND, commandChBuffer, BLOCKS_CH_BUFFER_SIZE_COMMAND, BLOCKS_CH_BUFFER_SIZE_COMMAND,
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE |
          GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);
  commandCh->requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

  stateCh = new GattCharacteristic(
      BLOCKS_CH_STATE, (uint8_t *)&stateChBuffer,
      BLOCKS_CH_BUFFER_SIZE_STATE, BLOCKS_CH_BUFFER_SIZE_STATE,
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);
  stateCh->requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

  directionCh = new GattCharacteristic(
      BLOCKS_CH_DIRECTION, (uint8_t *)&motionChBuffer,
      BLOCKS_CH_BUFFER_SIZE_MOTION, BLOCKS_CH_BUFFER_SIZE_MOTION,
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);
  directionCh->requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

  pinEventCh = new GattCharacteristic(
      BLOCKS_CH_PIN_EVENT, (uint8_t *)&pinEventChBuffer,
      BLOCKS_CH_BUFFER_SIZE_NOTIFY, BLOCKS_CH_BUFFER_SIZE_NOTIFY,
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
          GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);
  pinEventCh->requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

  actionEventCh = new GattCharacteristic(
      BLOCKS_CH_ACTION_EVENT, (uint8_t *)&actionEventChBuffer,
      BLOCKS_CH_BUFFER_SIZE_NOTIFY, BLOCKS_CH_BUFFER_SIZE_NOTIFY,
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ |
          GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY);
  actionEventCh->requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

  analogInP0Ch = new GattCharacteristic(
      BLOCKS_CH_ANALOG_IN_P0, (uint8_t *)&analogInP0ChBuffer,
      BLOCKS_CH_BUFFER_SIZE_ANALOG_IN, BLOCKS_CH_BUFFER_SIZE_ANALOG_IN,
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);
  analogInP0Ch->setReadAuthorizationCallback(
      this, &BlocksServiceDAL::onReadAnalogIn);
  analogInP0Ch->requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

  analogInP1Ch = new GattCharacteristic(
      BLOCKS_CH_ANALOG_IN_P1, (uint8_t *)&analogInP1ChBuffer,
      BLOCKS_CH_BUFFER_SIZE_ANALOG_IN, BLOCKS_CH_BUFFER_SIZE_ANALOG_IN,
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);
  analogInP1Ch->setReadAuthorizationCallback(
      this, &BlocksServiceDAL::onReadAnalogIn);
  analogInP1Ch->requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

  analogInP2Ch = new GattCharacteristic(
      BLOCKS_CH_ANALOG_IN_P2, (uint8_t *)&analogInP2ChBuffer,
      BLOCKS_CH_BUFFER_SIZE_ANALOG_IN, BLOCKS_CH_BUFFER_SIZE_ANALOG_IN,
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);
  analogInP2Ch->setReadAuthorizationCallback(
      this, &BlocksServiceDAL::onReadAnalogIn);
  analogInP2Ch->requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

  analogInP3Ch = new GattCharacteristic(
      BLOCKS_CH_ANALOG_IN_P3, (uint8_t *)&analogInP3ChBuffer,
      BLOCKS_CH_BUFFER_SIZE_ANALOG_IN, BLOCKS_CH_BUFFER_SIZE_ANALOG_IN,
      GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ);
  analogInP3Ch->setReadAuthorizationCallback(
      this, &BlocksServiceDAL::onReadAnalogIn);
  analogInP3Ch->requireSecurity(SecurityManager::MICROBIT_BLE_SECURITY_LEVEL);

  /*
  stateCh = digitalIn[4], lightLevel[1], temperature[1], microphone[1]
  directionCh = acceleration[10], magnet[8]
  pinEventCh = pinEvent
  actionEventCh = buttonEvent, gestureEvent
  analogInP0Ch, analogInP1Ch, analogInP2Ch,, analogInP3Ch
  */

  GattCharacteristic *blocksChs[] = {
      commandCh,
      stateCh,
      directionCh,
      pinEventCh,
      actionEventCh,
      analogInP0Ch,
      analogInP1Ch,
      analogInP2Ch,
      analogInP3Ch,
  };

  uBit.messageBus.listen(
      MICROBIT_ID_BLE,
      MICROBIT_BLE_EVT_CONNECTED,
      this,
      &BlocksServiceDAL::onBLEConnected,
      MESSAGE_BUS_LISTENER_QUEUE_IF_BUSY);

  GattService blocksService(BLOCKS_SERVICE, blocksChs,
                              sizeof(blocksChs) /
                                  sizeof(GattCharacteristic *));
  uBit.ble->addService(blocksService);

  // Setup callbacks for events.
  uBit.ble->onDataWritten(this, &BlocksServiceDAL::onDataWritten);
}

/**
   * Invoked when BLE connected.
   */
void BlocksServiceDAL::onBLEConnected(MicroBitEvent _e) {
  blocks->updateVersionData();
  uBit.ble->gattServer().write(commandCh->getValueHandle(), commandChBuffer,
                               BLOCKS_CH_BUFFER_SIZE_COMMAND);
}

/**
 * Callback. Invoked when AnalogIn is read via BLE.
 */
void BlocksServiceDAL::onReadAnalogIn(
    GattReadAuthCallbackParams *authParams) {
  if (authParams->handle == analogInP0Ch->getValueHandle()) {
    blocks->updateAnalogIn(analogInP0ChBuffer, 0);
    authParams->data = (uint8_t *)&analogInP0ChBuffer;
    authParams->offset = 0;
    authParams->len = BLOCKS_CH_BUFFER_SIZE_ANALOG_IN;
    authParams->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
  } else if (authParams->handle == analogInP1Ch->getValueHandle()) {
    blocks->updateAnalogIn(analogInP1ChBuffer, 1);
    authParams->data = (uint8_t *)&analogInP1ChBuffer;
    authParams->offset = 0;
    authParams->len = BLOCKS_CH_BUFFER_SIZE_ANALOG_IN;
    authParams->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
  } else if (authParams->handle == analogInP2Ch->getValueHandle()) {
    blocks->updateAnalogIn(analogInP2ChBuffer, 2);
    authParams->data = (uint8_t *)&analogInP2ChBuffer;
    authParams->offset = 0;
    authParams->len = BLOCKS_CH_BUFFER_SIZE_ANALOG_IN;
    authParams->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
  } else if (authParams->handle == analogInP3Ch->getValueHandle()) {
    blocks->updateAnalogIn(analogInP3ChBuffer, 3);
    authParams->data = (uint8_t *)&analogInP3ChBuffer;
    authParams->offset = 0;
    authParams->len = BLOCKS_CH_BUFFER_SIZE_ANALOG_IN;
    authParams->authorizationReply = AUTH_CALLBACK_REPLY_SUCCESS;
  }
}

/**
 * Callback. Invoked when any of our attributes are written via BLE.
 */
void BlocksServiceDAL::onDataWritten(const GattWriteCallbackParams *params) {
  blocks->onCommandReceived((uint8_t *)params->data, params->len);
}

/**
 * @brief Notify action event.
 */
void BlocksServiceDAL::notifyActionEvent() {
  uBit.ble->gattServer().notify(actionEventCh->getValueHandle(),
                                actionEventChBuffer, BLOCKS_CH_BUFFER_SIZE_NOTIFY);
}

/**
 * @brief Notify pin event.
 */
void BlocksServiceDAL::notifyPinEvent() {
  uBit.ble->gattServer().notify(pinEventCh->getValueHandle(), pinEventChBuffer,
                                BLOCKS_CH_BUFFER_SIZE_NOTIFY);
}

/**
 * Notify data to the blocks editor3
 */
void BlocksServiceDAL::notify() {}

/**
 * Update all GPIO and sensors state.
 */
void BlocksServiceDAL::update() {
  if (uBit.ble->gap().getState().connected) {
    blocks->updateState(stateChBuffer);
    uBit.ble->gattServer().write(stateCh->getValueHandle(), stateChBuffer,
                                 BLOCKS_CH_BUFFER_SIZE_STATE);
    blocks->updateMotion(motionChBuffer);
    uBit.ble->gattServer().write(directionCh->getValueHandle(),
                                 motionChBuffer,
                                 BLOCKS_CH_BUFFER_SIZE_MOTION);
  } else {
    blocks->displayFriendlyName();
  }
}

#endif // !MICROBIT_CODAL
