#include "sliding_window.h"
#include <math.h>


SlidingWindow::SlidingWindow(size_t capacity)
:_cap(capacity),
_buf(capacity, NAN),
_head(0),
_count(0),
_sum(0.0),
_sumsq(0.0),
_minmaxDirty(true),
_cachedMin(NAN),
_cachedMax(NAN) {}


void SlidingWindow::reset(){
    std::fill(_buf.begin(),_buf.end(),NAN);
    _head=0;
    _count=0;
    _sum=0;
    _sumsq=0.0;
    markMinMaxDirty();
}

void SlidingWindow::markMinMaxDirty(){
    _minmaxDirty=true;
    _cachedMin=NAN;
    _cachedMax=NAN;
}

void SlidingWindow::push(float x){
    if(_count==_cap){
        float old=_buf[_head];
        if(!isnan(old)){
            _sum-=(double)old;
            _sumsq-=(double)old*(double)old;
        }
        else{
            _count++;
        }

        _buf[_head]=x;
        if(!isnan(x)){
            _sum+=(double)x;
            _sumsq+=(double)x* (double)x;
        }

        _head=(_head+1)%_cap;
        markMinMaxDirty();
    }

    
}
float SlidingWindow::mean() const {
        if(_count==0) return NAN;
        return (float)(_sum/(double)_count);
    }

float SlidingWindow::variance() const{
    if (_count<2) return NAN;

    double n=(double)_count;
    double ex2=_sumsq/n;
    double ex=_sum/n;
    double v=ex2-ex*ex;
    if(v<0)v=0;
    return (float)v;
}

float SlidingWindow::stddev() const {
    float v=variance();
    if (isnan(v)) return NAN;
    return sqrtf(v);
}

void SlidingWindow::recomputeMinMax() const {
    if(_count==0) {_cachedMin=NAN; _cachedMax=NAN; _minmaxDirty=false;return;}

    float mn=INFINITY, mx=-INFINITY;
    for(size_t i=0;i<_count;i++){
        float v=at(i);
        if(isnan(v)) continue;
        if(v<mn)mn=v;
        if(v>mx)mx=v;
    }

    _cachedMin=(mn==INFINITY)?NAN: mn;
    _cachedMax=(mx==-INFINITY)?NAN:mx;
    _minmaxDirty=false;
}

float SlidingWindow::min() const{
    if(_minmaxDirty) recomputeMinMax();
    return _cachedMin;
}

float SlidingWindow::max() const{
    if(_minmaxDirty) recomputeMinMax();
    return _cachedMax;
}


float SlidingWindow::last() const {
  if (_count == 0) return NAN;
  return at(_count - 1);
}



float SlidingWindow::at(size_t i) const {
  // 0=oldest, size-1=newest
  if (i >= _count) return NAN;
  // oldest index = head - count (mod cap)
  int oldest = (int)_head - (int)_count;
  while (oldest < 0) oldest += (int)_cap;
  size_t idx = (oldest + i) % _cap;
  return _buf[idx];
}
