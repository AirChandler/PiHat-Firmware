//Host service
//Control service
#include "main.h"
extern jd_frame_t tempSend, tempRecv;
extern SPI_HandleTypeDef hspi1;
uint16_t pinMap[] = {
    GPIO_PIN_1,
    GPIO_PIN_2,
    GPIO_PIN_3,
    GPIO_PIN_4,
    GPIO_PIN_5,
    GPIO_PIN_6,
    GPIO_PIN_7,
    GPIO_PIN_8,
    GPIO_PIN_9,
    GPIO_PIN_10,
    GPIO_PIN_11,
    GPIO_PIN_12,
    GPIO_PIN_13,
    GPIO_PIN_14,
    GPIO_PIN_15
};

void setPin(){
    uint16_t pin = pinMap[tempRecv.data[1]];
    uint8_t pull = tempRecv.data[2];
    char port = tempRecv.data[3];
    if(pull == 1){
        if(port == 'A'){
            GPIOA->BSRR = pin;
        } else {
            GPIOB->BSRR = pin;
        }
    } else {
        if(port == 'A'){
            GPIOA->BSRR = (uint32_t)pin << 16U;
        } else if (port == 'B'){
            GPIOB->BSRR = (uint32_t)pin << 16U;
        }
    }
    return;
}

void getPin(){
    uint16_t pin = pinMap[tempRecv.data[1]];
    char port = tempRecv.data[2];
    uint8_t pull = tempRecv.data[3];
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Pull = pull;
    if(port == 'A'){
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
        tempRecv.data[2] = HAL_GPIO_ReadPin(GPIOA, pin);
    } else if (port == 'B'){
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
        tempRecv.data[2] = HAL_GPIO_ReadPin(GPIOB, pin);
    }
    HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *) &tempSend, (uint8_t *) &tempRecv, sizeof(tempSend));
    return;
}