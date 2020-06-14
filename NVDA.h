#include <SoftwareSerial.h>
#include "Arduino.h"

#ifndef NVDA_h
#define NVDA_h

#define NVDA_SIZE 20
extern SoftwareSerial port1;
class NVDA
{
  public:
	  bool is_src;
  	bool is_command;
  	bool is_header;
  	bool conn_up;
    int prev[NVDA_SIZE];
  	int buff[NVDA_SIZE];
  	void init();
    bool conn_status();
    void set_next();
    void set_previous();
  	bool handshake(); //call handshake before readData
    //void run(SoftwareSerial &port_x);
    int* read_data(bool &is_data);
  	//int* read_data1(bool &is_data);
    bool checkdump(int * buffer);
    void set_data(int * data);
};

#endif
