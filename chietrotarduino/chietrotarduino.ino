#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <HX711.h>
#include <Servo.h>

#define bangtai1 8
#define bomnuoc 9
#define bangtai2 10
#define xoaynap 11
#define pittong 12
//Xservo.attach(5);
#define cb1 15 //A1
#define cb2 14 //A0
//#define manualstep 13
//SoftwareSerial mySerial(3, 4);// RX, TX
//const int LOADCELL_DOUT_PIN = 6;
//const int LOADCELL_SCK_PIN = 7;


SoftwareSerial mySerial(3, 4);// RX, TX
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int LOADCELL_DOUT_PIN = 6;
const int LOADCELL_SCK_PIN = 7;
HX711 scale;
Servo Xservo;
unsigned int pos;

boolean chay;
boolean bt1;
boolean bt2;
boolean bn;
boolean pt;
boolean xn;

String ttbt1 = "";
String ttbt2 = "";
String ttpt = "";
String ttbn = "";
String ttxn = "";
String ttservo = "";


String data = "";
String caisochai = "";
String caiKL = "";
String chonchedo = "" ;
String ketthuc = "";
String lcdchedo = "";


unsigned int ckl;
unsigned int csc;
int luongnuoc;
int luongnuoc1;
unsigned int zero;
unsigned int demchai;
unsigned int pos1;

void setlcd();
void Auto();
void Manual();
void Stop();
void caidat();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);


  lcd.init();
  lcd.backlight();

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(1200.f);
  scale.tare();
  zero = scale.get_units(1);

  pinMode(bangtai1, OUTPUT);
  pinMode(bangtai2, OUTPUT);
  pinMode(bomnuoc, OUTPUT);
  pinMode(xoaynap, OUTPUT);
  pinMode(pittong, OUTPUT);
  pinMode(cb1, INPUT);
  pinMode(cb2, INPUT);
  pinMode(2, INPUT_PULLUP);
  //attachInterrupt(0, Stop, LOW);
  Xservo.attach(5);
  Xservo.write(150);

}

void loop() {
  // put your main code here, to run repeatedly:
  caidat();
  setlcd();

}//end loop

void caidat()
{
  

  while (mySerial.available() > 0)
  {
    digitalWrite(bangtai1, LOW);
    digitalWrite(bangtai2, LOW);
    digitalWrite(bomnuoc, LOW);
    digitalWrite(xoaynap, LOW);
    digitalWrite(pittong, LOW);
    //Xservo.write(150);

    data = "";
    caisochai = "";
    caiKL = "";
    chonchedo = "";
    ketthuc = "";
    ttbt1 = "";
    ttbt2 = "";
    ttpt = "";
    ttbn = "";
    ttxn = "";

    data = mySerial.readString();
    Serial.println(data);

    if (data[0] == '#')
    {
      for (int i = 1; i < data.length(); i++)
      {
        caisochai += data[i];
        csc = caisochai.toInt();
      }
    }
    else if (data[0] == '$')
    {
      for (int i = 1; i < data.length(); i++)
      {
        caiKL += data[i];
        ckl = caiKL.toInt();
      }
    }
    else if (data[0] == '@')
    {
      for (int i = 1; i < data.length(); i++)
      {
        ketthuc += data[i];
        chay = ketthuc.toInt();
        Serial.println(chay);
      }
    }
    else if (data[0] == '%')
    {
      for (int i = 1; i < data.length(); i++)
      {
        chonchedo += data[i];
        lcdchedo = "";
        lcdchedo = chonchedo;
      }
    }
    else if (data[0] == '?')
    {
      for (int i = 1; i < data.length(); i++)
      {
        ttbt1 += data[i];
        bt1 = ttbt1.toInt();
      }
    }
    else if (data[0] == '^')
    {
      for (int i = 1; i < data.length(); i++)
      {
        ttbt2 += data[i];
        bt2 = ttbt2.toInt();
      }
    }
    else if (data[0] == '&')
    {
      for (int i = 1; i < data.length(); i++)
      {
        ttbn += data[i];
        bn = ttbn.toInt();
      }
    }
    else if (data[0] == '*')
    {
      for (int i = 1; i < data.length(); i++)
      {
        ttpt += data[i];
        pt = ttpt.toInt();
      }
    }
    else if (data[0] == '+')
    {
      for (int i = 1; i < data.length(); i++)
      {
        ttxn += data[i];
        xn = ttxn.toInt();
      }
    }
    else if (data[0] == '=')
    {
      for (int i = 1; i < data.length(); i++)
      {
        ttservo += data[i];
        pos1 = ttservo.toInt();
      }
    }


    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(lcdchedo);

    lcd.setCursor(4, 0);
    lcd.print("Ch:");

    lcd.setCursor(7, 0);
    lcd.print(csc);

    lcd.setCursor(10, 0);
    lcd.print("KL:");

    lcd.setCursor(13, 0);
    lcd.print(ckl);

    lcd.setCursor(0, 1);
    lcd.print("Chai:");

    lcd.setCursor(9, 1);
    lcd.print("Can:");
  }
}

void setlcd()
{
  if (digitalRead(2) == 0)
  {
    Stop();
  }

  else
  {
    if (lcdchedo == "Au" )
    {
      Auto();
    }
    else if (lcdchedo == "Man" )
    {
      Manual();
    }
  }
}// end setlcd*/

void Auto()//////////////////////////////////////////////////////////////////////////////
{
  //Serial.println(chonchedo);

  digitalWrite(bangtai1, LOW);
  digitalWrite(bangtai2, LOW);
  digitalWrite(bomnuoc, LOW);
  digitalWrite(xoaynap, LOW);
  digitalWrite(pittong, LOW);
  Xservo.write(150);
  for ( int i = 1; i <= csc; i++)
  {

    ///////////////////////////////////// bang tai 1 chay khi gap cb1 dung
    if (digitalRead(2) == 0)
    {
      break;
    }
    while (digitalRead(cb1) == 1)
    {
      if (digitalRead(2) == 0)
      {
        break;
      }
      digitalWrite(bangtai1, HIGH);
      if (digitalRead(cb1) == 0)
      {
        break;
      }

    }

    digitalWrite(bangtai1, LOW);
    ////////////////////////////////////////////////// gat servo vao can
    if (digitalRead(2) == 0)
    {
      break;
    }
    for (pos = 150; pos > 110; pos--)
    {
      if (digitalRead(2) == 0)
      {
        break;
      }
      Xservo.write(pos);
      delay(100);

    }
    delay(2000);
    /////////////////////////////////////////////// can chai va bom nuoc
    if (digitalRead(2) == 0)
    {
      break;
    }
    while (luongnuoc < ckl)
    {
      if (digitalRead(2) == 0)
      {
        break;
      }
      luongnuoc = scale.get_units(1) * (-1) - zero ;
      Serial.println(luongnuoc);
      mySerial.println(luongnuoc);
      delay(10);
      lcd.setCursor(13, 1);
      lcd.print(luongnuoc);
      digitalWrite(bomnuoc, HIGH);
      if (digitalRead(2) == 0)
      {
        break;
      }
      if (luongnuoc >= ckl)
      {
        break;
      }
    }
    digitalWrite(bomnuoc, LOW);
    if (digitalRead(2) == 0)
    {
      break;
    }
    /////////////////////////////////////// chay bt2 va sero gat chai sang bt2
    digitalWrite(bangtai2, HIGH);
    if (digitalRead(2) == 0)
    {
      break;
    }
    delay(1000);
    Xservo.write(100);
    for (pos = 100; pos > 50; pos--)
    {
      if (digitalRead(2) == 0)
      {
        break;
      }
      Xservo.write(pos);
      delay(100);
    }
    Xservo.write(150);
    ///////////////////////////////////// bt2 chay gap cb2 dung
    if (digitalRead(2) == 0)
    {
      break;
    }
    while (digitalRead(cb2) == 1)
    {
      if (digitalRead(2) == 0)
      {
        break;
      }
      digitalWrite(bangtai2, HIGH);
      if (digitalRead(cb2) == 0)
      {
        break;
      }
    }
    luongnuoc = scale.get_units(1) * (-1) - zero ;
    lcd.setCursor(13, 1);
    lcd.print("000");
    if (digitalRead(2) == 0)
    {
      break;
    }
    /////////////////////////////////// mo pittong ha van, xoay d/c van nap
    digitalWrite(bangtai2, LOW);
    if (digitalRead(2) == 0)
    {
      break;
    }
    delay(1000);
    if (digitalRead(2) == 0)
    {
      break;
    }
    digitalWrite(pittong, HIGH);
    if (digitalRead(2) == 0)
    {
      break;
    }
    delay(2000);
    if (digitalRead(2) == 0)
    {
      break;
    }
    digitalWrite(xoaynap, HIGH);
    if (digitalRead(2) == 0)
    {
      break;
    }
    delay(3000);
    if (digitalRead(2) == 0)
    {
      break;
    }
    digitalWrite(xoaynap, LOW);
    if (digitalRead(2) == 0)
    {
      break;
    }
    delay(2000);
    if (digitalRead(2) == 0)
    {
      break;
    }
    digitalWrite(pittong, LOW);
    if (digitalRead(2) == 0)
    {
      break;
    }
    digitalWrite(bangtai2, HIGH);
    if (digitalRead(2) == 0)
    {
      break;
    }
    delay(3000);
    if (digitalRead(2) == 0)
    {
      break;
    }
    digitalWrite(bangtai2, LOW);
    if (digitalRead(2) == 0)
    {
      break;
    }
    lcd.setCursor(5, 1);
    lcd.print(i);
    mySerial.println('!' + String(i));
    if (digitalRead(2) == 0)
    {
      break;
    }
  }
  setlcd();
}// end Auto();

void Manual()//////////////////////////////////////////////////////////////////
{
  //Serial.println(chonchedo);
  digitalWrite(bangtai1, bt1);
  digitalWrite(bangtai2, bt2);
  digitalWrite(pittong, pt);
  digitalWrite(bomnuoc, bn);
  digitalWrite(xoaynap, xn);
  Xservo.write(pos1);
}

void Stop()
{

  digitalWrite(bangtai1, LOW);
  digitalWrite(bangtai2, LOW);
  digitalWrite(bomnuoc, LOW);
  digitalWrite(xoaynap, LOW);
  digitalWrite(pittong, LOW);
  Xservo.write(150);

}
