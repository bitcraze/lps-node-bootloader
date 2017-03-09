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

#include <stm32f0xx.h>

#include "system.h"
#include "gpio.h"
#include "led.h"
#include "bootmode.h"

static void bootLoad();
static void boot();

int main() {
  // Reset of all peripherals, Initializes the Flash interface and the Systick.
  HAL_Init();

  // Configure the system clock
  SystemClock_Config();

  // Initialize GPIO
  MX_GPIO_Init();

  if (!isBootLoaderModeActivated()) {
    HAL_Delay(500);
    clearBootLoaderModeFlag();
    boot();
  } else {
    clearBootLoaderModeFlag();
    bootLoad();
  }
}

static void bootLoad() {
  while (1) {
    ledOff(ledSync);
    ledOff(ledMode);
    ledOff(ledRanging);

    HAL_Delay(500);

    ledOn(ledSync);
    ledOn(ledMode);
    ledOn(ledRanging);

    HAL_Delay(500);
  }
}

static void boot() {
  while (1) {
    ledOff(ledSync);

    HAL_Delay(200);

    ledOn(ledSync);

    HAL_Delay(200);
  }
}

