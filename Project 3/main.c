//
// Created by dzyol on 2/21/2019.
//

#include "stdio.h"
#include "avr.h"
#include "lcd.h"

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
int volume = 5;
int play = 0;

int birthday[] = {G_NOTE, G_NOTE, A_NOTE, G_NOTE, C_NOTE, B_NOTE, G_NOTE, G_NOTE, A_NOTE, G_NOTE, D_NOTE, C_NOTE, G_NOTE,G_NOTE,G_NOTE,E_NOTE,C_NOTE,B_NOTE,A_NOTE,F_NOTE,F_NOTE,E_NOTE,C_NOTE, D_NOTE, C_NOTE};

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

int main(){
    ini_avr();
    ini_lcd();

    SET_BIT(DDRB,3);

    clr_lcd();
    char tempobuffer[20];
    char volumebuffer[20];

    while(1){
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
            if (volume > 1 && volume <= 10)
            {
                volume--;
            }
        }
        else if (num_to_value(num) == 'D')
        {
            if (volume >= 1 && volume < 10)
            {
                volume++;
            }
        }
        else if (num_to_value(num) == '1')
        {
            play_birthday();
            play = 1;
        }
        else if (num_to_value(num) == '*')
        {
            play = 0;
        }
        if (play == 1)
        {
            play_birthday();
            wait_avr(1000);
        }
    }
}