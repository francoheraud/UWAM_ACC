/**
 * Dual CAN driver for the ACC
 * - Franco H
 */

#include "can_driver.h"

HAL_StatusTypeDef CAN_InitDriver(CAN_Driver_t *drv, CAN_HandleTypeDef *hcan1,  CAN_HandleTypeDef *hcan2) {
    if (!drv || !hcan1 || !hcan2) return HAL_ERROR;
    drv->hcan1 = hcan1;
    drv->hcan2 = hcan2;

    return 	HAL_CAN_Init(drv->hcan1) 	||
    		HAL_CAN_Start(drv->hcan1) 	||
			HAL_CAN_Init(drv->hcan2) 	||
			HAL_CAN_Start(drv->hcan2);
}

HAL_StatusTypeDef CAN_Transmit1(CAN_Driver_t *drv, CAN_TxHeaderTypeDef *tx) {
    if (!drv || !drv->hcan1 || !tx) return HAL_ERROR;

    uint32_t mbox;
    tx->IDE  = CAN_ID_STD;
    tx->RTR  = CAN_RTR_DATA;
    tx->DLC  = drv->len;
    tx->StdId= drv->id;

    return HAL_CAN_AddTxMessage(drv->hcan1, tx, drv->tx_data, &mbox);
}

HAL_StatusTypeDef CAN_Transmit2(CAN_Driver_t *drv, CAN_TxHeaderTypeDef *tx) {
    if (!drv || !drv->hcan1 || !tx) return HAL_ERROR;

    uint32_t mbox;
    tx->IDE  = CAN_ID_STD;
    tx->RTR  = CAN_RTR_DATA;
    tx->DLC  = drv->len;
    tx->StdId= drv->id;

    return HAL_CAN_AddTxMessage(drv->hcan2, tx, drv->tx_data, &mbox);
}

HAL_StatusTypeDef CAN_Receive(CAN_Driver_t *drv, CAN_RxHeaderTypeDef *rx) {
    if (!drv || !drv->hcan1 || !rx) return HAL_ERROR;

    rx->IDE = CAN_ID_STD;
    rx->RTR = CAN_RTR_DATA;
    rx->DLC = drv->len;
    rx->StdId = drv->id;

    while (HAL_CAN_GetRxFifoFillLevel(drv->hcan1, CAN_RX_FIFO0) == 0) { /* spin or add timeout */ }
    return HAL_CAN_GetRxMessage(drv->hcan1, CAN_RX_FIFO0, rx, drv->rx_data);
}

void CAN_Test(void)
{
	// add some shit here
}

static HAL_StatusTypeDef Config_CAN_Filter(CAN_Driver_t *can) {
	// ...
	return HAL_OK;
}



