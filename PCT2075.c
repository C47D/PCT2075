/*
	\mainpage
	\author
	\version
	\date

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

#include "PCT2075.h"
#include <project.h>
#include <stdint.h>

extern uint8_t pointerReg;
extern uint8_t result[2];

/**
*  @brief  Configuration of the PCT2075.
*
*  In order to change the default configuration
*  we can use this function to make the PCT2075
*  fit our needs.
*
*  @param c a pointer to a PCT2075_t variable.
*
*  @see PCT2075_config();
*
*/
void PCT2075_setConfig (PCT2075_t *c){ uint8_t config;

    config = (uint8_t) ((c->QueueVal) << 3) |
                       ((c->OsPolActive) << 2) |
                       ((c->OsPolActive) << 1) |
                       ((c->Mode) << 1);

    I2C_MasterClearStatus();
    I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_WRITE_XFER_MODE);
    if(I2C_mstrStatus == I2C_MSTR_NO_ERROR){
        I2C_mstrStatus = I2C_MasterWriteByte(PCT2075_CONF);
        if(I2C_mstrStatus == I2C_MSTR_NO_ERROR){
            I2C_mstrStatus = I2C_MasterWriteByte(config);
        }
    }
    I2C_MasterSendStop();
    pointerReg = 1;
}

/**
*  @brief  Configuration of the PCT2075.
*
*  In order to change the default configuration
*  we can use this function to make the PCT2075
*  fit our needs.
*
*  @param c a pointer to a PCT2075_t variable.
*
*  @see PCT2075_config();
*
*/
void PCT2075_getConfig (PCT2075_t* configReg){ uint8_t content;

    I2C_MasterClearStatus(); /* Clear any previous status. */
    if(pointerReg != 1){
        I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_WRITE_XFER_MODE);
        if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
            I2C_mstrStatus = I2C_MasterWriteByte( PCT2075_CONF);
            if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
                I2C_mstrStatus = I2C_MasterSendRestart( PCT2075_ADDRESS, I2C_READ_XFER_MODE);
            }
        }
    }else{
        I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_READ_XFER_MODE);
    }
    if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
        content = I2C_MasterReadByte(I2C_NAK_DATA);
    }
    I2C_MasterSendStop();
    pointerReg = 1;

    configReg->QueueVal = (content & 0x18) >> 3;
    configReg->OsPolActive = (content & 0x4) >> 2;
    configReg->OsOperation = (content &2) >> 1;
    configReg->Mode = (content & 0x1);

    return;

}

/**
*  @brief  Configuration of the PCT2075.
*
*  In order to change the default configuration
*  we can use this function to make the PCT2075
*  fit our needs.
*
*  @param c a pointer to a PCT2075_t variable.
*
*  @see PCT2075_config();
*
*/
float PCT2075_getTemp (void){ float temp;

    I2C_MasterClearStatus(); /* Clear any previous status. */
    if(pointerReg != 0){
        I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_WRITE_XFER_MODE);
        if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
            I2C_mstrStatus = I2C_MasterWriteByte( PCT2075_TEMP);
            if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
                I2C_mstrStatus = I2C_MasterSendRestart( PCT2075_ADDRESS, I2C_READ_XFER_MODE);
            }
        }
    }else{
        I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_READ_XFER_MODE);
    }
    if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
        result[0] = I2C_MasterReadByte(I2C_ACK_DATA);
        result[1] = I2C_MasterReadByte(I2C_NAK_DATA);
    }
    I2C_MasterSendStop();
    pointerReg = 0;
    uint16_t concatenate = (((result[0] << 8) | result[1]) >> 5);
    if(concatenate & 0x400u){
        concatenate = ((~concatenate) & 0x7FF) +1;
        temp = concatenate * -0.125;
    }else{
        temp = concatenate * 0.125;
    }
    return (float) temp;
}

/**
*  @brief  Configuration of the PCT2075.
*
*  In order to change the default configuration
*  we can use this function to make the PCT2075
*  fit our needs.
*
*  @param c a pointer to a PCT2075_t variable.
*
*  @see PCT2075_config();
*
*/
void PCT2075_setShutdownThreshold (float Threshold){ uint16_t i; uint8_t lowerByte = 0, upperByte = 0;

    Threshold *= 2;
    if(Threshold < 0){
        i = (~((uint16_t) ((Threshold * -1) - 1))) << 7; // Two's complement
    }else{
        i = (uint16_t) Threshold << 7;
    }

    lowerByte = i & 0x00FF;
    upperByte = i & 0xFF00;

    I2C_MasterClearStatus();
    I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_WRITE_XFER_MODE);
    if(I2C_mstrStatus == I2C_MSTR_NO_ERROR){
        I2C_mstrStatus = I2C_MasterWriteByte(PCT2075_TOS);
        if(I2C_mstrStatus == I2C_MSTR_NO_ERROR){
            I2C_mstrStatus = I2C_MasterWriteByte(upperByte);
            if(I2C_mstrStatus == I2C_MSTR_NO_ERROR){
                I2C_mstrStatus = I2C_MasterWriteByte(lowerByte);
            }
        }
    }
    I2C_MasterSendStop();
    pointerReg = 3;
}

/**
*  @brief  Configuration of the PCT2075.
*
*  In order to change the default configuration
*  we can use this function to make the PCT2075
*  fit our needs.
*
*  @param c a pointer to a PCT2075_t variable.
*
*  @see PCT2075_config();
*
*/
float PCT2075_getShutdownThreshold (void){  float threshold;

    I2C_MasterClearStatus(); /* Clear any previous status. */
    if(pointerReg != 3){
        I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_WRITE_XFER_MODE);
        if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
            I2C_mstrStatus = I2C_MasterWriteByte( PCT2075_TOS);
            if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
                I2C_mstrStatus = I2C_MasterSendRestart( PCT2075_ADDRESS, I2C_READ_XFER_MODE);
            }
        }
    }else{
        I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_READ_XFER_MODE);
    }
    if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
        result[0] = I2C_MasterReadByte(I2C_ACK_DATA);
        result[1] = I2C_MasterReadByte(I2C_NAK_DATA);
    }
    I2C_MasterSendStop();
    pointerReg = 3;
    uint16_t concatenate = (((result[0] << 8) | result[1]) >> 5);
    if(concatenate & 0x100u){
        concatenate = ((~concatenate) & 0x1FF) +1;
        threshold = concatenate * -0.5;
    }else{
        threshold = concatenate * 0.5;
    }
    return (float) threshold;
}

/**
*  @brief  Configuration of the PCT2075.
*
*  In order to change the default configuration
*  we can use this function to make the PCT2075
*  fit our needs.
*
*  @param c a pointer to a PCT2075_t variable.
*
*  @see PCT2075_config();
*
*/
void PCT2075_setHysteresis (float Hysteresis){ uint16_t i; uint8_t lowerByte = 0, upperByte = 0;

    Hysteresis *= 2;
    if(Hysteresis < 0){
        i = (~((uint16_t) ((Hysteresis * -1) - 1))) << 7; // Two's complement
    }else{
        i = (uint16_t) Hysteresis << 7;
    }

    lowerByte = i & 0x00FF;
    upperByte = i & 0xFF00;

    I2C_MasterClearStatus();
    I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_WRITE_XFER_MODE);
    if(I2C_mstrStatus == I2C_MSTR_NO_ERROR){
        I2C_mstrStatus = I2C_MasterWriteByte(PCT2075_THYST);
        if(I2C_mstrStatus == I2C_MSTR_NO_ERROR){
            I2C_mstrStatus = I2C_MasterWriteByte(upperByte);
            if(I2C_mstrStatus == I2C_MSTR_NO_ERROR){
                I2C_mstrStatus = I2C_MasterWriteByte(lowerByte);
            }
        }
    }
    I2C_MasterSendStop();
    pointerReg = 2;
}

/**
*  @brief  Configuration of the PCT2075.
*
*  In order to change the default configuration
*  we can use this function to make the PCT2075
*  fit our needs.
*
*  @param c a pointer to a PCT2075_t variable.
*
*  @see PCT2075_config();
*
*/
float PCT2075_getHysteresis (void){ float hysteresis;

    I2C_MasterClearStatus(); /* Clear any previous status. */
    if(pointerReg != 2){
        I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_WRITE_XFER_MODE);
        if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
            I2C_mstrStatus = I2C_MasterWriteByte( PCT2075_THYST);
            if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
                I2C_mstrStatus = I2C_MasterSendRestart( PCT2075_ADDRESS, I2C_READ_XFER_MODE);
            }
        }
    }else{
        I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_READ_XFER_MODE);
    }
    if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
        result[0] = I2C_MasterReadByte(I2C_ACK_DATA);
        result[1] = I2C_MasterReadByte(I2C_NAK_DATA);
    }
    I2C_MasterSendStop();
    pointerReg = 2;
    uint16_t concatenate = (((result[0] << 8) | result[1]) >> 5);
    if(concatenate & 0x100u){
        concatenate = ((~concatenate) & 0x1FF) +1;
        hysteresis = concatenate * -0.5;
    }else{
        hysteresis = concatenate * 0.5;
    }
    return (float) hysteresis;
}

/**
*  @brief  Configuration of the PCT2075.
*
*  In order to change the default configuration
*  we can use this function to make the PCT2075
*  fit our needs.
*
*  @param c a pointer to a PCT2075_t variable.
*
*  @see PCT2075_config();
*
*/
void PCT2075_setSamplerate (uint8_t Samplerate_ms){
    Samplerate_ms /= 100;

    I2C_MasterClearStatus();
    I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_WRITE_XFER_MODE);
    if(I2C_mstrStatus == I2C_MSTR_NO_ERROR){
        I2C_mstrStatus = I2C_MasterWriteByte(PCT2075_TIDLE);
        if(I2C_mstrStatus == I2C_MSTR_NO_ERROR){
            I2C_mstrStatus = I2C_MasterWriteByte(Samplerate_ms);
        }
    }
    I2C_MasterSendStop();
    pointerReg = 4;
}

/**
*  @brief  Configuration of the PCT2075.
*
*  In order to change the default configuration
*  we can use this function to make the PCT2075
*  fit our needs.
*
*  @param c a pointer to a PCT2075_t variable.
*
*  @see PCT2075_config();
*
*/
uint16_t PCT2075_getSamplerate (void){ uint16_t sample;

    I2C_MasterClearStatus(); /* Clear any previous status. */
    if(pointerReg != 4){
        I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_WRITE_XFER_MODE);
        if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
            I2C_mstrStatus = I2C_MasterWriteByte( PCT2075_TIDLE);
            if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
                I2C_mstrStatus = I2C_MasterSendRestart( PCT2075_ADDRESS, I2C_READ_XFER_MODE);
            }
        }
    }else{
        I2C_mstrStatus = I2C_MasterSendStart( PCT2075_ADDRESS, I2C_READ_XFER_MODE);
    }
    if(I2C_MSTR_NO_ERROR == I2C_mstrStatus){
        sample = (uint16_t) I2C_MasterReadByte(I2C_NAK_DATA);
    }
    I2C_MasterSendStop();
    pointerReg = 4;
    sample *= 100;

    return (uint16_t) sample;
}

/* [] END OF FILE */
