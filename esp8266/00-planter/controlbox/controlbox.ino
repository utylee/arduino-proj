// #include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiClient.h>
// #include <uri/UriBraces.h>
#include <ArduinoOTA.h>
#include <WiFiUdp.h>

#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// #include <tone.h>

#ifndef STASSID
#define STASSID "R7000"
#define STAPSK "9321932193219"
#endif
#define D1 5 // fan assigning the ESP8266 pin to arduino pin
#define D2 4
#define D5 14 // fan assigning the ESP8266 pin to arduino pin
#define D6 12
#define D7 13 // fan assigning the ESP8266 pin to arduino pin
// #define D8 15

const char *ssid = STASSID;
const char *password = STAPSK;

/* const int pinLed = 13; */
/* LED_BUILTIN */

int pinPump1A = D1;
int pinPump1B = D5;

int pinPump2A = D2;
int pinPump2B = D6;

int pinFan = D5;

/* const char* PUMP_SPEED = "55";					// 55%
 * 의 전력으로 펌프를 구동합니다 */
/* const char* DURATION_POUR = "3";			// 3초간 분사합니다 */
/* const char* DURATION_IDLE = "10";			// 10초간 정지합니다 */
/* /1* const char* PARAM1 = "speed"; *1/ */

unsigned long prevMillis = 0;
unsigned long msCur = 0;
unsigned long msSprayMoment = 0;
unsigned long msDrainMoment = 0;

bool bAction = true;
// bool bAction = false;
bool bSprayStarted = false;
bool bDrainStarted = false;

// spray 펌프 설정입니다
// String SPRAY_PUMP_SPEED = "85"; // 55% 의 전력으로 펌프를 구동합니다
// String SPRAY_DURATION_ON = "6";  // 3초간 분사합니다
// String SPRAY_DURATION_OFF = "4"; // 10초간 정지합니다
// drain 펌프 설정입니다
// String DRAIN_PUMP_SPEED = "99"; // 55% 의 전력으로 펌프를 구동합니다
// String DRAIN_DURATION_ON = "10";   // 3초간 분사합니다
// String DRAIN_DURATION_OFF = "0.5"; // 10초간 정지합니다

long SPRAY_PUMP_SPEED = 95; // 55% 의 전력으로 펌프를 구동합니다
// long SPRAY_DURATION_ON = 0;   // 3초간 분사합니다
long SPRAY_DURATION_ON = 5;   // 3초간 분사합니다
long SPRAY_DURATION_OFF = 5;  // 10초간 정지합니다
long DRAIN_PUMP_SPEED = 75;   // 55% 의 전력으로 펌프를 구동합니다
long DRAIN_DURATION_ON = 10;  // 3초간 분사합니다
long DRAIN_DURATION_OFF =10 ; // 10초간 정지합니다
// long DRAIN_DURATION_OFF = 60; // 10초간 정지합니다
long FAN_SPEED=30;

char output[512]; // Serial 출력을 위한 버퍼입니다
char stopped_warning[255];

// ESP8266WebServer server(80);
AsyncWebServer server(80);

// int floatingValue = 0;

void set_start(AsyncWebServerRequest *req) {
    // // PUMP_SPEED = "55";    // 55% 의 전력으로 펌프를 구동합니다
    // DURATION_POUR = "0";  // 3초간 분사합니다
    // DURATION_IDLE = "0"; // 10초간 정지합니다
    // sprintf(output, "Started...\r\nspeed: %s, pour: %s, idle: %s",
    //         SPRAY_PUMP_SPEED.c_str(), SPRAY_DURATION_ON.c_str(),
    //         SPRAY_DURATION_OFF.c_str());
    // req->send_P(200, "text/plain", output);

    // 각 시각에 현재시각을 기록해줍니다
    msCur = millis();
    msSprayMoment = msCur;
    msDrainMoment = msCur;

    bAction = true;

    sprintf(output,
            "***Started...\r\nSpray:\r\n\tspeed: %lu, pour: %lu, idle: "
            "%lu\r\nDrain:\r\n\tspeed: %lu, pour: %lu, idle: %lu",
            SPRAY_PUMP_SPEED, SPRAY_DURATION_ON, SPRAY_DURATION_OFF,
            DRAIN_PUMP_SPEED, DRAIN_DURATION_ON, DRAIN_DURATION_OFF);
    Serial.println(output);
    req->send_P(200, "text/plain", output);
}

void set_stop(AsyncWebServerRequest *req) {
    // start = false;
    bAction = false;
    // // PUMP_SPEED = "55";    // 55% 의 전력으로 펌프를 구동합니다
    // DURATION_POUR = "0";  // 3초간 분사합니다
    // DURATION_IDLE = "0"; // 10초간 정지합니다
    // sprintf(output, "Stopped!!\r\nspeed: %s, pour: %s, idle: %s",
    //         SPRAY_PUMP_SPEED.c_str(), SPRAY_DURATION_ON.c_str(),
    //         SPRAY_DURATION_OFF.c_str());

    // sprintf(output, "Stopped!!\r\nspeed: %lu, pour: %lu, idle: %lu",
    //         SPRAY_PUMP_SPEED, SPRAY_DURATION_ON, SPRAY_DURATION_OFF);
    // // output = String("speed: %s, pour: %s, idle: %s")
    // Serial.println(output);
    // req->send_P(200, "text/plain", output);

    sprintf(output,
            "***Stopped...\r\nSpray:\r\n\tspeed: %lu, pour: %lu, idle: "
            "%lu\r\nDrain:\r\n\tspeed: %lu, pour: %lu, idle: %lu",
            SPRAY_PUMP_SPEED, SPRAY_DURATION_ON, SPRAY_DURATION_OFF,
            DRAIN_PUMP_SPEED, DRAIN_DURATION_ON, DRAIN_DURATION_OFF);
    Serial.println(output);
    req->send_P(200, "text/plain", output);
}

void set_default(AsyncWebServerRequest *req) {
    // SPRAY_PUMP_SPEED = "55";   // 55% 의 전력으로 펌프를 구동합니다
    // SPRAY_DURATION_ON = "3";   // 3초간 분사합니다
    // SPRAY_DURATION_OFF = "10"; // 10초간 정지합니다
    SPRAY_PUMP_SPEED = 55;   // 55% 의 전력으로 펌프를 구동합니다
    SPRAY_DURATION_ON = 3;   // 3초간 분사합니다
    SPRAY_DURATION_OFF = 10; // 10초간 정지합니다

    if (!bAction) {
        sprintf(stopped_warning, "\r\nbut stopped now!");
    } else {
        sprintf(stopped_warning, "");
    }

    // sprintf(output, "Set to Default...\r\nspeed: %s, pour: %s, idle: %s %s",
    //         SPRAY_PUMP_SPEED.c_str(), SPRAY_DURATION_ON.c_str(),
    //         SPRAY_DURATION_OFF.c_str(), stopped_warning);
    sprintf(output, "Set to Default...\r\nspeed: %lu, pour: %lu, idle: %lu %s",
            SPRAY_PUMP_SPEED, SPRAY_DURATION_ON, SPRAY_DURATION_OFF,
            stopped_warning);
    // output = String("speed: %s, pour: %s, idle: %s")
    Serial.println(output);
    req->send_P(200, "text/plain", output);
}

void set_spray(AsyncWebServerRequest *req) {
    // arg가 있을 때
    if (req->args() > 0) {
        if (req->arg("speed") != NULL) {
            // SPRAY_PUMP_SPEED = req->arg("speed");
            SPRAY_PUMP_SPEED = req->arg("speed").toInt();
        }
        if (req->arg("pour") != NULL) {
            // SPRAY_DURATION_ON = req->arg("pour");
            SPRAY_DURATION_ON = req->arg("pour").toInt();
        }
        if (req->arg("idle") != NULL) {
            // SPRAY_DURATION_OFF = req->arg("idle");
            SPRAY_DURATION_OFF = req->arg("idle").toInt();
        }
    }

    if (!bAction) {
        sprintf(stopped_warning, "\r\nbut stopped now!");
    } else {
        sprintf(stopped_warning, "");
    }

    // sprintf(output, "speed: %s, pour: %s, idle: %s %s",
    //         SPRAY_PUMP_SPEED.c_str(), SPRAY_DURATION_ON.c_str(),
    //         SPRAY_DURATION_OFF.c_str(), stopped_warning);
    sprintf(output, "Spary set:\r\nspeed: %lu, pour: %lu, idle: %lu %s",
            SPRAY_PUMP_SPEED, SPRAY_DURATION_ON, SPRAY_DURATION_OFF,
            stopped_warning);
    // output = String("speed: %s, pour: %s, idle: %s")
    Serial.println(output);
    req->send_P(200, "text/plain", output);
}

void set_drain(AsyncWebServerRequest *req) {
    // arg가 있을 때
    if (req->args() > 0) {
        if (req->arg("speed") != NULL) {
            DRAIN_PUMP_SPEED = req->arg("speed").toInt();
        }
        if (req->arg("pour") != NULL) {
            DRAIN_DURATION_ON = req->arg("pour").toInt();
        }
        if (req->arg("idle") != NULL) {
            DRAIN_DURATION_OFF = req->arg("idle").toInt();
        }
    }

    if (!bAction) {
        sprintf(stopped_warning, "\r\nbut stopped now!");
    } else {
        sprintf(stopped_warning, "");
    }

    sprintf(output, "Drain set:\r\nspeed: %lu, pour: %lu, idle: %lu %s",
            DRAIN_PUMP_SPEED, DRAIN_DURATION_ON, DRAIN_DURATION_OFF,
            stopped_warning);
    // output = String("speed: %s, pour: %s, idle: %s")
    Serial.println(output);
    req->send_P(200, "text/plain", output);
}

void handleRoot() {
    /* Serial.println("came into handle"); */
    /* digitalWrite(pinPump, LOW); */
    /* digitalWrite(LED_BUILTIN, 1); */
    analogWrite(pinPump1A, 78);
    analogWrite(pinPump1B, 0);
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
    analogWrite(pinPump1A, 67);
    analogWrite(pinPump1B, 0);
    digitalWrite(LED_BUILTIN, 0);
    /* server.send(200, "text/plain", "cdw's large pink nipple!\r\n"); */
    digitalWrite(LED_BUILTIN, 1);
}

void handleOff() {
    /* Serial.println("came into off"); */
    /* delay(200); */
    /* digitalWrite(pinPump, HIGH); */

    /* delay(200); */
    analogWrite(pinPump1A, 0);
    analogWrite(pinPump1B, 0);
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

    pinMode(pinPump1A, OUTPUT);
    pinMode(pinPump1B, OUTPUT);
    pinMode(pinPump2A, OUTPUT);
    pinMode(pinPump2B, OUTPUT);
    pinMode(pinFan, OUTPUT);
    /* digitalWrite(pinLed, ); */
    digitalWrite(pinPump1A, LOW);
    digitalWrite(pinPump1B, LOW);
    digitalWrite(pinPump2A, LOW);
    digitalWrite(pinPump2B, LOW);
    digitalWrite(pinFan, LOW);

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

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else { // U_FS
            type = "filesystem";
        }

        // NOTE: if updating FS this would be the place to unmount FS using
        // FS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() { Serial.println("\nEnd"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        }
    });
    ArduinoOTA.begin();

    // if (MDNS.begin("esp8266")) { Serial.println("MDNS responder started"); }

    // server.on("/", handleRoot);
    // server.on("/on", handleOn);
    // server.on("/off", handleOff);

    //// no async procedure
    // server.on(UriBraces("/v/{}"), [](){
    //         String speed = server.pathArg(0);
    //         Serial.println("no async: speed is " + speed);
    //         server.send(200, "text/plain", "no async : '" + speed + "'");
    // });

    server.on("/default", HTTP_GET, set_default);
    server.on("/start", HTTP_GET, set_start);
    server.on("/stop", HTTP_GET, set_stop);
    server.on("/spray", HTTP_GET, set_spray);
    server.on("/drain", HTTP_GET, set_drain);

    // 인라인 방식입니다
    // server.on("/v", HTTP_GET, [](AsyncWebServerRequest *req) {
    //     // Serial.println("came in");
    //     // arg가 있을 때
    //     if (req->args() > 0) {
    //         if (req->arg("speed") != NULL) {
    //             PUMP_SPEED = req->arg("speed");
    //         }
    //         if (req->arg("pour") != NULL) {
    //             DURATION_POUR = req->arg("pour");
    //         }
    //         if (req->arg("idle") != NULL) {
    //             DURATION_IDLE = req->arg("idle");
    //         }
    //     }

    //     sprintf(output, "speed: %s, pour: %s, idle: %s", PUMP_SPEED.c_str(),
    //             DURATION_POUR.c_str(), DURATION_IDLE.c_str());
    //     // output = String("speed: %s, pour: %s, idle: %s")
    //     Serial.println(output);
    //     req->send_P(200, "text/plain", output);
    // });

    server.begin();

    Serial.println("HTTP server started");
    Serial.println("Default speed");

    // 각 시각에 현재시각을 기록해줍니다
    msCur = millis();
    msSprayMoment = msCur;
    msDrainMoment = msCur;

    sprintf(output, "Spray:\r\n\tspeed: %lu, pour: %lu, idle: %lu",
            SPRAY_PUMP_SPEED, SPRAY_DURATION_ON, SPRAY_DURATION_OFF);
    Serial.println(output);
    sprintf(output, "Drain:\r\n\tspeed: %lu, pour: %lu, idle: %lu",
            DRAIN_PUMP_SPEED, DRAIN_DURATION_ON, DRAIN_DURATION_OFF);
    Serial.println(output);
}

void loop(void) {
    ArduinoOTA.handle();

    msCur = millis();

    // server.handleClient();
    if (bAction) {
        // spray 펌프 관련 로직입니다

        // spray 펌프가 동작중이면
        if (bSprayStarted) {
            // sprintf(output, "speed: %lu, pour: %lu, idle: %lu",
            // SPRAY_PUMP_SPEED,
            //         SPRAY_DURATION_ON, SPRAY_DURATION_OFF);
            // Serial.println(output);
            // 동작시간이 완료되면
            if (msCur - msSprayMoment >= SPRAY_DURATION_ON * 1000) {
                // sprintf(output, "speed: %lu, pour: %lu, idle: %lu",
                //         SPRAY_PUMP_SPEED, SPRAY_DURATION_ON,
                //         SPRAY_DURATION_OFF);
                // Serial.println(output);
                // sprintf(output, "msCur: %lu, msMoment:%lu, diff: %lu", msCur,
                //         msSprayMoment, msCur - msSprayMoment);
                // Serial.println(output);
                sprintf(output, "Spray stopped!(%lusec)", SPRAY_DURATION_OFF);
                Serial.println(output);
                // bool을 전환하고 모터동작을 멈춥니다
                analogWrite(pinPump1A, 0);
                analogWrite(pinPump1B, 0);

				analogWrite(pinFan, 0);

                bSprayStarted = !bSprayStarted;

                // msMoment에 현재
                msSprayMoment = msCur;

                // led를 어둡게해줍니다
                analogWrite(LED_BUILTIN, 99);
            }
            // spray 펌프가 정지중이면
        } else if (!bSprayStarted) {
            // sprintf(output, "msCur: %lu, msMoment:%lu, diff: %lu", msCur,
            //         msSprayMoment, msCur - msSprayMoment);
            // Serial.println(output);
            // 정지 시간이 완료되면
            if (msCur - msSprayMoment >= SPRAY_DURATION_OFF * 1000) {
                // sprintf(output, "speed: %lu, pour: %lu, idle: %lu",
                //         SPRAY_PUMP_SPEED, SPRAY_DURATION_ON,
                //         SPRAY_DURATION_OFF);
                // Serial.println(output);
                sprintf(output, "Spray started...(%lu%%, %lusec)",
                        SPRAY_PUMP_SPEED, SPRAY_DURATION_ON);
                Serial.println(output);
                // bool을 전환하고 모터동작을 시작합니다
                analogWrite(pinPump1A, SPRAY_PUMP_SPEED);
                analogWrite(pinPump1B, 0);

				analogWrite(pinFan, FAN_SPEED);

                bSprayStarted = !bSprayStarted;

                // led를 밝게 해줍니다
                analogWrite(LED_BUILTIN, 92);

                // msMoment에 현재 시각을 저장해줍니다
                msSprayMoment = msCur;
            }
        }

        // drain 펌프 관련 로직입니다

        // drain 펌프가 동작중이면
        if (bDrainStarted) {
            // 동작시간이 완료되면
            if (msCur - msDrainMoment >= DRAIN_DURATION_ON * 1000) {
                sprintf(output, "Drain stopped!(%lusec)", DRAIN_DURATION_OFF);
                Serial.println(output);

                // bool을 전환하고 모터동작을 멈춥니다
                analogWrite(pinPump2A, 0);
                analogWrite(pinPump2B, 0);

                bDrainStarted = !bDrainStarted;

                // led를 어둡게 해줍니다
                analogWrite(LED_BUILTIN, 99);

                // msMoment에 현재 시각을 저장해줍니다
                msDrainMoment = msCur;
            }
            // drain 펌프가 정지중이면
        } else if (!bDrainStarted) {
            // 정지 시간이 완료되면
            if (msCur - msDrainMoment >= DRAIN_DURATION_OFF * 1000) {
                sprintf(output, "Drain started(%lu%%, %lusec)",
                        DRAIN_PUMP_SPEED, DRAIN_DURATION_ON);
                Serial.println(output);
                // bool을 전환하고 모터동작을 시작합니다
                analogWrite(pinPump2A, DRAIN_PUMP_SPEED);
                analogWrite(pinPump2B, 0);

                bDrainStarted = !bDrainStarted;

                // led를 밝게 해줍니다
                analogWrite(LED_BUILTIN, 92);

                // msMoment에 현재 시각을 저장해줍니다
                msDrainMoment = msCur;
            }
        }

        // if (curMillis - prevMillis >= SPRAY_DURATION_ON * 1000) {
        //     analogWrite(LED_BUILTIN, 91);
        //     analogWrite(pinPump1A, String(SPRAY_PUMP_SPEED).toInt());
        //     analogWrite(pinPump1B, 0);

        //     // analogWrite(pinPumpB, HIGH);
        //     // analogWrite(pinPumpB, LOW);

        //     // digitalWrite(pinPump, HIGH);
        //     // digitalWrite(pinPumpB, LOW);

        //     // delay(SPRAY_DURATION_ON.toInt() * 1000);
        //     // analogWrite(pinPump, 55);
        //     // delay(5000);
        // }
    } else {
        // bool을 전환하고 모터동작을 멈춥니다
        analogWrite(pinPump1A, 0);
        analogWrite(pinPump1B, 0);
        analogWrite(pinPump2A, 0);
        analogWrite(pinPump2B, 0);

        bSprayStarted = false;
        bDrainStarted = false;

        // led를 어둡게 해줍니다
        analogWrite(LED_BUILTIN, 99);
    }

    // analogWrite(LED_BUILTIN, 99);
    // digitalWrite(pinPump, LOW);
    // digitalWrite(pinPumpB, LOW);
    // analogWrite(pinPump1A, 0);
    // analogWrite(pinPump1B, 0);
    // delay(SPRAY_DURATION_OFF * 1000);
    // delay(SPRAY_DURATION_OFF.toInt() * 1000);
    // analogWrite(pinPump, 0);
    // delay(5000);
    /* MDNS.update(); */
}
