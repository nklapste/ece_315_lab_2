/**
 * Keypad.h
 *
 *  Created on: Oct 14, 2016
 *      Author: nem
 *      Some of this code was originally written by Justin Smalley 2008
 *  Modified on: February 6, 2019
 *  	Author: Nathan Klapstein, Thomas Lorincz
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

#include <basictypes.h>

/* Defines for the data available line */
#define HIGH 			1
#define LOW 			0

/* Array storage maximums for the keypad mapping */
#define BUTTONS			16
#define MAX_BUTTON_NAME  10

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define RISING_EDGE		1
#define FALLING_EDGE 	-1

#define KEYPAD_POLL_MODE 0
#define KEYPAD_INT_MODE 1

#define KEYPAD_D_AVAIL 	J2[45] // B side Mux
#define KEYPAD_DO_D		J2[45] // A side Mux
#define KEYPAD_DO_A		J2[33] // A side Mux
#define KEYPAD_DO_B		J2[34] // A side Mux
#define KEYPAD_DO_C		J2[36] // A side Mux

#define KEYPAD_MUX_CTRL J2[35]  // A options  = 0 B options = 1

#define KEYPAD_A_SEL 0
#define KEYPAD_B_SEL 1

const char KeypadButtonMapText[BUTTONS] [MAX_BUTTON_NAME] =
{
"a", "e", "i", "m",
"b", "f", "j", "n",
"c", "g", "k", "o",
"d", "h", "l", "p"
};

class Keypad {

protected:

	/* Most recently read button from the Data Out lines on the encoder */
	unsigned char last_encoded_data;

	/* Read the Data Out lines on the encoder */
	void read_data(void);

	/* parameters setup in {@code Keypad::Init} */
	/* pointer to the interrupt service routine (ISR) function */
	void ( *isr_func )( void );
	BYTE byte_mode;

public:

	/* Constructor initializes only ivar */
	Keypad();

	/**
	 * Initializes all the GPIO pins that interface to the keypad.
	 *
	 * @param mode {@code KEYPAD_POLL_MODE} or {@code KEYPAD_INT_MODE}
	 * @param *button_isr_func pointer to the interrupt service routine function
	 */
	void Init(BYTE mode,  void ( *button_isr_func )( void ));

	/**
	 * @returns a pointer to a string corresponding to the last read button.
	 *
	 * @note Does not read the Data Out lines.
	 */
	const char * GetLastButtonString(void);

	/**
	 * @returns a pointer to a string corresponding to a new button press.
	 *
	 * @note The string mappings are in keypad.cpp
	 * @note This method does read the Data Out lines.
	 */
	const char* GetNewButtonString(void);

	/**
	 * @returns the state of the Data Available line. {@code TRUE} (high)
	 * 		means a button is being pressed and {@code FALSE} (low)
	 * 		means that no button is being pressed.
	 *
	 * @note The data available line goes low once the user takes their
	 * 		finger off a button.
	 */
	unsigned char ButtonPressed(void);

	/**
	 * @returns the value on the encoder DO A, DO B, DO C, and DO D lines.
	 * 		It varies from 0-15.
	 *
	 * @note This method does read the Data out lines.
	*/
	unsigned char GetNewButtonNumber(void);

	/**
	 * @returns the value on the encoder DO A, DO B, DO C,and DO D lines.
	 * 		It varies from 0-15.
	 *
	 * @note This method does not read the Data out lines.
	 */
	unsigned char GetLastButtonNumber(void);
};

#endif /* KEYPAD_H_ */
