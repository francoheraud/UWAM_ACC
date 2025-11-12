/**
 * Dual CAN driver for the ACC
 */

#include <Drivers/can_driver.h>

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;


// TODO: In the future, could reimplement this to work with the error codes of CAN?

/**
 * @brief Initializes the CAN driver. CAN_Init(...) should be automatically called in the MX_CAN_Init() in main.c.
* @param CAN_Driver_t *drv
* @return HAL_StatusTypeDef
* @note Auto-gen: fill details.
*/
HAL_StatusTypeDef CAN_InitDriver(CAN_Driver_t *drv) {
    if (!drv) return HAL_ERROR;
    if (hcan1.Instance == NULL || hcan2.Instance == NULL) return HAL_ERROR;
    drv->hcan1 = &hcan1;
    drv->hcan2 = &hcan2;
    //drv->is_ready_to_read = false;
    return  HAL_CAN_Start(drv->hcan1) || HAL_CAN_Start(drv->hcan2);
}


/**
 * @brief 			Serialize a float value into two bytes.
 * @param data_in	Temperature or pressure value.
 * @return void
 * @note Auto-gen: fill details.
 */
void CAN_16Bit_Serializer(float data_in, uint8_t output_buf[2]) {
	uint16_t data_in_16u 	= (uint16_t)lroundf(data_in);
	uint8_t high_byte 		= (uint8_t)(data_in_16u >> 8);
	uint8_t low_byte 		= (uint8_t)(data_in_16u & 0xff);
	output_buf[0] = low_byte;
	output_buf[1] = high_byte;
}


/**
 * @brief 				Deserializes rx data from CAN. Assumes rx data is of form: {lowbyte1, highbyte1, lowbyte2, highbyte2, ...}
 * @param data_in_buf	Translated input data from the CAN bus.
 * @param rx_data		Raw input data from the CAN bus.
 * @return return_type	void
 * @note Auto-gen: fill details.
 */
void CAN_16Bit_Deserializer(uint16_t data_in_buf[4], uint8_t rx_data[8]) {
	for (uint8_t i = 0; i < 4; i++)
		data_in_buf[i] =  (uint16_t)((rx_data[i+1] << 8) | (rx_data[i]));
}

/**
 * @brief Transmits a CAN message over to the 1st CAN bus.
 * @param CAN_Driver_t *drv
 * @return HAL_StatusTypeDef
 * @note Auto-gen: fill details.
 */
HAL_StatusTypeDef CAN_Transmit1(CAN_Driver_t *drv) {

    uint32_t mbox;
    drv->tx1.IDE  = CAN_ID_STD;
    drv->tx1.RTR  = CAN_RTR_DATA;
    drv->tx1.DLC  = drv->len;
    drv->tx1.StdId= drv->id;

    return HAL_CAN_AddTxMessage(drv->hcan1, &drv->tx1, drv->tx_data, &mbox);
}

/**
 * @brief Transmits a CAN message over to the 2nd CAN bus.
* @param CAN_Driver_t *drv
* @return HAL_StatusTypeDef
* @note Auto-gen: fill details.
*/
HAL_StatusTypeDef CAN_Transmit2(CAN_Driver_t *drv) {

    uint32_t mbox;
    drv->tx2.IDE  = CAN_ID_STD;
    drv->tx2.RTR  = CAN_RTR_DATA;
    drv->tx2.DLC  = drv->len;
    drv->tx2.StdId= drv->id;

    return HAL_CAN_AddTxMessage(drv->hcan2, &drv->tx2, drv->tx_data, &mbox);
}

/**
 * @brief Receives a CAN message from the 1st CAN bus. We only need 1 receive function for ACC.
* @param CAN_Driver_t *drv
* @return HAL_StatusTypeDef
* @note Auto-gen: fill details.
*/
HAL_StatusTypeDef CAN_Receive1(CAN_Driver_t *drv) {

    drv->rx1.IDE = CAN_ID_STD;
    drv->rx1.RTR = CAN_RTR_DATA;
    drv->rx1.DLC = drv->len;
    drv->rx1.StdId = drv->id;

    while (HAL_CAN_GetRxFifoFillLevel(drv->hcan1, CAN_RX_FIFO0) == 0) { /* spin or add timeout */ }
    return HAL_CAN_GetRxMessage(drv->hcan1, CAN_RX_FIFO0, &drv->rx1, drv->rx_data);
}


/**
 * @brief TODO: Re-implement CAN driver testing using CMock API?
* @param void
* @return void
* @note Auto-gen: fill details.
*/
void CAN_Test(void)
{
	// add some shit here
	return;
}



