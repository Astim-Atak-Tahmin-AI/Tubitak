#pragma once
#include <Arduino.h>


struct Range {
    float minv;
    float maxv;
};


bool inRange(float x, const Range&r);


class Ema {
public :
   explicit Ema(float alpha=0.2f) : _alpha(alpha), _init(false),_y(0)
{}
  float update(float x){
    if(!_init){_y=x;_init=true; return _y;}
    _y=_alpha*x+(1.0f-_alpha)*_y;
    return _y;
  }
  bool initialized() const {return _init;}
  float value() const {return _y;}
private:
   float _alpha;
   bool _init;
   float _y;
};