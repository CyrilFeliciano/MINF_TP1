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

#define TEMPSINIT 149

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************

S_pwmSettings pData;


// *****************************************************************************
/* Fonction :
    void __ISR(_TIMER_1_VECTOR, ipl4AUTO) IntHandlerDrvTmrInstance0(void)

  Résumé :
    Gestionnaire d'interruption pour le Timer 1. 

  Description :
    Ce gestionnaire d'interruption est déclenché lorsque le Timer 1 génère une
    interruption (tout les 20ms). Il est utilisé pour mettre à jour
    l'état de l'application afficher des informations sur l'afficheur LCD, exécuter la
    modulation de largeur d'impulsion (PWM), et appeler une fonction de
    rappel timer1.

  Remarques :
    - APP_UpdateState(APP_STATE_INIT) est appelé pendant les 3 premières secondes.

*/
// *****************************************************************************
void __ISR(_TIMER_1_VECTOR, ipl4AUTO) IntHandlerDrvTmrInstance0(void)
{
    static uint8_t compteur3s = 0;
    static uint8_t compteurClearLine = 0;

    // Pendant les 3 premières secondes
    if (compteur3s < TEMPSINIT)
    {
        APP_UpdateState(APP_STATE_INIT);
        compteur3s++;
    }
    else
    {
        // Après les 3 premières secondes

        // Affiche des informations sur l'afficheur LCD
        if (compteurClearLine <= 0)
        {
            lcd_ClearLine(2);
            lcd_ClearLine(3);
            compteurClearLine++;
        }

        // Allume la LED BSP_LED_0 pendant l'exécution des tâches
        BSP_LEDOn(BSP_LED_0);

        // Obtient les paramètres PWM, les affiche et exécute la PWM
        GPWM_GetSettings(&pData);
        GPWM_DispSettings(&pData);
        GPWM_ExecPWM(&pData);

        // Appelle une fonction de rappel timer1
        callback_timer1();

        // Éteint la LED BSP_LED_0 après l'exécution des tâches
        BSP_LEDOff(BSP_LED_0);
    }

    // Efface le drapeau d'interruption du Timer 1
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_TIMER_1);
}

// *****************************************************************************
/* Fonction :
    void __ISR(_TIMER_2_VECTOR, ipl0AUTO) IntHandlerDrvTmrInstance1(void)

  Résumé :
    Gestionnaire d'interruption pour le Timer 2.

  Description :
    Ce gestionnaire d'interruption est déclenché lorsque le Timer 2 génère une
    interruption. Il est utilisé pour effacer le drapeau d'interruption du Timer 2.

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

  Résumé :
    Gestionnaire d'interruption pour le Timer 3.

  Description :
    Ce gestionnaire d'interruption est déclenché lorsque le Timer 3 génère une
    interruption. Il est utilisé pour effacer le drapeau d'interruption du Timer 3.

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

  Résumé :
    Gestionnaire d'interruption pour le Timer 4.

  Description :
    Ce gestionnaire d'interruption est déclenché lorsque le Timer 4 génère une
    interruption (35us). Il est utilisé pour exécuter la modulation de largeur d'impulsion
    (PWM) logiciel et allumer/éteindre une LED BSP_LED_1 pendant son exécution.

  Remarques :
    - Appelle la fonction GPWM_ExecPWMSoft pour exécuter la PWM logiciel.
 

*/
// *****************************************************************************
void __ISR(_TIMER_4_VECTOR, ipl7AUTO) IntHandlerDrvTmrInstance3(void)
{
    // Allume la LED BSP_LED_1 pendant l'exécution de la PWM logiciel
    BSP_LEDOn(BSP_LED_1);

    // Exécute la PWM logiciel
    GPWM_ExecPWMSoft(&pData);

    // Éteint la LED BSP_LED_1 après l'exécution de la PWM logiciel
    BSP_LEDOff(BSP_LED_1);

    // Efface le drapeau d'interruption du Timer 4
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_TIMER_4);
}

 /*******************************************************************************
 End of File
*/
