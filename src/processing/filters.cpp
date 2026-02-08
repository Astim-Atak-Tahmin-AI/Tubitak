#include "filters.h"

bool inRange(float x, const Range& r){
    return (x>= r.minv) && (x<= r.maxv);
}