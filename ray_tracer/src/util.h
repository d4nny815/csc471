#ifndef UTILS_H
#define UTILS_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <vector>

using std::make_shared;
using std::shared_ptr;

const float MY_INFINITY = std::numeric_limits<float>::infinity();
const float PI = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * PI / 180.0;
}

#endif /* util.h */
