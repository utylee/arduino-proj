/* #include <ESP8266WiFi.h> */
/* #include <WiFiClient.h> */
/* #include <ESP8266WebServer.h> */
/* #include <ESP8266mDNS.h> */

#ifndef STASSID
#define STASSID "R7000"
#define STAPSK "9321932193219"
#endif
#define D1 5 // fan assigning the ESP8266 pin to arduino pin
#define D2 4

/* int pinFan = 5; */
int pinFan = D1;
/* int pinFan = D2; */
/* int pinLED = 16; */
int floatingValue = 0;

void setup(){
	/* Serial.begin(115200); */
	pinMode(pinFan, OUTPUT);
	pinMode(LED_BUILTIN, OUTPUT);
	analogWriteRange(100); // to have a range 1 - 100 for the fan
	/* analogWriteFreq(8000); */
	analogWriteFreq(25000);
	/* analogWriteFreq(1000); */
}

void loop(){
	/* digitalWrite(pinFan, HIGH); */
	/* digitalWrite(pinFan, LOW); */
	/* analogWrite(pinFan, 253); */
	/* analogWrite(pinFan, 1022); */
	/* for (int i=0; i< 98;i++) */

	/* for (int i=85; i > 79;i=i-1) */
	/* { */
	/* 	analogWrite(pinFan, i); */
	/* 	analogWrite(LED_BUILTIN, i); */
	/* 	delay(1000); */

	/* } */

	analogWrite(LED_BUILTIN, 98);

	analogWrite(pinFan, 95);
	delay(5000);
	analogWrite(pinFan, 0);
	delay(3000);

	/* analogWrite(pinFan, 82); */
	/* delay(5000); */
	/* analogWrite(pinFan, 0); */
	/* delay(3000); */

	/* analogWrite(pinFan, 72); */
	/* delay(5000); */
	/* analogWrite(pinFan, 0); */
	/* delay(3000); */

	/* analogWrite(pinFan, 62); */
	/* delay(5000); */
	/* analogWrite(pinFan, 0); */
	/* delay(3000); */


	analogWrite(pinFan, 52);
	delay(5000);
	analogWrite(pinFan, 0);
	/* analogWrite(LED_BUILTIN, 0); */
	delay(3000);

	/* digitalWrite(LED_BUILTIN, LOW); */
	/* //floatingValue = digitalRead(pinFan); */
	/* //Serial.println(floatingValue); */
	/* delay(7000); */
	/* analogWrite(pinFan, 48); */
	/* delay(7000); */

	/* //digitalWrite(pinFan, LOW); */
	/* analogWrite(pinFan, 0); */
	/* digitalWrite(LED_BUILTIN, HIGH); */
	/* delay(7000); */
}
