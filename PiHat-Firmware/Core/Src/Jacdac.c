//Host service
//Control service
//JACDAC service
#include "main.h"
#include "mbed_pinmap.h"
#include "stm32f4xx_ll_gpio.h"
#include "pin_device.h"
extern jd_frame_t tempSend, tempRecv;
extern uint8_t send[4], recv[4];
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

extern void pin_function(uint16_t pin, int data);

void setPin(){
    uint16_t pin = recv[1];
    uint8_t value = recv[2];
    if(value != 3){
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        GPIO_InitStruct.Pin = gpioPinMap(pin);
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        if (value == 0){
            HAL_GPIO_WritePin(GPIOB, gpioPinMap(pin), GPIO_PIN_RESET);
        } else if (value == 1){
            HAL_GPIO_WritePin(GPIOB, gpioPinMap(pin), GPIO_PIN_SET);
        } else if(value == 2){
            GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        }
        HAL_GPIO_Init(gpioPinPort(pin), &GPIO_InitStruct);
    } else {
        HAL_GPIO_DeInit(gpioPinPort(pin), gpioPinMap(pin));
    }
    return;
}

void getPin(){
    //uint16_t pin = tempRecv.data[1];
    uint8_t pull = tempRecv.data[3];
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = gpioPinMap(recv[1]);
    GPIO_InitStruct.Pull = pull;
    HAL_GPIO_Init(gpioPinPort(recv[1]), &GPIO_InitStruct);
    tempRecv.data[2] = HAL_GPIO_ReadPin(gpioPinPort(recv[1]), gpioPinMap(recv[1]));
    HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *) &tempRecv, (uint8_t *) &tempRecv, sizeof(tempSend));
    return;
}

void getButtons(){
    tempRecv.data[1] = 0;
    for (int i = 0; i < (sizeof(buttons)/sizeof(uint16_t)); i++) {
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        uint16_t pin = buttons[i];
        GPIO_InitStruct.Pin = gpioPinMap(recv[1]);
        int v = 0;
        if (i == 0) {
            GPIO_InitStruct.Pull = GPIO_PULLDOWN;
            HAL_GPIO_Init(gpioPinPort(recv[1]), &GPIO_InitStruct);
            v = HAL_GPIO_ReadPin(gpioPinPort(recv[1]), gpioPinMap(recv[1]));
        } else {
            GPIO_InitStruct.Pull = GPIO_PULLUP;
            HAL_GPIO_Init(gpioPinPort(recv[1]), &GPIO_InitStruct);
            v = !HAL_GPIO_ReadPin(gpioPinPort(recv[1]), gpioPinMap(recv[1]));
        }
        if (v) {
            tempRecv.data[1] = i;
            break;
        }
    }
    HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *) &tempRecv, (uint8_t *) &tempRecv, sizeof(tempSend));
    return;
}