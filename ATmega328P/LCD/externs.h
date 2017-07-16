/**************************************************************
 Target MCU & clock speed: ATmega328P @ 1Mhz internal
 Name    : externs.h
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
#include "defines.h"

extern uint8_t wd;
extern uint8_t words000[10][2][16];
extern uint8_t words001[10][2][16];

extern uint8_t prevMenuCnt;

extern uint8_t program_author[];
extern uint8_t program_version[];
extern uint8_t program_date[];

extern uint8_t menu_str1[];
extern uint8_t menu_str2[];

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
extern  void LCD_WriteTimeDateTempHumid();
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
extern char strTemp[3], strHumid[3];

extern int8_t dht_getdata(int8_t *, int8_t *);
extern int8_t dht_getdata_EXT16MHZ(int8_t *, int8_t *);
extern int8_t dht_getdata_INT8MHZ(int8_t *, int8_t *);

extern int8_t dht_gettemperaturehumidity(int8_t *, int8_t *);
extern void ioinit (void);

extern int8_t hour, min, sec;
extern int8_t year, month, date;
extern int8_t monthEndDate, day;
extern int8_t accumulatedHour, accumulatedMin, accumulatedSec;
//extern uint8_t hourlyAdjusted;

extern uint8_t pwrOn;

/************************************
External Functions
*************************************/
/*----------------------------
config.c (defined at)
------------------------------*/
extern void LCDconfig();
extern void SwitchConfig();
extern void RTC_DS1307config();
extern void DHT11config();
extern void debugPinConfig();

/*----------------------------
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
DHT11.c (defined at)
=======
DHT.c (defined at)
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
DHT.c (defined at)
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
DHT.c (defined at)
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
DHT.c (defined at)
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
------------------------------*/
extern  void getDHT();

/*----------------------------
DS1307.c (defined at)
------------------------------*/
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
extern void setTime2DS1307(byte ,byte ,byte ,byte , byte , byte , byte , byte );

extern void DS1307_GetTime(byte*, byte*, byte*);
=======
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
//void SetTimeDate(byte wkDay,byte month,byte day,byte year, byte ampm, byte h, byte m, byte s)
extern void SetTimeDate(byte ,byte ,byte ,byte , byte , byte , byte , byte );
extern void setTime2DS1307(byte ,byte ,byte ,byte , byte , byte , byte , byte );

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
extern void DS1307_GetDate(byte*, byte*, byte*);
extern void DS1307_GetWkDay(byte*);

//void SetTimeDate(byte wkDay,byte month,byte day,byte year, byte ampm, byte h, byte m, byte s)
extern void SetTimeDate(byte ,byte ,byte ,byte , byte , byte , byte , byte );

extern void adjustHour();
extern void adjustMin();
extern void adjustSec();

/*----------------------------
I2C.c (defined at)
------------------------------*/
extern void I2C_Init();
extern byte I2C_Detect(byte );
extern byte I2C_FindDevice(byte );
extern void I2C_Start (byte );
extern byte I2C_Write (byte ); 
extern byte I2C_ReadACK ();
extern byte I2C_ReadNACK ();
extern void I2C_WriteByte(byte , byte );
extern void I2C_WriteRegister(byte , byte , byte );
extern byte I2C_ReadRegister(byte , byte );

/*----------------------------
intrpt.c (defined at)
------------------------------*/
//ISR(PCINT0_vect)
//ISR(WDT_vect)
extern void WDT_Init(void);
extern void initINT(void);

extern void setup_wdt(void);
extern void check_wdt(void);

extern void init_devices(void);
extern void proceedClock();


/*----------------------------
LCD.c (defined at)
------------------------------*/
extern void lcd_init_4d(void);
extern void lcd_write_string_4d(uint8_t *);
extern void lcd_write_character_4d(uint8_t);
extern void lcd_write_instruction_4d(uint8_t);
extern void lcd_write_4(uint8_t);

/*----------------------------
LCD_DS1307_DHT11.c (defined at)
------------------------------*/
extern void ShowI2Cdevices();
extern void LCD_WriteDate();
extern void LCD_WriteTime();
extern void LCD_WriteTimeDate();
extern void LCD_WriteTimeDateTempHumid();
extern void test_LCD_DS1307();

/*----------------------------
LCDutil.c (defined at)
------------------------------*/
extern void LCD_Hex(int );
extern void LCD_BCDDigits(uint8_t );

extern void turnOnLCDBacklight();
extern void turnOffLCDBacklight();

extern void LCD_blankChar(uint8_t );

extern void lcd_SysTime();
extern void lcd_dispON();
extern void lcd_dispOFF();

extern void lcd_dispRealClock();
extern void lcd_dispAccumulatedTime();

extern void lcd_showDHT11();

extern void lcd_dispProgInfo();
extern void lcd_dispMenu();

<<<<<<< HEAD
extern void lcd_dispWords(uint8_t );
extern void lcd_testString();
=======
// ------------------------
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712

extern void LCD_dispNotice();
extern void chkButtonAndToggleBacklight();
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712

/*----------------------------
Menu.c (defined at)
------------------------------*/
extern void countButton(void);

/*----------------------------
PowerSave.c (defined at)
------------------------------*/
extern void makePDasOutput();
extern void makePDasInput();

/*----------------------------
strFunc.c (defined at)
------------------------------*/
extern void parseCompileTime(void);
extern void calcDay(void);

/*----------------------------
TimingDebug.c (defined at)
------------------------------*/
extern void testTimingOnDebugPin();
extern void sysClockTest();



/*----------------------------
PowerSave.c (defined at)
------------------------------*/
extern void LCD_pwrReady()

<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
<<<<<<< HEAD
=======
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
=======
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712
/*----------------------------
Menu.c (defined at)
------------------------------*/
extern void sysClockTest();
extern void countButton(void);
extern void adjustHour();
extern void adjustMin();
extern void adjustSec();
>>>>>>> c157654a527912666fa1b3f4686432252aa6a712

// Function Prototypes
extern void config();
// ------------------------
