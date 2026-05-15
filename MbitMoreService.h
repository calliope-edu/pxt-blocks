#include "pxt.h"

#if MICROBIT_CODAL

#ifndef MBIT_MORE_SERVICE_H
#define MBIT_MORE_SERVICE_H

#include "MicroBitConfig.h"

#if CONFIG_ENABLED(DEVICE_BLE)

#include "MicroBit.h"
#include "MicroBitMbitMoreService.h"

#include "MbitMoreCommon.h"
#include "MbitMoreDevice.h"

// Forward declaration
class MbitMoreDevice;

/**
 * pxt-blocks side of the MbitMore service.
 *
 * As of the CODAL refactor (codal-microbit-v2 ships an always-on
 * `MicroBitMbitMoreService` so the partial-flash DAL hash is identical
 * across all CODAL builds), this class no longer registers its own GATT
 * service. It is a thin shim that:
 *
 *   - Owns the per-characteristic byte buffers `MbitMoreDevice` and
 *     `MbitMoreSerial` write into directly (preserves their existing API).
 *   - Hooks the CODAL service's write / read-auth callbacks so writes from
 *     a Scratch central reach `MbitMoreDevice::onCommandReceived` and reads
 *     of the analog-in pins trigger `MbitMoreDevice::updateAnalogIn`.
 *   - Forwards `notify*` calls to the CODAL service's `notifyChar`.
 *   - Periodically mirrors fresh state/motion buffers into the CODAL
 *     service via `setCharValue` so plain reads see the latest values.
 *
 * The serial-side path through `MbitMoreSerial` is unchanged.
 */
class MbitMoreService {
public:
  // Buffer of characteristic for receiving commands.
  uint8_t commandChBuffer[MM_CH_BUFFER_SIZE_COMMAND] = {0};

  // Buffer of characteristic for sending data of GPIO and sensors state.
  uint8_t stateChBuffer[MM_CH_BUFFER_SIZE_STATE] = {0};

  // Buffer of characteristic for sending data about motion.
  uint8_t motionChBuffer[MM_CH_BUFFER_SIZE_MOTION] = {0};

  // Buffer of characteristic for sending pin events.
  uint8_t pinEventChBuffer[MM_CH_BUFFER_SIZE_NOTIFY] = {0};

  // Buffer of characteristic for sending action events.
  uint8_t actionEventChBuffer[MM_CH_BUFFER_SIZE_NOTIFY] = {0};

  // Buffer of characteristic for sending analog input values of P0..P3.
  uint8_t analogInP0ChBuffer[MM_CH_BUFFER_SIZE_ANALOG_IN] = {0};
  uint8_t analogInP1ChBuffer[MM_CH_BUFFER_SIZE_ANALOG_IN] = {0};
  uint8_t analogInP2ChBuffer[MM_CH_BUFFER_SIZE_ANALOG_IN] = {0};
  uint8_t analogInP3ChBuffer[MM_CH_BUFFER_SIZE_ANALOG_IN] = {0};

  // Buffer of characteristic for sending data.
  uint8_t dataChBuffer[MM_CH_BUFFER_SIZE_NOTIFY] = {0};

  /**
   * Constructor. Wires up CODAL service callbacks and pushes initial
   * version data into the COMMAND characteristic so the first central
   * read returns sensible values.
   */
  MbitMoreService();

  /**
   * @brief Notify action / pin / data buffers via the CODAL service.
   */
  void notifyActionEvent();
  void notifyPinEvent();
  void notifyData();

  /** Periodic notify hook (no-op — kept for fiber-loop call-site compat). */
  void notify();

  /**
   * Update state / motion buffers and mirror them into the CODAL
   * service so plain reads return fresh values.
   */
  void update();

  // Data-label glue — pure delegation to MbitMoreDevice (unchanged API).
  int registerWaitingDataLabel(ManagedString dataLabel, MbitMoreDataContentType dataType);
  MbitMoreDataContentType dataType(int labelID);
  float dataContentAsNumber(int labelID);
  ManagedString dataContentAsText(int labelID);
  void sendNumberWithLabel(ManagedString dataLabel, float dataContent);
  void sendTextWithLabel(ManagedString dataLabel, ManagedString dataContent);

private:
  MicroBit &uBit;
  MbitMoreDevice *mbitMore;

  // Static dispatchers handed to the CODAL service. Function pointers
  // (no `this`) — they reach the device through the MbitMoreDevice
  // singleton.
  static int s_onCommandWrite(const uint8_t *data, size_t len);
  static size_t s_onAnalogReadP0(uint8_t *out, size_t bufSize);
  static size_t s_onAnalogReadP1(uint8_t *out, size_t bufSize);
  static size_t s_onAnalogReadP2(uint8_t *out, size_t bufSize);
  static size_t s_onAnalogReadP3(uint8_t *out, size_t bufSize);
};

#endif // CONFIG_ENABLED(DEVICE_BLE)
#endif // MBIT_MORE_SERVICE_H
#endif // MICROBIT_CODAL
