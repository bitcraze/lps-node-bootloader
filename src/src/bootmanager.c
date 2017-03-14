/*
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * LPS node bootloader.
 *
 * Copyright 2016, Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Foobar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "bootmanager.h"

#include <stm32f0xx_hal.h>

static void stopInterrupts(void)
{
  // disable global interrupt
  __disable_irq();

  // Disable all peripheral interrupts
  HAL_NVIC_DisableIRQ(HardFault_IRQn);
  HAL_NVIC_DisableIRQ(SVC_IRQn);
  HAL_NVIC_DisableIRQ(PendSV_IRQn);
  HAL_NVIC_DisableIRQ(SysTick_IRQn);

  HAL_NVIC_DisableIRQ(WWDG_IRQn);
  HAL_NVIC_DisableIRQ(PVD_VDDIO2_IRQn);
  HAL_NVIC_DisableIRQ(RTC_IRQn);
  HAL_NVIC_DisableIRQ(FLASH_IRQn);
  HAL_NVIC_DisableIRQ(RCC_CRS_IRQn);
  HAL_NVIC_DisableIRQ(EXTI0_1_IRQn);
  HAL_NVIC_DisableIRQ(EXTI2_3_IRQn);
  HAL_NVIC_DisableIRQ(EXTI4_15_IRQn);
  HAL_NVIC_DisableIRQ(TSC_IRQn);
  HAL_NVIC_DisableIRQ(DMA1_Channel1_IRQn);
  HAL_NVIC_DisableIRQ(DMA1_Channel2_3_IRQn);
  HAL_NVIC_DisableIRQ(DMA1_Channel4_5_6_7_IRQn);
  HAL_NVIC_DisableIRQ(ADC1_COMP_IRQn);
  HAL_NVIC_DisableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
  HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
  HAL_NVIC_DisableIRQ(TIM2_IRQn);
  HAL_NVIC_DisableIRQ(TIM3_IRQn);
  HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
  HAL_NVIC_DisableIRQ(TIM7_IRQn);
  HAL_NVIC_DisableIRQ(TIM14_IRQn);
  HAL_NVIC_DisableIRQ(TIM15_IRQn);
  HAL_NVIC_DisableIRQ(TIM16_IRQn);
  HAL_NVIC_DisableIRQ(TIM17_IRQn);
  HAL_NVIC_DisableIRQ(I2C1_IRQn);
  HAL_NVIC_DisableIRQ(I2C2_IRQn);
  HAL_NVIC_DisableIRQ(SPI1_IRQn);
  HAL_NVIC_DisableIRQ(SPI2_IRQn);
  HAL_NVIC_DisableIRQ(USART1_IRQn);
  HAL_NVIC_DisableIRQ(USART2_IRQn);
  HAL_NVIC_DisableIRQ(USART3_4_IRQn);
  HAL_NVIC_DisableIRQ(CEC_CAN_IRQn);
  HAL_NVIC_DisableIRQ(USB_IRQn);
}

static void copyInterruptsTable(void)
{
  // Copy interrupt vector table to the RAM.
  volatile uint32_t *vectorTable = (volatile uint32_t *)RAM_BASE;

  for(int vectorIndex = 0; vectorIndex < 48; vectorIndex++)
  {
      vectorTable[vectorIndex] = *(__IO uint32_t*)((uint32_t)FIRMWARE_ADDRESS + (vectorIndex << 2));
  }
}

static void remapRam(void)
{
  __HAL_RCC_AHB_FORCE_RESET();

  //  Enable SYSCFG peripheral clock
  __HAL_RCC_SYSCFG_CLK_ENABLE();

  __HAL_RCC_AHB_RELEASE_RESET();

  // Remap RAM into 0x0000 0000
  __HAL_SYSCFG_REMAPMEMORY_SRAM();
}

static void bootFirmware(void)
{
  uint32_t *firmwarePointer = (uint32_t *)FIRMWARE_ADDRESS;

  // First address is the stack pointer initial value
  __set_MSP(*firmwarePointer); // Set stack pointer

  // Now get main app entry point address
  firmwarePointer++;
  void (*firmware)(void) = (void (*)(void))(*firmwarePointer);

  // Jump to main application (0x0800 0004)
  firmware();
}

void bootmanagerStartFirmware(void)
{
  stopInterrupts();
  copyInterruptsTable();
  remapRam();
  bootFirmware();
  while(1); // Should never reach this point!
}
