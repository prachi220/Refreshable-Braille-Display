/******************************************************************************
 * Includes
 ******************************************************************************/

#include "BrailleModule.h"
#include "Arduino.h"

extern SoftwareSerial port;
/******************************************************************************
 * User API
 ******************************************************************************/
void BrailleModule::init_host(int host_id)
{
	host_addr = host_id;
}
/*
 * Actuate with data of 10 Bytes
 */
void BrailleModule::actuate(int data[])
{
	bool is_same = true;
	for(int i = 0;i < 10; i++) {
		if(prev_data[i] != data[i]) {
			is_same = false;
			break;
		}
	}
  if((millis() - last_acutate > 500) && (!is_same)) {
	int sd[ACTUATE_LTH+4];
	sd[0] = SOM;
	sd[1] = ACTUATE_LTH;
	sd[2] = SRC;
	sd[3] = host_addr;
	sd[4] = ACTUATE_CMD;
	for(int i=0;i<10;i++) {
		prev_data[i] = data[i];
		sd[5+i] = data[i];
	}
	
	// chks calculation
	int chks=sd[2];
	for(int i=0;i<ACTUATE_LTH-1;i++){
		chks^=sd[i+3];
	}
	sd[ACTUATE_LTH+2]=chks;
	sd[ACTUATE_LTH+3] = EOM;
  //Serial.print("Acutate on host : " + host_addr + " ");
	for(int i=0;i<ACTUATE_LTH+4;i++) {
	    port.write(sd[i]);
      Serial.print(sd[i]);
      Serial.print(" ");
	 }
	//Serial.print("\n");
  last_acutate = millis();
	} else {
	   //Serial.println("Too fast actutate");
	  }
}
/*
 * Gives the status of device whether it is ready to send data or any error present in the module
 */
void BrailleModule::get_device_status()
{
	int sd[7];
	sd[0] = SOM;
	sd[1] = 3;
	sd[2] = SRC;
	sd[3] = host_addr;
	sd[4] = GET_DEVICE_STATUS_CMD;
	sd[5] = (sd[2]^sd[3])^sd[4];//checksum
	sd[6] = EOM;
	for(int i=0;i<7;i++)
		port.write(sd[i]);
}
/*
 * Do the Module Self test part on request
 */
void BrailleModule::self_test()
{
	int sd[7];
	sd[0] = SOM;
	sd[1] = 3;
	sd[2] = SRC;
	sd[3] = host_addr;
	sd[4] = SELF_TEST_CMD;
	sd[5] = (sd[2]^sd[3])^sd[4];//checksum
	sd[6] = EOM;
	for(int i=0;i<7;i++)
		port.write(sd[i]);
}
/* 
 * Set time interval at which data to be sent 
 * when cursor is pressed continuously (Data format is DATA = Time in mS/10)
 * Default Value is 100 mS so data is 0x0A
 */
void BrailleModule::set_cursor(int data)
{	
	int sd[8];
	sd[0] = SOM;
	sd[1] = 4;
	sd[2] = SRC;
	sd[3] = host_addr;
	sd[4] = SET_CURSOR_KEY_DATA_TIME_CMD;
	sd[5] = data;
	sd[6] = (((sd[2]^sd[3])^sd[4])^sd[5]);//checksum
	sd[7] = EOM;
	for(int i=0;i<8;i++)
		port.write(sd[i]);
}
/*
 * Get Feedback time of each pins
 */
void BrailleModule::get_fb()
{
	int sd[7];
	sd[0] = SOM;
	sd[1] = 3;
	sd[2] = SRC;
	sd[3] = host_addr;
	sd[4] = GET_FB_TIME_INFO;
	sd[5] = ((sd[2]^sd[3])^sd[4]);//checksum
	sd[6] = EOM;
	for(int i=0;i<7;i++)
		port.write(sd[i]);
}
/*
 * Resume the System from Halt/Error Condition
 */
void BrailleModule::resume()
{
	int sd[7];
	sd[0] = SOM;
	sd[1] = 3; // Length
	sd[2] = SRC;
	sd[3] = host_addr;
	sd[4] = RESUME_CMD;
	sd[5] = ((sd[2]^sd[3])^sd[4]);//checksum
	sd[6] = EOM;
	for(int i=0;i<7;i++)
		port.write(sd[i]);
}
/*
 * ERROR
 */
void BrailleModule::error(int sub_cmd)
{
	int sd[8];
	sd[0] = SOM;
	sd[1] = 4; // Length
	sd[2] = SRC;
	sd[3] = host_addr;
	sd[4] = ERR_CMD;
	sd[5] = sub_cmd;
	sd[6] = (((sd[2]^sd[3])^sd[4])^sd[5]);//checksum
	sd[7] = EOM;
	for(int i=0;i<8;i++)
		port.write(sd[i]);
}

bool BrailleModule::is_data_written()
{
	return true;
}

int BrailleModule::parse_message()
{
	int size = 8;
	bool end = false;
	char message[2*size];
	int count = 0;
	if(port.available())
	{
		while((!end)&&(count<size))
		{
			//int temp = port.read();
			//printf("%d\n", temp);
			// if(temp == EOM)
			// {
			// 	end = true;
			// }
			// else
			// {
				sprintf(&message[2*count], "%02X", port.read());
			//}
			count++;
		}
	}
	return 0;
}
