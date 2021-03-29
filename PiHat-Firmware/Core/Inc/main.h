/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

//Max frame size 256 Bytes
struct _jd_frame_t {
    //uint16_t frame_crc;
    //uint8_t frame_size;
    //unit8_t frame_flags;
    //unint64_t device_identifier;
    uint8_t data[4];
} __attribute__((__packed__, aligned(4)));
typedef struct _jd_frame_t jd_frame_t;

//Max packet size 240 Bytes
struct _jd_packet_t {
  uint8_t service_size;
  uint8_t service_index;
  uint16_t service_command;
  uint8_t payload[236];
} __attribute__((__packed__, aligned(4)));
typedef struct _jd_packet_t jd_packet_t;

void setPin();
void getPin();
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
