#include "MyParseNumber.h"

long myParseInt()
// A modified function to Serial.parseInt
{
  long value = 0;
  char c = Serial.read();
  if(c<0) return 0;

  // check if we're negative, and if so, skip to the next character
  bool isNegative = (c=='-');
  if(isNegative) {c = Serial.read();}

  while( isdigit(c)) { 
    value = (value * 10) + (c - '0');
    c = Serial.read();
  }

  if(isNegative) value = -value;
  return value;
}
