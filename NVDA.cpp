/******************************************************************************
 * Includes
 ******************************************************************************/

#include "Arduino.h"
#include "NVDA.h"
#include "SoftwareSerial.h"

// bool is_data;

int read_buff[NVDA_SIZE];

/******************************************************************************
 * User API
 ******************************************************************************/
//initialises allt he values
void NVDA::init()
{
	is_src = false;
	is_command = false;
	is_header = true;
	for(int i = 0; i < NVDA_SIZE; i++)
		buff[i] = 0;
}

void NVDA::set_next() {
	int array[8];
  array[0] = 0x1b;
  array[1] = 0x00;
  array[2] = 0xf3;
  array[3] = 0x00;
  array[4] = 0x00;
  array[5] = 0x00;
  array[6] = 0x00;
  array[7] = 0x02;
  for (int i = 0; i < 8; i++)
    port1.write(array[i]);
}

void NVDA::set_previous() {
	int array[8];
  array[0] = 0x1b;
  array[1] = 0x00;
  array[2] = 0xf3;
  array[3] = 0x00;
  array[4] = 0x00;
  array[5] = 0x00;
  array[6] = 0x00;
  array[7] = 0x04;
  for (int i = 0; i < 8; i++)
    port1.write(array[i]);
}

// void NVDA::run(SoftwareSerial &port_x) {
// 	//Serial.println("Thread for NVDA");
// 	while(port_x.available()) {
// 		Serial.print(port_x.read(),DEC);
// 		Serial.print(" ");
// 	}
// 	Serial.print("\n");
// 	port_x.flush();
// }

int* NVDA::read_data(bool &is_data_empty) {
	bool is_same = true;
	bool is_dump = false;
	is_dump = checkdump(buff);
	for (int i = 0; i < NVDA_SIZE; i++) {
		if (prev[i] != buff[i]) {
			is_same = false;
			break;
		}
	}
	is_data_empty = (is_same or is_dump);
	if(!is_data_empty) {
		for (int i = 0; i < NVDA_SIZE; i++) {
			prev[i] = buff[i];
		}
	}
	return buff;
}

//reads data of length buf_size when conn_up is true
/*
int* NVDA::read_data1(bool &is_data){
	//Serial.println(port1.available());
	//Serial.println("inside read data of nvda");
 	bool is_stuff = false;
	is_header = true;
    for (int i = 0; i < NVDA_SIZE; i++) {
        buff[i] = 0;
    }
	//if(port1.available() > 4)  {
		// Serial.println(port1.available());
      	int datasize = port1.available();
      	int count = 0, count_pt = 0;
        int headr_state = 0;
        while (port1.available()) {
        	int temp = port1.read();
        	//Serial.print(temp, DEC);
       		//Serial.print(" ");
	        if ((temp == 0X1B) && (headr_state == 0)) {
	            headr_state = 1;
	        }
	        if ((temp == 0X01) && (headr_state == 1)) {
	          headr_state = 2;
	        }
	        if ((temp == 0X03) && (headr_state == 2)) {
	          headr_state = 3;
	          count_pt = 0;
            for(int i = 0; i< NVDA_SIZE; i++)
              buff[i] = 0;
	        }
	        if (headr_state == 3) {
	        	is_stuff = true;
           		for (int i = 0; i < NVDA_SIZE; i++) {
	                int temp_read = port1.read();
	                //Serial.print(temp_read, DEC);
	                //Serial.print(" ");
	                buff[i] = temp_read;
                	if (temp_read < 0)
                  		buff[i] = 0;
            		}
              		headr_state = 0;
              		count_pt = 0;
	          	}
	        count++;	
        }
        // Serial.print("\n Parsed : ");
        // for (int i = 0; i < NVDA_SIZE; i++) {
        //   Serial.print(buff[i]);
        //   Serial.print(" ");
        // }
        // Serial.print("\n total port1.available() : ");
        // Serial.print(count);
        // Serial.print(" ~ ");
        // Serial.println(port1.available());
         port1.flush();
        // Serial.print("FLushed data : ");
        // Serial.println(port1.available());   
	//}
	bool is_same = true;
	bool is_dump = false;
	is_dump = checkdump(buff);
	for (int i = 0; i < NVDA_SIZE; i++) {
		if (prev[i] != buff[i]) {
			is_same = false;
			break;
		}
	}
 	if (is_stuff && (!is_same) && (!is_dump)){
 		//Serial.println("Char sending : ");
 		for (int i = 0; i < NVDA_SIZE; i++) {
 			prev[i] = buff[i];
 			// Serial.print(buff[i]);
 			// Serial.print(" ");
 		}
 		//Serial.print("\n");
    	is_data = false;
  		return buff;
  	} else {
	  	is_data = true;
  	}
	return buff;
}*/

void NVDA::set_data(int * data) {
	for (int i = 0 ; i < NVDA_SIZE; i++)
		buff[i] = data[i];
}


bool NVDA::checkdump(int * buffer) {
	if((buff[0] == 13) && (buff[1] == 37) && (buff[2] == 7) )
    	return true;
  	if((buff[0] == 101) && (buff[1] == 29))
    	return true;
	return false;
}
