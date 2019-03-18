//
// Created by dzyol on 3/17/2019.
//
#include "stdio.h"
#include "string.h"
#include "avr.h"
#include "lcd.h"
#include <math.h>

//original 100000/frequency/2 => period 
#define A_NOTE 113
#define A_SHARP_NOTE 108
#define B_NOTE 101
#define C_NOTE 96
#define C_SHARP_NOTE 90
#define D_NOTE 85
#define D_SHARP_NOTE 81
#define E_NOTE 76
#define F_NOTE 72
#define F_SHARP_NOTE 68
#define G_NOTE 64
#define G_SHARP_NOTE 60

int tempo = 10;
int volume = 2;
int play = 0;

int birthday[] = {G_NOTE, G_NOTE, A_NOTE, G_NOTE, C_NOTE, B_NOTE, G_NOTE, G_NOTE, A_NOTE, G_NOTE, D_NOTE, C_NOTE, G_NOTE,G_NOTE,G_NOTE,E_NOTE,C_NOTE,B_NOTE,A_NOTE,F_NOTE,F_NOTE,E_NOTE,C_NOTE, D_NOTE, C_NOTE};
	
//Clock Variables
int is_military = 0;
char twelve_hour[10] = "AM";
int year = 2019;
int month = 3;
int day = 17;
int hour = 14;
int min = 59;
int sec = 0;
int month_days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//timer variables
int timer_start = 0;
int timer_year = 1;
int timer_month = 0;
int timer_day = 0;
int timer_hour = 0;
int timer_min = 0;
int timer_sec = 5;
int timer_month_days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//Alarm 1 variables
int alarm_1_year = 2019;
int alarm_1_month = 3;
int alarm_1_day = 17;
int alarm_1_hour = 12;
int alarm_1_min = 0;
int alarm_1_sec = 0;
int alarm_1_month_days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//Alarm 2 variables
int alarm_2_year = 2019;
int alarm_2_month = 2;
int alarm_2_day = 28;
int alarm_2_hour = 23;
int alarm_2_min = 59;
int alarm_2_sec = 55;
int alarm_2_month_days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//World Clock variables
int time_zone = -7;

unsigned int is_pressed(int r, int c){
    //Set to default
    DDRC = 0;

    r += 4;

    //Set c strong 0
    SET_BIT(DDRC, c);
    CLR_BIT(PORTC, c);

    //Set r weak 1
    CLR_BIT(DDRC, r);
    SET_BIT(PORTC, r);

	wait_avr(1);

    if(GET_BIT(PINC,r)){
	    return 0;
    }

    return 1;
}

unsigned int get_key(){
	
    int r, c;
    for (r = 0; r < 4; ++r){
        for (c = 0; c < 4; ++c){
            if (is_pressed(r, c))
                return r * 4 + c + 1;
        }
    }
	
    return 0;
}

char num_to_value(int num){
	if (num == 1)
	{
		return '1';
	}
	else if (num == 5)
	{
		return '2';
	}
	else if (num == 9)
	{
		return '3';
	}
	else if (num == 2)
	{
		return '4';
	}
	else if (num == 6)
	{
		return '5';
	}
	else if (num == 10)
	{
		return '6';
	}
	else if (num == 3)
	{
		return '7';
	}
	else if (num == 7)
	{
		return '8';
	}
	else if (num == 11)
	{
		return '9';
	}
	else if (num == 10)
	{
		return '6';
	}
	else if (num == 4)
	{
		return '*';
	}
	else if (num == 8)
	{
		return '0';
	}
	else if (num == 12)
	{
		return '#';
	}
	else if (num == 10)
	{
		return '6';
	}
	else if (num == 13)
	{
		return 'A';
	}
	else if (num == 14)
	{
		return 'B';
	}
	else if (num == 15)
	{
		return 'C';
	}
	else if (num == 16)
	{
		return 'D';
	}
	return 0;
}

void wait_avr_2(unsigned short usec){
	TCCR0 = 2;
	while (usec--) {
		TCNT0 = (unsigned char)(256 - (XTAL_FRQ / 64) * 0.0001);
		SET_BIT(TIFR, TOV0);
		WDR();
		while (!GET_BIT(TIFR, TOV0));
	}
	TCCR0 = 0;
}

void play_note(unsigned int frequency,unsigned int duration){
	int i;
	//Since duration isn't long in default(too fast), so I time it by 20000 to make it slower and divide it by note period for correct note cycle.
	//also divide the tempo(range 1.1 - 3) to make the cycle smaller, so it plays faster. 
	int cycle = duration*20000/frequency/((float)(tempo+10)/10);

	//volume setting
	//th times a fraction of volume so make the sound louder or less louder.
	unsigned int th = frequency*(volume/100.0);
	unsigned int tl = frequency-th;
	for(i = 0; i < cycle; i++){
		SET_BIT(PORTB,3);
		wait_avr_2(th);
		CLR_BIT(PORTB,3);
		wait_avr_2(tl);
	}
	
}


void play_birthday(){
    clr_lcd();
    char tempobuffer[20];
    char volumebuffer[20];
	for (int i = 0; i < (int)( sizeof(birthday) / sizeof(birthday[0])); i++)
	{
		int num = get_key();
		sprintf(tempobuffer, "Tempo: %02i", tempo);
		pos_lcd(0, 0);
		puts_lcd2(tempobuffer);
		
		sprintf(volumebuffer, "Volume: %02i", volume);
		pos_lcd(1, 0);
		puts_lcd2(volumebuffer);
		if (num_to_value(num) == 'A')
		{
			if (tempo >= 1 && tempo < 20)
			{
				tempo++;

			}
		}
		else if (num_to_value(num) == 'B')
		{
			if (tempo > 1 && tempo <= 20)
			{
				tempo--;
			}
		}
		else if (num_to_value(num) == 'C')
		{
			if (volume >= 1 && volume < 10)
			{
				volume++;
			}
		}
		else if (num_to_value(num) == 'D')
		{
			if (volume > 1 && volume <= 10)
			{
				volume--;
			}
		}
		else if (num_to_value(num) == '*')
		{
			play = 0;
			break;
		}
		play_note(birthday[i], 1);
		wait_avr(50);
	}
}

int get_sample_1()
{
	ADMUX |= (1<<REFS0);
	CLR_BIT(ADMUX, 0);
	CLR_BIT(ADMUX, 1);
	CLR_BIT(ADMUX, 2);
	CLR_BIT(ADMUX, 3);
	CLR_BIT(ADMUX, 4);

	SET_BIT(ADCSRA, 0);
	SET_BIT(ADCSRA, 1);
	SET_BIT(ADCSRA, 2);

	SET_BIT(ADCSRA, 7); //enable
	SET_BIT(ADCSRA, 6); //start conversion
	while(GET_BIT(ADCSRA, 6)); //wait to finish
	wait_avr(15);
	return ADC;
}

int get_sample_2()
{
	ADMUX |= (1<<REFS0);
	CLR_BIT(ADMUX, 0);
	CLR_BIT(ADMUX, 1);
	CLR_BIT(ADMUX, 2);
	CLR_BIT(ADMUX, 3);
	CLR_BIT(ADMUX, 4);

	SET_BIT(ADMUX, 0);

	SET_BIT(ADCSRA, 0);
	SET_BIT(ADCSRA, 1);
	SET_BIT(ADCSRA, 2);
	
	SET_BIT(ADCSRA, 7); //enable
	SET_BIT(ADCSRA, 6); //start conversion
	while(GET_BIT(ADCSRA, 6)); //wait to finish
	wait_avr(15);
	return ADC;
}

char is_leap(){
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)){
		return 1;
	}
	else{
		return 0;
	}
	
}

char timer_is_leap(){
	if ((timer_year % 4 == 0 && timer_year % 100 != 0) || (timer_year % 400 == 0)){
		return 1;
	}
	else{
		return 0;
	}
	
}

void edit_day(){
	int n = 0;
	
	if (is_leap())
	{
		month_days[2] = 29;
	}
	else
	{
		month_days[2] = 28;
	}
	while(n < 3){
		int num = get_key();
		wait_avr(50);
		
		char buffer[17];
		
		sprintf(buffer, "%02i/%02i/%02i", month, day, year);
		pos_lcd(0, 0);
		puts_lcd2(buffer);
		wait_avr(100);
		if (n == 0)
		{
			sprintf(buffer, "  /%02i/%02i", day, year);
			pos_lcd(0, 0);
			puts_lcd2(buffer);
			wait_avr(100);

			if (num_to_value(num) == 'A' && month+1 <= 12)
			{
				month++;
			}
			else if (num_to_value(num) == 'B' && month-1 > 0)
			{
				month--;
			}
		}
		else if (n == 1)
		{
			sprintf(buffer, "%02i/  /%02i", month, year);
			pos_lcd(0, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && day+1 <= month_days[month])
			{
				day++;
			}
			else if (num_to_value(num) == 'B' && day-1 > 0)
			{
				day--;
			}
		}
		else if (n == 2)
		{
			sprintf(buffer, "%02i/%02i/    ", month, day);
			pos_lcd(0, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A')
			{
				year++;
			}
			else if (num_to_value(num) == 'B' && year-1 > 0)
			{
				year--;
			}
		}
		if (num_to_value(num) == '#')
		{
			n++;
		}
	}
	
	
	
}

void edit_time(){
	int n = 0;
	int item = 0;
	if (is_military)
	{
		item = 3;
	}
	else if (!is_military)
	{
		item = 4;
	}
	
	while(n < item){
		int num = get_key();
		wait_avr(50);
		
		char buffer[20];
		
		if (is_military)
		{
			sprintf(buffer, "%02i:%02i:%02i   ", hour, min, sec);
		}
		if (!is_military)
		{
			if (hour > 12)
			{
				sprintf(buffer, "%02i:%02i:%02i %s", hour-12, min, sec, twelve_hour);
			}
			else if (hour <= 12)
			{
				sprintf(buffer, "%02i:%02i:%02i %s", hour, min, sec, twelve_hour);
			}
		}
		pos_lcd(1, 0);
		puts_lcd2(buffer);
		wait_avr(100);
		if (n == 0)
		{
			sprintf(buffer, "  :%02i:%02i   ", min, sec);
			if (!is_military)
			{
				sprintf(buffer, "  :%02i:%02i %s", min, sec, twelve_hour);
			}
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);

			if (is_military)
			{
				if (num_to_value(num) == 'A' && hour+1 < 24)
				{
					hour++;
				}
				else if (num_to_value(num) == 'B' && hour-1 > 0)
				{
					hour--;
				}
			}
			else if (!is_military)
			{
				if (num_to_value(num) == 'A' && hour+1 < 12)
				{
					hour++;
				}
				else if (num_to_value(num) == 'B' && hour-1 > 0)
				{
					hour--;
				}
			}

		}
		else if (n == 1)
		{
			sprintf(buffer, "%02i:  :%02i ", hour, sec);
			if (!is_military)
			{
				if (hour >= 12)
				{
					sprintf(buffer, "%02i:  :%02i %s", hour-12, sec, twelve_hour);
				}
				else if (hour < 12)
				{
					sprintf(buffer, "%02i:  :%02i %s", hour, sec, twelve_hour);
				}
			}
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && min+1 < 60)
			{
				min++;
			}
			else if (num_to_value(num) == 'B' && min-1 > 0)
			{
				min--;
			}
		}
		else if (n == 2)
		{
			sprintf(buffer, "%02i:%02i:     ", hour, min);
			if (!is_military)
			{
				if (hour > 12)
				{
					sprintf(buffer, "%02i:%02i:   %s", hour-12, min, twelve_hour);
				}
				else if (hour <= 12)
				{
					sprintf(buffer, "%02i:%02i:   %s", hour, min, twelve_hour);
				}
			}
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && sec+1 < 60)
			{
				sec++;
			}
			else if (num_to_value(num) == 'B' && sec-1 > 0)
			{
				sec--;
			}
		}
		else if (n == 3)
		{
			if (!is_military)
			{
				if (hour > 12)
				{
					sprintf(buffer, "%02i:%02i:%02i   ", hour-12, min, sec, twelve_hour);
				}
				else if (hour <= 12)
				{
					sprintf(buffer, "%02i:%02i:%02i   ", hour, min, sec, twelve_hour);
				}
			}
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && hour+12 < 24)
			{
				strcpy(twelve_hour, "PM");
				hour +=12;
			}
			else if (num_to_value(num) == 'B' && hour-12 >= 0)
			{
				strcpy(twelve_hour, "AM");
				hour -=12;

			}

		}
		if (num_to_value(num) == '#')
		{
			n++;
		}
	}
	
	
	
}

void ticking(){
	if (is_leap())
	{
		month_days[2] = 29;
	}
	else
	month_days[2] = 28;
	sec++;
	if (sec == 60)
	{
		sec = 0;
		min++;
	}
	if (min == 60)
	{
		min = 0;
		hour++;
	}
	if (hour == 24)
	{
		hour = 0;
		day++;
	}
	if (day > month_days[month])
	{
		day = 1;
		month++;
	}
	if (month > 12)
	{
		month = 1;
		year++;
	}
}

void edit_timer_day(){
	int n = 0;
	char buffer[17];
	if (is_leap())
	{
		timer_month_days[2] = 29;
	}
	else
	{
		timer_month_days[2] = 28;
	}
	while(n < 3){
		int num = get_key();
		wait_avr(50);
		
		char buffer[17];
		
		sprintf(buffer, "%02i/%02i/%02i     ", timer_month, timer_day, timer_year);
		pos_lcd(0, 0);
		puts_lcd2(buffer);
		wait_avr(100);
		if (n == 0)
		{
			sprintf(buffer, "  /%02i/%02i     ", timer_day, timer_year);
			pos_lcd(0, 0);
			puts_lcd2(buffer);
			wait_avr(100);

			if (num_to_value(num) == 'A' && timer_month+1 <= 12)
			{
				timer_month++;
			}
			else if (num_to_value(num) == 'B' && timer_month-1 > 0)
			{
				timer_month--;
			}
		}
		else if (n == 1)
		{
			sprintf(buffer, "%02i/  /%02i     ", timer_month, timer_year);
			pos_lcd(0, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && timer_day+1 <= timer_month_days[month])
			{
				timer_day++;
			}
			else if (num_to_value(num) == 'B' && timer_day-1 > 0)
			{
				timer_day--;
			}
		}
		else if (n == 2)
		{
			sprintf(buffer, "%02i/%02i/      ", timer_month, timer_day);
			pos_lcd(0, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A')
			{
				timer_year++;
			}
			else if (num_to_value(num) == 'B' && timer_year-1 > 0)
			{
				timer_year--;
			}
		}
		if (num_to_value(num) == '#')
		{
			n++;
		}
		wait_avr(749);
		if (!check_timer() && timer_start == 1)
		{
			timer();

		}
		ticking();
	}
	
}

void edit_timer_time(){
	int n = 0;
	int item = 0;

	item = 3;
	

	
	while(n < item){
		int num = get_key();
		wait_avr(50);
		
		char buffer[20];
		
		if (is_military)
		{
			sprintf(buffer, "%02i:%02i:%02i          ", timer_hour, timer_min, timer_sec);
		}
		pos_lcd(1, 0);
		puts_lcd2(buffer);
		wait_avr(100);
		if (n == 0)
		{
			sprintf(buffer, "  :%02i:%02i           ", timer_min, timer_sec);
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);

			if (is_military)
			{
				if (num_to_value(num) == 'A' && timer_hour+1 < 24)
				{
					timer_hour++;
				}
				else if (num_to_value(num) == 'B' && timer_hour-1 > 0)
				{
					timer_hour--;
				}
			}
		}
		else if (n == 1)
		{
			sprintf(buffer, "%02i:  :%02i        ", timer_hour, timer_sec);
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && timer_min+1 < 60)
			{
				timer_min++;
			}
			else if (num_to_value(num) == 'B' && timer_min-1 > 0)
			{
				timer_min--;
			}
		}
		else if (n == 2)
		{
			sprintf(buffer, "%02i:%02i:            ", timer_hour, timer_min);
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && timer_sec+1 < 60)
			{
				timer_sec++;
			}
			else if (num_to_value(num) == 'B' && timer_sec-1 > 0)
			{
				timer_sec--;
			}
		}
		if (num_to_value(num) == '#')
		{
			n++;
		}
		wait_avr(749);
		if (!check_timer() && timer_start == 1)
		{
			timer();

		}
		ticking();
	}
	
	
	
}

void timer()
{
	if (timer_is_leap())
	{
		timer_month_days[2] = 29;
	}
	else
		timer_month_days[2] = 28;
	
	timer_sec--;
	if (timer_sec < 0)
	{
		if (timer_min != 0 || timer_hour != 0 || timer_day != 0 || timer_month != 0 || timer_year != 0)
		{
			timer_sec = 59;
			timer_min--;
		}
		else
		{
			timer_sec = 0;
		}

	}
	if (timer_min < 0)
	{
		if (timer_hour != 0 || timer_day != 0 || timer_month != 0 || timer_year != 0)
		{
			timer_min = 59;
			timer_hour--;
		}
		else
		{
			timer_hour = 0;
		}

	}
	if (timer_hour < 0)
	{
		if (timer_day != 0 || timer_month != 0 || timer_year != 0)
		{
			timer_hour = 23;
			timer_day--;
		}
		else
		{
			timer_day = 0;
		}
	}
	if (timer_day < 0)
	{
		if(timer_month != 0 || timer_year != 0)
		{
			timer_day = month_days[timer_month];
			timer_month--;
		}
		else
		{
			timer_month = 0;
		}
	}
	if (timer_month < 0)
	{
		if (timer_year != 0)
		{
			timer_month = 12-1;
			timer_day = month_days[timer_month+1];
			timer_year--;
		}
		else
		{
			timer_year = 0;
		}

	}
			
}

int check_timer()
{
	if (timer_sec == 0 && timer_min == 0 && timer_hour == 0 && timer_day == 0 && timer_month == 0 && timer_year == 0)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void edit_alarm_1_day(){
	int n = 0;
	
	if (is_leap())
	{
		alarm_1_month_days[2] = 29;
	}
	else
	{
		alarm_1_month_days[2] = 28;
	}
	while(n < 3){
		int num = get_key();
		wait_avr(50);
		
		char buffer[17];
		
		sprintf(buffer, "%02i/%02i/%02i       ", alarm_1_month, alarm_1_day, alarm_1_year);
		pos_lcd(0, 0);
		puts_lcd2(buffer);
		wait_avr(100);
		if (n == 0)
		{
			sprintf(buffer, "  /%02i/%02i       ", alarm_1_day, alarm_1_year);
			pos_lcd(0, 0);
			puts_lcd2(buffer);
			wait_avr(100);

			if (num_to_value(num) == 'A' && alarm_1_month+1 <= 12)
			{
				alarm_1_month++;
			}
			else if (num_to_value(num) == 'B' && alarm_1_month-1 > 0)
			{
				alarm_1_month--;
			}
		}
		else if (n == 1)
		{
			sprintf(buffer, "%02i/  /%02i       ", alarm_1_month, timer_year);
			pos_lcd(0, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && alarm_1_day+1 <= alarm_1_month_days[month])
			{
				alarm_1_day++;
			}
			else if (num_to_value(num) == 'B' && alarm_1_day-1 > 0)
			{
				alarm_1_day--;
			}
		}
		else if (n == 2)
		{
			sprintf(buffer, "%02i/%02i/         ", alarm_1_month, alarm_1_day);
			pos_lcd(0, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A')
			{
				alarm_1_year++;
			}
			else if (num_to_value(num) == 'B' && alarm_1_year-1 > 0)
			{
				alarm_1_year--;
			}
		}
		if (num_to_value(num) == '#')
		{
			n++;
		}
		wait_avr(749);
		if (!check_timer() && timer_start == 1)
		{
			timer();

		}
		ticking();
	}
	
	
	
}

void edit_alarm_1_time(){
	int n = 0;
	int item = 0;

	item = 3;
	

	
	while(n < item){
		int num = get_key();
		wait_avr(50);
		
		char buffer[20];
		
		sprintf(buffer, "%02i:%02i:%02i   ", alarm_1_hour, alarm_1_min, alarm_1_sec);
		pos_lcd(1, 0);
		puts_lcd2(buffer);
		wait_avr(100);
		if (n == 0)
		{
			sprintf(buffer, "  :%02i:%02i   ", alarm_1_min, alarm_1_sec);
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);


			if (num_to_value(num) == 'A' && alarm_1_hour+1 < 24)
			{
				alarm_1_hour++;
			}
			else if (num_to_value(num) == 'B' && alarm_1_hour-1 > 0)
			{
				alarm_1_hour--;
			}
			
		}
		else if (n == 1)
		{
			sprintf(buffer, "%02i:  :%02i ", alarm_1_hour, alarm_1_sec);
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && alarm_1_min+1 < 60)
			{
				alarm_1_min++;
			}
			else if (num_to_value(num) == 'B' && alarm_1_min-1 > 0)
			{
				alarm_1_min--;
			}
		}
		else if (n == 2)
		{
			sprintf(buffer, "%02i:%02i:     ", alarm_1_hour, alarm_1_min);
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && alarm_1_sec+1 < 60)
			{
				alarm_1_sec++;
			}
			else if (num_to_value(num) == 'B' && alarm_1_sec-1 > 0)
			{
				alarm_1_sec--;
			}
		}
		if (num_to_value(num) == '#')
		{
			n++;
		}
		wait_avr(749);
		if (!check_timer() && timer_start == 1)
		{
			timer();

		}
		ticking();
	}
	
	
	
}

void edit_alarm_2_day(){
	int n = 0;
	
	if (is_leap())
	{
		alarm_2_month_days[2] = 29;
	}
	else
	{
		alarm_2_month_days[2] = 28;
	}
	while(n < 3){
		int num = get_key();
		wait_avr(50);
		
		char buffer[17];
		
		sprintf(buffer, "%02i/%02i/%02i       ", alarm_2_month, alarm_2_day, alarm_2_year);
		pos_lcd(0, 0);
		puts_lcd2(buffer);
		wait_avr(100);
		if (n == 0)
		{
			sprintf(buffer, "  /%02i/%02i       ", alarm_2_day, alarm_2_year);
			pos_lcd(0, 0);
			puts_lcd2(buffer);
			wait_avr(100);

			if (num_to_value(num) == 'A' && alarm_2_month+1 <= 12)
			{
				alarm_2_month++;
			}
			else if (num_to_value(num) == 'B' && alarm_2_month-1 > 0)
			{
				alarm_2_month--;
			}
		}
		else if (n == 1)
		{
			sprintf(buffer, "%02i/  /%02i       ", alarm_2_month, timer_year);
			pos_lcd(0, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && alarm_2_day+1 <= alarm_2_month_days[month])
			{
				alarm_2_day++;
			}
			else if (num_to_value(num) == 'B' && alarm_2_day-1 > 0)
			{
				alarm_2_day--;
			}
		}
		else if (n == 2)
		{
			sprintf(buffer, "%02i/%02i/         ", alarm_2_month, alarm_2_day);
			pos_lcd(0, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A')
			{
				alarm_2_year++;
			}
			else if (num_to_value(num) == 'B' && alarm_2_year-1 > 0)
			{
				alarm_2_year--;
			}
		}
		if (num_to_value(num) == '#')
		{
			n++;
		}
		wait_avr(749);
		if (!check_timer() && timer_start == 1)
		{
			timer();

		}
		ticking();
	}
	
	
	
}

void edit_alarm_2_time(){
	int n = 0;
	int item = 0;

	item = 3;
	

	
	while(n < item){
		int num = get_key();
		wait_avr(50);
		
		char buffer[20];
		
		sprintf(buffer, "%02i:%02i:%02i   ", alarm_2_hour, alarm_2_min, alarm_2_sec);
		pos_lcd(1, 0);
		puts_lcd2(buffer);
		wait_avr(100);
		if (n == 0)
		{
			sprintf(buffer, "  :%02i:%02i   ", alarm_2_min, alarm_2_sec);
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);


			if (num_to_value(num) == 'A' && alarm_2_hour+1 < 24)
			{
				alarm_2_hour++;
			}
			else if (num_to_value(num) == 'B' && alarm_2_hour-1 > 0)
			{
				alarm_2_hour--;
			}
			
		}
		else if (n == 1)
		{
			sprintf(buffer, "%02i:  :%02i ", alarm_2_hour, alarm_2_sec);
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && alarm_2_min+1 < 60)
			{
				alarm_2_min++;
			}
			else if (num_to_value(num) == 'B' && alarm_2_min-1 > 0)
			{
				alarm_2_min--;
			}
		}
		else if (n == 2)
		{
			sprintf(buffer, "%02i:%02i:     ", alarm_2_hour, alarm_2_min);
			pos_lcd(1, 0);
			puts_lcd2(buffer);
			wait_avr(100);
			if (num_to_value(num) == 'A' && alarm_2_sec+1 < 60)
			{
				alarm_2_sec++;
			}
			else if (num_to_value(num) == 'B' && alarm_2_sec-1 > 0)
			{
				alarm_2_sec--;
			}
		}
		if (num_to_value(num) == '#')
		{
			n++;
		}
		wait_avr(749);
		if (!check_timer() && timer_start == 1)
		{
			timer();

		}
		ticking();
	}
	
	
	
}

int check_alarm()
{
	if (alarm_1_sec == sec && alarm_1_min == min && alarm_1_hour == hour && alarm_1_day == day && alarm_1_month == month && alarm_1_year == year)
	{
		return 1;
	}
	else if (alarm_2_sec == sec && alarm_2_min == min && alarm_2_hour == hour && alarm_2_day == day && alarm_2_month == month && alarm_2_year == year)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void world_time(int h)
{
	if (is_leap())
	{
		month_days[2] = 29;
	}
	else
		month_days[2] = 28;
	if (h > 0)
	{
		hour++;
		if (sec == 60)
		{
			sec = 0;
			min++;
		}
		if (min == 60)
		{
			min = 0;
			hour++;
		}
		if (hour == 24)
		{
			hour = 0;
			day++;
		}
		if (day > month_days[month])
		{
			day = 1;
			month++;
		}
		if (month > 12)
		{
			month = 1;
			year++;
		}
		
	}
	else if (h <0)
	{
		hour--;
		if (sec < 0)
		{
			if (min != 0 || hour != 0 || day != 0 || month != 0 || year != 0)
			{
				sec = 59;
				min--;
			}
			else
			{
				sec = 0;
			}

		}
		if (min < 0)
		{
			if (hour != 0 || day != 0 || month != 0 || year != 0)
			{
				min = 59;
				hour--;
			}
			else
			{
				hour = 0;
			}

		}
		if (hour < 0)
		{
			if (day != 0 || month != 0 || year != 0)
			{
				hour = 23;
				day--;
			}
			else
			{
				day = 0;
			}
		}
		if (day < 1)
		{
			if(month != 0 || year != 0)
			{
				day = month_days[month-1];
				month--;
			}
			else
			{
				month = 0;
			}
		}
		if (month < 1)
		{
			if (year != 0)
			{
				month = 12;
				day = month_days[month];
				year--;
			}
			else
			{
				year = 0;
			}

		}
		

	}
	time_zone += h;
}
int main(){
    ini_avr();
    ini_lcd();
    clr_lcd();
	SET_BIT(DDRB,3);
	SET_BIT(DDRB,4);

	float curr = 0.0;
	int mode = 0;
	int clock_mode = 0;
	int alarm = 0;
	int alarm_count = 0;
	int page = 0;
    char buffer1[20];
    char buffer2[20];
    char daybuffer[20];
    char timebuffer[20];
	

	if (hour > 12)
	{
		strcpy(twelve_hour, "PM");
	}
	if (hour <= 12)
	{
		strcpy(twelve_hour, "AM");
	}
	
    while(1){
		int num = get_key();
		wait_avr(50);

		curr = (1023 / (float)get_sample_1()) - 1;
		curr = 10000 / curr;
		curr = curr / 10000;
		curr = log(curr);
		curr = curr / 3950;
		curr += 1.0 / (25 + 273.15);
		curr = 1 / curr;
		curr = curr - 273.15;
		
		if (curr > 31.0)
		{
			SET_BIT(PORTB,4);
		}
		else
		{
			CLR_BIT(PORTB,4);
		}
		if (check_alarm())
		{
			alarm_count = 10;
		}
		
		if (check_timer() && timer_start == 1)
		{
			timer_start = 0;
			alarm_count = 10;
		}
		
		if (alarm_count > 0)
		{
			play_note(A_NOTE, 1);
			alarm_count--;
		}
		//page 0: main menu
		//page 1: clock 1 & 2
		//page 2: clock 3 & 4
		//page 3: clock alarm 1 & 2
		//page 4: clock alarm 1
		//page 5: clock alarm 2
		//page 6: world clock
		if (page == 0)
		{
			if (num_to_value(num) == '1')
			{
				mode = 1;
				page = 1;
			}
			else if (num_to_value(num) == '2')
			{
				mode = 2;
			}
		}
		else if (page == 1)
		{
			if (num_to_value(num) == 'D')
			{
				page = 2;
			}
			else if (num_to_value(num) == '1')
			{
				clock_mode = 1;
			}
			else if (num_to_value(num) == '2')
			{
				clock_mode = 2;
			}
		}
		else if (page == 2)
		{
			if (num_to_value(num) == 'C')
			{
				page = 1;
			}
			else if (num_to_value(num) == '3')
			{
				clock_mode = 3;
				page = 3;
			}
			else if (num_to_value(num) == '4')
			{
				clock_mode = 4;
				page = 6;
			}
		}
		else if (page == 3)
		{
			if (num_to_value(num) == '1')
			{
				alarm = 1;
				page = 4;
			}
			else if (num_to_value(num) == '2')
			{
				alarm = 2;
				page = 5;
			}
		}

		
		//return to menu
		if (num_to_value(num) == '0')
		{
			mode = 0;
			clock_mode = 0;
			page = 0;
			
		}
		
		
		if (mode == 0)
		{
			sprintf(buffer1, "1. Time        ");
			pos_lcd(0, 0);
			puts_lcd2(buffer1);
			
			sprintf(buffer2, "2. Temperature        ", curr);
			pos_lcd(1, 0);
			puts_lcd2(buffer2);
		}
		else if (mode == 1)
		{
			//clock menu
			if (clock_mode == 0 && page == 1)
			{
				sprintf(buffer1, "1. Current Time       ");
				pos_lcd(0, 0);
				puts_lcd2(buffer1);
				
				sprintf(buffer2, "2. Timer        ");
				pos_lcd(1, 0);
				puts_lcd2(buffer2);
			}
			else if (clock_mode == 0 && page == 2)
			{
				sprintf(buffer1, "3. Alarm        ");
				pos_lcd(0, 0);
				puts_lcd2(buffer1);
								
				sprintf(buffer2, "4. World Clock        ");
				pos_lcd(1, 0);
				puts_lcd2(buffer2);
			}
			else if (clock_mode == 3 && page == 3)
			{
				sprintf(buffer1, "1. Alarm 1        ");
				pos_lcd(0, 0);
				puts_lcd2(buffer1);
				
				sprintf(buffer2, "2. Alarm 2        ");
				pos_lcd(1, 0);
				puts_lcd2(buffer2);
			}
			//show time
			else if (clock_mode == 1)
			{
				if (num_to_value(num) == 'C')
				{
					edit_day();
				}
				
				else if (num_to_value(num) == 'D')
				{
					edit_time();
				}
				
				else if (num_to_value(num) == '*')
				{
					is_military = 0;
				}
				else if (num_to_value(num) == '#')
				{
					is_military = 1;
				}
				
				
				sprintf(daybuffer, "%02i/%02i/%02iUTC%02i      ", month, day, year, time_zone);
				pos_lcd(0, 0);
				puts_lcd2(daybuffer);
				
				if (is_military)
				{
					sprintf(timebuffer, "%02i:%02i:%02i          ", hour, min, sec);
					pos_lcd(1, 0);
					puts_lcd2(timebuffer);
				}
				else
				{
					if (hour > 12 && hour < 24)
					{
						sprintf(timebuffer, "%02i:%02i:%02i PM      ", hour - 12, min, sec);
						pos_lcd(1, 0);
						puts_lcd2(timebuffer);
					}
					else if (hour <= 12 && hour > 0)
					{
						sprintf(timebuffer, "%02i:%02i:%02i AM      ", hour, min, sec);
						pos_lcd(1, 0);
						puts_lcd2(timebuffer);
					}
					else if (hour == 0)
					{
						sprintf(timebuffer, "%02i:%02i:%02i AM      ", hour + 12, min, sec);
						pos_lcd(1, 0);
						puts_lcd2(timebuffer);
					}

				}
			}
			//show timer
			else if (clock_mode == 2)
			{
				if (num_to_value(num) == 'C')
				{
					edit_timer_day();
				}
				
				else if (num_to_value(num) == 'D')
				{
					edit_timer_time();
				}
			
				else if (num_to_value(num) == 'A')
				{
					timer_start = 1;
				}
				else if (num_to_value(num) == 'B')
				{
					timer_start = 0;
				}
				sprintf(daybuffer, "%02i/%02i/%02i        ", timer_month, timer_day, timer_year);
				pos_lcd(0, 0);
				puts_lcd2(daybuffer);
				
				sprintf(timebuffer, "%02i:%02i:%02i        ", timer_hour, timer_min, timer_sec);
				pos_lcd(1, 0);
				puts_lcd2(timebuffer);
				
			}
			//show alarm 1
			else if (clock_mode == 3 && alarm == 1)
			{
				if (num_to_value(num) == 'C')
				{
					edit_alarm_1_day();
				}
				
				if (num_to_value(num) == 'D')
				{
					edit_alarm_1_time();
				}
				
				
				sprintf(daybuffer, "%02i/%02i/%02i        ", alarm_1_month, alarm_1_day, alarm_1_year);
				pos_lcd(0, 0);
				puts_lcd2(daybuffer);
				

				sprintf(timebuffer, "%02i:%02i:%02i        ", alarm_1_hour, alarm_1_min, alarm_1_sec);
				pos_lcd(1, 0);
				puts_lcd2(timebuffer);
				
			}
			else if (clock_mode == 3 && alarm == 2)
			{
				if (num_to_value(num) == 'C')
				{
					edit_alarm_2_day();
				}
				
				if (num_to_value(num) == 'D')
				{
					edit_alarm_2_time();
				}
				
				
				sprintf(daybuffer, "%02i/%02i/%02i        ", alarm_2_month, alarm_2_day, alarm_2_year);
				pos_lcd(0, 0);
				puts_lcd2(daybuffer);
				

				sprintf(timebuffer, "%02i:%02i:%02i        ", alarm_2_hour, alarm_2_min, alarm_2_sec);
				pos_lcd(1, 0);
				puts_lcd2(timebuffer);
				
			}
			else if (clock_mode == 4)
			{
				if (num_to_value(num) == 'C')
				{
					if (time_zone + 1 <= 12)
					{
						world_time(1);
					}
				}
				
				else if (num_to_value(num) == 'D')
				{
					if (time_zone - 1 >= -11)
					{
						world_time(-1);
					}
				}
				sprintf(daybuffer, "%02i/%02i/%02iUTC%02i      ", month, day, year, time_zone);
				pos_lcd(0, 0);
				puts_lcd2(daybuffer);
				
				if (is_military)
				{
					sprintf(timebuffer, "%02i:%02i:%02i          ", hour, min, sec);
					pos_lcd(1, 0);
					puts_lcd2(timebuffer);
				}
				else
				{
					if (hour > 12 && hour < 24)
					{
						sprintf(timebuffer, "%02i:%02i:%02i PM      ", hour - 12, min, sec);
						pos_lcd(1, 0);
						puts_lcd2(timebuffer);
					}
					else if (hour <= 12 && hour > 0)
					{
						sprintf(timebuffer, "%02i:%02i:%02i AM      ", hour, min, sec);
						pos_lcd(1, 0);
						puts_lcd2(timebuffer);
					}
					else if (hour == 0)
					{
						sprintf(timebuffer, "%02i:%02i:%02i AM      ", hour + 12, min, sec);
						pos_lcd(1, 0);
						puts_lcd2(timebuffer);
					}

				}
			}

		}
		else if (mode == 2)
		{
			sprintf(buffer1, "Current Temp:       ");
			pos_lcd(0, 0);
			puts_lcd2(buffer1);
			
			sprintf(buffer2, "%.2fC          ", curr);
			pos_lcd(1, 0);
			puts_lcd2(buffer2);
			//wait_avr(500);
		}

		


		wait_avr(949);
		if (!check_timer() && timer_start == 1)
		{
			timer();

		}
		ticking();
    }
}
