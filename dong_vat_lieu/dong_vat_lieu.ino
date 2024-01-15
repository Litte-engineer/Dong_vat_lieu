#include "main.h"

uint8_t step;
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

  boolean pro_input;

  uint8_t ngat;
  uint8_t home;
} Status;
Status status;

uint32_t time;
uint16_t count_time;
uint8_t count_input;
uint8_t count_reset;

uint16_t set_weight;
uint16_t weight;

uint8_t counter;
uint8_t zero;

void run_mode(void);
void setup_mode(void);
void lcd_run_mode(void);
void lcd_setup_mode(void);
void lcd_cursor(void);
void run_function(void);
void go_home(void);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  scale.begin(LOADCELL_DT_PIN , LOADCELL_SCK_PIN);
  scale.set_scale(1200.f);
  scale.tare();
  zero = scale.get_units(1);


  lcd.init();
  lcd.backlight();
  lcd.clear();

  pinMode(13, OUTPUT);
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

  set_weight = readIntFromEEPROM(ADD_WEIGHT);
  counter    = EEPROM.read(ADD_COUNTER);
  attachInterrupt(0, interruptFunction, RISING);

  if (digitalRead(H92B4_PIN) == 1) Step1.stop();



}

void loop() {
  // put your main code here, to run repeatedly:

  status.start_bnt = digitalRead(START_BNT);
  status.stop_bnt  = digitalRead(STOP_BNT);
  status.ir1       = digitalRead(IR1);
  status.ir2       = digitalRead(IR2);
  status.h92b4     = digitalRead(H92B4_PIN);
  go_home();
  if (status.home == 1)
  {
    if (status.mode == 0)
    {
      run_mode();
    }
    else if (status.mode == 1)
    {
      setup_mode();
    }
  }
}
/***** ngat ***************/
void interruptFunction() {
  status.ngat = 1;
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
    /*********** buoc 0 ********/
    if (status.run == 0)
    {
      BANG_TAI_OFF;
      DOUBLE_MOTOR_OFF;
      CTR_OFF;
      if (status.start_bnt == 0 && status.stop_bnt == 1)
      {
        DOUBLE_MOTOR_ON;
        BANG_TAI_ON;
        status.run = 1;
      }
    }
    /*********** buoc 1 ********/
    if (status.run == 1)
    {    
      if (status.ir1 == 0)
      {
        delay(350);    /// thoi gian de bang tai dung
        BANG_TAI_OFF;
        DOUBLE_MOTOR_OFF;
        CTR_ON;
        delay(100);
        status.run = 2;
      }
      else 
      {
        DOUBLE_MOTOR_OFF;
        BANG_TAI_ON;
      }
    }
    /*********** buoc 2 ********/
    if (status.run == 2)
    {
      if (status.pro_input == 0)
      {
        digitalWrite(STEP_EN, LOW);
        Step1.moveTo (50);
        Step1.setMaxSpeed(50);
        Step1.setAcceleration(100);
        Step1.run();
        if (Step1.currentPosition() == 50) status.run = 3;
      }
      else
      {
        digitalWrite(STEP_EN, LOW);
        Step1.moveTo (-50);
        Step1.setMaxSpeed(50);
        Step1.setAcceleration(100);
        Step1.run();
        if (Step1.currentPosition() == -50) status.run = 3;
      }
    }
    /*********** buoc 3 ********/
    if (status.run == 3)
    {
      weight = (scale.get_units(1) * (-1) - zero) + 145 ;
      lcd.setCursor(7, 2);
      char WEIGHT[4];
      sprintf(WEIGHT, "%04d", weight);
      lcd.print(WEIGHT);
      if (weight >=  set_weight) status.run = 4;
    }
    /*********** buoc 4 ********/
    if (status.run == 4)
    {
      CTR_OFF;
      delay(3000);
      BANG_TAI_ON;
      DOUBLE_MOTOR_ON; 
      status.run = 5;
    }
    /*********** buoc 5 ********/
    if (status.run == 5)
    {
      delay(5000);
      counter = counter + 1;
      status.run = 1;
    }
  }
  else if (status.emergency == 1)
  {
    BANG_TAI_OFF;
    DOUBLE_MOTOR_OFF;
    CTR_OFF;
    if (status.start_bnt == 0 && status.stop_bnt == 1) status.emergency = 2;
  }
  else if (status.emergency == 2)
  {
    if (status.start_bnt == 1 && status.stop_bnt == 1) status.emergency = 0;
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
      writeIntToEEPROM(set_weight, ADD_WEIGHT ) ;
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
  lcd.setCursor(11, 0);
  if (status.pro_input == 0)
  {
    lcd.print("GAO_1");
  }
  else if (status.pro_input == 1)
  {
    lcd.print("GAO_2");
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
  lcd.setCursor(-4, 2);
  lcd.print("KL THUC");
  lcd.setCursor(5, 2);
  lcd.print(":");
  lcd.setCursor(7, 2);
  char WEIGHT[4];
  sprintf(WEIGHT, "%04d", weight);
  lcd.print(WEIGHT);
  lcd.setCursor(11, 2);
  lcd.print("g");

  /***** san luong *****/
  lcd.setCursor(-4, 3);
  lcd.print("SAN LUONG");
  lcd.setCursor(5, 3);
  lcd.print(":");
  lcd.setCursor(7, 3);
  char COUNTER[3];
  sprintf(COUNTER, "%03d", counter);
  lcd.print(COUNTER);

  /*****//////////////////////////////////////////////////////////
  lcd.setCursor(11, 3);
  lcd.print(status.run);
}
/****** man hinh setup mode ****/
void lcd_setup_mode(void)
{
  /**** dong 0 ****/
  lcd.setCursor(4, 0);
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
  lcd.setCursor(-3, 2);
  lcd.print("XOA SL :");
  lcd.setCursor(6, 2);
  char COUNTER[3];
  sprintf(COUNTER, "%03d", counter);
  lcd.print(COUNTER);

  /***** dong 3 *****/
  lcd.setCursor(-3, 3);
  lcd.print("CHON VL:");
  lcd.setCursor(6, 3);
  if (status.pro_input == 0)
  {
    lcd.print("GAO_1");
  }
  else if (status.pro_input == 1)
  {
    lcd.print("GAO_2");
  }
  /**** con tro ****/
  if (status.cursor == 0)
  {
    lcd.setCursor(0, 1);
    lcd.print(">");
    lcd.setCursor(-4, 2);
    lcd.print(" ");
    lcd.setCursor(-4, 3);
    lcd.print(" ");
  }
  else if (status.cursor == 1)
  {
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(-4, 2);
    lcd.print(">");
    lcd.setCursor(-4, 3);
    lcd.print(" ");
  }
  else if (status.cursor == 2)
  {
    lcd.setCursor(0, 1);
    lcd.print(" ");
    lcd.setCursor(-4, 2);
    lcd.print(" ");
    lcd.setCursor(-4, 3);
    lcd.print(">");
  }
}
/******** cac muc lua chon ******/
void lcd_cursor(void)
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
  else if (status.start_bnt == 1 && status.stop_bnt == 1)
  {
    count_input = 0;
    count_reset = 0;
    count_time = 0;
  }

  switch (status.cursor)
  {
    case 0 :
      uint16_t adc_value = analogRead(POT2);
      if (adc_value > 800) adc_value = 800;
      set_weight = adc_value;
      break;
    case 1 :
      if (status.start_bnt == 1 && status.stop_bnt == 0)
      {
        if (millis() - time > 50)
        {
          count_reset ++;
          if (count_reset > 2) count_reset = 3;
          time = millis();
        }
        if (count_reset == 2)
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
          count_input ++;
          if (count_input > 2) count_input = 3;
          time = millis();
        }
        if (count_input == 2)
        {
          status.pro_input = !status.pro_input;
        }
      }
      break;
  }
}

/****** ham set up *****/
void go_home(void)
{
  if (status.home == 0)
  {
    if (status.ngat == 0)
    {
      lcd.setCursor(3, 0);
      lcd.print("CAI GOC .....");
      digitalWrite(STEP_EN, LOW);
      Step1.moveTo (400);
      Step1.setMaxSpeed(50);
      Step1.setAcceleration(100);
      Step1.run();
      if (Step1.currentPosition() == 400) status.ngat = 1;
    }
    if (status.ngat == 1)
    {
      digitalWrite(STEP_EN, HIGH);
      Step1.setCurrentPosition(0);
      lcd.clear();
      status.ngat = 2;
      status.home = 1;
    }
  }
}
