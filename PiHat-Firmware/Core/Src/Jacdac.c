//Host service
//Control service
#include "main.h"
#include "mbed_pinmap.h"
#include "stm32f4xx_ll_gpio.h"
#include "pin_device.h"
extern jd_frame_t frameSend, frameRecv;
extern jd_packet_t pktSend, pktRecv;
extern SPI_HandleTypeDef hspi1;
extern uint16_t broadcast_size;
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
    uint8_t pin = pktRecv.payload[0];
    uint8_t value = pktRecv.payload[1];

    // // clear previous mode
    // GPIO_PORT()->MODER &= ~(0x2 << (2*pin));
    // // set new mode (output)
    // GPIO_PORT()->MODER |= (0x1 << (2*pin));
    
    // // push pull output
    // GPIO_PORT()->OTYPER &= ~(0x1 << pin);

    // // clear pull up
    // GPIO_PORT()->PUPDR &= ~(0x2 << (2*pin));

    if (value){
        GPIO_PORT()->BSRR = GPIO_PIN();
    } else {
        GPIO_PORT()->BSRR = GPIO_PIN() << 16;
    }
    pin_function(pin, STM_PIN_DATA(STM_PIN_OUTPUT, GPIO_NOPULL, 0));
    return;
}

void getPin(){
    uint16_t pin = pktRecv.payload[1];
    uint8_t pull = pktRecv.payload[3];
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN();
    GPIO_InitStruct.Pull = pull;
    HAL_GPIO_Init(GPIO_PORT(), &GPIO_InitStruct);
    pktSend.payload[2] = HAL_GPIO_ReadPin(GPIO_PORT(), GPIO_PIN());
    frameSend.data = pktSend;
    HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *) &frameSend, (uint8_t *) &frameRecv, sizeof(frameSend));
    return;
}

void getButtons(){
    pktSend.payload[1] = 0;
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
            pktSend.payload[1] = i;
            break;
        }
    }
    frameSend.data = pktSend;
    HAL_SPI_TransmitReceive_DMA(&hspi1, (uint8_t *) &frameSend, (uint8_t *) &frameRecv, sizeof(frameSend));
    return;
}

void getAnalog(){

}