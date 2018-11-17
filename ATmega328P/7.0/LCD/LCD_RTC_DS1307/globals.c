/**************************************************************
 Target MCU & clock speed: ATmega328P @ 1Mhz internal
 Name    : globals.c
 Author  : Insoo Kim (insoo@hotmail.com)
 Created : May 15, 2015
 Updated : May 17, 2015

 Description: Get system compile time & date and display on LCD 2*16
    Button toggling to turn on or off the backlight of LCD

 HEX size[Byte]:

 Ref:
    Donald Weiman    (weimandn@alfredstate.edu)
    http://web.alfredstate.edu/weimandn/programming/lcd/ATmega328/LCD_code_gcc_4d.html
 *****************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include "defines.h"

uint8_t wd=0;

// New words
uint8_t words000[10][2][16] =
    {
      {//0 "1234512345123456" "1234512345123456"
           //"petrified",       "cause2bcmStnlike"
           "succumb",         "fail to resist"
      },
      {//1 "1234512345123456" "1234512345123456"
         "arid",            "lckgSuffWtr/Rain"
      },
      {//2 "1234512345123456" "1234512345123456"
         "enliven",         "hgtn/intensify"
      },
      {//3"1234512345123456" "1234512345123456"
         "lurch",           "mv abruptly"
      },
      {//4 "1234512345123456" "1234512345123456"
        "oblivious to",     "lckgCnscsAwrnOf"
      },
      {//5 "1234512345123456"   "1234512345123456"
         "wtn a whisker of",  "extremelyClose 2"
      },
      {//6 "1234512345123456"   "1234512345123456"
           "colloid",           "mixWpropBtSolSus"
      },
      {//7 "1234512345123456"   "1234512345123456"
           "reticent",           "disclined2talk"
      },
      {//8 "1234512345123456"   "1234512345123456"
           "pry~apart",         "force2getSthOpen"
      },
      {//9 "1234512345123456"   "1234512345123456"
           "esoteric",          "cnfnd2onyEnlgtnd"
      },
    }; //words000[10][2][16]

uint8_t words001[10][2][16] =
    {
      {//0 "1234512345123456" "1234512345123456"
         "snag",              "hiddenObstacle"
      },
      {//1 "1234512345123456" "1234512345123456"
         "mission-agnostic",  "genApplicable"
      },
      {//2 "1234512345123456" "1234512345123456"
           "deep-stedCulture","fracturedRltnshp"
      },
      {//3"1234512345123456" "1234512345123456"
         "tumult",           "loudCnfsdNseByMs"
      },
      {//4 "1234512345123456" "1234512345123456"
        "sobering",           "tending2mkSober"
      },
      {//5 "1234512345123456"   "1234512345123456"
         "miscarrage",          "brthBbyAlrdDd@Wm"
      },
      {//6 "1234512345123456"   "1234512345123456"
           "class-actnLwSt",    "lawSuitTogether"
      },
      {//7 "1234512345123456"   "1234512345123456"
           "blister",           "fluidPocket"
      },
      {//8 "1234512345123456"   "1234512345123456"
           "titter",            "laughQuietNrvsly"
      },
      {//9 "1234512345123456"   "1234512345123456"
           "line cook",         "a cook inKitchen"
      },
    }; //words001[10][2][16]

uint8_t prevMenuCnt;

int8_t hour=0, min=0, sec=0;
int8_t year=0, month=0, date=0;
int8_t monthEndDate, day=0;
int8_t accumulatedHour=0, accumulatedMin=0,accumulatedSec=0;
//uint8_t hourlyAdjusted=0;

// Program ID
uint8_t program_author[]   = "Insoo Kim     ";
uint8_t program_version[]  = "LCD-ATmega328p";
//uint8_t program_date[]     = "May 23, 2015  "; // Initial Creation Date
//uint8_t program_date[]     = "Mar 22, 2017  "; // Last Update
uint8_t program_date[]     = "Jul 11, 2017  "; // Last Update

// Menu info
uint8_t menu_str1[]   = "1:Rdy Or PrevSel";
uint8_t menu_str2[]  =  "2CK3TH4BL5AJ6AT";

