/*
NXP - PTC2075

I2C-bus Fm+, 1 degree C accuracy, digital temperature sensor
and thermal watchdog.

Temperature-to-digital converter featuring ±1 ºC accuracy over
-25 ºC to +100 ºC range.

Version 0.1

In OS comparator mode, the OS outputs behaves like a thermostat. It becomes
active when the Temp exceeds the Tots, and is reset when the Temp drops below
Thys. The OS output in this case can be used to control cooling fans or thermal
switches.

In OS interrupt mode, the OS output is used for thermal interruption. When the
device is powered-up, the OS output is first activated only when the Temp exceeds
the Tots, then it remains active indefinitely until being reset by a read of any
register. Once the OS output has been activated by crossing Tots and then reset,
it can be activated again only when the Temp drops below the Thys; then again,
it remains active indefinitely until being reset by a read of any register.

----------------------------------------------------------------------
Copyright (C), 2015

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
----------------------------------------------------------------------

*/

/**
 * @file PCT2075.h
 *
 * @brief
 *
 * @details
 *
 */
#include <project.h>
#include <stdint.h>

#ifndef PCT2075_H
#define PCT2075_H

#define PCT2075_ADDRESS 0x48u /**< Assuming A2, A1, A0 are connected to ground.*/

/*
Temperature register:
Read only
POR state 0x0000
*/
#define PCT2075_TEMP 	0x00u /**< Contains two 8-bit data bytes to store the measured Temp data.*/
/*
Configuration register:
R/W
POR state 0x00
*/
#define PCT2075_CONF 	0x01u /**< Contains a single 8-bit data byte to set the device operating condition.*/
/*
Hysteresis register:
R/W
POR state 0x4B00
*/
#define PCT2075_THYST 	0x02u /**< contains two 8-bit data bytes to store the hysteresis This limit; default is 75ºC.*/
/*
Overtemperature shutdown threshold register:
R/W
POR state 0x5000
*/
#define PCT2075_TOS		0x03u /**< Contains two 8-bit data bytes to store the overtemperature shutdown Tots limit; default is 80ºC*/
/*
R/W
POR state 0x00
*/
#define PCT2075_TIDLE 	0x04u /**< Temperature conversion cycle default to 100 ms.*/

// Configuration Register
#define PCT2075_CONF_OS_F_QUE_BIT 	        4u
#define PCT2075_CONF_OS_F_QUE_BIT_LENGHT    2u
#define PCT2075_CONF_OS_POL_BIT 	        2u
#define PCT2075_CONF_OS_COMP_INT_BIT 	    1u
#define PCT2075_CONF_SHUTDOWN_BIT 	        0u

#define PCT2075_QUEUE_VAL_1		0x00u
#define PCT2075_QUEUE_VAL_2		0x01u
#define PCT2075_QUEUE_VAL_4		0x10u
#define PCT2075_QUEUE_VAL_6		0x11u
#define PCT2075_OS_POL_ACTIVE_LOW	0u
#define PCT2075_OS_POL_ACTIVE_HIGH	1u
#define PCT2075_OS_COMPARATOR_OPERATION	0u
#define PCT2075_OS_INTERRUPT_OPERATION	1u
#define PCT2075_NORMAL_MODE		0u
#define PCT2075_SHUTDOWN_MODE		1u

/** @brief */
typedef struct PCT2075{
	uint8_t 	QueueVal;		/**< Module ID.*/
	uint8_t	    OsPolActive;	/**< Module ID.*/
	uint8_t 	OsOperation;	/**< Module ID.*/
	uint8_t 	Mode;		/**< Module ID.*/
} PCT2075_t;

uint8_t pointerReg = 0;
uint8_t result[2] = {0u, 0u};

/** @brief */
void PCT2075_setConfig (PCT2075_t *c);

/** @brief */
void PCT2075_getConfig (PCT2075_t* configReg);

/** @brief */
float PCT2075_getTemp (void);

/** @brief */
void PCT2075_setShutdownThreshold (float Threshold);

/** @brief */
float PCT2075_getShutdownThreshold (void);

/** @brief */
void PCT2075_setHysteresis (float Hysteresis);

/** @brief */
float PCT2075_getHysteresis (void);

/** @brief */
void PCT2075_setSamplerate (uint8_t Samplerate_ms);

/** @brief */
uint16_t PCT2075_getSamplerate (void);

#endif /* PCT2075_H__ */

/* [] END OF FILE */