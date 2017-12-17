
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"


int btn_start = 4;		// 타이머 스타트 버튼
int btn_inc = 5;		// 타이머 + 버튼
int btn_dec = 6;		// 타이머 - 버튼
int pinBuzz = 9;

int init_min = 5;		// 기본 설정 분
int minute = 5;			// 분
int second = 0;			// 초

int btn_inc_down = false; 
int btn_inc_up = false; 
int btn_dec_down = false; 
int btn_dec_up = false; 

#define MODE_TIMER      1
#define MODE_SETTING    2
#define PIN_CLK         2 
#define PIN_DIO         3 


int mode = MODE_SETTING;

SevenSegmentExtended      display(PIN_CLK, PIN_DIO);





void setup()
{
    pinMode(pinBuzz, OUTPUT);
    pinMode(btn_dec, INPUT_PULLUP);
    pinMode(btn_inc, INPUT_PULLUP);
    pinMode(btn_start, INPUT_PULLUP);
    display.begin();
    display.setBacklight(100);
    display.print("   HELLO    ");

	//처음엔 5분 숫자를 표시
}


void loop()
{
    if (mode == MODE_TIMER)
    {


    }
    else if (mode == MODE_SETTING)
    {
		display.print(minute);

        if(digitalRead(btn_inc) == LOW)
        {
			if(btn_inc_down == false)
			{
				soundtick();
				increase();
			}
			
			btn_inc_down = true;
        }
		else if(digitalRead(btn_inc) == HIGH)
        {
			if(btn_inc_down == true)
			{
				btn_inc_down = false;
			}
			
        }

		if(digitalRead(btn_dec) == LOW)
        {
			if(btn_dec_down == false)
			{
				soundtick();
				decrease();
			}
			
			btn_dec_down = true;
        }
		else if(digitalRead(btn_dec) == HIGH)
        {
			if(btn_dec_down == true)
			{
				btn_dec_down = false;
			}
			
        }


		if(digitalRead(btn_start) == LOW)
        {
			display.print(" END");
			soundfinish();
			//soundstart();
			display.print("    ");
			
		//	mode = MODE_TIMER;
        }
    }


}

void increase()
{
	minute += 1;

	if(minute >= 99) minute = 99;
}

void decrease()
{
	minute -= 1;

	if(minute <= 1) minute = 1;
}



void soundtick()
{
    tone(pinBuzz, 500);
    delay(5);
    noTone(pinBuzz);
}

void soundstart()
{
	int tones[] = {261, 294, 330, 349, 392, 440, 494, 523};

    tone(pinBuzz, tones[0]);
    delay(200);
    noTone(pinBuzz);

    tone(pinBuzz, tones[1]);
    delay(200);
    noTone(pinBuzz);

    tone(pinBuzz, tones[2]);
    delay(200);
    noTone(pinBuzz);

    tone(pinBuzz, tones[3]);
    delay(200);
    noTone(pinBuzz);

    tone(pinBuzz, tones[4]);
    delay(600);
    noTone(pinBuzz);
}

void soundfinish()
{
	int tones[] = {261, 294, 330, 349, 392, 440, 494, 523};

    tone(pinBuzz, tones[4]);
    delay(1000);
    noTone(pinBuzz);
	delay(500);

    tone(pinBuzz, tones[4]);
    delay(1000);
    noTone(pinBuzz);
	delay(500);

    tone(pinBuzz, tones[4]);
    delay(1000);
    noTone(pinBuzz);
	delay(500);

    tone(pinBuzz, tones[4]);
    delay(1000);
    noTone(pinBuzz);
	delay(500);

    tone(pinBuzz, tones[7]);
    delay(3000);
    noTone(pinBuzz);
	delay(500);

    tone(pinBuzz, tones[7]);
    delay(100);
    noTone(pinBuzz);
	delay(100);

    tone(pinBuzz, tones[0]);
    delay(100);
    noTone(pinBuzz);
	delay(20);
    tone(pinBuzz, tones[0]);
    delay(100);
    noTone(pinBuzz);
	delay(20);
    tone(pinBuzz, tones[0]);
    delay(100);
    noTone(pinBuzz);
	delay(20);
}


void start_timer()
{

}
