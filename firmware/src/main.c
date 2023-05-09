/*******************************************************************************
  D21 Frequency Measure - Main Source File
 * Measures and incoming frequency on PB00, and send statistics out USB Serial
 * port. Takes an average every 200ms

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
    
    // Register 24bit Callback
    TCC0_CaptureCallbackRegister(Freq24BitInterrupt, (uintptr_t) NULL );
    
    // Start the SysTick System timer
    SYSTICK_TimerStart();
    
    // Start the capture timer
    //TC3_CaptureStart();
    
    // Start the 24bit capture timer
    TCC0_CaptureStart();
    
    // Start the PWM test freq
    //TC7_CompareStart();
    
    // Send 'Hello World'
    //w_i = sprintf((char*)WrBuffer, "Hello World!\r\n");
    //Status = SERCOM3_USART_Write(WrBuffer, w_i );
    printf("\r\nMenu Commands:\r\n");
    printf("\r\nMenu Commands:\r\n");
    printf("s - Start / Stop the data collection\r\n");
    printf("p - PWM start/stop\r\n");
    printf("? - This command - prints menu\r\n\r\n");
    printf("Motor Pulse Measurement\r\n");
    printf("Time(Secs), Freq (Hz), Min Period (ms), Max Period (ms), Average Period Count\r\n\r\n");
    
//    w_i = sprintf((char*)WrBuffer, "Length: %ld\r\n", w_i);
//    Status = SERCOM3_USART_Write(WrBuffer, w_i );
    

    while ( true )
    {
        
        DoHeartBeat(); // LED toggle and other housekeeping tasks
        
        DoMenu(); // Check if user wants to do something....
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

