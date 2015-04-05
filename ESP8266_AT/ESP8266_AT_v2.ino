/*
Ricardo Mena C Apr 5 2015

rgacort12@gmail.com

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
  _dbg.println("Hello World! :) ");

*/

#include <SoftwareSerial.h>

SoftwareSerial debug(2, 3);//ARDUINO(RX, TX)->ESP8266(TX, RX)

#define _serial Serial
#define _dbg  debug

boolean _find(String text, String fnd1 = "OK", String fnd2 = "no change");

void setup(){
  _serial.begin(115200);//ESP serial
  _dbg.begin(9600);//Debug serial, FTDI
  flush_serial();
  delay(1000);
}
void loop(){
  //AT+RST take 2s
  //Some petitions take less than 2s, then you can modify this for some purposes
  serial_read(1000);
  serial_write(1000);
}


//Find if some instrucction goes wrong or good
//You can use this if you don't have a FTDI, example
/*
  _serial.println("AT+CWJAP=\"SSID\",\"\PASSWORD"");
  if(_find(serial_read(1000), "OK")){
    digitalWrite(red_led, LOW);
    digitalWrite(green_led, HIGH);
  }else{
    digitalWrite(green_led, LOW);
    digitalWrite(red_led, HIGH);
  }
*/

boolean _find(String text, String fnd1, String fnd2){
  if((text.indexOf(fnd1)!=-1) || (text.indexOf(fnd2)!=-1)){
    return true;
  }//end if
  return false;
}//end _find


String serial_write(unsigned long timeOut){
  String cmd="";
  unsigned long start = millis();
  while(millis()-start < timeOut){ 
    if(_dbg.available()){
      cmd+=(char)_dbg.read();
    }//end if
  }//end while
  if(cmd!=""){
    _serial.println(cmd);
  }
  return cmd;
}//end serial_write

String serial_read(unsigned long timeOut){
  String msg = "";
  unsigned long start = millis();
  while(millis()-start < timeOut){   
    if(_serial.available()){   
      char c = _serial.read(); // read the next character.
      msg +=c;
    }//end if
  }//end while
  if(msg!=""){
    _dbg.println(msg);
  }
  return msg;
}//end serial_read


void flush_serial(){
  _serial.flush();
  _dbg.flush();
}
