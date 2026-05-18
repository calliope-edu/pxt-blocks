#include "pxt.h"

#include "MicroBit.h"
#include "MicroBitConfig.h"

#if !MICROBIT_CODAL

#ifndef BLOCKS_SERVICE_DAL_H
#define BLOCKS_SERVICE_DAL_H

#include "BlocksCommon.h"
#include "BlocksDevice.h"

// // Forward declaration
class BlocksDevice;

/**
 * Class definition for a MicroBitMore Service.
 * Provides a BLE service to remotely read the state of sensors from the blocks editor3.
 */
class BlocksServiceDAL {
public:
  /**
   * Constructor.
   * Create a representation of the BlocksService
   */
  BlocksServiceDAL();

  /**
   * Invoked when BLE connected.
   */
  void onBLEConnected(MicroBitEvent _e);

  void notify();

  /**
   * @brief Notify action event.
   */
  void notifyActionEvent();

  /**
   * @brief Notify action event.
   */
  void notifyPinEvent();

  /**
   * Callback. Invoked when AnalogIn is read via BLE.
   */
  void onReadAnalogIn(GattReadAuthCallbackParams *authParams);

  /**
   * Callback. Invoked when any of our attributes are written via BLE.
   */
  void onDataWritten(const GattWriteCallbackParams *params);

  void update();

  // Buffer of characteristic for receiving commands.
  uint8_t commandChBuffer[BLOCKS_CH_BUFFER_SIZE_COMMAND] = {0};

  // Buffer of characteristic for sending data of GPIO and sensors state.
  uint8_t stateChBuffer[BLOCKS_CH_BUFFER_SIZE_STATE] = {0};

  // Buffer of characteristic for sending data about motion.
  uint8_t motionChBuffer[BLOCKS_CH_BUFFER_SIZE_MOTION] = {0};

  // Buffer of characteristic for sending pin events.
  uint8_t pinEventChBuffer[BLOCKS_CH_BUFFER_SIZE_NOTIFY] = {0};

  // Buffer of characteristic for sending action events.
  uint8_t actionEventChBuffer[BLOCKS_CH_BUFFER_SIZE_NOTIFY] = {0};

  // Buffer of characteristic for sending analog input values of P0.
  uint8_t analogInP0ChBuffer[BLOCKS_CH_BUFFER_SIZE_ANALOG_IN] = {0};

  // Buffer of characteristic for sending analog input values of P1.
  uint8_t analogInP1ChBuffer[BLOCKS_CH_BUFFER_SIZE_ANALOG_IN] = {0};

  // Buffer of characteristic for sending analog input values of P2.
  uint8_t analogInP2ChBuffer[BLOCKS_CH_BUFFER_SIZE_ANALOG_IN] = {0};

  // Buffer of characteristic for sending analog input values of P3.
  uint8_t analogInP3ChBuffer[BLOCKS_CH_BUFFER_SIZE_ANALOG_IN] = {0};

private:
  /**
   * @brief micro:bit runtime object.
   *
   */
  MicroBit &uBit;

  /**
   * @brief Microbit More object.
   *
   */
  BlocksDevice *blocks;

  GattCharacteristic *commandCh;
  GattCharacteristic *stateCh;
  GattCharacteristic *directionCh;
  GattCharacteristic *pinEventCh;
  GattCharacteristic *actionEventCh;
  GattCharacteristic *analogInP0Ch;
  GattCharacteristic *analogInP1Ch;
  GattCharacteristic *analogInP2Ch;
  GattCharacteristic *analogInP3Ch;
};

#endif // BLOCKS_SERVICE_DAL_H
#endif // !MICROBIT_CODAL
