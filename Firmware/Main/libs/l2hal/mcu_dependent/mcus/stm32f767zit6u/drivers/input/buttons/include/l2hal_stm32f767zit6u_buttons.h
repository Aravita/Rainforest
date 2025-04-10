/*
	This file is part of Aravita's STM32 level 2 HAL.

	STM32 level 2 HAL is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

	-------------------------------------------------------------------------

	Created by Aravita Cor

	Feel free to contact: aravita_cor@proton.me

	Repository: https://github.com/Aravita/stm32-l2hal

	-------------------------------------------------------------------------
 */

#ifndef L2HAL_MCU_DEPENDENT_MCUS_STM32F767ZIT6U_DRIVERS_INPUT_BUTTONS_INCLUDE_L2HAL_STM32F767ZIT6U_BUTTONS_H_
#define L2HAL_MCU_DEPENDENT_MCUS_STM32F767ZIT6U_DRIVERS_INPUT_BUTTONS_INCLUDE_L2HAL_STM32F767ZIT6U_BUTTONS_H_

#include "../../../../../../l2hal_mcu.h"

/**
 * Default pin initializer for buttons driver. Clocking port in, setting pin as input and enabling pull-up for it.
 * @port port Pins belongs to this port.
 * @port pins Pins to initialize.
 */
void L2HAL_Buttons_DefaultPinInitializer(GPIO_TypeDef* port, uint32_t pins);

#endif /* L2HAL_MCU_DEPENDENT_MCUS_STM32F767ZIT6U_DRIVERS_INPUT_BUTTONS_INCLUDE_L2HAL_STM32F767ZIT6U_BUTTONS_H_ */
