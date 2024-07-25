#include <ESP8266WiFi.h>
#include <WiFiClient.h>
/* #include <ESP8266WebServer.h> */
/* #include <ESP8266mDNS.h> */
/* #include <uri/UriBraces.h> */

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

#ifndef STASSID
#define STASSID "R7000"
#define STAPSK "9321932193219"
#endif
#define D1 5 // fan assigning the ESP8266 pin to arduino pin
#define D2 4

const char *ssid = STASSID;
const char *password = STAPSK;

/* const char* PUMP_SPEED = "55";					// 55%
 * 의 전력으로 펌프를 구동합니다 */
/* const char* DURATION_POUR = "3";			// 3초간 분사합니다 */
/* const char* DURATION_IDLE = "10";			// 10초간 정지합니다 */
/* /1* const char* PARAM1 = "speed"; *1/ */

String PUMP_SPEED = "55";    // 55% 의 전력으로 펌프를 구동합니다
String DURATION_POUR = "3";  // 3초간 분사합니다
String DURATION_IDLE = "10"; // 10초간 정지합니다

char output[512]; // Serial 출력을 위한 버퍼입니다

/* ESP8266WebServer server(80); */
AsyncWebServer server(80);

/* const int pinLed = 13; */
/* LED_BUILTIN */

int pinPump = D1;
int floatingValue = 0;

void handleRoot() {
    /* Serial.println("came into handle"); */
    /* digitalWrite(pinPump, LOW); */
    /* digitalWrite(LED_BUILTIN, 1); */
    analogWrite(pinPump, 78);
    analogWrite(LED_BUILTIN, 78);
    /* server.send(200, "text/plain", "cdw's cleanest virgina!\r\n"); */
    /* digitalWrite(LED_BUILTIN, 0); */
    analogWrite(LED_BUILTIN, 98);
}

void handleOn() {
    /* Serial.println("came into off"); */
    /* delay(200); */
    /* digitalWrite(pinPump, HIGH); */

    /* delay(200); */
    analogWrite(pinPump, 67);
    digitalWrite(LED_BUILTIN, 0);
    /* server.send(200, "text/plain", "cdw's large pink nipple!\r\n"); */
    digitalWrite(LED_BUILTIN, 1);
}

void handleOff() {
    /* Serial.println("came into off"); */
    /* delay(200); */
    /* digitalWrite(pinPump, HIGH); */

    /* delay(200); */
    analogWrite(pinPump, 0);
    digitalWrite(LED_BUILTIN, 0);
    /* server.send(200, "text/plain", "cdw's large pink nipple!\r\n"); */
    digitalWrite(LED_BUILTIN, 1);
}

/*
void handleNotFound() {
        digitalWrite(LED_BUILTIN, 0);
        String message = "File Not Found\n\n";
        message += "URI: ";
        message += server.uri();
        message += "\nMethod: ";
        message += (server.method() == HTTP_GET) ? "GET" : "POST";
        message += "\nArguments: ";
        message += server.args();
        message += "\n";
        for (uint8_t i = 0; i < server.args(); i++) { message += " " +
server.argName(i) + ": " + server.arg(i) + "\n"; } server.send(404,
"text/plain", message); digitalWrite(LED_BUILTIN, 1);
}
*/

void setup(void) {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(pinPump, OUTPUT);
    /* digitalWrite(pinLed, ); */

    analogWriteRange(100); // to have a range 1 - 100 for the fan
    /* analogWriteFreq(8000); */
    analogWriteFreq(25000);
    /* analogWriteFreq(1000); */

    analogWrite(LED_BUILTIN, 98);
    /* digitalWrite(pinPump, LOW); */

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    /* if (MDNS.begin("esp8266")) { Serial.println("MDNS responder started"); }
     */

    /* server.on("/", handleRoot); */
    /* server.on("/on", handleOn); */
    /* server.on("/off", handleOff); */

    // no async procedure
    /*
    server.on(UriBraces("/v/{}"), [](){
            String speed = server.pathArg(0);
            Serial.println("no async: speed is " + speed);
            server.send(200, "text/plain", "no async : '" + speed + "'");
    });
    */
    server.on("/v", HTTP_GET, [](AsyncWebServerRequest *req) {
        /* String speed = "0"; */
        /* Serial.println(String(req->args()).c_str()); */

        // arg가 있을 때
        if (req->args() > 0) {
            // String speed = req->getParam("speed")->value();

            // speed = String(req->arg("speed")).toInt();
            /* if (strcmp(req->arg("speed"), "0") != 0) { */

            /* if (req->arg("speed") != String("-1")) { */
            if (req->arg("speed") != NULL) {
                PUMP_SPEED = req->arg("speed");
                /* PUMP_SPEED = String(req->arg("speed")).c_str(); */
            }
            /* if (req->arg("pour") != String("-1")) { */
            if (req->arg("pour") != NULL) {
                DURATION_POUR = req->arg("pour");
            }
            if (req->arg("idle") != NULL) {
                /* if (req->arg("idle") != String("-1")) { */
                DURATION_IDLE = req->arg("idle");
            }
        }

        /* Serial.println("async: speed is " + String(PUMP_SPEED)); */
        /* Serial.println("async: pour is " + String(DURATION_POUR)); */
        /* Serial.println("async: idle is " + String(DURATION_IDLE)); */
        /* req->send_P(200, "text/plain", String("speed : " +
         * String(PUMP_SPEED)).c_str()); */
        /* String("pour : " + String(DURATION_POUR)).c_str() + */
        /* String("idle : " + String(DURATION_IDLE)).c_str()); */
        sprintf(output, "speed: %s, pour: %s, idle: %s", PUMP_SPEED.c_str(),
                DURATION_POUR.c_str(), DURATION_IDLE.c_str());
        /* output = String("speed: %s, pour: %s, idle: %s")
         * Serial.println(output);
         */
        Serial.println(output);
        req->send_P(200, "text/plain", output);
        /* req->send_P(200, "text/plain", "sex"); */
        // analogWrite(pinPump, speed.toInt());
    });

    /* server.on("/inline", []() { */
    /* 	server.send(200, "text/plain", "this works as well"); */
    /* }); */

    /* server.onNotFound(handleNotFound); */

    /////////////////////////////////////////////////////////
    // Hook examples

    /*
    server.addHook([](const String& method, const String& url, WiFiClient*
  client, ESP8266WebServer::ContentTypeFunction contentType) { (void)method; //
  GET, PUT, ... (void)url;          // example: /root/myfile.html (void)client;
  // the webserver tcp client connection (void)contentType;  //
  contentType(".html") => "text/html" Serial.printf("A useless web hook has
  passed\n"); Serial.printf("(this hook is in 0x%08x area (401x=IRAM
  402x=FLASH))\n", esp_get_program_counter()); return
  ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
    });

    server.addHook([](const String&, const String& url, WiFiClient*,
  ESP8266WebServer::ContentTypeFunction) { if (url.startsWith("/fail")) {
              Serial.printf("An always failing web hook has been triggered\n");
              return ESP8266WebServer::CLIENT_MUST_STOP;
            }
            return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
    });

    server.addHook([](const String&, const String& url, WiFiClient* client,
  ESP8266WebServer::ContentTypeFunction) { if (url.startsWith("/dump")) {
                    Serial.printf("The dumper web hook is on the run\n");

                    // Here the request is not interpreted, so we cannot for
  sure
                    // swallow the exact amount matching the full
  request+content,
                    // hence the tcp connection cannot be handled anymore by the
                    // webserver.
  #ifdef STREAMSEND_API
                    // we are lucky
                    client->sendAll(Serial, 500);
  #else
                    auto last = millis();
                    while ((millis() - last) < 500) {
                            char buf[32];
                            size_t len = client->read((uint8_t*)buf,
  sizeof(buf)); if (len > 0) { Serial.printf("(<%d> chars)", (int)len);
                              Serial.write(buf, len);
                              last = millis();
                            }
                    }
  #endif
                    // Two choices: return MUST STOP and webserver will close it
                    //                       (we already have the example with
  '/fail' hook)
                    // or                  IS GIVEN and webserver will forget it
                    // trying with IS GIVEN and storing it on a dumb WiFiClient.
                    // check the client connection: it should not immediately be
  closed
                    // (make another '/dump' one to close the first)
                    Serial.printf("\nTelling server to forget this
  connection\n"); static WiFiClient forgetme = *client;  // stop previous one if
  present and transfer client refcounter return
  ESP8266WebServer::CLIENT_IS_GIVEN;
            }
            return ESP8266WebServer::CLIENT_REQUEST_CAN_CONTINUE;
    });
    */

    // Hook examples
    /////////////////////////////////////////////////////////

    server.begin();
    Serial.println("HTTP server started");
    Serial.println("Default speed, pour, idle is " + PUMP_SPEED + ", " +
                   DURATION_POUR + ", " + DURATION_IDLE);
}

void loop(void) {
    /* server.handleClient(); */
    analogWrite(LED_BUILTIN, 91);
    analogWrite(pinPump, String(PUMP_SPEED).toInt());
    delay(String(DURATION_POUR).toInt() * 1000);
    /* delay(5000); */

    analogWrite(LED_BUILTIN, 99);
    analogWrite(pinPump, 0);
    delay(String(DURATION_IDLE).toInt() * 1000);
    /* delay(5000); */
    /* MDNS.update(); */
}
