#include <stdint.h>
#include <Arduino.h>
#include <Wire.h>
#include <time.h>
#include <ds1307.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#define LED     4
#define RELAY   1

void initTimerCounter1(void);

void setup()
{
  ds1307_init();

  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(RELAY, LOW);    
  digitalWrite(LED, LOW);
  
  initTimerCounter1();
}

void loop()
{
  // Wait for Interruption
}

void initTimerCounter1(void)
{
  TCCR1 = 0;                  //stop the timer
  TCNT1 = 0;                  //zero the timer
  GTCCR = _BV(PSR1);          //reset the prescaler
  OCR1A = 243;                //set the compare value
  OCR1C = 243;
  TIMSK = _BV(OCIE1A);        //interrupt on Compare Match A
  //start timer, ctc mode, prescaler clk/16384    
  TCCR1 = _BV(CTC1) | _BV(CS13) | _BV(CS12) | _BV(CS11) | _BV(CS10);
  sei();
}

ISR(TIMER1_COMPA_vect)
{
  time_t time = get_time();

  struct tm tm;
  gmtime_r(&time, &tm);
  
  if((tm.tm_hour > 7)  && (tm.tm_hour < 22) && ((tm.tm_min == 0) || (tm.tm_min == 30)) )
  {
    digitalWrite(RELAY, HIGH);
    digitalWrite(LED, HIGH);
  }
  else
  {
    digitalWrite(RELAY, LOW);
    digitalWrite(LED, LOW);
  }
}