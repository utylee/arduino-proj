
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"


int btn_start = 1;		// 타이머 스타트 버튼
int btn_inc = 12;		// 타이머 + 버튼
int btn_dec = 11;		// 타이머 - 버튼
int init_min = 5;		// 기본 설정 분
int minute = 0;			// 분
int second = 0;			// 초

#define MODE_TIMER      1
#define MODE_SETTING    2
#define PIN_CLK         2 
#define PIN_DIO         3 

int mode = MODE_SETTING;

SevenSegmentExtended      display(PIN_CLK, PIN_DIO);



int pinBuzz = 6;


void setup()
{
    pinMode(pinBuzz, OUTPUT);
    pinMode(btn_inc, INPUT_PULLUP);
    display.begin();
    display.setBacklight(30);
    display.print("  Hi");

	//처음엔 5분 숫자를 표시
}


void loop()
{
    if (mode == MODE_TIMER)
    {


    }
    else if (mode == MODE_SETTING)
    {
        if(digitalRead(btn_inc) == LOW)
        {
            //soundtick();
        }
    }


}

/*

void soundtick()
{
    tone(pinBuzz, 500);
    delay(5);
    noTone(pinBuzz);
}

void soundstart()
{
    tone(pinBuzz, 500);
    delay(5);
    noTone(pinBuzz);
}

void start_timer()
{

}
*/
