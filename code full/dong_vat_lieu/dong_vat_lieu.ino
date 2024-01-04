#include "main.h"

typedef struct
{
  uint8_t start_bnt;
  uint8_t stop_bnt;
  uint8_t mode;
  uint8_t cursor;
  uint8_t ir1;
  uint8_t ir2;
  uint8_t h92b4;

  uint8_t en_run_mode;
  uint8_t en_setup_mode;

  uint8_t emergency;

  uint8_t run;

  uint8_t pro_input;
} Status;
Status status;

uint32_t time;
uint16_t count_time;

uint16_t set_weight;
uint16_t weight;

uint8_t counter;

void run_mode(void);
void setup_mode(void);
void lcd_run_mode(void);
void lcd_setup_mode(void);
void lcd_cursor(void);
void run_function(void);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  scale.begin(LOADCELL_DT_PIN , LOADCELL_SCK_PIN);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(STEP_STEP, OUTPUT);
  pinMode(STEP_EN, OUTPUT);
  pinMode(STEP_DIR, OUTPUT);

  pinMode(H92B4_PIN, INPUT);
  pinMode(IR1, INPUT_PULLUP);
  pinMode(IR2, INPUT_PULLUP);
  pinMode(START_BNT, INPUT_PULLUP);
  pinMode(STOP_BNT, INPUT_PULLUP);

  pinMode(CTR, OUTPUT);
  pinMode(BANG_TAI, OUTPUT);
  pinMode(DOUBLE_MOTOR, OUTPUT);

  set_weight = EEPROM.read(ADD_COUNTER);

}

void loop() {
  // put your main code here, to run repeatedly:
  status.start_bnt = digitalRead(START_BNT);
  status.stop_bnt  = digitalRead(STOP_BNT);
  status.ir1       = digitalRead(IR1);
  status.ir2       = digitalRead(IR2);
  status.h92b4     = digitalRead(H92B4_PIN);
  if (status.mode == 0)
  {
    run_mode();
  }
  else if (status.mode == 1)
  {
    setup_mode();
  }

}

/****** che do chay *****/
void run_mode(void)
{
  lcd_run_mode();
  run_function();
  /***** chuyen che do cai dat *****/
  if (status.en_run_mode == 0)
  {
    if (status.start_bnt == 1 && status.stop_bnt == 1) status.en_run_mode = 1;
  }
  if (status.en_run_mode == 1)
  {
    if (status.start_bnt == 0 && status.stop_bnt == 0)
    {
      status.mode = 1;
      status.en_setup_mode = 0;
      lcd.clear();
    }
  }
}
/********** chay mo hinh *******/
void run_function(void)
{
  if (status.emergency == 0)
  {
    if (status.start_bnt == 1 && status.stop_bnt == 0 && status.run != 0) status.emergency = 1;
    
    if (status.run == 0)
    {
      BANG_TAI_OFF;
      DOUBLE_MOTOR_OFF;
      CTR_OFF;
      if (status.start_bnt == 0 && status.stop_bnt == 1)
      {
        BANG_TAI_ON;
        status.run = 1;
      }
    }
    if (status.run == 1)
    {
      if (status.ir1 == 0)
      {
        delay(100);    /// thoi gian de bang tai dung
        BANG_TAI_OFF;

        CTR_ON;
        delay(100);
        status.run = 2;
      }
    }

    if(status.run == 2)
    {
      
    }
    
  }
  else
  {
    BANG_TAI_OFF;
    DOUBLE_MOTOR_OFF;
    CTR_OFF;
  }
}

/****** che do cai dat ****/
void setup_mode(void)
{
  lcd_cursor();
  lcd_setup_mode();
  /******* chuyen che do chay *******/
  if (status.en_setup_mode == 0)
  {
    if (status.start_bnt == 1 && status.stop_bnt == 1) status.en_setup_mode = 1;
  }
  if (status.en_setup_mode == 1)
  {
    if (status.start_bnt == 0 && status.stop_bnt == 0)
    {
      status.mode = 0;
      status.en_run_mode = 0;
      EEPROM.write(ADD_COUNTER, counter);
      delay(500);
      lcd.clear();
    }
  }
}

/****** man hinh mode run *****/
void lcd_run_mode(void)
{
  lcd.setCursor(0, 0);
  lcd.print("VAT LIEU");
  lcd.setCursor(9, 0);
  lcd.print(":");
  lcd.setCursor(11, 2);
  if (status.pro_input == 0)
  {
    lcd.print("GAO");
  }
  else if (status.pro_input == 1)
  {
    lcd.print("LAC");
  }

  /**** dong 1 ****/
  lcd.setCursor(0, 1);
  lcd.print("KL CAI");
  lcd.setCursor(9, 1);
  lcd.print(":");
  lcd.setCursor(11, 1);
  char SET_WEIGHT[4];
  sprintf(SET_WEIGHT, "%04d", set_weight);
  lcd.print(SET_WEIGHT);
  lcd.setCursor(15, 1);
  lcd.print("g");

  /*** dong 2 ****/
  lcd.setCursor(0, 2);
  lcd.print("KL THUC");
  lcd.setCursor(9, 2);
  lcd.print(":");
  lcd.setCursor(11, 2);
  char WEIGHT[4];
  sprintf(WEIGHT, "%04d", weight);
  lcd.print(WEIGHT);
  lcd.setCursor(15, 2);
  lcd.print("g");

  /***** san luong *****/
  lcd.setCursor(0, 3);
  lcd.print("SAN LUONG");
  lcd.setCursor(9, 3);
  lcd.print(":");
  lcd.setCursor(11, 3);
  char COUNTER[3];
  sprintf(COUNTER, "%03d", counter);
  lcd.print(COUNTER);
}
/****** man hinh setup mode ****/
void lcd_setup_mode(void)
{
  /**** dong 0 ****/
  lcd.setCursor(0, 0);
  lcd.print("CAI DAT");

  /**** dong 1 ****/
  lcd.setCursor(1, 1);
  lcd.print("CAI KL :");
  lcd.setCursor(10, 1);
  char SET_WEIGHT[4];
  sprintf(SET_WEIGHT, "%04d", set_weight);
  lcd.print(SET_WEIGHT);
  lcd.setCursor(15, 1);
  lcd.print("g");

  /***** dong 2 *****/
  lcd.setCursor(1, 2);
  lcd.print("XOA SL :");
  lcd.setCursor(10, 2);
  char COUNTER[3];
  sprintf(COUNTER, "%03d", counter);
  lcd.print(COUNTER);

  /***** dong 3 *****/
  lcd.setCursor(1, 3);
  lcd.print("CHON VL:");
  lcd.setCursor(10, 2);
  if (status.pro_input == 0)
  {
    lcd.print("GAO");
  }
  else if (status.pro_input == 1)
  {
    lcd.print("LAC");
  }

  /**** con tro ****/
  lcd.setCursor(0, status.cursor + 1);
  lcd.print(">");
}
/******** cac muc lua chon ******/
void lcd_cursor()
{
  if (status.start_bnt == 0 && status.stop_bnt == 1)
  {
    if (millis() - time > 50)
    {
      count_time ++;
      if (count_time > 2) count_time = 3;
      time = millis();
    }
    if (count_time == 2)
    {
      status.cursor ++;
      if (status.cursor > 2) status.cursor = 0;
    }
  }
  else if (status.start_bnt == 1 && status.stop_bnt == 1) count_time = 0;

  switch (status.cursor)
  {
    case 0 :
      uint16_t adc_value = analogRead(POT2);
      if (adc_value > 1000) adc_value = 1000;
      set_weight = adc_value;
      break;

    case 1 :
      if (status.start_bnt == 1 && status.stop_bnt == 0)
      {
        if (millis() - time > 50)
        {
          count_time ++;
          if (count_time > 2) count_time = 3;
          time = millis();
        }
        if (count_time == 2)
        {
          counter = 0;
        }
      }
      break;

    case 2 :
      if (status.start_bnt == 1 && status.stop_bnt == 0)
      {
        if (millis() - time > 50)
        {
          count_time ++;
          if (count_time > 2) count_time = 3;
          time = millis();
        }
        if (count_time == 2)
        {
          status.pro_input ++;
          if (status.pro_input > 1) status.pro_input = 0;
        }
      }
      break;
  }
}
