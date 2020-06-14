#include "arduino_to_nvda.h"
#include "Arduino.h"
#include "NVDA.h"

extern NVDA nvda;

void arduino_to_nvda::init(int left_button, int right_button) {
	left_button = left_button;
	right_button = right_button;
}

void arduino_to_nvda::run() {
	int left_read, right_read;
	left_read = digitalRead(left_button);
	right_read = digitalRead(right_button);
 //Serial.println("Checking for key presses");
	if (left_read == HIGH) {
		// tell nvda to set next 20 chars to the port buffer
		nvda.set_next();
	}
	if (right_read == HIGH) {
		// tell nvda to set the previous 20 chars to the port buffer
		nvda.set_previous();
	}
}
