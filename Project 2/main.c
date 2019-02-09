//
// Created by dzyol on 2/3/2019.
//
#include "stdio.h"
#include "string.h"
#include "avr.h"
#include "lcd.h"

int is_military = 0;
char twelve_hour[10] = "AM";
int year = 2020;
int month = 2;
int day = 28;
int hour = 23;
int min = 59;
int sec = 55;
int month_days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

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

char is_leap(){
	if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)){
		return 1;
	}
	else{
		return 0;
	}
	
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



int main(){
    ini_avr();
    ini_lcd();

    clr_lcd();

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
		if (num_to_value(num) == 'C')
		{
			edit_day();
		}
		
		if (num_to_value(num) == 'D')
		{
			edit_time();
		}
		
		if (num_to_value(num) == '*')
		{
			is_military = 0;
		}
		if (num_to_value(num) == '#')
		{
			is_military = 1;
		}
		
		
		sprintf(daybuffer, "%02i/%02i/%02i   ", month, day, year);
		pos_lcd(0, 0);
		puts_lcd2(daybuffer);
		
		if (is_military)
		{
			sprintf(timebuffer, "%02i:%02i:%02i   ", hour, min, sec);
			pos_lcd(1, 0);
			puts_lcd2(timebuffer);
		}
		else
		{
			if (hour > 12 && hour < 24)
			{
				sprintf(timebuffer, "%02i:%02i:%02i PM", hour - 12, min, sec);
				pos_lcd(1, 0);
				puts_lcd2(timebuffer);
			}
			else if (hour <= 12 && hour > 0)
			{
				sprintf(timebuffer, "%02i:%02i:%02i AM", hour, min, sec);
				pos_lcd(1, 0);
				puts_lcd2(timebuffer);
			}
			else if (hour == 0)
			{
				sprintf(timebuffer, "%02i:%02i:%02i AM", hour + 12, min, sec);
				pos_lcd(1, 0);
				puts_lcd2(timebuffer);
			}

		}

		
		
		wait_avr(950);
		ticking();
    }
}
