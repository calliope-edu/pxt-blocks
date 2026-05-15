#include "pxt.h"

#if MICROBIT_CODAL

#include "MicroBitConfig.h"

#if CONFIG_ENABLED(DEVICE_BLE)

#include "MbitMoreService.h"
#include "MicroBitMbitMoreService.h"

using codalMbit = codal::MicroBitMbitMoreService;

// ---- Static dispatchers to MbitMoreDevice singleton ----------------------
//
// The CODAL service hands us C-style function pointers, so we route back
// into the singleton MbitMoreDevice that owns the real logic.

int MbitMoreService::s_onCommandWrite(const uint8_t *data, size_t len) {
  // Match the original BLE behaviour: dispatch to MbitMoreDevice without
  // touching commandChBuffer, which stays seeded with version data for
  // any consumer that reads it back as the cached read value.
  MbitMoreDevice::getInstance().onCommandReceived((uint8_t *)data, len);
  return 0;
}

size_t MbitMoreService::s_onAnalogReadP0(uint8_t *out, size_t bufSize) {
  if (bufSize < MM_CH_BUFFER_SIZE_ANALOG_IN) return 0;
  MbitMoreDevice::getInstance().updateAnalogIn(out, 0);
  return MM_CH_BUFFER_SIZE_ANALOG_IN;
}
size_t MbitMoreService::s_onAnalogReadP1(uint8_t *out, size_t bufSize) {
  if (bufSize < MM_CH_BUFFER_SIZE_ANALOG_IN) return 0;
  MbitMoreDevice::getInstance().updateAnalogIn(out, 1);
  return MM_CH_BUFFER_SIZE_ANALOG_IN;
}
size_t MbitMoreService::s_onAnalogReadP2(uint8_t *out, size_t bufSize) {
  if (bufSize < MM_CH_BUFFER_SIZE_ANALOG_IN) return 0;
  MbitMoreDevice::getInstance().updateAnalogIn(out, 2);
  return MM_CH_BUFFER_SIZE_ANALOG_IN;
}
size_t MbitMoreService::s_onAnalogReadP3(uint8_t *out, size_t bufSize) {
  if (bufSize < MM_CH_BUFFER_SIZE_ANALOG_IN) return 0;
  MbitMoreDevice::getInstance().updateAnalogIn(out, 3);
  return MM_CH_BUFFER_SIZE_ANALOG_IN;
}

// ---- Construction --------------------------------------------------------

MbitMoreService::MbitMoreService() : uBit(pxt::uBit) {
  mbitMore = &MbitMoreDevice::getInstance();
  mbitMore->moreService = this;

  codalMbit *svc = codalMbit::sharedInstance();
  if (svc) {
    svc->registerWriteHandler(codalMbit::mmCommand, &s_onCommandWrite);
    svc->registerReadAuthHandler(codalMbit::mmAnalogInP0, &s_onAnalogReadP0);
    svc->registerReadAuthHandler(codalMbit::mmAnalogInP1, &s_onAnalogReadP1);
    svc->registerReadAuthHandler(codalMbit::mmAnalogInP2, &s_onAnalogReadP2);
    svc->registerReadAuthHandler(codalMbit::mmAnalogInP3, &s_onAnalogReadP3);

    // Seed the COMMAND characteristic with version data so the very first
    // read from Scratch returns the hardware + protocol version.
    mbitMore->updateVersionData();
    svc->setCharValue(codalMbit::mmCommand, commandChBuffer, MM_CH_BUFFER_SIZE_COMMAND);
    // Seed STATE so the widget's program-type probe can distinguish a
    // real runtime (non-zero state buffer — temperature byte etc.) from
    // the bare CODAL stub (all-zero buffer) on the very first connect,
    // before the periodic update fiber has run.
    mbitMore->updateState(stateChBuffer);
    svc->setCharValue(codalMbit::mmState, stateChBuffer, MM_CH_BUFFER_SIZE_STATE);
  }
}

// ---- Notify forwarders ---------------------------------------------------

void MbitMoreService::notifyActionEvent() {
  codalMbit *svc = codalMbit::sharedInstance();
  if (!svc) return;
  svc->notifyChar(codalMbit::mmActionEvent, actionEventChBuffer, MM_CH_BUFFER_SIZE_NOTIFY);
}

void MbitMoreService::notifyPinEvent() {
  codalMbit *svc = codalMbit::sharedInstance();
  if (!svc) return;
  svc->notifyChar(codalMbit::mmPinEvent, pinEventChBuffer, MM_CH_BUFFER_SIZE_NOTIFY);
}

void MbitMoreService::notifyData() {
  codalMbit *svc = codalMbit::sharedInstance();
  if (!svc) return;
  svc->notifyChar(codalMbit::mmData, dataChBuffer, MM_CH_BUFFER_SIZE_NOTIFY);
}

void MbitMoreService::notify() {}

// ---- Periodic state refresh ---------------------------------------------

void MbitMoreService::update() {
  codalMbit *svc = codalMbit::sharedInstance();
  if (!svc || !svc->getConnected()) return;
  mbitMore->updateState(stateChBuffer);
  mbitMore->updateMotion(motionChBuffer);
  svc->setCharValue(codalMbit::mmState, stateChBuffer, MM_CH_BUFFER_SIZE_STATE);
  svc->setCharValue(codalMbit::mmMotion, motionChBuffer, MM_CH_BUFFER_SIZE_MOTION);
}

// ---- Data-label delegation ----------------------------------------------

int MbitMoreService::registerWaitingDataLabel(ManagedString dataLabel, MbitMoreDataContentType dataType) {
  return mbitMore->registerWaitingDataLabel(dataLabel, dataType);
}
MbitMoreDataContentType MbitMoreService::dataType(int labelID) {
  return mbitMore->dataType(labelID);
}
float MbitMoreService::dataContentAsNumber(int labelID) {
  return mbitMore->dataContentAsNumber(labelID);
}
ManagedString MbitMoreService::dataContentAsText(int labelID) {
  return mbitMore->dataContentAsText(labelID);
}
void MbitMoreService::sendNumberWithLabel(ManagedString dataLabel, float dataContent) {
  mbitMore->sendNumberWithLabel(dataLabel, dataContent);
}
void MbitMoreService::sendTextWithLabel(ManagedString dataLabel, ManagedString dataContent) {
  mbitMore->sendTextWithLabel(dataLabel, dataContent);
}

#endif // CONFIG_ENABLED(DEVICE_BLE)
#endif // MICROBIT_CODAL
