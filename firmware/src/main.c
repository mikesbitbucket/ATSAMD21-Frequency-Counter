/*******************************************************************************
  D21 Frequency Measure - Main Source File

  Company:
 SupraTech

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include "misc.h"
#include "menu.h"
#include "global_defs.h"

uint32_t w_i;
uint8_t WrBuffIndex = 0;
size_t Status;
uint8_t WrBuffer[32];



// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    

    // Register the SysTick Interrupt Callback
    SYSTICK_TimerCallbackSet( IncSysTick, (uintptr_t) NULL );
    
    // Register Capture Callback
    TC3_CaptureCallbackRegister( FreqCaptureInterrupt, (uintptr_t) NULL );
    
    // Start the SysTick System timer
    SYSTICK_TimerStart();
    
    // Start the capture timer
    TC3_CaptureStart();
    
    // Start the PWM test freq
    TC7_CompareStart();
    
    // Send 'Hello World'
    //w_i = sprintf((char*)WrBuffer, "Hello World!\r\n");
    //Status = SERCOM3_USART_Write(WrBuffer, w_i );
    printf("Motor Pulse Measurement\r\n");
    printf("Freq (Hz), Min Period (ms), Max Period (ms)\r\n\r\n");
    
//    w_i = sprintf((char*)WrBuffer, "Length: %ld\r\n", w_i);
//    Status = SERCOM3_USART_Write(WrBuffer, w_i );
    

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        DoHeartBeat();
        
        DoMenu();
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

