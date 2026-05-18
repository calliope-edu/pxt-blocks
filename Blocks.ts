namespace Blocks {
  const BLOCKS_DATA_RECEIVED = 8000;

  /**
  * Starts BLE services for the Calliope Blocks extension.
  */
  //% blockId=Blocks_startBlocksService block="start Blocks service"
  //% shim=Blocks::startBlocksService
  export function startService(): void {
    console.log("Blocks started");
  }

  /**
  * Register a label and return its ID.
  */
  //% shim=Blocks::call_registerWaitingDataLabel
  export function registerWaitingDataLabel(label: string, type: BlocksDataContentType): number {
    console.log("Blocks registered label: " + label);
    return 1; // dummy for sim
  }

  /**
   * Read received data as a number
   */
  //% shim=Blocks::call_dataContentAsNumber
  export function dataContentAsNumber(labelID: number): number {
    return 0.0; // dummy for sim
  }

  /**
   * Read received data as a text
   */
  //% shim=Blocks::call_dataContentAsText
  export function dataContentAsText(labelID: number): string {
    return "text"; // dummy for sim
  }

  /**
   * Run blocks with data when a number data with the label is received.
   * @param label - label of the data
   * @param handler - blocks to run
   */
  //% blockId=Blocks_onReceivedNumberWithLabel
  //% block="on number $numberData with label $label"
  //% label.defl="label-01"
  //% draggableParameters
  export function onReceivedNumberWithLabel(label: string, handler: (numberData: number) => void) {
    let labelID = Blocks.registerWaitingDataLabel(label, BlocksDataContentType.BLOCKS_DATA_NUMBER);
    if (0 === labelID) {
      throw "max waiting label counts exceed";
    }
    control.onEvent(BLOCKS_DATA_RECEIVED, labelID, function () {
      handler(Blocks.dataContentAsNumber(labelID));
      return;
    });
  }

  /**
   * Run blocks with data when a text data with the label is received.
   * @param label - label of the data
   * @param handler - blocks to run
   */
  //% blockId=Blocks_onReceivedTextWithLabel
  //% block="on text $textData with label $label"
  //% label.defl="label-01"
  //% draggableParameters
  export function onReceivedTextWithLabel(label: string, handler: (textData: string) => void) {
    let labelID = Blocks.registerWaitingDataLabel(label, BlocksDataContentType.BLOCKS_DATA_TEXT);
    if (0 === labelID) {
      throw "max waiting label counts exceed";
    }
    control.onEvent(BLOCKS_DATA_RECEIVED, labelID, function () {
      handler(Blocks.dataContentAsText(labelID));
      return;
    });
  }

  /**
   * Send number with label
   * @param label lavel of the data 
   * @param data number value to send
   */
  //% blockId=Blocks_sendNumberWithLabel
  //% block="send number $numberData with label $label"
  //% shim=Blocks::call_sendNumberWithLabel
  //% label.defl="label-01"
  //% numberData.defl=0.0
  export function sendNumberWithLabel(label: string, numberData: number): void {
    console.log("Blocks send a number: " + label + " = " + numberData);
  }

  /**
   * Send text with label
   * @param label lavel of the data
   * @param data text to send
   */
  //% blockId=Blocks_sendTextWithLabel
  //% block="send text $textData with label $label"
  //% shim=Blocks::call_sendTextWithLabel
  //% label.defl="label-01"
  //% textData.defl="text"
  export function sendTextWithLabel(label: string, textData: string): void {
    console.log("Blocks send a text: " + label + " = " + textData);
  }

} // namespace Blocks