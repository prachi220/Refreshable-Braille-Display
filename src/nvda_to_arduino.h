#ifndef _NVDA_TO_ARDUINO_H
#define _NVDA_TO_ARDUINO_H

#include "Arduino.h"

#define SIZE 20
#define STATUS_ACK 0
#define STATUS_NOT_ACK 1
#define STATUS_ERR 2

class nvda_to_arduino {
public:
	int state;
	int buffer[SIZE];
	bool is_temp_data;
	int * temp_data;
//public:
	void init();
	void run();
};
#endif //_NVDA_TO_ARDUINO_H
