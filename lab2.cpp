/**
 * lab2.cpp
 *
 * Copyright 2019 Nathan Klapstein, Thomas Lorincz
 */

#include <predef.h>
#include <stdio.h>
#include <ctype.h>
#include <startnet.h>
#include <autoupdate.h>
#include <smarttrap.h>
#include <taskmon.h>
#include <NetworkDebug.h>
#include "keypad.h"
#include "error_wrapper.h"
#include "LCD.h"

/* Custom button definitions mapping keypad numbers to a direction */
#define UP 15
#define RIGHT 12
#define LEFT 3
#define DOWN 0

extern "C" {
	void UserMain(void * pd);
}

const char * AppName="Lab 2: Nathan Klapstein, Thomas Lorincz";
Keypad myKeypad;
LCD myLCD;

/**
 * Position of the dollar $ sprite/
 *
 * Initially set to the top left corner of the LCD screen.
 */
point cursor = {0, 0};

// custom dollar $ sprite
const BYTE dollar[7] = {0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12, 0x00};

/**
 * Move and redraw the dollar $ sprite.
 *
 * @param direction integer that represents a direction to move the dollar $ sprite.
 * 		Valid integers representing directions include:
 *			0 - moves the dollar $ spite up
 *			15 - moves the dollar $ sprite down
 *			3 - moves the dollar $ sprite right
 *			12 - moves the dollar $ sprite left
 *
 * The dollar $ sprite's position is protected from being moved off of the LCD screen.
 *
 * Other integers will cause the dollar sprite $ to redraw without changing position.
 */
void moveDollar(int direction)
{
	printf("%d", direction);
	switch (direction)
	{
		case UP:
			if (cursor.row > 0)
			{
				cursor.row -= 1;
			}
			break;
		case DOWN:
			if (cursor.row < 5)
			{
				cursor.row += 1;
			}
			break;
		case LEFT:
			if (cursor.col > 0)
			{
				cursor.col -= 7;
			}
			break;
		case RIGHT:
			if (cursor.col < 77)
			{
				cursor.col += 7;
			}
			break;
		default:
			break;
	}

	// update the LCD screen
	myLCD.Clear();
	myLCD.DrawChar(dollar, cursor);
}

/**
 * Flag that notes that the keypad button is pressed.
 *
 * {@code EdgePortISR1} **will** set this to {@code true}
 * {@code UserMain} **can** set this to {@code false}
 */
volatile bool buttonPressed;

/**
 * Interrupt service request handler.
 *
 * Sets {@code buttonPressed} to {@code true}. This then proceeds to the change
 * behavior within the main execution loop.
 */
void EdgePortISR1(void)
{
	// Insert your ISR code here for exercise 3
	buttonPressed = true;
}

/**
 * Main execution
 *
 * Sets up both the Keypad and LCD and starts the loop checking the buttonPressed flag.
 */
void UserMain(void * pd)
{
	BYTE err = OS_NO_ERR;

    InitializeStack();
    OSChangePrio(MAIN_PRIO);
    EnableAutoUpdate();
    StartHTTP();
    EnableTaskMonitor();

    #ifndef _DEBUG
    EnableSmartTraps();
    #endif

    #ifdef _DEBUG
    InitializeNetworkGDB_and_Wait();
    #endif

    // new keypad interrupt service request
    myKeypad.Init(KEYPAD_INT_MODE, &EdgePortISR1);

    myLCD.Init();
    myLCD.Clear();
    myLCD.DrawChar(dollar, cursor);

    iprintf("Application started %s\n", AppName);
    while (1)
    {
    	if (buttonPressed)
    	{
    		moveDollar(myKeypad.GetNewButtonNumber());
    		// set the buttonPressed flag to false as we just finished handling a button press
    		buttonPressed = false;
    	}
    }
}
