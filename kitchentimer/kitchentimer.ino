
#include "SevenSegmentTM1637.h"
#include "SevenSegmentExtended.h"
#include "toneAC.h"
//#include "toneAC2.h"
#include "Timer.h"

int clock_hour = 10;
int clock_min = 31; 
int clock_sec = 0; 
int clock_min_before = clock_min - 1; 
int print_clock = true;

int btn_start = 4;		// 타이머 스타트 버튼
int btn_inc = 5;		// 타이머 + 버튼
int btn_dec = 6;		// 타이머 - 버튼
int pinBuzz = 9;

int init_min = 5;		// 기본 설정 분
int minute = 5;			// 분
int second = minute * 60;			// 초

int cur = 0;
int before = 0;

int btn_inc_down = false; 
int btn_inc_up = false; 
int btn_dec_down = false; 
int btn_dec_up = false; 
int btn_start_down = false; 
int btn_start_up = false; 
int start_down_time = 0; 

#define MODE_TIMER      1
#define MODE_SETTING    2
#define MODE_CLOCK		3

#define PIN_CLK         2 
#define PIN_DIO         3 


int mode = MODE_CLOCK;
//int mode = MODE_SETTING;

SevenSegmentExtended      display(PIN_CLK, PIN_DIO);
Timer clock;


void setup()
{
	//delay(500);
	Serial.begin(9600);
	Serial.println("start");
    pinMode(pinBuzz, OUTPUT);
    pinMode(btn_dec, INPUT_PULLUP);
    pinMode(btn_inc, INPUT_PULLUP);
    pinMode(btn_start, INPUT_PULLUP);
    display.begin();
    display.setBacklight(100);
    display.print("   HELLO    ");

	clock.every(1000, timego);	

	//처음엔 5분 숫자를 표시
}

void timego()
{
	clock_sec += 1;

	if(clock_sec >= 60)
	{
		clock_sec = 0;
		clock_min += 1;
	}

	if(clock_min >= 60)
	{
		clock_min = 0;
		clock_hour += 1;
	}
	if(clock_hour >= 24)
	{
		clock_hour = 0;
	}

	//Serial.println(clock_hour);
	//Serial.println(clock_min);
	Serial.println(clock_sec);
}



void loop()
{
	clock.update();

	// 시계모드
	if (mode == MODE_CLOCK)
	{
		if(print_clock || clock_min != clock_min_before)
		{
			print_clock = false;
			display.printTime(clock_hour, clock_min, true);
			clock_min_before = clock_min;
		}

		// 아무버튼이나 누르면 세팅모드로 들어갑니다
		if(digitalRead(btn_start) == LOW || digitalRead(btn_inc) == LOW || digitalRead(btn_dec) == LOW)
		//if(digitalRead(btn_start) == LOW )
		{
			if(btn_start_down == false)
			{
				btn_start_down = true;
			}
		}
		else if(digitalRead(btn_start) == HIGH )
		{
			if(btn_start_down == true)
			{
				btn_start_down = false;
				display.print("    ");
				mode = MODE_SETTING;
				soundtickAC();
				//soundtick();
			}

		}
	}

	else if (mode == MODE_TIMER)
    {
		cur = millis();
		//Serial.println(cur);
		if (before == 0) before == millis();

		if (cur - before >= 1000) // 1초경과
		{
			second -= 1;
			//Serial.println(second);
			int p_min = second / 60;
			int p_sec = second - p_min * 60;

			display.printTime(p_min, p_sec, true);

			before = cur;
		}

		if (second == 0 )    // 타이머 종료
		{
			display.print(" END");
			soundfinishAC();


			display.print("    BYE    ");
			delay(1000);
			display.print("    ");
			minute = 5;
			second = minute * 60;
			delay(1000);

			//mode = MODE_SETTING;
			mode = MODE_CLOCK;
			print_clock = true;
		}

		// Cancel 취소모드 <-- 버튼을 길게 누르고 있을 시
		if (digitalRead(btn_start) == LOW)
		{
			// 처음 누르는 시점일 경우의 현재타임을 저장해둡니다
			if (btn_start_down == false)
			{
				btn_start_down = true;
				start_down_time = millis();
			}
			// 계속 눌려있는 경우 시간을 잽니다
			else if (btn_start_down == true)
			{
				cur = millis();
				// 1.5초 이상 눌려있을 경우 취소 후 세팅모드로 돌입
				if (start_down_time != 0 && cur - start_down_time >= 1500)
				{
					//Serial.println(start_down_time);
					//Serial.println(cur);
					display.print("STOP");
					soundcancelAC();
					//soundcancel();
					btn_start_down = false;
					start_down_time = 0;
					minute = 5;
					second = minute * 60;
					//mode = MODE_SETTING;
					mode = MODE_CLOCK;
					print_clock = true;
					delay(600);
					display.print("     ");
					delay(200);

					//soundtick();
					soundtickAC();
				}
			}
		}
		else if (digitalRead(btn_start) == HIGH)
		{
			if (btn_start_down == true)
			{
				start_down_time = 0;
				btn_start_down = false;
			}

		}
    }

	else if (mode == MODE_SETTING)
    {
		display.print(minute);

        if(digitalRead(btn_inc) == LOW)
        {
			if(btn_inc_down == false)
			{
				soundtickAC();
				//soundtick();
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
				soundtickAC();
				//soundtick();
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
			//display.print(" END");
			//soundfinish();
			display.print("GOGO");
			soundstartAC();
			//soundstart();
			
			mode = MODE_TIMER;
        }
    }


}

void increase()
{
	minute += 1;

	if(minute >= 99) minute = 99;
	second = minute * 60;
	
}

void decrease()
{
	minute -= 1;

	if(minute <= 1) minute = 1;
	second = minute * 60;
}



void soundtick()
{
    tone(pinBuzz, 80);
    //tone(pinBuzz, 500);
    delay(5);
    noTone(pinBuzz);
}

void soundtickAC()
{
    toneAC(500);
    delay(15);
	toneAC();
}

void soundstartAC()
{
	int tones[] = {261, 294, 330, 349, 392, 440, 494, 523};

    //tone(pinBuzz, tones[0]);
    toneAC(tones[0]);
    delay(200);
    //noTone(pinBuzz);
	toneAC();

	/*
    tone(pinBuzz, tones[1]);
    delay(200);
    noTone(pinBuzz);
	*/

    //tone(pinBuzz, tones[2]);
    toneAC(tones[2]);
    delay(200);
    //noTone(pinBuzz);
	toneAC();

	/*
    tone(pinBuzz, tones[3]);
    delay(200);
    noTone(pinBuzz);
	*/

    //tone(pinBuzz, tones[4]);
    toneAC(tones[4]);
    delay(600);
    //noTone(pinBuzz);
	toneAC();
}

void soundstart()
{
	int tones[] = {261, 294, 330, 349, 392, 440, 494, 523};

    tone(pinBuzz, tones[0]);
    delay(200);
    noTone(pinBuzz);

	/*
    tone(pinBuzz, tones[1]);
    delay(200);
    noTone(pinBuzz);
	*/

    tone(pinBuzz, tones[2]);
    delay(200);
    noTone(pinBuzz);

	/*
    tone(pinBuzz, tones[3]);
    delay(200);
    noTone(pinBuzz);
	*/

    tone(pinBuzz, tones[4]);
    delay(600);
    noTone(pinBuzz);
}

void soundfinish()
{
	int tones[] = {261, 294, 330, 349, 392, 440, 494, 523};

    tone(pinBuzz, tones[0]);
    delay(70);
    tone(pinBuzz, tones[4]);
    delay(900);
    noTone(pinBuzz);
	delay(500);

    tone(pinBuzz, tones[0]);
    delay(70);
    tone(pinBuzz, tones[4]);
    delay(900);
    noTone(pinBuzz);
	delay(500);

    tone(pinBuzz, tones[0]);
    delay(70);
    tone(pinBuzz, tones[4]);
    delay(900);
    noTone(pinBuzz);
	delay(500);

    tone(pinBuzz, tones[0]);
    delay(70);
    tone(pinBuzz, tones[4]);
    delay(900);
    noTone(pinBuzz);
	delay(500);

    tone(pinBuzz, tones[0]);
    delay(70);
    tone(pinBuzz, tones[7]);
    delay(2900);
    noTone(pinBuzz);
	delay(1000);

    tone(pinBuzz, tones[0]);
    delay(20);
    tone(pinBuzz, tones[7]);
    delay(80);
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


void soundfinishAC()
{
	int tones[] = {261, 294, 330, 349, 392, 440, 494, 523};

    toneAC(tones[0]);
    delay(70);
    toneAC(tones[4]);
    delay(900);
	toneAC();
	delay(500);

    toneAC(tones[0]);
    delay(70);
    toneAC(tones[4]);
    delay(900);
	toneAC();
	delay(500);

    toneAC(tones[0]);
    delay(70);
    toneAC(tones[4]);
    delay(900);
	toneAC();
	delay(500);

    toneAC(tones[0]);
    delay(70);
    toneAC(tones[4]);
    delay(900);
	toneAC();
	delay(500);

    toneAC(tones[0]);
    delay(70);
    toneAC(tones[7]);
    delay(2900);
	toneAC();
	delay(1000);

    toneAC(tones[0]);
    delay(20);
    toneAC(tones[7]);
    delay(80);
	toneAC();
	delay(100);

    toneAC(tones[0]);
    delay(100);
	toneAC();
	delay(20);
    toneAC(tones[0]);
    delay(100);
	toneAC();
	delay(20);
    toneAC(tones[0]);
    delay(100);
	toneAC();
	delay(20);
}
					
void soundcancel()
{
    tone(pinBuzz, 330);
    delay(50);
    noTone(pinBuzz);
    delay(20);
    tone(pinBuzz, 330);
    delay(50);
    noTone(pinBuzz);
    delay(20);
}

void soundcancelAC()
{
    toneAC(330);
    delay(50);
	toneAC();
    delay(20);
    toneAC(330);
    delay(50);
	toneAC();
    delay(20);
}

