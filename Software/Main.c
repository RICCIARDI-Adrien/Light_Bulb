/** @file Main.c
 * Display light patterns on the leds.
 * @author Adrien RICCIARDI
 */
#include <system.h>

//--------------------------------------------------------------------------------------------------
// Microcontroller configuration
//--------------------------------------------------------------------------------------------------
#pragma DATA _CONFIG, _CPD_OFF & _CP_OFF & _BODEN_ON & _MCLRE_OFF & _PWRTE_ON & _WDT_OFF & _INTRC_OSC_NOCLKOUT // Disable EEPROM protection, disable code protection, enable brown-out detection, disable /MCLR pin to use GP3 as a GPIO, enable the power-up timer, disable the watchdog timer, select the internal oscillator and keep all pins as GPIOs
// The clock frequency is needed by software delay functions
#pragma CLOCK_FREQ 4000000

//--------------------------------------------------------------------------------------------------
// Private types
//--------------------------------------------------------------------------------------------------
/** All available leds. */
typedef enum
{
	LED_ID_1_RED, //!< Connected to GP0 pin.
	LED_ID_2_YELLOW, //!< Connected to GP1 pin.
	LED_ID_3_AMBER, //!< Connected to GP2 pin.
	LED_ID_4_GREEN, //!< Connected to GP4 pin.
	LED_ID_5_RED, //!< Connected to GP5 pin.
	LED_IDS_COUNT
} TLedID;

//--------------------------------------------------------------------------------------------------
// Private variables
//--------------------------------------------------------------------------------------------------
/** A latch containing the expected GPIO state (could not directly read from GPIO register due to its read-modify-write working way). */
static unsigned char GPIOs_State = 0;

//--------------------------------------------------------------------------------------------------
// Private functions
//--------------------------------------------------------------------------------------------------
/** Turn a led on.
 * @param Led_ID The led to light.
 */
static void LedOn(unsigned char Led_ID)
{
	// Adjust the led ID to use it as shift value
	if (Led_ID >= LED_ID_4_GREEN) Led_ID++; // Bypass unconnected GP3 pin
	
	// Turn led on
	GPIOs_State |= 1 << Led_ID;
	gpio = GPIOs_State;
}

/** Turn a led off.
 * @param Led_ID The led to turn off.
 */
static void LedOff(unsigned char Led_ID)
{
	// Adjust the led ID to use it as shift value
	if (Led_ID >= LED_ID_4_GREEN) Led_ID++; // Bypass unconnected GP3 pin
	
	// Turn led off
	GPIOs_State &= ~(1 << Led_ID);
	gpio = GPIOs_State;
}

/** Turn all leds on. */
inline void LedAllOn(void)
{
	GPIOs_State = 0x37;
	gpio = 0x37;
}

/** Turn all leds off. */
inline void LedAllOff(void)
{
	GPIOs_State = 0;
	gpio = 0;
}

//--------------------------------------------------------------------------------------------------
// Entry point
//--------------------------------------------------------------------------------------------------
void main(void)
{
	unsigned char i;
	
	// Turn off all leds
	gpio = 0;
	// Set all pins as output
	trisio = 0;
	
	while (1)
	{
		// 1-2-3
		LedOn(LED_ID_1_RED);
		LedOn(LED_ID_3_AMBER);
		LedOn(LED_ID_5_RED);
		delay_s(1);
		
		// 2-4
		LedAllOff();
		LedOn(LED_ID_2_YELLOW);
		LedOn(LED_ID_4_GREEN);
		delay_s(1);
		
		// 1 -> 2 -> 3 -> 4 -> 5
		LedAllOff();
		for (i = 0; i < LED_IDS_COUNT; i++)
		{
			LedOn(i);
			delay_s(1);
		
			LedOff(i);
		}
		
		// 4 -> 3 -> 2 -> 1
		for (i = LED_IDS_COUNT - 2; i != 255; i--)
		{
			LedOn(i);
			delay_s(1);
		
			LedOff(i);
		}
		
		// All leds lighted
		LedAllOn();
		delay_s(1);
		LedAllOff();
		delay_s(1);
		
		// 5 -> 4 -> 3 -> 2 -> 1
		for (i = LED_IDS_COUNT - 1; i != 255; i--)
		{
			LedOn(i);
			delay_s(1);
		
			LedOff(i);
		}
		
		// 2 -> 3 -> 4 -> 5
		for (i = 1; i < LED_IDS_COUNT; i++)
		{
			LedOn(i);
			delay_s(1);
		
			LedOff(i);
		}
		
		// All leds lighted
		LedAllOn();
		delay_s(1);
		LedAllOff();
		delay_s(1);
		
		// 1 -> 1-2 -> 1-2-3 -> 1-2-3-4 -> 1-2-3-4-5
		for (i = 0; i < LED_IDS_COUNT; i++)
		{
			LedOn(i);
			delay_s(1);
		}
		
		// 1-2-3-4-5 -> 1-2-3-4 -> 1-2-3 -> 1-2 -> 1
		for (i = LED_IDS_COUNT - 1; i != 255; i--)
		{
			LedOff(i);
			delay_s(1);
		}
		
		// 5 -> 4-5 -> 3-4-5 -> 2-3-4-5 -> 1-2-3-4-5
		for (i = LED_IDS_COUNT - 1; i != 255; i--)
		{
			LedOn(i);
			delay_s(1);
		}
		
		// 1-2-3-4-5 -> 2-3-4-5 -> 3-4-5 -> 4-5 -> 5
		for (i = 0; i < LED_IDS_COUNT; i++)
		{
			LedOff(i);
			delay_s(1);
		}
	}
}