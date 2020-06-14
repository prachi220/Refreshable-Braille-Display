/*State 1 = read data from nvda;
State 2 = send data to hosts;
State 3 = seek for acknowledgement;
if ack is true go to state 1 else go to state 2;
*/
#include "NVDA.h"
#include "BrailleModule.h"
#include "nvda_to_arduino.h"

int dummy = 0;
extern NVDA nvda;
extern BrailleModule host1, host2;
bool is_data_empty = false;
//extern arduino_to_nvda a2n;

void nvda_to_arduino::init() {
	state = 0;
	is_temp_data = false;
	//initializing the buffer
	for (int i = 0; i < SIZE; i++) {
		buffer[i] = 0;
	}
}

void nvda_to_arduino::run() {
	if(state == 0) {
		Serial.println(" N2A : state 0");
		// read from the nvda and fill the buffer;
		int * received_buffer;
		if (!is_temp_data) {
			received_buffer = nvda.read_data(is_data_empty);
		} else {
			received_buffer = temp_data;
		}
		if (!is_data_empty) {
			for (int i = 0 ; i < SIZE; i++) {
				buffer[i] = received_buffer[i];
			}
     		state = 1;
		} else {
			state = 0;
            is_temp_data = false;
		}
	} 
	if(state == 1) {
		Serial.println(" N2A : state 1 - actuate");
		//send the data to hosts
		host1.resume();
		host2.resume();
		int buffer_host1[SIZE/2], buffer_host2[SIZE/2];
		for (int i = 0 ; i < SIZE/2; i++) {
			buffer_host1[i] = buffer[i];
			buffer_host2[i] = buffer[i + (SIZE/2)];
		}
		host1.actuate(buffer_host1);
		host2.actuate(buffer_host2);
		// change the state 2;
		state = 2;
	} 
	if(state == 2) {
		Serial.println(" N2A : state 2");
		// check the acknowledgement;
		temp_data = nvda.read_data(is_data_empty);
		is_temp_data = is_data_empty;
		int status_1, status_2;
		status_1 = host1.parse_message();
		status_2 = host2.parse_message();
		/*
		if ack is recived
			if ack id true
				go to state 3;
			else // means error
				say some thing here;
		else ack_not recived
			be in this state;
		*/
		if ((status_1 == STATUS_ACK) && (status_2 == STATUS_ACK)) {
			state = 0;
			//clearing the buffer
			for (int i = 0; i < SIZE; i++) {
				buffer[i] = 0;
			}
		} else if ((status_1 == STATUS_ERR) or (status_2 == STATUS_ERR)) {
			state = 3;
		} else {
			state = 2;
		}
	} 
	if(state == 3) {   			
		Serial.println(" N2A : state 3");
		//error in presenting the data to hosts;
	}
}
