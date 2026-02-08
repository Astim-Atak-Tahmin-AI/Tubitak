#pragma once
#include <Arduino.h>
#include <vector>

class SlidingWindow{
    public:
      explicit SlidingWindow(size_t capacity);

      void reset();

      void push(float x);

      bool full() const {return _count==_cap;}
      size_t size() const {return _count;}
      size_t capacity() const {return _cap;}

      float mean() const;
      float variance() const;
      float stddev() const;
      float min() const;
      float max() const;
      float last() const;


      float at(size_t i) const;


    private:
      size_t _cap;
      std::vector<float> _buf;
      size_t _head;
      size_t _count;

      double _sum;
      double _sumsq;

      mutable bool _minmaxDirty;
      mutable float _cachedMin;
      mutable float _cachedMax;

      void markMinMaxDirty();
      void recomputeMinMax() const;

};