/**
 * Dual CAN driver for the ACC
 * - Franco H
 */

#include <Drivers/can_driver.h>

extern CAN_HandleTypeDef hcan1;
extern CAN_HandleTypeDef hcan2;

HAL_StatusTypeDef CAN_InitDriver(CAN_Driver_t *drv) {
    if (!drv) return HAL_ERROR;
    if (hcan1.Instance == NULL || hcan2.Instance == NULL) return HAL_ERROR;
    drv->hcan1 = &hcan1;
    drv->hcan2 = &hcan2;
    return  HAL_CAN_Start(drv->hcan1) || HAL_CAN_Start(drv->hcan2);
}

HAL_StatusTypeDef CAN_Transmit1(CAN_Driver_t *drv) {

    uint32_t mbox;
    drv->tx1.IDE  = CAN_ID_STD;
    drv->tx1.RTR  = CAN_RTR_DATA;
    drv->tx1.DLC  = drv->len;
    drv->tx1.StdId= drv->id;

    return HAL_CAN_AddTxMessage(drv->hcan1, &drv->tx1, drv->tx_data, &mbox);
}

HAL_StatusTypeDef CAN_Transmit2(CAN_Driver_t *drv) {

    uint32_t mbox;
    drv->tx2.IDE  = CAN_ID_STD;
    drv->tx2.RTR  = CAN_RTR_DATA;
    drv->tx2.DLC  = drv->len;
    drv->tx2.StdId= drv->id;

    return HAL_CAN_AddTxMessage(drv->hcan2, &drv->tx2, drv->tx_data, &mbox);
}

HAL_StatusTypeDef CAN_Receive1(CAN_Driver_t *drv) {

    drv->rx1.IDE = CAN_ID_STD;
    drv->rx1.RTR = CAN_RTR_DATA;
    drv->rx1.DLC = drv->len;
    drv->rx1.StdId = drv->id;

    while (HAL_CAN_GetRxFifoFillLevel(drv->hcan1, CAN_RX_FIFO0) == 0) { /* spin or add timeout */ }
    return HAL_CAN_GetRxMessage(drv->hcan1, CAN_RX_FIFO0, &drv->rx1, drv->rx_data);
}

void CAN_Test(void)
{
	// add some shit here
}



