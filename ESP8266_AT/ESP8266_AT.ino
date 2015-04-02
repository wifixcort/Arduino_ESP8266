#include <SoftwareSerial.h>
//Ricardo Mena C.  Thu Apr 2 2015

/*
Serial comunication with ESP8266 through FTDI
Check IteadStudio to see AT commands
http://wiki.iteadstudio.com/ESP8266_Serial_WIFI_Module

Connections

ESP8266_TX->D0(RX0)
ESP8266_RX->D1(TX1)
ESP8266_CH_PD->3.3V
ESP8266_VCC->3.3V
ESP8266_GND->GND

FTDI_RX->D3
FTDI_TX->D2

For debug purpose. Example
  debug.println("Hello World! XD ");

Special thanks to AllAboutEE
http://allaboutee.com/2014/12/27/esp8266-arduino-code-and-circuit/
This is and implementation of ther code, that include FTDI conecction and
better timing, at least for me XD

*/
/You can modify this value and others delays to, if you have problems reading messages
#define TIMING 10

SoftwareSerial debug(2,3); //(RX, TX)->ESP(TX, RX)
void setup(){
  debug.begin(9600);  //SoftwareSerial conecciont(FTDI)
  Serial.begin(115200); //ESP baudrate, coud be diferent
  Serial.setTimeout(5000);
  Serial.println("AT+RST");
  delay(1000);
}//end setup
 
void loop(){
  if(Serial.available()) // check if the esp is sending a message 
  {
    /
    delay(TIMING);//Wait this time to read message clearly
    while(Serial.available()){
      // The esp has data so display its output to the serial window 
      char c = Serial.read(); // read the next character.
      debug.write(c);
      delay(TIMING);//Wait this time to read message clearly
    }
    debug.println("");
  }
  
  if(debug.available()){
    delay(TIMING);//Wait this time to read message clearly
    
    String command="";
    while(debug.available()){// read the command character by character
    // read one character
    command+=(char)debug.read();
    delay(TIMING);//Wait this time to read message clearly
    }//end while
    Serial.println(command); // send the read character to the esp8266
  }//end if
}//end loop
