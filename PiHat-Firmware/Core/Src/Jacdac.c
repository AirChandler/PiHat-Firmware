//Host service
//Control service
#include "main.h"
extern jd_frame_t tempSend, tempRecv;
extern SPI_HandleTypeDef hspi1;
uint16_t buttons[] = {
    0, // menu
    23, // A
    22, // B
    17, // left
    15, // right
    8, // up
    10, // down
};

void setPin(){
    uint16_t pin = tempRecv.data[1];
    uint8_t pull = tempRecv.data[2];
    if (pull){
        GPIO_PORT()->BSRR = GPIO_PIN();
    } else {
        GPIO_PORT()->BSRR = GPIO_PIN() << 16;
    }
    return;
}

void getPin(){
    uint16_t pin = tempRecv.data[1];
    uint8_t pull = tempRecv.data[3];
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN();
    GPIO_InitStruct.Pull = pull;
    HAL_GPIO_Init(GPIO_PORT(), &GPIO_InitStruct);
    tempRecv.data[2] = HAL_GPIO_ReadPin(GPIO_PORT(), GPIO_PIN());
    HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *) &tempRecv, (uint8_t *) &tempRecv, sizeof(tempSend));
    return;
}

void getButtons(){
    tempRecv.data[1] = 0;
    for (int i = 0; i < (sizeof(buttons)/sizeof(uint16_t)); i++) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        uint16_t pin = buttons[i];
        GPIO_InitStruct.Pin = GPIO_PIN();
        int v = 0;
        if (i == 0) {
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            HAL_GPIO_Init(GPIO_PORT(), &GPIO_InitStruct);
            v = HAL_GPIO_ReadPin(GPIO_PORT(), GPIO_PIN());
        } else {
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            HAL_GPIO_Init(GPIO_PORT(), &GPIO_InitStruct);
            v = !HAL_GPIO_ReadPin(GPIO_PORT(), GPIO_PIN());
        }
        if (v) {
            tempRecv.data[1] = i;
            break;
        }
    }
    HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *) &tempRecv, (uint8_t *) &tempRecv, sizeof(tempSend));
    return;
}