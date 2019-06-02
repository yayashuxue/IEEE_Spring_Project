#include "SoftwareSerial.h"
String ssid ="WIFI ID";
String password="WIFI password";

SoftwareSerial esp(6, 7);// RX, TX

String data;

String server = "ucsdlaundary.000webhostapp.com"; // www.example.com 

String uri = "/laundry.php";// our example is /esppost.php

int DHpin = 8;//sensor pin


// dryer machine
const int sensorIn = A0;
const int sensorDry = A1;
// machine's light
int light1, light2, mode;
// Threshhold of lightening
const int threshhold = 2;

 byte dat [5];

String washer,dryer;

void setup() {

pinMode (DHpin, OUTPUT);
//sesonr input
pinMode(sensorIn, INPUT);
pinMode(sensorDry, INPUT);


esp.begin(9600);

Serial.begin(9600);

reset();

connectWifi();
  
}

//reset the esp8266 module

void reset() {

esp.println("AT+RST");

delay(1000);

if(esp.find("OK") ) Serial.println("Module Reset");

}

//connect to your wifi network

void connectWifi() {

String cmd = "AT+CWJAP=\"" +ssid+"\",\"" + password + "\"";

esp.println(cmd);

delay(4000);

if(esp.find("OK")) {

Serial.println("Connected!");

}

else {

connectWifi();

Serial.println("Cannot connect to wifi"); }

}

/*
 * There is three return value 0, 1, 2
 * 0 - vacant 
 * 1 - working
 * 2 - completed but not vacant 
 */
int MachineMode(int light1, int light2){
  Serial.print("1:    ");
  Serial.println(light1);
    Serial.print("2:    ");
  Serial.println(light2);

  if(light1 >= threshhold)
  {
    return 0;
  }else if(light2 >= threshhold)
  {
    return 1;
  }
  else{
    return 2;
  }
  }

void loop () {

//start_test ();


light1 = analogRead(sensorIn);
light2 = analogRead(sensorDry);
mode = MachineMode(light1, light2);

washer = "0";

dryer = String(mode);
  Serial.print("dryer:    ");
  Serial.println(dryer);


data = "washerMode=" + washer + "&dryerMode=" + dryer;// data sent must be under this form //name1=value1&name2=value2.

httppost();

delay(1000); // TODO turn into 8000 in real life

}

void httppost () {

esp.println("AT+CIPSTART=\"TCP\",\"" + server + "\",80");//start a TCP connection.

if( esp.find("OK")) {

Serial.println("TCP connection ready");

} delay(1000);

String postRequest =

"POST " + uri + " HTTP/1.0\r\n" +

"Host: " + server + "\r\n" +

"Accept: *" + "/" + "*\r\n" +

"Content-Length: " + data.length() + "\r\n" +

"Content-Type: application/x-www-form-urlencoded\r\n" +

"\r\n" + data;

String sendCmd = "AT+CIPSEND=";//determine the number of caracters to be sent.

esp.print(sendCmd);

esp.println(postRequest.length() );

delay(500);

if(esp.find(">")) { Serial.println("Sending.."); esp.print(postRequest);

if( esp.find("SEND OK")) { Serial.println("Packet sent");

while (esp.available()) {

String tmpResp = esp.readString();

Serial.println(tmpResp);

}

// close the connection

esp.println("AT+CIPCLOSE");

}

}}
