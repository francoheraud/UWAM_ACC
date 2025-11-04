/**
 * Dual CAN driver for the ACC
 * - Franco H
 */


#ifndef INC_CAN_DRIVER_H_
#define INC_CAN_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include <stdint.h>

typedef struct {
    CAN_HandleTypeDef *hcan1;
    CAN_HandleTypeDef *hcan2;
    uint8_t  tx_data[8];
    uint8_t  rx_data[8];
    uint32_t id;
    uint8_t  len;
} CAN_Driver_t;

HAL_StatusTypeDef CAN_InitDriver(CAN_Driver_t *drv, CAN_HandleTypeDef *hcan1,  CAN_HandleTypeDef *hcan2);
HAL_StatusTypeDef CAN_Transmit1(CAN_Driver_t *drv, CAN_TxHeaderTypeDef *tx);
HAL_StatusTypeDef CAN_Transmit2(CAN_Driver_t *drv, CAN_TxHeaderTypeDef *tx);
HAL_StatusTypeDef CAN_Receive(CAN_Driver_t *drv, CAN_RxHeaderTypeDef *rx);
void              CAN_Test(void);

#ifdef __cplusplus
}
#endif
#endif /* INC_CAN_DRIVER_H_ */
