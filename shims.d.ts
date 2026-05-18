// Auto-generated. Do not edit.



    //% color=#FF9900 weight=95 icon="\uf1b0"
declare namespace Blocks {

    /**
     * @brief Start Microbit More service.
     * 
     */
    //% shim=Blocks::startBlocksService
    function startBlocksService(): void;

    /**
     * @brief Register a label in waiting data list and return an ID for the label.
     * This starts Microbit More service if it was not available.
     * 
     * @param dataLabel label to register
     * @param dataType type of the data to be received
     * @return int ID for the label
     */
    //% shim=Blocks::call_registerWaitingDataLabel
    function call_registerWaitingDataLabel(dataLabel: string, dataType: BlocksDataContentType): int32;

    /**
     * @brief Get number which was received with the label.
     * 
     * @param labelID ID in registered labels
     * @return float received data with the label
     */
    //% shim=Blocks::call_dataContentAsNumber
    function call_dataContentAsNumber(labelID: int32): number;

    /**
     * @brief Get text which was received with the label.
     * 
     * @param labelID ID in registered labels
     * @return String received data with the label
     */
    //% shim=Blocks::call_dataContentAsText
    function call_dataContentAsText(labelID: int32): string;

    /**
     * @brief Send a float with labele to the blocks editor.
     * Do nothing if the blocks editor was not connected.
     * 
     * @param dataLabel - label of the data
     * @param dataContent - content of the data
     */
    //% shim=Blocks::call_sendNumberWithLabel
    function call_sendNumberWithLabel(dataLabel: string, dataContent: number): void;

    /**
     * @brief Send a text with label to the blocks editor.
     * Do nothing if the blocks editor was not connected.
     * 
     * @param dataLabel - label of the data
     * @param dataContent - content of the data
     */
    //% shim=Blocks::call_sendTextWithLabel
    function call_sendTextWithLabel(dataLabel: string, dataContent: string): void;
}

// Auto-generated. Do not edit. Really.
