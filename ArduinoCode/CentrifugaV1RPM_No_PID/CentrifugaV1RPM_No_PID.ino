/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

See more at http://blog.squix.ch
*/
#include <Wire.h>
#include <SPI.h>
#include "SSD1306.h"
#include "OLEDDisplayUi.h"
#include <Servo.h> 

SSD1306  display(0x3c, D4, D3);
Servo myservo; 

OLEDDisplayUi ui     ( &display );

// how many frames are there?
int frameCount = 3,overlaysCount = 1;

int ESCpin = D0;

int Apin = D6,Bpin = D5,ENTERpin = D7;

int MIN = 0,SEC = 1,ESC_PWM = 20;
unsigned long previusMillis = 0,previusSec = 0,TIME = 0;

int encDIR = 0;
int encSTATE = 0;
int encCOUNT = 0;
volatile byte A_SIG = LOW,B_SIG = HIGH,ENTER = LOW;
volatile unsigned long SPEED = 4166000;

// rpm reading 
int sensorvalue;
int state1 = HIGH;
int state2;
float RPM = 0;
float TRPM = 0;
unsigned long prevMillis = 0;
unsigned long previusPWM = 0;
unsigned long previusENTER = 0;
unsigned long previusPrint = 0;
unsigned long interval = 200;
unsigned long currentTime;
unsigned long prevTime = 1;
unsigned long diffTime = 0;
int sensorthreshold = 120;           // this value indicates the limit reading between dark and light,
///////////////////////////////////////////////////////////////////////////////////////////////////
// internal timer interrupt for stepper controll



///////////////////////////////////////////////////////////////////////////////////////////////////
// interrupts for encoder reading
void A_RISE()
{
 detachInterrupt(Apin);
  A_SIG = HIGH;
  if(B_SIG == LOW) encDIR = 1;
  if(B_SIG == HIGH) encDIR = -1;
  ENC_COUNT();
 attachInterrupt(Apin, A_FALL, FALLING);
}
void A_FALL()
{
  detachInterrupt(Apin);
  A_SIG = LOW;
  if(B_SIG == HIGH) encDIR = 1;
  if(B_SIG == LOW) encDIR = -1;
  ENC_COUNT();
  attachInterrupt(Apin, A_RISE, RISING); 
}
void B_RISE()
{
  detachInterrupt(Bpin);
  B_SIG = HIGH;
  if(A_SIG == HIGH) encDIR = 1;
  if(A_SIG == LOW) encDIR = -1;
  ENC_COUNT();
  attachInterrupt(Bpin, B_FALL, FALLING);
}
void B_FALL()
{
  detachInterrupt(Bpin);
  B_SIG = LOW;
  if(A_SIG == LOW) encDIR = 1;
  if(A_SIG == HIGH) encDIR = -1;
  ENC_COUNT();
  attachInterrupt(Bpin, B_RISE, RISING); 
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// definiton of frames
void msOverlay(OLEDDisplay *display, OLEDDisplayUiState* state) 
{
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->setFont(ArialMT_Plain_10);
  display->drawString(128, 0, String(millis()));
}

void drawTIME(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
{
  //basic menu for setting time
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16);
  display->drawString(64 , 0 , "SET TIME");
  //display->setTextAlignment(TEXT_ALIGN_LEFT);
  //display->setFont(ArialMT_Plain_10);
  //display->drawString(0 , 20 , "SET TIME");
  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  String Time = String(MIN) + ":" + String(SEC);
  display->drawString(64 , 28 , Time);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 , 52 , "PLUS");
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 , 52 , "MINUS");
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128 , 52 , "OK");
}

void drawSPEED(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
{
  // Display to set Rpm
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16);
  display->drawString(64 , 0 , "SET SPEED");
  //display->setTextAlignment(TEXT_ALIGN_LEFT);
  //display->setFont(ArialMT_Plain_10);
  //display->drawString(0 , 20 , "SET SPEED");
  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  String Rpm = String(ESC_PWM);
  display->drawString(64 , 28 , Rpm);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->drawString(0 , 52 , "PLUS");
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 , 52 , "MINUS");
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128 , 52 , "OK");
}

void drawWORK(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
{

  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->setFont(ArialMT_Plain_16);
  display->drawString(64 , 0 , "WORKING");
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  display->setFont(ArialMT_Plain_10);
  String RPm = String(RPM) + " rpm";
  display->drawString(0 , 20 , RPm);
  display->setFont(ArialMT_Plain_24);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  String Time = String(MIN) + ":" + String(SEC);
  display->drawString(64 , 28 , Time);
  display->setFont(ArialMT_Plain_10);
  display->setTextAlignment(TEXT_ALIGN_LEFT);
  if(state1 == HIGH) display->drawString(0 , 52 , "L");
  else display->drawString(0 , 52 , "H");
  String Sens = String(sensorvalue);
  display->setTextAlignment(TEXT_ALIGN_CENTER);
  display->drawString(64 , 52 , Sens);
  String Esc = String(ESC_PWM);
  display->setTextAlignment(TEXT_ALIGN_RIGHT);
  display->drawString(128 , 52 , Esc);
}

void drawFrame4(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) 
{
   
}

void (*frames[])(OLEDDisplay *display, OLEDDisplayUiState* state, int16_t x, int16_t y) = { drawTIME, drawSPEED, drawWORK, drawFrame4 }; 
void (*overlays[])(OLEDDisplay *display, OLEDDisplayUiState* state)             = { msOverlay };

///////////////////////////////////////////////////////////////////////////////////////////////////


void setup()   
{  

  Serial.begin(115200);
  pinMode(Apin, INPUT_PULLUP);
  pinMode(Bpin, INPUT_PULLUP);
  pinMode(ENTERpin, INPUT_PULLUP);
  myservo.attach(ESCpin);
 
//////////////////////////////////////////////////////////////////////////////////////////////////
//display setup

  ui.setTargetFPS(10);
  ui.disableAutoTransition();
  ui.disableIndicator();
  ui.setIndicatorPosition(RIGHT);            // You can change this to TOP, LEFT, BOTTOM, RIGHT
  ui.setIndicatorDirection(LEFT_RIGHT);       // Defines where the first frame is located in the bar.
  ui.setFrameAnimation(SLIDE_LEFT);           // You can change the transition that is used SLIDE_LEFT, SLIDE_RIGHT, SLIDE_TOP, SLIDE_DOWN
  ui.setFrames(frames, frameCount);           // Add frames
  ui.setOverlays(overlays, overlaysCount);    // Add overlays

  // Inital UI takes care of initalising the display too.
  ui.init();
  display.flipScreenVertically();
  
////////////////////////////////////////////////////////////////////////////////////////////////
 //digitalWrite(IR_LEDpin, HIGH);
 
 interrupts();
 attachInterrupt(Apin, A_RISE, RISING);
 attachInterrupt(Bpin, B_RISE, RISING);
 
///////////////////////////////////////////////////////////////////////////////////////////////
// Esc Calibration 
 /* ESC_PWM = 255;
  myservo.write(ESC_PWM);
  delay(3000);
  ESC_PWM = 20;
  myservo.write(ESC_PWM);*/
   myservo.write(ESC_PWM);
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
void loop() 
{
  int remainingTimeBudget = ui.update();      // display refresh
  unsigned long currentMillis = millis();     //countdown timer
  if (currentMillis - previusPrint >= 1000)   // Serial print of current speed
  {
      
      Serial.print(RPM,0);
      Serial.print("/");
      Serial.println(ESC_PWM);
      previusPrint = currentMillis;
  }
  if (currentMillis - previusSec >= 1000)
  {
    previusSec = currentMillis;
    SEC = SEC - 1;
    if(SEC<=0)                              
      {
        if(SEC<=0 && MIN<=0)                    //runs on first round if SEC and MIN set to 0
         {
            while(ESC_PWM > 20){                // deceleration when time hits 0:0
              ESC_PWM = ESC_PWM - 5;
              myservo.write(ESC_PWM);
            }
              ESC_PWM = 20;
              SetTIME();
              SetSPEED();
         }
         else
         {
            MIN = MIN - 1;
            SEC = SEC + 59;
         }
      }
   }
  
  if(ENTER == LOW) {
    while(ESC_PWM > 20){                          // deceleration if canceled via encoder click
        ESC_PWM = ESC_PWM - 5;
        myservo.write(ESC_PWM);
        delay(10);
      }
    ESC_PWM = 20;
    myservo.write(ESC_PWM);
    ENTER = HIGH;
    SetTIME();                                     // stops and returns to Set time menu
    SetSPEED();
  }

    if(ESC_PWM >= 20 && ESC_PWM <= 255){           // changing the rotation speed while running
      if(encSTATE!=0){
        /*Serial.print("In ESC: ");
        Serial.print(ESC_PWM,DEC);
        Serial.print(" in encSTATE: ");
        Serial.print(encSTATE,DEC);*/                           
        ESC_PWM = ESC_PWM + 1*encSTATE;
        encSTATE = 0;
        /*Serial.print(" out ESC: ");
        Serial.print(ESC_PWM,DEC);
        Serial.print(" out endDIR: ");
        Serial.println(encSTATE,DEC); */
        //range limiting
        if(ESC_PWM <= 20){
          ESC_PWM = 20;
        }
        else if(ESC_PWM >= 255){
          ESC_PWM = 255;
        }
        Serial.print("Write ESC: ");
        Serial.println(ESC_PWM,DEC);
        myservo.write(ESC_PWM);
      }  
    }

  sensorvalue = analogRead(A0);                  // read from analog pin for rpm sens
  if(sensorvalue < sensorthreshold)
    state1 = HIGH;
   else
    state1 = LOW;                           
   if(state2!=state1){                           //counts when the state change, thats from (dark to light) or 
                                                 //from (light to dark)
     if (state2>state1){                         // only every second change
       currentTime = micros();                   // Get the arduino time in microseconds
       diffTime = currentTime - prevTime;        // calculate the time diff from the last meet-up
       RPM = 60000000/diffTime;                  // calculate how many rev per minute
       prevTime = currentTime;
     }
     state2 = state1;
   }
   
  ReadEnter();
  //SetESC();  
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Setting up RPM

void SetSPEED()
{
  ENTER = HIGH;
  ui.ThisFrame(3);
  
   while(ENTER == HIGH)                                         //runs till confirm with enter.
    {
    if(ESC_PWM >= 20 && ESC_PWM <= 255){                           
        ESC_PWM = ESC_PWM + 1*encSTATE;
        //Range limiting
        if(ESC_PWM <= 20)
        {
          ESC_PWM = 20;
        }
        if(ESC_PWM >= 255)
        {
          ESC_PWM = 255;
        }
        encSTATE = 0;  
      }

      int remainingTimeBudget = ui.update();
      if (remainingTimeBudget > 0) {
      delay(remainingTimeBudget);  
      } 
      ReadEnter();                        
    }
    
    ENTER = HIGH;
    ui.ThisFrame(1);
    
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Setting up Time

void SetTIME()
{
  ui.ThisFrame(2);
  
  ENTER = HIGH;
  while(ENTER == HIGH)                                        //set minutes
    {
      MIN = MIN + encSTATE;
      encSTATE = 0;
      if(MIN <= 0)MIN = 0;
      int remainingTimeBudget = ui.update();
      if (remainingTimeBudget > 0) {
      delay(remainingTimeBudget);
      }
      ReadEnter();
    }
 
  ENTER = HIGH;
  while(ENTER == HIGH)                                        //set seconds
   {
      SEC = SEC + encSTATE;
      encSTATE = 0;
      if(SEC < 0 && MIN > 0){
        SEC = 59;
        MIN = MIN - 1;
      }
      if(SEC > 59){
        SEC = 0;
        MIN = MIN + 1; 
      }
      int remainingTimeBudget = ui.update();
      if (remainingTimeBudget > 0) {
      delay(remainingTimeBudget);
      }
      ReadEnter();   
   }              
}
////////////////////////////////////////////////////////////////////////////////////////////////////////
//encoder click debounce
void ReadEnter()
{
  
  bool Enter = digitalRead(ENTERpin);                      // Enter debounce
  if(Enter == LOW){
    unsigned long currentmillis = millis();
    if (currentmillis - previusENTER >= 500)
    {
     ENTER = Enter;
     previusENTER = currentmillis;
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Improvised avto speed correction, copares current rpm target rpm and scales PWM on esc accordingly
 
/* void SetESC()                                             
{
  unsigned long currentmillis = millis();
  if (currentmillis - previusPWM >= 500)
  {
  float diffRPM = RPM - TRPM;
  if(diffRPM > 200) ESC_PWM--;
  if(ESC_PWM >= 255) ESC_PWM = 255;
  if(diffRPM < -200) ESC_PWM++;
  if(ESC_PWM <= 20) ESC_PWM = 20;
  if(TRPM <= 3000) ESC_PWM = 20;
  if(diffRPM > 2000) ESC_PWM = 20;  // it current rpm is greatly bigger than target rpm set throttle to zero
  previusPWM = currentmillis;
  myservo.write(ESC_PWM);
  }
}*/ 

////////////////////////////////////////////////////////////////////////////////////////////////////////
// encoder pulse counter. to counter jumping values in case of missing a encoder pulse.
void ENC_COUNT()
{
  encCOUNT = encCOUNT + encDIR;
  if(encCOUNT <= -4)
  {
    encSTATE = -1;
    encCOUNT = 0;
  }
  else if(encCOUNT >= 4)
  {
    encSTATE = 1;
    encCOUNT = 0;
  }
  encDIR = 0;
}

