/* ************************************************************************** */
/** Descriptive File Name

 * Menu system for the serial port
  @Company
    SuprTech

  @File Name
    menu.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include "menu.h"
#include "definitions.h"                // SYS function prototypes
#include "global_defs.h"
#include "misc.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************
/*
Menu system via serial port
Checks the serial port periodically and makes decisions based on what
is received

*/

/** 
  @Function
    void DoMenu(void)
  

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */
void DoMenu(void)
{
    static uint8_t RdBuffer[32];
    static uint8_t Index = 0;
    static bool PWM_Status = false;
    
    if(SERCOM3_USART_ReadCountGet())
    {
        SERCOM3_USART_Read(&RdBuffer[Index], 1 );  // Read a byte
        if((RdBuffer[Index] >= 0x20) && (RdBuffer[Index] <= 0x7a))  // Ignore CR LF
        {
            //printf("Got a char %d\r\n", WrBuffer[WrBuffIndex]);
            // We got a good char - do something with it....
            switch(RdBuffer[Index])
            {
                case 's':
                case 'S':
                {
                    StartStopCapture();
                    break;
                } // End start stop collection
                
                case 'p':
                case 'P':
                {
                    // start and stop the onboard PWM generator
                    if(PWM_Status == true)
                    {
                        TC7_CompareStop();
                        PWM_Status = false;
                    }
                    else
                    {
                        TC7_CompareStart();
                        PWM_Status = true;
                    }
                    break;
                } // end case P
                
                case '?':
                {
                    // Print out the menu
                    printf("\r\nMenu Commands:\r\n");
                    printf("s - Start / Stop the data collection\r\n");
                    printf("p - PWM start/stop\r\n");
                    printf("? - This command - prints menu\r\n");
                    break;
                } // end ? - Print out menu
                default:
                    break; // Do nothing - not a char we recognize
            } // End Switch
        }
        Index++;
        if(32 == Index) Index = 0;
    }
    
}


/* *****************************************************************************
 End of File
 */
