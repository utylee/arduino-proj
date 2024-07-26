// #include <ESP8266WebServer.h>
// #include <ESP8266WiFi.h>
// #include <ESP8266mDNS.h>
#include <WiFiClient.h>
// #include <uri/UriBraces.h>

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

bool start = true;
String PUMP_SPEED = "55";    // 55% 의 전력으로 펌프를 구동합니다
String DURATION_POUR = "3";  // 3초간 분사합니다
String DURATION_IDLE = "10"; // 10초간 정지합니다

char output[512]; // Serial 출력을 위한 버퍼입니다
char stopped_warning[255];

// ESP8266WebServer server(80);
AsyncWebServer server(80);

/* const int pinLed = 13; */
/* LED_BUILTIN */

int pinPump = D1;
// int floatingValue = 0;

void set_start(AsyncWebServerRequest *req) {
    start = true;
    // // PUMP_SPEED = "55";    // 55% 의 전력으로 펌프를 구동합니다
    // DURATION_POUR = "0";  // 3초간 분사합니다
    // DURATION_IDLE = "0"; // 10초간 정지합니다
    sprintf(output, "Started...\r\nspeed: %s, pour: %s, idle: %s",
            PUMP_SPEED.c_str(), DURATION_POUR.c_str(), DURATION_IDLE.c_str());
    // output = String("speed: %s, pour: %s, idle: %s")
    Serial.println(output);
    req->send_P(200, "text/plain", output);
}

void set_stop(AsyncWebServerRequest *req) {
    start = false;
    // // PUMP_SPEED = "55";    // 55% 의 전력으로 펌프를 구동합니다
    // DURATION_POUR = "0";  // 3초간 분사합니다
    // DURATION_IDLE = "0"; // 10초간 정지합니다
    sprintf(output, "Stopped!!\r\nspeed: %s, pour: %s, idle: %s",
            PUMP_SPEED.c_str(), DURATION_POUR.c_str(), DURATION_IDLE.c_str());
    // output = String("speed: %s, pour: %s, idle: %s")
    Serial.println(output);
    req->send_P(200, "text/plain", output);
}

void set_default(AsyncWebServerRequest *req) {
    PUMP_SPEED = "55";    // 55% 의 전력으로 펌프를 구동합니다
    DURATION_POUR = "3";  // 3초간 분사합니다
    DURATION_IDLE = "10"; // 10초간 정지합니다

    if (!start) {
        sprintf(stopped_warning, "\r\nbut stopped now!");
    } else {
        sprintf(stopped_warning, "");
    }

    sprintf(output, "Set to Default...\r\nspeed: %s, pour: %s, idle: %s %s",
            PUMP_SPEED.c_str(), DURATION_POUR.c_str(), DURATION_IDLE.c_str(),
            stopped_warning);
    // output = String("speed: %s, pour: %s, idle: %s")
    Serial.println(output);
    req->send_P(200, "text/plain", output);
}

void change(AsyncWebServerRequest *req) {
    // arg가 있을 때
    if (req->args() > 0) {
        if (req->arg("speed") != NULL) {
            PUMP_SPEED = req->arg("speed");
        }
        if (req->arg("pour") != NULL) {
            DURATION_POUR = req->arg("pour");
        }
        if (req->arg("idle") != NULL) {
            DURATION_IDLE = req->arg("idle");
        }
    }

    if (!start) {
        sprintf(stopped_warning, "\r\nbut stopped now!");
    } else {
        sprintf(stopped_warning, "");
    }

    sprintf(output, "speed: %s, pour: %s, idle: %s %s", PUMP_SPEED.c_str(),
            DURATION_POUR.c_str(), DURATION_IDLE.c_str(), stopped_warning);
    // output = String("speed: %s, pour: %s, idle: %s")
    Serial.println(output);
    req->send_P(200, "text/plain", output);
}

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
    server.on("/set", HTTP_GET, change);

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
}

void loop(void) {
    // server.handleClient();
    if (start) {
        analogWrite(LED_BUILTIN, 91);
        // analogWrite(pinPump, String(PUMP_SPEED).toInt());
        analogWrite(pinPump, 55);
        delay(5000);
        // delay(DURATION_POUR.toInt() * 1000);

        analogWrite(LED_BUILTIN, 99);
        analogWrite(pinPump, 0);
        delay(5000);
        // delay(DURATION_IDLE.toInt() * 1000);
        /* MDNS.update(); */
    }
	delay(50)
}
