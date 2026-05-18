// Auto-generated. Do not edit.


    /**
     * Data type of content.
     */

    declare const enum BlocksDataContentType
    {
    //% block="number"
    BLOCKS_DATA_NUMBER = 1,
    //% block="text"
    BLOCKS_DATA_TEXT = 2,
    }


    declare const enum BlocksCommand
    {
    CMD_CONFIG = 0x00,
    CMD_PIN = 0x01,
    CMD_DISPLAY = 0x02,
    CMD_AUDIO = 0x03,
    CMD_DATA = 0x04,
    CMD_RGB = 0x05,
    CMD_MOTOR = 0x06,
    }


    declare const enum BlocksMotorCommand
    {
    SET_M0 = 0x01,
    SET_M1 = 0x02,
    SET_M0_M1 = 0x03,
    SET_MOTIONKIT_LEFT = 0x04,
    SET_MOTIONKIT_RIGHT = 0x05,
    SET_MOTIONKIT_BOTH = 0x06,
    }


    declare const enum BlocksPinCommand
    {
    SET_OUTPUT = 0x01,
    SET_PWM = 0x02,
    SET_SERVO = 0x03,
    SET_PULL = 0x04,
    SET_EVENT = 0x05,
    }


    declare const enum BlocksDisplayCommand
    {
    CLEAR = 0x00,
    TEXT = 0x01,
    PIXELS_0 = 0x02,
    PIXELS_1 = 0x03,
    }


    /**
     * @brief Enum for write mode of display pixels.
     */

    declare const enum BlocksDisplayWriteMode
    {
    LAYER = 0,
    OVER_WRITE = 1,
    }


    declare const enum BlocksPullMode
    {
    None = 0,
    Down = 1,
    Up = 2,
    }


    declare const enum BlocksDataFormat
    {
    CONFIG = 0x10,
    PIN_EVENT = 0x11,
    ACTION_EVENT = 0x12,
    DATA_NUMBER = 0x13,
    DATA_TEXT = 0x14,
    }


    declare const enum BlocksActionEvent
    {
    BUTTON = 0x01,
    GESTURE = 0x02,
    }


    declare const enum BlocksButtonEvent
    {
    DOWN = 1,
    UP = 2,
    CLICK = 3,
    LONG_CLICK = 4,
    HOLD = 5,
    DOUBLE_CLICK = 6,
    }


    declare const enum BlocksGestureEvent
    {
    TILT_UP = 1,
    TILT_DOWN = 2,
    TILT_LEFT = 3,
    TILT_RIGHT = 4,
    FACE_UP = 5,
    FACE_DOWN = 6,
    FREEFALL = 7,
    G3 = 8,
    G6 = 9,
    G8 = 10,
    SHAKE = 11,
    }


    declare const enum BlocksPinEventType
    {
    NONE = 0,
    ON_EDGE = 1,
    ON_PULSE = 2,
    ON_TOUCH = 3,
    }


    declare const enum BlocksPinEvent
    {
    RISE = 2,
    FALL = 3,
    PULSE_HIGH = 4,
    PULSE_LOW = 5,
    }


    /**
     * @brief Enum for sub-command about configurations.
     * 
     */

    declare const enum BlocksConfig
    {
    MICPIN = 0x01,
    TOUCH = 0x02,
    }


    /**
     * @brief Enum for sub-commands about audio.
     * 
     */

    declare const enum BlocksAudioCommand
    {
    STOP_TONE = 0x00,
    PLAY_TONE = 0x01,
    }


    /**
     * @brief Button ID in Blocks
     * This number is used to memory offset in state data.
     */

    declare const enum BlocksButtonStateIndex
    {
    // GPIO array using [0..20]
    P0 = 24,
    P1 = 25,
    P2 = 26,
    P3 = 27,
    A = 28,
    B = 29,
    LOGO = 30,
    }


    /**
     * @brief Version of this micro:bit
     * 
     */

    declare const enum BlocksHardwareVersion
    {
    MICROBIT_V1 = 1,
    MICROBIT_V2 = 2,
    }


    /**
     * @brief Version of protocol to use
     * 
     */

    declare const enum BlocksProtocol
    {
    BLOCKS_V2 = 2,
    }


    /**
     * @brief Communication route between the blocks editor and the device
     * 
     */

    declare const enum BlocksCommunicationRoute
    {
    BLE = 0,
    SERIAL = 1,
    }


    /**
     * @brief Request type from the blocks editor
     * 
     */

    declare const enum ChRequest
    {
    REQ_READ = 0x01,
    REQ_WRITE = 0x10,
    REQ_WRITE_RESPONSE = 0x11,
    REQ_NOTIFY_STOP = 0x20,
    REQ_NOTIFY_START = 0x21,
    }


    /**
     * @brief Response type to the blocks editor
     * 
     */

    declare const enum ChResponse
    {
    RES_READ = 0x01,
    RES_WRITE = 0x11,
    RES_NOTIFY = 0x21,
    }
declare namespace Blocks {
}

// Auto-generated. Do not edit. Really.
