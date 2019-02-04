//
// Created by dzyol on 2/3/2019.
//
#include "stdio.h"
#include "avr.h"
#include "lcd.h"

unsigned int is_pressed(int r, int c){
    //Set to default
    DDRC = 0;

    r += 4;

    //Set c strong 1
    CLR_BIT(DDRC, c);
    SET_BIT(PORTC, c);

    //Set r weak 1
    SET_BIT(DDRC, r);
    CLR_BIT(PORTC, r);



    return (GET_BIT(PINC, r)) == 1 ? 0 : 1;
}

unsigned int get_key(){
    int r, c;
    for (r = 0; r < 4; ++r)
    {
        for (c = 0; c < 4; ++c)
        {
            if (is_pressed(r, c))
                return r * 4 + c + 1;
        }
    }
    return 0;
}