#ifndef _ARDUINO_TO_NVDA_H
#define _ARDUINO_TO_NVDA_H

#include "Arduino.h"

class arduino_to_nvda {
public:
	int left_button;
	int right_button;
	long previous_time;
	void init(int left_button, int right_button);
	void run();
};
#endif //_ARDUINO_TO_NVDA_H
