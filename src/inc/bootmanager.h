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
#ifndef __BOOT_MANAGER_H__
#define __BOOT_MANAGER_H__

#include <stdint.h>

// Useful constants
#define FIRMWARE_ADDRESS ((uint32_t)0x08005000)
#define RAM_BASE ((uint32_t)0x20000000)

/**
 * Stop all interrupt, copy the firmware interrupt vector table to ram,
 * remap ram to begining of address space, set firmware stack pointer and
 * finally jump to the firmware
 */
void bootmanagerStartFirmware(void) __attribute__((noreturn));

#endif // __BOOT_MANAGER_H__
