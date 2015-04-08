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

TODO no responde al iniciar sin tocar nada solo si se le pone momentaneamente
RST a GND y se quita

*/

#include <SoftwareSerial.h>

SoftwareSerial debug(2, 3);//ARDUINO(RX, TX)->ESP8266(TX, RX)


#define _serial Serial
#define _dbg  debug

#define LED_GREEN 12
#define LED_RED 11

#define SSID "YourApName"
#define PASSWORD "YourApPassword"


boolean _find(String text, String fnd1 = "OK", String fnd2 = "no change");

boolean ini_blink = false;
boolean at_cwjap_flag = false;
boolean at_flag = false;

String at_cwjap ="AT+CWJAP=\"";


void setup(){
  _serial.begin(115200);//ESP serial
  _dbg.begin(9600);//Debug serial, FTDI
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  blink_leds(5000);//wait that ESP8266 stabilize
  _serial.println("AT");
  serial_read(1000);
  _serial.println("AT+RST");
  serial_read(5000);  
  _serial.println("AT");
  if(_find(serial_read(1000))){//ESP8266 is good, otherwise ckeck
      digitalWrite(LED_GREEN, HIGH);
      delay(1000);
      digitalWrite(LED_GREEN, LOW);
      at_flag = true;
  }else{
      blink_red();//check ESP8366
  }//end if
  if(at_flag){
    _serial.println("AT+CWMODE=1");//set station mode
    if(_find(serial_read(500))){
      digitalWrite(LED_GREEN, HIGH);
      delay(1000);
      digitalWrite(LED_GREEN, LOW);
    }//end if
  }//end if
    at_cwjap += SSID;
    at_cwjap += "\",\"";
    at_cwjap += PASSWORD;
    at_cwjap += "\"";
}//end setup
void loop(){
  //Join AP
  if(!at_cwjap_flag){
    at_cwjap_flag = join_ap(at_cwjap);//Joining AP
  }
  
  //TODO verificar cada 2h
  //Check if you lost the connection, if this happend, allow check it again
  if(at_cwjap_flag){
    at_cwjap_flag =check_ip("192.168.2.");//expect always true
  }
  
  
  
}

//Join AP
boolean join_ap(String &jap){
  _serial.println(jap);//Joining AP
  if(_find(serial_read(10000))){
    /*In the start it need a lot of time
    If just have "lost connection" need less.*/
    digitalWrite(LED_GREEN, HIGH);
    return true;// joinig successful
  }else{
    blink_red();
  }
  return false;
}//end join_ap


//Check IP
boolean check_ip(String ip_range){
  //expected IP range
  _serial.println("AT+CIFSR");
  //It supposed that I should use "AT+CIPSTATUS" but can entangle, I prefere this way
  return _find(serial_read(500), ip_range);
}//end check_ip

void blink_red(){
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);  
  unsigned long start = millis();
  while(millis()-start < 5000){//blink 5s
    delay(500);
    digitalWrite(LED_RED, HIGH);
    delay(500);
    digitalWrite(LED_RED, LOW);
    delay(500);
  }//end while
}//end blink_red


void blink_leds(unsigned int timIn){  
  unsigned long start = millis();
  while(millis()-start < timIn){//blink this time
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_RED, LOW);
    delay(500);
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    delay(500);
  }//end while
  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, HIGH);
  delay(250);
  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
}//end blink_leds


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
  while(millis()-start < timeOut){//Read befor this time
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
  while(millis()-start < timeOut){//Read befor this time
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
