/* ************************************************************************** */
/** Misc Functions

  @Company
    SupraTech

  @File Name
    misc.c

  @Summary
    Misc functions - timers, etc.

  @Description
    Various button handling functions
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */


#include "definitions.h" 
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
static uint32_t MySysTick = 0;
static uint32_t Heartbeat_tmr = 0;
static uint32_t SerialUpdate_tmr = 0;

//static uint8_t LED_Heartbeat_tmr;


static bool WindowFlag = false;

//
//enum FlashStates
//{
//    START = 0,
//    FIRST_ON,
//    FIRST_OFF,
//    SECOND_ON,
//    SECOND_OFF,
//    FULL_ON
//};



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
//static int ExampleLocalFunction(int param1, int param2) {
//    return 0;
//}








/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */


// *****************************************************************************

/** 
  @Function
    IncSysTick 

  @Summary
 Increments the System Tic

  @Remarks
 In SysTic
 */
//void IncSysTick(void)
void IncSysTick( uintptr_t context )
{
    // Note: Context is not used but needed for function definition - if left void, will throw error
    MySysTick++;
}



/** 
  @Function
    ClearSysTick 

  @Summary
 Clears the System Tic

  @Remarks
 Clear SysTic
 */
void ClearSysTick(void)
{
    MySysTick = 0;
}

// *****************************************************************************

/** 
  @Function
    GetSysTick 

  @Summary
    Gets the current SysTick

  @Remarks
    Get and return current SysTick
 */
uint32_t GetSysTick(void)
{
    return MySysTick;  // Is this atomic on this processor or do we have to disable interrupts
}



// *****************************************************************************

/** 
  @Function
    DoHeartBeat 

  @Summary
 * Does any heartbeat related tasks - blink LED, etc

  @Remarks
 *  Heartbeat Stuff
 */

void DoHeartBeat()
{
    //static size_t counter = 0;
    
    // Heartbeat check - Also the Blink Status of the LED
    if((uint32_t)(GetSysTick() - Heartbeat_tmr) >= LED_HEARTBEAT_INTERVAL)
    {
        Heartbeat_tmr = GetSysTick(); // get new time val
        
        LED_Toggle();  // Toggle the on board LED
        
    } // End LED Beat
    
    // Check if time to send out new update
    if((uint32_t)(GetSysTick() - SerialUpdate_tmr) >= SERIAL_UPDATE_INTERVAL)
    {
        // set flag to update serial output
        WindowFlag = true;
        SerialUpdate_tmr = GetSysTick();  // get new time val
    }
        
}  // end heartbeat
    
void FreqCaptureInterrupt( TC_CAPTURE_STATUS status, uintptr_t context )
{
    uint8_t i;
    int int_i;
    static uint8_t BufCount = 0;
    uint32_t PeriodSum, PulseSum;
    float PeriodAverage, PulseAverage, Freq;
    float TimeMaxPer, TimeMinPer;
    static uint16_t PeriodBuf[256], PulseBuf[256];
    static uint16_t MinPer = 0xffff, MaxPer = 0;  // holds min and max period for each cycle
    uint8_t WrBuffer[32];
    
    // Check Status Flags??
    // Page 597 on family pdf
//    if(status != 0)
//    {
//        // test and clear flags (just clearing now - no testing...)
//        TC3_REGS->COUNT16.TC_INTFLAG = 0xff; // Clear flags
//    }
    if ((status  & TC_CAPTURE_STATUS_CAPTURE0_READY) == TC_CAPTURE_STATUS_CAPTURE0_READY)
    {
        // it is a good capture
    
    
        // We got a new period, get the period and duty cycle
        PeriodBuf[BufCount] = TC3_Capture16bitChannel0Get();  // This is the period count
        PulseBuf[BufCount] = TC3_Capture16bitChannel1Get();  // Pulse width count
        if(PeriodBuf[BufCount] < MinPer) MinPer = PeriodBuf[BufCount];
        if(PeriodBuf[BufCount] > MaxPer) MaxPer = PeriodBuf[BufCount];
        BufCount++;

        if(WindowFlag == true)
        {
            // Time to send out new data

            // Get new averages
            PeriodSum = 0; PulseSum = 0;
            for(i=0; i < BufCount; i++)
            {
                PeriodSum += PeriodBuf[i];
                PulseSum += PulseBuf[i];
            }
            PeriodAverage = (float)(PeriodSum)/i;
            PulseAverage = (float)(PulseSum)/i;

            Freq = (TC3_CaptureFrequencyGet() / PeriodAverage);
            TimeMinPer = (float)(MinPer) / TC3_CaptureFrequencyGet() * 1000UL;
            TimeMaxPer = (float)(MaxPer) / TC3_CaptureFrequencyGet() * 1000UL;
            //printf("Freq: %.3f Min: %.3f Max %.3f", Freq, TimeMinPer, TimeMaxPer);
            printf("%.3f,%.3f,%.3f", Freq, TimeMinPer, TimeMaxPer);  // no test on what each is
            //printf(" Count: %ld", PeriodSum);
            printf("\r\n");
            
            //printf("PerAve: %.2f PulAve: %.2f\r\n", PeriodAverage, PulseAverage);
            //int_i = sprintf((char*)WrBuffer, "PerAve: %.2f PulAve: %.2f\r\n", PeriodAverage, PulseAverage);
            //i = SERCOM3_USART_Write(WrBuffer, int_i );

            // Reset all the variables
            BufCount = 0;
            WindowFlag = false; // set back to false
            MinPer = 0xffff;  // reset min and max variables
            MaxPer = 0;

        }
    }
    else
    {
        // Not good capture - do something with the errors??
        TC3_REGS->COUNT16.TC_INTFLAG = 0xff; // Clear flags for now... Should not get here....
    }

}


/* *****************************************************************************
 End of File
 */
