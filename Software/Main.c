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
// Entry point
//--------------------------------------------------------------------------------------------------
void main(void)
{
	unsigned char GPIO_Value = 0;

	// Turn off all leds
	gpio = 0;
	// Set all pins as output
	trisio = 0;
	
	while (1)
	{
		gpio = GPIO_Value;
		GPIO_Value = ~GPIO_Value;
		delay_s(1);
	}
}