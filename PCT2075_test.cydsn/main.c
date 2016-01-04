/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>
#include "PCT2075.h"

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    float temp;
    
    I2C_Start();    

    for(;;)
    {
        temp = PCT2075_getTemp();
    }
}

/* [] END OF FILE */
