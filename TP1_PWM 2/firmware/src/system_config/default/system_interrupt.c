/*******************************************************************************
 System Interrupts File

  File Name:
    system_interrupt.c

  Summary:
    Raw ISR definitions.

  Description:
    This file contains a definitions of the raw ISRs required to support the
    interrupt sub-system.

  Summary:
    This file contains source code for the interrupt vector functions in the
    system.

  Description:
    This file contains source code for the interrupt vector functions in the
    system.  It implements the system and part specific vector "stub" functions
    from which the individual "Tasks" functions are called for any modules
    executing interrupt-driven in the MPLAB Harmony system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    interrupt-driven in the system.  These handles are passed into the individual
    module "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system/common/sys_common.h"
#include "app.h"
#include "system_definitions.h"
#include <stdint.h>

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************

S_pwmSettings pData;


// *****************************************************************************
/* Fonction :
    void __ISR(_TIMER_1_VECTOR, ipl4AUTO) IntHandlerDrvTmrInstance0(void)

  R�sum� :
    Gestionnaire d'interruption pour le Timer 1. 

  Description :
    Ce gestionnaire d'interruption est d�clench� lorsque le Timer 1 g�n�re une
    interruption (tout les 20ms). Il est utilis� pour mettre � jour
    l'�tat de l'application afficher des informations sur l'afficheur LCD, ex�cuter la
    modulation de largeur d'impulsion (PWM), et appeler une fonction de
    rappel timer1.

  Remarques :
    - APP_UpdateState(APP_STATE_INIT) est appel� pendant les 3 premi�res secondes.

*/
// *****************************************************************************
void __ISR(_TIMER_1_VECTOR, ipl4AUTO) IntHandlerDrvTmrInstance0(void)
{
    static uint8_t compteur3s = 0;
    static uint8_t compteurClearLine = 0;

    // Pendant les 3 premi�res secondes
    if (compteur3s < 149)
    {
        APP_UpdateState(APP_STATE_INIT);
        compteur3s++;
    }
    else
    {
        // Apr�s les 3 premi�res secondes

        // Affiche des informations sur l'afficheur LCD
        if (compteurClearLine <= 0)
        {
            lcd_ClearLine(2);
            lcd_ClearLine(3);
            compteurClearLine++;
        }

        // Allume la LED BSP_LED_0 pendant l'ex�cution des t�ches
        BSP_LEDOn(BSP_LED_0);

        // Obtient les param�tres PWM, les affiche et ex�cute la PWM
        GPWM_GetSettings(&pData);
        GPWM_DispSettings(&pData);
        GPWM_ExecPWM(&pData);

        // Appelle une fonction de rappel timer1
        callback_timer1();

        // �teint la LED BSP_LED_0 apr�s l'ex�cution des t�ches
        BSP_LEDOff(BSP_LED_0);
    }

    // Efface le drapeau d'interruption du Timer 1
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_TIMER_1);
}

// *****************************************************************************
/* Fonction :
    void __ISR(_TIMER_2_VECTOR, ipl0AUTO) IntHandlerDrvTmrInstance1(void)

  R�sum� :
    Gestionnaire d'interruption pour le Timer 2.

  Description :
    Ce gestionnaire d'interruption est d�clench� lorsque le Timer 2 g�n�re une
    interruption. Il est utilis� pour effacer le drapeau d'interruption du Timer 2.

*/
// *****************************************************************************
void __ISR(_TIMER_2_VECTOR, ipl0AUTO) IntHandlerDrvTmrInstance1(void)
{
    // Efface le drapeau d'interruption du Timer 2
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_TIMER_2);
}

// *****************************************************************************
/* Fonction :
    void __ISR(_TIMER_3_VECTOR, ipl0AUTO) IntHandlerDrvTmrInstance2(void)

  R�sum� :
    Gestionnaire d'interruption pour le Timer 3.

  Description :
    Ce gestionnaire d'interruption est d�clench� lorsque le Timer 3 g�n�re une
    interruption. Il est utilis� pour effacer le drapeau d'interruption du Timer 3.

*/
// *****************************************************************************
void __ISR(_TIMER_3_VECTOR, ipl0AUTO) IntHandlerDrvTmrInstance2(void)
{
    // Efface le drapeau d'interruption du Timer 3
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_TIMER_3);
}

// *****************************************************************************
/* Fonction :
    void __ISR(_TIMER_4_VECTOR, ipl7AUTO) IntHandlerDrvTmrInstance3(void)

  R�sum� :
    Gestionnaire d'interruption pour le Timer 4.

  Description :
    Ce gestionnaire d'interruption est d�clench� lorsque le Timer 4 g�n�re une
    interruption (35us). Il est utilis� pour ex�cuter la modulation de largeur d'impulsion
    (PWM) logiciel et allumer/�teindre une LED BSP_LED_1 pendant son ex�cution.

  Remarques :
    - Appelle la fonction GPWM_ExecPWMSoft pour ex�cuter la PWM logiciel.
 

*/
// *****************************************************************************
void __ISR(_TIMER_4_VECTOR, ipl7AUTO) IntHandlerDrvTmrInstance3(void)
{
    // Allume la LED BSP_LED_1 pendant l'ex�cution de la PWM logiciel
    BSP_LEDOn(BSP_LED_1);

    // Ex�cute la PWM logiciel
    GPWM_ExecPWMSoft(&pData);

    // �teint la LED BSP_LED_1 apr�s l'ex�cution de la PWM logiciel
    BSP_LEDOff(BSP_LED_1);

    // Efface le drapeau d'interruption du Timer 4
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_TIMER_4);
}

 /*******************************************************************************
 End of File
*/
