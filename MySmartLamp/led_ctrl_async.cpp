#include <Arduino.h>
#include "led_ctrl_async.h"

led_ctrl::led_ctrl( int p ){
  LED = p;
  _start_time = millis();
}


void led_ctrl::set_pattern( int a[] ){
  pattern = a;
  p_length = 0;
  while( a[ p_length ] != '\0' ){
    p_length++;

    if( _debug ){
      Serial.print("Data: ");
      Serial.print( pattern[ p_length ] );
      Serial.print(", index: ");
      Serial.print( p_length );
      Serial.println( );
    }
  }
}
void led_ctrl::start( ){
  _start_time = millis();
  p_index = 0;
}

void led_ctrl::blink(){
  unsigned long cur = millis() - _start_time;
  if( cur >= pattern[ p_index ] ){
    if( _debug ){
      Serial.print( "Delay: " + String( pattern[ p_index ] ) );
      Serial.print( ", Cur time: " + String( cur ) );
      Serial.println( ", Led: " + String( LED ) + " blinking" );
    }

    _start_time = millis();
    high();
    p_index = ( p_index + 1 ) % p_length;
    
  }else if( state == HIGH && cur > _delay ){
    low();
  }
}

void led_ctrl::high(){
  state = HIGH;
  digitalWrite( LED, state );
}

void led_ctrl::low(){
  state = LOW;
  digitalWrite( LED, state );
}

int led_ctrl::status(){
  return state;
}

void led_ctrl::set_delay( int d ){
  _delay = d;
}

void led_ctrl::opposite( ){
  unsigned long cur = millis() - _start_time;
  if( cur > _delay ){
    if( !state ) high();
    else         low( );
    _start_time = millis();
    if( _debug ){
      Serial.print("Time: ");
      Serial.print( cur );
      Serial.print("; delay: ");
      Serial.print( _delay );
      Serial.print("; State: ");
      Serial.println( state );
    }
  }

}
